#pragma once

#include <functional>
#include <numeric>
#include <optional>
#include <variant>
#include <array>

#include <cstring>

// to-do
#define GalAssert(condition) do{}while(0)

namespace Gal {
    using ColorType = unsigned int;

    constexpr auto RealTimeTaskCycleMs              = 50;
    constexpr auto QueueBufferMaxLength             = 1024;
    constexpr auto SurfaceCountMax                  = 6;//root + pages
    constexpr auto WordBufferLength                 = 16;
    constexpr ColorType DefaultColorMask            = 0xFF080408;

    /*
     * flag = enum1 | enum2
     * if no exist enumerates equal flag, it will lose all data(it maybe equal to the first enumerate but no guarantee)
     * so cast it's type to EnumType
     */
    template <typename Enum, typename EnumType = std::underlying_type_t<Enum>, typename = std::enable_if_t<std::is_enum_v<Enum>>, typename = std::enable_if_t<std::is_same_v<EnumType, std::underlying_type_t<Enum>>>>
    constexpr EnumType operator|(const Enum& lhs, const Enum& rhs)
    {
        return static_cast<EnumType>(static_cast<EnumType>(lhs) | static_cast<EnumType>(rhs));
    }

    /*
     * enum = flag1 | enum1
     * usage:
     *      if(flag1 | enum1 == ENUM::XXX){xxx;}
     *      switch(flag1 | enum1){case ENUM:XXX: xxx;}
     *
     * note: if no exist enumerates equal flag, it will lose all data(it maybe equal to the first enumerate but no guarantee)
     */
    template <typename Enum, typename EnumType, typename = std::enable_if_t<std::is_enum_v<Enum>>, typename = std::enable_if_t<std::is_same_v<EnumType, std::underlying_type_t<Enum>>>>
    constexpr Enum operator|(const EnumType& lhs, const Enum& rhs)
    {
        return static_cast<Enum>(lhs | static_cast<EnumType>(rhs));
    }

    /*
     * enum = enum1 | flag1
     * usage:
     *      if(enum1 | flag1 == ENUM::XXX){xxx;}
     *      switch(enum1 | flag1){case ENUM:XXX: xxx;}
     *
     * note: if no exist enumerates equal flag, it will lose all data(it maybe equal to the first enumerate but no guarantee)
     */
    template <typename Enum, typename EnumType, typename = std::enable_if_t<std::is_enum_v<Enum>>, typename = std::enable_if_t<std::is_same_v<EnumType, std::underlying_type_t<Enum>>>>
    constexpr Enum operator|(const Enum& lhs, const EnumType& rhs)
    {
        return static_cast<Enum>(static_cast<EnumType>(lhs) | rhs);
    }

    /*
     * flag |= enum
     * only flag |= enum operator is valid
     * use enum |= flag is unreasonable
     */
    template <typename Enum, typename EnumType, typename = std::enable_if_t<std::is_enum_v<Enum>>, typename = std::enable_if_t<std::is_same_v<EnumType, std::underlying_type_t<Enum>>>>
    constexpr EnumType& operator|=(EnumType& lhs, const Enum& rhs)
    {
        return (lhs = static_cast<EnumType>(lhs | static_cast<EnumType>(rhs)));
    }

    /*
     * flag = enum1 & enum2
     * if no exist enumerates equal flag, it will lose all data(it maybe equal to the first enumerate but no guarantee)
     * so cast it's type to EnumType
     */
    template <typename Enum, typename EnumType = std::underlying_type_t<Enum>, typename = std::enable_if_t<std::is_enum_v<Enum>>, typename = std::enable_if_t<std::is_same_v<EnumType, std::underlying_type_t<Enum>>>>
    constexpr EnumType operator&(const Enum& lhs, const Enum& rhs)
    {
        return static_cast<EnumType>(static_cast<EnumType>(lhs) & static_cast<EnumType>(rhs));
    }

    /*
     * enum = flag1 & enum1
     * usage:
     *      if(flag1 & enum1 == ENUM::XXX){xxx;}
     *      switch(flag1 & enum1){case ENUM:XXX: xxx;}
     *
     * note: if no exist enumerates equal flag, it will lose all data(it maybe equal to the first enumerate but no guarantee)
     */
    template <typename EnumType, typename Enum, typename = std::enable_if_t<std::is_enum_v<Enum>>, typename = std::enable_if_t<std::is_same_v<EnumType, std::underlying_type_t<Enum>>>>
    constexpr Enum operator&(const EnumType& lhs, const Enum& rhs)
    {
        return static_cast<Enum>(lhs & static_cast<EnumType>(rhs));
    }

    /*
     * enum = enum1 & flag1
     * usage:
     *      if(enum1 & flag1 == ENUM::XXX){xxx;}
     *      switch(enum1 & flag1){case ENUM:XXX: xxx;}
     *
     * note: if no exist enumerates equal flag, it will lose all data(it maybe equal to the first enumerate but no guarantee)
     */
    template <typename Enum, typename EnumType, typename = std::enable_if_t<std::is_enum_v<Enum>>, typename = std::enable_if_t<std::is_same_v<EnumType, std::underlying_type_t<Enum>>>>
    constexpr Enum operator&(const Enum& lhs, const EnumType& rhs)
    {
        return static_cast<Enum>(static_cast<EnumType>(lhs) & rhs);
    }

    /*
     * flag &= enum
     * only flag &= enum operator is valid
     * use enum &= flag is unreasonable
     */
    template <typename EnumType, typename Enum, typename = std::enable_if_t<std::is_enum_v<Enum>>, typename = std::enable_if_t<std::is_same_v<EnumType, std::underlying_type_t<Enum>>>>
    constexpr EnumType& operator&=(EnumType& lhs, const Enum& rhs)
    {
        return (lhs = static_cast<EnumType>(lhs & static_cast<EnumType>(rhs)));
    }

    namespace utility {
        template<typename T, typename... Args>
        constexpr decltype(auto) max(const T &a, const Args &...args) {
            if constexpr (sizeof...(args) == 0) {
                return a;
            } else {
                return a > max(args...) ? a : max(args...);
            }
        }

        template<typename T, typename... Args>
        constexpr decltype(auto) min(const T &a, const Args &...args) {
            if constexpr (sizeof...(args) == 0) {
                return a;
            } else {
                return a < min(args...) ? a : min(args...);
            }
        }

        template<typename T, typename = std::enable_if_t<std::numeric_limits<T>::is_integer>>
        constexpr decltype(auto) abs(const T& n)
        {
            auto bit_length = sizeof(T) * 8;
            return (n ^ (n >> (bit_length - 1))) - (n >> (bit_length - 1));
        }

        constexpr ColorType build_argb(ColorType alpha, ColorType red, ColorType green, ColorType blue) {
            return (alpha << 24) | (red << 16) | (green << 8) | (blue << 0);
        }

        constexpr ColorType build_rgb(ColorType red, ColorType green, ColorType blue) {
            return (red << 16) | (green << 8) | (blue << 0);
        }

        constexpr ColorType get_alpha_from_argb(ColorType argb) {
            return (argb >> 24) & 0xFF;
        }

        constexpr ColorType get_red_from_argb(ColorType argb) {
            return (argb >> 16) & 0xFF;
        }

        constexpr ColorType get_green_from_argb(ColorType argb) {
            return (argb >> 8) & 0xFF;
        }

        constexpr ColorType get_blue_from_argb(ColorType argb) {
            return (argb >> 0) & 0xFF;
        }

        constexpr ColorType rgb_convert_32_to_16(ColorType rgb) {
            return ((rgb & 0xFF) >> 3) | ((rgb & 0xFC00) >> 5) | ((rgb & 0xF80000) >> 8);
        }

        constexpr ColorType rgb_convert_16_to_32(ColorType rgb) {
            return (0xFF << 24) | ((rgb & 0x1F) << 3) | ((rgb & 0x7E0) << 5) | ((rgb & 0xF800) << 8);
        }

        int build_bitmap(const char* file_name, unsigned width, unsigned height, unsigned char* data);

        template<typename... Args>
        struct overloaded : Args... { using Args::operator()...; };
        template<typename... Args>
        overloaded(Args...) -> overloaded<Args...>;
    }

    namespace detail
    {
        class CoreRect
        {
        public:
            explicit CoreRect() : m_left(-1), m_top(-1), m_right(-1), m_bottom(-1){};

            CoreRect(int left, int top, int width, int height)
                : m_left(left), m_top(top), m_right(left + width - 1), m_bottom(top + height - 1){};

            void set_rect(int left, int top, int width, int height)
            {
                GalAssert(width > 0 && height > 0);
                m_left = left;
                m_top = top;
                m_right = left + width - 1;
                m_bottom = top + height - 1;
            }

            [[nodiscard]] bool is_in_rect(int x, int y) const
            {
                return x >= m_left && x <= m_right && y >= m_top && y <= m_bottom;
            }

            [[nodiscard]] int distance_of_left(int x) const
            {
                return x - m_left;
            }

            [[nodiscard]] int distance_of_top(int y) const
            {
                return y - m_top;
            }

            bool operator==(const CoreRect& rect) const
            {
                return (m_left == rect.m_left) && (m_top == rect.m_top) && (m_right == rect.m_right) && (m_bottom == rect.m_bottom);
            }

