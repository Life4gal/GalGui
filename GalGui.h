#pragma once

#include <functional>
#include <numeric>
#include <optional>
#include <variant>
#include <array>

#include <cstring>

// to-do
#define GalAssert(condition)

namespace Gal {
    constexpr auto RealTimeTaskCycleMs              = 50;
    constexpr auto QueueBufferMaxLength             = 1024;
    constexpr auto SurfaceCountMax                  = 6;//root + pages
    constexpr auto WordBufferLength                 = 16;
    constexpr auto DefaultColorMask                 = 0xFF080408;

    enum class ALIGN_TYPE
    {
        ALIGN_HORIZONTAL_CENTER                     = 0x00000000L,
        ALIGN_HORIZONTAL_LEFT                       = 0x01000000L,
        ALIGN_HORIZONTAL_RIGHT                      = 0x02000000L,
        ALIGN_HORIZONTAL_MASK                       = 0x03000000L,
        ALIGN_VERTICAL_CENTER                       = 0x00000000L,
        ALIGN_VERTICAL_TOP                          = 0x00100000L,
        ALIGN_VERTICAL_BOTTOM                       = 0x00200000L,
        ALIGN_VERTICAL_MASK                         = 0x00300000L
    };
    using ALIGN_UNDERLYING_TYPE = std::underlying_type_t<ALIGN_TYPE>;

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

        constexpr unsigned build_argb(unsigned alpha, unsigned red, unsigned green, unsigned blue) {
            return (alpha << 24) | (red << 16) | (green << 8) | (blue << 0);
        }

        constexpr unsigned build_rgb(unsigned red, unsigned green, unsigned blue) {
            return (red << 16) | (green << 8) | (blue << 0);
        }

        constexpr unsigned get_alpha_from_argb(unsigned argb) {
            return (argb >> 24) & 0xFF;
        }

        constexpr unsigned get_red_from_argb(unsigned argb) {
            return (argb >> 16) & 0xFF;
        }

        constexpr unsigned get_green_from_argb(unsigned argb) {
            return (argb >> 8) & 0xFF;
        }

        constexpr unsigned get_blue_from_argb(unsigned argb) {
            return (argb >> 0) & 0xFF;
        }

        constexpr unsigned rgb_convert_32_to_16(unsigned rgb) {
            return ((rgb & 0xFF) >> 3) | ((rgb & 0xFC00) >> 5) | ((rgb & 0xF80000) >> 8);
        }

        constexpr unsigned rgb_convert_16_to_32(unsigned rgb) {
            return (0xFF << 24) | ((rgb & 0x1F) << 3) | ((rgb & 0x7E0) << 5) | ((rgb & 0xF800) << 8);
        }

        int build_bitmap(const char* file_name, unsigned width, unsigned height, unsigned char* data);

