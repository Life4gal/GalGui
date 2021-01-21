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

    constexpr auto AlignHorizontalCenter            = 0x00000000L;
    constexpr auto AlignLeft                        = 0x01000000L;
    constexpr auto AlignRight                       = 0x02000000L;
    constexpr auto AlignHorizontalMask              = 0x03000000L;
    constexpr auto AlignVerticalCenter              = 0x00000000L;
    constexpr auto AlignTop                         = 0x00100000L;
    constexpr auto AlignBottom                      = 0x00200000L;
    constexpr auto AlignVerticalMask                = 0x00300000L;

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

       enum COLOR_BYTE_TYPE
       {
           COLOR_BITS_16 = 2,
           COLOR_BITS_32 = 4
       };

       using B16ByteType = unsigned short;
       using B32ByteType = unsigned int;

       class CoreTheme
       {
           enum FONT_TYPE
           {
               FONT_NULL,
               FONT_DEFAULT,
               FONT_CUSTOM,
               FONT_MAX
           };
           enum BITMAP_TYPE
           {
               BITMAP_CUSTOM,
               BITMAP_MAX
           };
           enum COLOR_TYPE
           {
               COLOR_WND_FONT,
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

       public:
           static bool add_font(FONT_TYPE index, const font_info* font)
           {
               if (index >= FONT_MAX)
               {
                   GalAssert(false);
                   return false;
               }
               s_font_map[index] = font;
               return true;
           }

           static const font_info* get_font(FONT_TYPE index)
           {
               if(index >= FONT_MAX)
               {
                   GalAssert(false);
                   return nullptr;
               }
               return s_font_map[index];
           }

           static bool add_bitmap(BITMAP_TYPE index, const bitmap_info* bitmap)
           {
               if(index >= BITMAP_MAX)
               {
                   GalAssert(false);
                   return false;
               }
               s_bitmap_map[index] = bitmap;
               return true;
           }

           static const bitmap_info* get_bitmap(BITMAP_TYPE index)
           {
               if(index >= BITMAP_MAX)
               {
                   GalAssert(false);
                   return nullptr;
               }
               return s_bitmap_map[index];
           }

           static bool add_color(COLOR_TYPE index, const unsigned int color)
           {
               if(index >= COLOR_MAX)
               {
                   GalAssert(false);
                   return false;
               }
               s_color_map[index] = color;
               return true;
           }

           static unsigned int get_color(COLOR_TYPE index)
           {
               if(index >= COLOR_MAX)
               {
                   GalAssert(false);
                   return static_cast<unsigned int>(-1);
               }
               return s_color_map[index];
           }

       private:
           static std::array<const font_info*, FONT_MAX> s_font_map;
           static std::array<const bitmap_info*, BITMAP_MAX> s_bitmap_map;
           static std::array<unsigned int, COLOR_MAX> s_color_map;
       };

       class CoreSurface {
           enum Z_ORDER_LEVEL {
               Z_ORDER_LEVEL_LOWEST,
               Z_ORDER_LEVEL_MIDDLE,
               Z_ORDER_LEVEL_HIGHEST,
               Z_ORDER_LEVEL_MAX
           };

           template<COLOR_BYTE_TYPE Bytes>
           using ColorByteType = std::conditional_t<Bytes == COLOR_BITS_16, B16ByteType, B32ByteType>;
           using B16ColorByteType = ColorByteType<COLOR_BITS_16>;
           using B32ColorByteType = ColorByteType<COLOR_BITS_32>;
           using FramebufferType = std::variant<B16ColorByteType*, B32ColorByteType*, std::nullptr_t>;

           class CoreDisplay {
               struct external_gfx_operator {
                   std::function<void(int, int, unsigned int)> draw_pixel;
                   std::function<void(int, int, int, int, unsigned int)> fill_rect;
               };

               friend class CoreSurface;

           public:
               CoreDisplay(
                       FramebufferType* physical_framebuffer,
                       int display_width,
                       int display_height,
                       int suerface_width,
                       int surface_height,
                       int surface_count,
                       external_gfx_operator *gfx_operator = nullptr);//multiple surface or surface_no_fb

               CoreDisplay(
                       FramebufferType* physical_framebuffer,
                       int display_width,
                       int display_height,
                       CoreSurface* surface);//single custom surface

               CoreSurface* alloc_surface(
                       Z_ORDER_LEVEL maz_z_order,
                       CoreRect layer_rect = CoreRect());//for multiple surfaces

               int swipe_surface(CoreSurface* surface1, CoreSurface* surface2, int x0, int x1, int y0, int y1, int offset);

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
               COLOR_BYTE_TYPE bytes;

               if(std::holds_alternative<B16ColorByteType*>(m_framebuffer))
               {
                   bytes = COLOR_BITS_16;
                   source = reinterpret_cast<char*>(std::get<B16ColorByteType*>(m_framebuffer));
                   dest = reinterpret_cast<char*>(std::get<B16ColorByteType*>(m_physical_framebuffer));
               }
               else
               {
                   bytes = COLOR_BITS_32;
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

               for(;y0 <= y1; ++y0)
               {
                   auto framebuffer_index = y0 * m_width + x0;
                   auto physical_framebuffer_index = y0 * display_width + x0;
                   void* framebuffer = nullptr;
                   void* physical_framebuffer = nullptr;
                   COLOR_BYTE_TYPE bytes;
                   bool is_framebuffer_null = false;

                   if(std::holds_alternative<std::nullptr_t>(m_framebuffer))
                   {
                       is_framebuffer_null = true;
                   }

                   if(std::holds_alternative<B32ColorByteType*>(m_physical_framebuffer))
                   {
                       bytes = COLOR_BITS_32;
                   }
                   else
                   {
                       bytes = COLOR_BITS_16;
                   }

                   if(bytes == COLOR_BITS_16)
                   {
                       if(!is_framebuffer_null)
                       {
                           framebuffer = &std::get<B16ColorByteType*>(m_framebuffer)[framebuffer_index];
                       }
                       physical_framebuffer = &std::get<B16ColorByteType*>(m_physical_framebuffer)[physical_framebuffer_index];
                       rgb = utility::rgb_convert_32_to_16(rgb);
                   }
                   else
                   {
                       if(!is_framebuffer_null)
                       {
                           framebuffer = &std::get<B32ColorByteType*>(m_framebuffer)[framebuffer_index];
                       }
                       physical_framebuffer = &std::get<B32ColorByteType*>(m_physical_framebuffer)[physical_framebuffer_index];
                   }

                   ++*m_physical_write_index;

                   for(auto x = x0, offset = 0; x <= x1; ++x, ++offset)
                   {
                       if(bytes == COLOR_BITS_16)
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
                   if(bytes == COLOR_BITS_16)
                   {
                       return (std::get<B16ColorByteType*>(buffer) = static_cast<B16ColorByteType*>(std::calloc(m_width * m_height, bytes)));
                   }
                   else if(bytes == COLOR_BITS_32)
                   {
                       return (std::get<B32ColorByteType*>(buffer) = static_cast<B32ColorByteType*>(std::calloc(m_width * m_height, bytes)));
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

       private:
           static void write_rgb_to_buffer(FramebufferType buffer, unsigned int index, unsigned int rgb)
           {
               std::visit(utility::overloaded{
                                  [&](B16ColorByteType* data){data[index] = utility::rgb_convert_32_to_16(rgb);},
                                  [&](B32ColorByteType* data){data[index] = rgb;},
                                  [&](std::nullptr_t data){GalAssert(false);}
                          }, buffer);
           }

           [[nodiscard]] static unsigned int get_rgb_from_buffer(const FramebufferType buffer, unsigned int index)
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
    }
}