            [[nodiscard]] int get_width() const
            {
                return m_right - m_left + 1;
            }

            [[nodiscard]] int get_height() const
            {
                return m_bottom - m_top + 1;
            }

            int m_left;
            int m_top;
            int m_right;
            int m_bottom;
       };

        enum class COLOR_BYTE_TYPE
       {
           BITS_16 = 2,
           BITS_32 = 4
       };

       using B16ByteType = unsigned short;
       using B32ByteType = unsigned int;

       class CoreTheme
       {
       public:
           enum class FONT_TYPE
           {
               FONT_NULL = 0,
               FONT_DEFAULT,
               FONT_CUSTOM,
               FONT_MAX
           };
           enum class BITMAP_TYPE
           {
               BITMAP_CUSTOM = 0,
               BITMAP_MAX
           };
           enum class COLOR_TYPE
           {
               COLOR_WND_FONT = 0,
               COLOR_WND_NORMAL,
               COLOR_WND_PUSHED,
               COLOR_WND_FOCUS,
               COLOR_WND_BORDER,
               COLOR_CUSTOM,
               COLOR_MAX
           };

           struct bitmap_info
           {
               //support 16 bits only
               B16ByteType width;
               B16ByteType height;
               B16ByteType color_bits;
               const B16ByteType* pixel_color_array;
           };

           struct font_info
           {
               struct lattice
               {
                   unsigned int utf8_code;
                   unsigned char width;
                   const unsigned char* pixel_gray_array;
               };
               unsigned char height;
               unsigned int count;
               lattice* lattice_array;
           };

           static bool add_font(FONT_TYPE index, const font_info* font)
           {
               if (index >= FONT_TYPE::FONT_MAX)
               {
                   GalAssert(false);
                   return false;
               }
               s_font_map[static_cast<std::size_t>(index)] = font;
               return true;
           }

           static const font_info* get_font(FONT_TYPE index)
           {
               if(index >= FONT_TYPE::FONT_MAX)
               {
                   GalAssert(false);
                   return nullptr;
               }
               return s_font_map[static_cast<std::size_t>(index)];
           }

           static bool add_bitmap(BITMAP_TYPE index, const bitmap_info* bitmap)
           {
               if(index >= BITMAP_TYPE::BITMAP_MAX)
               {
                   GalAssert(false);
                   return false;
               }
               s_bitmap_map[static_cast<std::size_t>(index)] = bitmap;
               return true;
           }

           static const bitmap_info* get_bitmap(BITMAP_TYPE index)
           {
               if(index >= BITMAP_TYPE::BITMAP_MAX)
               {
                   GalAssert(false);
                   return nullptr;
               }
               return s_bitmap_map[static_cast<std::size_t>(index)];
           }

           static bool add_color(COLOR_TYPE index, const ColorType color)
           {
               if(index >= COLOR_TYPE::COLOR_MAX)
               {
                   GalAssert(false);
                   return false;
               }
               s_color_map[static_cast<std::size_t>(index)] = color;
               return true;
           }

           static ColorType get_color(COLOR_TYPE index)
           {
               if(index >= COLOR_TYPE::COLOR_MAX)
               {
                   GalAssert(false);
                   return static_cast<ColorType>(-1);
               }
               return s_color_map[static_cast<std::size_t>(index)];
           }

       private:
           static std::array<const font_info*, static_cast<std::size_t>(FONT_TYPE::FONT_MAX)> s_font_map;
           static std::array<const bitmap_info*, static_cast<std::size_t>(BITMAP_TYPE::BITMAP_MAX)> s_bitmap_map;
           static std::array<ColorType, static_cast<std::size_t>(COLOR_TYPE::COLOR_MAX)> s_color_map;
       };

       class CoreSurface {
           friend class CoreBitmap;
       public:
           enum class Z_ORDER_LEVEL {
               Z_ORDER_LEVEL_LOWEST,
               Z_ORDER_LEVEL_MIDDLE,
               Z_ORDER_LEVEL_HIGHEST,
               Z_ORDER_LEVEL_MAX
           };

           template<COLOR_BYTE_TYPE Bytes>
           using ColorByteType = std::conditional_t<Bytes == COLOR_BYTE_TYPE::BITS_16, B16ByteType, B32ByteType>;
           using B16ColorByteType = ColorByteType<COLOR_BYTE_TYPE::BITS_16>;
           using B32ColorByteType = ColorByteType<COLOR_BYTE_TYPE::BITS_32>;
           using FramebufferType = std::variant<B16ColorByteType*, B32ColorByteType*, std::nullptr_t>;

           struct external_gfx_operator {
               std::function<void(int, int, ColorType)> draw_pixel;
               std::function<void(int, int, int, int, ColorType)> fill_rect;
           };

           class CoreDisplay {
               friend class CoreSurface;
               friend class CoreSurfaceNoFramebuffer;

           public:
               //multiple surface or surface_no_fb
               CoreDisplay(
                       FramebufferType physical_framebuffer,
                       int display_width,
                       int display_height,
                       COLOR_BYTE_TYPE color_bytes,
                       int surface_width,
                       int surface_height,
                       int surface_count,
                       external_gfx_operator *gfx_operator = nullptr);

               //single custom surface
               CoreDisplay(
                       FramebufferType physical_framebuffer,
                       int display_width,
                       int display_height,
                       CoreSurface* surface);

               //for multiple surfaces
               CoreSurface* alloc_surface(
                       Z_ORDER_LEVEL max_z_order,
                       CoreRect layer_rect = CoreRect())
               {
                   GalAssert(max_z_order < Z_ORDER_LEVEL_MAX && m_surface_index < m_surface_count);
                   GalAssert(!std::holds_alternative<std::nullptr_t>(m_physical_framebuffer));

                   COLOR_BYTE_TYPE bytes = std::holds_alternative<B16ColorByteType*>(m_physical_framebuffer) ? COLOR_BYTE_TYPE::BITS_16 : COLOR_BYTE_TYPE::BITS_32;

                   (layer_rect == CoreRect())
                   ?
                   m_surface_group[m_surface_index]->set_surface(max_z_order, CoreRect(0, 0, m_width - 1, m_height - 1), bytes)
                   :
                   m_surface_group[m_surface_index]->set_surface(max_z_order, layer_rect, bytes);

                   return m_surface_group[m_surface_index++];
               }

               bool swipe_surface(CoreSurface* surface1, CoreSurface* surface2, int x0, int x1, int y0, int y1, int offset);

               [[nodiscard]] int get_width() const {
                   return m_width;
               }

               [[nodiscard]] int get_height() const {
                   return m_height;
               }

               FramebufferType get_updated_framebuffer(int *width, int *height, bool force_update = false) {
                   if (width && height) {
                       *width = get_width();
                       *height = get_height();
                   }
                   if (force_update) {
                       return m_physical_framebuffer;
                   }
                   if (m_physical_read_index == m_physical_write_index) {
                       return nullptr;//No update
                   }
                   m_physical_read_index = m_physical_write_index;
                   return m_physical_framebuffer;
               }

               int snap_shot(const char *file_name) {
                   if (std::holds_alternative<std::nullptr_t>(m_physical_framebuffer)) {
                       return -1;
                   }

                   auto width = get_width();
                   auto height = get_height();
                   if (std::holds_alternative<B16ColorByteType*>(m_physical_framebuffer)) {
                       //16 bits framebuffer
                       return utility::build_bitmap(file_name, width, height, reinterpret_cast<unsigned char *>(std::get<B16ColorByteType*>(m_physical_framebuffer)));
                   } else {
                       //32 bits framebuffer
                       auto p_bitmap565_data = new B16ColorByteType[width * height];
                       auto p_raw_data = std::get<B32ColorByteType*>(m_physical_framebuffer);
                       for (auto i = 0; i < width * height; ++i) {
                           p_bitmap565_data[i] = utility::rgb_convert_32_to_16(*p_raw_data++);
                       }
                       auto ret = utility::build_bitmap(file_name, width, height, reinterpret_cast<unsigned char *>(p_bitmap565_data));
                       delete[] p_bitmap565_data;
                       return ret;
                   }
               }

           private:
               int m_width;      //in pixels
               int m_height;     //in pixels
               FramebufferType m_physical_framebuffer;
               int m_physical_read_index;
               int m_physical_write_index;
               std::array<CoreSurface*, SurfaceCountMax> m_surface_group;
               int m_surface_count;
               int m_surface_index;
           };

           class CoreLayer
           {
           public:
               CoreLayer() : framebuffer(nullptr), rect(CoreRect()) {};
               FramebufferType framebuffer;
               CoreRect rect;//framebuffer area
           };

       public:
           CoreSurface(
                   int width,
                   int height,
                   COLOR_BYTE_TYPE color_bytes,
                   Z_ORDER_LEVEL max_z_order = Z_ORDER_LEVEL::Z_ORDER_LEVEL_LOWEST,
                   CoreRect overlapped_rect = CoreRect())
               : m_width(width),
                 m_height(height),
                 m_is_active(false),
                 m_max_z_order(max_z_order),
                 m_framebuffer(nullptr),
                 m_top_z_order(Z_ORDER_LEVEL::Z_ORDER_LEVEL_LOWEST),
                 m_physical_framebuffer(nullptr),
                 m_physical_write_index(nullptr),
                 m_display(nullptr) {
               (overlapped_rect == CoreRect())
                       ?
                       set_surface(max_z_order, CoreRect(0, 0, width - 1, height - 1), color_bytes)
                       :
                       set_surface(max_z_order, overlapped_rect, color_bytes);
           }