        template<typename... Args> struct overloaded : Args... { using Args::operator()...; };
        template<typename... Args> overloaded(Args...) -> overloaded<Args...>;
    }

    namespace detail
    {
        class CoreRect
        {
        public:
            explicit CoreRect(int left = -1, int top = -1, int right = -1, int bottom = -1)
                : m_left(left), m_top(top), m_right(right), m_bottom(bottom){};

            CoreRect(int left, int top, int width, int height, int)
                : CoreRect(left, top, left + width - 1, top + height - 1){};

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
           COLOR_BITS_16 = 2,
           COLOR_BITS_32 = 4
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

           static bool add_color(COLOR_TYPE index, const unsigned int color)
           {
               if(index >= COLOR_TYPE::COLOR_MAX)
               {
                   GalAssert(false);
                   return false;
               }
               s_color_map[static_cast<std::size_t>(index)] = color;
               return true;
           }

           static unsigned int get_color(COLOR_TYPE index)
           {
               if(index >= COLOR_TYPE::COLOR_MAX)
               {
                   GalAssert(false);
                   return static_cast<unsigned int>(-1);
               }
               return s_color_map[static_cast<std::size_t>(index)];
           }

       private:
           static std::array<const font_info*, static_cast<std::size_t>(FONT_TYPE::FONT_MAX)> s_font_map;
           static std::array<const bitmap_info*, static_cast<std::size_t>(BITMAP_TYPE::BITMAP_MAX)> s_bitmap_map;
           static std::array<unsigned int, static_cast<std::size_t>(COLOR_TYPE::COLOR_MAX)> s_color_map;
       };

       class CoreSurface {
       public:
           enum Z_ORDER_LEVEL {
               Z_ORDER_LEVEL_LOWEST,
               Z_ORDER_LEVEL_MIDDLE,
               Z_ORDER_LEVEL_HIGHEST,
               Z_ORDER_LEVEL_MAX
           };

           template<COLOR_BYTE_TYPE Bytes>
           using ColorByteType = std::conditional_t<Bytes == COLOR_BYTE_TYPE::COLOR_BITS_16, B16ByteType, B32ByteType>;
           using B16ColorByteType = ColorByteType<COLOR_BYTE_TYPE::COLOR_BITS_16>;
           using B32ColorByteType = ColorByteType<COLOR_BYTE_TYPE::COLOR_BITS_32>;
           using FramebufferType = std::variant<B16ColorByteType*, B32ColorByteType*, std::nullptr_t>;

           struct external_gfx_operator {
               std::function<void(int, int, unsigned int)> draw_pixel;
               std::function<void(int, int, int, int, unsigned int)> fill_rect;
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

                   COLOR_BYTE_TYPE bytes = std::holds_alternative<B16ColorByteType*>(m_physical_framebuffer) ? COLOR_BYTE_TYPE::COLOR_BITS_16 : COLOR_BYTE_TYPE::COLOR_BITS_32;

                   (layer_rect == CoreRect())
                   ?
                   m_surface_group[m_surface_index]->set_surface(max_z_order, CoreRect(0, 0, m_width - 1, m_height - 1, 0), bytes)
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
                   Z_ORDER_LEVEL max_z_order = Z_ORDER_LEVEL_LOWEST,
                   CoreRect overlapped_rect = CoreRect())
               : m_width(width),
                 m_height(height),
                 m_is_active(false),
                 m_max_z_order(max_z_order),
                 m_framebuffer(nullptr),
                 m_top_z_order(Z_ORDER_LEVEL_LOWEST),
                 m_physical_framebuffer(nullptr),
                 m_physical_write_index(nullptr),
                 m_display(nullptr) {
               (overlapped_rect == CoreRect())
                       ?
                       set_surface(max_z_order, CoreRect(0, 0, width - 1, height - 1, 0), color_bytes)
                       :
                       set_surface(max_z_order, overlapped_rect, color_bytes);
           }

           [[nodiscard]] unsigned int get_width() const {
               return m_width;
           }

           [[nodiscard]] unsigned int get_height() const {
               return m_height;
           }

           unsigned int get_pixel(int x, int y, unsigned int z_order) {
               if (x >= m_width || y >= m_height || x < 0 || y < 0 || z_order >= Z_ORDER_LEVEL_MAX) {
                   GalAssert(false);
                   return static_cast<unsigned int>(-1);
               }

               auto index = y * m_width + x;
               FramebufferType buffers[3] = {m_layers[z_order].framebuffer, m_framebuffer, m_physical_framebuffer};

               for(auto& buffer : buffers)
               {
                   if (!std::holds_alternative<std::nullptr_t>(buffer)) {
                       return get_rgb_from_buffer(buffer, index);
                   }
               }

               return static_cast<unsigned int>(-1);
           }

           virtual void draw_pixel(int x, int y, unsigned int rgb, unsigned int z_order) {
               if (x >= m_width || y >= m_height || x < 0 || y < 0) {
                   return;
               }
               if (z_order > static_cast<unsigned int>(m_max_z_order)) {
                   GalAssert(false);
                   return;
               }
               if (z_order == m_max_z_order) {
                   return draw_pixel_on_framebuffer(x, y, rgb);
               }

               if (z_order > static_cast<unsigned int>(m_top_z_order)) {
                   m_top_z_order = static_cast<Z_ORDER_LEVEL>(z_order);
               }
               if (auto layer = m_layers[z_order]; layer.rect.is_in_rect(x, y)) {
                   auto layer_rect = layer.rect;
                   auto buffer = layer.framebuffer;
                   auto index = layer_rect.distance_of_left(x) + layer_rect.distance_of_top(y) * layer_rect.get_width();

                   write_rgb_to_buffer(buffer, index, rgb);
               }

               if (z_order == m_top_z_order) {
                   return draw_pixel_on_framebuffer(x, y, rgb);
               }

               bool be_overlapped = false;
               for (unsigned int tmp_z_order = Z_ORDER_LEVEL_MAX - 1; tmp_z_order > z_order; --tmp_z_order) {
                   if (m_layers[tmp_z_order].rect.is_in_rect(x, y)) {
                       be_overlapped = true;
                       break;
                   }
               }
               if (!be_overlapped) {
                   draw_pixel_on_framebuffer(x, y, rgb);
               }
           }

           virtual void fill_rect(int x0, int y0, int x1, int y1, unsigned int rgb, unsigned int z_order) {
               x0 = utility::max(x0, 0);
               y0 = utility::max(y0, 0);
               x1 = utility::min(x1, m_width - 1);
               y1 = utility::min(y1, (m_height - 1));

               if (z_order == m_max_z_order) {
                   return fill_rect_on_framebuffer(x0, y0, x1, y1, rgb);
               }
               if (z_order == m_top_z_order) {
                   auto layer = m_layers[z_order];
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

           void draw_horizontal_line(int x0, int x1, int y, unsigned int rgb, unsigned int z_order) {
               for (; x0 <= x1; ++x0) {
                   draw_pixel(x0, y, rgb, z_order);
               }
           }

           void draw_vertical_line(int x, int y0, int y1, unsigned int rgb, unsigned int z_order) {
               for (; y0 <= y1; ++y0) {
                   draw_pixel(x, y0, rgb, z_order);
               }
           }

           void draw_line(int x0, int y0, int x1, int y1, unsigned int rgb, unsigned int z_order) {
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

           void draw_rect(int x0, int y0, int x1, int y1, unsigned int rgb, unsigned int z_order, unsigned int scale = 1) {
               for (auto offset = 0; offset < scale; ++offset) {
                   draw_horizontal_line(x0 + offset, x1 - offset, y0 + offset, rgb, z_order);
                   draw_horizontal_line(x0 + offset, x1 - offset, y1 - offset, rgb, z_order);
                   draw_vertical_line(x0 + offset, y0 + offset, y1 - offset, rgb, z_order);
                   draw_vertical_line(x1 - offset, y0 + offset, y1 - offset, rgb, z_order);
               }
           }

           void draw_rect(CoreRect rect, unsigned int rgb, unsigned int z_order, unsigned int scale) {
               draw_rect(rect.m_left, rect.m_top, rect.m_right, rect.m_bottom, rgb, z_order, scale);
           }

           void fill_rect(CoreRect rect, unsigned int rgb, unsigned int z_order) {
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
                   bytes = static_cast<UNDERLYING_BYTE_TYPE>(COLOR_BYTE_TYPE::COLOR_BITS_16);
                   source = reinterpret_cast<char*>(std::get<B16ColorByteType*>(m_framebuffer));
                   dest = reinterpret_cast<char*>(std::get<B16ColorByteType*>(m_physical_framebuffer));
               }
               else
               {
                   bytes = static_cast<UNDERLYING_BYTE_TYPE>(COLOR_BYTE_TYPE::COLOR_BITS_32);
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

           void show_layer(CoreRect& rect, unsigned int z_order)
           {
               GalAssert(z_order >= Z_ORDER_LEVEL_LOWEST && z_order < Z_ORDER_LEVEL_MAX);
               auto layer = m_layers[z_order];
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
           virtual void draw_pixel_on_framebuffer(int x, int y, unsigned int rgb)
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
           virtual void fill_rect_on_framebuffer(int x0, int y0, int x1, int y1, unsigned int rgb)
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
                   bytes = COLOR_BYTE_TYPE::COLOR_BITS_16;
                   if(!is_framebuffer_null)
                   {
                       framebuffer = std::get<B16ColorByteType*>(m_framebuffer);
                   }
                   physical_framebuffer = std::get<B16ColorByteType*>(m_physical_framebuffer);
                   rgb = utility::rgb_convert_32_to_16(rgb);
               }
               else
               {
                   bytes = COLOR_BYTE_TYPE::COLOR_BITS_32;
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

                   if(bytes == COLOR_BYTE_TYPE::COLOR_BITS_16)
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
                       if(bytes == COLOR_BYTE_TYPE::COLOR_BITS_16)
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
                   if(bytes == COLOR_BYTE_TYPE::COLOR_BITS_16)
                   {
                       return (std::get<B16ColorByteType*>(buffer) = static_cast<B16ColorByteType*>(std::calloc(m_width * m_height, static_cast<std::size_t>(bytes))));
                   }
                   else if(bytes == COLOR_BYTE_TYPE::COLOR_BITS_32)
                   {
                       return (std::get<B32ColorByteType*>(buffer) = static_cast<B32ColorByteType*>(std::calloc(m_width * m_height, static_cast<std::size_t>(bytes))));
                   }

                   return nullptr;
               };

               if(m_display && (m_display->m_surface_count > 1))
               {
                   malloc_buffer(m_framebuffer);
               }
               for(int i = Z_ORDER_LEVEL_LOWEST; i < Z_ORDER_LEVEL_MAX; ++i)
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
           CoreLayer m_layers[Z_ORDER_LEVEL_MAX];
           bool m_is_active;
           Z_ORDER_LEVEL m_max_z_order;
           Z_ORDER_LEVEL m_top_z_order;
           FramebufferType m_physical_framebuffer;
           int* m_physical_write_index;
           CoreDisplay* m_display;

           static void write_rgb_to_buffer(FramebufferType& buffer, unsigned int index, unsigned int rgb)
           {
               std::visit(utility::overloaded{
                                  [&](B16ColorByteType* data){data[index] = utility::rgb_convert_32_to_16(rgb);},
                                  [&](B32ColorByteType* data){data[index] = rgb;},
                                  [&](std::nullptr_t data){GalAssert(false);}
                          }, buffer);
           }

           [[nodiscard]] static unsigned int get_rgb_from_buffer(const FramebufferType& buffer, unsigned int index)
           {
               auto ret = static_cast<unsigned int>(-1);
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
                   CoreSurface::Z_ORDER_LEVEL max_z_order = CoreSurface::Z_ORDER_LEVEL_LOWEST,
                   CoreRect overlapped_rect = CoreRect())
                   : CoreSurface(width, height, color_bytes, max_z_order, overlapped_rect), m_gfx_operator(gfx_operator) {};

           [[nodiscard]] CoreSurface::external_gfx_operator* get_gfx_operator() const
           {
               return m_gfx_operator;
           }

       protected:
           void draw_pixel_on_framebuffer(int x, int y, unsigned int rgb) override
           {
               if(m_gfx_operator && m_gfx_operator->draw_pixel && m_is_active)
               {
                   m_gfx_operator->draw_pixel(x, y, rgb);
               }
               if(std::holds_alternative<std::nullptr_t>(m_framebuffer)){return;}
               write_rgb_to_buffer(m_framebuffer, y * m_width + x, rgb);
           }

           void fill_rect_on_framebuffer(int x0, int y0, int x1, int y1, unsigned int rgb) override
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
                   bytes = COLOR_BYTE_TYPE::COLOR_BITS_16;
                   framebuffer = std::get<B16ColorByteType*>(m_framebuffer);
                   rgb = utility::rgb_convert_32_to_16(rgb);
               }
               else
               {
                   bytes = COLOR_BYTE_TYPE::COLOR_BITS_32;
                   framebuffer = std::get<B32ColorByteType*>(m_framebuffer);
               }

               for(auto y = y0; y <= y1; ++y)
               {
                   auto index = y0 * m_width + x0;
                   if(bytes == COLOR_BYTE_TYPE::COLOR_BITS_16)
                   {
                       framebuffer = static_cast<B16ColorByteType*>(framebuffer) + index;
                   }
                   else
                   {
                       framebuffer = static_cast<B32ColorByteType*>(framebuffer) + index;
                   }

                   for(auto x = x0, offset = 0; x <= x1; ++x, ++offset)
                   {
                       if(bytes == COLOR_BYTE_TYPE::COLOR_BITS_16)
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
           GalAssert(color_bytes == COLOR_BITS_16 || color_bytes == COLOR_BITS_32);
           GalAssert(m_surface_count <= SurfaceCountMax);

           for(int i = 0; i < m_surface_count; ++i)
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
                           static_cast<UNDERLYING_BYTE_TYPE>(COLOR_BYTE_TYPE::COLOR_BITS_16)
                           :
                           static_cast<UNDERLYING_BYTE_TYPE>(COLOR_BYTE_TYPE::COLOR_BITS_32);

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
                   bytes = static_cast<UNDERLYING_BYTE_TYPE>(COLOR_BYTE_TYPE::COLOR_BITS_16);
                   left_source = reinterpret_cast<char*>(std::get<B16ColorByteType*>(surface1->m_framebuffer));
                   right_source = reinterpret_cast<char*>(std::get<B16ColorByteType*>(surface2->m_framebuffer));
                   dest = reinterpret_cast<char*>(std::get<B16ColorByteType*>(m_physical_framebuffer));
               }
               else
               {
                   bytes = static_cast<UNDERLYING_BYTE_TYPE>(COLOR_BYTE_TYPE::COLOR_BITS_32);
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
           static void draw_string(
                   CoreSurface* surface,
                   CoreSurface::Z_ORDER_LEVEL z_order,
                   const char* str,
                   int x, int y,
                   const CoreTheme::font_info* font,
                   unsigned int font_color,
                   unsigned background_color,
                   ALIGN_TYPE align_type = ALIGN_TYPE::ALIGN_HORIZONTAL_LEFT)
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
                   unsigned int font_color,
                   unsigned int background_color,
                   ALIGN_TYPE align_type = ALIGN_TYPE::ALIGN_HORIZONTAL_LEFT)
           {
               if(str == nullptr){return;}

               int x;
               int y;
               get_string_pos(str, font, rect, align_type, x, y);
               draw_string(surface, z_order, str, rect.m_left + x, rect.m_top + y, font, font_color, background_color, align_type);
           }

           static void draw_value(
                   CoreSurface* surface,
                   CoreSurface::Z_ORDER_LEVEL z_order,
                   int value,
                  int dot_position,
                   int x, int y,
                   const CoreTheme::font_info* font,
                   unsigned int font_color,
                   unsigned int background_color,
                   ALIGN_TYPE align_type = ALIGN_TYPE::ALIGN_HORIZONTAL_LEFT)
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
                   unsigned int font_color,
                   unsigned int background_color,
                   ALIGN_TYPE align_type = ALIGN_TYPE::ALIGN_HORIZONTAL_LEFT)
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
                   unsigned int font_color,
                   unsigned int background_color)
           {
               auto error_color = static_cast<unsigned int>(-1);
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
                   unsigned int font_color,
                   unsigned int background_color)
           {
               unsigned char block_value;
               unsigned char block_count;
               unsigned int red;
               unsigned int green;
               unsigned int blue;
               unsigned int rgb;

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

           static void get_string_pos(const char* str, const CoreTheme::font_info* font, CoreRect rect, ALIGN_TYPE align_type, int& x, int& y)
           {
               auto _align_type = static_cast<ALIGN_UNDERLYING_TYPE>(align_type);

               int x_size;
               int y_size;
               get_str_size(str, font, x_size, y_size);
               auto width = rect.get_width();
               auto height = rect.get_height();
               x = 0;
               y = 0;

               switch (_align_type & ALIGN_TYPE::ALIGN_HORIZONTAL_MASK) {
                   case ALIGN_TYPE::ALIGN_HORIZONTAL_CENTER:
                       if(width > x_size)
                       {
                           x = (width - x_size) / 2;
                       }
                       break;
                   case ALIGN_TYPE::ALIGN_HORIZONTAL_LEFT:
                       x = 0;
                       break;
                   case ALIGN_TYPE::ALIGN_HORIZONTAL_RIGHT:
                       if(width > x_size)
                       {
                           x = width - x_size;
                       }
                       break;
                   default:
                       GalAssert(false);
                       break;
               }
               switch (_align_type & ALIGN_TYPE::ALIGN_VERTICAL_MASK) {
                   case ALIGN_TYPE::ALIGN_VERTICAL_CENTER:
                       if(height > y_size)
                       {
                           y = (height - y_size) / 2;
                       }
                       break;
                   case ALIGN_TYPE::ALIGN_VERTICAL_TOP:
                       y = 0;
                       break;
                   case ALIGN_TYPE::ALIGN_VERTICAL_BOTTOM:
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

       class CoreBitmap;
    }
}
