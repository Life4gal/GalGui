#ifndef GALGUI
#define GALGUI

#include <functional>
#include <numeric>
#include <optional>
#include <utility>
#include <variant>
#include <array>
#include <vector>
#include <any>
#include <string>
#include <numeric>

#include <cstring>
#include <cmath>

// to-do
#define GalAssert(condition) do{}while(0)

namespace Gal {
    /*
     * <del>enum = flag1 | enum1</del>
     * <del>usage:</del>
     * <del>     if(flag1 | enum1 == ENUM::XXX){xxx;}</del>
     * <del>     switch(flag1 | enum1){case ENUM:XXX: xxx;}</del>
     *
     * <del>note: if no exist enumerates equal flag, it will lose all data(it maybe equal to the first enumerate but no guarantee)</del>
     *
     * flag = flag1 | enum1
     * make return type to EnumType(which used to be Enum)
     *
     * usage:
     *      EnumType flag = xxx;
     *      ......
     *      EnumType another_flag = flag | some_enum;
     *
     *      <del>std::array<CoordinateType, (1 | EnumMax)> replace std::array<CoordinateType, static_cast<size_t>(EnumMax)> also supported</del>
     */
    template <typename Enum, typename EnumType, typename = std::enable_if_t<std::is_enum_v<Enum>>, typename = std::enable_if_t<std::is_same_v<EnumType, std::underlying_type_t<Enum>>>>
    constexpr EnumType operator|(const EnumType& lhs, const Enum& rhs)
    {
        // using EnumType | EnumType
        return static_cast<EnumType>(lhs | static_cast<EnumType>(rhs));
    }

    /*
     * <del>enum = enum1 | flag1</del>
     * <del>usage:</del>
     * <del>     if(enum1 | flag1 == ENUM::XXX){xxx;}</del>
     * <del>     switch(enum1 | flag1){case ENUM:XXX: xxx;}</del>
     *
     * <del>note: if no exist enumerates equal flag, it will lose all data(it maybe equal to the first enumerate but no guarantee)</del>
     *
     * enum = enum1 | flag1
     * although the enum may not exist, but it will store it's data correctly by use it's underlying type
     *
     * usage:
     *      Enum enum = xxx;
     *      ......
     *      Enum another_enum = enum | some_enum_type_val;
     */
    template <typename Enum, typename EnumType, typename = std::enable_if_t<std::is_enum_v<Enum>>, typename = std::enable_if_t<std::is_same_v<EnumType, std::underlying_type_t<Enum>>>>
    constexpr Enum operator|(const Enum& lhs, const EnumType& rhs)
    {
        // forward to EnumType operator|(const EnumType& lhs, const Enum& rhs)
        // forward is necessary ?
        return static_cast<Enum>(std::forward<const EnumType&>(rhs) | std::forward<const Enum&>(lhs));
    }

    /*
     * <del>flag = enum1 | enum2</del>
     * <del>if no exist enumerates equal flag, it will lose all data(it maybe equal to the first enumerate but no guarantee)</del>
     * <del>so cast it's type to EnumType</del>
     *
     * enum = enum1 | enum2
     * although the enum may not exist, but it will store it's data correctly by use it's underlying type
     * make return type to Enum(which used to be EnumType)
     *
     * usage:
     *      void Foo(Enum arg);
     *      call Foo(enum1 | enum2) -> (enum1 | enum2) <=> enum
     *
     *      void Bar(Enum arg)
     *      {
     *          switch(arg | EnumMask)
     *          {
     *              case enum1: xxx
     *              case enum2: xxx
     *              ...
     *          }
     *      }
     */
    template <typename Enum, typename EnumType = std::underlying_type_t<Enum>, typename = std::enable_if_t<std::is_enum_v<Enum>>, typename = std::enable_if_t<std::is_same_v<EnumType, std::underlying_type_t<Enum>>>>
    constexpr Enum operator |(const Enum& lhs, const Enum& rhs)
    {
        // using EnumType | EnumType
        return static_cast<Enum>(static_cast<EnumType>(lhs) | static_cast<EnumType>(rhs));
    }

    /*
     * flag |= enum
     * only flag |= enum operator is valid
     * use enum |= flag is unreasonable
     */
    template <typename Enum, typename EnumType, typename = std::enable_if_t<std::is_enum_v<Enum>>, typename = std::enable_if_t<std::is_same_v<EnumType, std::underlying_type_t<Enum>>>>
    constexpr EnumType& operator|=(EnumType& lhs, const Enum& rhs)
    {
        // forward to EnumType operator|(const EnumType& lhs, const Enum& rhs)
        // forward is necessary ?
        return (lhs = (std::forward<const EnumType&>(lhs) | std::forward<const Enum&>(rhs)));
    }

    /*
     * <del>enum = flag1 & enum1</del>
     * <del>usage:</del>
     * <del>     if(flag1 & enum1 == ENUM::XXX){xxx;}</del>
     * <del>     switch(flag1 & enum1){case ENUM:XXX: xxx;}</del>
     *
     * <del>note: if no exist enumerates equal flag, it will lose all data(it maybe equal to the first enumerate but no guarantee)</del>
     *
     * flag = flag1 & enum1
     * make return type to EnumType(which used to be Enum)
     *
     * usage:
     *      EnumType flag = xxx;
     *      ......
     *      EnumType another_flag = flag & some_enum;
     */
    template <typename Enum, typename EnumType, typename = std::enable_if_t<std::is_enum_v<Enum>>, typename = std::enable_if_t<std::is_same_v<EnumType, std::underlying_type_t<Enum>>>>
    constexpr EnumType operator&(const EnumType& lhs, const Enum& rhs)
    {
        // using EnumType & EnumType
        return static_cast<EnumType>(lhs & static_cast<EnumType>(rhs));
    }

    /*
     * <del>enum = enum1 & flag1</del>
     * <del>usage:</del>
     * <del>     if(enum1 & flag1 == ENUM::XXX){xxx;}</del>
     * <del>     switch(enum1 & flag1){case ENUM:XXX: xxx;}</del>
     *
     * <del>note: if no exist enumerates equal flag, it will lose all data(it maybe equal to the first enumerate but no guarantee)</del>
     *
     * enum = enum1 & flag1
     * although the enum may not exist, but it will store it's data correctly by use it's underlying type
     *
     * usage:
     *      Enum enum = xxx;
     *      ......
     *      Enum another_enum = enum & some_enum_type_val;
     */
    template <typename Enum, typename EnumType, typename = std::enable_if_t<std::is_enum_v<Enum>>, typename = std::enable_if_t<std::is_same_v<EnumType, std::underlying_type_t<Enum>>>>
    constexpr Enum operator&(const Enum& lhs, const EnumType& rhs)
    {
        // forward to EnumType operator&(const EnumType& lhs, const Enum& rhs)
        // forward is necessary ?
        return static_cast<Enum>(std::forward<const EnumType&>(rhs) & std::forward<const Enum&>(lhs));
    }

    /*
     * <del>flag = enum1 & enum2</del>
     * <del>if no exist enumerates equal flag, it will lose all data(it maybe equal to the first enumerate but no guarantee)</del>
     * <del>so cast it's type to EnumType</del>
     *
     * enum = enum1 & enum2
     * although the enum may not exist, but it will store it's data correctly by use it's underlying type
     * make return type to Enum(which used to be EnumType)
     *
     * usage:
     *      void Foo(Enum arg);
     *      call Foo(enum1 | enum2) -> (enum1 | enum2) <=> enum
     *
     *      void Bar(Enum arg)
     *      {
     *          switch(arg & EnumMask)
     *          {
     *              case enum1: xxx
     *              case enum2: xxx
     *              ...
     *          }
     *      }
     */
    template <typename Enum, typename EnumType = std::underlying_type_t<Enum>, typename = std::enable_if_t<std::is_enum_v<Enum>>, typename = std::enable_if_t<std::is_same_v<EnumType, std::underlying_type_t<Enum>>>>
    constexpr Enum operator&(const Enum& lhs, const Enum& rhs)
    {
        // using EnumType & EnumType
        return static_cast<Enum>(static_cast<EnumType>(lhs) & static_cast<EnumType>(rhs));
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
            // if CoordinateType is not a integer type(like abs(1.1f)), it will not call this abs(invisible)
            constexpr auto bit_length = sizeof(T) * 8;
            return (n ^ (n >> (bit_length - 1))) - (n >> (bit_length - 1));
        }

        // to-do
        int build_bitmap(const char* file_name, unsigned width, unsigned height, unsigned char* data);

        template<typename... Args>
        struct overloaded : Args... { using Args::operator()...; };
        template<typename... Args>
        overloaded(Args...) -> overloaded<Args...>;