           [[nodiscard]] unsigned int get_width() const {
               return m_width;
           }

           [[nodiscard]] unsigned int get_height() const {
               return m_height;
           }

           ColorType get_pixel(int x, int y, Z_ORDER_LEVEL z_order) {
               if (x >= m_width || y >= m_height || x < 0 || y < 0 || z_order >= Z_ORDER_LEVEL::Z_ORDER_LEVEL_MAX) {
                   GalAssert(false);
                   return static_cast<ColorType>(-1);
               }

               auto _z_order = static_cast<size_t>(z_order);
               auto index = y * m_width + x;
               FramebufferType buffers[3] = {m_layers[_z_order].framebuffer, m_framebuffer, m_physical_framebuffer};

               for(auto& buffer : buffers)
               {
                   if (!std::holds_alternative<std::nullptr_t>(buffer)) {
                       return get_rgb_from_buffer(buffer, index);
                   }
               }

               return static_cast<ColorType>(-1);
           }

           virtual void draw_pixel(int x, int y, ColorType rgb, Z_ORDER_LEVEL z_order) {
               if (x >= m_width || y >= m_height || x < 0 || y < 0) {
                   return;
               }
               if (z_order > m_max_z_order) {
                   GalAssert(false);
                   return;
               }
               if (z_order == m_max_z_order) {
                   return draw_pixel_on_framebuffer(x, y, rgb);
               }

               if (z_order > m_top_z_order) {
                   m_top_z_order = z_order;
               }
               if (auto layer = m_layers[static_cast<size_t>(z_order)]; layer.rect.is_in_rect(x, y)) {
                   auto layer_rect = layer.rect;
                   auto buffer = layer.framebuffer;
                   auto index = layer_rect.distance_of_left(x) + layer_rect.distance_of_top(y) * layer_rect.get_width();

                   write_rgb_to_buffer(buffer, index, rgb);
               }

               if (z_order == m_top_z_order) {
                   return draw_pixel_on_framebuffer(x, y, rgb);
               }

               bool be_overlapped = false;
               for (auto tmp_z_order = static_cast<size_t>(Z_ORDER_LEVEL::Z_ORDER_LEVEL_MAX) - 1; tmp_z_order > static_cast<size_t>(z_order); --tmp_z_order) {
                   if (m_layers[tmp_z_order].rect.is_in_rect(x, y)) {
                       be_overlapped = true;
                       break;
                   }
               }
               if (!be_overlapped) {
                   draw_pixel_on_framebuffer(x, y, rgb);
               }
           }

           virtual void fill_rect(int x0, int y0, int x1, int y1, ColorType rgb, Z_ORDER_LEVEL z_order) {
               x0 = utility::max(x0, 0);
               y0 = utility::max(y0, 0);
               x1 = utility::min(x1, m_width - 1);
               y1 = utility::min(y1, (m_height - 1));

               if (z_order == m_max_z_order) {
                   return fill_rect_on_framebuffer(x0, y0, x1, y1, rgb);
               }
               if (z_order == m_top_z_order) {
                   auto layer = m_layers[static_cast<size_t>(z_order)];
                   auto layer_rect = layer.rect;
                   auto buffer = layer.framebuffer;
                   for (auto y = y0; y <= y1; ++y) {
                       for (auto x = x0; x <= x1; ++x) {
                           if (layer_rect.is_in_rect(x, y)) {
                               auto index = layer_rect.distance_of_left(x) + layer_rect.distance_of_top(y) * layer_rect.get_width();
                               write_rgb_to_buffer(buffer, index, rgb);
                           }
                       }
                   }
                   return fill_rect_on_framebuffer(x0, y0, x1, y1, rgb);
               }
               for (; y0 <= y1; ++y0) {
                   draw_horizontal_line(x0, x1, y0, rgb, z_order);
               }
           }

           void draw_horizontal_line(int x0, int x1, int y, ColorType rgb, Z_ORDER_LEVEL z_order) {
               for (; x0 <= x1; ++x0) {
                   draw_pixel(x0, y, rgb, z_order);
               }
           }

           void draw_vertical_line(int x, int y0, int y1, ColorType rgb, Z_ORDER_LEVEL z_order) {
               for (; y0 <= y1; ++y0) {
                   draw_pixel(x, y0, rgb, z_order);
               }
           }

           void draw_line(int x0, int y0, int x1, int y1, ColorType rgb, Z_ORDER_LEVEL z_order) {
               auto dx = utility::abs(x1 - x0);
               auto dy = utility::abs(y1 - y0);
               int x;
               int y;
               if (((dx > dy) && (x0 > x1)) || ((dx <= dy) && (y0 > y1))) {
                   x = x1;
                   y = y1;
                   x1 = x0;
                   y1 = y0;
                   x0 = x;
                   y0 = y;
               }
               x = x0;
               y = y0;
               if (dx > dy) {
                   auto e = dy - dx / 2;
                   for (; x0 <= x1; ++x0, e += dy) {
                       draw_pixel(x0, y0, rgb, z_order);
                       if (e > 0) {
                           e -= dx;
                           (y > y1) ? --y0 : ++y0;
                       }
                   }
               } else {
                   auto e = dx - dy / 2;
                   for (; y0 <= y1; ++y0, e += dx) {
                       draw_pixel(x0, y0, rgb, z_order);
                       if (e > 0) {
                           e -= dy;
                           (x > x1) ? --x0 : ++x0;
                       }
                   }
               }
           }

           void draw_rect(int x0, int y0, int x1, int y1, ColorType rgb, Z_ORDER_LEVEL z_order, unsigned int scale = 1) {
               for (auto offset = 0; offset < scale; ++offset) {
                   draw_horizontal_line(x0 + offset, x1 - offset, y0 + offset, rgb, z_order);
                   draw_horizontal_line(x0 + offset, x1 - offset, y1 - offset, rgb, z_order);
                   draw_vertical_line(x0 + offset, y0 + offset, y1 - offset, rgb, z_order);
                   draw_vertical_line(x1 - offset, y0 + offset, y1 - offset, rgb, z_order);
               }
           }

           void draw_rect(CoreRect rect, ColorType rgb, Z_ORDER_LEVEL z_order, unsigned int scale) {
               draw_rect(rect.m_left, rect.m_top, rect.m_right, rect.m_bottom, rgb, z_order, scale);
           }

           void fill_rect(CoreRect rect, ColorType rgb, Z_ORDER_LEVEL z_order) {
               fill_rect(rect.m_left, rect.m_top, rect.m_right, rect.m_bottom, rgb, z_order);
           }

           bool flush_screen(int left, int top, int right, int bottom)
           {
               if(left < 0 || left >= m_width || right < 0 || right >= m_width
                   || top < 0 || top >= m_height || bottom < 0 || bottom >= m_height)
               {
                   GalAssert(false);
               }
               if(!m_is_active || std::holds_alternative<std::nullptr_t>(m_physical_framebuffer) || std::holds_alternative<std::nullptr_t>(m_framebuffer))
               {
                   return false;
               }

               auto display_width = m_display->get_width();
               auto display_height = m_display->get_height();
               left = (left >= display_width) ? (display_width - 1) : left;
               right = (right >= display_width) ? (display_width - 1) : right;
               top = (top >= display_height) ? (display_height - 1) : top;
               bottom = (bottom >= display_height) ? (display_height - 1) : bottom;

               char* source = nullptr;
               char* dest = nullptr;
               using UNDERLYING_BYTE_TYPE = std::underlying_type_t<COLOR_BYTE_TYPE>;
               UNDERLYING_BYTE_TYPE bytes;

               if(std::holds_alternative<B16ColorByteType*>(m_framebuffer))
               {
                   bytes = static_cast<UNDERLYING_BYTE_TYPE>(COLOR_BYTE_TYPE::BITS_16);
                   source = reinterpret_cast<char*>(std::get<B16ColorByteType*>(m_framebuffer));
                   dest = reinterpret_cast<char*>(std::get<B16ColorByteType*>(m_physical_framebuffer));
               }
               else
               {
                   bytes = static_cast<UNDERLYING_BYTE_TYPE>(COLOR_BYTE_TYPE::BITS_32);
                   source = reinterpret_cast<char*>(std::get<B32ColorByteType*>(m_framebuffer));
                   dest = reinterpret_cast<char*>(std::get<B32ColorByteType*>(m_physical_framebuffer));
               }

               for(auto y = top; y < bottom; ++y)
               {
                   source += (y * m_width + left) * bytes;
                   dest += (y * display_width + left) * bytes;

                   std::memcpy(dest, source, (right - left) * bytes);
               }
               ++*m_physical_write_index;
               return true;
           }

           [[nodiscard]] bool is_active() const
           {
               return m_is_active;
           }

           void set_active(bool flag)
           {
               m_is_active = flag;
           }

           CoreDisplay* get_display()
           {
               return m_display;
           }

           void show_layer(CoreRect& rect, CoreSurface::Z_ORDER_LEVEL z_order)
           {
               GalAssert(z_order >= Z_ORDER_LEVEL_LOWEST && z_order < Z_ORDER_LEVEL_MAX);
               auto layer = m_layers[static_cast<size_t>(z_order)];
               auto layer_rect = layer.rect;
               GalAssert(rect.m_left >= layer_rect.m_left && rect.m_right <= layer_rect.m_right &&
                         rect.m_top >= layer_rect.m_top && rect.m_bottom <= layer_rect.m_bottom);
               auto buffer = layer.framebuffer;
               auto width = layer_rect.get_width();
               for(auto y = rect.m_top; y <= rect.m_bottom; ++y)
               {
                   for(auto x = rect.m_left; x <= rect.m_right; ++x)
                   {
                       auto index = layer_rect.distance_of_left(x) + layer_rect.distance_of_top(y) * width;
                       draw_pixel_on_framebuffer(x, y, get_rgb_from_buffer(buffer, index));
                   }
               }
           }

       protected:
           virtual void draw_pixel_on_framebuffer(int x, int y, ColorType rgb)
           {
               if(!std::holds_alternative<std::nullptr_t>(m_framebuffer))
               {
                   write_rgb_to_buffer(m_framebuffer, y * m_width + x, rgb);
               }
               if(m_is_active && (x < m_display->get_width()) && (y < m_display->get_height()))
               {
                   write_rgb_to_buffer(m_physical_framebuffer, y * m_display->get_width() + x, rgb);
               }
               ++*m_physical_write_index;
           }
           virtual void fill_rect_on_framebuffer(int x0, int y0, int x1, int y1, ColorType rgb)
           {
               auto display_width = m_display->get_width();
               auto display_height = m_display->get_height();

               void* framebuffer = nullptr;
               void* physical_framebuffer = nullptr;
               COLOR_BYTE_TYPE bytes;
               bool is_framebuffer_null = false;

               if(std::holds_alternative<std::nullptr_t>(m_framebuffer))
               {
                   is_framebuffer_null = true;
               }

               if(std::holds_alternative<B16ColorByteType*>(m_physical_framebuffer))
               {
                   bytes = COLOR_BYTE_TYPE::BITS_16;
                   if(!is_framebuffer_null)
                   {
                       framebuffer = std::get<B16ColorByteType*>(m_framebuffer);
                   }
                   physical_framebuffer = std::get<B16ColorByteType*>(m_physical_framebuffer);
                   rgb = utility::rgb_convert_32_to_16(rgb);
               }
               else
               {
                   bytes = COLOR_BYTE_TYPE::BITS_32;
                   if(!is_framebuffer_null)
                   {
                       framebuffer = std::get<B32ColorByteType*>(m_framebuffer);
                   }
                   physical_framebuffer = std::get<B32ColorByteType*>(m_physical_framebuffer);
               }

               for(;y0 <= y1; ++y0)
               {
                   auto framebuffer_index = y0 * m_width + x0;
                   auto physical_framebuffer_index = y0 * display_width + x0;

                   if(bytes == COLOR_BYTE_TYPE::BITS_16)
                   {
                       framebuffer = static_cast<B16ColorByteType*>(framebuffer) + framebuffer_index;
                       physical_framebuffer = static_cast<B16ColorByteType*>(physical_framebuffer) + physical_framebuffer_index;
                   }
                   else
                   {
                       framebuffer = static_cast<B32ColorByteType*>(framebuffer) + framebuffer_index;
                       physical_framebuffer = static_cast<B32ColorByteType*>(physical_framebuffer) + physical_framebuffer_index;
                   }

                   ++*m_physical_write_index;

                   for(auto x = x0, offset = 0; x <= x1; ++x, ++offset)
                   {
                       if(bytes == COLOR_BYTE_TYPE::BITS_16)
                       {
                           if(!is_framebuffer_null)
                           {
                               *(static_cast<B16ColorByteType*>(framebuffer) + offset) = rgb;
                           }
                           if(m_is_active && (x < display_width) && (y0 < display_height))
                           {
                               *(static_cast<B16ColorByteType*>(physical_framebuffer) + offset) = rgb;
                           }
                       }
                       else
                       {
                           if(!is_framebuffer_null)
                           {
                               *(static_cast<B32ColorByteType*>(framebuffer) + offset) = rgb;
                           }
                           if(m_is_active && (x < display_width) && (y0 < display_height))
                           {
                               *(static_cast<B32ColorByteType*>(physical_framebuffer) + offset) = rgb;
                           }
                       }
                   }
               }
           }

           void attach_display(CoreDisplay* display)
           {
               GalAssert(display);

               m_display = display;
               m_physical_framebuffer = display->m_physical_framebuffer;
               m_physical_write_index = &display->m_physical_write_index;
           }

           void set_surface(Z_ORDER_LEVEL max_z_order, CoreRect layer_rect, COLOR_BYTE_TYPE bytes)
           {
               m_max_z_order = max_z_order;

               auto malloc_buffer = [&](FramebufferType buffer) -> void*
               {
                   if(bytes == COLOR_BYTE_TYPE::BITS_16)
                   {
                       return (std::get<B16ColorByteType*>(buffer) = static_cast<B16ColorByteType*>(std::calloc(m_width * m_height, static_cast<std::size_t>(bytes))));
                   }
                   else if(bytes == COLOR_BYTE_TYPE::BITS_32)
                   {
                       return (std::get<B32ColorByteType*>(buffer) = static_cast<B32ColorByteType*>(std::calloc(m_width * m_height, static_cast<std::size_t>(bytes))));
                   }

                   return nullptr;
               };

               if(m_display && (m_display->m_surface_count > 1))
               {
                   malloc_buffer(m_framebuffer);
               }
               for(auto i = static_cast<size_t>(Z_ORDER_LEVEL::Z_ORDER_LEVEL_LOWEST); i < static_cast<size_t>(Z_ORDER_LEVEL::Z_ORDER_LEVEL_MAX); ++i)
               {
                   // top layer framebuffer always be o
                   auto p = malloc_buffer(m_layers[i].framebuffer);
                   GalAssert(p != nullptr);
                   m_layers[i].rect = layer_rect;
               }
           }

           unsigned int m_width;
           unsigned int m_height;
           FramebufferType m_framebuffer;
           std::array<CoreLayer, static_cast<size_t>(Z_ORDER_LEVEL::Z_ORDER_LEVEL_MAX)> m_layers;
           bool m_is_active;
           Z_ORDER_LEVEL m_max_z_order;
           Z_ORDER_LEVEL m_top_z_order;
           FramebufferType m_physical_framebuffer;
           int* m_physical_write_index;
           CoreDisplay* m_display;

       public:
           static void write_rgb_to_buffer(FramebufferType& buffer, unsigned int index, ColorType rgb)
           {
               std::visit(utility::overloaded{
                                  [&](B16ColorByteType* data){data[index] = utility::rgb_convert_32_to_16(rgb);},
                                  [&](B32ColorByteType* data){data[index] = rgb;},
                                  [&](std::nullptr_t data){GalAssert(false);}
                          }, buffer);
           }

           [[nodiscard]] static ColorType get_rgb_from_buffer(const FramebufferType& buffer, unsigned int index)
           {
               auto ret = static_cast<ColorType>(-1);
               std::visit(utility::overloaded{
                       [&](const B16ColorByteType* data){ret = utility::rgb_convert_16_to_32(data[index]);},
                       [&](const B32ColorByteType* data){ret =  data[index];},
                       [&](std::nullptr_t data){GalAssert(false);}
               }, buffer);

               return ret;
           }
       };

       class CoreSurfaceNoFramebuffer : public CoreSurface
       {
           //No physical framebuffer, render with external graphic interface
       public:
           CoreSurfaceNoFramebuffer(
                   int width,
                   int height,
                   COLOR_BYTE_TYPE color_bytes,
                   CoreSurface::external_gfx_operator* gfx_operator,
                   CoreSurface::Z_ORDER_LEVEL max_z_order = CoreSurface::Z_ORDER_LEVEL::Z_ORDER_LEVEL_LOWEST,
                   CoreRect overlapped_rect = CoreRect())
                   : CoreSurface(width, height, color_bytes, max_z_order, overlapped_rect), m_gfx_operator(gfx_operator) {};

           [[nodiscard]] CoreSurface::external_gfx_operator* get_gfx_operator() const
           {
               return m_gfx_operator;
           }

       protected:
           void draw_pixel_on_framebuffer(int x, int y, ColorType rgb) override
           {
               if(m_gfx_operator && m_gfx_operator->draw_pixel && m_is_active)
               {
                   m_gfx_operator->draw_pixel(x, y, rgb);
               }
               if(std::holds_alternative<std::nullptr_t>(m_framebuffer)){return;}
               write_rgb_to_buffer(m_framebuffer, y * m_width + x, rgb);
           }