        /*
         * (enum1 & enum2) == enum2 <=> EnumContains(enum1, enum2) == true
         *
         * determine whether enum1 contains enum2 or not
         * because of operator&(const Enum& lhs, const Enum& rhs) return Enum
         *
         * to-do: check is a EnumContains(flag, enum1, enum2, enum3...) expression valid or not in edit time(not compile time)
         *      such as: EnumContains(flag, enum1, enum2, 123)
         */
        template<typename Enum, typename... EnumPack, typename = std::enable_if_t<std::is_enum_v<Enum>>>
        constexpr bool EnumContains(const Enum& enum1, const Enum& enum2, const EnumPack&... enums)
        {
            if constexpr (sizeof...(enums) == 0)
            {
                return (enum1 & enum2) == enum2;
            }
            else
            {
                return EnumContains(enum1, enums...);
            }
        }
    }

    namespace detail {
        template<typename CoordinateType = int, typename = std::enable_if_t<std::is_arithmetic_v<CoordinateType>>>
        class CorePoint
        {
        public:
            constexpr explicit CorePoint() noexcept : m_x(CoordinateType()), m_y(CoordinateType()) {};

            constexpr CorePoint(CoordinateType x, CoordinateType y) noexcept : m_x(x), m_y(y) {};

            constexpr CorePoint(std::initializer_list<CoordinateType> list)
            {
                GalAssert(list.size() == 2);

                m_x = *list.begin();
                m_y = *list.end();
            }

            template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T> && std::is_convertible_v<T, CoordinateType>>>
            CorePoint operator+(const CorePoint<T>& other) const noexcept
            {
                return {m_x + other.m_x, m_y + other.m_y};
            }

            template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T> && std::is_convertible_v<T, CoordinateType>>>
            CorePoint operator+(T distance) const noexcept
            {
                return {m_x + distance, m_y + distance};
            }

            template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T> && std::is_convertible_v<T, CoordinateType>>>
            CorePoint& operator+=(const CorePoint<T>& other) noexcept
            {
                m_x += other.m_x;
                m_y += other.m_y;

                return *this;
            }

            template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T> && std::is_convertible_v<T, CoordinateType>>>
            CorePoint& operator+=(T distance) noexcept
            {
                m_x += distance;
                m_y += distance;

                return *this;
            }

            template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T> && std::is_convertible_v<T, CoordinateType>>>
            CorePoint operator-(const CorePoint<T>& other) const noexcept
            {
                return {m_x - other.m_x, m_y - other.m_y};
            }

            template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T> && std::is_convertible_v<T, CoordinateType>>>
            CorePoint operator-(T distance) const noexcept
            {
                return {m_x - distance, m_y - distance};
            }

            template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T> && std::is_convertible_v<T, CoordinateType>>>
            CorePoint& operator-=(const CorePoint<T>& other) noexcept
            {
                m_x -= other.m_x;
                m_y -= other.m_y;

                return *this;
            }

            template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T> && std::is_convertible_v<T, CoordinateType>>>
            CorePoint& operator-=(T distance) noexcept
            {
                m_x -= distance;
                m_y -= distance;

                return *this;
            }

            template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T> && std::is_convertible_v<T, CoordinateType>>>
            bool operator==(const CorePoint<T>& other) const noexcept
            {
                return m_x == other.m_x && m_y == other.m_y;
            }

            template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T> && std::is_convertible_v<T, CoordinateType>>>
            bool operator>(const CorePoint<T>& other) const noexcept
            {
                return m_x > other.m_x && m_y > other.m_y;
            }

            template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T> && std::is_convertible_v<T, CoordinateType>>>
            bool operator>=(const CorePoint<T>& other) const noexcept
            {
                return this->operator>(other) || this->operator==(other);
            }

            template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T> && std::is_convertible_v<T, CoordinateType>>>
            bool operator<(const CorePoint<T>& other) const noexcept
            {
                return !(this->operator>(other) || this->operator==(other));
            }

            template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T> && std::is_convertible_v<T, CoordinateType>>>
            bool operator<=(const CorePoint<T>& other) const noexcept
            {
                return this->operator<(other) || this->operator==(other);
            }

            template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T> && std::is_convertible_v<T, CoordinateType>>>
            [[nodiscard]] constexpr bool horizontal_greater_than(T x) const noexcept
            {
                return m_x > x;
            }

            template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T> && std::is_convertible_v<T, CoordinateType>>>
            [[nodiscard]] constexpr bool horizontal_greater_equal(T x) const noexcept
            {
                return m_x >= x;
            }

            template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T> && std::is_convertible_v<T, CoordinateType>>>
            [[nodiscard]] constexpr bool vertical_greater_than(T y) const noexcept
            {
                return m_y > y;
            }

            template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T> && std::is_convertible_v<T, CoordinateType>>>
            [[nodiscard]] constexpr bool vertical_greater_equal(T y) const noexcept
            {
                return m_y >= y;
            }

            template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T> && std::is_convertible_v<T, CoordinateType>>>
            [[nodiscard]] constexpr bool greater_than(T x, T y) const noexcept
            {
                return horizontal_greater_than(x) && vertical_greater_than(y);
            }

            template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T> && std::is_convertible_v<T, CoordinateType>>>
            [[nodiscard]] constexpr bool greater_equal(T x, T y) const noexcept
            {
                return horizontal_greater_equal(x) && vertical_greater_equal(y);
            }

            template <typename T, typename RetType = std::common_type_t<T, CoordinateType>, typename = std::enable_if_t<std::is_arithmetic_v<T> && std::is_convertible_v<T, CoordinateType>>>
            [[nodiscard]] constexpr RetType horizontal_distance(const CorePoint<T>& other) const noexcept
            {
                return other.m_x - m_x + 1;
            }

            template <typename T, typename RetType = std::common_type_t<T, CoordinateType>, typename = std::enable_if_t<std::is_arithmetic_v<T> && std::is_convertible_v<T, CoordinateType>>>
            [[nodiscard]] constexpr RetType vertical_distance(const CorePoint<T>& other) const noexcept
            {
                return other.m_y - m_y + 1;
            }

            template <typename T, typename RetType = std::common_type_t<T, CoordinateType>, typename = std::enable_if_t<std::is_arithmetic_v<T> && std::is_convertible_v<T, CoordinateType>>>
            [[nodiscard]] constexpr RetType horizontal_distance(T x) const noexcept
            {
                return x - m_x + 1;
            }

            template <typename T, typename RetType = std::common_type_t<T, CoordinateType>, typename = std::enable_if_t<std::is_arithmetic_v<T> && std::is_convertible_v<T, CoordinateType>>>
            [[nodiscard]] constexpr RetType vertical_distance(T y) const noexcept
            {
                return y - m_y + 1;
            }

            template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T> && std::is_convertible_v<T, CoordinateType>>>
            [[nodiscard]] constexpr auto get_distance(const CorePoint<T>& other) const noexcept
            {
                return std::sqrt(std::pow(horizontal_distance(other), 2) + std::pow(vertical_distance(other), 2));
            }

            template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T> && std::is_convertible_v<T, CoordinateType>>>
            constexpr void reposition(T x, T y) noexcept
            {
                m_x = x;
                m_y = y;
            }

            template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T> && std::is_convertible_v<T, CoordinateType>>>
            constexpr void horizontal_move(T x) noexcept
            {
                m_x += x;
            }

            template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T> && std::is_convertible_v<T, CoordinateType>>>
            constexpr void vertical_move(T y) noexcept
            {
                m_y += y;
            }

            template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T> && std::is_convertible_v<T, CoordinateType>>>
            constexpr void move_to(T x, T y) noexcept
            {
                horizontal_move(x);
                vertical_move(y);
            }

            // clamp a point in the specified area (not allowed on the edge of the area)
            template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T> && std::is_convertible_v<T, CoordinateType>>>
            constexpr void clamp_point_upper(T x, T y) noexcept
            {
                m_x = (m_x >= x) ? (x - 1) : m_x;
                m_y = (m_y >= y) ? (y - 1) : m_y;
            }

            // clamp a point in the specified area (allowed on the edge of the area)
            template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T> && std::is_convertible_v<T, CoordinateType>>>
            constexpr void clamp_point_lower(T x, T y) noexcept
            {
                m_x = (m_x < x) ? x : m_x;
                m_y = (m_y < y) ? y : m_y;
            }

            // [x1, x2)
            template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T> && std::is_convertible_v<T, CoordinateType>>>
            [[nodiscard]] constexpr bool horizontal_between(T x1, T x2) const noexcept
            {
                return m_x >= x1 && m_x < x2;
            }

            // [y1, y2)
            template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T> && std::is_convertible_v<T, CoordinateType>>>
            [[nodiscard]] constexpr bool vertical_between(T y1, T y2) const noexcept
            {
                return m_y >= y1 && m_y < y2;
            }

            CoordinateType m_x;
            CoordinateType m_y;
        };

        template<typename CoordinateType = int, typename = std::enable_if_t<std::is_arithmetic_v<CoordinateType>>>
        class CoreRect {
        public:
            constexpr explicit CoreRect() noexcept
                : m_left_top_corner(), m_right_bottom_corner() {};

            template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T> && std::is_convertible_v<T, CoordinateType>>>
            constexpr CoreRect(T left, T top, T width, T height) noexcept
                : m_left_top_corner(left, top), m_right_bottom_corner(left + width - 1, top + height - 1) {};

            template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T> && std::is_convertible_v<T, CoordinateType>>>
            constexpr CoreRect(CorePoint<T> left_top, CorePoint<T> right_bottom) noexcept
                : m_left_top_corner(left_top), m_right_bottom_corner(right_bottom) {};

            template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T> && std::is_convertible_v<T, CoordinateType>>>
            void set_rect(T left, T top, T width, T height) noexcept
            {
                GalAssert(width > 0 && height > 0);

                m_left_top_corner.reposition(left, top);
                m_right_bottom_corner.reposition(left + width - 1, top + height - 1);
            }

            template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T> && std::is_convertible_v<T, CoordinateType>>>
            void set_rect(const CorePoint<T>& left_top, const CorePoint<T>& right_bottom) noexcept
            {
                m_left_top_corner = left_top;
                m_right_bottom_corner = right_bottom;
            }

            // negative left value means expand forward left, positive right value means expand forward right
            template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T> && std::is_convertible_v<T, CoordinateType>>>
            void horizontal_expand(T left, T right) noexcept
            {
                m_left_top_corner.horizontal_move(left);
                m_right_bottom_corner.horizontal_move(right);
            }

            // negative top value means expand forward top, positive bottom value means expand forward bottom
            template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T> && std::is_convertible_v<T, CoordinateType>>>
            void vertical_expand(T top, T bottom) noexcept
            {
                m_left_top_corner.vertical_move(top);
                m_right_bottom_corner.vertical_move(bottom);
            }

            template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T> && std::is_convertible_v<T, CoordinateType>>>
            void move_rect(T horizontal, T vertical) noexcept
            {
                m_left_top_corner.move_to(horizontal, vertical);
                m_right_bottom_corner.move_to(horizontal, vertical);
            }

            template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T> && std::is_convertible_v<T, CoordinateType>>>
            [[nodiscard]] bool is_in_rect(T x, T y) const noexcept
            {
                return !m_left_top_corner.greater_than(x, y) && m_right_bottom_corner.greater_equal(x, y);
            }

            template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T> && std::is_convertible_v<T, CoordinateType>>>
            [[nodiscard]] bool is_in_rect(const CorePoint<T>& point) const noexcept
            {
                return point >= m_left_top_corner && point <= m_right_bottom_corner;
            }

            template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T> && std::is_convertible_v<T, CoordinateType>>>
            [[nodiscard]] bool is_in_rect(const CoreRect<T>& rect) const noexcept
            {
                return is_in_rect(rect.m_left_top_corner) && is_in_rect(rect.m_right_bottom_corner);
            }

            template <typename T, typename RetType = std::common_type_t<T, CoordinateType>, typename = std::enable_if_t<std::is_arithmetic_v<T> && std::is_convertible_v<T, CoordinateType>>>
            [[nodiscard]] RetType distance_of_left(T x) const noexcept
            {
                return m_left_top_corner.horizontal_distance(x);
            }

            template <typename T, typename RetType = std::common_type_t<T, CoordinateType>, typename = std::enable_if_t<std::is_arithmetic_v<T> && std::is_convertible_v<T, CoordinateType>>>
            [[nodiscard]] RetType distance_of_top(T y) const noexcept
            {
                return m_left_top_corner.vertical_distance(y);
            }

            template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T> && std::is_convertible_v<T, CoordinateType>>>
            bool operator==(const CoreRect<T> &other) const noexcept
            {
                return m_left_top_corner == other.m_left_top_corner && m_right_bottom_corner == other.m_right_bottom_corner;
            }

            [[nodiscard]] CoordinateType get_width() const noexcept{
                return m_right_bottom_corner.horizontal_distance(m_left_top_corner);
            }

            [[nodiscard]] CoordinateType get_height() const noexcept{
                return m_right_bottom_corner.vertical_distance(m_left_top_corner);
            }

            [[nodiscard]] CoordinateType& left() noexcept
            {
                return m_left_top_corner.m_x;
            }

            [[nodiscard]] CoordinateType& left() const noexcept
            {
                return m_left_top_corner.m_x;
            }

            [[nodiscard]] CoordinateType& top() noexcept
            {
                return m_left_top_corner.m_y;
            }

            [[nodiscard]] CoordinateType& top() const noexcept
            {
                return m_left_top_corner.m_y;
            }

            [[nodiscard]] CoordinateType& right() noexcept
            {
                return m_right_bottom_corner.m_x;
            }

            [[nodiscard]] CoordinateType& right() const noexcept
            {
                return m_right_bottom_corner.m_x;
            }

            [[nodiscard]] CoordinateType& bottom() noexcept
            {
                return m_right_bottom_corner.m_y;
            }

            [[nodiscard]] CoordinateType& bottom() const noexcept
            {
                return m_right_bottom_corner.m_y;
            }

            CorePoint<CoordinateType> m_left_top_corner;
            CorePoint<CoordinateType> m_right_bottom_corner;
        };

    }

    using ColorType = unsigned int;
    constexpr ColorType DefaultColorMask = 0xFF080408;

    namespace utility{
        constexpr ColorType build_argb(ColorType alpha, ColorType red, ColorType green, ColorType blue) noexcept {
            return (alpha << 24) | (red << 16) | (green << 8) | (blue << 0);
        }

        constexpr ColorType build_rgb(ColorType red, ColorType green, ColorType blue) noexcept {
            return (red << 16) | (green << 8) | (blue << 0);
        }

        constexpr ColorType get_alpha_from_argb(ColorType argb) noexcept {
            return (argb >> 24) & 0xFF;
        }

        constexpr ColorType get_red_from_argb(ColorType argb) noexcept {
            return (argb >> 16) & 0xFF;
        }

        constexpr ColorType get_green_from_argb(ColorType argb) noexcept {
            return (argb >> 8) & 0xFF;
        }

        constexpr ColorType get_blue_from_argb(ColorType argb) noexcept {
            return (argb >> 0) & 0xFF;
        }

        constexpr ColorType rgb_convert_32_to_16(ColorType rgb) noexcept {
            return ((rgb & 0xFF) >> 3) | ((rgb & 0xFC00) >> 5) | ((rgb & 0xF80000) >> 8);
        }

        constexpr ColorType rgb_convert_16_to_32(ColorType rgb) noexcept {
            return (0xFF << 24) | ((rgb & 0x1F) << 3) | ((rgb & 0x7E0) << 5) | ((rgb & 0xF800) << 8);
        }
    }

    constexpr auto SurfaceCountMax = 6;//root + pages
    constexpr auto WordBufferLength = 16;

    namespace detail{
        enum class COLOR_BYTE_TYPE {
            BITS_16 = 2,
            BITS_32 = 4
        };

        using B16ByteType = unsigned short;
        using B32ByteType = unsigned int;

        class CoreTheme {
        public:
            enum class FONT_TYPE {
                NONE = 0,
                DEFAULT,
                CUSTOM0,
                CUSTOM1,
                CUSTOM2,
                MAX
            };
            enum class BITMAP_TYPE {
                CUSTOM0 = 0,
                CUSTOM1,
                CUSTOM2,
                BITMAP_MAX
            };
            enum class COLOR_TYPE {
                WND_FONT = 0,
                WND_NORMAL,
                WND_PUSHED,
                WND_FOCUS,
                WND_BORDER,
                CUSTOM0,
                CUSTOM1,
                CUSTOM2,
                MAX
            };

            struct bitmap_info {
                //support 16 bits only
                B16ByteType width;
                B16ByteType height;
                const B16ByteType *pixel_color_array;
            };

            struct font_info {
                struct lattice {
                    unsigned int utf8_code;
                    unsigned char width;
                    const unsigned char *pixel_gray_array;
                };
                unsigned char height;
                unsigned int count;
                lattice *lattice_array;
            };

            static bool add_font(FONT_TYPE index, const font_info *font) {
                if (index >= FONT_TYPE::MAX) {
                    GalAssert(false);
                    return false;
                }
                s_font_map[static_cast<std::size_t>(index)] = font;
                return true;
            }

            static const font_info *get_font(FONT_TYPE index) {
                if (index >= FONT_TYPE::MAX) {
                    GalAssert(false);
                    return nullptr;
                }
                return s_font_map[static_cast<std::size_t>(index)];
            }

            static bool add_bitmap(BITMAP_TYPE index, const bitmap_info *bitmap) {
                if (index >= BITMAP_TYPE::BITMAP_MAX) {
                    GalAssert(false);
                    return false;
                }
                s_bitmap_map[static_cast<std::size_t>(index)] = bitmap;
                return true;
            }

            static const bitmap_info *get_bitmap(BITMAP_TYPE index) {
                if (index >= BITMAP_TYPE::BITMAP_MAX) {
                    GalAssert(false);
                    return nullptr;
                }
                return s_bitmap_map[static_cast<std::size_t>(index)];
            }

            static bool add_color(COLOR_TYPE index, const ColorType color) {
                if (index >= COLOR_TYPE::MAX) {
                    GalAssert(false);
                    return false;
                }
                s_color_map[static_cast<std::size_t>(index)] = color;
                return true;
            }

            static ColorType get_color(COLOR_TYPE index) {
                if (index >= COLOR_TYPE::MAX) {
                    GalAssert(false);
                    return static_cast<ColorType>(-1);
                }
                return s_color_map[static_cast<std::size_t>(index)];
            }

        private:
            static std::array<const font_info *, static_cast<std::size_t>(FONT_TYPE::MAX)> s_font_map;
            static std::array<const bitmap_info *, static_cast<std::size_t>(BITMAP_TYPE::BITMAP_MAX)> s_bitmap_map;
            static std::array<ColorType, static_cast<std::size_t>(COLOR_TYPE::MAX)> s_color_map;
        };

        class CoreSurface {
            friend class CoreBitmap;

        public:
            enum class Z_ORDER_LEVEL {
                LOWEST = 0,
                MIDDLE = 1,
                HIGHEST = 2,
                MAX = 3
            };

            template<COLOR_BYTE_TYPE Bytes>
            using ColorByteType = std::conditional_t<Bytes == COLOR_BYTE_TYPE::BITS_16, B16ByteType, B32ByteType>;
            using B16ColorByteType = ColorByteType<COLOR_BYTE_TYPE::BITS_16>;
            using B32ColorByteType = ColorByteType<COLOR_BYTE_TYPE::BITS_32>;

            using FramebufferType = std::variant<B16ColorByteType *, B32ColorByteType *, std::nullptr_t>;

            using PixelPositionType = int;
            using SurfaceSizeType = int;

            template<typename T = PixelPositionType>
            struct external_gfx_operator {
                std::function<void(CorePoint<T>, ColorType)> draw_pixel;
                std::function<void(CorePoint<T>, CorePoint<T>, ColorType)> fill_rect;
            };

            class CoreDisplay {
                friend class CoreSurface;
                friend class CoreSurfaceNoFramebuffer;

            public:
                //multiple surface or surface_no_fb
                CoreDisplay(
                        FramebufferType physical_framebuffer,
                        SurfaceSizeType display_width,
                        SurfaceSizeType display_height,
                        COLOR_BYTE_TYPE color_bytes,
                        SurfaceSizeType surface_width,
                        SurfaceSizeType surface_height,
                        size_t surface_count,
                        external_gfx_operator<PixelPositionType> *gfx_operator = nullptr);

                //single custom surface
                CoreDisplay(
                        FramebufferType physical_framebuffer,
                        SurfaceSizeType display_width,
                        SurfaceSizeType display_height,
                        CoreSurface *surface);

                //for multiple surfaces
                CoreSurface *alloc_surface(
                        Z_ORDER_LEVEL max_z_order,
                        CoreRect<PixelPositionType> layer_rect = CoreRect<PixelPositionType>()) {
                    GalAssert(max_z_order < MAX && m_surface_index < m_surface_count);
                    GalAssert(!std::holds_alternative<std::nullptr_t>(m_physical_framebuffer));

                    COLOR_BYTE_TYPE bytes = std::holds_alternative<B16ColorByteType *>(m_physical_framebuffer) ? COLOR_BYTE_TYPE::BITS_16 : COLOR_BYTE_TYPE::BITS_32;

                    (layer_rect == CoreRect<PixelPositionType>())
                            ? m_surface_group[m_surface_index]->set_surface(max_z_order, CoreRect(0, 0, m_width - 1, m_height - 1), bytes)
                            : m_surface_group[m_surface_index]->set_surface(max_z_order, layer_rect, bytes);

                    return m_surface_group[m_surface_index++];
                }

                bool swipe_surface(CoreSurface* surface1, CoreSurface* surface2, CorePoint<PixelPositionType> left_top, CorePoint<PixelPositionType> right_bottom, PixelPositionType offset);

                bool swipe_surface(CoreSurface* surface1, CoreSurface* surface2, const CoreRect<PixelPositionType>& rect, PixelPositionType offset)
                {
                    return swipe_surface(surface1, surface2, rect.m_left_top_corner, rect.m_right_bottom_corner, offset);
                }

                [[nodiscard]] SurfaceSizeType get_width() const noexcept {
                    return m_width;
                }

                [[nodiscard]] SurfaceSizeType get_height() const noexcept {
                    return m_height;
                }

                FramebufferType get_updated_framebuffer(SurfaceSizeType *width, SurfaceSizeType *height, bool force_update = false) {
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
                    if (std::holds_alternative<B16ColorByteType *>(m_physical_framebuffer)) {
                        //16 bits framebuffer
                        return utility::build_bitmap(file_name, width, height, reinterpret_cast<unsigned char *>(std::get<B16ColorByteType *>(m_physical_framebuffer)));
                    } else {
                        //32 bits framebuffer
                        auto p_bitmap565_data = new B16ColorByteType[width * height];
                        auto p_raw_data = std::get<B32ColorByteType *>(m_physical_framebuffer);
                        for (auto i = 0; i < width * height; ++i) {
                            p_bitmap565_data[i] = utility::rgb_convert_32_to_16(*p_raw_data++);
                        }
                        auto ret = utility::build_bitmap(file_name, width, height, reinterpret_cast<unsigned char *>(p_bitmap565_data));
                        delete[] p_bitmap565_data;
                        return ret;
                    }
                }

            private:
                SurfaceSizeType m_width; //in pixels
                SurfaceSizeType m_height;//in pixels
                FramebufferType m_physical_framebuffer;
                size_t m_physical_read_index;
                size_t m_physical_write_index;
                std::array<CoreSurface *, SurfaceCountMax> m_surface_group;
                size_t m_surface_count;
                size_t m_surface_index;
            };

            class CoreLayer {
            public:
                CoreLayer() : framebuffer(nullptr), rect(CoreRect<PixelPositionType>()){};
                FramebufferType framebuffer;
                CoreRect<PixelPositionType> rect;//framebuffer area
            };

        public:
            CoreSurface(
                    SurfaceSizeType width,
                    SurfaceSizeType height,
                    COLOR_BYTE_TYPE color_bytes,
                    Z_ORDER_LEVEL max_z_order = Z_ORDER_LEVEL::LOWEST,
                    CoreRect<PixelPositionType> overlapped_rect = CoreRect<PixelPositionType>())
                : m_width(width),
                  m_height(height),
                  m_is_active(false),
                  m_max_z_order(max_z_order),
                  m_framebuffer(nullptr),
                  m_top_z_order(Z_ORDER_LEVEL::LOWEST),
                  m_physical_framebuffer(nullptr),
                  m_physical_write_index(nullptr),
                  m_display(nullptr) {
                (overlapped_rect == CoreRect<PixelPositionType>())
                        ? set_surface(max_z_order, CoreRect(0, 0, width - 1, height - 1), color_bytes)
                        : set_surface(max_z_order, overlapped_rect, color_bytes);
            }

            [[nodiscard]] SurfaceSizeType get_width() const noexcept {
                return m_width;
            }

            [[nodiscard]] SurfaceSizeType get_height() const noexcept {
                return m_height;
            }

            [[nodiscard]] ColorType get_pixel(const CorePoint<PixelPositionType>& point, Z_ORDER_LEVEL z_order) const {
                if(!point.horizontal_between(0, m_width) || !point.vertical_between(0, m_height) || z_order >= Z_ORDER_LEVEL::MAX)
                {
                    GalAssert(false);
                    return static_cast<ColorType>(-1);
                }

                auto index = point.m_y * m_width + point.m_x;

                for (auto &buffer : {m_layers[static_cast<size_t>(z_order)].framebuffer, m_framebuffer, m_physical_framebuffer}) {
                    if (!std::holds_alternative<std::nullptr_t>(buffer)) {
                        return get_rgb_from_buffer(buffer, index);
                    }
                }

                return static_cast<ColorType>(-1);
            }

            virtual void draw_pixel(CorePoint<PixelPositionType> point, ColorType rgb, Z_ORDER_LEVEL z_order) {
                if (!point.horizontal_between(0, m_width) || !point.vertical_between(0, m_height)) {
                    return;
                }
                if (z_order > m_max_z_order) {
                    GalAssert(false);
                    return;
                }
                if (z_order == m_max_z_order) {
                    return draw_pixel_on_framebuffer(point, rgb);
                }

                if (z_order > m_top_z_order) {
                    m_top_z_order = z_order;
                }
                if (auto layer = m_layers[static_cast<size_t>(z_order)]; layer.rect.is_in_rect(point)) {
                    auto layer_rect = layer.rect;
                    auto buffer = layer.framebuffer;
                    auto index = layer_rect.distance_of_left(point.m_x) + layer_rect.distance_of_top(point.m_y) * layer_rect.get_width();

                    write_rgb_to_buffer(buffer, index, rgb);
                }

                if (z_order == m_top_z_order) {
                    return draw_pixel_on_framebuffer(point, rgb);
                }

                bool be_overlapped = false;
                for (auto tmp_z_order = static_cast<size_t>(Z_ORDER_LEVEL::MAX) - 1; tmp_z_order > static_cast<size_t>(z_order); --tmp_z_order) {
                    if (m_layers[tmp_z_order].rect.is_in_rect(point)) {
                        be_overlapped = true;
                        break;
                    }
                }
                if (!be_overlapped) {
                    draw_pixel_on_framebuffer(point, rgb);
                }
            }

            virtual void fill_rect(CorePoint<PixelPositionType> left_top, CorePoint<PixelPositionType> right_bottom, ColorType rgb, Z_ORDER_LEVEL z_order) {
                left_top.clamp_point_lower(0, 0);
                right_bottom.clamp_point_upper(m_width, m_height);

                if (z_order == m_max_z_order) {
                    return fill_rect_on_framebuffer(left_top, right_bottom, rgb);
                }
                if (z_order == m_top_z_order) {
                    auto layer = m_layers[static_cast<size_t>(z_order)];
                    auto layer_rect = layer.rect;
                    auto buffer = layer.framebuffer;
                    for (auto y = left_top.m_y; y <= right_bottom.m_y; ++y) {
                        for (auto x = left_top.m_x; x <= right_bottom.m_x; ++x) {
                            if (layer_rect.is_in_rect(x, y)) {
                                auto index = layer_rect.distance_of_left(x) + layer_rect.distance_of_top(y) * layer_rect.get_width();
                                write_rgb_to_buffer(buffer, index, rgb);
                            }
                        }
                    }
                    return fill_rect_on_framebuffer(left_top, right_bottom, rgb);
                }
                for (auto y = left_top.m_y; y <= right_bottom.m_y; ++y) {
                    draw_horizontal_line(left_top.m_x, right_bottom.m_x, y, rgb, z_order);
                }
            }

            void draw_horizontal_line(PixelPositionType x0, PixelPositionType x1, PixelPositionType y, ColorType rgb, Z_ORDER_LEVEL z_order) {
                for (; x0 <= x1; ++x0) {
                    draw_pixel({x0, y}, rgb, z_order);
                }
            }

            void draw_vertical_line(PixelPositionType x, PixelPositionType y0, PixelPositionType y1, ColorType rgb, Z_ORDER_LEVEL z_order) {
                for (; y0 <= y1; ++y0) {
                    draw_pixel({x, y0}, rgb, z_order);
                }
            }

            void draw_line(CorePoint<PixelPositionType> left_top, CorePoint<PixelPositionType> right_bottom, ColorType rgb, Z_ORDER_LEVEL z_order) {
                auto [x0, y0] = left_top;
                auto [x1, y1] = right_bottom;

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
                        draw_pixel({x0, y0}, rgb, z_order);
                        if (e > 0) {
                            e -= dx;
                            (y > y1) ? --y0 : ++y0;
                        }
                    }
                } else {
                    auto e = dx - dy / 2;
                    for (; y0 <= y1; ++y0, e += dx) {
                        draw_pixel({x0, y0}, rgb, z_order);
                        if (e > 0) {
                            e -= dy;
                            (x > x1) ? --x0 : ++x0;
                        }
                    }
                }
            }

            void draw_rect(CorePoint<PixelPositionType> left_top, CorePoint<PixelPositionType> right_bottom, ColorType rgb, Z_ORDER_LEVEL z_order, unsigned int scale = 1) {
                auto [x0, y0] = left_top;
                auto [x1, y1] = right_bottom;
                for (auto offset = 0; offset < scale; ++offset) {
                    draw_horizontal_line(x0 + offset, x1 - offset, y0 + offset, rgb, z_order);
                    draw_horizontal_line(x0 + offset, x1 - offset, y1 - offset, rgb, z_order);
                    draw_vertical_line(x0 + offset, y0 + offset, y1 - offset, rgb, z_order);
                    draw_vertical_line(x1 - offset, y0 + offset, y1 - offset, rgb, z_order);
                }
            }

            void draw_rect(CoreRect<PixelPositionType> rect, ColorType rgb, Z_ORDER_LEVEL z_order, unsigned int scale) {
                draw_rect(rect.m_left_top_corner, rect.m_right_bottom_corner, rgb, z_order, scale);
            }

            void fill_rect(CoreRect<PixelPositionType> rect, ColorType rgb, Z_ORDER_LEVEL z_order) {
                fill_rect(rect.m_left_top_corner, rect.m_right_bottom_corner, rgb, z_order);
            }

            bool flush_screen(CoreRect<PixelPositionType> rect)
            {
                return flush_screen(rect.m_left_top_corner, rect.m_right_bottom_corner);
            }

            bool flush_screen(CorePoint<PixelPositionType> left_top, CorePoint<PixelPositionType> right_bottom) {
                if(!left_top.horizontal_between(0, m_width) || !right_bottom.horizontal_between(0, m_width) ||
                    !left_top.vertical_between(0, m_height) || !right_bottom.vertical_between(0, m_height))
                {
                    GalAssert(false);
                }
                if (!m_is_active || std::holds_alternative<std::nullptr_t>(m_physical_framebuffer) || std::holds_alternative<std::nullptr_t>(m_framebuffer)) {
                    return false;
                }

                auto display_width = m_display->get_width();
                auto display_height = m_display->get_height();
                left_top.clamp_point_upper(display_width, display_height);
                right_bottom.clamp_point_upper(display_width, display_height);

                char *source = nullptr;
                char *dest = nullptr;
                using UNDERLYING_BYTE_TYPE = std::underlying_type_t<COLOR_BYTE_TYPE>;
                UNDERLYING_BYTE_TYPE bytes;

                if (std::holds_alternative<B16ColorByteType *>(m_framebuffer)) {
                    bytes = static_cast<UNDERLYING_BYTE_TYPE>(COLOR_BYTE_TYPE::BITS_16);
                    source = reinterpret_cast<char *>(std::get<B16ColorByteType *>(m_framebuffer));
                    dest = reinterpret_cast<char *>(std::get<B16ColorByteType *>(m_physical_framebuffer));
                } else {
                    bytes = static_cast<UNDERLYING_BYTE_TYPE>(COLOR_BYTE_TYPE::BITS_32);
                    source = reinterpret_cast<char *>(std::get<B32ColorByteType *>(m_framebuffer));
                    dest = reinterpret_cast<char *>(std::get<B32ColorByteType *>(m_physical_framebuffer));
                }

                for (auto y = left_top.m_y; y < right_bottom.m_y; ++y) {
                    source += (y * m_width + left_top.m_x) * bytes;
                    dest += (y * display_width + left_top.m_x) * bytes;

                    std::memcpy(dest, source, left_top.horizontal_distance(right_bottom) * bytes);
                }
                ++*m_physical_write_index;
                return true;
            }

            [[nodiscard]] bool is_active() const {
                return m_is_active;
            }

            void set_active(bool flag) {
                m_is_active = flag;
            }

            CoreDisplay *get_display() {
                return m_display;
            }

            void show_layer(CoreRect<PixelPositionType> rect, CoreSurface::Z_ORDER_LEVEL z_order) {
                GalAssert(z_order >= LOWEST && z_order < MAX);
                auto layer = m_layers[static_cast<size_t>(z_order)];
                auto layer_rect = layer.rect;
                GalAssert(layer_rect.is_in_rect(rect));
                auto buffer = layer.framebuffer;
                auto width = layer_rect.get_width();

                for (auto y = rect.top(); y <= rect.bottom(); ++y) {
                    for (auto x = rect.left(); x <= rect.right(); ++x) {
                        auto index = layer_rect.distance_of_left(x) + layer_rect.distance_of_top(y) * width;
                        draw_pixel_on_framebuffer({x, y}, get_rgb_from_buffer(buffer, index));
                    }
                }
            }

        protected:
            virtual void draw_pixel_on_framebuffer(CorePoint<PixelPositionType> point, ColorType rgb) {
                if (!std::holds_alternative<std::nullptr_t>(m_framebuffer)) {
                    write_rgb_to_buffer(m_framebuffer, point.m_y * m_width + point.m_x, rgb);
                }
                if (m_is_active && (point.m_x < m_display->get_width()) && (point.m_y < m_display->get_height())) {
                    write_rgb_to_buffer(m_physical_framebuffer, point.m_y * m_display->get_width() + point.m_x, rgb);
                }
                ++*m_physical_write_index;
            }
            virtual void fill_rect_on_framebuffer(CorePoint<PixelPositionType> left_top, CorePoint<PixelPositionType> right_bottom, ColorType rgb) {
                auto display_width = m_display->get_width();
                auto display_height = m_display->get_height();

                void *framebuffer = nullptr;
                void *physical_framebuffer = nullptr;
                COLOR_BYTE_TYPE bytes;
                bool is_framebuffer_null = false;

                if (std::holds_alternative<std::nullptr_t>(m_framebuffer)) {
                    is_framebuffer_null = true;
                }

                if (std::holds_alternative<B16ColorByteType *>(m_physical_framebuffer)) {
                    bytes = COLOR_BYTE_TYPE::BITS_16;
                    if (!is_framebuffer_null) {
                        framebuffer = std::get<B16ColorByteType *>(m_framebuffer);
                    }
                    physical_framebuffer = std::get<B16ColorByteType *>(m_physical_framebuffer);
                    rgb = utility::rgb_convert_32_to_16(rgb);
                } else {
                    bytes = COLOR_BYTE_TYPE::BITS_32;
                    if (!is_framebuffer_null) {
                        framebuffer = std::get<B32ColorByteType *>(m_framebuffer);
                    }
                    physical_framebuffer = std::get<B32ColorByteType *>(m_physical_framebuffer);
                }

                auto [x0, y0] = left_top;
                auto [x1, y1] = right_bottom;
                for (; y0 <= y1; ++y0) {
                    auto framebuffer_index = y0 * m_width + x0;
                    auto physical_framebuffer_index = y0 * display_width + x0;

                    if (bytes == COLOR_BYTE_TYPE::BITS_16) {
                        framebuffer = static_cast<B16ColorByteType *>(framebuffer) + framebuffer_index;
                        physical_framebuffer = static_cast<B16ColorByteType *>(physical_framebuffer) + physical_framebuffer_index;
                    } else {
                        framebuffer = static_cast<B32ColorByteType *>(framebuffer) + framebuffer_index;
                        physical_framebuffer = static_cast<B32ColorByteType *>(physical_framebuffer) + physical_framebuffer_index;
                    }

                    ++*m_physical_write_index;

                    for (auto x = x0, offset = 0; x <= x1; ++x, ++offset) {
                        if (bytes == COLOR_BYTE_TYPE::BITS_16) {
                            if (!is_framebuffer_null) {
                                *(static_cast<B16ColorByteType *>(framebuffer) + offset) = rgb;
                            }
                            if (m_is_active && (x < display_width) && (y0 < display_height)) {
                                *(static_cast<B16ColorByteType *>(physical_framebuffer) + offset) = rgb;
                            }
                        } else {
                            if (!is_framebuffer_null) {
                                *(static_cast<B32ColorByteType *>(framebuffer) + offset) = rgb;
                            }
                            if (m_is_active && (x < display_width) && (y0 < display_height)) {
                                *(static_cast<B32ColorByteType *>(physical_framebuffer) + offset) = rgb;
                            }
                        }
                    }
                }
            }

            void attach_display(CoreDisplay *display) {
                GalAssert(display);

                m_display = display;
                m_physical_framebuffer = display->m_physical_framebuffer;
                m_physical_write_index = &display->m_physical_write_index;
            }

            void set_surface(Z_ORDER_LEVEL max_z_order, CoreRect<PixelPositionType> layer_rect, COLOR_BYTE_TYPE bytes) {
                m_max_z_order = max_z_order;

                auto malloc_buffer = [&](FramebufferType buffer) -> void * {
                    if (bytes == COLOR_BYTE_TYPE::BITS_16) {
                        return (std::get<B16ColorByteType *>(buffer) = static_cast<B16ColorByteType *>(std::calloc(m_width * m_height, static_cast<std::size_t>(bytes))));
                    } else if (bytes == COLOR_BYTE_TYPE::BITS_32) {
                        return (std::get<B32ColorByteType *>(buffer) = static_cast<B32ColorByteType *>(std::calloc(m_width * m_height, static_cast<std::size_t>(bytes))));
                    }

                    return nullptr;
                };

                if (m_display && (m_display->m_surface_count > 1)) {
                    malloc_buffer(m_framebuffer);
                }
                for (auto i = static_cast<size_t>(Z_ORDER_LEVEL::LOWEST); i < static_cast<size_t>(Z_ORDER_LEVEL::MAX); ++i) {
                    // top layer framebuffer always be o
                    auto p = malloc_buffer(m_layers[i].framebuffer);
                    GalAssert(p != nullptr);
                    m_layers[i].rect = layer_rect;
                }
            }

            SurfaceSizeType m_width;
            SurfaceSizeType m_height;
            FramebufferType m_framebuffer;
            std::array<CoreLayer, static_cast<size_t>(Z_ORDER_LEVEL::MAX)> m_layers;
            bool m_is_active;
            Z_ORDER_LEVEL m_max_z_order;
            Z_ORDER_LEVEL m_top_z_order;
            FramebufferType m_physical_framebuffer;
            size_t *m_physical_write_index;
            CoreDisplay *m_display;

        public:
            static void write_rgb_to_buffer(FramebufferType &buffer, size_t index, ColorType rgb) {
                std::visit(utility::overloaded{
                                   [&](B16ColorByteType *data) { data[index] = utility::rgb_convert_32_to_16(rgb); },
                                   [&](B32ColorByteType *data) { data[index] = rgb; },
                                   [&](std::nullptr_t data) { GalAssert(false); }},
                           buffer);
            }

            [[nodiscard]] static ColorType get_rgb_from_buffer(const FramebufferType &buffer, size_t index) {
                auto ret = static_cast<ColorType>(-1);
                std::visit(utility::overloaded{
                                   [&](const B16ColorByteType *data) { ret = utility::rgb_convert_16_to_32(data[index]); },
                                   [&](const B32ColorByteType *data) { ret = data[index]; },
                                   [&](std::nullptr_t data) { GalAssert(false); }},
                           buffer);

                return ret;
            }
        };

        class CoreSurfaceNoFramebuffer : public CoreSurface {
            //No physical framebuffer, render with external graphic interface
        public:
            CoreSurfaceNoFramebuffer(
                    SurfaceSizeType width,
                    SurfaceSizeType height,
                    COLOR_BYTE_TYPE color_bytes,
                    CoreSurface::external_gfx_operator<PixelPositionType> *gfx_operator,
                    CoreSurface::Z_ORDER_LEVEL max_z_order = CoreSurface::Z_ORDER_LEVEL::LOWEST,
                    CoreRect<PixelPositionType> overlapped_rect = CoreRect<PixelPositionType>())
                : CoreSurface(width, height, color_bytes, max_z_order, overlapped_rect), m_gfx_operator(gfx_operator){};

            [[nodiscard]] CoreSurface::external_gfx_operator<PixelPositionType> *get_gfx_operator() const {
                return m_gfx_operator;
            }

        protected:
            void draw_pixel_on_framebuffer(CorePoint<PixelPositionType> point, ColorType rgb) override {
                if (m_gfx_operator && m_gfx_operator->draw_pixel && m_is_active) {
                    m_gfx_operator->draw_pixel(point, rgb);
                }
                if (std::holds_alternative<std::nullptr_t>(m_framebuffer)) { return; }
                write_rgb_to_buffer(m_framebuffer, point.m_y * m_width + point.m_x, rgb);
            }

            void fill_rect_on_framebuffer(CorePoint<PixelPositionType> left_top, CorePoint<PixelPositionType> right_bottom, ColorType rgb) override {
                if (!m_gfx_operator) { return; }

                if (auto &filler = m_gfx_operator->fill_rect; filler) {
                    return m_gfx_operator->fill_rect(left_top, right_bottom, rgb);
                }

                auto [x0, y0] = left_top;
                auto [x1, y1] = right_bottom;
                if (auto &drawer = m_gfx_operator->draw_pixel; drawer && m_is_active) {
                    for (auto y = y0; y <= y1; ++y) {
                        for (auto x = x0; x <= x1; ++x) {
                            drawer({x, y}, rgb);
                        }
                    }
                }
                if (std::holds_alternative<std::nullptr_t>(m_framebuffer)) { return; }

                void *framebuffer = nullptr;
                COLOR_BYTE_TYPE bytes;

                if (std::holds_alternative<B16ColorByteType *>(m_framebuffer)) {
                    bytes = COLOR_BYTE_TYPE::BITS_16;
                    framebuffer = std::get<B16ColorByteType *>(m_framebuffer);
                    rgb = utility::rgb_convert_32_to_16(rgb);
                } else {
                    bytes = COLOR_BYTE_TYPE::BITS_32;
                    framebuffer = std::get<B32ColorByteType *>(m_framebuffer);
                }

                for (auto y = y0; y <= y1; ++y) {
                    auto index = y0 * m_width + x0;
                    if (bytes == COLOR_BYTE_TYPE::BITS_16) {
                        framebuffer = static_cast<B16ColorByteType *>(framebuffer) + index;
                    } else {
                        framebuffer = static_cast<B32ColorByteType *>(framebuffer) + index;
                    }

                    for (auto x = x0, offset = 0; x <= x1; ++x, ++offset) {
                        if (bytes == COLOR_BYTE_TYPE::BITS_16) {
                            *(static_cast<B16ColorByteType *>(framebuffer) + offset) = rgb;
                        } else {
                            *(static_cast<B32ColorByteType *>(framebuffer) + offset) = rgb;
                        }
                    }
                }
            }

            //Rendering by external method
            CoreSurface::external_gfx_operator<PixelPositionType> *m_gfx_operator;
        };

        CoreSurface::CoreDisplay::CoreDisplay(
                FramebufferType physical_framebuffer,
                SurfaceSizeType display_width,
                SurfaceSizeType display_height,
                COLOR_BYTE_TYPE color_bytes,
                SurfaceSizeType surface_width,
                SurfaceSizeType surface_height,
                size_t surface_count,
                external_gfx_operator<PixelPositionType> *gfx_operator)
            : m_width(display_width),
              m_height(display_height),
              m_physical_framebuffer(physical_framebuffer),
              m_physical_read_index(0),
              m_physical_write_index(0),
              m_surface_count(surface_count),
              m_surface_index(0),
              m_surface_group({nullptr}) {
            GalAssert(color_bytes == BITS_16 || color_bytes == BITS_32);
            GalAssert(m_surface_count <= SurfaceCountMax);

            for (auto i = 0; i < m_surface_count; ++i) {
                m_surface_group[i] = (std::holds_alternative<std::nullptr_t>(physical_framebuffer))
                                             ? new CoreSurface(surface_width, surface_height, color_bytes)
                                             : new CoreSurfaceNoFramebuffer(surface_width, surface_height, color_bytes, gfx_operator);

                m_surface_group[i]->attach_display(this);
            }
        }

        CoreSurface::CoreDisplay::CoreDisplay(
                FramebufferType physical_framebuffer,
                SurfaceSizeType display_width,
                SurfaceSizeType display_height,
                CoreSurface *surface)
            : m_width(display_width),
              m_height(display_height),
              m_physical_framebuffer(physical_framebuffer),
              m_physical_read_index(0),
              m_physical_write_index(0),
              m_surface_count(1),
              m_surface_index(0),
              m_surface_group({nullptr}) {
            surface->m_is_active = true;
            m_surface_group[0] = surface;
            m_surface_group[0]->attach_display(this);
        }

        bool CoreSurface::CoreDisplay::swipe_surface(CoreSurface *surface1, CoreSurface *surface2, CorePoint<PixelPositionType> left_top, CorePoint<PixelPositionType> right_bottom, PixelPositionType offset)
        {
            auto surface_width = surface1->get_width();
            auto surface_height = surface1->get_height();
            if(offset < 0 || offset > surface_width ||
                !left_top.horizontal_between(0, surface_width) || !left_top.vertical_between(0, surface_height) ||
                !right_bottom.horizontal_between(0, surface_width) || !right_bottom.vertical_between(0, surface_height)
                )
            {
                GalAssert(false);
                return false;
            }

            auto width = left_top.horizontal_distance(right_bottom);
            if(width < 0 || width > surface_width || width < offset)
            {
                GalAssert(false);
                return false;
            }

            GalAssert(!std::holds_alternative<std::nullptr_t>(surface1->m_framebuffer));
            GalAssert(!std::holds_alternative<std::nullptr_t>(surface2->m_framebuffer));
            using UNDERLYING_BYTE_TYPE = std::underlying_type_t<COLOR_BYTE_TYPE>;
            UNDERLYING_BYTE_TYPE bytes =
                    std::holds_alternative<B16ColorByteType *>(surface1->m_framebuffer)
                    ? static_cast<UNDERLYING_BYTE_TYPE>(COLOR_BYTE_TYPE::BITS_16)
                    : static_cast<UNDERLYING_BYTE_TYPE>(COLOR_BYTE_TYPE::BITS_32);

            left_top.clamp_point_upper(m_width, m_height);
            right_bottom.clamp_point_upper(m_width, m_height);

            auto x0 = left_top.m_x;
            auto x1 = right_bottom.m_x;

            if (!std::holds_alternative<std::nullptr_t>(m_physical_framebuffer)) {
                char *left_source = nullptr;
                char *right_source = nullptr;
                char *dest = nullptr;

                if (std::holds_alternative<B16ColorByteType *>(m_physical_framebuffer)) {
                    bytes = static_cast<UNDERLYING_BYTE_TYPE>(COLOR_BYTE_TYPE::BITS_16);
                    left_source = reinterpret_cast<char *>(std::get<B16ColorByteType *>(surface1->m_framebuffer));
                    right_source = reinterpret_cast<char *>(std::get<B16ColorByteType *>(surface2->m_framebuffer));
                    dest = reinterpret_cast<char *>(std::get<B16ColorByteType *>(m_physical_framebuffer));
                } else {
                    bytes = static_cast<UNDERLYING_BYTE_TYPE>(COLOR_BYTE_TYPE::BITS_32);
                    left_source = reinterpret_cast<char *>(std::get<B32ColorByteType *>(surface1->m_framebuffer));
                    right_source = reinterpret_cast<char *>(std::get<B32ColorByteType *>(surface2->m_framebuffer));
                    dest = reinterpret_cast<char *>(std::get<B32ColorByteType *>(m_physical_framebuffer));
                }

                for (auto y = left_top.m_y; y <= right_bottom.m_y; ++y) {
                    //Left surface
                    auto left_source_index = (y * surface1->get_width() + x0 + offset) * bytes;
                    auto left_dest_index = (y * m_width + x0) * bytes;
                    //Right surface
                    auto right_source_index = (y * surface2->get_width() + x0) * bytes;
                    auto right_dest_index = (y * m_width + x0 + (width - offset)) * bytes;

                    std::memcpy(dest + left_dest_index, left_source + left_source_index, (width - offset) * bytes);
                    std::memcpy(dest + right_dest_index, right_source + right_source_index, offset * bytes);
                }
            } else {
                auto draw_pixel = dynamic_cast<CoreSurfaceNoFramebuffer *>(surface1)->get_gfx_operator()->draw_pixel;

                for (auto y = left_top.m_y; y <= right_bottom.m_y; ++y) {
                    //Left surface
                    for (auto x = x0; x <= (x1 - offset); ++x) {
                        draw_pixel({x, y}, get_rgb_from_buffer(surface1->m_framebuffer, y * m_width + x + offset));
                    }
                    //Right surface
                    for (auto x = x1 - offset; x <= x1; ++x) {
                        draw_pixel({x, y}, get_rgb_from_buffer(surface2->m_framebuffer, y * m_width + x + offset - x1 + x0));
                    }
                }
            }

            ++m_physical_write_index;
            return true;
        }

        class CoreWord {
        public:
            enum class ALIGN_TYPE {
                HORIZONTAL_CENTER = 0x00000000L,
                HORIZONTAL_LEFT = 0x01000000L,
                HORIZONTAL_RIGHT = 0x02000000L,
                HORIZONTAL_MASK = 0x03000000L,
                VERTICAL_CENTER = 0x00000000L,
                VERTICAL_TOP = 0x00100000L,
                VERTICAL_BOTTOM = 0x00200000L,
                VERTICAL_MASK = 0x00300000L
            };

            static void draw_string(
                    CoreSurface *surface,
                    CoreSurface::Z_ORDER_LEVEL z_order,
                    const char *str,
                    CorePoint<CoreSurface::PixelPositionType> point,
                    const CoreTheme::font_info *font,
                    ColorType font_color,
                    ColorType background_color,
                    ALIGN_TYPE align_type = ALIGN_TYPE::HORIZONTAL_LEFT) {
                if (str == nullptr) { return; }

                auto offset = 0;
                while (*str) {
                    unsigned int code;
                    str += get_utf8_code(str, code);
                    point.horizontal_move(offset);
                    offset += draw_single_char(surface, z_order, code, point, font, font_color, background_color);
                }
            }

            static void draw_string_in_rect(
                    CoreSurface *surface,
                    CoreSurface::Z_ORDER_LEVEL z_order,
                    const char *str,
                    CoreRect<CoreSurface::PixelPositionType> rect,
                    const CoreTheme::font_info *font,
                    ColorType font_color,
                    ColorType background_color,
                    ALIGN_TYPE align_type = ALIGN_TYPE::HORIZONTAL_LEFT) {
                if (str == nullptr) { return; }

                auto [x, y] = get_string_position(str, font, rect, align_type);
                auto point = rect.m_left_top_corner;
                point.move_to(x, y);
                draw_string(surface, z_order, str, point, font, font_color, background_color, align_type);
            }

            static void draw_value(
                    CoreSurface *surface,
                    CoreSurface::Z_ORDER_LEVEL z_order,
                    int value,
                    int dot_position,
                    CorePoint<CoreSurface::PixelPositionType> point,
                    const CoreTheme::font_info *font,
                    ColorType font_color,
                    ColorType background_color,
                    ALIGN_TYPE align_type = ALIGN_TYPE::HORIZONTAL_LEFT) {
                char buffer[WordBufferLength];
                value_to_string(value, dot_position, buffer, WordBufferLength);
                draw_string(surface, z_order, buffer, point, font, font_color, background_color, align_type);
            }

            static void draw_value_in_rect(
                    CoreSurface *surface,
                    CoreSurface::Z_ORDER_LEVEL z_order,
                    int value,
                    int dot_position,
                    CoreRect<CoreSurface::PixelPositionType> rect,
                    const CoreTheme::font_info *font,
                    ColorType font_color,
                    ColorType background_color,
                    ALIGN_TYPE align_type = ALIGN_TYPE::HORIZONTAL_LEFT) {
                char buffer[WordBufferLength];
                value_to_string(value, dot_position, buffer, WordBufferLength);
                draw_string_in_rect(surface, z_order, buffer, rect, font, font_color, background_color, align_type);
            }

            static void value_to_string(int value, int dot_position, char *buffer, int length) {
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

            static std::pair<CoreSurface::PixelPositionType, CoreSurface::PixelPositionType> get_str_size(const char *str, const CoreTheme::font_info *font) {
                if (str == nullptr || font == nullptr) {
                    return std::make_pair(0, 0);
                }
                auto lattice_width = 0;
                while (*str) {
                    unsigned int code;
                    str += get_utf8_code(str, code);
                    auto lattice = get_lattice(font, code);
                    lattice_width += lattice ? lattice->width : font->height;
                }

                return std::make_pair(lattice_width, font->height);
            }

        private:
            static int draw_single_char(
                    CoreSurface *surface,
                    CoreSurface::Z_ORDER_LEVEL z_order,
                    unsigned int utf8_code,
                    CorePoint<CoreSurface::PixelPositionType> point,
                    const CoreTheme::font_info *font,
                    ColorType font_color,
                    ColorType background_color) {
                auto error_color = static_cast<ColorType>(-1);
                if (font) {
                    const auto lattice = get_lattice(font, utf8_code);
                    if (lattice) {
                        draw_lattice(surface, z_order, point, lattice->width, font->height, lattice->pixel_gray_array, font_color, background_color);
                        return lattice->width;
                    }
                } else {
                    error_color = utility::build_rgb(255, 0, 0);
                }
                //lattice/font not found, draw "X"
                const int length = 16;
                auto [x, y] = point;
                for (auto _y = 0; _y < length; ++_y) {
                    for (auto _x = 0; _x < length; ++_x) {
                        auto diff = (_x - _y);
                        auto sum = (_x + _y);
                        if (utility::abs(diff) <= 1 || utility::abs(sum - length) <= 1) {
                            surface->draw_pixel({x + _x, y + _y}, error_color, z_order);
                        } else {
                            surface->draw_pixel({x + _x, y + _y}, 0, z_order);
                        }
                    }
                }

                return length;
            }

            static void draw_lattice(
                    CoreSurface *surface,
                    CoreSurface::Z_ORDER_LEVEL z_order,
                    CorePoint<CoreSurface::PixelPositionType> point,
                    int width, int height,
                    const unsigned char *data,
                    ColorType font_color,
                    ColorType background_color) {
                unsigned char block_value;
                unsigned char block_count;
                ColorType red;
                ColorType green;
                ColorType blue;
                ColorType rgb;

                auto load_new_block = [&]() {
                    block_value = *data++;
                    block_count = *data++;
                    red = (utility::get_red_from_argb(font_color) * block_value + utility::get_red_from_argb(background_color) * (255 - block_value)) >> 8;
                    green = (utility::get_green_from_argb(font_color) * block_value + utility::get_green_from_argb(background_color) * (255 - block_value)) >> 8;
                    blue = (utility::get_blue_from_argb(font_color) * block_value + utility::get_blue_from_argb(background_color) * (255 - block_value)) >> 8;
                    rgb = utility::build_rgb(red, green, blue);
                };

                auto [x, y] = point;
                load_new_block();
                for (auto _y = 0; _y < height; ++_y) {
                    for (auto _x = 0; _x < width; ++_x) {
                        GalAssert(block_count);
                        if (block_value == 0x00) {
                            if (utility::get_alpha_from_argb(background_color)) {
                                surface->draw_pixel({x + _x, y + _y}, background_color, z_order);
                            }
                        } else {
                            surface->draw_pixel({x + _x, y + _y}, rgb, z_order);
                        }
                        if (--block_count == 0) {
                            //reload new block
                            load_new_block();
                        }
                    }
                }
            }

            static const CoreTheme::font_info::lattice *get_lattice(const CoreTheme::font_info *font, unsigned int utf8_code) {
                auto last = font->count - 1;
                auto first = static_cast<decltype(last)>(0);
                auto middle = static_cast<decltype(last)>(first + last) / 2;

                while (first <= last) {
                    auto code = font->lattice_array[middle].utf8_code;
                    if (code < utf8_code) {
                        first = middle + 1;
                    } else if (code == utf8_code) {
                        return &font->lattice_array[middle];
                    } else {
                        last = middle - 1;
                    }
                    middle = (first + last) / 2;
                }

                return nullptr;
            }

            static std::pair<CoreSurface::PixelPositionType, CoreSurface::PixelPositionType> get_string_position(const char *str, const CoreTheme::font_info *font, CoreRect<CoreSurface::PixelPositionType> rect, ALIGN_TYPE align_type) {
                auto [x_size, y_size] = get_str_size(str, font);
                auto width = rect.get_width();
                auto height = rect.get_height();
                auto x = 0;
                auto y = 0;

                switch (align_type & ALIGN_TYPE::HORIZONTAL_MASK) {
                    case ALIGN_TYPE::HORIZONTAL_CENTER:
                        if (width > x_size) {
                            x = (width - x_size) / 2;
                        }
                        break;
                    case ALIGN_TYPE::HORIZONTAL_LEFT:
                        x = 0;
                        break;
                    case ALIGN_TYPE::HORIZONTAL_RIGHT:
                        if (width > x_size) {
                            x = width - x_size;
                        }
                        break;
                    default:
                        GalAssert(false);
                        break;
                }
                switch (align_type & ALIGN_TYPE::VERTICAL_MASK) {
                    case ALIGN_TYPE::VERTICAL_CENTER:
                        if (height > y_size) {
                            y = (height - y_size) / 2;
                        }
                        break;
                    case ALIGN_TYPE::VERTICAL_TOP:
                        y = 0;
                        break;
                    case ALIGN_TYPE::VERTICAL_BOTTOM:
                        if (height > y_size) {
                            y = height - y_size;
                        }
                        break;
                    default:
                        GalAssert(false);
                        break;
                }

                return std::make_pair(x, y);
            }

            static int get_utf8_code(const char *str, unsigned int &output_utf8_code) {
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
                auto p = reinterpret_cast<unsigned char *>(const_cast<char *>(str));
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

        class CoreBitmap {
        public:
            static void draw_bitmap(
                    CoreSurface *surface,
                    CoreSurface::Z_ORDER_LEVEL z_order,
                    const CoreTheme::bitmap_info *bitmap,
                    CorePoint<CoreSurface::PixelPositionType> point,
                    ColorType mask_rgb = DefaultColorMask) {
                GalAssert(bitmap);
                GalAssert(z_order >= CoreSurface::Z_ORDER_LEVEL::MIDDLE);

                auto layer = surface->m_layers[static_cast<size_t>(z_order) - 1];
                auto lower_framebuffer_rect = layer.rect;
                auto lower_framebuffer_width = lower_framebuffer_rect.get_width();

                auto mask_rgb_16 = utility::rgb_convert_32_to_16(mask_rgb);
                auto data = bitmap->pixel_color_array;

                auto [x, y] = point;
                for (auto _y = y, offset = 0; _y < y + bitmap->height; ++_y) {
                    for (auto _x = x; _x < x + bitmap->width; ++_x, ++offset) {
                        auto rgb = *(data + offset);
                        if (mask_rgb_16 == rgb) {
                            if (lower_framebuffer_rect.is_in_rect(_x, _y)) {
                                //show lower layer
                                auto index = lower_framebuffer_rect.distance_of_top(_y) * lower_framebuffer_width + lower_framebuffer_rect.distance_of_left(_x);
                                surface->draw_pixel({_x, _y}, CoreSurface::get_rgb_from_buffer(layer.framebuffer, index), z_order);
                            }
                        } else {
                            surface->draw_pixel({_x, _y}, utility::rgb_convert_16_to_32(rgb), z_order);
                        }
                    }
                }
            }

            static void draw_bitmap(
                    CoreSurface *surface,
                    CoreSurface::Z_ORDER_LEVEL z_order,
                    const CoreTheme::bitmap_info *bitmap,
                    CorePoint<CoreSurface::PixelPositionType> point,
                    CorePoint<CoreSurface::PixelPositionType> source_point,
                    CoreSurface::SurfaceSizeType width, CoreSurface::SurfaceSizeType height,
                    ColorType mask_rgb = DefaultColorMask) {
                auto [source_x, source_y] = source_point;
                if (bitmap == nullptr || (source_x + width > bitmap->width) || (source_y + height > bitmap->height)) { return; }

                GalAssert(z_order >= CoreSurface::Z_ORDER_LEVEL::MIDDLE);

                auto layer = surface->m_layers[static_cast<size_t>(z_order) - 1];
                auto lower_framebuffer_rect = layer.rect;
                auto lower_framebuffer_width = lower_framebuffer_rect.get_width();

                auto mask_rgb_16 = utility::rgb_convert_32_to_16(mask_rgb);
                auto data = bitmap->pixel_color_array;

                auto [x, y] = point;
                for (auto _y = y; _y < y + bitmap->height; ++_y) {
                    auto p = &data[(source_y + _y) * bitmap->width + source_x];
                    for (auto _x = x, offset = 0; _x < x + bitmap->width; ++_x, ++offset) {
                        auto rgb = *(p + offset);
                        if (mask_rgb_16 == rgb) {
                            if (lower_framebuffer_rect.is_in_rect(_x, _y)) {
                                //show lower layer
                                auto index = (y + lower_framebuffer_rect.distance_of_top(_y)) * lower_framebuffer_width + (x + lower_framebuffer_rect.distance_of_left(_x));
                                surface->draw_pixel({_x, _y}, CoreSurface::get_rgb_from_buffer(layer.framebuffer, index), z_order);
                            }
                        } else {
                            surface->draw_pixel({_x, _y}, utility::rgb_convert_16_to_32(rgb), z_order);
                        }
                    }
                }
            }
        };

        class CoreWindow {
        public:
            enum class WINDOW_ATTRIBUTION {
                VISIBLE = 0x40000000L,
                FOCUS = 0x20000000L,
                PRIORITY = 0x10000000L// Handle touch action at high priority
            };

            enum class WINDOW_STATE {
                NORMAL,
                PUSHED,
                FOCUSED,
                DISABLED
            };

            enum class NAVIGATION_KEY {
                FORWARD,
                BACKWARD,
                ENTER
            };

            enum class TOUCH_ACTION {
                DOWN,
                UP
            };

            enum class CONNECT_STATE {
                INVALID_ID = -1,
                INVALID_USER = -2,
                SUCCESS = 0
            };

            enum class UNLINK_STATE {
                INVALID_TOP_CHILD = -2,
                INVALID_CHILD = -1,
                NOT_MY_CHILD = -1,
                DO_NOTHING = 0,
                SUCCESS = 1
            };

            using IdType = unsigned short;
            constexpr static IdType s_id_not_used = IdType();

//            template<typename Enum, typename = std::enable_if_t<std::is_enum_v<Enum>>>
//            using WindowCallback = std::function<void(IdType, Enum)>;
            // to-do, wrap it
            using WindowCallback = void(CoreWindow::*)(IdType, const std::any&);

            struct window_tree {
                CoreWindow *window;//window instance
                IdType resource_id;//ID
                const char *str;   //caption
                CorePoint<CoreSurface::PixelPositionType> point;//position x && y
                CoreSurface::SurfaceSizeType width;
                CoreSurface::SurfaceSizeType height;
                window_tree *child_tree;//sub tree
            };

            CoreWindow()
                : m_state(WINDOW_STATE::NORMAL),
                  m_attribution(WINDOW_ATTRIBUTION::VISIBLE | WINDOW_ATTRIBUTION::FOCUS),
                  m_parent(nullptr),
                  m_top_child(nullptr),
                  m_prev_sibling(nullptr),
                  m_next_sibling(nullptr),
                  m_str(nullptr),
                  m_font_type(nullptr),
                  m_font_color(0),
                  m_background_color(0),
                  m_id(s_id_not_used),
                  m_z_order(CoreSurface::Z_ORDER_LEVEL::LOWEST),
                  m_focus_child(nullptr),
                  m_surface(nullptr){};

            virtual ~CoreWindow() = default;
            ;

            virtual CONNECT_STATE connect(
                    CoreWindow *parent,
                    IdType resource_id,
                    const char *str,
                    CorePoint<CoreSurface::PixelPositionType> point,
                    CoreSurface::SurfaceSizeType width, CoreSurface::SurfaceSizeType height,
                    window_tree *child_tree) {
                if (resource_id == s_id_not_used) {
                    GalAssert(false);
                    return CONNECT_STATE::INVALID_ID;
                }
                m_id = resource_id;
                set_str(str);
                m_parent = parent;
                m_state = WINDOW_STATE::NORMAL;
                if (parent) {
                    m_z_order = parent->m_z_order;
                    m_surface = parent->m_surface;
                }
                if (m_surface == nullptr) {
                    GalAssert(false);
                    return CONNECT_STATE::INVALID_USER;
                }
                /* (cs.x = x * 1024 / 768) for 1027*768=>800*600 quickly*/
                m_window_rect.set_rect(point.m_x, point.m_y, width, height);
                pre_create_window();
                if (parent != nullptr) {
                    parent->add_child_to_tail(this);
                }
                if (load_child_window(child_tree) >= 0) {
                    on_init_children();
                }
                return CONNECT_STATE::SUCCESS;
            }
            void disconnect() {
                if (m_id == s_id_not_used) { return; }

                if (m_top_child != nullptr) {
                    auto child = m_top_child;
                    while (child) {
                        auto next = child->m_next_sibling;
                        child->disconnect();
                        child = next;
                    }
                }
                if (m_parent != nullptr) {
                    m_parent->unlink_child(this);
                }
                m_focus_child = nullptr;
                m_id = s_id_not_used;
            }

            virtual void on_init_children(){};
            virtual void on_paint(){};
            virtual void show_window() {
                if (utility::EnumContains(m_attribution, WINDOW_ATTRIBUTION::VISIBLE)) {
                    on_paint();
                    auto child = m_top_child;
                    if (child != nullptr) {
                        while (child) {
                            child->show_window();
                            child = child->m_next_sibling;
                        }
                    }
                }
            }

            [[nodiscard]] IdType get_id() const { return m_id; }

            [[nodiscard]] CoreSurface::Z_ORDER_LEVEL get_z_order() const { return m_z_order; }

            [[nodiscard]] CoreWindow *get_window_by_id(IdType id) const {
                auto child = m_top_child;
                while (child) {
                    if (child->m_id == id) {
                        break;
                    }
                    child = child->m_next_sibling;
                }
                return child;
            }

            [[nodiscard]] WINDOW_ATTRIBUTION get_attribution() const { return m_attribution; }

            void set_str(const char *str) { m_str = str; }

            void set_attribution(WINDOW_ATTRIBUTION attribution) { m_attribution = attribution; }

            [[nodiscard]] bool is_focus_window() const {
                return utility::EnumContains(m_attribution, WINDOW_ATTRIBUTION::VISIBLE, WINDOW_ATTRIBUTION::FOCUS);
            }

            void set_font_color(ColorType color) { m_font_color = color; }

            [[nodiscard]] ColorType get_font_color() const { return m_font_color; }

            void set_background_color(ColorType color) { m_background_color = color; }

            [[nodiscard]] ColorType get_background_color() const { return m_background_color; }

            void set_font_type(const CoreTheme::font_info *font_type) { m_font_type = font_type; }

            [[nodiscard]] const CoreTheme::font_info *get_font_type() const { return m_font_type; }

            void set_window_position(CoreSurface::PixelPositionType x, CoreSurface::PixelPositionType y, CoreSurface::PixelPositionType width, CoreSurface::PixelPositionType height) {
                m_window_rect.set_rect(x, y, width, height);
            }

            [[nodiscard]] CoreRect<CoreSurface::PixelPositionType> get_window_rect() const { return m_window_rect; }

            [[nodiscard]] CoreRect<CoreSurface::PixelPositionType> get_screen_rect() const {
                auto [left, top] = window_to_screen();
                return {left, top, m_window_rect.get_width(), m_window_rect.get_height()};
            }

            CoreWindow *set_child_focus(CoreWindow *focus_child) {
                GalAssert(focus_child != nullptr);
                GalAssert(focus_child->m_parent == this);

                auto old_focus_child = m_focus_child;
                if (focus_child->is_focus_window()) {
                    if (focus_child != old_focus_child) {
                        if (old_focus_child) {
                            old_focus_child->on_blur();
                        }
                        m_focus_child = focus_child;
                        m_focus_child->on_focus();
                    }
                }
                return m_focus_child;
            }

            [[nodiscard]] CoreWindow *get_parent() const { return m_parent; }

            [[nodiscard]] CoreWindow *get_last_child() const {
                if (m_top_child == nullptr) { return nullptr; }

                auto child = m_top_child;
                while (child->m_next_sibling) {
                    child = child->m_next_sibling;
                }

                return child;
            }

            UNLINK_STATE unlink_child(CoreWindow *child) {
                if (child == nullptr) { return UNLINK_STATE::INVALID_CHILD; }
                if (this != child->m_parent) { return UNLINK_STATE::NOT_MY_CHILD; }
                if (m_top_child == nullptr) { return UNLINK_STATE::INVALID_TOP_CHILD; }

                bool found = false;
                if (child == m_top_child) {
                    m_top_child = child->m_next_sibling;
                    if (child->m_next_sibling != nullptr) {
                        child->m_next_sibling->m_prev_sibling = nullptr;
                    }
                    found = true;
                } else {
                    auto tmp_child = m_top_child;
                    while (tmp_child->m_next_sibling) {
                        if (child == tmp_child->m_next_sibling) {
                            tmp_child->m_next_sibling = child->m_next_sibling;
                            if (child->m_next_sibling != nullptr) {
                                child->m_next_sibling->m_prev_sibling = tmp_child;
                            }
                            found = true;
                            break;
                        }
                        tmp_child = tmp_child->m_next_sibling;
                    }
                }
                if (found) {
                    if (m_focus_child == child) {
                        m_focus_child = nullptr;
                    }
                    child->m_next_sibling = nullptr;
                    child->m_prev_sibling = nullptr;
                    return UNLINK_STATE::SUCCESS;
                }
                return UNLINK_STATE::DO_NOTHING;
            }

            [[nodiscard]] CoreWindow *get_prev_sibling() const { return m_prev_sibling; }

            [[nodiscard]] CoreWindow *get_next_sibling() const { return m_next_sibling; }

            virtual void on_touch(CorePoint<CoreSurface::PixelPositionType> point, TOUCH_ACTION action) {
                auto [x, y] = point;
                x = m_window_rect.distance_of_left(x);
                y = m_window_rect.distance_of_top(y);
                CoreWindow *priority_window = nullptr;
                CoreWindow *tmp_child = m_top_child;

                while (tmp_child) {
                    if (auto attr = tmp_child->m_attribution; utility::EnumContains(attr, WINDOW_ATTRIBUTION::PRIORITY, WINDOW_ATTRIBUTION::VISIBLE)) {
                        priority_window = tmp_child;
                        break;
                    }
                    tmp_child = tmp_child->m_next_sibling;
                }
                if (priority_window) {
                    return priority_window->on_touch({x, y}, action);
                }
                auto child = m_top_child;
                while (child) {
                    if (child->is_focus_window()) {
                        auto rect = child->get_window_rect();
                        if (rect.is_in_rect(x, y)) {
                            return child->on_touch({x, y}, action);
                        }
                    }
                    child = child->m_next_sibling;
                }
            }

            virtual void on_navigate(NAVIGATION_KEY key) {
                CoreWindow *priority_window = nullptr;
                auto tmp_child = m_top_child;
                while (tmp_child) {
                    if (auto attr = tmp_child->m_attribution; utility::EnumContains(attr, WINDOW_ATTRIBUTION::PRIORITY, WINDOW_ATTRIBUTION::VISIBLE)) {
                        priority_window = tmp_child;
                        break;
                    }
                    tmp_child = tmp_child->m_next_sibling;
                }
                if (priority_window) {
                    return priority_window->on_navigate(key);
                }
                if (!is_focus_window()) {
                    return;
                }
                if (key != NAVIGATION_KEY::BACKWARD && key != NAVIGATION_KEY::FORWARD) {
                    if (m_focus_child) {
                        m_focus_child->on_navigate(key);
                    }
                    return;
                }
                // Move focus
                auto old_focus_window = m_focus_child;
                // No current focus wnd, new one
                if (!old_focus_window) {
                    auto child = m_top_child;
                    while (child) {
                        if (child->is_focus_window()) {
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
                while (next_focus_window && (!next_focus_window->is_focus_window())) {
                    // Search neighbor of old focus window
                    next_focus_window = (key == NAVIGATION_KEY::FORWARD) ? next_focus_window->m_next_sibling : next_focus_window->m_prev_sibling;
                }
                if (!next_focus_window) {
                    // Search whole brother window
                    next_focus_window = (key == NAVIGATION_KEY::FORWARD) ? old_focus_window->m_parent->m_top_child : old_focus_window->m_parent->get_last_child();
                    while (next_focus_window && (!next_focus_window->is_focus_window())) {
                        next_focus_window = (key == NAVIGATION_KEY::FORWARD) ? next_focus_window->m_next_sibling : next_focus_window->m_prev_sibling;
                    }
                }
                if (next_focus_window) {
                    next_focus_window->m_parent->set_child_focus(next_focus_window);
                }
            }

            [[nodiscard]] CoreSurface *get_surface() const { return m_surface; }

            void set_surface(CoreSurface *surface) { m_surface = surface; }

        protected:
            virtual void pre_create_window(){};
            void add_child_to_tail(CoreWindow *child) {
                if (child == nullptr) { return; }
                if (child == get_window_by_id(child->m_id)) { return; }
                if (m_top_child == nullptr) {
                    m_top_child = child;
                    child->m_prev_sibling = nullptr;
                    child->m_next_sibling = nullptr;
                } else {
                    auto last_child = get_last_child();
                    if (last_child == nullptr) {
                        GalAssert(false);
                    }
                    last_child->m_next_sibling = child;
                    child->m_prev_sibling = last_child;
                    child->m_next_sibling = nullptr;
                }
            }

            [[nodiscard]] std::pair<CoreSurface::PixelPositionType, CoreSurface::PixelPositionType> window_to_screen() const {
                auto parent = m_parent;
                auto point = m_window_rect.m_left_top_corner;
                while (parent != nullptr) {
                    auto rect = parent->get_window_rect();
                    point += rect.m_left_top_corner;
                    parent = parent->m_parent;
                }

                return std::make_pair(point.m_x, point.m_y);
            }

            int load_child_window(window_tree *child_tree) {
                if (child_tree == nullptr) { return 0; }

                auto sum = 0;
                auto curr = child_tree;
                while (curr->window) {
                    if (curr->window->m_id != 0) {
                        //This wnd has been used! Do not share!
                        GalAssert(false);
                        return -1;
                    } else {
                        curr->window->connect(this, curr->resource_id, curr->str, curr->point, curr->width, curr->height, curr->child_tree);
                    }
                    ++curr;
                    ++sum;
                }

                return sum;
            }
            void set_active_child(CoreWindow *child) { m_focus_child = child; }
            virtual void on_focus(){};
            virtual void on_blur(){};

            IdType m_id;
            WINDOW_STATE m_state;
            WINDOW_ATTRIBUTION m_attribution;
            CoreRect<CoreSurface::PixelPositionType> m_window_rect;    //position relative to parent window
            CoreWindow *m_parent;      //parent window
            CoreWindow *m_top_child;   //the first sub window would be navigated
            CoreWindow *m_prev_sibling;//previous brother
            CoreWindow *m_next_sibling;//next brother
            CoreWindow *m_focus_child; //current focused window
            const char *m_str;         //caption
            const CoreTheme::font_info *m_font_type;
            ColorType m_font_color;
            ColorType m_background_color;
            CoreSurface::Z_ORDER_LEVEL m_z_order;
            CoreSurface *m_surface;
        };

        class CoreButton : public CoreWindow {
        public:
            void set_on_click(WindowCallback on_click) { this->m_on_click = on_click; }

        protected:
            void on_paint() override {
                auto rect = get_screen_rect();
                auto drawer = [surface = m_surface, z_oder = m_z_order,
                               str = m_str, rect = rect,
                               font_type = m_font_type, font_color = m_font_color](ColorType background_color) {
                    return CoreWord::draw_string_in_rect(
                            surface, z_oder,
                            str, rect,
                            font_type, font_color,
                            background_color,
                            CoreWord::ALIGN_TYPE::HORIZONTAL_CENTER | CoreWord::ALIGN_TYPE::VERTICAL_CENTER);
                };

                switch (m_state) {
                    case CoreWindow::WINDOW_STATE::NORMAL: {
                        auto color = CoreTheme::get_color(CoreTheme::COLOR_TYPE::WND_NORMAL);
                        m_surface->fill_rect(rect, color, m_z_order);
                        if (m_str) {
                            drawer(color);
                        }
                        break;
                    }
                    case CoreWindow::WINDOW_STATE::FOCUSED: {
                        auto color = CoreTheme::get_color(CoreTheme::COLOR_TYPE::WND_FOCUS);
                        m_surface->fill_rect(rect, color, m_z_order);
                        if (m_str) {
                            drawer(color);
                        }
                        break;
                    }
                    case CoreWindow::WINDOW_STATE::PUSHED: {
                        auto color = CoreTheme::get_color(CoreTheme::COLOR_TYPE::WND_PUSHED);
                        m_surface->fill_rect(rect, color, m_z_order);
                        m_surface->draw_rect(rect, CoreTheme::get_color(CoreTheme::COLOR_TYPE::WND_BORDER), m_z_order, 2);
                        if (m_str) {
                            drawer(color);
                        }
                        break;
                    }
                    default:
                        GalAssert(false);
                        break;
                }
            }

            void on_focus() override {
                m_state = CoreWindow::WINDOW_STATE::FOCUSED;
                on_paint();
            }

            void on_blur() override {
                m_state = CoreWindow::WINDOW_STATE::NORMAL;
                on_paint();
            }

            void pre_create_window() override {
                m_on_click = nullptr;
                m_attribution = WINDOW_ATTRIBUTION::VISIBLE | WINDOW_ATTRIBUTION::FOCUS;
                m_font_type = CoreTheme::get_font(CoreTheme::FONT_TYPE::DEFAULT);
                m_font_color = CoreTheme::get_color(CoreTheme::COLOR_TYPE::WND_FONT);
            }

            void on_touch(CorePoint<CoreSurface::PixelPositionType> point, TOUCH_ACTION action) override {
                if (action == CoreWindow::TOUCH_ACTION::DOWN) {
                    m_parent->set_child_focus(this);
                    m_state = CoreWindow::WINDOW_STATE::PUSHED;
                    on_paint();
                } else {
                    m_state = CoreWindow::WINDOW_STATE::FOCUSED;
                    on_paint();
                    if (m_on_click) {
                        (m_parent->*m_on_click)(m_id, 0);
                    }
                }
            }

            void on_navigate(NAVIGATION_KEY key) override {
                switch (key) {
                    case CoreWindow::NAVIGATION_KEY::ENTER: {
                        auto [x, y] = m_window_rect.m_left_top_corner;
                        on_touch({x, y}, CoreWindow::TOUCH_ACTION::DOWN);
                        on_touch({x, y}, CoreWindow::TOUCH_ACTION::UP);
                    }
                    case CoreWindow::NAVIGATION_KEY::FORWARD:
                    case CoreWindow::NAVIGATION_KEY::BACKWARD:
                        break;
                }
                return CoreWindow::on_navigate(key);
            }

            WindowCallback m_on_click{};
        };

        class CoreDialog : public CoreWindow {
        public:
            enum class OPEN_DIALOG_STATE {
                FAILED = 0,
                SUCCESS = 1
            };

            enum class CLOSE_DIALOG_STATE {
                FAILED = -1,
                DO_NOTHING = 0,
                SUCCESS = 1
            };

            enum class SET_DIALOG_STATE {
                FAILED = -1,
                SET_DIALOG = 0,
                SET_DIALOG_AND_SURFACE = 1
            };

            struct dialog_array {
                CoreDialog *dialog;
                CoreSurface *surface;
            };

            static OPEN_DIALOG_STATE open_dialog(CoreDialog *dialog, bool modal = true) {
                if (dialog == nullptr) {
                    GalAssert(false);
                    return OPEN_DIALOG_STATE::FAILED;
                }
                auto curr = get_dialog(dialog->get_surface());
                if (curr == dialog) {
                    return OPEN_DIALOG_STATE::SUCCESS;
                }
                if (curr) {
                    curr->set_attribution(static_cast<WINDOW_ATTRIBUTION>(0));
                }
                auto mode = WINDOW_ATTRIBUTION::FOCUS | WINDOW_ATTRIBUTION::VISIBLE;
                dialog->set_attribution(modal ? (mode | WINDOW_ATTRIBUTION::PRIORITY) : static_cast<WINDOW_ATTRIBUTION>(mode));
                dialog->show_window();
                dialog->set_dialog();
                return OPEN_DIALOG_STATE::SUCCESS;
            }

            static CLOSE_DIALOG_STATE close_dialog(CoreSurface *surface) {
                auto dialog = get_dialog(surface);
                if (dialog == nullptr) { return CLOSE_DIALOG_STATE::DO_NOTHING; }

                auto rect = dialog->get_screen_rect();
                dialog->set_attribution(static_cast<WINDOW_ATTRIBUTION>(0));
                surface->show_layer(rect, static_cast<CoreSurface::Z_ORDER_LEVEL>(static_cast<std::underlying_type_t<CoreSurface::Z_ORDER_LEVEL>>(dialog->m_z_order) - 1));

                // clear dialog
                for (auto &each : s_dialogs) {
                    if (each.surface == surface) {
                        each.dialog = nullptr;
                        return CLOSE_DIALOG_STATE::SUCCESS;
                    }
                }

                GalAssert(false);
                return CLOSE_DIALOG_STATE::FAILED;
            }

            static CoreDialog *get_dialog(CoreSurface *surface) {
                for (auto &dialog : s_dialogs) {
                    if (dialog.surface == surface) {
                        return dialog.dialog;
                    }
                }

                return nullptr;
            }

        protected:
            void pre_create_window() override {
                // No focus/visible
                m_attribution = static_cast<WINDOW_ATTRIBUTION>(0);
                m_z_order = CoreSurface::Z_ORDER_LEVEL::MIDDLE;
                m_background_color = utility::build_rgb(33, 42, 53);
            }

            void on_paint() override {
                auto rect = get_screen_rect();
                m_surface->fill_rect(rect, m_background_color, m_z_order);
                if (m_str) {
                    auto point = rect.m_left_top_corner;
                    point.horizontal_move(35);
                    CoreWord::draw_string(
                            m_surface, m_z_order,
                            m_str,
                            point,
                            CoreTheme::get_font(CoreTheme::FONT_TYPE::DEFAULT),
                            utility::build_rgb(255, 255, 255),
                            utility::build_argb(0, 0, 0, 0),
                            CoreWord::ALIGN_TYPE::HORIZONTAL_LEFT);
                }
            }

        private:
            SET_DIALOG_STATE set_dialog() {
                auto surface = get_surface();
                for (auto &dialog : s_dialogs) {
                    if (dialog.surface == surface) {
                        dialog.dialog = this;
                        return SET_DIALOG_STATE::SET_DIALOG;
                    }
                }
                for (auto &dialog : s_dialogs) {
                    if (dialog.surface == nullptr) {
                        dialog.dialog = this;
                        dialog.surface = surface;
                        return SET_DIALOG_STATE::SET_DIALOG_AND_SURFACE;
                    }
                }

                GalAssert(false);
                return SET_DIALOG_STATE::FAILED;
            }

            static std::array<dialog_array, SurfaceCountMax> s_dialogs;
        };
    }

    using KeyboardSizeType = unsigned short;
    using KeyboardIdType = unsigned short;
    using KeyboardCaptionType = const char*;

    //Changing key width/height will change the get_width/get_height of keyboard
    constexpr KeyboardSizeType KeyWidth = 65;
    constexpr KeyboardSizeType KeyHeight = 38;
    constexpr KeyboardSizeType KeyboardWidth = (KeyWidth + 2) * 10;
    constexpr KeyboardSizeType KeyboardHeight = (KeyHeight + 2) * 4;
    constexpr KeyboardSizeType NumpadWidth = (KeyWidth + 2) * 4;
    constexpr KeyboardSizeType NumpadHeight = (KeyHeight + 2) * 4;
    constexpr KeyboardSizeType CapsWidth = KeyWidth * 3 / 2;
    constexpr KeyboardSizeType DelWidth = KeyWidth * 3 / 2 + 1;
    constexpr KeyboardSizeType EscWidth = KeyWidth * 2 + 2;
    constexpr KeyboardSizeType SwitchWidth = KeyWidth * 3 / 2;
    constexpr KeyboardSizeType SpaceWidth = KeyWidth * 3 + 2 * 2;
    constexpr KeyboardSizeType DotWidth = KeyWidth * 3 / 2 + 3;
    constexpr KeyboardSizeType EnterWidth = KeyWidth * 2 + 2;

    constexpr KeyboardIdType Num1Id = '1';
    constexpr KeyboardIdType Num2Id = '2';
    constexpr KeyboardIdType Num3Id = '3';
    constexpr KeyboardIdType Num4Id = '4';
    constexpr KeyboardIdType Num5Id = '5';
    constexpr KeyboardIdType Num6Id = '6';
    constexpr KeyboardIdType Num7Id = '7';
    constexpr KeyboardIdType Num8Id = '8';
    constexpr KeyboardIdType Num9Id = '9';
    constexpr KeyboardIdType Num0Id = '0';

    constexpr KeyboardIdType CharQId = 'Q';
    constexpr KeyboardIdType CharWId = 'W';
    constexpr KeyboardIdType CharEId = 'E';
    constexpr KeyboardIdType CharRId = 'R';
    constexpr KeyboardIdType CharTId = 'T';
    constexpr KeyboardIdType CharYId = 'Y';
    constexpr KeyboardIdType CharUId = 'U';
    constexpr KeyboardIdType CharIId = 'I';
    constexpr KeyboardIdType CharOId = 'O';
    constexpr KeyboardIdType CharPId = 'P';

    constexpr KeyboardIdType CharAId = 'A';
    constexpr KeyboardIdType CharSId = 'S';
    constexpr KeyboardIdType CharDId = 'D';
    constexpr KeyboardIdType CharFId = 'F';
    constexpr KeyboardIdType CharGId = 'G';
    constexpr KeyboardIdType CharHId = 'H';
    constexpr KeyboardIdType CharJId = 'J';
    constexpr KeyboardIdType CharKId = 'K';
    constexpr KeyboardIdType CharLId = 'L';

    constexpr KeyboardIdType CharZId = 'Z';
    constexpr KeyboardIdType CharXId = 'X';
    constexpr KeyboardIdType CharCId = 'C';
    constexpr KeyboardIdType CharVId = 'V';
    constexpr KeyboardIdType CharBId = 'B';
    constexpr KeyboardIdType CharNId = 'N';
    constexpr KeyboardIdType CharMId = 'M';

    constexpr KeyboardIdType EscId = 0x1B;
    constexpr KeyboardIdType DotId = '.';
    constexpr KeyboardIdType DelId = 0x7F;
    constexpr KeyboardIdType EnterId = '\n';
    constexpr KeyboardIdType NumSwitchId = 0x90;
    constexpr KeyboardIdType CapsId = 0x14;
    constexpr KeyboardIdType SpaceId = ' ';
    constexpr KeyboardIdType BackId = 0x7F;
    constexpr KeyboardIdType ArrowUpId = 0x1111;
    constexpr KeyboardIdType ArrowDownId = 0x2222;

    constexpr KeyboardCaptionType EscChar = "Esc";
    constexpr KeyboardCaptionType DotChar = ".";
    constexpr KeyboardCaptionType DelChar = "Del";
    constexpr KeyboardCaptionType EnterChar = "Enter";
    constexpr KeyboardCaptionType NumSwitchChar = "?123";
    constexpr KeyboardCaptionType CapsChar = "Caps";
    constexpr KeyboardCaptionType SpaceChar = "Space";
    constexpr KeyboardCaptionType BackChar = "Back";
    constexpr KeyboardCaptionType ArrowUpChar = "+";
    constexpr KeyboardCaptionType ArrowDownChar = "-";

    namespace utility{
        constexpr KeyboardSizeType get_x_in_keyboard(KeyboardSizeType column)
        {
            return (KeyWidth * column) + (column + 1) * 2;
        }

        constexpr KeyboardSizeType get_y_in_keyboard(KeyboardSizeType row)
        {
            return (KeyHeight * row) + (row + 1) * 2;
        }
    }

    namespace detail{

        class CoreKeyboardButton;
        class CoreKeyboard : public CoreWindow
        {
        public:
            enum class KEYBOARD_CASE
            {
                UPPERCASE,
                LOWERCASE
            };

            enum class KEYBOARD_BOARD
            {
                ALL,
                NUM_ONLY
            };

            enum class KEYBOARD_CLICK
            {
                CHARACTER,
                ENTER,
                ESCAPE
            };

            virtual CONNECT_STATE connect(CoreWindow* user, IdType resource_id, KEYBOARD_BOARD style);

            void on_init_children() override
            {
                auto child = m_top_child;
                if (child != nullptr)
                {
                    while(child)
                    {
                        dynamic_cast<CoreButton*>(child)->set_on_click(static_cast<WindowCallback>(&CoreKeyboard::on_key_clicked));
                        child = child->get_next_sibling();
                    }
                }
            }

            [[nodiscard]] KEYBOARD_CASE get_cap_state() const {return m_cap_state;}

            [[nodiscard]] const char *get_str() const {return m_str;}

            void set_on_click(WindowCallback on_click) {m_on_click = on_click;}

            static CoreKeyboardButton s_key_0, s_key_1, s_key_2, s_key_3, s_key_4, s_key_5, s_key_6, s_key_7, s_key_8, s_key_9;
            static CoreKeyboardButton s_key_a, s_key_b, s_key_c, s_key_d, s_key_e, s_key_f, s_key_g, s_key_h, s_key_i, s_key_j;
            static CoreKeyboardButton s_key_k, s_key_l, s_key_m, s_key_n, s_key_o, s_key_p, s_key_q, s_key_r, s_key_s, s_key_t;
            static CoreKeyboardButton s_key_u, s_key_v, s_key_w, s_key_x, s_key_y, s_key_z;
            static CoreKeyboardButton s_key_dot, s_key_caps, s_key_space, s_key_enter, s_key_del, s_key_esc, s_key_num_switch;
            static CoreWindow::window_tree s_keyboard_tree[34];
            static CoreWindow::window_tree s_numpad_tree[15];


        protected:
            void pre_create_window() override
            {
                m_attribution = static_cast<WINDOW_ATTRIBUTION>(WINDOW_ATTRIBUTION::VISIBLE | WINDOW_ATTRIBUTION::FOCUS);
                m_cap_state = KEYBOARD_CASE::UPPERCASE;
                std::memset(m_str, 0, sizeof(m_str));
                m_str_length = 0;
            }

            void on_paint() override
            {
                auto rect = get_screen_rect();
                m_surface->fill_rect(rect, utility::build_rgb(0, 0, 0), m_z_order);
            }

            void on_key_clicked(IdType id, const std::any& param)
            {
                switch(id)
                {
                    case CapsId:
                        on_caps_clicked(id, std::forward<const std::any&>(param));
                        break;
                    case EnterId:
                        on_enter_clicked(id, std::forward<const std::any&>(param));
                        break;
                    case EscId:
                        on_esc_clicked(id, std::forward<const std::any&>(param));
                        break;
                    case DelId:
                        on_del_clicked(id, std::forward<const std::any&>(param));
                        break;
                    default:
                        on_char_clicked(id, std::forward<const std::any&>(param));
                        break;
                }
            }

            void on_char_clicked(IdType id, const std::any& param)
            {
                //id = char ascii code
                if(m_str_length >= sizeof(m_str)) {return;}

                auto input_char = [&]()
                {
                  m_str[m_str_length++] = id;
                  (m_parent->*m_on_click)(m_id, KEYBOARD_CLICK::CHARACTER);
                };
                if((id >= Num0Id && id <= Num9Id) || id == SpaceId || id == DotId)
                {
                    input_char();
                }
                if(id >= CharAId && id <= CharZId)
                {
                    if(m_cap_state == KEYBOARD_CASE::LOWERCASE)
                    {
                        id += 0x20;
                    }
                    input_char();
                }
                GalAssert(false);
            }

            void on_del_clicked(IdType id, const std::any& param)
            {
                if(m_str_length <= 0) {return;}

                m_str[--m_str_length] = 0;
                (m_parent->*m_on_click)(m_id, KEYBOARD_CLICK::CHARACTER);
            }

            void on_caps_clicked(IdType id, const std::any& param)
            {
                m_cap_state = (m_cap_state == KEYBOARD_CASE::LOWERCASE) ? KEYBOARD_CASE::UPPERCASE : KEYBOARD_CASE::LOWERCASE;
                show_window();
            }

            void on_enter_clicked(IdType id, const std::any& param)
            {
                std::memset(m_str, 0, sizeof(m_str));
                (m_parent->*m_on_click)(m_id, KEYBOARD_CLICK::ENTER);
            }

            void on_esc_clicked(IdType id, const std::any& param)
            {
                std::memset(m_str, 0, sizeof(m_str));
                (m_parent->*m_on_click)(m_id, KEYBOARD_CLICK::ESCAPE);
            }

        private:
            char m_str[32];
            int m_str_length;
            KEYBOARD_CASE m_cap_state;
            WindowCallback m_on_click;
        };

        class CoreKeyboardButton : public CoreButton
        {
        protected:
            void on_paint() override
            {
                auto rect = get_screen_rect();
                switch (m_state) {
                    case CoreWindow::WINDOW_STATE::NORMAL:
                        m_surface->fill_rect(rect, CoreTheme::get_color(CoreTheme::COLOR_TYPE::WND_NORMAL), m_z_order);
                        break;
                    case CoreWindow::WINDOW_STATE::FOCUSED:
                        m_surface->fill_rect(rect, CoreTheme::get_color(CoreTheme::COLOR_TYPE::WND_FOCUS), m_z_order);
                        break;
                    case CoreWindow::WINDOW_STATE::PUSHED:
                        m_surface->fill_rect(rect, CoreTheme::get_color(CoreTheme::COLOR_TYPE::WND_PUSHED), m_z_order);
                        m_surface->draw_rect(rect, CoreTheme::get_color(CoreTheme::COLOR_TYPE::WND_BORDER), m_z_order, 2);
                        break;
                    default:
                        GalAssert(false);
                        break;
                }

                auto drawer = [surface = m_surface, z_order = m_z_order,
                               rect = rect, font_type = m_font_type,
                               font_color = m_font_color,
                               background_color = utility::build_argb(0, 0, 0, 0)](KeyboardCaptionType str)
                {
                    return CoreWord::draw_string_in_rect(surface, z_order, str, rect, font_type, font_color, background_color);
                };

                if(m_id == CapsId)
                {
                    return drawer(CapsChar);
                }
                else if(m_id == EscId)
                {
                    return drawer(EscChar);
                }
                else if(m_id == SpaceId)
                {
                    return drawer(SpaceChar);
                }
                else if(m_id == EnterId)
                {
                    return drawer(EnterChar);
                }
                else if(m_id == DotId)
                {
                    return drawer(DotChar);
                }
                else if(m_id == BackId)
                {
                    return drawer(BackChar);
                }
                else if(m_id == NumSwitchId)
                {
                    return drawer(NumSwitchChar);
                }

                IdType letter[] = {0, 0};
                if(m_id >= CharAId && m_id <= CharZId)
                {
                    letter[0] = dynamic_cast<CoreKeyboard *>(m_parent)->get_cap_state() == CoreKeyboard::KEYBOARD_CASE::UPPERCASE ? m_id : (m_id + 0x20);
                }
                else if(m_id >= Num0Id && m_id <= Num9Id)
                {
                    letter[0] = m_id;
                }
                CoreWord::draw_string_in_rect(m_surface, m_z_order, reinterpret_cast<const char*>(letter), rect, m_font_type, m_font_color, utility::build_argb(0, 0, 0, 0));
            }
        };

//        CoreWindow::window_tree CoreKeyboard::s_keyboard_tree[] = /* NOLINT */
//        {
//                //Row 1
//                {&CoreKeyboard::s_key_q, 'Q', nullptr, {utility::get_x_in_keyboard(0), utility::get_y_in_keyboard(0)}, KeyWidth, KeyHeight},
//                {&CoreKeyboard::s_key_w, 'W', nullptr, {utility::get_x_in_keyboard(1), utility::get_y_in_keyboard(0)}, KeyWidth, KeyHeight},
//                {&CoreKeyboard::s_key_e, 'E', nullptr, {utility::get_x_in_keyboard(2), utility::get_y_in_keyboard(0)}, KeyWidth, KeyHeight},
//                {&CoreKeyboard::s_key_r, 'R', nullptr, {utility::get_x_in_keyboard(3), utility::get_y_in_keyboard(0)}, KeyWidth, KeyHeight},
//                {&CoreKeyboard::s_key_t, 'T', nullptr, {utility::get_x_in_keyboard(4), utility::get_y_in_keyboard(0)}, KeyWidth, KeyHeight},
//                {&CoreKeyboard::s_key_y, 'Y', nullptr, {utility::get_x_in_keyboard(5), utility::get_y_in_keyboard(0)}, KeyWidth, KeyHeight},
//                {&CoreKeyboard::s_key_u, 'U', nullptr, {utility::get_x_in_keyboard(6), utility::get_y_in_keyboard(0)}, KeyWidth, KeyHeight},
//                {&CoreKeyboard::s_key_i, 'I', nullptr, {utility::get_x_in_keyboard(7), utility::get_y_in_keyboard(0)}, KeyWidth, KeyHeight},
//                {&CoreKeyboard::s_key_o, 'O', nullptr, {utility::get_x_in_keyboard(8), utility::get_y_in_keyboard(0)}, KeyWidth, KeyHeight},
//                {&CoreKeyboard::s_key_p, 'P', nullptr, {utility::get_x_in_keyboard(9), utility::get_y_in_keyboard(0)}, KeyWidth, KeyHeight},
//                //Row 2
//                {&CoreKeyboard::s_key_a, 'A', nullptr, {utility::get_x_in_keyboard(0) + (KeyWidth / 2), utility::get_y_in_keyboard(1)}, KeyWidth, KeyHeight},
//                {&CoreKeyboard::s_key_s, 'S', nullptr, {utility::get_x_in_keyboard(1) + (KeyWidth / 2), utility::get_y_in_keyboard(1)}, KeyWidth, KeyHeight},
//                {&CoreKeyboard::s_key_d, 'D', nullptr, {utility::get_x_in_keyboard(2) + (KeyWidth / 2), utility::get_y_in_keyboard(1)}, KeyWidth, KeyHeight},
//                {&CoreKeyboard::s_key_f, 'F', nullptr, {utility::get_x_in_keyboard(3) + (KeyWidth / 2), utility::get_y_in_keyboard(1)}, KeyWidth, KeyHeight},
//                {&CoreKeyboard::s_key_g, 'G', nullptr, {utility::get_x_in_keyboard(4) + (KeyWidth / 2), utility::get_y_in_keyboard(1)}, KeyWidth, KeyHeight},
//                {&CoreKeyboard::s_key_h, 'H', nullptr, {utility::get_x_in_keyboard(5) + (KeyWidth / 2), utility::get_y_in_keyboard(1)}, KeyWidth, KeyHeight},
//                {&CoreKeyboard::s_key_j, 'J', nullptr, {utility::get_x_in_keyboard(6) + (KeyWidth / 2), utility::get_y_in_keyboard(1)}, KeyWidth, KeyHeight},
//                {&CoreKeyboard::s_key_k, 'K', nullptr, {utility::get_x_in_keyboard(7) + (KeyWidth / 2), utility::get_y_in_keyboard(1)}, KeyWidth, KeyHeight},
//                {&CoreKeyboard::s_key_l, 'L', nullptr, {utility::get_x_in_keyboard(8) + (KeyWidth / 2), utility::get_y_in_keyboard(1)}, KeyWidth, KeyHeight},
//                //Row 3
//                {&CoreKeyboard::s_key_caps, CapsId, nullptr, {utility::get_x_in_keyboard(0), utility::get_y_in_keyboard(2)}, CapsWidth, KeyHeight},
//                {&CoreKeyboard::s_key_z, 'Z', nullptr, {utility::get_x_in_keyboard(1) + (KeyWidth / 2), utility::get_y_in_keyboard(2)}, KeyWidth, KeyHeight},
//                {&CoreKeyboard::s_key_x, 'X', nullptr, {utility::get_x_in_keyboard(2) + (KeyWidth / 2), utility::get_y_in_keyboard(2)}, KeyWidth, KeyHeight},
//                {&CoreKeyboard::s_key_c, 'C', nullptr, {utility::get_x_in_keyboard(3) + (KeyWidth / 2), utility::get_y_in_keyboard(2)}, KeyWidth, KeyHeight},
//                {&CoreKeyboard::s_key_v, 'V', nullptr, {utility::get_x_in_keyboard(4) + (KeyWidth / 2), utility::get_y_in_keyboard(2)}, KeyWidth, KeyHeight},
//                {&CoreKeyboard::s_key_b, 'B', nullptr, {utility::get_x_in_keyboard(5) + (KeyWidth / 2), utility::get_y_in_keyboard(2)}, KeyWidth, KeyHeight},
//                {&CoreKeyboard::s_key_n, 'N', nullptr, {utility::get_x_in_keyboard(6) + (KeyWidth / 2), utility::get_y_in_keyboard(2)}, KeyWidth, KeyHeight},
//                {&CoreKeyboard::s_key_m, 'M', nullptr, {utility::get_x_in_keyboard(7) + (KeyWidth / 2), utility::get_y_in_keyboard(2)}, KeyWidth, KeyHeight},
//                {&CoreKeyboard::s_key_del, DelId, nullptr, {utility::get_x_in_keyboard(8) + (KeyWidth / 2), utility::get_y_in_keyboard(2)}, DelWidth, KeyHeight},
//                //Row 4
//                {&CoreKeyboard::s_key_esc, EscId, nullptr, {utility::get_x_in_keyboard(0), utility::get_y_in_keyboard(3)}, EscWidth, KeyHeight},
//                {&CoreKeyboard::s_key_num_switch, NumSwitchId, nullptr, {utility::get_x_in_keyboard(2), utility::get_y_in_keyboard(3)}, SwitchWidth, KeyHeight},
//                {&CoreKeyboard::s_key_space, SpaceId, nullptr, {utility::get_x_in_keyboard(3) + (KeyWidth / 2), utility::get_y_in_keyboard(3)}, SpaceWidth, KeyHeight},
//                {&CoreKeyboard::s_key_dot, DotId, nullptr, {utility::get_x_in_keyboard(6) + (KeyWidth / 2), utility::get_y_in_keyboard(3)}, DotWidth, KeyHeight},
//                {&CoreKeyboard::s_key_enter, EnterId, nullptr, {utility::get_x_in_keyboard(8), utility::get_y_in_keyboard(3)}, EnterWidth, KeyHeight},
//                {nullptr, 0, nullptr, {0, 0}, 0, 0}
//        };
//        CoreWindow::window_tree CoreKeyboard::s_numpad_tree[] = /* NOLINT */
//        {
//                {&CoreKeyboard::s_key_1, '1', nullptr, {utility::get_x_in_keyboard(0), utility::get_y_in_keyboard(0)}, KeyWidth, KeyWidth},
//                {&CoreKeyboard::s_key_2, '2', nullptr, {utility::get_x_in_keyboard(1), utility::get_y_in_keyboard(0)}, KeyWidth, KeyWidth},
//                {&CoreKeyboard::s_key_3, '3', nullptr, {utility::get_x_in_keyboard(2), utility::get_y_in_keyboard(0)}, KeyWidth, KeyWidth},
//                {&CoreKeyboard::s_key_4, '4', nullptr, {utility::get_x_in_keyboard(0), utility::get_y_in_keyboard(1)}, KeyWidth, KeyWidth},
//                {&CoreKeyboard::s_key_5, '5', nullptr, {utility::get_x_in_keyboard(1), utility::get_y_in_keyboard(1)}, KeyWidth, KeyWidth},
//                {&CoreKeyboard::s_key_6, '6', nullptr, {utility::get_x_in_keyboard(2), utility::get_y_in_keyboard(1)}, KeyWidth, KeyWidth},
//                {&CoreKeyboard::s_key_7, '7', nullptr, {utility::get_x_in_keyboard(0), utility::get_y_in_keyboard(2)}, KeyWidth, KeyWidth},
//                {&CoreKeyboard::s_key_8, '8', nullptr, {utility::get_x_in_keyboard(1), utility::get_y_in_keyboard(2)}, KeyWidth, KeyWidth},
//                {&CoreKeyboard::s_key_9, '9', nullptr, {utility::get_x_in_keyboard(2), utility::get_y_in_keyboard(2)}, KeyWidth, KeyWidth},
//
//                {&CoreKeyboard::s_key_esc, EscId, nullptr, {utility::get_x_in_keyboard(0), utility::get_y_in_keyboard(3)}, KeyWidth, KeyHeight},
//                {&CoreKeyboard::s_key_0, '0', nullptr, {utility::get_x_in_keyboard(1), utility::get_y_in_keyboard(3)}, KeyWidth, KeyHeight},
//                {&CoreKeyboard::s_key_dot, DotId, nullptr, {utility::get_x_in_keyboard(2), utility::get_y_in_keyboard(3)}, KeyWidth, KeyHeight},
//                {&CoreKeyboard::s_key_del, DelId, nullptr, {utility::get_x_in_keyboard(3), utility::get_y_in_keyboard(0)}, KeyWidth, KeyHeight * 2 + 2},
//                {&CoreKeyboard::s_key_enter, EnterId, nullptr, {utility::get_x_in_keyboard(3), utility::get_y_in_keyboard(2)}, KeyWidth, KeyHeight * 2 + 2},
//                {nullptr, 0, nullptr, {0, 0}, 0, 0}
//        };

        CoreKeyboard::CONNECT_STATE CoreKeyboard::connect(CoreWindow *user, IdType resource_id, KEYBOARD_BOARD style)
        {
            if(!user) {return CoreWindow::CONNECT_STATE::INVALID_USER;}

            auto rect = user->get_window_rect();
            if(style == KEYBOARD_BOARD::ALL)
            {
                //Place keyboard at the bottom of user's parent window
                auto parent_rect = user->get_parent()->get_window_rect();
                return CoreWindow::connect(
                        user, resource_id,
                        nullptr,
                        {rect.distance_of_left(0), rect.distance_of_top(parent_rect.get_height() - KeyboardHeight - 1)},
                        KeyboardWidth, KeyboardHeight,
                        nullptr/*CoreKeyboard::s_keyboard_tree*/);
            }
            else if(style == KEYBOARD_BOARD::NUM_ONLY)
            {
                //Place keyboard below the user window.
                return CoreWindow::connect(user, resource_id, nullptr, {0, rect.get_height()}, NumpadWidth, NumpadHeight, nullptr/*CoreKeyboard::s_numpad_tree*/);
            }
            else
            {
                GalAssert(false);
            }
            return CONNECT_STATE::INVALID_ID;
        }
    }

    constexpr auto MaxEditStringLength = 32;
    constexpr auto EditKeyboardId = 0x1;
    constexpr auto ListboxItemHeight = 45;

    namespace detail {
        class CoreEdit : public CoreWindow {
        public:
            [[nodiscard]] const char *get_text() const { return m_str; }

            void set_text(const char *str) {
                if (str != nullptr && std::strlen(str) < sizeof(m_str)) {
                    std::strcpy(m_str, str);
                }
            }

            void set_keyboard_style(CoreKeyboard::KEYBOARD_BOARD style) { m_keyboard_style = style; }

        protected:
            void pre_create_window() override {
                m_attribution = CoreWindow::WINDOW_ATTRIBUTION::VISIBLE | CoreWindow::WINDOW_ATTRIBUTION::FOCUS;
                m_keyboard_style = CoreKeyboard::KEYBOARD_BOARD::ALL;
                m_font_type = CoreTheme::get_font(CoreTheme::FONT_TYPE::DEFAULT);
                m_font_color = CoreTheme::get_color(CoreTheme::COLOR_TYPE::WND_FONT);
                std::memset(m_str_input, 0, sizeof(m_str_input));
                std::memset(m_str, 0, sizeof(m_str));
                set_text(CoreWindow::m_str);
            }

            void on_paint() override {
                auto rect = get_screen_rect();
                auto keyboard_rect = s_keyboard.get_screen_rect();
                switch (m_state) {
                    case CoreWindow::WINDOW_STATE::NORMAL: {
                        auto z_order = m_parent->get_z_order();
                        if (m_z_order > z_order) {
                            s_keyboard.disconnect();
                            m_z_order = z_order;
                            m_surface->show_layer(keyboard_rect, m_z_order);
                            m_attribution = CoreWindow::WINDOW_ATTRIBUTION::VISIBLE | CoreWindow::WINDOW_ATTRIBUTION::FOCUS;
                        }
                        m_surface->fill_rect(rect, CoreTheme::get_color(CoreTheme::COLOR_TYPE::WND_NORMAL), m_z_order);
                        CoreWord::draw_string_in_rect(m_surface, z_order, m_str, rect, m_font_type, m_font_color, CoreTheme::get_color(CoreTheme::COLOR_TYPE::WND_NORMAL), CoreWord::ALIGN_TYPE::HORIZONTAL_CENTER | CoreWord::ALIGN_TYPE::VERTICAL_CENTER);
                        break;
                    }
                    case CoreWindow::WINDOW_STATE::FOCUSED: {
                        auto z_order = m_parent->get_z_order();
                        if (m_z_order > z_order) {
                            s_keyboard.disconnect();
                            m_z_order = z_order;
                            m_surface->show_layer(keyboard_rect, m_z_order);
                            m_attribution = CoreWindow::WINDOW_ATTRIBUTION::VISIBLE | CoreWindow::WINDOW_ATTRIBUTION::FOCUS;
                        }
                        m_surface->fill_rect(rect, CoreTheme::get_color(CoreTheme::COLOR_TYPE::WND_FOCUS), m_z_order);
                        CoreWord::draw_string_in_rect(m_surface, z_order, m_str, rect, m_font_type, m_font_color, CoreTheme::get_color(CoreTheme::COLOR_TYPE::WND_FOCUS), CoreWord::ALIGN_TYPE::HORIZONTAL_CENTER | CoreWord::ALIGN_TYPE::VERTICAL_CENTER);
                        break;
                    }
                    case CoreWindow::WINDOW_STATE::PUSHED: {
                        auto z_order = m_parent->get_z_order();
                        if (m_z_order == z_order) {
                            m_z_order = static_cast<CoreSurface::Z_ORDER_LEVEL>(static_cast<std::underlying_type_t<CoreSurface::Z_ORDER_LEVEL>>(m_z_order) + 1);
                            m_attribution = CoreWindow::WINDOW_ATTRIBUTION::VISIBLE | CoreWindow::WINDOW_ATTRIBUTION::FOCUS | CoreWindow::WINDOW_ATTRIBUTION::PRIORITY;
                            show_keyboard();
                        }
                        m_surface->fill_rect(rect, CoreTheme::get_color(CoreTheme::COLOR_TYPE::WND_PUSHED), z_order);
                        m_surface->draw_rect(rect, CoreTheme::get_color(CoreTheme::COLOR_TYPE::WND_BORDER), m_z_order, 2);
                        (std::strlen(m_str_input) != 0)
                                ? CoreWord::draw_string_in_rect(m_surface, z_order, m_str_input, rect, m_font_type, m_font_color, CoreTheme::get_color(CoreTheme::COLOR_TYPE::WND_PUSHED), CoreWord::ALIGN_TYPE::HORIZONTAL_CENTER | CoreWord::ALIGN_TYPE::VERTICAL_CENTER)
                                : CoreWord::draw_string_in_rect(m_surface, z_order, m_str, rect, m_font_type, m_font_color, CoreTheme::get_color(CoreTheme::COLOR_TYPE::WND_PUSHED), CoreWord::ALIGN_TYPE::HORIZONTAL_CENTER | CoreWord::ALIGN_TYPE::VERTICAL_CENTER);
                        break;
                    }
                    default:
                        GalAssert(false);
                        break;
                }
            }

            void on_focus() override {
                m_state = CoreWindow::WINDOW_STATE::FOCUSED;
                on_paint();
            }

            void on_blur() override {
                m_state = CoreWindow::WINDOW_STATE::NORMAL;
                on_paint();
            }

            void on_navigate(NAVIGATION_KEY key) override {
                switch (key) {
                    case CoreWindow::NAVIGATION_KEY::ENTER:
                        (m_state == CoreWindow::WINDOW_STATE::PUSHED)
                                ? s_keyboard.on_navigate(key)
                                : (
                                   on_touch(m_window_rect.m_left_top_corner, CoreWindow::TOUCH_ACTION::DOWN),
                                   on_touch(m_window_rect.m_left_top_corner, CoreWindow::TOUCH_ACTION::UP)
                                   );
                        break;
                    case CoreWindow::NAVIGATION_KEY::BACKWARD:
                    case CoreWindow::NAVIGATION_KEY::FORWARD:
                        (m_state == CoreWindow::WINDOW_STATE::PUSHED) ? s_keyboard.on_navigate(key) : CoreWindow::on_navigate(key);
                        break;
                }
            }

            void on_touch(CorePoint<CoreSurface::PixelPositionType> point, TOUCH_ACTION action) override {
                (action == CoreWindow::TOUCH_ACTION::DOWN) ? on_touch_down(point) : on_touch_up(point);
            }

            void on_keyboard_click(IdType id, const std::any &param) {
                switch (std::any_cast<CoreKeyboard::KEYBOARD_CLICK>(param)) {
                    case CoreKeyboard::KEYBOARD_CLICK::CHARACTER:
                        std::strcpy(m_str_input, s_keyboard.get_str());
                        on_paint();
                        break;
                    case CoreKeyboard::KEYBOARD_CLICK::ENTER:
                        if (std::strlen(m_str_input) != 0) {
                            std::memcpy(m_str, m_str_input, sizeof(m_str_input));
                        }
                        m_state = CoreWindow::WINDOW_STATE::FOCUSED;
                        on_paint();
                        break;
                    case CoreKeyboard::KEYBOARD_CLICK::ESCAPE:
                        std::memset(m_str_input, 0, sizeof(m_str_input));
                        m_state = CoreWindow::WINDOW_STATE::FOCUSED;
                        on_paint();
                        break;
                    default:
                        GalAssert(false);
                        break;
                }
            }

        private:
            void show_keyboard() {
                s_keyboard.connect(this, EditKeyboardId, m_keyboard_style);
                s_keyboard.set_on_click(static_cast<WindowCallback>(&CoreEdit::on_keyboard_click));
                s_keyboard.show_window();
            }

            void on_touch_down(CorePoint<CoreSurface::PixelPositionType> point) {
                if (m_window_rect.is_in_rect(point)) {
                    //click edit box
                    if (m_state == CoreWindow::WINDOW_STATE::NORMAL) {
                        m_parent->set_child_focus(this);
                    }
                } else {
                    auto rect = s_keyboard.get_window_rect();
                    rect.move_rect(m_window_rect.m_left_top_corner.m_x, m_window_rect.m_left_top_corner.m_y);
                    if (rect.is_in_rect(point)) {
                        //click key board
                        CoreWindow::on_touch(point, CoreWindow::TOUCH_ACTION::DOWN);
                    } else if (m_state == CoreWindow::WINDOW_STATE::PUSHED) {
                        m_state = CoreWindow::WINDOW_STATE::FOCUSED;
                        on_paint();
                    }
                }
            }

            void on_touch_up(CorePoint<CoreSurface::PixelPositionType> point) {
                if (m_state == CoreWindow::WINDOW_STATE::FOCUSED) {
                    m_state = CoreWindow::WINDOW_STATE::PUSHED;
                    on_paint();
                } else if (m_state == CoreWindow::WINDOW_STATE::PUSHED) {
                    if (m_window_rect.is_in_rect(point)) {
                        // click edit box
                        m_state = CoreWindow::WINDOW_STATE::FOCUSED;
                        on_paint();
                    } else {
                        CoreWindow::on_touch(point, CoreWindow::TOUCH_ACTION::UP);
                    }
                }
            }

            static CoreKeyboard s_keyboard;
            CoreKeyboard::KEYBOARD_BOARD m_keyboard_style;
            char m_str_input[MaxEditStringLength];
            char m_str[MaxEditStringLength];
        };

        class CoreLabel : public CoreWindow {
        public:
            void on_paint() override {
                if (m_str) {
                    auto rect = get_screen_rect();
                    auto background_color = m_background_color ? m_background_color : m_parent->get_background_color();
                    m_surface->fill_rect(rect, background_color, m_z_order);
                    CoreWord::draw_string_in_rect(m_surface, m_z_order, m_str, rect, m_font_type, m_font_color, background_color, CoreWord::ALIGN_TYPE::HORIZONTAL_LEFT | CoreWord::ALIGN_TYPE::VERTICAL_CENTER);
                }
            }

        protected:
            void pre_create_window() override {
                m_attribution = CoreWindow::WINDOW_ATTRIBUTION::VISIBLE;
                m_font_color = CoreTheme::get_color(CoreTheme::COLOR_TYPE::WND_FONT);
                m_font_type = CoreTheme::get_font(CoreTheme::FONT_TYPE::DEFAULT);
            }
        };

        class CoreListbox : public CoreWindow {
        public:
            void set_on_change(WindowCallback on_change) { this->m_on_change = on_change; }

            [[nodiscard]] size_t get_item_count() const { return m_item_array.size(); }

            void add_item(char *str) {
                m_item_array.push_back(str);
                update_list_list();
            }

            void clear_item() {
                m_item_array.clear();
                update_list_list();
            }

            void select_item(size_t index) {
                GalAssert(index < m_item_array.size());
                m_select_item = index;
            }

        protected:
            void pre_create_window() override {
                m_attribution = CoreWindow::WINDOW_ATTRIBUTION::VISIBLE | CoreWindow::WINDOW_ATTRIBUTION::FOCUS;
                m_select_item = 0;
                m_font_type = CoreTheme::get_font(CoreTheme::FONT_TYPE::DEFAULT);
                m_font_color = CoreTheme::get_color(CoreTheme::COLOR_TYPE::WND_FONT);
            }

            void on_paint() override {
                auto rect = get_screen_rect();
                switch (m_state) {
                    case CoreWindow::WINDOW_STATE::NORMAL: {
                        auto z_order = m_parent->get_z_order();
                        if (m_z_order > z_order) {
                            m_z_order = z_order;
                            m_surface->show_layer(m_list_screen_rect, m_z_order);
                            m_attribution = CoreWindow::WINDOW_ATTRIBUTION::VISIBLE | CoreWindow::WINDOW_ATTRIBUTION::FOCUS;
                        }
                        m_surface->fill_rect(rect, CoreTheme::get_color(CoreTheme::COLOR_TYPE::WND_NORMAL), m_z_order);
                        CoreWord::draw_string_in_rect(m_surface, m_z_order, m_item_array[m_select_item], rect, m_font_type, m_font_color, CoreTheme::get_color(CoreTheme::COLOR_TYPE::WND_NORMAL), CoreWord::ALIGN_TYPE::HORIZONTAL_CENTER | CoreWord::ALIGN_TYPE::VERTICAL_CENTER);
                        break;
                    }
                    case CoreWindow::WINDOW_STATE::FOCUSED: {
                        auto z_order = m_parent->get_z_order();
                        if (m_z_order > z_order) {
                            m_z_order = z_order;
                            m_surface->show_layer(m_list_screen_rect, m_z_order);
                            m_attribution = CoreWindow::WINDOW_ATTRIBUTION::VISIBLE | CoreWindow::WINDOW_ATTRIBUTION::FOCUS;
                        }
                        m_surface->fill_rect(rect, CoreTheme::get_color(CoreTheme::COLOR_TYPE::WND_FOCUS), m_z_order);
                        CoreWord::draw_string_in_rect(m_surface, m_z_order, m_item_array[m_select_item], rect, m_font_type, m_font_color, CoreTheme::get_color(CoreTheme::COLOR_TYPE::WND_FOCUS), CoreWord::ALIGN_TYPE::HORIZONTAL_CENTER | CoreWord::ALIGN_TYPE::VERTICAL_CENTER);
                        break;
                    }
                    case CoreWindow::WINDOW_STATE::PUSHED: {
                        m_surface->fill_rect(rect, CoreTheme::get_color(CoreTheme::COLOR_TYPE::WND_PUSHED), m_z_order);
                        m_surface->draw_rect(rect, CoreTheme::get_color(CoreTheme::COLOR_TYPE::WND_BORDER), m_z_order, 2);
                        CoreWord::draw_string_in_rect(m_surface, m_z_order, m_item_array[m_select_item], rect, m_font_type, utility::build_rgb(2, 124, 165), utility::build_argb(0, 0, 0, 0), CoreWord::ALIGN_TYPE::HORIZONTAL_CENTER | CoreWord::ALIGN_TYPE::VERTICAL_CENTER);
                        //draw list
                        if (!m_item_array.empty()) {
                            if (m_z_order == m_parent->get_z_order()) {
                                m_z_order = static_cast<CoreSurface::Z_ORDER_LEVEL>(static_cast<std::underlying_type_t<CoreSurface::Z_ORDER_LEVEL>>(m_z_order) + 1);
                            }
                            m_attribution = CoreWindow::WINDOW_ATTRIBUTION::VISIBLE | CoreWindow::WINDOW_ATTRIBUTION::FOCUS | CoreWindow::WINDOW_ATTRIBUTION::PRIORITY;
                            show_list();
                        }
                        break;
                    }
                    default:
                        GalAssert(false);
                        break;
                }
            }

            void on_focus() override {
                m_state = CoreWindow::WINDOW_STATE::FOCUSED;
                on_paint();
            }

            void on_blur() override {
                m_state = CoreWindow::WINDOW_STATE::NORMAL;
                on_paint();
            }

            void on_navigate(NAVIGATION_KEY key) override {
                switch (key) {
                    case CoreWindow::NAVIGATION_KEY::ENTER:
                        on_touch(m_window_rect.m_left_top_corner, CoreWindow::TOUCH_ACTION::DOWN);
                        on_touch(m_window_rect.m_left_top_corner, CoreWindow::TOUCH_ACTION::UP);
                        return;
                    case CoreWindow::NAVIGATION_KEY::BACKWARD:
                        if (m_state != CoreWindow::WINDOW_STATE::PUSHED) {
                            return CoreWindow::on_navigate(key);
                        }
                        m_select_item = (m_select_item > 0) ? (m_select_item - 1) : m_select_item;
                        return show_list();
                    case CoreWindow::NAVIGATION_KEY::FORWARD:
                        if (m_state != CoreWindow::WINDOW_STATE::PUSHED) {
                            return CoreWindow::on_navigate(key);
                        }
                        m_select_item = (m_select_item < (m_item_array.size() - 1)) ? (m_select_item + 1) : m_select_item;
                        return show_list();
                }
            }

            void on_touch(CorePoint<CoreSurface::PixelPositionType> point, TOUCH_ACTION action) override {
                (action == CoreWindow::TOUCH_ACTION::DOWN) ? on_touch_down(point) : on_touch_up(point);
            }

        private:
            void update_list_list() {
                m_list_window_rect = m_window_rect;
                m_list_window_rect.m_left_top_corner.m_y = m_window_rect.m_right_bottom_corner.m_y + 1;
                m_list_window_rect.m_right_bottom_corner.m_y = m_list_window_rect.m_left_top_corner.m_y + static_cast<int>(m_item_array.size() * ListboxItemHeight);

                m_list_screen_rect = get_screen_rect();
                m_list_screen_rect.m_left_top_corner.m_y = m_list_screen_rect.m_right_bottom_corner.m_y + 1;
                m_list_screen_rect.m_right_bottom_corner.m_y = m_list_screen_rect.m_left_top_corner.m_y + static_cast<int>(m_item_array.size() * ListboxItemHeight);
            }

            void show_list() {
                //draw all items
                auto [left, top] = m_list_screen_rect.m_left_top_corner;
                auto right = m_list_screen_rect.m_right_bottom_corner.m_x;
                for (auto i = 0; i < m_item_array.size(); ++i) {
                    auto rect = CoreRect(left, top + i * ListboxItemHeight, right, top + i * ListboxItemHeight + ListboxItemHeight);
                    if (m_select_item == i) {
                        m_surface->fill_rect(rect, CoreTheme::get_color(CoreTheme::COLOR_TYPE::WND_FOCUS), m_z_order);
                        CoreWord::draw_string_in_rect(m_surface, m_z_order, m_item_array[i], rect, m_font_type, m_font_color, CoreTheme::get_color(CoreTheme::COLOR_TYPE::WND_FOCUS), CoreWord::ALIGN_TYPE::HORIZONTAL_CENTER | CoreWord::ALIGN_TYPE::VERTICAL_CENTER);
                    } else {
                        m_surface->fill_rect(rect, utility::build_rgb(17, 17, 17), m_z_order);
                        CoreWord::draw_string_in_rect(m_surface, m_z_order, m_item_array[i], rect, m_font_type, m_font_color, utility::build_rgb(17, 17, 17), CoreWord::ALIGN_TYPE::HORIZONTAL_CENTER | CoreWord::ALIGN_TYPE::VERTICAL_CENTER);
                    }
                }
            }

            void on_touch_down(CorePoint<CoreSurface::PixelPositionType> point) {
                if (m_window_rect.is_in_rect(point)) {
                    //click base
                    if (m_state == CoreWindow::WINDOW_STATE::NORMAL) {
                        m_parent->set_child_focus(this);
                    }
                } else if (m_list_window_rect.is_in_rect(point)) {
                    //click extend list
                    CoreWindow::on_touch(point, CoreWindow::TOUCH_ACTION::DOWN);
                } else {
                    if (m_state == CoreWindow::WINDOW_STATE::PUSHED) {
                        m_state = CoreWindow::WINDOW_STATE::FOCUSED;
                        on_paint();
                        if (m_on_change) {
                            (m_parent->*m_on_change)(m_id, m_select_item);
                        }
                    }
                }
            }

            void on_touch_up(CorePoint<CoreSurface::PixelPositionType> point) {
                if (m_state == CoreWindow::WINDOW_STATE::FOCUSED) {
                    m_state = CoreWindow::WINDOW_STATE::PUSHED;
                    on_paint();
                } else if (m_state == CoreWindow::WINDOW_STATE::PUSHED) {
                    if (m_window_rect.is_in_rect(point)) {
                        //click base
                        m_state = CoreWindow::WINDOW_STATE::FOCUSED;
                        on_paint();
                    } else if (m_list_window_rect.is_in_rect(point)) {
                        //click extend list
                        m_state = CoreWindow::WINDOW_STATE::FOCUSED;
                        select_item((point.m_y - m_list_window_rect.m_left_top_corner.m_y) / ListboxItemHeight);
                        on_paint();
                        if (m_on_change) {
                            (m_parent->*m_on_change)(m_id, m_select_item);
                        }
                    } else {
                        CoreWindow::on_touch(point, CoreWindow::TOUCH_ACTION::UP);
                    }
                }
            }

            size_t m_select_item;
            std::vector<const char *> m_item_array;
            CoreRect<CoreSurface::PixelPositionType> m_list_window_rect;//rect relative to parent wnd
            CoreRect<CoreSurface::PixelPositionType> m_list_screen_rect;//rect relative to physical screen(framebuffer)
            WindowCallback m_on_change;
        };
    }

    constexpr auto MaxSlideGroupPages = 5;
    //constexpr auto GestureSwipeStep = 300; //for arm
    constexpr auto GestureSwipeStep = 10; //for PC & ANDROID
    constexpr auto GestureMoveThreshold = 10;

    namespace detail {
        class CoreGesture;

        class CoreSlideGroup : public CoreWindow
        {
        public:
            enum class SET_SLIDE_STATE {
                INVALID_INDEX = -1,
                INVALID_SLIDE = -2,
                SUCCESS = 0
            };

            enum class ADD_SLIDE_STATE {
                INVALID_SLIDE = -1,
                EXIST_SLIDE = -2,
                GROUP_FULL = -3,
                SUCCESS = 0
            };

            CoreSlideGroup();

            SET_SLIDE_STATE set_active_slide(int index, bool redraw = true)
            {
                if(index >= MaxSlideGroupPages || index < 0) {return SET_SLIDE_STATE::INVALID_INDEX;}
                if(m_slides[index] == nullptr) {return SET_SLIDE_STATE::INVALID_SLIDE;}

                m_active_slide_index = index;
                for(auto i = 0; i < MaxSlideGroupPages; ++i)
                {
                    auto& slide = m_slides[i];
                    if(slide == nullptr) {continue;}

                    if(i == index)
                    {
                        slide->get_surface()->set_active(true);
                        add_child_to_tail(slide);
                        if(redraw)
                        {
                            slide->get_surface()->flush_screen(get_screen_rect());
                        }
                    }
                    else
                    {
                        slide->get_surface()->set_active(false);
                    }
                }
                return SET_SLIDE_STATE::SUCCESS;
            }

            [[nodiscard]] CoreWindow* get_slide(int index) const {return (index < m_slides.size() && index >= 0) ? m_slides[index] : nullptr;}

            [[nodiscard]] CoreWindow* get_active_slide() const {return m_slides[m_active_slide_index];}

            [[nodiscard]] int get_active_slide_index() const {return m_active_slide_index;}

            ADD_SLIDE_STATE add_slide(
                    CoreWindow* slide, IdType resource_id,
                    CorePoint<CoreSurface::PixelPositionType> point,
                    CoreSurface::SurfaceSizeType width, CoreSurface::SurfaceSizeType height,
                    CoreWindow::window_tree* child_tree = nullptr,
                    CoreSurface::Z_ORDER_LEVEL max_z_order = CoreSurface::Z_ORDER_LEVEL::LOWEST)
            {
                if(slide == nullptr) {return ADD_SLIDE_STATE::INVALID_SLIDE;}

                auto old_surface = get_surface();
                auto new_surface = old_surface->get_display()->alloc_surface(max_z_order);
                new_surface->set_active(false);
                set_surface(new_surface);

                slide->connect(this, resource_id, nullptr, point, width, height, child_tree);
                set_surface(old_surface);

                for(auto& each : m_slides)
                {
                    if(each == slide)
                    {
                        // slide already exist
                        GalAssert(false);
                        return ADD_SLIDE_STATE::EXIST_SLIDE;
                    }
                }

                //new slide
                for(auto& each : m_slides)
                {
                    if(each == nullptr)
                    {
                        each = slide;
                        slide->show_window();
                        return ADD_SLIDE_STATE::SUCCESS;
                    }
                }

                //no more slide can be add
                GalAssert(false);
                return ADD_SLIDE_STATE::GROUP_FULL;
            }

            void disable_all_slide()
            {
                for(auto& slide : m_slides)
                {
                    if(slide)
                    {
                        slide->get_surface()->set_active(false);
                    }
                }
            }

            void on_touch(CorePoint<CoreSurface::PixelPositionType> point, TOUCH_ACTION action) override;

            void on_navigate(NAVIGATION_KEY key) override
            {
                if(auto slide = m_slides[m_active_slide_index]; slide)
                {
                    slide->on_navigate(key);
                }
            }

        protected:
            std::array<CoreWindow*, MaxSlideGroupPages> m_slides;
            int m_active_slide_index;
            CoreGesture* m_gesture;
        };

        class CoreGesture
        {
        public:
            enum class TOUCH_STATE
            {
                MOVE,
                IDLE
            };

            enum class SWIPE_STATE
            {
                INVALID_SLIDE_GROUP = -1,
                INVALID_ACTIVE_SLIDE_INDEX = -2,
                INVALID_SLIDE_SURFACE = -3,
            };

            explicit CoreGesture(CoreSlideGroup* group)
                :
                  m_down_x(0), m_move_x(0),
                  m_state(TOUCH_STATE::IDLE), m_slide_group(group) {};

            bool handle_swipe(CorePoint<CoreSurface::PixelPositionType> point, CoreKeyboard::TOUCH_ACTION action)
            {
                auto [x, _] = point;
                if(action == CoreWindow::TOUCH_ACTION::DOWN)
                {
                    if(m_state == TOUCH_STATE::IDLE)
                    {
                        m_state = TOUCH_STATE::MOVE;
                        m_move_x = x;
                        m_down_x = x;
                        return true;
                    }
                    else
                    {
                        return on_move(x);
                    }
                }
                else if(action == CoreWindow::TOUCH_ACTION::UP)
                {
                    if(m_state == TOUCH_STATE::MOVE)
                    {
                        m_state = TOUCH_STATE::IDLE;
                        return on_swipe(x);
                    }
                    else
                    {
                        return false;
                    }
                }
                return true;
            }

        private:
            bool on_move(CoreSurface::PixelPositionType x)
            {
                if(m_slide_group == nullptr) {return true;}
                if(utility::abs(x - m_move_x) < GestureMoveThreshold) { return false;}

                m_slide_group->disable_all_slide();
                m_move_x = x;
                (m_move_x > m_down_x) ? move_right() : move_left();
                return false;
            }

            bool on_swipe(CoreSurface::PixelPositionType x)
            {
                if(m_slide_group == nullptr) {return true;}
                if((m_down_x == m_move_x) && utility::abs(x - m_down_x) < GestureMoveThreshold) {return true;}

                m_slide_group->disable_all_slide();
                auto page = -1;
                m_move_x = x;
                page = (m_move_x > m_down_x) ? swipe_right() : swipe_left();
                if(page >= 0)
                {
                    m_slide_group->set_active_slide(page);
                }
                else
                {
                    m_slide_group->set_active_slide(m_slide_group->get_active_slide_index(), false);
                }
                return false;
            }

            int swipe_left()
            {
                if(m_slide_group == nullptr) {return static_cast<int>(SWIPE_STATE::INVALID_SLIDE_GROUP);}

                auto index = m_slide_group->get_active_slide_index();
                auto slide1 = m_slide_group->get_slide(index + 1);
                auto slide2 = m_slide_group->get_slide(index);
                if(slide1 == nullptr || slide2 == nullptr)
                {
                    return static_cast<int>(SWIPE_STATE::INVALID_ACTIVE_SLIDE_INDEX);
                }

                auto surface1 = slide1->get_surface();
                auto surface2 = slide2->get_surface();
                if(surface1->get_display() != surface2->get_display()) { return static_cast<int>(SWIPE_STATE::INVALID_SLIDE_SURFACE);}

                auto step = m_down_x - m_move_x;
                auto rect = m_slide_group->get_screen_rect();
                while(step < rect.get_width())
                {
                    surface1->get_display()->swipe_surface(surface2, surface1, rect, step);
                    step += GestureSwipeStep;
                }
                if(step != rect.get_width())
                {
                    surface1->get_display()->swipe_surface(surface2, surface1, rect, rect.get_width());
                }

                return index + 1;
            }

            int swipe_right()
            {
                if(m_slide_group == nullptr) {return static_cast<int>(SWIPE_STATE::INVALID_SLIDE_GROUP);}

                auto index = m_slide_group->get_active_slide_index();
                auto slide1 = m_slide_group->get_slide(index - 1);
                auto slide2 = m_slide_group->get_slide(index);
                if(slide1 == nullptr || slide2 == nullptr)
                {
                    return static_cast<int>(SWIPE_STATE::INVALID_ACTIVE_SLIDE_INDEX);
                }

                auto surface1 = slide1->get_surface();
                auto surface2 = slide2->get_surface();
                if(surface1->get_display() != surface2->get_display()) { return static_cast<int>(SWIPE_STATE::INVALID_SLIDE_SURFACE);}

                auto rect = m_slide_group->get_screen_rect();
                auto step = rect.get_width() - (m_move_x - m_down_x);
                while(step > 0)
                {
                    surface1->get_display()->swipe_surface(surface1, surface2, rect, step);
                    step -= GestureSwipeStep;
                }
                if(step != rect.get_width())
                {
                    surface1->get_display()->swipe_surface(surface1, surface2, rect, 0);
                }

                return index - 1;
            }

            void move_left()
            {
                auto index = m_slide_group->get_active_slide_index();
                auto slide1 = m_slide_group->get_slide(index + 1);
                auto slide2 = m_slide_group->get_slide(index);
                if(slide1 == nullptr || slide2 == nullptr) {return;}

                auto surface1 = slide1->get_surface();
                auto surface2 = slide2->get_surface();

                if(auto display = surface1->get_display(); display == surface2->get_display())
                {
                    display->swipe_surface(surface2, surface1, m_slide_group->get_screen_rect(), (m_down_x - m_move_x));
                }
            }

            void move_right()
            {
                auto index = m_slide_group->get_active_slide_index();
                auto slide1 = m_slide_group->get_slide(index - 1);
                auto slide2 = m_slide_group->get_slide(index);
                if(slide1 == nullptr || slide2 == nullptr) {return;}

                auto surface1 = slide1->get_surface();
                auto surface2 = slide2->get_surface();

                if(auto display = surface1->get_display(); display == surface2->get_display())
                {
                    auto rect = m_slide_group->get_screen_rect();
                    display->swipe_surface(surface1, surface2, rect, (rect.get_width() - (m_move_x - m_down_x)));
                }
            }

            CoreSurface::PixelPositionType m_down_x;
            CoreSurface::PixelPositionType m_move_x;
            TOUCH_STATE m_state;
            CoreSlideGroup* m_slide_group;
        };

        CoreSlideGroup::CoreSlideGroup()
            :
              m_active_slide_index(0),
              m_gesture(new CoreGesture(this)),
              m_slides({nullptr}){}

        void CoreSlideGroup::on_touch(CorePoint<CoreSurface::PixelPositionType> point, TOUCH_ACTION action)
        {
            auto [x, y] = point;
            x = m_window_rect.distance_of_left(x);
            y = m_window_rect.distance_of_top(y);
            if(m_gesture->handle_swipe({x, y}, action))
            {
                m_slides[m_active_slide_index]->on_touch({x, y}, action);
            }
        }

        class CoreSpinBox;
        class CoreSpinBoxButton : public CoreButton
        {
            friend class CoreSpinBox;
            void on_touch(CorePoint<CoreSurface::PixelPositionType> point, TOUCH_ACTION action) override;
            CoreSpinBox* m_spin_box;
        };

        class CoreSpinBox : public CoreWindow
        {
            friend class CoreSpinBoxButton;
        public:
            [[nodiscard]] int get_value() const {return m_value;}

            void set_value(int value) {m_value = value; m_curr_value = value;}

            void set_min_max(int min, int max) {m_min = min; m_max = max;}

            void set_step(int step) {m_step = step;}

            [[nodiscard]] int get_min() const {return m_min;}

            [[nodiscard]] int get_max() const {return m_max;}

            [[nodiscard]] int get_step() const {return m_step;}

            void set_value_digit(int digit) {m_digit = digit;}

            [[nodiscard]] int get_value_digit() const {return m_digit;}

            void set_on_change(WindowCallback on_change) {m_on_change = on_change;}

        protected:
            void on_paint() override
            {
                auto rect = get_screen_rect();
                rect.m_right_bottom_corner.m_x = rect.m_left_top_corner.m_x + (rect.get_width() * 2 / 3);
                m_surface->fill_rect(rect, CoreTheme::get_color(CoreTheme::COLOR_TYPE::WND_NORMAL), m_z_order);
                CoreWord::draw_value_in_rect(m_surface, m_parent->get_z_order(), m_curr_value, m_digit, rect, m_font_type, m_font_color, CoreTheme::get_color(CoreTheme::COLOR_TYPE::WND_NORMAL), CoreWord::ALIGN_TYPE::HORIZONTAL_CENTER | CoreWord::ALIGN_TYPE::VERTICAL_CENTER);
            }

            void pre_create_window() override
            {
                m_attribution = CoreWindow::WINDOW_ATTRIBUTION::VISIBLE;
                m_font_type = CoreTheme::get_font(CoreTheme::FONT_TYPE::DEFAULT);
                m_font_color = CoreTheme::get_color(CoreTheme::COLOR_TYPE::WND_FONT);
                m_max = 6;
                m_min = 1;
                m_digit = 0;
                m_step = 1;
                //link arrow button position
                m_button_down.m_spin_box = this;
                m_button_up.m_spin_box = this;

                auto rect = get_window_rect();
                auto [point, _] = rect;
                point.horizontal_move(rect.get_width() * 2 / 3);
                m_button_down.connect(m_parent, ArrowDownId, ArrowDownChar, point, rect.get_width() / 3, rect.get_height() / 2, nullptr);
                point.vertical_move(rect.get_height() / 2);
                m_button_up.connect(m_parent, ArrowUpId, ArrowUpChar, point, rect.get_width() / 3, rect.get_height() / 2, nullptr);
            }

            void on_arrow_button_clicked(TOUCH_ACTION action)
            {
                if(action == CoreWindow::TOUCH_ACTION::UP)
                {
                    if(m_curr_value + m_step < m_min) {return;}
                    m_curr_value += m_step;
                }
                else
                {
                    if(m_curr_value + m_step > m_max) {return;}
                    m_curr_value -= m_step;
                }

                m_curr_value -= m_step;
                if(m_on_change)
                {
                    (m_parent->*m_on_change)(m_id, m_curr_value);
                }
                on_paint();
            }

            int m_curr_value;
            int m_value;
            int m_step;
            int m_max;
            int m_min;
            int m_digit;
            CoreSpinBoxButton m_button_up;
            CoreSpinBoxButton m_button_down;
            WindowCallback m_on_change;
        };

        void CoreSpinBoxButton::on_touch(CorePoint<CoreSurface::PixelPositionType> point, TOUCH_ACTION action)
        {
            m_spin_box->on_arrow_button_clicked(action);
            CoreButton::on_touch(point, action);
        }

        class CoreTable : public CoreWindow
        {
        public:
            void set_sheet_align_type(CoreWord::ALIGN_TYPE align_type) {m_align_type = align_type;}

            void set_all_row_height(unsigned int height)
            {
                std::fill(m_row_heights.begin(), m_row_heights.end(), height);
            }

            void set_all_column_width(unsigned int width)
            {
                std::fill(m_column_widths.begin(), m_column_widths.end(), width);
            }

            size_t set_row_height(size_t index, unsigned int height)
            {
                if(index < m_row_heights.size())
                {
                    m_row_heights[index] = height;
                    return index;
                }

                return static_cast<size_t>(-1);
            }

            size_t set_column_width(size_t index, unsigned int width)
            {
                if(index < m_column_widths.size())
                {
                    m_column_widths[index] = width;
                    return index;
                }

                return static_cast<size_t>(-1);
            }

            void set_item(size_t row, size_t column, const char* str, ColorType color)
            {
                draw_items(row, column, str, color);
            }

            [[nodiscard]] size_t get_row_size() const {return m_row_heights.size();}

            [[nodiscard]] size_t get_column_size() const {return m_column_widths.size();}

            CoreRect<CoreSurface::PixelPositionType> get_item_rect(size_t row, size_t column)
            {
                static CoreRect<CoreSurface::PixelPositionType> rect;
                if(row >= m_row_heights.size() || column >= m_column_widths.size())
                {
                    return rect;
                }
                auto column_end = m_column_widths.begin();
                auto row_end = m_row_heights.begin();
                std::advance(column_end, column);
                std::advance(row_end, row);
                auto total_width = std::accumulate<decltype(m_column_widths.begin()), unsigned int>(m_column_widths.begin(), column_end, 0);
                auto total_height = std::accumulate<decltype(m_row_heights.begin()), unsigned int>(m_row_heights.begin(), row_end, 0);

                auto tmp_rect = get_screen_rect();
                rect.m_left_top_corner.m_x = tmp_rect.m_left_top_corner.m_x + total_width;
                rect.m_right_bottom_corner.m_x = rect.m_left_top_corner.m_x + m_column_widths[column];
                if(auto& right = rect.m_right_bottom_corner.m_x; right > tmp_rect.m_right_bottom_corner.m_x)
                {
                    right = tmp_rect.m_right_bottom_corner.m_x;
                }
                rect.m_left_top_corner.m_y = tmp_rect.m_left_top_corner.m_y + total_height;
                rect.m_right_bottom_corner.m_y = rect.m_left_top_corner.m_y + m_row_heights[row];
                if(auto& bottom = rect.m_right_bottom_corner.m_y; bottom > tmp_rect.m_right_bottom_corner.m_y)
                {
                    bottom = tmp_rect.m_right_bottom_corner.m_y;
                }
                return rect;
            }

        protected:
            void pre_create_window() override
            {
                m_attribution = CoreWindow::WINDOW_ATTRIBUTION::VISIBLE;
                m_font_type = CoreTheme::get_font(CoreTheme::FONT_TYPE::DEFAULT);
                m_font_color = CoreTheme::get_color(CoreTheme::COLOR_TYPE::WND_FONT);
            }

            void draw_items(size_t row, size_t col, const char* str, ColorType color)
            {
                auto rect = get_item_rect(row, col);
                rect.move_rect(1, -1);
                m_surface->fill_rect(rect, color, m_z_order);
                CoreWord::draw_string_in_rect(m_surface, m_z_order, str, rect, m_font_type, m_font_color, utility::build_argb(0, 0, 0, 0), m_align_type);
            }

            CoreWord::ALIGN_TYPE m_align_type;
            std::vector<unsigned int> m_row_heights;
            std::vector<unsigned int> m_column_widths;
        };
    }
}

std::array<const Gal::detail::CoreTheme::font_info *, static_cast<std::size_t>(Gal::detail::CoreTheme::FONT_TYPE::MAX)> s_font_map;
std::array<const Gal::detail::CoreTheme::bitmap_info *, static_cast<std::size_t>(Gal::detail::CoreTheme::BITMAP_TYPE::BITMAP_MAX)> s_bitmap_map;
std::array<Gal::ColorType, static_cast<std::size_t>(Gal::detail::CoreTheme::COLOR_TYPE::MAX)> s_color_map;

#endif