           void fill_rect_on_framebuffer(int x0, int y0, int x1, int y1, ColorType rgb) override
           {
               if(!m_gfx_operator){return;}

               if(auto& filler = m_gfx_operator->fill_rect; filler)
               {
                   return m_gfx_operator->fill_rect(x0, y0, x1, y1, rgb);
               }
               if(auto& drawer = m_gfx_operator->draw_pixel; drawer && m_is_active)
               {
                   for(auto y = y0; y <= y1; ++y)
                   {
                       for(auto x = x0; x <= x1; ++x)
                       {
                           drawer(x, y, rgb);
                       }
                   }
               }
               if(std::holds_alternative<std::nullptr_t>(m_framebuffer)){return;}

               void* framebuffer = nullptr;
               COLOR_BYTE_TYPE bytes;

               if(std::holds_alternative<B16ColorByteType*>(m_framebuffer))
               {
                   bytes = COLOR_BYTE_TYPE::BITS_16;
                   framebuffer = std::get<B16ColorByteType*>(m_framebuffer);
                   rgb = utility::rgb_convert_32_to_16(rgb);
               }
               else
               {
                   bytes = COLOR_BYTE_TYPE::BITS_32;
                   framebuffer = std::get<B32ColorByteType*>(m_framebuffer);
               }

               for(auto y = y0; y <= y1; ++y)
               {
                   auto index = y0 * m_width + x0;
                   if(bytes == COLOR_BYTE_TYPE::BITS_16)
                   {
                       framebuffer = static_cast<B16ColorByteType*>(framebuffer) + index;
                   }
                   else
                   {
                       framebuffer = static_cast<B32ColorByteType*>(framebuffer) + index;
                   }

                   for(auto x = x0, offset = 0; x <= x1; ++x, ++offset)
                   {
                       if(bytes == COLOR_BYTE_TYPE::BITS_16)
                       {
                           *(static_cast<B16ColorByteType*>(framebuffer) + offset) = rgb;
                       }
                       else
                       {
                           *(static_cast<B32ColorByteType*>(framebuffer) + offset) = rgb;
                       }
                   }
               }
           }

           //Rendering by external method
           CoreSurface::external_gfx_operator* m_gfx_operator;
       };

       CoreSurface::CoreDisplay::CoreDisplay(
               FramebufferType physical_framebuffer,
               int display_width,
               int display_height,
               COLOR_BYTE_TYPE color_bytes,
               int surface_width,
               int surface_height,
               int surface_count,
               external_gfx_operator *gfx_operator)
               :
               m_width(display_width),
               m_height(display_height),
               m_physical_framebuffer(physical_framebuffer),
               m_physical_read_index(0),
               m_physical_write_index(0),
               m_surface_count(surface_count),
               m_surface_index(0),
               m_surface_group({nullptr})
       {
           GalAssert(color_bytes == BITS_16 || color_bytes == BITS_32);
           GalAssert(m_surface_count <= SurfaceCountMax);

           for(auto i = 0; i < m_surface_count; ++i)
           {
               m_surface_group[i] = (std::holds_alternative<std::nullptr_t>(physical_framebuffer))
                       ?
                       new CoreSurface(surface_width, surface_height, color_bytes)
                       :
                       new CoreSurfaceNoFramebuffer(surface_width, surface_height, color_bytes, gfx_operator);

               m_surface_group[i]->attach_display(this);
           }
       }

       CoreSurface::CoreDisplay::CoreDisplay(
               FramebufferType physical_framebuffer,
               int display_width,
               int display_height,
               CoreSurface *surface)
               :
               m_width(display_width),
               m_height(display_height),
               m_physical_framebuffer(physical_framebuffer),
               m_physical_read_index(0),
               m_physical_write_index(0),
               m_surface_count(1),
               m_surface_index(0),
               m_surface_group({nullptr})
       {
           surface->m_is_active = true;
           m_surface_group[0] = surface;
           m_surface_group[0]->attach_display(this);
       }

       bool CoreSurface::CoreDisplay::swipe_surface(CoreSurface *surface1, CoreSurface *surface2, int x0, int x1, int y0, int y1, int offset)
       {
           auto surface_width = surface1->get_width();
           auto surface_height = surface1->get_height();
           if(
                   offset < 0 || offset > surface_width
                   || x0 < 0 || x0 >= surface_width
                   || x1 < 0 || x1 >= surface_width
                   || y0 < 0 || y0 >= surface_height
                   || y1 < 0 || y1 >= surface_height
                   )
           {
               GalAssert(false);
               return false;
           }

           auto width = (x1 - x0 + 1);
           if(width < 0 || width > surface_width || width < offset)
           {
               GalAssert(false);
               return false;
           }

           GalAssert(!std::holds_alternative<std::nullptr_t>(surface1->m_framebuffer));
           GalAssert(!std::holds_alternative<std::nullptr_t>(surface2->m_framebuffer));
           using UNDERLYING_BYTE_TYPE = std::underlying_type_t<COLOR_BYTE_TYPE>;
           UNDERLYING_BYTE_TYPE bytes =
                   std::holds_alternative<B16ColorByteType*>(surface1->m_framebuffer)
                           ?
                           static_cast<UNDERLYING_BYTE_TYPE>(COLOR_BYTE_TYPE::BITS_16)
                           :
                           static_cast<UNDERLYING_BYTE_TYPE>(COLOR_BYTE_TYPE::BITS_32);

           x0 = (x0 >= m_width) ? (m_width - 1) : x0;
           x1 = (x1 >= m_width) ? (m_width - 1) : x1;
           y0 = (y0 >= m_height) ? (m_height - 1) : y0;
           y1 = (y1 >= m_height) ? (m_height - 1) : y1;
           if(!std::holds_alternative<std::nullptr_t>(m_physical_framebuffer))
           {
               char* left_source = nullptr;
               char* right_source = nullptr;
               char* dest = nullptr;

               if(std::holds_alternative<B16ColorByteType*>(m_physical_framebuffer))
               {
                   bytes = static_cast<UNDERLYING_BYTE_TYPE>(COLOR_BYTE_TYPE::BITS_16);
                   left_source = reinterpret_cast<char*>(std::get<B16ColorByteType*>(surface1->m_framebuffer));
                   right_source = reinterpret_cast<char*>(std::get<B16ColorByteType*>(surface2->m_framebuffer));
                   dest = reinterpret_cast<char*>(std::get<B16ColorByteType*>(m_physical_framebuffer));
               }
               else
               {
                   bytes = static_cast<UNDERLYING_BYTE_TYPE>(COLOR_BYTE_TYPE::BITS_32);
                   left_source = reinterpret_cast<char*>(std::get<B32ColorByteType*>(surface1->m_framebuffer));
                   right_source = reinterpret_cast<char*>(std::get<B32ColorByteType*>(surface2->m_framebuffer));
                   dest = reinterpret_cast<char*>(std::get<B32ColorByteType*>(m_physical_framebuffer));
               }

               for(int y = y0; y <= y1; ++y)
               {
                   //Left surface
                   auto left_source_index = (y * surface1->get_width() + x0 + offset) * bytes;
                   auto left_dest_index = (y * m_width + x0) * bytes;
                   //Right surface
                   auto right_source_index = (y * surface2->get_width() + x0) * bytes;
                   auto right_dest_index = (y * m_width + x0 + (width - offset)) * bytes;

                   std::memcpy(dest + left_dest_index, left_source + left_source_index, (width - offset) * bytes);
                   std::memcpy(dest + right_dest_index, right_source + right_source_index, offset * bytes);
               }
           }
           else
           {
               auto draw_pixel = dynamic_cast<CoreSurfaceNoFramebuffer*>(surface1)->get_gfx_operator()->draw_pixel;

               for(auto y = y0; y <= y1; ++y)
               {
                   //Left surface
                   for(auto x = x0; x <= (x1 - offset); ++x)
                   {
                       draw_pixel(x, y, get_rgb_from_buffer(surface1->m_framebuffer, y * m_width + x + offset));
                   }
                   //Right surface
                   for(auto x = x1 - offset; x <= x1; ++x)
                   {
                       draw_pixel(x, y, get_rgb_from_buffer(surface2->m_framebuffer, y * m_width + x + offset - x1 + x0));
                   }
               }
           }

           ++m_physical_write_index;
           return true;
       }

       class CoreWord
       {
       public:
           enum class ALIGN_TYPE
           {
               HORIZONTAL_CENTER                     = 0x00000000L,
               HORIZONTAL_LEFT                       = 0x01000000L,
               HORIZONTAL_RIGHT                      = 0x02000000L,
               HORIZONTAL_MASK                       = 0x03000000L,
               VERTICAL_CENTER                       = 0x00000000L,
               VERTICAL_TOP                          = 0x00100000L,
               VERTICAL_BOTTOM                       = 0x00200000L,
               VERTICAL_MASK                         = 0x00300000L
           };

           static void draw_string(
                   CoreSurface* surface,
                   CoreSurface::Z_ORDER_LEVEL z_order,
                   const char* str,
                   int x, int y,
                   const CoreTheme::font_info* font,
                   ColorType font_color,
                   ColorType background_color,
                   ALIGN_TYPE align_type = ALIGN_TYPE::HORIZONTAL_LEFT)
           {
               if(str == nullptr){return;}

               auto offset = 0;
               while(*str)
               {
                   unsigned int code;
                   str += get_utf8_code(str, code);
                   offset += draw_single_char(surface, z_order, code, x + offset, y, font, font_color, background_color);
               }
           }

           static void draw_string_in_rect(
                   CoreSurface* surface,
                   CoreSurface::Z_ORDER_LEVEL z_order,
                   const char* str,
                   CoreRect rect,
                   const CoreTheme::font_info* font,
                   ColorType font_color,
                   ColorType background_color,
                   ALIGN_TYPE align_type = ALIGN_TYPE::HORIZONTAL_LEFT)
           {
               if(str == nullptr){return;}

               int x;
               int y;
               get_string_position(str, font, rect, align_type, x, y);
               draw_string(surface, z_order, str, rect.m_left + x, rect.m_top + y, font, font_color, background_color, align_type);
           }

           static void draw_value(
                   CoreSurface* surface,
                   CoreSurface::Z_ORDER_LEVEL z_order,
                   int value,
                  int dot_position,
                   int x, int y,
                   const CoreTheme::font_info* font,
                   ColorType font_color,
                   ColorType background_color,
                   ALIGN_TYPE align_type = ALIGN_TYPE::HORIZONTAL_LEFT)
           {
               char buffer[WordBufferLength];
               value_to_string(value, dot_position, buffer, WordBufferLength);
               draw_string(surface, z_order, buffer, x, y, font, font_color, background_color, align_type);
           }

           static void draw_value_in_rect(
                   CoreSurface* surface,
                   CoreSurface::Z_ORDER_LEVEL z_order,
                   int value,
                   int dot_position,
                   CoreRect rect,
                   const CoreTheme::font_info* font,
                   ColorType font_color,
                   ColorType background_color,
                   ALIGN_TYPE align_type = ALIGN_TYPE::HORIZONTAL_LEFT)
           {
               char buffer[WordBufferLength];
               value_to_string(value, dot_position, buffer, WordBufferLength);
               draw_string_in_rect(surface, z_order, buffer, rect, font, font_color, background_color, align_type);
           }

           static void value_to_string(int value, int dot_position, char* buffer, int length)
           {
               memset(buffer, 0, length);
               switch (dot_position) {
                   case 0:
                       sprintf(buffer, "%d", value);
                       break;
                   case 1:
                       sprintf(buffer, "%.1f", value * 1.0 / 10);
                       break;
                   case 2:
                       sprintf(buffer, "%.2f", value * 1.0 / 100);
                       break;
                   case 3:
                       sprintf(buffer, "%.3f", value * 1.0 / 1000);
                       break;
                   default:
                       GalAssert(false);
                       break;
               }
           }

           static bool get_str_size(const char* str, const CoreTheme::font_info* font, int& width, int& height)
           {
               if(str == nullptr || font == nullptr)
               {
                   width = 0;
                   height = 0;
                   return false;
               }
               auto lattice_width = 0;
               while (*str)
               {
                   unsigned int code;
                   str += get_utf8_code(str, code);
                   auto lattice = get_lattice(font, code);
                   lattice_width += lattice ? lattice->width : font->height;
               }
               width = lattice_width;
               height = font->height;
               return true;
           }

       private:
           static int draw_single_char(
                   CoreSurface* surface,
                   CoreSurface::Z_ORDER_LEVEL z_order,
                   unsigned int utf8_code,
                   int x, int y,
                   const CoreTheme::font_info* font,
                   ColorType font_color,
                   ColorType background_color)
           {
               auto error_color = static_cast<ColorType>(-1);
               if(font)
               {
                   const auto lattice = get_lattice(font, utf8_code);
                   if(lattice)
                   {
                       draw_lattice(surface, z_order, x, y, lattice->width, font->height, lattice->pixel_gray_array, font_color, background_color);
                       return lattice->width;
                   }
               }
               else
               {
                   error_color = utility::build_rgb(255, 0, 0);
               }
               //lattice/font not found, draw "X"
               const int length = 16;
               for(auto _y = 0; _y < length; ++_y)
               {
                   for(auto _x = 0; _x < length; ++_x)
                   {
                       auto diff = (_x - _y);
                       int sum = (_x + _y);
                       if(utility::abs(diff) <= 1 || utility::abs(sum - length) <= 1)
                       {
                           surface->draw_pixel(x + _x, y + _y, error_color, z_order);
                       }
                       else
                       {
                           surface->draw_pixel(x+_x, y + _y, 0, z_order);
                       }
                   }
               }

               return length;
           }

           static void draw_lattice(
                   CoreSurface* surface,
                   CoreSurface::Z_ORDER_LEVEL z_order,
                   int x, int y,
                   int width, int height,
                   const unsigned char* data,
                   ColorType font_color,
                   ColorType background_color)
           {
               unsigned char block_value;
               unsigned char block_count;
               ColorType red;
               ColorType green;
               ColorType blue;
               ColorType rgb;

               auto load_new_block = [&]()
               {
                   block_value = *data++;
                   block_count = *data++;
                   red = (utility::get_red_from_argb(font_color) * block_value + utility::get_red_from_argb(background_color) * (255 - block_value)) >> 8;
                   green = (utility::get_green_from_argb(font_color) * block_value + utility::get_green_from_argb(background_color) * (255 - block_value)) >> 8;
                   blue = (utility::get_blue_from_argb(font_color) * block_value + utility::get_blue_from_argb(background_color) * (255 - block_value)) >> 8;
                   rgb = utility::build_rgb(red, green, blue);
               };

               load_new_block();
               for(auto _y = 0; _y < height; ++_y)
               {
                   for(auto _x = 0; _x < width; ++_x)
                   {
                       GalAssert(block_count);
                       if(block_value == 0x00)
                       {
                           if(utility::get_alpha_from_argb(background_color))
                           {
                               surface->draw_pixel(x + _x, y + _y, background_color, z_order);
                           }
                       }
                       else
                       {
                           surface->draw_pixel(x + _x, y + _y, rgb, z_order);
                       }
                       if(--block_count == 0)
                       {
                           //reload new block
                           load_new_block();
                       }
                   }
               }
           }

           static const CoreTheme::font_info::lattice* get_lattice(const CoreTheme::font_info* font, unsigned int utf8_code)
           {
               auto last = font->count - 1;
               auto first = static_cast<decltype(last)>(0);
               auto middle = static_cast<decltype(last)>(first + last) / 2;

               while (first <= last)
               {
                   auto code = font->lattice_array[middle].utf8_code;
                   if(code < utf8_code)
                   {
                       first = middle + 1;
                   }
                   else if(code == utf8_code)
                   {
                       return &font->lattice_array[middle];
                   }
                   else
                   {
                       last = middle - 1;
                   }
                   middle = (first + last) / 2;
               }

               return nullptr;
           }

           static void get_string_position(const char* str, const CoreTheme::font_info* font, CoreRect rect, ALIGN_TYPE align_type, int& x, int& y)
           {
               auto _align_type = static_cast<std::underlying_type_t<ALIGN_TYPE>>(align_type);

               int x_size;
               int y_size;
               get_str_size(str, font, x_size, y_size);
               auto width = rect.get_width();
               auto height = rect.get_height();
               x = 0;
               y = 0;

               switch (_align_type & ALIGN_TYPE::HORIZONTAL_MASK) {
                   case ALIGN_TYPE::HORIZONTAL_CENTER:
                       if(width > x_size)
                       {
                           x = (width - x_size) / 2;
                       }
                       break;
                   case ALIGN_TYPE::HORIZONTAL_LEFT:
                       x = 0;
                       break;
                   case ALIGN_TYPE::HORIZONTAL_RIGHT:
                       if(width > x_size)
                       {
                           x = width - x_size;
                       }
                       break;
                   default:
                       GalAssert(false);
                       break;
               }
               switch (_align_type & ALIGN_TYPE::VERTICAL_MASK) {
                   case ALIGN_TYPE::VERTICAL_CENTER:
                       if(height > y_size)
                       {
                           y = (height - y_size) / 2;
                       }
                       break;
                   case ALIGN_TYPE::VERTICAL_TOP:
                       y = 0;
                       break;
                   case ALIGN_TYPE::VERTICAL_BOTTOM:
                       if(height > y_size)
                       {
                           y = height - y_size;
                       }
                       break;
                   default:
                       GalAssert(false);
                       break;
               }
           }

           static int get_utf8_code(const char* str, unsigned int& output_utf8_code)
           {
               static std::array<unsigned char, 256> s_utf8_length_table =
                       {
                               1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                               1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                               1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                               1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                               1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                               1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                               1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                               1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                               1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                               1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                               1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                               1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                               2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
                               2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
                               3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
                               4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 6, 6, 1, 1
                       };
               auto p = reinterpret_cast<unsigned char*>(const_cast<char*>(str));
               auto utf8_bytes = s_utf8_length_table[*p];
               switch (utf8_bytes) {
                   case 1:
                       output_utf8_code = *p;
                       break;
                   case 2:
                       output_utf8_code = (*p << 8) | *(p + 1);
                       break;
                   case 3:
                       output_utf8_code = (*p << 16) | (*(p + 1) << 8) | *(p + 2);
                       break;
                   case 4:
                       output_utf8_code = (*p << 24) | (*(p + 1) << 16) | (*(p + 2) << 8) | *(p + 3);
                       break;
                   default:
                       GalAssert(false);
                       break;
               }
               return utf8_bytes;
           }
       };

       class CoreBitmap
       {
       public:
           static void draw_bitmap(
                   CoreSurface* surface,
                   CoreSurface::Z_ORDER_LEVEL z_order,
                   const CoreTheme::bitmap_info* bitmap,
                   int x, int y,
                   ColorType mask_rgb = DefaultColorMask)
           {
               GalAssert(bitmap);
               GalAssert(z_order >= CoreSurface::Z_ORDER_LEVEL::Z_ORDER_LEVEL_MIDDLE);

               auto layer = surface->m_layers[static_cast<size_t>(z_order) - 1];
               auto lower_framebuffer_rect = layer.rect;
               auto lower_framebuffer_width = lower_framebuffer_rect.get_width();

               auto mask_rgb_16 = utility::rgb_convert_32_to_16(mask_rgb);
               auto data = bitmap->pixel_color_array;

               for(auto _y = y, offset = 0; _y < y + bitmap->height; ++_y)
               {
                   for(auto _x = x; _x < x + bitmap->width; ++_x, ++offset)
                   {
                       auto rgb = *(data + offset);
                       if(mask_rgb_16 == rgb)
                       {
                           if(lower_framebuffer_rect.is_in_rect(_x, _y))
                           {
                               //show lower layer
                               auto index = lower_framebuffer_rect.distance_of_top(_y) * lower_framebuffer_width + lower_framebuffer_rect.distance_of_left(_x);
                               surface->draw_pixel(_x, _y, CoreSurface::get_rgb_from_buffer(layer.framebuffer, index), z_order);
                           }
                       }
                       else
                       {
                           surface->draw_pixel(_x, _y, utility::rgb_convert_16_to_32(rgb), z_order);
                       }
                   }
               }
           }

           static void draw_bitmap(
                   CoreSurface* surface,
                   CoreSurface::Z_ORDER_LEVEL z_order,
                   const CoreTheme::bitmap_info* bitmap,
                   int x, int y,
                   int source_x, int source_y,
                   int width, int height,
                   ColorType mask_rgb = DefaultColorMask)
           {
               if(bitmap == nullptr || (source_x + width > bitmap->width) || (source_y + height > bitmap->height)){return;}

               GalAssert(z_order >= CoreSurface::Z_ORDER_LEVEL::Z_ORDER_LEVEL_MIDDLE);

               auto layer = surface->m_layers[static_cast<size_t>(z_order) - 1];
               auto lower_framebuffer_rect = layer.rect;
               auto lower_framebuffer_width = lower_framebuffer_rect.get_width();

               auto mask_rgb_16 = utility::rgb_convert_32_to_16(mask_rgb);
               auto data = bitmap->pixel_color_array;

               for(auto _y = y; _y < y + bitmap->height; ++_y)
               {
                   auto p = &data[(source_y + _y) * bitmap->width + source_x];
                   for(auto _x = x, offset = 0; _x < x + bitmap->width; ++_x, ++offset)
                   {
                       auto rgb = *(p + offset);
                       if(mask_rgb_16 == rgb)
                       {
                           if(lower_framebuffer_rect.is_in_rect(_x, _y))
                           {
                               //show lower layer
                               auto index = (y + lower_framebuffer_rect.distance_of_top(_y)) * lower_framebuffer_width + (x + lower_framebuffer_rect.distance_of_left(_x));
                               surface->draw_pixel(_x, _y, CoreSurface::get_rgb_from_buffer(layer.framebuffer, index), z_order);
                           }
                       }
                       else
                       {
                           surface->draw_pixel(_x, _y, utility::rgb_convert_16_to_32(rgb), z_order);
                       }
                   }
               }
           }
       };

       class CoreWindow
       {
       public:
           enum class WINDOW_ATTRIBUTION
           {
               VISIBLE = 0x40000000L,
               FOCUS = 0x20000000L,
               PRIORITY = 0x10000000L// Handle touch action at high priority
           };

           enum class WINDOW_STATUS
           {
               NORMAL,
               PUSHED,
               FOCUSED,
               DISABLED
           };

           enum class NAVIGATION_KEY
           {
               FORWARD,
               BACKWARD,
               ENTER
           };

           enum class TOUCH_ACTION
           {
               DOWN,
               UP
           };

           enum class CONNECT_STATUS
           {
               INVALID_ID = -1,
               INVALID_SURFACE = -2,
               SUCCESS = 0
           };

           enum class UNLINK_STATUS
           {
               INVALID_TOP_CHILD = -2,
               INVALID_CHILD = -1,
               NOT_MY_CHILD = -1,
               DO_NOTHING = 0,
               SUCCESS = 1
           };

           using WindowCallback = std::function<void (int, int)>;
           using IdType = unsigned short;

           constexpr static IdType s_id_not_used = IdType();

           struct window_tree
           {
               CoreWindow* window;//window instance
               IdType resource_id;//ID
               const char* str;//caption
               short x;//position x
               short y;//position y
               short width;
               short height;
               window_tree* child_tree;//sub tree
           };

           CoreWindow()
               :
                 m_status(WINDOW_STATUS::NORMAL),
                 m_attribution(static_cast<WINDOW_ATTRIBUTION>(WINDOW_ATTRIBUTION::VISIBLE | WINDOW_ATTRIBUTION::FOCUS)),
                 m_parent(nullptr),
                 m_top_child(nullptr),
                 m_prev_sibling(nullptr),
                 m_next_sibling(nullptr),
                 m_str(nullptr),
                 m_font_type(nullptr),
                 m_font_color(0),
                 m_background_color(0),
                 m_id(s_id_not_used),
                 m_z_order(CoreSurface::Z_ORDER_LEVEL::Z_ORDER_LEVEL_LOWEST),
                 m_focus_child(nullptr),
                 m_surface(nullptr){};

           virtual ~CoreWindow()= default;;

           virtual CONNECT_STATUS connect(
                   CoreWindow* parent,
                   IdType resource_id,
                   const char* str,
                   short x, short y,
                   short width, short height,
                   window_tree* child_tree)
           {
               if(resource_id == s_id_not_used)
               {
                   GalAssert(false);
                   return CONNECT_STATUS::INVALID_ID;
               }
               m_id = resource_id;
               set_str(str);
               m_parent = parent;
               m_status = WINDOW_STATUS::NORMAL;
               if(parent)
               {
                   m_z_order = parent->m_z_order;
                   m_surface = parent->m_surface;
               }
               if(m_surface == nullptr)
               {
                   GalAssert(false);
                   return CONNECT_STATUS::INVALID_SURFACE;
               }
               /* (cs.x = x * 1024 / 768) for 1027*768=>800*600 quickly*/
               m_window_rect.set_rect(x, y, width, height);
               pre_create_window();
               if(parent != nullptr)
               {
                   parent->add_child_to_tail(this);
               }
               if(load_child_window(child_tree) >= 0)
               {
                   on_init_children();
               }
               return CONNECT_STATUS::SUCCESS;
           }
           void disconnect()
           {
               if(m_id == s_id_not_used){return;}

               if(m_top_child != nullptr)
               {
                   auto child = m_top_child;
                   while (child)
                   {
                       auto next = child->m_next_sibling;
                       child->disconnect();
                       child = next;
                   }
               }
               if(m_parent != nullptr)
               {
                   m_parent->unlink_child(this);
               }
               m_focus_child = nullptr;
               m_id = s_id_not_used;
           }

           virtual void on_init_children(){};
           virtual void on_paint(){};
           virtual void show_window()
           {
               if((m_attribution & WINDOW_ATTRIBUTION::VISIBLE) == static_cast<std::underlying_type_t<WINDOW_ATTRIBUTION>>(WINDOW_ATTRIBUTION::VISIBLE))
               {
                   on_paint();
                   auto child = m_top_child;
                   if(child != nullptr)
                   {
                       while (child)
                       {
                           child->show_window();
                           child = child->m_next_sibling;
                       }
                   }
               }
           }

           [[nodiscard]] IdType get_id() const {return m_id;}

           [[nodiscard]] CoreSurface::Z_ORDER_LEVEL get_z_order() const {return m_z_order;}

           [[nodiscard]] CoreWindow* get_window_by_id(IdType id) const
           {
               auto child = m_top_child;
               while(child)
               {
                   if(child->m_id == id)
                   {
                       break;
                   }
                   child = child->m_next_sibling;
               }
               return child;
           }

           [[nodiscard]] WINDOW_ATTRIBUTION get_attribution() const {return m_attribution;}

           void set_str(const char* str) {m_str = str;}

           void set_attribution(WINDOW_ATTRIBUTION attribution) {m_attribution = attribution;}

           [[nodiscard]] bool is_focus_window() const
           {
               return ((m_attribution & WINDOW_ATTRIBUTION::VISIBLE) && (m_attribution & WINDOW_ATTRIBUTION::FOCUS));
           }

           void set_font_color(ColorType color) {m_font_color = color;}

           [[nodiscard]] ColorType get_font_color() const {return m_font_color;}

           void set_background_color(ColorType color) {m_background_color = color;}

           [[nodiscard]] ColorType get_background_color() const {return m_background_color;}

           void set_font_type(const CoreTheme::font_info* font_type) {m_font_type = font_type;}

           [[nodiscard]] const CoreTheme::font_info* get_font_type() const {return m_font_type;}

           void set_window_position(short x, short y, short width, short height)
           {
               m_window_rect.set_rect(x, y, width, height);
           }

           [[nodiscard]] CoreRect get_window_rect() const {return m_window_rect;}

           [[nodiscard]] CoreRect get_screen_rect() const
           {
               auto [left, top] = window_to_screen();
               return {left, top, m_window_rect.get_width(), m_window_rect.get_height()};
           }

           CoreWindow* set_child_focus(CoreWindow* focus_child)
           {
               GalAssert(focus_child != nullptr);
               GalAssert(focus_child->m_parent == this);

               auto old_focus_child = m_focus_child;
               if(focus_child->is_focus_window())
               {
                   if(focus_child != old_focus_child)
                   {
                       if(old_focus_child)
                       {
                           old_focus_child->on_kill_focus();
                       }
                       m_focus_child = focus_child;
                       m_focus_child->on_focus();
                   }
               }
               return m_focus_child;
           }

           [[nodiscard]] CoreWindow* get_parent() const {return m_parent;}

           [[nodiscard]] CoreWindow* get_last_child() const
           {
               if(m_top_child == nullptr){return nullptr;}

               auto child = m_top_child;
               while(child->m_next_sibling)
               {
                   child = child->m_next_sibling;
               }

               return child;
           }

           UNLINK_STATUS unlink_child(CoreWindow* child)
           {
               if(child == nullptr) {return UNLINK_STATUS::INVALID_CHILD;}
               if(this != child->m_parent) {return UNLINK_STATUS::NOT_MY_CHILD;}
               if(m_top_child == nullptr) {return UNLINK_STATUS::INVALID_TOP_CHILD;}

               bool found = false;
               if(child == m_top_child)
               {
                   m_top_child = child->m_next_sibling;
                   if(child->m_next_sibling != nullptr)
                   {
                       child->m_next_sibling->m_prev_sibling = nullptr;
                   }
                   found = true;
               }
               else
               {
                   auto tmp_child = m_top_child;
                   while(tmp_child->m_next_sibling)
                   {
                       if(child == tmp_child->m_next_sibling)
                       {
                           tmp_child->m_next_sibling = child->m_next_sibling;
                           if(child->m_next_sibling != nullptr)
                           {
                               child->m_next_sibling->m_prev_sibling = tmp_child;
                           }
                           found = true;
                           break;
                       }
                       tmp_child = tmp_child->m_next_sibling;
                   }
               }
               if(found)
               {
                   if(m_focus_child == child)
                   {
                       m_focus_child = nullptr;
                   }
                   child->m_next_sibling = nullptr;
                   child->m_prev_sibling = nullptr;
                   return UNLINK_STATUS::SUCCESS;
               }
               return UNLINK_STATUS::DO_NOTHING;
           }

           [[nodiscard]] CoreWindow* get_prev_sibling() const {return m_prev_sibling;}

           [[nodiscard]] CoreWindow* get_next_sibling() const {return m_next_sibling;}

           virtual void on_touch(int x, int y, TOUCH_ACTION action)
           {
               x = m_window_rect.distance_of_left(x);
               y = m_window_rect.distance_of_top(y);
               CoreWindow* priority_window = nullptr;
               CoreWindow* tmp_child = m_top_child;

               while (tmp_child)
               {
                   if(auto attr = tmp_child->m_attribution; (attr & WINDOW_ATTRIBUTION::PRIORITY) && (attr & WINDOW_ATTRIBUTION::VISIBLE))
                   {
                       priority_window = tmp_child;
                       break;
                   }
                   tmp_child = tmp_child->m_next_sibling;
               }
               if(priority_window)
               {
                   return priority_window->on_touch(x, y, action);
               }
               auto child = m_top_child;
               while (child)
               {
                   if(child->is_focus_window())
                   {
                       auto rect = child->get_window_rect();
                       if(rect.is_in_rect(x, y))
                       {
                           return child->on_touch(x, y, action);
                       }
                   }
                   child = child->m_next_sibling;
               }
           }

           virtual void on_navigate(NAVIGATION_KEY key)
           {
               CoreWindow* priority_window = nullptr;
               auto tmp_child = m_top_child;
               while(tmp_child)
               {
                   if(auto attr = tmp_child->m_attribution; (attr & WINDOW_ATTRIBUTION::PRIORITY) && (attr & WINDOW_ATTRIBUTION::VISIBLE))
                   {
                       priority_window = tmp_child;
                       break;
                   }
                   tmp_child = tmp_child->m_next_sibling;
               }
               if(priority_window)
               {
                   return priority_window->on_navigate(key);
               }
               if(!is_focus_window())
               {
                   return;
               }
               if(key != NAVIGATION_KEY::BACKWARD && key != NAVIGATION_KEY::FORWARD)
               {
                   if(m_focus_child)
                   {
                       m_focus_child->on_navigate(key);
                   }
                   return;
               }
               // Move focus
               auto old_focus_window = m_focus_child;
               // No current focus wnd, new one
               if(!old_focus_window)
               {
                   auto child = m_top_child;
                   while (child)
                   {
                       if(child->is_focus_window())
                       {
                           auto new_focus_window = child;
                           new_focus_window->m_parent->set_child_focus(new_focus_window);
                           child = child->m_top_child;
                           continue;
                       }
                       child = child->m_next_sibling;
                   }
                   return;
               }
               // Move focus from old wnd to next window
               auto next_focus_window = (key == NAVIGATION_KEY::FORWARD) ? old_focus_window->m_next_sibling : old_focus_window->m_prev_sibling;
               while(next_focus_window && (!next_focus_window->is_focus_window()))
               {
                   // Search neighbor of old focus window
                   next_focus_window = (key == NAVIGATION_KEY::FORWARD) ? next_focus_window->m_next_sibling : next_focus_window->m_prev_sibling;
               }
               if(!next_focus_window)
               {
                   // Search whole brother window
                   next_focus_window = (key == NAVIGATION_KEY::FORWARD) ? old_focus_window->m_parent->m_top_child : old_focus_window->m_parent->get_last_child();
                   while(next_focus_window && (!next_focus_window->is_focus_window()))
                   {
                       next_focus_window = (key == NAVIGATION_KEY::FORWARD) ? next_focus_window->m_next_sibling : next_focus_window->m_prev_sibling;
                   }
               }
               if(next_focus_window)
               {
                   next_focus_window->m_parent->set_child_focus(next_focus_window);
               }
           }

           [[nodiscard]] CoreSurface* get_surface() const {return m_surface;}

           void set_surface(CoreSurface* surface) {m_surface = surface;}

       protected:
           virtual void pre_create_window() {};
           void add_child_to_tail(CoreWindow* child)
           {
               if(child == nullptr) {return;}
               if(child == get_window_by_id(child->m_id)) {return;}
               if(m_top_child == nullptr)
               {
                   m_top_child = child;
                   child->m_prev_sibling = nullptr;
                   child->m_next_sibling = nullptr;
               }
               else
               {
                   auto last_child = get_last_child();
                   if(last_child == nullptr)
                   {
                       GalAssert(false);
                   }
                   last_child->m_next_sibling = child;
                   child->m_prev_sibling = last_child;
                   child->m_next_sibling = nullptr;
               }
           }

           [[nodiscard]] std::pair<int, int> window_to_screen() const
           {
               auto parent = m_parent;
               int x = m_window_rect.m_left;
               int y = m_window_rect.m_top;
               while (parent != nullptr)
               {
                   auto rect = parent->get_window_rect();
                   x += rect.m_left;
                   y += rect.m_top;
                   parent = parent->m_parent;
               }

               return std::make_pair(x, y);
           }

           int load_child_window(window_tree* child_tree)
           {
               if(child_tree == nullptr){return 0;}

               auto sum = 0;
               auto curr = child_tree;
               while(curr->window)
               {
                   if(curr->window->m_id != 0)
                   {
                       //This wnd has been used! Do not share!
                       GalAssert(false);
                       return -1;
                   }
                   else
                   {
                       curr->window->connect(this, curr->resource_id, curr->str, curr->x, curr->y, curr->width, curr->height, curr->child_tree);
                   }
                   ++curr;
                   ++sum;
               }

               return sum;
           }
           void set_active_child(CoreWindow* child){m_focus_child = child;}
           virtual void on_focus(){};
           virtual void on_kill_focus(){};

           IdType m_id;
           WINDOW_STATUS m_status;
           WINDOW_ATTRIBUTION m_attribution;
           CoreRect m_window_rect;//position relative to parent window
           CoreWindow* m_parent;//parent window
           CoreWindow* m_top_child;//the first sub window would be navigated
           CoreWindow* m_prev_sibling;//previous brother
           CoreWindow* m_next_sibling;//next brother
           CoreWindow* m_focus_child;//current focused window
           const char* m_str;//caption
           const CoreTheme::font_info* m_font_type;
           ColorType m_font_color;
           ColorType m_background_color;
           CoreSurface::Z_ORDER_LEVEL m_z_order;
           CoreSurface* m_surface;
       };
    }
}
