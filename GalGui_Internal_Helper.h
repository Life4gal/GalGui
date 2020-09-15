#pragma once

#include <algorithm>
#include <any>
#include <variant>
#include <type_traits>

#include <cassert>
#include <cfloat>
#include <cstdarg>
#include <cstddef>
#include <cstring>

/*
// 对 enum class 的 operator& operator&= operator| operator|= 运算支持
// 如果目标类型不是 enum 则编译期报错
// TODO 这会阻止 operator& operator&= operator| operator|= 运算的其他模版
template <typename T, typename = std::enable_if_t<std::is_enum_v<T>, T>>
constexpr T operator&(T lhs, T rhs)
{
	return static_cast<T>(
		static_cast<typename std::underlying_type<T>::type>(lhs)&
		static_cast<typename std::underlying_type<T>::type>(rhs));
}

// 用于 if(flag & enum) 运算
template <typename Target, typename T, typename = std::enable_if_t<std::is_enum_v<T>, T>>
constexpr Target operator&(Target lhs, T rhs)
{
	return static_cast<Target>(
		static_cast<typename std::underlying_type<T>::type>(lhs)&
		static_cast<typename std::underlying_type<T>::type>(rhs));
}

template <typename Target, typename T, typename = std::enable_if_t<std::is_enum_v<T>, T>>
constexpr Target& operator&=(Target& lhs, T rhs)
{
	return lhs = static_cast<Target>(
		static_cast<typename std::underlying_type<T>::type>(lhs)&
		static_cast<typename std::underlying_type<T>::type>(rhs));
}

template <typename T, typename = std::enable_if_t<std::is_enum_v<T>, T>>
constexpr T operator|(T lhs, T rhs)
{
	return static_cast<T>(
		static_cast<typename std::underlying_type<T>::type>(lhs) |
		static_cast<typename std::underlying_type<T>::type>(rhs));
}

template <typename Target, typename T, typename = std::enable_if_t<std::is_enum_v<T>, T>>
constexpr Target& operator|=(Target& lhs, T rhs)
{
	return lhs = static_cast<Target>(
		static_cast<typename std::underlying_type<T>::type>(lhs) |
		static_cast<typename std::underlying_type<T>::type>(rhs));
}

// TODO 经过考虑我们觉得我们无法承受放弃其他支持这些运算的类型的运算而仅仅为了支持Enum的代价
*/


/* EnumType 应该是 Enum 的类型, 例如 enum class Enum : EnumType
 * 之所以把 EnumType 写成 decltype(EnumType()),把 Enum 写成 decltype(Enum()) 是因为下面这个警告:
 *		Macro argument should be enclosed in parentheses [bugprone-macro-parentheses]
 *
 * inline constexpr Enum operator|(Enum lhs, Enum rhs)				用于 flag = enum1 | enum2 (正常来说 enum1 | enum2 操作不会等于另一个 enum)
 * inline constexpr Enum operator|(EnumType lhs, Enum rhs)			用于 flag1 = flag2 | enum2 (正常来说 flag | enum 操作不会等于另一个 enum)
 * inline constexpr Enum operator|(Enum lhs, EnumType rhs)			用于 flag1 = enum2 | flag2 (正常来说 enum | flag 操作不会等于另一个 enum)
 * inline constexpr EnumType operator|=(EnumType& lhs, Enum rhs)	只允许 flag |= enum, 而不允许反过来
 * inline constexpr Enum operator&(Enum lhs, Enum rhs)				用于 flag = enum1 & enum2 (正常来说 enum1 & enum2 操作不会等于另一个 enum)
 * inline constexpr EnumType operator&(EnumType lhs, Enum rhs)		用于 flag1 = flag2 & enum2 (正常来说 flag & enum 操作不会等于另一个 enum)
 * inline constexpr EnumType operator&(Enum rhs, EnumType lhs)		用于 flag1 = enum2 & flag2 (正常来说 enum & flag 操作不会等于另一个 enum)
 * inline constexpr EnumType operator&=(EnumType& lhs, Enum rhs)	只允许 flag &= enum, 而不允许反过来
 *
 * TODO 需要更多测试
 */
// 考虑了很久,我们还是不得不使用 macro 这个坏东西
#define ENUM_FLAG_OPERATORS(Enum, EnumType) \
inline constexpr decltype(EnumType()) operator|(Enum lhs, Enum rhs) \
{ \
	static_assert(std::is_enum_v<Enum>); \
	return static_cast<EnumType>(static_cast<typename std::underlying_type_t<decltype(Enum())>>(lhs) | static_cast<typename std::underlying_type_t<decltype(Enum())>>(rhs)); \
} \
inline constexpr decltype(EnumType()) operator|(EnumType lhs, Enum rhs) \
{ \
	static_assert(std::is_enum_v<Enum>); \
	return static_cast<EnumType>(lhs | static_cast<EnumType>(rhs)); \
} \
inline constexpr decltype(EnumType()) operator|(Enum lhs, EnumType rhs) \
{ \
	static_assert(std::is_enum_v<Enum>); \
	return static_cast<EnumType>(static_cast<EnumType>(lhs) | rhs); \
} \
inline constexpr decltype(EnumType()) operator|=(decltype(EnumType())& lhs, Enum rhs) \
{ \
	static_assert(std::is_enum_v<Enum>); \
	return lhs = static_cast<EnumType>(static_cast<EnumType>(lhs) | rhs); \
}\
inline constexpr decltype(EnumType()) operator&(Enum lhs, Enum rhs)\
{\
	static_assert(std::is_enum_v<Enum>); \
	return static_cast<EnumType>(static_cast<typename std::underlying_type_t<decltype(Enum())>>(lhs) & static_cast<typename std::underlying_type_t<decltype(Enum())>>(rhs)); \
} \
inline constexpr decltype(EnumType()) operator&(EnumType lhs, Enum rhs) \
{ \
	static_assert(std::is_enum_v<Enum>); \
	return static_cast<EnumType>(lhs & static_cast<EnumType>(rhs)); \
} \
inline constexpr decltype(EnumType()) operator&(Enum rhs, EnumType lhs) \
{ \
	static_assert(std::is_enum_v<Enum>); \
	return static_cast<EnumType>(static_cast<EnumType>(lhs) & rhs); \
} \
inline constexpr decltype(EnumType()) operator&=(decltype(EnumType())& lhs, Enum rhs) \
{ \
	static_assert(std::is_enum_v<Enum>); \
	return lhs = static_cast<EnumType>(static_cast<EnumType>(lhs) & rhs); \
}


using GalGuiCol = int; 
// Enum: A color identifier for styling
enum class EnumGalGuiCol : GalGuiCol
{
	GALGUI_COL_TEXT,
	GALGUI_COL_TEXT_DISABLE,
	GALGUI_COL_WINDOW_BACKGROUND,
	GALGUI_COL_CHILD_BACKGROUND,
	GALGUI_COL_POPUP_BACKGROUND,
	GALGUI_COL_BORDER,
	GALGUI_COL_BORDER_SHADOW,
	GALGUI_COL_FRAME_BACKGROUND,
	GALGUI_COL_FRAME_BACKGROUND_HOVERED,
	GALGUI_COL_FRAME_BACKGROUND_ACTIVE,
	GALGUI_COL_TITTLE_BACKGROUND,
	GALGUI_COL_TITTLE_BACKGROUND_ACTIVE,
	GALGUI_COL_TITTLE_BACKGROUND_COLLAPSED,
	GALGUI_COL_MENU_BAR_BACKGROUND,
	GALGUI_COL_SCROLLBAR_BACKGROUND,
	GALGUI_COL_SCROLLBAR_GRAB,
	GALGUI_COL_SCROLLBAR_GRAB_HOVERED,
	GALGUI_COL_SCROLLBAR_GRAB_ACTIVE,
	GALGUI_COL_CHECK_MARK,
	GALGUI_COL_SLIDER_GRAB,
	GALGUI_COL_SLIDER_GRAB_ACTIVE,
	GALGUI_COL_BUTTON,
	GALGUI_COL_BUTTON_HOVERED,
	GALGUI_COL_BUTTON_ACTIVE,
	GALGUI_COL_HEADER,
	GALGUI_COL_HEADER_HOVERED,
	GALGUI_COL_HEADER_ACTIVE,
	GALGUI_COL_SEPARATOR,
	GALGUI_COL_SEPARATOR_HOVERED,
	GALGUI_COL_SEPARATOR_ACTIVE,
	GALGUI_COL_RESIZE_GRIP,
	GALGUI_COL_RESIZE_GRIP_HOVERED,
	GALGUI_COL_RESIZE_GRIP_ACTIVE,
	GALGUI_COL_TAB,
	GALGUI_COL_TAB_HOVERED,
	GALGUI_COL_TAB_ACTIVE,
	GALGUI_COL_TAB_UNFOCUSED,
	GALGUI_COL_TAB_UNFOCUSED_ACTIVE,
	GALGUI_COL_PLOT_LINE,
	GALGUI_COL_PLOT_LINE_HOVERED,
	GALGUI_COL_PLOT_HISTOGRAM,
	GALGUI_COL_PLOT_HISTOGRAM_HOVERED,
	GALGUI_COL_TEXT_SELECTED_BACKGROUND,
	GALGUI_COL_DRAG_DROP_TARGET,
	GALGUI_COL_NAV_HIGHLIGHT,
	GALGUI_COL_NAV_WINDOW_HIGHLIGHT,
	GALGUI_COL_NAV_WINDOW_DIM_BACKGROUND,
	GALGUI_COL_MODAL_WINDOW_DIM_BACKGROUND,
	GALGUI_COL_COUNT
};

using GalGuiCond = int;
// Enum: A condition for many Set*() functions
enum class EnumGalGuiCond : GalGuiCond
{
	GALGUI_COND_NONE = 0,
	GALGUI_COND_ALWAYS = 1 << 0,
	GALGUI_COND_ONCE = 1 << 1,
	GALGUI_COND_FIRST_USE_EVER = 1 << 2,
	GALGUI_COND_APPEARING = 1 << 3
};
// operator
ENUM_FLAG_OPERATORS(EnumGalGuiCond, GalGuiCond)

using GalS8 = signed char;
using GalU8 = unsigned char;
using GalS16 = signed short;
using GalU16 = unsigned short;
using GalS32 = signed int;
using GalU32 = unsigned int;
using GalS64 = signed   __int64;
using GalU64 = unsigned __int64;

using GalGuiDataType = int;
// Enum: A primary data type
enum class EnumGalGuiDataType : GalGuiDataType
{
	GALGUI_DATA_TYPE_S8,
	GALGUI_DATA_TYPE_U8,
	GALGUI_DATA_TYPE_S16,
	GALGUI_DATA_TYPE_U16,
	GALGUI_DATA_TYPE_S32,
	GALGUI_DATA_TYPE_U32,
	GALGUI_DATA_TYPE_S64,
	GALGUI_DATA_TYPE_U64,
	GALGUI_DATA_TYPE_FLOAT,
	GALGUI_DATA_TYPE_DOUBLE,
	GALGUI_DATA_TYPE_COUNT
};
enum class EnumGalGuiDataPrivateType : GalGuiDataType
{
	GALGUI_DATA_PRIVATE_TYPE_STRING = static_cast<GalGuiDataType>(EnumGalGuiDataType::GALGUI_DATA_TYPE_COUNT) + 1,
	GALGUI_DATA_PRIVATE_TYPE_POINTER,
	GALGUI_DATA_PRIVATE_TYPE_ID
};

using GalGuiDir = int;
// Enum: A cardinal direction
enum class EnumGalGuiDir : GalGuiDir
{
	GALGUI_DIR_NONE = -1,
	GALGUI_DIR_LEFT = 0,
	GALGUI_DIR_RIGHT = 1,
	GALGUI_DIR_UP = 2,
	GALGUI_DIR_DOWN = 3,
	GALGUI_DIR_COUNT
};

using GalGuiKey = int;
// Enum: A key identifier (GalGui-side enum)
enum class EnumGalGuiKey : GalGuiKey
{
	GALGUI_KEY_TAB,
	GALGUI_KEY_LEFT_ARROW,
	GALGUI_KEY_RIGHT_ARROW,
	GALGUI_KEY_UP_ARROW,
	GALGUI_KEY_DOWN_ARROW,
	GALGUI_KEY_PAGE_UP,
	GALGUI_KEY_PAGE_DOWN,
	GALGUI_KEY_HOME,
	GALGUI_KEY_END,
	GALGUI_KEY_INSERT,
	GALGUI_KEY_DELETE,
	GALGUI_KEY_BACKSPACE,
	GALGUI_KEY_SPACE,
	GALGUI_KEY_ENTER,
	GALGUI_KEY_ESCAPE,
	GALGUI_KEY_KEYPAD_ENTER,
	GALGUI_KEY_A,					// for text edit CTRL+A: select all
	GALGUI_KEY_C,					// for text edit CTRL+C: copy
	GALGUI_KEY_V,					// for text edit CTRL+V: paste
	GALGUI_KEY_X,					// for text edit CTRL+X: cut
	GALGUI_KEY_Y,					// for text edit CTRL+Y: redo
	GALGUI_KEY_Z,					// for text edit CTRL+Z: undo
	GALGUI_KEY_COUNT
};

using GalGuiNavInput = int;	
// Enum: An input identifier for navigation
enum class EnumGalGuiNavInput : GalGuiNavInput
{
	// TODO 也许不需要
	GALGUI_NAV_INPUT_ACTIVATE,
	GALGUI_NAV_INPUT_CANCEL,
	GALGUI_NAV_INPUT_INPUT,
	GALGUI_NAV_INPUT_MENU,
	GALGUI_NAV_INPUT_DPAD_LEFT,
	GALGUI_NAV_INPUT_DPAD_RIGHT,
	GALGUI_NAV_INPUT_DPAD_UP,
	GALGUI_NAV_INPUT_DPAD_DOWN,
	GALGUI_NAV_INPUT_LSTICK_LEFT,
	GALGUI_NAV_INPUT_LSTICK_RIGHT,
	GALGUI_NAV_INPUT_LSTICK_UP,
	GALGUI_NAV_INPUT_LSTICK_DOWN,
	GALGUI_NAV_INPUT_FOCUS_PREV,
	GALGUI_NAV_INPUT_FOCUS_NEXT,
	GALGUI_NAV_INPUT_TWEAK_SLOW,
	GALGUI_NAV_INPUT_TWEAK_FAST,

	// [Internal]
	GALGUI_NAV_INPUT_KEY_MENU_,
	GALGUI_NAV_INPUT_KEY_LEFT,
	GALGUI_NAV_INPUT_KEY_RIGHT_,
	GALGUI_NAV_INPUT_KEY_UP_,
	GALGUI_NAV_INPUT_KEY_DOWN_,
	GALGUI_NAV_INPUT_COUNT,
	GALGUI_NAV_INPUT_INTERNAL_START_ = GALGUI_NAV_INPUT_KEY_MENU_
};

using GalGuiMouseButton = int;
// Enum: A mouse button identifier (0=left, 1=right, 2=middle)
enum class EnumGalGuiMouseButton : GalGuiMouseButton
{
	GALGUI_MOUSE_BUTTON_LEFT = 0,
	GALGUI_MOUSE_BUTTON_RIGHT = 1,
	GALGUI_MOUSE_BUTTON_MIDDLE = 2,
	GALGUI_MOUSE_BUTTON_X1 = 3,
	GALGUI_MOUSE_BUTTON_X2 = 4,
	GALGUI_MOUSE_BUTTON_COUNT = 5
};

using GalGuiMouseCursor = int;
// Enum: A mouse cursor identifier
enum class EnumGalGuiMouseCursor : GalGuiMouseCursor
{
	GALGUI_MOUSE_CURSOR_NONE = -1,
	GALGUI_MOUSE_CURSOR_ARROW = 0,
	GALGUI_MOUSE_CURSOR_TEXT_INPUT,
	GALGUI_MOUSE_CURSOR_RESIZE_ALL,
	GALGUI_MOUSE_CURSOR_RESIZE_HEIGHT,	// 在界面上下边界调整大小
	GALGUI_MOUSE_CURSOR_RESIZE_WIDTH,	// 在界面左右边界调整大小
	GALGUI_MOUSE_CURSOR_RESIZE_BL,		// 在界面左下角调整大小
	GALGUI_MOUSE_CURSOR_RESIZE_BR,		// 在界面右下角调整大小
	GALGUI_MOUSE_CURSOR_HAND,
	GALGUI_MOUSE_CURSOR_NOT_ALLOWED,
	GALGUI_MOUSE_CURSOR_COUNT
};

using GalGuiStyleVar = int;
// Enum: A variable identifier for styling
enum class EnumGalGuiStyleVar : GalGuiStyleVar
{
	GALGUI_STYLE_VAR_ALPHA,
	GALGUI_STYLE_VAR_WINDOW_PADDING,
	GALGUI_STYLE_VAR_WINDOW_ROUNDING,
	GALGUI_STYLE_VAR_WINDOW_BORDER_SIZE,
	GALGUI_STYLE_VAR_WINDOW_MIN_SIZE,
	GALGUI_STYLE_VAR_WINDOW_TITTLE_ALIGN,
	GALGUI_STYLE_VAR_CHILD_ROUNDING,
	GALGUI_STYLE_VAR_CHILD_BORDER_SIZE,
	GALGUI_STYLE_VAR_POPUP_ROUNDING,
	GALGUI_STYLE_VAR_POPUP_BORDER_SIZE,
	GALGUI_STYLE_VAR_FRAME_PADDING,
	GALGUI_STYLE_VAR_FRAME_ROUNDING,
	GALGUI_STYLE_VAR_FRAME_BORDER_SIZE,
	GALGUI_STYLE_VAR_ITEM_SPACING,
	GALGUI_STYLE_VAR_ITEM_INNER_SPACING,
	GALGUI_STYLE_VAR_INDENT_SPACING,
	GALGUI_STYLE_VAR_SCROLLBAR_SIZE,
	GALGUI_STYLE_VAR_SCROLLBAR_ROUNDING,
	GALGUI_STYLE_VAR_GRAB_MIN_SIZE,
	GALGUI_STYLE_VAR_GRAB_ROUNDING,
	GALGUI_STYLE_VAR_TAB_ROUNDING,
	GALGUI_STYLE_VAR_BUTTON_TEXT_ALIGN,
	GALGUI_STYLE_VAR_SELECTABLE_TEXT_ALIGN,
	GALGUI_STYLE_VAR_COUNT
};

using GalDrawCornerFlags = int;	
// Flags: for GalDrawList::AddRect(), AddRectFilled() etc.
enum class EnumGalDrawCornerFlags : GalDrawCornerFlags
{
	GAL_DRAW_CORNER_FLAGS_NONE = 0,
	GAL_DRAW_CORNER_FLAGS_TOP_LEFT = 1 << 0,
	GAL_DRAW_CORNER_FLAGS_TOP_RIGHT = 1 << 1,
	GAL_DRAW_CORNER_FLAGS_BOTTOM_LEFT = 1 << 2,
	GAL_DRAW_CORNER_FLAGS_BOTTOM_RIGHT = 1 << 3,
	GAL_DRAW_CORNER_FLAGS_TOP =
		GAL_DRAW_CORNER_FLAGS_TOP_LEFT |
		GAL_DRAW_CORNER_FLAGS_TOP_RIGHT,
	GAL_DRAW_CORNER_FLAGS_BOTTOM =
		GAL_DRAW_CORNER_FLAGS_BOTTOM_LEFT |
		GAL_DRAW_CORNER_FLAGS_BOTTOM_RIGHT,
	GAL_DRAW_CORNER_FLAGS_LEFT =
		GAL_DRAW_CORNER_FLAGS_TOP_LEFT |
		GAL_DRAW_CORNER_FLAGS_BOTTOM_LEFT,
	GAL_DRAW_CORNER_FLAGS_RIGHT =
		GAL_DRAW_CORNER_FLAGS_TOP_RIGHT |
		GAL_DRAW_CORNER_FLAGS_BOTTOM_RIGHT,
	
	GAL_DRAW_CORNER_FLAGS_ALL = 0XF
};
// operator
ENUM_FLAG_OPERATORS(EnumGalDrawCornerFlags, GalDrawCornerFlags)

using GalDrawListFlags = int;
// Flags: for GalDrawList
enum class EnumGalDrawListFlags : GalDrawListFlags
{
	GAL_DRAW_LIST_FLAGS_NONE = 0,
	GAL_DRAW_LIST_FLAGS_ANTIALIASED_LINE = 1 << 0,
	GAL_DRAW_LIST_FLAGS_ANTIALIASED_LINE_USE_TEX = 1 << 1,
	GAL_DRAW_LIST_FLAGS_ANTIALIASED_FILL = 1 << 2,
	GAL_DRAW_LIST_FLAGS_ALLOW_VERTEX_OFFSET = 1 << 3
};
// operator
ENUM_FLAG_OPERATORS(EnumGalDrawListFlags, GalDrawListFlags)

using GalFontAtlasFlags = int;
// Flags: for GalFontAtlas build
enum class EnumGalFontAtlasFlags : GalFontAtlasFlags
{
	GAL_FONT_ATLAS_FLAGS_NONE = 0,
	GAL_FONT_ATLAS_FLAGS_NO_POWER_OF_TWO_HEIGHT = 1 << 0,
	GAL_FONT_ATLAS_FLAGS_NO_MOUSE_CURSORS = 1 << 1,
	GAL_FONT_ATLAS_FLAGS_NO_BAKED_LINES = 1 << 2
};
// operator
ENUM_FLAG_OPERATORS(EnumGalFontAtlasFlags, GalFontAtlasFlags)

using GalGuiBackendFlags = int;	
// Flags: for io.BackendFlags
enum class EnumGalGuiBackendFlags : GalGuiBackendFlags
{
	GALGUI_BACKEND_FLAGS_NONE = 0,
	GALGUI_BACKEND_FLAGS_HAS_GAMEPAD = 1 << 0,
	GALGUI_BACKEND_FLAGS_HAS_MOUSE_CURSORS = 1 << 1,
	GALGUI_BACKEND_FLAGS_HAS_SET_MOUSE_POS = 1 << 2,
	GALGUI_BACKEND_FLAGS_RENDERER_HAS_VERTEX_OFFSET = 1 << 3
};
// operator
ENUM_FLAG_OPERATORS(EnumGalGuiBackendFlags, GalGuiBackendFlags)

using GalGuiColorEditFlags = int;
// Flags: for ColorEdit4(), ColorPicker4() etc.
enum class EnumGalGuiColorEditFlags : GalGuiColorEditFlags
{
	GALGUI_COLOR_EDIT_FLAGS_NONE = 0,
	GALGUI_COLOR_EDIT_FLAGS_NO_ALPHA = 1 << 1,
	GALGUI_COLOR_EDIT_FLAGS_NO_PICKER = 1 << 2,
	GALGUI_COLOR_EDIT_FLAGS_NO_OPTIONS = 1 << 3,
	GALGUI_COLOR_EDIT_FLAGS_NO_SMALL_PREVIEW = 1 << 4,
	GALGUI_COLOR_EDIT_FLAGS_NO_INPUT = 1 << 5,
	GALGUI_COLOR_EDIT_FLAGS_NO_TOOLTIP = 1 << 6,
	GALGUI_COLOR_EDIT_FLAGS_NO_LABEL = 1 << 7,
	GALGUI_COLOR_EDIT_FLAGS_NO_SIDE_PREVIEW = 1 << 8,
	GALGUI_COLOR_EDIT_FLAGS_NO_DRAG_DROP = 1 << 9,
	GALGUI_COLOR_EDIT_FLAGS_NO_BORDER = 1 << 10,

	GALGUI_COLOR_EDIT_FLAGS_ALPHA_BAR = 1 << 16,
	GALGUI_COLOR_EDIT_FLAGS_ALPHA_PREVIEW = 1 << 17,
	GALGUI_COLOR_EDIT_FLAGS_ALPHA_PREVIEW_HALF = 1 << 18,
	GALGUI_COLOR_EDIT_FLAGS_HDR = 1 << 19,
	GALGUI_COLOR_EDIT_FLAGS_DISPLAY_RGB = 1 << 20,
	GALGUI_COLOR_EDIT_FLAGS_DISPLAY_HSV = 1 << 21,
	GALGUI_COLOR_EDIT_FLAGS_DISPLAY_HEX = 1 << 22,
	GALGUI_COLOR_EDIT_FLAGS_UINT8 = 1 << 23,
	GALGUI_COLOR_EDIT_FLAGS_FLOAT = 1 << 24,
	GALGUI_COLOR_EDIT_FLAGS_PICKER_HUE_BAR = 1 << 25,
	GALGUI_COLOR_EDIT_FLAGS_PICKER_HUE_WHEEL = 1 << 26,
	GALGUI_COLOR_EDIT_FLAGS_INPUT_RGB = 1 << 27,
	GALGUI_COLOR_EDIT_FLAGS_INPUT_HSV = 1 << 28,

	// Defaults Options
	GALGUI_COLOR_EDIT_FLAGS_OPTION_DEFAULT = 
		GALGUI_COLOR_EDIT_FLAGS_UINT8 | 
		GALGUI_COLOR_EDIT_FLAGS_DISPLAY_RGB | 
		GALGUI_COLOR_EDIT_FLAGS_INPUT_RGB | 
		GALGUI_COLOR_EDIT_FLAGS_PICKER_HUE_BAR,
	
	// [Internal] Masks
	GALGUI_COLOR_EDIT_FLAGS_DISPLAY_MASK =
		GALGUI_COLOR_EDIT_FLAGS_DISPLAY_RGB |
		GALGUI_COLOR_EDIT_FLAGS_DISPLAY_HSV |
		GALGUI_COLOR_EDIT_FLAGS_DISPLAY_HEX,
	GALGUI_COLOR_EDIT_FLAGS_DATA_TYPE_MASK =
		GALGUI_COLOR_EDIT_FLAGS_UINT8 | 
		GALGUI_COLOR_EDIT_FLAGS_FLOAT,
	GALGUI_COLOR_EDIT_FLAGS_PICKER_MASK =
		GALGUI_COLOR_EDIT_FLAGS_PICKER_HUE_WHEEL | 
		GALGUI_COLOR_EDIT_FLAGS_PICKER_HUE_BAR,
	GALGUI_COLOR_EDIT_FLAGS_INPUT_MASK =
		GALGUI_COLOR_EDIT_FLAGS_INPUT_RGB | 
		GALGUI_COLOR_EDIT_FLAGS_INPUT_HSV
};
// operator
ENUM_FLAG_OPERATORS(EnumGalGuiColorEditFlags, GalGuiColorEditFlags)

using GalGuiConfigFlags = int;
// Flags: for io.ConfigFlags
enum class EnumGalGuiConfigFlags : GalGuiConfigFlags
{
	GALGUI_CONFIG_FLAGS_NONE = 0,
	GALGUI_CONFIG_FLAGS_NAV_ENABLE_KEYBOARD = 1 << 0,
	GALGUI_CONFIG_FLAGS_NAV_ENABLE_GAMEPAD = 1 << 1,
	GALGUI_CONFIG_FLAGS_NAV_ENABLE_SET_MOUSE_POS = 1 << 2,
	GALGUI_CONFIG_FLAGS_NAV_NO_CAPTURE_KEYBOARD = 1 << 3,
	GALGUI_CONFIG_FLAGS_NO_MOUSE = 1 << 4,
	GALGUI_CONFIG_FLAGS_NO_MOUSE_CURSOR_CHANGE = 1 << 5,
};
// operator
ENUM_FLAG_OPERATORS(EnumGalGuiConfigFlags, GalGuiConfigFlags)

using GalGuiComboFlags = int;
// Flags: for BeginCombo()
enum class EnumGalGuiComboFlags : GalGuiComboFlags
{
	GALGUI_COMBO_FLAGS_NONE = 0,
	GALGUI_COMBO_FLAGS_POPUP_ALIGN_LEFT = 1 << 0,
	GALGUI_COMBO_FLAGS_HEIGHT_SMALL = 1 << 1,
	GALGUI_COMBO_FLAGS_HEIGHT_REGULAR = 1 << 2,
	GALGUI_COMBO_FLAGS_HEIGHT_LARGE = 1 << 3,
	GALGUI_COMBO_FLAGS_HEIGHT_LARGEST = 1 << 4,
	GALGUI_COMBO_FLAGS_NO_ARROW_BUTTON = 1 << 5,
	GALGUI_COMBO_FLAGS_NO_PREVIEW = 1 << 6,
	GALGUI_COMBO_FLAGS_HEIGHT_MASK =
		GALGUI_COMBO_FLAGS_HEIGHT_SMALL |
		GALGUI_COMBO_FLAGS_HEIGHT_REGULAR |
		GALGUI_COMBO_FLAGS_HEIGHT_LARGE |
		GALGUI_COMBO_FLAGS_HEIGHT_LARGEST
};
// operator
ENUM_FLAG_OPERATORS(EnumGalGuiComboFlags, GalGuiComboFlags)

using GalGuiDragDropFlags = int;
// Flags: for BeginDragDropSource(), AcceptDragDropPayload()
enum class EnumGalGuiDragDropFlags : GalGuiDragDropFlags
{
	GALGUI_DRAG_DROP_FLAGS_NONE = 0,
	
	GALGUI_DRAG_DROP_FLAGS_SOURCE_NO_PREVIEW_TOOLTIP = 1 << 0,
	GALGUI_DRAG_DROP_FLAGS_SOURCE_NO_DISABLE_HOVER = 1 << 1,
	GALGUI_DRAG_DROP_FLAGS_SOURCE_NO_HOLD_TO_OPEN_OTHERS = 1 << 2,
	GALGUI_DRAG_DROP_FLAGS_SOURCE_ALLOW_NULL_ID = 1 << 3,
	GALGUI_DRAG_DROP_FLAGS_SOURCE_EXTERN = 1 << 4,
	GALGUI_DRAG_DROP_FLAGS_SOURCE_AUTO_EXPIRE_PAYLOAD = 1 << 5,

	GALGUI_DRAG_DROP_FLAGS_ACCEPT_BEFORE_DELIVERY = 1 << 10,
	GALGUI_DRAG_DROP_FLAGS_ACCEPT_NO_DRAW_DEFAULT_RECT = 1 << 11,
	GALGUI_DRAG_DROP_FLAGS_ACCEPT_NO_PREVIEW_TOOLTIP = 1 << 12,
	GALGUI_DRAG_DROP_FLAGS_ACCEPT_PEEK_ONLY = 
		GALGUI_DRAG_DROP_FLAGS_ACCEPT_BEFORE_DELIVERY | 
		GALGUI_DRAG_DROP_FLAGS_ACCEPT_NO_DRAW_DEFAULT_RECT
};
// operator
ENUM_FLAG_OPERATORS(EnumGalGuiDragDropFlags, GalGuiDragDropFlags)

using GalGuiFocusedFlags = int;
// Flags: for IsWindowFocused()
enum class EnumGalGuiFocusedFlags : GalGuiFocusedFlags
{
	GALGUI_FOCUSED_FLAGS_NONE = 0,
	GALGUI_FOCUSED_FLAGS_CHILD_WINDOWS = 1 << 0,
	GALGUI_FOCUSED_FLAGS_ROOT_WINDOW = 1 << 1,
	GALGUI_FOCUSED_FLAGS_ANY_WINDOW = 1 << 2,
	GALGUI_FOCUSED_FLAGS_ROOT_AND_CHILD_WINDOWS =
		GALGUI_FOCUSED_FLAGS_CHILD_WINDOWS |
		GALGUI_FOCUSED_FLAGS_ROOT_WINDOW
};
// operator
ENUM_FLAG_OPERATORS(EnumGalGuiFocusedFlags, GalGuiFocusedFlags)

using GalGuiHoveredFlags = int;
// Flags: for IsItemHovered(), IsWindowHovered() etc.
enum class EnumGalGuiHoveredFlags : GalGuiHoveredFlags
{
	GALGUI_HOVERED_FLAGS_NONE = 0,
	GALGUI_HOVERED_FLAGS_CHILD_WINDOWS = 1 << 0,
	GALGUI_HOVERED_FLAGS_ROOT_WINDOW = 1 << 1,
	GALGUI_HOVERED_FLAGS_ANY_WINDOW = 1 << 2,
	GALGUI_HOVERED_FLAGS_ALLOW_WHEN_BLOCKED_BY_POPUP = 1 << 3,
	// GALGUI_HOVERED_FLAGS_ALLOW_WHEN_BLOCKED_BY_MODAL = 1 << 4,
	GALGUI_HOVERED_FLAGS_ALLOW_WHEN_BLOCKED_BY_ACTIVE_ITEM = 1 << 5,
	GALGUI_HOVERED_FLAGS_ALLOW_WHEN_OVERLAPPED = 1 << 6,
	GALGUI_HOVERED_FLAGS_ALLOW_WHEN_DISABLE = 1 << 7,
	GALGUI_HOVERED_FLAGS_RECT_ONLY =
		GALGUI_HOVERED_FLAGS_ALLOW_WHEN_BLOCKED_BY_POPUP |
		GALGUI_HOVERED_FLAGS_ALLOW_WHEN_BLOCKED_BY_ACTIVE_ITEM |
		GALGUI_HOVERED_FLAGS_ALLOW_WHEN_OVERLAPPED,
	GALGUI_HOVERED_FLAGS_ROOT_AND_CHILD_WINDOWS =
		GALGUI_HOVERED_FLAGS_ROOT_WINDOW | 
		GALGUI_HOVERED_FLAGS_CHILD_WINDOWS 
};
// operator
ENUM_FLAG_OPERATORS(EnumGalGuiHoveredFlags, GalGuiHoveredFlags)

using GalGuiInputTextFlags = int;
// Flags: for InputText(), InputTextMultiline()
enum class EnumGalGuiInputTextFlags : GalGuiInputTextFlags
{
	GALGUI_INPUT_TEXT_FLAGS_NONE = 0,
	GALGUI_INPUT_TEXT_FLAGS_CHARS_DECIMAL = 1 << 0,
	GALGUI_INPUT_TEXT_FLAGS_CHARS_HEXADECIMAL = 1 << 1,
	GALGUI_INPUT_TEXT_FLAGS_CHARS_UPPERCASE = 1 << 2,
	GALGUI_INPUT_TEXT_FLAGS_CHARS_NO_BLANK = 1 << 3,
	GALGUI_INPUT_TEXT_FLAGS_AUTO_SELECT_ALL = 1 << 4,
	GALGUI_INPUT_TEXT_FLAGS_ENTER_RETURNS_TRUE = 1 << 5,
	GALGUI_INPUT_TEXT_FLAGS_CALLBACK_COMPLETION = 1 << 6,
	GALGUI_INPUT_TEXT_FLAGS_CALLBACK_HISTORY = 1 << 7,
	GALGUI_INPUT_TEXT_FLAGS_CALLBACK_ALWAYS = 1 << 8,
	GALGUI_INPUT_TEXT_FLAGS_CALLBACK_CHAR_FILTER = 1 << 9,
	GALGUI_INPUT_TEXT_FLAGS_ALLOW_TAB_INPUT = 1 << 10,
	GALGUI_INPUT_TEXT_FLAGS_CTRL_ENTER_FOR_NEW_LINE = 1 << 11,
	GALGUI_INPUT_TEXT_FLAGS_NO_HORIZONTAL_SCROLL = 1 << 12,
	GALGUI_INPUT_TEXT_FLAGS_ALWAYS_INSERT_MODE = 1 << 13,
	GALGUI_INPUT_TEXT_FLAGS_READONLY = 1 << 14,
	GALGUI_INPUT_TEXT_FLAGS_PASSWORD = 1 << 15,
	GALGUI_INPUT_TEXT_FLAGS_NO_UNDO_REDO = 1 << 16,
	GALGUI_INPUT_TEXT_FLAGS_CHARS_SCIENTIFIC = 1 << 17,
	GALGUI_INPUT_TEXT_FLAGS_CALLBACK_RESIZE = 1 << 18,

	// [Internal]
	GALGUI_INPUT_TEXT_FLAGS_MULTILINE = 1 << 20,
	GALGUI_INPUT_TEXT_FLAGS_NO_MARK_EDITED = 1 << 21
};
// operator
ENUM_FLAG_OPERATORS(EnumGalGuiInputTextFlags, GalGuiInputTextFlags)

using GalGuiKeyModFlags = int;
// Flags: for io.keyMods (Ctrl/Shift/Alt/Super)
enum class EnumGalGuiKeyModFlags : GalGuiKeyModFlags
{
	GALGUI_KEY_MOD_FLAGS_NONE = 0,
	GALGUI_KEY_MOD_FLAGS_CTRL = 1 << 0,
	GALGUI_KEY_MOD_FLAGS_SHIFT = 1 << 1,
	GALGUI_KEY_MOD_FLAGS_ALT = 1 << 2,
	GALGUI_KEY_MOD_FLAGS_SUPER = 1 << 3
};
// operator
ENUM_FLAG_OPERATORS(EnumGalGuiKeyModFlags, GalGuiKeyModFlags)

using GalGuiPopupFlags = int;
// Flags: for OpenPopup*(), BeginPopupContext*(), IsPopupOpen()
enum class EnumGalGuiPopupFlags : GalGuiPopupFlags
{
	GALGUI_POPUP_FLAGS_NONE = 0,
	GALGUI_POPUP_FLAGS_MOUSE_BUTTON_LEFT = 0,
	GALGUI_POPUP_FLAGS_MOUSE_BUTTON_RIGHT = 1,
	GALGUI_POPUP_FLAGS_MOUSE_BUTTON_MIDDLE = 2,
	GALGUI_POPUP_FLAGS_MOUSE_BUTTON_MASK = 0X1F,
	GALGUI_POPUP_FLAGS_MOUSE_BUTTON_DEFAULT = 1,
	GALGUI_POPUP_FLAGS_NO_OPEN_OVER_EXISTING_POPUP = 1 << 5,
	GALGUI_POPUP_FLAGS_NO_OPEN_OVER_ITEMS = 1 << 6,
	GALGUI_POPUP_FLAGS_ANY_POPUP_ID = 1 << 7,
	GALGUI_POPUP_FLAGS_ANY_POPUP_LEVEL = 1 << 8,
	GALGUI_POPUP_FLAGS_ANY_POPUP =
		GALGUI_POPUP_FLAGS_ANY_POPUP_ID |
		GALGUI_POPUP_FLAGS_ANY_POPUP_LEVEL
};
// operator
ENUM_FLAG_OPERATORS(EnumGalGuiPopupFlags, GalGuiPopupFlags)

using GalGuiSelectableFlags = int;
// Flags: for Selectable()
enum class EnumGalGuiSelectableFlags : GalGuiSelectableFlags
{
	GALGUI_SELECTABLE_FLAGS_NONE = 0,
	GALGUI_SELECTABLE_FLAGS_DOT_NO_CLOSE_POPUP = 1 << 0,
	GALGUI_SELECTABLE_FLAGS_SPAN_ALL_COLUMNS = 1 << 1,
	GALGUI_SELECTABLE_FLAGS_ALLOW_DOUBLE_CLICK = 1 << 2,
	GALGUI_SELECTABLE_FLAGS_DISABLE = 1 << 3,
	GALGUI_SELECTABLE_FLAGS_ALLOW_ITEM_OVERLAP = 1 << 4
};
// operator
ENUM_FLAG_OPERATORS(EnumGalGuiSelectableFlags, GalGuiSelectableFlags)
enum class EnumGalGuiSelectablePrivateFlags : GalGuiSelectableFlags
{
	GALGUI_SELECTABLE_PRIVATE_FLAGS_NO_HOLDING_ACTIVE_ID = 1 << 20,
	GALGUI_SELECTABLE_PRIVATE_FLAGS_SELECT_ON_CLICK = 1 << 21,
	GALGUI_SELECTABLE_PRIVATE_FLAGS_SELECT_ON_RELEASE = 1 << 22,
	GALGUI_SELECTABLE_PRIVATE_FLAGS_SPAN_AVAIL_WIDTH = 1 << 23,
	GALGUI_SELECTABLE_PRIVATE_FLAGS_DRAW_HOVERED_WHEN_HELD = 1 << 24,
	GALGUI_SELECTABLE_PRIVATE_FLAGS_SET_NAV_ID_ON_HOVER = 1 << 25
};
// operator
ENUM_FLAG_OPERATORS(EnumGalGuiSelectablePrivateFlags, GalGuiSelectableFlags)

using GalGuiTabBarFlags = int;
// Flags: for BeginTabBar()
enum class EnumGalGuiTabBarFlags : GalGuiTabBarFlags
{
	GALGUI_TAB_BAR_FLAGS_NONE = 0,
	GALGUI_TAB_BAR_FLAGS_REORDER = 1 << 0,
	GALGUI_TAB_BAR_FLAGS_AUTO_SELECT_NEW_TABS = 1 << 1,
	GALGUI_TAB_BAR_FLAGS_TAB_LIST_POPUP_BUTTON = 1 << 2,
	GALGUI_TAB_BAR_FLAGS_NO_CLOSE_WITH_MIDDLE_MOUSE_BUTTON = 1 << 3,
	GALGUI_TAB_BAR_FLAGS_NO_TAB_LIST_SCROLLING_BUTTONS = 1 << 4,
	GALGUI_TAB_BAR_FLAGS_NO_TOOLTIP = 1 << 5,
	GALGUI_TAB_BAR_FLAGS_FITTING_POLICY_RESIZE_DOWN = 1 << 6,
	GALGUI_TAB_BAR_FLAGS_FITTING_POLICY_SCROLL = 1 << 7,
	GALGUI_TAB_BAR_FLAGS_FITTING_POLICY_MASK =
		GALGUI_TAB_BAR_FLAGS_FITTING_POLICY_RESIZE_DOWN |
		GALGUI_TAB_BAR_FLAGS_FITTING_POLICY_SCROLL,
	GALGUI_TAB_BAR_FLAGS_FITTING_POLICY_DEFAULT = GALGUI_TAB_BAR_FLAGS_FITTING_POLICY_RESIZE_DOWN
};
// operator
ENUM_FLAG_OPERATORS(EnumGalGuiTabBarFlags, GalGuiTabBarFlags)
enum class EnumGalGuiTabBarPrivateFlags : GalGuiTabBarFlags
{
	GALGUI_TAB_BAR_PRIVATE_FLAGS_DOCK_NODE = 1 << 20,
	GALGUI_TAB_BAR_PRIVATE_FLAGS_IS_FOCUSED = 1 << 21,
	GALGUI_TAB_BAR_PRIVATE_FLAGS_SAVE_SETTINGS = 1 << 22
};
// operator
ENUM_FLAG_OPERATORS(EnumGalGuiTabBarPrivateFlags, GalGuiTabBarFlags)

using GalGuiTabItemFlags = int;
// Flags: for BeginTabItem()
enum class EnumGalGuiTabItemFlags : GalGuiTabItemFlags
{
	GALGUI_TAB_ITEM_FLAGS_NONE = 0,
	GALGUI_TAB_ITEM_FLAGS_UNSAVED_DOCUMENT = 1 << 0,
	GALGUI_TAB_ITEM_FLAGS_SET_SELECTED = 1 << 1,
	GALGUI_TAB_ITEM_FLAGS_NO_CLOSE_WITH_MIDDLE_MOUSE_BUTTON = 1 << 2,
	GALGUI_TAB_ITEM_FLAGS_NO_PUSH_ID = 1 << 3,
	GALGUI_TAB_ITEM_FLAGS_NO_TOOLTIP = 1 << 4
};
// operator
ENUM_FLAG_OPERATORS(EnumGalGuiTabItemFlags, GalGuiTabItemFlags)
enum class EnumGalGuiTabItemPrivateFlags : GalGuiTabItemFlags
{
	GALGUI_TAB_ITEM_FLAGS_NO_CLOSE_BUTTON = 1 << 20
};
// operator
ENUM_FLAG_OPERATORS(EnumGalGuiTabItemPrivateFlags, GalGuiTabItemFlags)

using GalGuiTreeNodeFlags = int;
// Flags: for TreeNode(), TreeNodeEx(), CollapsingHeader()
enum class EnumGalGuiTreeNodeFlags : GalGuiTreeNodeFlags
{
	GALGUI_TREE_NODE_FLAGS_NONE = 0,
	GALGUI_TREE_NODE_FLAGS_SELECTED = 1 << 0,
	GALGUI_TREE_NODE_FLAGS_FRAMED = 1 << 1,
	GALGUI_TREE_NODE_FLAGS_ALLOW_ITEM_OVERLAP = 1 << 2,
	GALGUI_TREE_NODE_FLAGS_NO_TREE_PUSH_ON_OPEN = 1 << 3,
	GALGUI_TREE_NODE_FLAGS_NO_AUTO_OPEN_ON_LOG = 1 << 4,
	GALGUI_TREE_NODE_FLAGS_DEFAULT_OPEN = 1 << 5,
	GALGUI_TREE_NODE_FLAGS_OPEN_ON_DOUBLE_CLICK = 1 << 6,
	GALGUI_TREE_NODE_FLAGS_OPEN_ON_ARROW  = 1 << 7,
	GALGUI_TREE_NODE_FLAGS_LEAF = 1 << 8,
	GALGUI_TREE_NODE_FLAGS_BULLET = 1 << 9,
	GALGUI_TREE_NODE_FLAGS_FRAME_PADDING = 1 << 10,
	GALGUI_TREE_NODE_FLAGS_SPAN_AVAIL_WIDTH = 1 << 11,
	GALGUI_TREE_NODE_FLAGS_SPAN_FULL_WIDTH = 1 < 12,
	GALGUI_TREE_NODE_FLAGS_NAV_LEFT_JUMPS_BACK_HERE = 1 << 13,
	// GALGUI_TREE_NODE_FLAGS_NO_SCROLL_ON_OPEN = 1 << 14,
	GALGUI_TREE_NODE_FLAGS_COLLAPSING_HEADER =
		GALGUI_TREE_NODE_FLAGS_FRAMED |
		GALGUI_TREE_NODE_FLAGS_NO_TREE_PUSH_ON_OPEN |
		GALGUI_TREE_NODE_FLAGS_NO_AUTO_OPEN_ON_LOG
};
// operator
ENUM_FLAG_OPERATORS(EnumGalGuiTreeNodeFlags, GalGuiTreeNodeFlags)
enum class EnumGalGuiTreeNodePrivateFlags : GalGuiTreeNodeFlags
{
	GALGUI_TREE_NODE_PRIVATE_FLAGS_CLIP_LABEL_FOR_TRAILING_BUTTON = 1 << 20
};
// operator
ENUM_FLAG_OPERATORS(EnumGalGuiTreeNodePrivateFlags, GalGuiTreeNodeFlags)

using GalGuiWindowFlags = int;
// Flags: for Begin(), BeginChild()
enum class EnumGalGuiWindowFlags : GalGuiWindowFlags
{
	GALGUI_WINDOW_FLAGS_NONE = 0,
	GALGUI_WINDOW_FLAGS_NO_TITLEBAR = 1 << 0,
	GALGUI_WINDOW_FLAGS_NO_RESIZE = 1 << 1,
	GALGUI_WINDOW_FLAGS_NO_MOVE = 1 << 2,
	GALGUI_WINDOW_FLAGS_NO_SCROLLBAR = 1 << 3,
	GALGUI_WINDOW_FLAGS_NO_SCROLL_WITH_MOUSE = 1 << 4,
	GALGUI_WINDOW_FLAGS_NO_COLLAPSE = 1 << 5,
	GALGUI_WINDOW_FLAGS_ALWAYS_AUTO_RESIZE = 1 << 6,
	GALGUI_WINDOW_FLAGS_NO_BACKGROUND = 1 << 7,
	GALGUI_WINDOW_FLAGS_NO_SAVED_SETTING = 1 << 8,
	GALGUI_WINDOW_FLAGS_NO_MOUSE_INPUT = 1 << 9,
	GALGUI_WINDOW_FLAGS_MENU_BAR = 1 << 10,
	GALGUI_WINDOW_FLAGS_HORIZONTAL_SCROLLBAR = 1 << 11,
	GALGUI_WINDOW_FLAGS_NO_FOCUS_ON_APPEARING = 1 << 12,
	GALGUI_WINDOW_FLAGS_NO_BRING_TO_FRONT_ON_FOCUS = 1 << 13,
	GALGUI_WINDOW_FLAGS_ALWAYS_VERTICAL_SCROLLBAR = 1 << 14,
	GALGUI_WINDOW_FLAGS_ALWAYS_HORIZONTAL_SCROLLBAR = 1 << 15,
	GALGUI_WINDOW_FLAGS_ALWAYS_USE_WINDOW_PADDING = 1 << 16,
	GALGUI_WINDOW_FLAGS_NO_NAV_INPUTS = 1 << 18,
	GALGUI_WINDOW_FLAGS_NO_NAV_FOCUS = 1 << 19,
	GALGUI_WINDOW_FLAGS_UNSAVED_DOCUMENT = 1 << 20,
	GALGUI_WINDOW_FLAGS_NO_NAV = 
		GALGUI_WINDOW_FLAGS_NO_NAV_INPUTS | 
		GALGUI_WINDOW_FLAGS_NO_NAV_FOCUS,
	GALGUI_WINDOW_FLAGS_NO_DECORATION =
		GALGUI_WINDOW_FLAGS_NO_TITLEBAR |
		GALGUI_WINDOW_FLAGS_NO_RESIZE |
		GALGUI_WINDOW_FLAGS_NO_SCROLLBAR |
		GALGUI_WINDOW_FLAGS_NO_COLLAPSE,
	GALGUI_WINDOW_FLAGS_NO_INPUTS =
		GALGUI_WINDOW_FLAGS_NO_MOUSE_INPUT |
		GALGUI_WINDOW_FLAGS_NO_NAV_INPUTS |
		GALGUI_WINDOW_FLAGS_NO_NAV_FOCUS,

	// [Internal]
	GALGUI_WINDOW_FLAGS_NAV_FLATTENED = 1 << 23,
	GALGUI_WINDOW_FLAGS_CHILD_WINDOW = 1 << 24,
	GALGUI_WINDOW_FLAGS_TOOLTIP = 1 << 25,
	GALGUI_WINDOW_FLAGS_POPUP = 1 << 26,
	GALGUI_WINDOW_FLAGS_MODAL = 1 << 27,
	GALGUI_WINDOW_FLAGS_CHILD_MENU = 1 << 28
};
// operator
ENUM_FLAG_OPERATORS(EnumGalGuiWindowFlags, GalGuiWindowFlags)

using GalGuiLayoutType = int;
enum class EnumGalGuiLayoutType : GalGuiLayoutType
{
	GALGUI_LAYOUT_TYPE_HORIZONTAL = 0,
	GALGUI_LAYOUT_TYPE_VERTICAL = 1
};

using GalGuiButtonFlags = int;
enum class EnumGalGuiButtonFlags : GalGuiButtonFlags
{
	GALGUI_BUTTON_FLAGS_NONE = 0,
	GALGUI_BUTTON_FLAGS_REPEAT = 1 << 0,
	GALGUI_BUTTON_FLAGS_PRESSED_ON_CLICK = 1 << 1,
	GALGUI_BUTTON_FLAGS_PRESSED_ON_CLICK_RELEASE = 1 << 2,
	GALGUI_BUTTON_FLAGS_PRESSED_ON_CLICK_RELEASE_ANY_WHERE = 1 << 3,
	GALGUI_BUTTON_FLAGS_PRESSED_ON_RELEASE = 1 << 4,
	GALGUI_BUTTON_FLAGS_PRESSED_ON_DOUBLE_CLICK = 1 << 5,
	GALGUI_BUTTON_FLAGS_PRESSED_ON_DRAG_DROP_HOLD = 1 << 6,
	GALGUI_BUTTON_FLAGS_FLATTEN_CHILDREN = 1 << 7,
	GALGUI_BUTTON_FLAGS_ALLOW_ITEM_OVERLAP = 1 << 8,
	GALGUI_BUTTON_FLAGS_DO_NOT_CLOSE_POPUPS = 1 << 9,
	GALGUI_BUTTON_FLAGS_DISABLE = 1 << 10,
	GALGUI_BUTTON_FLAGS_ALIGN_TEXT_BASE_LINE = 1 << 11,
	GALGUI_BUTTON_FLAGS_NO_KEY_MODIFIERS = 1 << 12,
	GALGUI_BUTTON_FLAGS_NO_HOLDING_ACTIVE_ID = 1 << 13,
	GALGUI_BUTTON_FLAGS_NO_NAV_FOCUS = 1 << 14,
	GALGUI_BUTTON_FLAGS_NO_HOVERED_ON_FOCUS = 1 << 15,
	GALGUI_BUTTON_FLAGS_MOUSE_BUTTON_LEFT = 1 << 16,
	GALGUI_BUTTON_FLAGS_MOUSE_BUTTON_RIGHT = 1 << 17,
	GALGUI_BUTTON_FLAGS_MOUSE_BUTTON_MIDDLE = 1 << 18,

	GALGUI_BUTTON_FLAGS_MOUSE_BUTTON_MASK =
		GALGUI_BUTTON_FLAGS_MOUSE_BUTTON_LEFT |
		GALGUI_BUTTON_FLAGS_MOUSE_BUTTON_RIGHT |
		GALGUI_BUTTON_FLAGS_MOUSE_BUTTON_MIDDLE,
	GALGUI_BUTTON_FLAGS_MOUSE_BUTTON_SHIFT = 1 << 4,
	GALGUI_BUTTON_FLAGS_DEFAULT = GALGUI_BUTTON_FLAGS_MOUSE_BUTTON_LEFT,
	GALGUI_BUTTON_FLAGS_PRESSED_ON_MASK =
		GALGUI_BUTTON_FLAGS_PRESSED_ON_CLICK |
		GALGUI_BUTTON_FLAGS_PRESSED_ON_CLICK_RELEASE |
		GALGUI_BUTTON_FLAGS_PRESSED_ON_CLICK_RELEASE_ANY_WHERE |
		GALGUI_BUTTON_FLAGS_PRESSED_ON_RELEASE |
		GALGUI_BUTTON_FLAGS_PRESSED_ON_DOUBLE_CLICK |
		GALGUI_BUTTON_FLAGS_PRESSED_ON_DRAG_DROP_HOLD,
	GALGUI_BUTTON_FLAGS_PRESSED_ON_DEFAULT = GALGUI_BUTTON_FLAGS_PRESSED_ON_CLICK_RELEASE
};
// operator
ENUM_FLAG_OPERATORS(EnumGalGuiButtonFlags, GalGuiButtonFlags)

using GalGuiColumnsFlags = int;
enum class EnumGalGuiColumnsFlags : GalGuiColumnsFlags
{
	GALGUI_COLUMNS_FLAGS_NONE = 0,
	GALGUI_COLUMNS_FLAGS_NO_BORDER = 1 << 0,
	GALGUI_COLUMNS_FLAGS_NO_RESIZE = 1 << 1,
	GALGUI_COLUMNS_FLAGS_NO_PRESERVE_WIDTHS = 1 << 2,
	GALGUI_COLUMNS_FLAGS_NO_FORCE_WITHIN_WINDOW = 1 << 3,
	GALGUI_COLUMNS_FLAGS_GROW_PARENT_CONTENT_SIZE = 1 << 4
};
// operator
ENUM_FLAG_OPERATORS(EnumGalGuiColumnsFlags, GalGuiColumnsFlags)

using GalGuiDragFlags = int;
enum class EnumGalGuiDragFlags : GalGuiDragFlags
{
	GALGUI_DRAG_FLAGS_NONE = 0,
	GALGUI_DRAG_FLAGS_VERTICAL = 1 << 0
};
// operator
ENUM_FLAG_OPERATORS(EnumGalGuiDragFlags, GalGuiDragFlags)

using GalGuiItemFlags = int;
// Transient per-window flags, reset at the beginning of the frame. For child window, inherited from parent on first Begin().
enum class EnumGalGuiItemFlags : GalGuiItemFlags
{
	GALGUI_ITEM_FLAGS_NONE = 0,
	GALGUI_ITEM_FLAGS_NO_TAB_STOP = 1 << 0,
	GALGUI_ITEM_FLAGS_BUTTON_REPEAT = 1 << 1,
	GALGUI_ITEM_FLAGS_DISABLE = 1 << 2,
	GALGUI_ITEM_FLAGS_NO_NAV = 1 << 3,
	GALGUI_ITEM_FLAGS_NO_NAV_DEFAULT_FOCUS = 1 << 4,
	GALGUI_ITEM_FLAGS_SELECTABLE_DO_NOT_CLOSE_POPUP = 1 << 5,
	GALGUI_ITEM_FLAGS_MIXED_VALUE = 1 << 6,
	GALGUI_ITEM_FLAGS_READONLY = 1 << 7,
	GALGUI_ITEM_FLAGS_DEFAULT = 0
};
// operator
ENUM_FLAG_OPERATORS(EnumGalGuiItemFlags, GalGuiItemFlags)

using GalGuiItemStatusFlags = int;
// Storage for LastItem data
enum class EnumGalGuiItemStatusFlags : GalGuiItemStatusFlags
{
	GALGUI_ITEM_STATUS_FLAGS_NONE = 0,
	GALGUI_ITEM_STATUS_FLAGS_HOVERED_RECT = 1 << 0,
	GALGUI_ITEM_STATUS_FLAGS_HAS_DISPLAY_RECT = 1 << 1,
	GALGUI_ITEM_STATUS_FLAGS_EDITED = 1 << 2,
	GALGUI_ITEM_STATUS_FLAGS_TOGGLED_SELECTION = 1 << 3,
	GALGUI_ITEM_STATUS_FLAGS_TOGGLED_OPEN = 1 << 4,
	GALGUI_ITEM_STATUS_FLAGS_HAS_DEACTIVATED = 1 << 5,
	GALGUI_ITEM_STATUS_FLAGS_DEACTIVATED = 1 << 6
};
// operator
ENUM_FLAG_OPERATORS(EnumGalGuiItemStatusFlags, GalGuiItemStatusFlags)

using GalGuiNavHighlightFlags = int;
enum class EnumGalGuiNavHighlightFlags : GalGuiNavHighlightFlags
{
	GALGUI_NAV_HIGHLIGHT_FLAGS_NONE = 0,
	GALGUI_NAV_HIGHLIGHT_FLAGS_TYPE_DEFAULT = 1 << 0,
	GALGUI_NAV_HIGHLIGHT_FLAGS_TYPE_THIN = 1 << 1,
	GALGUI_NAV_HIGHLIGHT_FLAGS_ALWAYS_DRAW = 1 << 2,
	GALGUI_NAV_HIGHLIGHT_FLAGS_NO_ROUNDING = 1 << 3
};
// operator
ENUM_FLAG_OPERATORS(EnumGalGuiNavHighlightFlags, GalGuiNavHighlightFlags)

using GalGuiNavDirSourceFlags = int;
enum class EnumGalGuiNavDirSourceFlags : GalGuiNavDirSourceFlags
{
	GALGUI_NAV_DIR_SOURCE_FLAGS_NONE = 0,
	GALGUI_NAV_DIR_SOURCE_FLAGS_KEYBOARD = 1 << 0,
	GALGUI_NAV_DIR_SOURCE_FLAGS_PAD_DPAD = 1 << 1,
	GALGUI_NAV_DIR_SOURCE_FLAGS_PAD_LSTICK = 1 << 2
};
// operator
ENUM_FLAG_OPERATORS(EnumGalGuiNavDirSourceFlags, GalGuiNavDirSourceFlags)

using GalGuiNavMoveFlags = int;
enum class EnumGalGuiNavMoveFlags : GalGuiNavMoveFlags
{
	GALGUI_NAV_MOVE_FLAGS_NONE = 0,
	GALGUI_NAV_MOVE_FLAGS_LOOP_X = 1 << 0,
	GALGUI_NAV_MOVE_FLAGS_LOOP_Y = 1 << 1,
	GALGUI_NAV_MOVE_FLAGS_WARP_X = 1 << 2,
	GALGUI_NAV_MOVE_FLAGS_WARP_Y = 1 << 3,
	GALGUI_NAV_MOVE_FLAGS_ALLOW_CURRENT_NAV_ID = 1 << 4,
	GALGUI_NAV_MOVE_FLAGS_ALSO_SCORE_VISIBLE_SET = 1 << 5,
	GALGUI_NAV_MOVE_FLAGS_SCROLL_TO_EDGE = 1 << 6
};
// operator
ENUM_FLAG_OPERATORS(EnumGalGuiNavMoveFlags, GalGuiNavMoveFlags)

using GalGuiNextItemDataFlags = int;
enum class EnumGalGuiNextItemDataFlags : GalGuiNextItemDataFlags
{
	GALGUI_NEXT_ITEM_DATA_FLAGS_NONE = 0,
	GALGUI_NEXT_ITEM_DATA_FLAGS_HAS_WIDTH = 1 << 0,
	GALGUI_NEXT_ITEM_DATA_FLAGS_HAS_OPEN = 1 << 1
};
// operator
ENUM_FLAG_OPERATORS(EnumGalGuiNextItemDataFlags, GalGuiNextItemDataFlags)

using GalGuiNextWindowDataFlags = int;
enum class EnumGalGuiNextWindowDataFlags : GalGuiNextWindowDataFlags
{
	GALGUI_NEXT_WINDOW_DATA_FLAGS_NONE = 0,
	GALGUI_NEXT_WINDOW_DATA_FLAGS_HAS_POS = 1 << 0,
	GALGUI_NEXT_WINDOW_DATA_FLAGS_HAS_SIZE = 1 << 1,
	GALGUI_NEXT_WINDOW_DATA_FLAGS_HAS_CONTENT_SIZE = 1 << 2,
	GALGUI_NEXT_WINDOW_DATA_FLAGS_HAS_COLLAPSED = 1 << 3,
	GALGUI_NEXT_WINDOW_DATA_FLAGS_HAS_SIZE_CONSTRAINT = 1 << 4,
	GALGUI_NEXT_WINDOW_DATA_FLAGS_HAS_FOCUS = 1 << 5,
	GALGUI_NEXT_WINDOW_DATA_FLAGS_HAS_BACKGROUND_ALPHA = 1 << 6,
	GALGUI_NEXT_WINDOW_DATA_FLAGS_HAS_SCROLL = 1 << 7
};
// operator
ENUM_FLAG_OPERATORS(EnumGalGuiNextWindowDataFlags, GalGuiNextWindowDataFlags)

using GalGuiSeparatorFlags = int;
enum class EnumGalGuiSeparatorFlags : GalGuiSeparatorFlags
{
	GALGUI_SEPARATOR_FLAGS_NONE = 0,
	GALGUI_SEPARATOR_FLAGS_HORIZONTAL = 1 << 0,
	GALGUI_SEPARATOR_FLAGS_VERTICAL = 1 << 1,
	GALGUI_SEPARATOR_FLAGS_SPAN_ALL_COLUMNS = 1 << 2
};
// operator
ENUM_FLAG_OPERATORS(EnumGalGuiSeparatorFlags, GalGuiSeparatorFlags)

using GalGuiSliderFlags = int;
enum class EnumGalGuiSliderFlags : GalGuiSliderFlags
{
	GALGUI_SLIDER_FLAGS_NONE = 0,
	GALGUI_SLIDER_FLAGS_VERTICAL = 1 << 0
};
// operator
ENUM_FLAG_OPERATORS(EnumGalGuiSliderFlags, GalGuiSliderFlags)

using GalGuiTextFlags = int;
enum class EnumGalGuiTextFlags : GalGuiTextFlags
{
	GALGUI_TEXT_FLAGS_NONE = 0,
	GALGUI_TEXT_FLAGS_NO_WIDTH_FOR_LARGE_CLIPPED_TEXT = 1 << 0
};
// operator
ENUM_FLAG_OPERATORS(EnumGalGuiTextFlags, GalGuiTextFlags)

using GalGuiTooltipFlags = int;
enum class EnumGalGuiTooltipFlags : GalGuiTooltipFlags
{
	GALGUI_TOOLTIP_FLAGS_NONE = 0,
	GALGUI_TOOLTIP_FLAGS_OVERRIDE_PREVIOUS_TOOLTIP = 1 << 0
};
// operator
ENUM_FLAG_OPERATORS(EnumGalGuiTooltipFlags, GalGuiTooltipFlags)

enum class EnumGalGuiLogType
{
	GALGUI_LOG_TYPE_NONE = 0,
	GALGUI_LOG_TYPE_TTY,
	GALGUI_LOG_TYPE_FILE,
	GALGUI_LOG_TYPE_BUFFER,
	GALGUI_LOG_TYPE_CLIPBOARD
};

// X/Y enums are fixed to 0/1 so they may be used to index ImVec2
enum class EnumGalGuiAxis
{
	GALGUI_AXIS_NONE = -1,
	GALGUI_AXIS_X = 0,
	GALGUI_AXIS_Y = 1
};

enum class EnumGalGuiPlotType
{
	GALGUI_PLOT_TYPE_LINE,
	GALGUI_PLOT_TYPE_HISTOGRAM
};

enum class EnumGalGuiInputSource
{
	GALGUI_INPUT_SOURCE_NONE = 0,
	GALGUI_INPUT_SOURCE_MOUSE,
	GALGUI_INPUT_SOURCE_NAV,
	GALGUI_INPUT_SOURCE_NAV_KEYBOARD,
	GALGUI_INPUT_SOURCE_NV_GAMEPAD,
	GALGUI_INPUT_SOURCE_COUNT
};

enum class EnumGalGuiInputReadMode
{
	GALGUI_INPUT_READ_MODE_DOWN,
	GALGUI_INPUT_READ_MODE_PRESSED,
	GALGUI_INPUT_READ_MODE_RELEASED,
	GALGUI_INPUT_READ_MODE_REPEAT,
	GALGUI_INPUT_READ_MODE_REPEAT_SLOW,
	GALGUI_INPUT_READ_MODE_REPEAT_FAST
};

enum class EnumGalGuiNavForward
{
	GALGUI_NAV_FORWARD_NONE,
	GALGUI_NAV_FORWARD_FORWARD_QUEUED,
	GALGUI_NAV_FORWARD_FORWARD_ACTIVE
};

enum class EnumGalGuiNavLayer
{
	GALGUI_NAV_LAYER_MAIN = 0,
	GALGUI_NAV_LAYER_MENU = 1,
	GALGUI_NAV_LAYER_COUNT
};

enum class EnumGalGuiPopupPositionPolicy
{
	GALGUI_POPUP_POSITION_POLICY_DEFAULT,
	GALGUI_POPUP_POSITION_POLICY_COMBOBOX
};

enum class EnumGalFontAtlas : int
{
	GALGUI_FONT_ATLAS_NONE = 0,
	GALGUI_FONT_ATLAS_NO_POWER_OF_TWO_HEIGHT = 1 << 0,
	GALGUI_FONT_ATLAS_NO_MOUSE_CURSORS = 1 << 1,
	GALGUI_FONT_ATLAS_NO_BACKED_LINES = 1 << 2
};
// operator
ENUM_FLAG_OPERATORS(EnumGalFontAtlas, int)

// 字符解码
using GalWChar16 = unsigned short;
using GalWChar32 = unsigned int;
using GalWChar = GalWChar16;

using GalTextureID = void*;
using GalGuiID = unsigned int;// 一个独有的 ID ,通常来自字符串的哈希值


// 函数指针 GalGuiInputTextCallback
//typedef int (*GalGuiInputTextCallback)(GalGuiInputTextCallbackData* data);
// 函数指针 GalGuiSizeCallback
//typedef void (*GalGuiSizeCallback)(GalGuiSizeCallbackData* data);

// TODO 想办法取代这个macro
#define GAL_ASSERT assert  // NOLINT(cppcoreguidelines-macro-usage)

inline auto GalMemCpy(void* dest, const size_t destSize, void* source, const size_t sourceSize)
{
	return memcpy_s(dest, destSize, source, sourceSize);
}

inline auto GalMemMove(void* dest, const size_t destSize, void* source, const size_t sourceSize)
{
	return memmove_s(dest, destSize, source, sourceSize);
}

inline auto GalMemSet(void* dest, const int val, const size_t size)
{
	return memset(dest, val, size);
}

extern GalGuiContext* g_galGuiContext;

struct GalNewDummy {};
inline void* operator new(size_t, GalNewDummy, void* ptr)
{
	return ptr;
}

inline void operator delete(void*, GalNewDummy, void*) {}

constexpr void* GalMemAlloc(const size_t size)
{
	if(GalGuiContext* context = g_galGuiContext)
	{
		context->++IO.MetricsActiveAllocations;
	}
	return malloc(size);
}

constexpr void GalMemFree(void* ptr)
{
	if(ptr)
	{
		if (GalGuiContext* context = g_galGuiContext)
		{
			context->--IO.MetricsActiveAllocations;
		}
	}
	return free(ptr);
}

template <typename Ptr, typename T>
constexpr decltype(auto) GalPlacementNew(Ptr ptr)
{
	return new(GalNewDummy(), ptr) T();
}

template <typename T>
constexpr decltype(auto) GalNew()
{
	return new(GalNewDummy(), GalMemAlloc(sizeof(T))) T;
}

template <typename T>
constexpr void GalDelete(T* ptr)
{
	if(ptr)
	{
		ptr->~T();
		GalMemFree(ptr);
	}
}

// 只支持例如 int[] char[] 等,不支持 int* char*等
template <typename T, size_t Length>
constexpr size_t GalArraySize(const T(&)[Length])
{
	return Length;
}


using GalFileHandle = FILE*;
inline GalFileHandle GalFileOpen(const char* filename, const char* mode)
{
	// TODO We need a fopen() wrapper because MSVC/Windows fopen doesn't handle UTF-8 filenames.

	GalFileHandle file;
	// fopen is unsafe, although the errno is useless for us
	fopen_s(&file, filename, mode);
	return file;
}

inline bool GalFileClose(const GalFileHandle file)
{
	return fclose(file) == 0;
}

inline GalU64 GalFileGetSize(const GalFileHandle file)
{
	long off;
	long size;
	return
	(off = ftell(file)) != -1 && 
	!fseek(file, 0, SEEK_END) && 
	(size = ftell(file)) != -1 && 
	!fseek(file, off, SEEK_SET)
	? static_cast<GalU64>(size) : static_cast<GalU64>(-1);
}

inline GalU64 GalFileRead(void* data, const GalU64 size, const GalU64 count, const GalFileHandle file)
{
	return fread(data, size, count, file);
}

inline GalU64 GalFileWrite(const void* data, const GalU64 size, const GalU64 count, const GalFileHandle file)
{
	return fwrite(data, size, count, file);
}

inline void* GalFileLoadToMemory(const char* filename, const char* mode, size_t* outSize, int paddingBytes)
{
	GAL_ASSERT(filename && mode);
	if (outSize)
	{
		*outSize = 0;
	}

	GalFileHandle file;
	if((file = GalFileOpen(filename, mode)) == nullptr)
	{
		return nullptr;
	}

	const auto size = GalFileGetSize(file);
	if(size == static_cast<GalU64>(-1))
	{
		GalFileClose(file);
		return nullptr;
	}
	
	const auto data = GalMemAlloc(size + paddingBytes);
	if(data == nullptr)
	{
		GalFileClose(file);
		return nullptr;
	}
	if(GalFileRead(data, 1, size, file) != size)
	{
		GalFileClose(file);
		GalMemFree(data);
		return nullptr;
	}

	if(paddingBytes > 0)
	{
		GalMemSet((static_cast<char*>(data)+size), 0, paddingBytes);
	}

	GalFileClose(file);
	if(outSize)
	{
		*outSize = size;
	}

	return data;
}


constexpr auto GalUnicodeCodePointInvalid = 0xFFFD;
constexpr auto GalUnicodeCodePointMax = 0xFFFF;
constexpr auto GalDrawListTexLinesWidthMax = 63;




constexpr auto GalColRShift = 0;
constexpr auto GalColGShift = 8;
constexpr auto GalColBShift = 16;
constexpr auto GalColAShift = 24;
constexpr auto GalColAMask = 0xFF000000;

constexpr GalU32 GalCol32(const unsigned short r, const unsigned short g, const unsigned short b, const unsigned short a)
{
	return (a << GalColAShift) | (b << GalColBShift) | (g << GalColGShift) | (r << GalColGShift);
}

constexpr auto GalCol32White = GalCol32(255, 255, 255, 255);
constexpr auto GalCol32Black = GalCol32(0, 0, 0, 255);
constexpr auto GalCol32BlackTrans = GalCol32(0, 0, 0, 0);

template <typename T>
static constexpr T GalClamp(const T& v, const T& mn, const T& mx)
{
	return (v < mn) ? mn : (v > mx) ? mx : v;
}

template <typename T>
static constexpr T GalLerp(const T& a, const T& b, const float t)
{
	return static_cast<T>(a + (b - 1) * t);
}

template <typename T>
static constexpr void GalSwap(T& a, T& b)
{
	using std::swap;
	swap(a, b);
}

template <typename T>
static constexpr T GalAddClampOverflow(const T& a, const T& b, const T& mn, const T& mx)
{
	if(b < 0 && (a < mn - b))
	{
		return mn;
	}
	if(b > 0 && (a > mx - b))
	{
		return mx;
	}
	return a + b;
}

template <typename T>
static constexpr T GalSubClampOverflow(const T& a, const T& b, const T& mn, const T& mx)
{
	if (b > 0 && (a < mn + b))
	{
		return mn;
	}
	if (b < 0 && (a > mx + b))
	{
		return mx;
	}
	return a - b;
}














static constexpr GalU32 GCrc32LookupTable[256] =
{
    0x00000000,0x77073096,0xEE0E612C,0x990951BA,0x076DC419,0x706AF48F,0xE963A535,0x9E6495A3,0x0EDB8832,0x79DCB8A4,0xE0D5E91E,0x97D2D988,0x09B64C2B,0x7EB17CBD,0xE7B82D07,0x90BF1D91,
    0x1DB71064,0x6AB020F2,0xF3B97148,0x84BE41DE,0x1ADAD47D,0x6DDDE4EB,0xF4D4B551,0x83D385C7,0x136C9856,0x646BA8C0,0xFD62F97A,0x8A65C9EC,0x14015C4F,0x63066CD9,0xFA0F3D63,0x8D080DF5,
    0x3B6E20C8,0x4C69105E,0xD56041E4,0xA2677172,0x3C03E4D1,0x4B04D447,0xD20D85FD,0xA50AB56B,0x35B5A8FA,0x42B2986C,0xDBBBC9D6,0xACBCF940,0x32D86CE3,0x45DF5C75,0xDCD60DCF,0xABD13D59,
    0x26D930AC,0x51DE003A,0xC8D75180,0xBFD06116,0x21B4F4B5,0x56B3C423,0xCFBA9599,0xB8BDA50F,0x2802B89E,0x5F058808,0xC60CD9B2,0xB10BE924,0x2F6F7C87,0x58684C11,0xC1611DAB,0xB6662D3D,
    0x76DC4190,0x01DB7106,0x98D220BC,0xEFD5102A,0x71B18589,0x06B6B51F,0x9FBFE4A5,0xE8B8D433,0x7807C9A2,0x0F00F934,0x9609A88E,0xE10E9818,0x7F6A0DBB,0x086D3D2D,0x91646C97,0xE6635C01,
    0x6B6B51F4,0x1C6C6162,0x856530D8,0xF262004E,0x6C0695ED,0x1B01A57B,0x8208F4C1,0xF50FC457,0x65B0D9C6,0x12B7E950,0x8BBEB8EA,0xFCB9887C,0x62DD1DDF,0x15DA2D49,0x8CD37CF3,0xFBD44C65,
    0x4DB26158,0x3AB551CE,0xA3BC0074,0xD4BB30E2,0x4ADFA541,0x3DD895D7,0xA4D1C46D,0xD3D6F4FB,0x4369E96A,0x346ED9FC,0xAD678846,0xDA60B8D0,0x44042D73,0x33031DE5,0xAA0A4C5F,0xDD0D7CC9,
    0x5005713C,0x270241AA,0xBE0B1010,0xC90C2086,0x5768B525,0x206F85B3,0xB966D409,0xCE61E49F,0x5EDEF90E,0x29D9C998,0xB0D09822,0xC7D7A8B4,0x59B33D17,0x2EB40D81,0xB7BD5C3B,0xC0BA6CAD,
    0xEDB88320,0x9ABFB3B6,0x03B6E20C,0x74B1D29A,0xEAD54739,0x9DD277AF,0x04DB2615,0x73DC1683,0xE3630B12,0x94643B84,0x0D6D6A3E,0x7A6A5AA8,0xE40ECF0B,0x9309FF9D,0x0A00AE27,0x7D079EB1,
    0xF00F9344,0x8708A3D2,0x1E01F268,0x6906C2FE,0xF762575D,0x806567CB,0x196C3671,0x6E6B06E7,0xFED41B76,0x89D32BE0,0x10DA7A5A,0x67DD4ACC,0xF9B9DF6F,0x8EBEEFF9,0x17B7BE43,0x60B08ED5,
    0xD6D6A3E8,0xA1D1937E,0x38D8C2C4,0x4FDFF252,0xD1BB67F1,0xA6BC5767,0x3FB506DD,0x48B2364B,0xD80D2BDA,0xAF0A1B4C,0x36034AF6,0x41047A60,0xDF60EFC3,0xA867DF55,0x316E8EEF,0x4669BE79,
    0xCB61B38C,0xBC66831A,0x256FD2A0,0x5268E236,0xCC0C7795,0xBB0B4703,0x220216B9,0x5505262F,0xC5BA3BBE,0xB2BD0B28,0x2BB45A92,0x5CB36A04,0xC2D7FFA7,0xB5D0CF31,0x2CD99E8B,0x5BDEAE1D,
    0x9B64C2B0,0xEC63F226,0x756AA39C,0x026D930A,0x9C0906A9,0xEB0E363F,0x72076785,0x05005713,0x95BF4A82,0xE2B87A14,0x7BB12BAE,0x0CB61B38,0x92D28E9B,0xE5D5BE0D,0x7CDCEFB7,0x0BDBDF21,
    0x86D3D2D4,0xF1D4E242,0x68DDB3F8,0x1FDA836E,0x81BE16CD,0xF6B9265B,0x6FB077E1,0x18B74777,0x88085AE6,0xFF0F6A70,0x66063BCA,0x11010B5C,0x8F659EFF,0xF862AE69,0x616BFFD3,0x166CCF45,
    0xA00AE278,0xD70DD2EE,0x4E048354,0x3903B3C2,0xA7672661,0xD06016F7,0x4969474D,0x3E6E77DB,0xAED16A4A,0xD9D65ADC,0x40DF0B66,0x37D83BF0,0xA9BCAE53,0xDEBB9EC5,0x47B2CF7F,0x30B5FFE9,
    0xBDBDF21C,0xCABAC28A,0x53B39330,0x24B4A3A6,0xBAD03605,0xCDD70693,0x54DE5729,0x23D967BF,0xB3667A2E,0xC4614AB8,0x5D681B02,0x2A6F2B94,0xB40BBE37,0xC30C8EA1,0x5A05DF1B,0x2D02EF8D,
};

// Known size hash
// It is ok to call ImHashData on a string with known length but the ### operator won't be supported.
// FIXME-OPT: Replace with e.g. FNV1a hash? CRC32 pretty much randomly access 1KB. Need to do proper measurements.
constexpr GalU32 GalHashData(const unsigned char* data, size_t size, const GalU32 seed)
{
	auto crc = ~seed;
	const auto crc32Lut = GCrc32LookupTable;
	while(size-- != 0)
	{
        crc = (crc >> 8) ^ crc32Lut[(crc & 0xFF) ^ *data++];
	}

    return ~crc;
}

// Zero-terminated string hash, with support for ### to reset back to seed value
// We support a syntax of "label###id" where only "###id" is included in the hash, and only "label" gets displayed.
// Because this syntax is rarely used we are optimizing for the common case.
// - If we reach ### in the string we discard the hash so far and reset to the seed.
// - We don't do 'current += 2; continue;' after handling ### to keep the code smaller/faster (measured ~10% diff in Debug build)
// FIXME-OPT: Replace with e.g. FNV1a hash? CRC32 pretty much randomly access 1KB. Need to do proper measurements.
constexpr GalU32 GalHashStr(const unsigned char* data, size_t size, GalU32 seed)
{
    seed = ~seed;
    auto crc = seed;
    const auto crc32Lut = GCrc32LookupTable;
	if(size != 0)
	{
		while(size-- != 0)
		{
            const auto c = *data++;
			if(c == '#' && size >= 2 && data[0] == '#' && data[1] == '#')
			{
				crc = seed;
			}
            crc = (crc >> 8) ^ crc32Lut[(crc & 0xFF) ^ c];
		}
	}
	else
	{
		// @TODO 人间迷惑行为
		while(const auto c = *data++)
		{
			if(c == '#' && data[0] == '#' && data[1] == '#')
			{
				crc = seed;
			}
			crc = (crc >> 8) ^ crc32Lut[(crc & 0xFF) ^ c];
		}
	}

    return ~crc;
}

static constexpr GalU32 GalHash(const unsigned char* data, size_t size, GalU32 seed = 0)
{
	return size == 0 ? GalHashStr(data, 0, seed) : GalHashData(data, size, seed);
}

constexpr GalU32 GalAlphaBlendColors(GalU32 a, GalU32 b)
{
	const auto t = static_cast<float>(b >> GalColAShift & 0xff) / 255.0f;
	return
		GalCol32(
			GalLerp(a >> GalColRShift & 0xFF, b >> GalColRShift & 0xFF, t),
			GalLerp(a >> GalColGShift & 0xFF, b >> GalColGShift & 0xFF, t),
			GalLerp(a >> GalColBShift & 0xFF, b >> GalColBShift & 0xFF, t),
			0xFF
		);
}

static constexpr bool GalIsPowerOfTwo(const int v)
{
	return v != 0 && (v & (v - 1)) == 0;
}

static constexpr int GalUpperPowerOfTwo(int v)
{
	--v;
	v |= v >> 1;
	v |= v >> 2;
	v |= v >> 4;
	v |= v >> 8;
	v |= v >> 16;
	++v;
	return v;
}

constexpr void GalStrTrimBlank(char* str)
{
	auto p = str;
	while(p[0] == ' ' || p[0] == '\t')
	{
		++p;
	}
	const auto pStart = p;
	
	while (*p)
	{
		++p;
	}

	while(pStart < p && p[-1] == ' ' || p[-1] == '\t')
	{
		--p;
	}

	if(pStart != str)
	{
		memmove_s(str, p - pStart, pStart, p - pStart);
	}
	str[p - pStart] = '\0';
}

constexpr const char* GalStrSkipBlank(const char* str)
{
	while(str[0] == ' ' || str[0] == '\t')
	{
		++str;
	}
	return str;
}

inline int GalFormatString(char* buffer, const size_t length, const char* fmt, ...)
{
	va_list args = nullptr;
	va_start(args, fmt);
	auto w = vsnprintf(buffer, length, fmt, args);
	va_end(args);

	if(buffer == nullptr)
	{
		return w;
	}
	if(w == -1 || static_cast<size_t>(w) >= length)
	{
		w = length - 1;
	}
	buffer[w] = '\0';
	return w;
}

constexpr const char* GalParseFormatFindStart(const char* str)
{
	while(const auto c = str[0])
	{
		if(c == '%')
		{
			if(str[1] != '%')
			{
				return str;
			}
			++str;
		}
		++str;
	}
	return str;
}

constexpr const char* GalParseFormatFindEnd(const char* str)
{
	if(str[0] != '%')
	{
		return str;
	}

	const auto ignoreUppercaseMask = (1 << ('I' - 'A')) | (1 << ('L' - 'A'));
	const auto ignoreLowercaseMask =
		(1 << ('h' - 'a')) |
		(1 << ('j' - 'a')) |
		(1 << ('l' - 'a')) |
		(1 << ('t' - 'a')) |
		(1 << ('w' - 'a')) |
		(1 << ('z' - 'a'));
	
	for( ; ; )
	{
		const char c = str[0];
		if(c == '\0')
		{
			return str;
		}
		if(c >= 'A' && c <= 'Z' && ((1 << (c - 'A')) & ignoreUppercaseMask) == 0)
		{
			return ++str;
		}
		if(c >= 'a' && c <= 'z' && ((1 << (c - 'a')) & ignoreLowercaseMask) == 0)
		{
			return ++str;
		}
		++str;
	}
}

// Extract the format out of a format string with leading or trailing decorations
//  fmt = "blah blah"  -> return fmt
//  fmt = "%.3f"       -> return fmt
//  fmt = "hello %.3f" -> return fmt + 6
//  fmt = "%.3f hello" -> return buf written with "%.3f"
constexpr const char* GalParseFormatTrimDecoration(const char* str, char* dest, const size_t length)
{
	const auto start = GalParseFormatFindStart(str);
	if(start[0] != '%')
	{
		return str;
	}

	const auto end = GalParseFormatFindEnd(start);
	if(end[0] == '\0')
	{
		return start;
	}

	strcpy_s(dest, std::min(static_cast<size_t>(end - start + 1), length), str);

	return dest;
}

constexpr int GalParseFormatPrecision(const char* str, int defaultPrecision)
{
	str = GalParseFormatFindStart(str);
	if(str[0] != '%')
	{
		return defaultPrecision;
	}

	++str;
	while(str[0] >= '0' && str[0] <= '9')
	{
		++str;
	}
	auto precision = INT_MAX;
	if(str[0] == '.')
	{
		str = 
		[](const char* src, int& output) -> const char*
		{
			bool neg = false;
			if(src[0] == '-')
			{
				neg = true;
				++src;
			}
			if(src[0] == '+')
			{
				++src;
			}
			int v = 0;
			while(src[0] >= '0' && src[0] <= '9')
			{
				(v *= 10) += (src[0] - '0');
				++src;
			}
			
			output = neg ? -v : v;
			return src;
		}(++str, precision);

		if(precision < 0 || precision > 99)
		{
			precision = defaultPrecision;
		}
	}

	if(str[0] == 'e' || str[0] == 'E')
	{
		precision = -1;
	}
	if((str[0] == 'g' || str[0] == 'G') && precision == INT_MAX)
	{
		precision = -1;
	}

	return (precision == INT_MAX) ? defaultPrecision : precision;
}


constexpr bool GalCharIsBlank(const char c)
{
	return c == ' ' || c == '\t';
}

constexpr bool GalCharIsBlankW(const unsigned int c)
{
	return c == ' ' || c == '\t' || c == 0x3000;
}

constexpr int GalTextStrToUtf8(char* dest, const size_t length, const GalWChar* text, const GalWChar* textEnd)
{
	auto it = dest;
	const auto end = dest + length;

	while(it < end - 1 && (!textEnd || text < textEnd) && *text)
	{
		const auto c = static_cast<unsigned int>(*text++);
		if(c < 0x80)
		{
			*it++ = static_cast<char>(c);
		}
		else
		{
			it +=
			// convert char from unicode to utf8
			[](char* src, const size_t srcLength, const unsigned int c) -> int
			{
				if(c < 0x80)
				{
					src[0] = static_cast<char>(c);
					return 1;
				}
				if(c < 0x800)
				{
					if(srcLength < 2)
					{
						return 0;
					}
					src[0] = static_cast<char>(0xc0 + (c >> 6));
					src[1] = static_cast<char>(0x80 + (c & 0x3f));
					return 2;
				}
				if(c < 0x10000)
				{
					if(srcLength < 3)
					{
						return 0;
					}
					src[0] = static_cast<char>(0xe0 + (c >> 12));
					src[1] = static_cast<char>(0x80 + ((c >> 6) & 0x3f));
					src[2] = static_cast<char>(0x80 + (c & 0x3f));
					return 3;
				}
				if(c <= 0x10ffff)
				{
					if(srcLength < 4)
					{
						return 0;
					}
					src[0] = static_cast<char>(0xf0 + (c >> 18));
					src[1] = static_cast<char>(0x80 + ((c >> 12) & 0x3f));
					src[2] = static_cast<char>(0x80 + ((c >> 6) & 0x3f));
					src[3] = static_cast<char>(0x80 + (c & 0x3f));
					return 4;
				}
				return 0;
			}(it, end - it - 1, c);
		}
	}
	
	*it = '\0';
	return it - dest;
}

// Convert UTF-8 to 32-bit character, process single character input.
inline int GalTextCharFromUtf8(unsigned int* out, const char* text, const char* textEnd)
{
	auto str = reinterpret_cast<const unsigned char*>(text);
	
	if(!(str[0] & 0x80))
	{
		*out = static_cast<unsigned int>(*str);
		return 1;
	}
	if((str[0] & 0xe0) == 0xc0)
	{
		*out = GalUnicodeCodePointInvalid;
		if(textEnd && textEnd - reinterpret_cast<const char*>(str) < 2)
		{
			return 1;
		}
		if(str[0] < 0xc2)
		{
			return 2;
		}
		unsigned int c = (*str++ & 0x1f) << 6;
		if((str[0] & 0xc0) != 0x80)
		{
			return 2;
		}
		c += (*str & 0x3f);
		*out = c;
		return 2;
	}
	if((str[0] & 0xf0) == 0xe0)
	{
		*out = GalUnicodeCodePointInvalid;
		if(textEnd && textEnd - reinterpret_cast<const char*>(str) < 3)
		{
			return 1;
		}
		if(str[0] == 0xe0 && (str[1] < 0xa0 || str[1] > 0xbf))
		{
			return 3;
		}
		if(str[0] == 0xed && str[1] > 0x9f)
		{
			return 3;
		}
		unsigned int c = (*str++ & 0x0f) << 12;
		if((str[0] & 0xc0) != 0x80)
		{
			return 3;
		}
		c += (*str++ & 0x3f) << 6;
		if((str[0] & 0xc0) != 0x80)
		{
			return 3;
		}
		c += (*str & 0x3f);
		*out = c;
		return 3;
	}
	if((str[0] & 0xf8) == 0xf0)
	{
		*out = GalUnicodeCodePointInvalid;
		if (textEnd && textEnd - reinterpret_cast<const char*>(str) < 4)
		{
			return 1;
		}
		if(str[0] > 0xf4)
		{
			return 4;
		}
		if(str[0] == 0xf0 && (str[1] < 0x90 || str[1] > 0xbf))
		{
			return 4;
		}
		if(str[0] == 0xf4 && str[1] > 0x8f)
		{
			return 4;
		}
		unsigned int c = (*str++ & 0x07) << 18;
		if((str[0] & 0xc0) != 0x80)
		{
			return 4;
		}
		c += (*str++ & 0x3f) << 12;
		if((str[0] & 0xc0) != 0x80)
		{
			return 4;
		}
		c += (*str++ & 0x3f) << 6;
		if((str[0] & 0xc0) != 0x80)
		{
			return 4;
		}
		c += (*str & 0x3f);
		// utf-8 encodings of values used in surrogate pairs are invalid
		if((c & 0xfffff800) == 0xd800)
		{
			return 4;
		}
		// If codepoint does not fit in GalWChar, use replacement character U+FFFD instead
		if(c > GalUnicodeCodePointMax)
		{
			c = GalUnicodeCodePointInvalid;
		}
		*out = c;
		return 4;
	}
	*out = 0;
	return 0;
}

constexpr int GalTextStrFromUtf8(
	GalWChar* dest, const size_t length, 
	const char* text, const char* textEnd, 
	const char** textRemaining)
{
	auto it = dest;
	const auto end = dest + length;
	while(it < end - 1 && (!textEnd || text < textEnd) && *text)
	{
		unsigned int c = -1;
		text += GalTextCharFromUtf8(&c, text, textEnd);
		if(c == 0)
		{
			break;
		}
		*it++ = static_cast<GalWChar>(c);
	}
	*it = 0;
	if(textRemaining)
	{
		*textRemaining = text;
	}
	
	return it - dest;
}

constexpr int GalTextCountCharsFromUtf8(const char* text, const char* textEnd)
{
	int count = 0;
	while((!textEnd || text < textEnd) && *text)
	{
		unsigned int c = -1;
		text += GalTextCharFromUtf8(&c, text, textEnd);
		if(c == 0)
		{
			break;
		}
		++count;
	}

	return count;
}

inline int GalTextCountUtf8BytesFromChar(const char* text, const char* textEnd)
{
	unsigned int dummy = 0;
	return GalTextCharFromUtf8(&dummy, text, textEnd);
}

static constexpr int GalTextCountUtf8BytesFromChar(const unsigned int c)
{
	if (c < 0x80) return 1;
	if (c < 0x800) return 2;
	if (c < 0x10000) return 3;
	if (c <= 0x10ffff) return 4;
	return 3;
}

constexpr int GalTextCountUtf88BytesFromStr(const GalWChar* text, const GalWChar* textEnd)
{
	int count = 0;
	while((!textEnd || text < textEnd) && *text)
	{
		const auto c = static_cast<unsigned int>(*text++);
		if(c < 0x80)
		{
			++count;
		}
		else
		{
			count += GalTextCountUtf8BytesFromChar(c);
		}
	}
	
	return count;
}

struct GalVec2;
struct GalVec2Half;
struct GalVec4;
struct GalRect;

// template <typename PtrType>
struct GalGuiStorage;


struct GalBitVec;                 // Store 1-bit per value
struct GalRect;                      // An axis-aligned rectangle (2 points)
struct GalDrawDataBuilder;           // Helper to build a ImDrawData instance
struct GalDrawListSharedData;        // Data shared between all ImDrawList instances
struct GalGuiColorMod;               // Stacked color modifier, backup of modified data so we can restore it
struct GalGuiColumnData;             // Storage data for a single column
struct GalGuiColumns;                // Storage data for a columns set
struct GalGuiContext;                // Main Dear ImGui context
struct GalGuiDataTypeInfo;           // Type information associated to a ImGuiDataType enum
struct GalGuiGroupData;              // Stacked storage data for BeginGroup()/EndGroup()
struct GalGuiInputTextState;         // Internal state of the currently focused/edited text input box
struct GalGuiItemHoveredDataBackup;  // Backup and restore IsItemHovered() internal data
struct GalGuiMenuColumns;            // Simple column measurement, currently used for MenuItem() only
struct GalGuiNavMoveResult;          // Result of a gamepad/keyboard directional navigation move query result
struct GalGuiNextWindowData;         // Storage for SetNextWindow** functions
struct GalGuiNextItemData;           // Storage for SetNextItem** functions
struct GalGuiPopupData;              // Storage for current popup stack
struct GalGuiSettingsHandler;        // Storage for one type registered in the .ini file
struct GalGuiStyleMod;               // Stacked style modifier, backup of modified data so we can restore it
struct GalGuiTabBar;                 // Storage for a tab bar
struct GalGuiTabItem;                // Storage for a tab item (within a tab bar)
struct GalGuiWindow;                 // Storage for one window
struct GalGuiWindowTempData;         // Temporary storage for one window (that's the data which in theory we could ditch at the end of the frame)
struct GalGuiWindowSettings;         // Storage for a window .ini settings (we keep one of those even if the actual window wasn't instanced during this session)

/* =====================================================
 *		GalVec2 & GalVec2Half & GalVec4 & GalRect
 *		GalVec2 & GalVec2Half & GalVec4 & GalRect
 *		GalVec2 & GalVec2Half & GalVec4 & GalRect
 *		GalVec2 & GalVec2Half & GalVec4 & GalRect
 *		GalVec2 & GalVec2Half & GalVec4 & GalRect
 *		GalVec2 & GalVec2Half & GalVec4 & GalRect
 *		GalVec2 & GalVec2Half & GalVec4 & GalRect
 *		GalVec2 & GalVec2Half & GalVec4 & GalRect
 *		GalVec2 & GalVec2Half & GalVec4 & GalRect
 *		GalVec2 & GalVec2Half & GalVec4 & GalRect
 *		GalVec2 & GalVec2Half & GalVec4 & GalRect
 *		GalVec2 & GalVec2Half & GalVec4 & GalRect
 *		GalVec2 & GalVec2Half & GalVec4 & GalRect
 *		GalVec2 & GalVec2Half & GalVec4 & GalRect
 *		GalVec2 & GalVec2Half & GalVec4 & GalRect
 *		GalVec2 & GalVec2Half & GalVec4 & GalRect
 *		GalVec2 & GalVec2Half & GalVec4 & GalRect
 *		GalVec2 & GalVec2Half & GalVec4 & GalRect
 *		GalVec2 & GalVec2Half & GalVec4 & GalRect
 * ======================================================
 */

// 二维向量,通常用于储存位置或者大小
struct GalVec2
{
	float x;
	float y;

	GalVec2() = default;
	~GalVec2() = default;

	GalVec2(const GalVec2&) = default;
	GalVec2& operator=(const GalVec2&) = default;

	GalVec2(GalVec2&&) = default;
	GalVec2& operator=(GalVec2&&) = default;

	constexpr GalVec2(const float x, const float y) : x(x), y(y) {}
	explicit GalVec2(_In_reads_(2) const float* pArray) : x(pArray[0]), y(pArray[1]) {}

	explicit operator float* ()
	{
		return reinterpret_cast<float*>(this);
	}

	constexpr GalVec2 operator+(const float scale) const
	{
		return { x + scale, y + scale };
	}

	constexpr GalVec2 operator-(const float scale) const
	{
		return { x - scale, y - scale };
	}

	constexpr GalVec2 operator*(const float scale) const
	{
		return { x * scale, y * scale };
	}

	constexpr GalVec2 operator/(const float scale) const
	{
		return { x / scale, y / scale };
	}

	constexpr GalVec2 operator+(const GalVec2& rhs) const
	{
		return { x + rhs.x, y + rhs.y };
	}

	constexpr GalVec2 operator-(const GalVec2& rhs) const
	{
		return { x - rhs.x, y - rhs.y };
	}

	constexpr GalVec2 operator*(const GalVec2& rhs) const
	{
		return { x * rhs.x, y * rhs.y };
	}

	constexpr GalVec2 operator/(const GalVec2& rhs) const
	{
		return { x / rhs.x, y / rhs.y };
	}

	constexpr GalVec2& operator+=(const float scale)
	{
		x += scale;
		y += scale;
		return *this;
	}

	constexpr GalVec2& operator-=(const float scale)
	{
		x -= scale;
		y -= scale;
		return *this;
	}

	constexpr GalVec2& operator*=(const float scale)
	{
		x *= scale;
		y *= scale;
		return *this;
	}

	constexpr GalVec2& operator/=(const float scale)
	{
		x /= scale;
		y /= scale;
		return *this;
	}

	constexpr GalVec2& operator+=(const GalVec2& rhs)
	{
		x += rhs.x;
		y += rhs.x;
		return *this;
	}

	constexpr GalVec2& operator-=(const GalVec2& rhs)
	{
		x -= rhs.x;
		y -= rhs.x;
		return *this;
	}

	constexpr GalVec2& operator*=(const GalVec2& rhs)
	{
		x *= rhs.x;
		y *= rhs.x;
		return *this;
	}

	constexpr GalVec2& operator/=(const GalVec2& rhs)
	{
		x /= rhs.x;
		y /= rhs.x;
		return *this;
	}

	// 没有 > 和 < 比较,只有从两个 GalVec2 的组合情况中获得 最大/最小 的组合
	static constexpr GalVec2 GalVec2Min(const GalVec2& lhs, const GalVec2& rhs)
	{
		return { lhs.x < rhs.x ? lhs.x : rhs.x, lhs.y < rhs.y ? lhs.y : rhs.y };
	}

	static constexpr GalVec2 GalVec2Max(const GalVec2& lhs, const GalVec2& rhs)
	{
		return { lhs.x >= rhs.x ? lhs.x : rhs.x, lhs.y >= rhs.y ? lhs.y : rhs.y };
	}
};

struct GalVec2Half
{
	short x;
	short y;

	GalVec2Half() = default;
	~GalVec2Half() = default;

	GalVec2Half(const GalVec2Half&) = default;
	GalVec2Half& operator=(const GalVec2Half&) = default;

	GalVec2Half(GalVec2Half&&) = default;
	GalVec2Half& operator=(GalVec2Half&&) = default;

	constexpr GalVec2Half(const short x, const short y) : x(x), y(y) {}
	explicit GalVec2Half(_In_reads_(2) const short* pArray) : x(pArray[0]), y(pArray[1]) {}
	explicit GalVec2Half(const GalVec2& vec2) : x(static_cast<short>(vec2.x)), y(static_cast<short>(vec2.y)) {}
};

//四维向量,通常用于储存颜色
struct GalVec4
{
	float x;
	float y;
	float z;
	float w;

	GalVec4() = default;
	~GalVec4() = default;

	GalVec4(const GalVec4&) = default;
	GalVec4& operator=(const GalVec4&) = default;

	GalVec4(GalVec4&&) = default;
	GalVec4& operator=(GalVec4&&) = default;

	constexpr GalVec4(const float x, const float y, const float z, const float w) : x(x), y(y), z(z), w(w) {}
	constexpr explicit GalVec4(_In_reads_(4) const float* pArray) : x(pArray[0]), y(pArray[1]), z(pArray[2]), w(pArray[3]) {}

	explicit operator float* ()
	{
		return reinterpret_cast<float*>(this);
	}

	constexpr GalVec4 operator+(const GalVec4& rhs) const
	{
		return { x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w };
	}

	constexpr GalVec4 operator-(const GalVec4& rhs) const
	{
		return { x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w };
	}

	constexpr GalVec4 operator*(const GalVec4& rhs) const
	{
		return { x * rhs.x, y * rhs.y, z * rhs.z, w * rhs.w };
	}
};

template <>
constexpr GalVec2 GalClamp(const GalVec2& v, const GalVec2& mn, const GalVec2& mx)
{
	return { v.x < mn.x ? mn.x : v.x > mx.x ? mx.x : v.x, v.y < mn.y ? mn.y : v.y > mx.y ? mx.y : v.y };
}

template <>
constexpr GalVec2 GalLerp(const GalVec2& a, const GalVec2& b, const float t)
{
	return { a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t };
}

constexpr GalVec2 GalLerp(const GalVec2& a, const GalVec2& b, const GalVec2& t)
{
	return { a.x + (b.x - a.x) * t.x, a.y + (b.y - a.y) * t.y };
}

template <>
constexpr GalVec4 GalLerp(const GalVec4& a, const GalVec4& b, const float t)
{
	return { a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t, a.z + (b.z - a.z) * t, a.w + (b.w - a.w) * t };
}

constexpr float GalSaturate(const float f)
{
	return f < 0.0f ? 0.0f : f > 1.0f ? 1.0f : f;
}

constexpr float GalLengthSqr(const GalVec2& vec)
{
	return vec.x * vec.x + vec.y * vec.y;
}

constexpr float GalLengthSqr(const GalVec4& vec)
{
	return vec.x * vec.x + vec.y * vec.y + vec.z * vec.z + vec.w * vec.w;
}

constexpr float GalInvLength(const GalVec2& vec, const float failVal)
{
	const auto d = vec.x * vec.x + vec.y * vec.y;
	if(d > 0.0f)
	{
		return 1.0f / sqrtf(d);
	}
	return failVal;
}

constexpr float GalFloor(const float val)
{
	return static_cast<float>(static_cast<int>(val));
}

constexpr GalVec2 GalFloor(const GalVec2& vec)
{
	return { GalFloor(vec.x), GalFloor(vec.y) };
}

constexpr int GalModePositive(const int a, const int b)
{
	return (a + b) % b;
}

constexpr float GalDot(const GalVec2& a, const GalVec2& b)
{
	return a.x * b.x + a.y * b.y;
}

constexpr GalVec2 GalRotate(const GalVec2& vec, const float cosA, const float sinA)
{
	return { vec.x * cosA - vec.y * sinA, vec.x * sinA + vec.y * cosA };
}

constexpr float GalLinearSweep(const float current, const float target, const float speed)
{
	if(current < target)
	{
		return std::min(current + speed, target);
	}
	if(current > target)
	{
		return std::max(current - speed, target);
	}
	return current;
}

constexpr GalVec2 GalLineClosestPoint(const GalVec2& a, const GalVec2& b, const GalVec2& p)
{
	const auto ap = p - a;
	const auto ab = b - a;
	const auto dot = GalDot(ap, ab);
	if(dot < 0.0f)
	{
		return a;
	}
	const auto abLengthSqr = GalLengthSqr(ab);
	if(dot > abLengthSqr)
	{
		return b;
	}
	return a + ab * dot / abLengthSqr;
}

// Cubic Bezier
constexpr GalVec2 GalBezierCalc(
	const GalVec2& p1, const GalVec2& p2, 
	const GalVec2& p3, const GalVec2& p4, const float t
)
{
	const auto u = 1.0f - t;
	const auto w1 = u * u * u;
	const auto w2 = 3 * u * u * t;
	const auto w3 = 3 * u * t * t;
	const auto w4 = t * t * t;
	return {
		w1 * p1.x + w2 * p2.x + w3 * p3.x + w4 * p4.x,
		w1 * p1.y + w2 * p2.y + w3 * p3.y + w4 * p4.y,
	};
}

// For curves with explicit number of segments
constexpr GalVec2 GalBezierClosestPoint(
	const GalVec2& p1, const GalVec2& p2, 
	const GalVec2& p3, const GalVec2& p4, 
	const GalVec2& p, const int numSegments
)
{
	GAL_ASSERT(numSegments > 0);
	auto last = p1;
	GalVec2 closest{};
	auto dist = FLT_MAX;
	const auto step = 1.0f / static_cast<float>(numSegments);
	for(int i = 1; i <= numSegments; ++i)
	{
		auto curr = GalBezierCalc(p1, p2, p3, p4, step * static_cast<float>(i));
		auto line = GalLineClosestPoint(last, curr, p);
		const auto dist2 = GalLengthSqr(p - line);
		if(dist2 < dist)
		{
			closest = line;
			dist = dist2;
		}
		last = curr;
	}
	return closest;
}

constexpr void GalBezierClosestPointCasteljauStep(
	const GalVec2& p, GalVec2& closest, GalVec2& last, float& dist, 
	const float x1, const float y1, 
	const float x2, const float y2, 
	const float x3, const float y3, 
	const float x4, const float y4,
	const float tessTol, const int level
)
{
	const auto dx = x4 - x1;
	const auto dy = y4 - y1;
	auto d2 = (x2 - x4) * dy - (y2 - y4) * dx;
	auto d3 = (x3 - x4) * dy - (y3 - y4) * dx;
	d2 = (d2 >= 0) ? d2 : -d2;
	d3 = (d3 >= 0) ? d3 : -d3;

	if ((d2 + d3) * (d2 + d3) < tessTol * (dx * dx + dy * dy))
	{
		const GalVec2 curr{ x4,y4 };
		const auto line = GalLineClosestPoint(last, curr, p);
		const auto dist2 = GalLengthSqr(p - line);
		if(dist2 < dist)
		{
			closest = line;
			dist = dist2;
		}
		last = curr;
	}
	else if(level < 10)
	{
		const auto x12 = (x1 + x2) * 0.5f;
		const auto y12 = (y1 + y2) * 0.5f;
		
		const auto x23 = (x2 + x3) * 0.5f;
		const auto y23 = (y2 + y3) * 0.5f;
		
		const auto x34 = (x3 + x4) * 0.5f;
		const auto y34 = (y3 + y4) * 0.5f;

		const auto x1223 = (x12 + x23) * 0.5f;
		const auto y1223 = (y12 + y23) * 0.5f;

		const auto x2334 = (x23 + x34) * 0.5f;
		const auto y2334 = (y23 + y34) * 0.5f;

		const auto x123234 = (x1223 + x2334) * 0.5f;
		const auto y123234 = (y1223 + y2334) * 0.5f;

		GalBezierClosestPointCasteljauStep(p, closest, last, dist, x1, y1, x12, y12, x1223, y1223, x123234, y123234, tessTol, level + 1);
		GalBezierClosestPointCasteljauStep(p, closest, last, dist, x123234, y123234, x2334, y2334, x34, y34, x4, y4, tessTol, level + 1);
	}
}

// For auto-tessellated curves you can use tessTol = style.curveTessellationTol
constexpr GalVec2 GalBezierClosestPointCasteljau(
	const GalVec2& p1, const GalVec2& p2, 
	const GalVec2& p3, const GalVec2& p4, 
	const GalVec2& p, const float tessTol
)
{
	GAL_ASSERT(tessTol > 0.0f);
	auto last = p1;
	GalVec2 closest{};
	auto dist = FLT_MAX;
	GalBezierClosestPointCasteljauStep(p, closest, last, dist, p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, p4.x, p4.y, tessTol, 0);
	return closest;
}

constexpr bool GalTriangleContainsPoint(
	const GalVec2& a, const GalVec2& b, const GalVec2& c,
	const GalVec2& p
)
{
	const auto b1 = (p.x - b.x) * (a.y - b.y) - (p.y - b.y) * (a.x - b.x) < 0.0f;
	const auto b2 = (p.x - c.x) * (b.y - c.y) - (p.y - c.y) * (b.x - c.x) < 0.0f;
	const auto b3 = (p.x - a.x) * (c.y - a.y) - (p.y - a.y) * (c.x - a.x) < 0.0f;

	return b1 == b2 && b2 == b3;
}

constexpr GalVec2 GalTriangleClosestPoint(
	const GalVec2& a, const GalVec2& b, const GalVec2& c,
	const GalVec2& p
)
{
	const auto abProj = GalLineClosestPoint(a, b, p);
	const auto bcProj = GalLineClosestPoint(b, c, p);
	const auto caProj = GalLineClosestPoint(c, a, p);
	const auto abDist = GalLengthSqr(p - abProj);
	const auto bcDist = GalLengthSqr(p - bcProj);
	const auto caDist = GalLengthSqr(p - caProj);
	const auto min = std::min(abDist, std::min(bcDist, caDist));

	if(min == abDist)
	{
		return abProj;
	}
	if(min == bcDist)
	{
		return bcProj;
	}
	return caProj;
}

constexpr void GalTriangleBarycentricCoords(
	const GalVec2& a, const GalVec2& b, const GalVec2& c,
	const GalVec2& p, float& outU, float& outV, float& outW)
{
	const auto v0 = b - a;
	const auto v1 = c - a;
	const auto v2 = p - a;
	const auto d = v0.x * v1.y - v1.x * v0.y;
	outV = (v2.x * v1.y - v1.x * v2.y) / d;
	outW = (v0.x * v2.y - v2.x * v0.y) / d;
	outU = 1.0f - outV - outW;
}

constexpr float GalTriangleArea(const GalVec2& a, const GalVec2& b, const GalVec2& c)
{
	const auto v = a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y);
	return (v > 0.0f ? v : -v) * 0.5f;
}

constexpr GalGuiDir GalGetDirQuadrantFromDelta(float dx, float dy)
{
	auto x = true;
	auto y = true;
	dx = dx > 0.0f ? dx : (x = false, -dx);
	dy = dy > 0.0f ? dy : (y = false, -dy);
	if(dx > dy)
	{
		return x ?
			static_cast<GalGuiDir>(EnumGalGuiDir::GALGUI_DIR_RIGHT)
			:
			static_cast<GalGuiDir>(EnumGalGuiDir::GALGUI_DIR_LEFT);
	}
	return y ?
		static_cast<GalGuiDir>(EnumGalGuiDir::GALGUI_DIR_DOWN)
		:
		static_cast<GalGuiDir>(EnumGalGuiDir::GALGUI_DIR_UP);
}

struct GalRect
{
	GalVec2 min;		// upper-left
	GalVec2 max;		// lower-right;

	GalRect() = default;
	~GalRect() = default;

	GalRect(const GalRect&) = default;
	GalRect& operator=(const GalRect&) = default;

	GalRect(GalRect&&) = default;
	GalRect& operator=(GalRect&&) = default;
	
	constexpr explicit GalRect(const GalVec2& min, const GalVec2& max) : min(min), max(max) {}
	constexpr explicit GalRect(const GalVec4& v) : min(v.x, v.y), max(v.z, v.w) {}
	constexpr explicit GalRect(const float x1, const float y1, const float x2, const float y2) : min(x1, y1), max(x2, y2) {}
	constexpr explicit GalRect(_In_reads_(4) const float* pArray) : min(pArray[0], pArray[1]), max(pArray[2], pArray[3]) {}

	explicit operator GalVec4()
	{
		return static_cast<GalVec4>(*this);
	}

	[[nodiscard]] constexpr GalVec2 GetCenter() const
	{
		return { (min.x + max.x) * 0.5f, (min.y + max.y) * 0.5f };
	}

	[[nodiscard]] constexpr GalVec2 GetSize() const
	{
		return { max.x - min.x, max.y - min.y };
	}

	[[nodiscard]] constexpr float GetWidth() const
	{
		return max.x - min.x;
	}

	[[nodiscard]] constexpr float GetHeight() const
	{
		return max.y - min.y;
	}

	[[nodiscard]] constexpr GalVec2 GetTopLeft() const
	{
		return min;
	}

	[[nodiscard]] constexpr GalVec2 GetTopRight() const
	{
		return { max.x, min.y };
	}

	[[nodiscard]] constexpr GalVec2 GetBottomLeft() const
	{
		return { min.x, max.y };
	}

	[[nodiscard]] constexpr GalVec2 GetBottomRight() const
	{
		return max;
	}

	[[nodiscard]] constexpr bool Contains(const GalVec2& point) const
	{
		return point.x >= min.x && point.y >= min.y && point.x < max.x && point.y < max.y;
	}
	
	[[nodiscard]] constexpr bool Contains(const GalRect& rect) const
	{
		return rect.min.x >= min.x && rect.min.y >= min.y && rect.max.x <= max.x && rect.max.y <= max.y;
	}
	
	[[nodiscard]] constexpr bool Overlaps(const GalRect& rect) const
	{
		return rect.min.x < max.x && rect.max.x > min.x && rect.min.y < max.y && rect.max.y > min.y;
	}

	constexpr void Add(const GalVec2& point)
	{
		if(min.x > point.x)
		{
			min.x = point.x;
		}
		if(min.y > point.y)
		{
			min.y = point.y;
		}
		if(max.x < point.x)
		{
			max.x = point.x;
		}
		if(max.y < point.y)
		{
			max.y = point.y;
		}
	}
	
	constexpr void Add(const GalRect& rect)
	{
		if(min.x > rect.min.x)
		{
			min.x = rect.min.x;
		}
		if(min.y > rect.min.y)
		{
			min.y = rect.min.y;
		}
		if(max.x < rect.max.x)
		{
			max.x = rect.max.x;
		}
		if(max.y < rect.max.y)
		{
			max.y = rect.max.y;
		}
	}

	constexpr void Expand(const float amount)
	{
		min -= amount;
		max += amount;
	}

	constexpr void Expand(const GalVec2& amount)
	{
		min -= amount;
		max += amount;
	}

	constexpr void Translate(const GalVec2& distance)
	{
		min += distance;
		max += distance;
	}

	constexpr void TranslateX(const float distance)
	{
		min.x += distance;
		max.x += distance;
	}

	constexpr void TranslateY(const float distance)
	{
		min.y += distance;
		max.y += distance;
	}

	// Simple version, may lead to an inverted rectangle, which is fine for Contains/Overlaps test but not for display.
	constexpr void ClipWith(const GalRect& rect)
	{
		min = GalVec2::GalVec2Max(min, rect.min);
		max = GalVec2::GalVec2Min(max, rect.max);
	}

	// Full version, ensure both points are fully clipped.
	constexpr void ClipWithFull(const GalRect& rect)
	{
		min = GalClamp(min, rect.min, rect.max);
		max = GalClamp(max, rect.min, rect.max);
	}

	constexpr void Floor()
	{
		min = GalFloor(min);
		max = GalFloor(max);
	}

	[[nodiscard]] constexpr bool IsInverted() const
	{
		return min.x > max.x || min.y > max.y;
	}
};

/* =====================================================
 *						GalVector 
 *						GalVector 
 *						GalVector 
 *						GalVector 
 *						GalVector 
 *						GalVector 
 *						GalVector 
 *						GalVector 
 *						GalVector 
 *						GalVector 
 *						GalVector 
 *						GalVector 
 *						GalVector 
 *						GalVector 
 *						GalVector 
 *						GalVector 
 *						GalVector 
 *						GalVector 
 *						GalVector 
 * ======================================================
 */
template <typename T>
struct GalVector
{
	size_t size;
	size_t capacity;
	T* data;

	constexpr GalVector() : size(0), capacity(0), data(nullptr) {}
	
	GalVector(const GalVector<T>& src) : size(0), capacity(0), data(nullptr)
	{
		this->operator=(src);
	}

	GalVector& operator=(const GalVector& src)  // NOLINT(bugprone-unhandled-self-assignment, cert-oop54-cpp)
	{
		if(&src == this)
		{
			return *this;
		}
		
		this->Clear();
		this->Resize(src.size);
		::GalMemCpy(data, size * sizeof(T), src.data, size * sizeof(T));
		return *this;
	}

	GalVector(GalVector&& other) noexcept = delete;
	GalVector& operator=(GalVector&& other) noexcept = delete;

	// ReSharper disable once CppInconsistentNaming
	constexpr void swap(GalVector<T>& rhs) noexcept
	{
		using std::swap;
		swap(size, rhs.size);
		swap(capacity, rhs.capacity);
		swap(data, rhs.data);
	}
	
	~GalVector()
	{
		if(data)
		{
			::GalMemFree(data);
		}
	}

	T& operator[](size_t index)
	{
		GAL_ASSERT(index < size);
		return data[index];
	}

	const T& operator[](size_t index) const
	{
		GAL_ASSERT(index < size);
		return data[index];
	}
	
	[[nodiscard]] constexpr bool Empty() const
	{
		return size == 0;
	}

	[[nodiscard]] constexpr size_t Size() const
	{
		return size;
	}

	[[nodiscard]] constexpr size_t SizeInBytes() const
	{
		return size * sizeof(T);
	}

	[[nodiscard]] constexpr size_t Capacity() const
	{
		return capacity;
	}

	constexpr void Clear()
	{
		if(data)
		{
			size = 0;
			capacity = 0;
			::GalMemFree(data);
			data = nullptr;
		}
	}

	constexpr T* Begin()
	{
		return data;
	}

	[[nodiscard]] constexpr const T* Begin() const
	{
		return data;
	}

	constexpr T* End()
	{
		return data + size;
	}

	[[nodiscard]] constexpr const T* End() const
	{
		return data + size;
	}

	constexpr T& Front()
	{
		GAL_ASSERT(size > 0);
		return data[0];
	}

	[[nodiscard]] constexpr const T& Front() const
	{
		GAL_ASSERT(size > 0);
		return data[0];
	}

	constexpr T& Back()
	{
		GAL_ASSERT(size > 0);
		return data[size - 1];
	}

	[[nodiscard]] constexpr const T& Back() const
	{
		GAL_ASSERT(size > 0);
		return data[size - 1];
	}

	constexpr void Reserve(const size_t newCapacity)
	{
		if (newCapacity <= capacity) return;
		auto newData = ::GalMemAlloc(newCapacity * sizeof(T));  // NOLINT(bugprone-sizeof-expression)
		if (data)
		{
			::GalMemCpy(newData, size * sizeof(T), data, size * sizeof(T));  // NOLINT(bugprone-sizeof-expression)
			::GalMemFree(data);
		}
		data = static_cast<T*>(newData);
		capacity = newCapacity;
	}
	
	[[nodiscard]] constexpr size_t GrowCapacity(size_t newSize) const
	{
		auto newCapacity = capacity ? capacity + capacity / 2 : 8;
		return newCapacity > newSize ? newCapacity : newSize;
	}

	constexpr void Resize(const size_t newSize)
	{
		if(newSize > capacity)
		{
			this->Reserve(this->GrowCapacity(newSize));
		}
		size = newSize;
	}

	constexpr void Resize(const size_t newSize, const T& v)
	{
		if (newSize > capacity)
		{
			this->Reserve(this->GrowCapacity(newSize));
		}
		if(newSize > size)
		{
			for(auto n = size; n < newSize; ++n)
			{
				::GalMemCpy(&data[n], sizeof(v), &v, sizeof(v));
			}
		}
		size = newSize;
	}

	constexpr void Shrink(const size_t newSize)
	{
		GAL_ASSERT(newSize <= size);
		size = newSize;
	}

	constexpr void PushBack(const T& v)
	{
		if(size == capacity)
		{
			this->Reserve(this->GrowCapacity(size + 1));
		}
		::GalMemCpy(&data[size], sizeof(v), const_cast<T*>(&v), sizeof(v));  // NOLINT(bugprone-sizeof-expression)
		++size;
	}

	constexpr void PopBack()
	{
		GAL_ASSERT(size > 0);
		--size;
	}

	constexpr void PushFront(const T& v)
	{
		if(size == 0)
		{
			this->PushBack(v);
		}
		else
		{
			this->Insert(data, v);
		}
	}

	constexpr T* Erase(const T* it)
	{
		GAL_ASSERT(it >= data && it < data + size);
		const std::ptrdiff_t off = it - data;
		const auto s = (size - off - 1) * sizeof(T);
		::GalMemMove(data + off, s, data + off + 1, s);
		--size;
		return data + off;
	}

	constexpr T* Erase(const T* it, const T* last)
	{
		GAL_ASSERT(it >= data && it < data + size && last > it && last <= data + size);
		const std::ptrdiff_t count = last - it;
		const std::ptrdiff_t off = it - data;
		const auto s = (size - off - count) * sizeof(T);
		::GalMemMove(data + off, s, data + off + count, s);
		size -= count;
		return data + off;
	}

	constexpr T* EraseUnsorted(const T* it)
	{
		GAL_ASSERT(it >= data && it < data + size);
		const std::ptrdiff_t off = it - data;
		if(it < data + size - 1)
		{
			::GalMemCpy(data + off, sizeof(T), data + size - 1, sizeof(T));
		}
		--size;
		return data + off;
	}

	constexpr T* Insert(const T* it, const T& v)
	{
		GAL_ASSERT(it >= data && it <= data + size);
		const std::ptrdiff_t off = it - data;
		if(size == capacity)
		{
			this->Reserve(this->GrowCapacity(size + 1));
		}
		if(static_cast<size_t>(off) < size)
		{
			const auto s = (size - off) * sizeof(T);
			::GalMemMove(data + off + 1, s, data + off, s);
		}
		::GalMemCpy(&data[off], sizeof(v), &v, sizeof(v));
		++size;
		return data + off;
	}

	[[nodiscard]] constexpr bool Contains(const T& v) const
	{
		T* it = data;
		const T* end = data + size;
		while(it < end)
		{
			if(*it++ == v)
			{
				return true;
			}
		}
		return false;
	}

	constexpr T* Find(const T& v)
	{
		T* it = data;
		const T* end = data + size;
		while(it < end)
		{
			if(*data == v)
			{
				break;
			}
			++it;
		}
		return it;
	}

	constexpr size_t IndexFromPtr(const T* it) const
	{
		GAL_ASSERT(it >= data && it < data + size);
		const std::ptrdiff_t off = it - data;
		return off;
	}
};

/* =====================================================
 *						GalBitVec
 *						GalBitVec
 *						GalBitVec
 *						GalBitVec
 *						GalBitVec
 *						GalBitVec
 *						GalBitVec
 *						GalBitVec
 *						GalBitVec
 *						GalBitVec
 *						GalBitVec
 *						GalBitVec
 *						GalBitVec
 *						GalBitVec
 *						GalBitVec
 *						GalBitVec
 *						GalBitVec
 *						GalBitVec
 *						GalBitVec
 * ======================================================
 */
struct GalBitVec
{
	GalVector<GalU32> storage;

	void Create(const size_t size)
	{
		storage.Resize((size + 31) >> 5);
		GalMemSet(storage.data, 0, storage.size * sizeof(storage.data[0]));
	}

	constexpr void Clear()
	{
		storage.Clear();
	}

	[[nodiscard]] constexpr bool TestBit(const size_t n) const
	{
		GAL_ASSERT(n < (storage.size << 5));
		
		const GalU32 mask = 1 << (n & 31);
		return (storage.data[n >> 5] & mask) != 0;
	}

	constexpr void SetBit(const size_t n) const
	{
		GAL_ASSERT(n < (storage.size << 5));

		const GalU32 mask = 1 << (n & 31);
		storage.data[n >> 5] |= mask;
	}

	constexpr void ClearBit(const size_t n) const
	{
		GAL_ASSERT(n < (storage.size << 5));

		const GalU32 mask = 1 << (n & 31);
		storage.data[n >> 5] &= ~mask;
	}

	constexpr void SetBitRange(int n, const int n2) const
	{
		while (n <= n2)
		{
			const auto aMod = n & 31;
			const auto bMod = ((n2 >= n + 31) ? 31 : (n2 & 31)) + 1;
			const auto mask =
				static_cast<GalU32>((static_cast<GalU64>(1) << bMod) - 1)
				&
				~static_cast<GalU32>((static_cast<GalU64>(1) << aMod) - 1);
			storage.data[n >> 5] |= mask;
			n = (n + 32) & ~31;
		}
	}
};

/* =====================================================
 *						GalGuiStorage
 *						GalGuiStorage
 *						GalGuiStorage
 *						GalGuiStorage
 *						GalGuiStorage
 *						GalGuiStorage
 *						GalGuiStorage
 *						GalGuiStorage
 *						GalGuiStorage
 *						GalGuiStorage
 *						GalGuiStorage
 *						GalGuiStorage
 *						GalGuiStorage
 *						GalGuiStorage
 *						GalGuiStorage
 *						GalGuiStorage
 *						GalGuiStorage
 *						GalGuiStorage
 *						GalGuiStorage
 * ======================================================
 */
 // template <typename PtrType>
 // TODO 这里应该可以使用模版,但是有很多问题
 // TODO 可以用参数包而不是写死的 int, float, PtrType 作为类型,但是我们缺少类型检查的手段
 // TODO 注意: 如果修改了 std::variant<int, float, void*> val 的类型,那么下面的函数也要修改,因为他们不得不写死
struct GalGuiStorage
{
	// [Internal]
	struct GalGuiStoragePair
	{
		GalGuiID key;

		std::variant<int, float, void*> val;

		constexpr GalGuiStoragePair(const GalGuiID key, const int val) : key(key), val(val) {  }
		constexpr GalGuiStoragePair(const GalGuiID key, const float val) : key(key), val(val) {}
		constexpr GalGuiStoragePair(const GalGuiID key, void* val) : key(key), val(val) {}
	};

	GalVector<GalGuiStoragePair> data;

	constexpr void Clear()
	{
		data.Clear();
	}

	// 将其作为静态的主要原因是我们的 Getter 需要 const 限定符,而这个函数不能加 const 限定符(因为 setter 会通过这个函数的返回值修改数据)
	static constexpr GalGuiStoragePair* LowerBound(GalVector<GalGuiStoragePair>& data, const GalGuiID key)
	{
		auto first = data.Begin();
		auto count = data.Size();

		while (count > 0)
		{
			const auto count2 = count >> 2;
			auto mid = first + count2;
			if (mid->key < key)
			{
				first = ++mid;
				count -= count2 + 1;
			}
			else
			{
				count = count2;
			}
		}
		return first;
	}

	template <typename T>
	constexpr T Get(const GalGuiID key, const T defaultVal = T{})
	{
		if (std::is_same_v<T, bool>)
		{
			return static_cast<bool>(this->Get<int>(key, defaultVal ? 1 : 0));
		}

		// 虽然不会改变data,但是要用到的函数不能返回常指针,其他地方会有修改其值的情况,所以用了个const_cast转型
		const auto it = LowerBound(const_cast<GalVector<GalGuiStoragePair>&>(data), key);
		if (it == data.End() || it->key != key)
		{
			return defaultVal;
		}

		struct Output
		{
			constexpr int operator()(const int v) const
			{
				return v;
			}
			constexpr float operator()(const float v) const
			{
				return v;
			}
			constexpr void* operator()(void* v) const
			{
				return v;
			}
			void operator()(const std::any&) const
			{
				// 不支持的类型抛出异常
				throw std::bad_any_cast();
			}
		};

		return std::visit(Output(), it->val);
	}

	template <typename T>
	constexpr void Set(const GalGuiID key, T val)
	{
		if (std::is_same_v<T, bool>)
		{
			return this->Set<int>(key, val ? 1 : 0);
		}

		const auto it = LowerBound(data, key);
		if (it == data.End() || it->key != key)
		{
			data.Insert(it, { key, val });
			return;
		}

		// TODO 在设置的时候,如果目标的类型与我们的不一致,是算错误呢还是覆盖掉呢?
		// TODO 覆盖
		if (std::get_if<T>(&it->val) == nullptr)
		{
			it->val.emplace<T>(val);
		}
		else
		{
			it->val = val;
		}
	}

	template <typename T>
	constexpr T* GetRef(const GalGuiID key, T val = T{})
	{
		if (std::is_same_v<T, bool>)
		{
			return static_cast<bool*>(this->GetRef<int>(key, val ? 1 : 0));
		}

		const auto it = LowerBound(const_cast<GalVector<GalGuiStoragePair>&>(data), key);
		if (it == data.End() || it->key != key)
		{
			// 取引用的话没有直接插入
			it = data.Insert(it, { key, val });
		}

		struct Output
		{
			constexpr int* operator()(int* v) const
			{
				return v;
			}
			constexpr float* operator()(float* v) const
			{
				return v;
			}
			constexpr void** operator()(void** v) const
			{
				return v;
			}
			void operator()(const std::any&) const
			{
				// 不支持的类型抛出异常
				throw std::bad_any_cast();
			}
		};

		// 注意这里需要传指针过去
		return std::visit(Output(), &it->val);
	}

	constexpr void SetAllInt(const int val)
	{
		for (size_t i = 0; i < data.Size(); ++i)
		{
			auto& v = data[i].val;
			if (std::get_if<int>(&v) == nullptr)
			{
				// TODO 如果目标不是 int 类型怎么处理?
				// TODO 覆盖
				v.emplace<int>(val);
			}
			else
			{
				std::get<int>(data[i].val) = val;
			}
		}
	}

	void BuildSortByKey() const
	{
		if (data.size > 1)
		{
			qsort(data.data, data.Size(), sizeof(GalGuiStoragePair),
				[](const void* lhs, const void* rhs) -> int
				{
					if (static_cast<const GalGuiStoragePair*>(lhs)->key > static_cast<const GalGuiStoragePair*>(rhs)->key)
					{
						return 1;
					}
					if (static_cast<const GalGuiStoragePair*>(lhs)->key < static_cast<const GalGuiStoragePair*>(rhs)->key)
					{
						return -1;
					}
					return 0;
				}
			);
		}
	}
};

/* =====================================================
 *						GalPool
 *						GalPool
 *						GalPool
 *						GalPool
 *						GalPool
 *						GalPool
 *						GalPool
 *						GalPool
 *						GalPool
 *						GalPool
 *						GalPool
 *						GalPool
 *						GalPool
 *						GalPool
 *						GalPool
 *						GalPool
 *						GalPool
 *						GalPool
 *						GalPool
 * ======================================================
 */
using GalPoolIndex = int;
template <typename T>
struct GalPool
{
	GalVector<T> buffer;		// Contiguous data
	GalGuiStorage map;			// ID->Index
	GalPoolIndex freeIndex;		// Next free idx to use

	GalPool() : freeIndex(0) {}
	~GalPool() { Clear(); };


	GalPool(const GalPool& other) = delete;
	GalPool(GalPool&& other) noexcept = delete;
	GalPool& operator=(const GalPool& other) = delete;
	GalPool& operator=(GalPool&& other) noexcept = delete;

	constexpr T* GetByKey(const GalGuiID key)
	{
		auto index = map.Get<int>(key, -1);
		return index != -1 ? &buffer[index] : nullptr;
	}

	constexpr T* GetByIndex(const GalPoolIndex index)
	{
		return &buffer[index];
	}

	constexpr GalPoolIndex GetIndex(const T* it) const
	{
		return buffer.IndexFromPtr(it);
	}

	constexpr T* GetOrAddByKey(const GalGuiID key)
	{
		const auto index = map.GetRef<int>(key, -1);
		if(*index != -1)
		{
			return &buffer[*index];
		}
		*index = freeIndex;
		return this->Add();
	}

	constexpr bool Contains(const T* it)
	{
		return it >= buffer.Begin() && it < buffer.End();
	}

	constexpr void Clear()
	{
		for(int i = 0; i < map.data.Size(); ++i)
		{
			auto index = std::get<int>(map.data[i].val);
			if(index != -1)
			{
				buffer[index].~T();
			}
		}
		map.Clear();
		buffer.Clear();
		freeIndex = 0;
	}

	constexpr T* Add()
	{
		auto index = freeIndex;
		if(index == buffer.Size())
		{
			buffer.Resize(index + 1);
			++freeIndex;
		}
		else
		{
			freeIndex = *static_cast<int*>(&buffer[index]);
		}

		::GalPlacementNew<T>(&buffer[index]);
		
		return &buffer[index];
	}

	constexpr void Remove(const GalGuiID key, const T* it)
	{
		this->Remove(key, this->GetIndex(it));
	}

	constexpr void Remove(const GalGuiID key, const GalPoolIndex index)
	{
		buffer[index].~T();
		*static_cast<int*>(buffer[index]) = freeIndex;
		freeIndex = index;
		map.Set<int>(key, -1);
	}

	constexpr void Reserve(size_t capacity)
	{
		buffer.Resize(capacity);
		map.data.Resize(capacity);
	}

	[[nodiscard]] constexpr size_t Size() const
	{
		return buffer.Size();
	}
};






























































struct GalFontConfig;
struct GalFontGlyph;
struct GalFontAtlasCustomRect;
struct GalFontGlyphRangesBuilder;
struct GalFontAtlas;
struct GalFont;



struct GalFontConfig
{
	void* fontData = nullptr;						// TTF/OTF data
	int fontDataSize = 0;							// TTF/OTF data size
	bool fontDataOwnedByAtlas = true;				// TTF/OTF data ownership taken by the container ImFontAtlas (will delete memory itself).
	int fontNo = 0;									// Index of font within TTF/OTF file
	float sizePixels = 0.0f;						// Size in pixels for rasterizer (more or less maps to the resulting font height).
	int overSampleH = 3;							// Rasterizer at higher quality for sub-pixel positioning. Read https://github.com/nothings/stb/blob/master/tests/oversample/README.md for details.
	int overSampleV = 1;							// Rasterizer at higher quality for sub-pixel positioning. We don't use sub-pixel positions on the Y axis.
	bool pixelSnapH = false;						// Align every glyph to pixel boundary. Useful e.g. if you are merging a non-pixel aligned font with the default font. If enabled, you can set OversampleH/V to 1.
	GalVec2 glyphExtraSpacing = { 0.0f, 0.0f };	// Extra spacing (in pixels) between glyphs. Only X axis is supported for now.
	GalVec2 glyphOffset = { 0.0f, 0.0f };		// Offset all glyphs from this font input.
	const GalWChar* glyphRanges = nullptr;			// Pointer to a user-provided list of Unicode range (2 value per range, values are inclusive, zero-terminated list). THE ARRAY DATA NEEDS TO PERSIST AS LONG AS THE FONT IS ALIVE.
	float glyphMinAdvanceX = 0.0f;					// Minimum AdvanceX for glyphs, set Min to align font icons, set both Min/Max to enforce mono-space font
	float glyphMaxAdvanceX = FLT_MAX;				// Maximum AdvanceX for glyphs
	bool mergeMode = false;							// Merge into previous ImFont, so you can combine multiple inputs font into one ImFont (e.g. ASCII font + icons + Japanese glyphs). You may want to use GlyphOffset.y when merge font of different heights.
	unsigned int rasterizerFlags = 0x00;			// Settings for custom font rasterizer (e.g. ImGuiFreeType). Leave as zero if you aren't using one.
	float rasterizerMultiply = 1.0f;				// Brighten (>1.0f) or darken (<1.0f) font output. Brightening small fonts may be a good workaround to make them more readable.
	GalWChar ellipsisChar = -1;						// Explicitly specify unicode codepoint of ellipsis character. When fonts are being merged first specified ellipsis will be used.

	// [Internal]
	char name[64] = {0};
	
	GalFont* destFont = nullptr;
};

struct GalFontGlyph
{
	unsigned int codePoint : 31;	// 0x0000..0xFFFF
	unsigned int visible : 1;		// Flag to allow early out when rendering
	float advanceX;					// Distance to next character (= data from font + ImFontConfig::GlyphExtraSpacing.x baked in)
	float x0;
	float y0;
	float x1;
	float y1;
	float u0;
	float v0;
	float u1;
	float v1;
};

struct GalFontAtlasCustomRect
{
	// [Input]
	unsigned short width = 0;
	unsigned short height = 0;
	unsigned int glyphID = 0;
	float glyphAdvanceX = 0.0f;
	GalVec2 glyphOffset = {0.0f, 0.0f};
	GalFont* font = nullptr;
	// [Output]
	unsigned short x = 0xffff;
	unsigned short y = 0xffff;

	[[nodiscard]] constexpr bool IsPacked() const
	{
		return x != 0xffff;
	}
};

struct GalFontGlyphRangesBuilder
{
	GalVector<GalU32> usedChars;

	void Clear()
	{
		const auto sizeInBytes = (GalUnicodeCodePointMax + 1) / 8;
		usedChars.Resize(sizeInBytes / sizeof(GalU32));
		memset(usedChars.data, 0, sizeInBytes);
	}

	[[nodiscard]] constexpr bool GetBit(const size_t n) const
	{
		const auto off = n >> 5;
		const auto mask = 1u << (n & 31);
		return (usedChars[off] & mask) != 0;
	}

	constexpr void SetBit(const size_t n)
	{
		const auto off = n >> 5;
		const auto mask = 1u << (n & 31);
		usedChars[off] |= mask;
	}

	constexpr void AddChar(const GalWChar c)
	{
		SetBit(c);
	}

	constexpr void AddText(const char* text, const char* textEnd = nullptr)
	{
		while(textEnd ? (text < textEnd) : *text)
		{
			unsigned int c = 0;
			const int len = GalTextCharFromUtf8(&c, text, textEnd);
			if(len == 0)
			{
				break;
			}
			text += len;
			AddChar(c);
		}
	}

	constexpr void AddRanges(const GalWChar* ranges)
	{
		for(;ranges[0]; ranges += 2)
		{
			for(auto c = ranges[0]; c <= ranges[1]; ++c)
			{
				AddChar(c);
			}
		}
	}

	constexpr void BuildRanges(GalVector<GalWChar>& outRanges) const
	{
		const auto max = GalUnicodeCodePointMax;
		for(int i = 0; i <= max; ++i)
		{
			if(GetBit(i))
			{
				outRanges.PushBack(i);
				while(i < max && GetBit(i + 1))
				{
					++i;
				}
				outRanges.PushBack(i);
			}
		}
		outRanges.PushBack(0);
	}
};

static constexpr char ProggyCleanTtfCompressedDataBase85[11980 + 1] =
{
	"7])#######hV0qs'/###[),##/l:$#Q6>##5[n42>c-TH`->>#/e>11NNV=Bv(*:.F?uu#(gRU.o0XGH`$vhLG1hxt9?W`#,5LsCp#-i>.r$<$6pD>Lb';9Crc6tgXmKVeU2cD4Eo3R/"
	"2*>]b(MC;$jPfY.;h^`IWM9<Lh2TlS+f-s$o6Q<BWH`YiU.xfLq$N;$0iR/GX:U(jcW2p/W*q?-qmnUCI;jHSAiFWM.R*kU@C=GH?a9wp8f$e.-4^Qg1)Q-GL(lf(r/7GrRgwV%MS=C#"
	"`8ND>Qo#t'X#(v#Y9w0#1D$CIf;W'#pWUPXOuxXuU(H9M(1<q-UE31#^-V'8IRUo7Qf./L>=Ke$$'5F%)]0^#0X@U.a<r:QLtFsLcL6##lOj)#.Y5<-R&KgLwqJfLgN&;Q?gI^#DY2uL"
	"i@^rMl9t=cWq6##weg>$FBjVQTSDgEKnIS7EM9>ZY9w0#L;>>#Mx&4Mvt//L[MkA#W@lK.N'[0#7RL_&#w+F%HtG9M#XL`N&.,GM4Pg;-<nLENhvx>-VsM.M0rJfLH2eTM`*oJMHRC`N"
	"kfimM2J,W-jXS:)r0wK#@Fge$U>`w'N7G#$#fB#$E^$#:9:hk+eOe--6x)F7*E%?76%^GMHePW-Z5l'&GiF#$956:rS?dA#fiK:)Yr+`&#0j@'DbG&#^$PG.Ll+DNa<XCMKEV*N)LN/N"
	"*b=%Q6pia-Xg8I$<MR&,VdJe$<(7G;Ckl'&hF;;$<_=X(b.RS%%)###MPBuuE1V:v&cX&#2m#(&cV]`k9OhLMbn%s$G2,B$BfD3X*sp5#l,$R#]x_X1xKX%b5U*[r5iMfUo9U`N99hG)"
	"tm+/Us9pG)XPu`<0s-)WTt(gCRxIg(%6sfh=ktMKn3j)<6<b5Sk_/0(^]AaN#(p/L>&VZ>1i%h1S9u5o@YaaW$e+b<TWFn/Z:Oh(Cx2$lNEoN^e)#CFY@@I;BOQ*sRwZtZxRcU7uW6CX"
	"ow0i(?$Q[cjOd[P4d)]>ROPOpxTO7Stwi1::iB1q)C_=dV26J;2,]7op$]uQr@_V7$q^%lQwtuHY]=DX,n3L#0PHDO4f9>dC@O>HBuKPpP*E,N+b3L#lpR/MrTEH.IAQk.a>D[.e;mc."
	"x]Ip.PH^'/aqUO/$1WxLoW0[iLA<QT;5HKD+@qQ'NQ(3_PLhE48R.qAPSwQ0/WK?Z,[x?-J;jQTWA0X@KJ(_Y8N-:/M74:/-ZpKrUss?d#dZq]DAbkU*JqkL+nwX@@47`5>w=4h(9.`G"
	"CRUxHPeR`5Mjol(dUWxZa(>STrPkrJiWx`5U7F#.g*jrohGg`cg:lSTvEY/EV_7H4Q9[Z%cnv;JQYZ5q.l7Zeas:HOIZOB?G<Nald$qs]@]L<J7bR*>gv:[7MI2k).'2($5FNP&EQ(,)"
	"U]W]+fh18.vsai00);D3@4ku5P?DP8aJt+;qUM]=+b'8@;mViBKx0DE[-auGl8:PJ&Dj+M6OC]O^((##]`0i)drT;-7X`=-H3[igUnPG-NZlo.#k@h#=Ork$m>a>$-?Tm$UV(?#P6YY#"
	"'/###xe7q.73rI3*pP/$1>s9)W,JrM7SN]'/4C#v$U`0#V.[0>xQsH$fEmPMgY2u7Kh(G%siIfLSoS+MK2eTM$=5,M8p`A.;_R%#u[K#$x4AG8.kK/HSB==-'Ie/QTtG?-.*^N-4B/ZM"
	"_3YlQC7(p7q)&](`6_c)$/*JL(L-^(]$wIM`dPtOdGA,U3:w2M-0<q-]L_?^)1vw'.,MRsqVr.L;aN&#/EgJ)PBc[-f>+WomX2u7lqM2iEumMTcsF?-aT=Z-97UEnXglEn1K-bnEO`gu"
	"Ft(c%=;Am_Qs@jLooI&NX;]0#j4#F14;gl8-GQpgwhrq8'=l_f-b49'UOqkLu7-##oDY2L(te+Mch&gLYtJ,MEtJfLh'x'M=$CS-ZZ%P]8bZ>#S?YY#%Q&q'3^Fw&?D)UDNrocM3A76/"
	"/oL?#h7gl85[qW/NDOk%16ij;+:1a'iNIdb-ou8.P*w,v5#EI$TWS>Pot-R*H'-SEpA:g)f+O$%%`kA#G=8RMmG1&O`>to8bC]T&$,n.LoO>29sp3dt-52U%VM#q7'DHpg+#Z9%H[K<L"
	"%a2E-grWVM3@2=-k22tL]4$##6We'8UJCKE[d_=%wI;'6X-GsLX4j^SgJ$##R*w,vP3wK#iiW&#*h^D&R?jp7+/u&#(AP##XU8c$fSYW-J95_-Dp[g9wcO&#M-h1OcJlc-*vpw0xUX&#"
	"OQFKNX@QI'IoPp7nb,QU//MQ&ZDkKP)X<WSVL(68uVl&#c'[0#(s1X&xm$Y%B7*K:eDA323j998GXbA#pwMs-jgD$9QISB-A_(aN4xoFM^@C58D0+Q+q3n0#3U1InDjF682-SjMXJK)("
	"h$hxua_K]ul92%'BOU&#BRRh-slg8KDlr:%L71Ka:.A;%YULjDPmL<LYs8i#XwJOYaKPKc1h:'9Ke,g)b),78=I39B;xiY$bgGw-&.Zi9InXDuYa%G*f2Bq7mn9^#p1vv%#(Wi-;/Z5h"
	"o;#2:;%d&#x9v68C5g?ntX0X)pT`;%pB3q7mgGN)3%(P8nTd5L7GeA-GL@+%J3u2:(Yf>et`e;)f#Km8&+DC$I46>#Kr]]u-[=99tts1.qb#q72g1WJO81q+eN'03'eM>&1XxY-caEnO"
	"j%2n8)),?ILR5^.Ibn<-X-Mq7[a82Lq:F&#ce+S9wsCK*x`569E8ew'He]h:sI[2LM$[guka3ZRd6:t%IG:;$%YiJ:Nq=?eAw;/:nnDq0(CYcMpG)qLN4$##&J<j$UpK<Q4a1]MupW^-"
	"sj_$%[HK%'F####QRZJ::Y3EGl4'@%FkiAOg#p[##O`gukTfBHagL<LHw%q&OV0##F=6/:chIm0@eCP8X]:kFI%hl8hgO@RcBhS-@Qb$%+m=hPDLg*%K8ln(wcf3/'DW-$.lR?n[nCH-"
	"eXOONTJlh:.RYF%3'p6sq:UIMA945&^HFS87@$EP2iG<-lCO$%c`uKGD3rC$x0BL8aFn--`ke%#HMP'vh1/R&O_J9'um,.<tx[@%wsJk&bUT2`0uMv7gg#qp/ij.L56'hl;.s5CUrxjO"
	"M7-##.l+Au'A&O:-T72L]P`&=;ctp'XScX*rU.>-XTt,%OVU4)S1+R-#dg0/Nn?Ku1^0f$B*P:Rowwm-`0PKjYDDM'3]d39VZHEl4,.j']Pk-M.h^&:0FACm$maq-&sgw0t7/6(^xtk%"
	"LuH88Fj-ekm>GA#_>568x6(OFRl-IZp`&b,_P'$M<Jnq79VsJW/mWS*PUiq76;]/NM_>hLbxfc$mj`,O;&%W2m`Zh:/)Uetw:aJ%]K9h:TcF]u_-Sj9,VK3M.*'&0D[Ca]J9gp8,kAW]"
	"%(?A%R$f<->Zts'^kn=-^@c4%-pY6qI%J%1IGxfLU9CP8cbPlXv);C=b),<2mOvP8up,UVf3839acAWAW-W?#ao/^#%KYo8fRULNd2.>%m]UK:n%r$'sw]J;5pAoO_#2mO3n,'=H5(et"
	"Hg*`+RLgv>=4U8guD$I%D:W>-r5V*%j*W:Kvej.Lp$<M-SGZ':+Q_k+uvOSLiEo(<aD/K<CCc`'Lx>'?;++O'>()jLR-^u68PHm8ZFWe+ej8h:9r6L*0//c&iH&R8pRbA#Kjm%upV1g:"
	"a_#Ur7FuA#(tRh#.Y5K+@?3<-8m0$PEn;J:rh6?I6uG<-`wMU'ircp0LaE_OtlMb&1#6T.#FDKu#1Lw%u%+GM+X'e?YLfjM[VO0MbuFp7;>Q&#WIo)0@F%q7c#4XAXN-U&VB<HFF*qL("
	"$/V,;(kXZejWO`<[5?\?ewY(*9=%wDc;,u<'9t3W-(H1th3+G]ucQ]kLs7df($/*JL]@*t7Bu_G3_7mp7<iaQjO@.kLg;x3B0lqp7Hf,^Ze7-##@/c58Mo(3;knp0%)A7?-W+eI'o8)b<"
	"nKnw'Ho8C=Y>pqB>0ie&jhZ[?iLR@@_AvA-iQC(=ksRZRVp7`.=+NpBC%rh&3]R:8XDmE5^V8O(x<<aG/1N$#FX$0V5Y6x'aErI3I$7x%E`v<-BY,)%-?Psf*l?%C3.mM(=/M0:JxG'?"
	"7WhH%o'a<-80g0NBxoO(GH<dM]n.+%q@jH?f.UsJ2Ggs&4<-e47&Kl+f//9@`b+?.TeN_&B8Ss?v;^Trk;f#YvJkl&w$]>-+k?'(<S:68tq*WoDfZu';mM?8X[ma8W%*`-=;D.(nc7/;"
	")g:T1=^J$&BRV(-lTmNB6xqB[@0*o.erM*<SWF]u2=st-*(6v>^](H.aREZSi,#1:[IXaZFOm<-ui#qUq2$##Ri;u75OK#(RtaW-K-F`S+cF]uN`-KMQ%rP/Xri.LRcB##=YL3BgM/3M"
	"D?@f&1'BW-)Ju<L25gl8uhVm1hL$##*8###'A3/LkKW+(^rWX?5W_8g)a(m&K8P>#bmmWCMkk&#TR`C,5d>g)F;t,4:@_l8G/5h4vUd%&%950:VXD'QdWoY-F$BtUwmfe$YqL'8(PWX("
	"P?^@Po3$##`MSs?DWBZ/S>+4%>fX,VWv/w'KD`LP5IbH;rTV>n3cEK8U#bX]l-/V+^lj3;vlMb&[5YQ8#pekX9JP3XUC72L,,?+Ni&co7ApnO*5NK,((W-i:$,kp'UDAO(G0Sq7MVjJs"
	"bIu)'Z,*[>br5fX^:FPAWr-m2KgL<LUN098kTF&#lvo58=/vjDo;.;)Ka*hLR#/k=rKbxuV`>Q_nN6'8uTG&#1T5g)uLv:873UpTLgH+#FgpH'_o1780Ph8KmxQJ8#H72L4@768@Tm&Q"
	"h4CB/5OvmA&,Q&QbUoi$a_%3M01H)4x7I^&KQVgtFnV+;[Pc>[m4k//,]1?#`VY[Jr*3&&slRfLiVZJ:]?=K3Sw=[$=uRB?3xk48@aeg<Z'<$#4H)6,>e0jT6'N#(q%.O=?2S]u*(m<-"
	"V8J'(1)G][68hW$5'q[GC&5j`TE?m'esFGNRM)j,ffZ?-qx8;->g4t*:CIP/[Qap7/9'#(1sao7w-.qNUdkJ)tCF&#B^;xGvn2r9FEPFFFcL@.iFNkTve$m%#QvQS8U@)2Z+3K:AKM5i"
	"sZ88+dKQ)W6>J%CL<KE>`.d*(B`-n8D9oK<Up]c$X$(,)M8Zt7/[rdkqTgl-0cuGMv'?>-XV1q['-5k'cAZ69e;D_?$ZPP&s^+7])$*$#@QYi9,5P&#9r+$%CE=68>K8r0=dSC%%(@p7"
	".m7jilQ02'0-VWAg<a/''3u.=4L$Y)6k/K:_[3=&jvL<L0C/2'v:^;-DIBW,B4E68:kZ;%?8(Q8BH=kO65BW?xSG&#@uU,DS*,?.+(o(#1vCS8#CHF>TlGW'b)Tq7VT9q^*^$$.:&N@@"
	"$&)WHtPm*5_rO0&e%K&#-30j(E4#'Zb.o/(Tpm$>K'f@[PvFl,hfINTNU6u'0pao7%XUp9]5.>%h`8_=VYbxuel.NTSsJfLacFu3B'lQSu/m6-Oqem8T+oE--$0a/k]uj9EwsG>%veR*"
	"hv^BFpQj:K'#SJ,sB-'#](j.Lg92rTw-*n%@/;39rrJF,l#qV%OrtBeC6/,;qB3ebNW[?,Hqj2L.1NP&GjUR=1D8QaS3Up&@*9wP?+lo7b?@%'k4`p0Z$22%K3+iCZj?XJN4Nm&+YF]u"
	"@-W$U%VEQ/,,>>#)D<h#`)h0:<Q6909ua+&VU%n2:cG3FJ-%@Bj-DgLr`Hw&HAKjKjseK</xKT*)B,N9X3]krc12t'pgTV(Lv-tL[xg_%=M_q7a^x?7Ubd>#%8cY#YZ?=,`Wdxu/ae&#"
	"w6)R89tI#6@s'(6Bf7a&?S=^ZI_kS&ai`&=tE72L_D,;^R)7[$s<Eh#c&)q.MXI%#v9ROa5FZO%sF7q7Nwb&#ptUJ:aqJe$Sl68%.D###EC><?-aF&#RNQv>o8lKN%5/$(vdfq7+ebA#"
	"u1p]ovUKW&Y%q]'>$1@-[xfn$7ZTp7mM,G,Ko7a&Gu%G[RMxJs[0MM%wci.LFDK)(<c`Q8N)jEIF*+?P2a8g%)$q]o2aH8C&<SibC/q,(e:v;-b#6[$NtDZ84Je2KNvB#$P5?tQ3nt(0"
	"d=j.LQf./Ll33+(;q3L-w=8dX$#WF&uIJ@-bfI>%:_i2B5CsR8&9Z&#=mPEnm0f`<&c)QL5uJ#%u%lJj+D-r;BoF&#4DoS97h5g)E#o:&S4weDF,9^Hoe`h*L+_a*NrLW-1pG_&2UdB8"
	"6e%B/:=>)N4xeW.*wft-;$'58-ESqr<b?UI(_%@[P46>#U`'6AQ]m&6/`Z>#S?YY#Vc;r7U2&326d=w&H####?TZ`*4?&.MK?LP8Vxg>$[QXc%QJv92.(Db*B)gb*BM9dM*hJMAo*c&#"
	"b0v=Pjer]$gG&JXDf->'StvU7505l9$AFvgYRI^&<^b68?j#q9QX4SM'RO#&sL1IM.rJfLUAj221]d##DW=m83u5;'bYx,*Sl0hL(W;;$doB&O/TQ:(Z^xBdLjL<Lni;''X.`$#8+1GD"
	":k$YUWsbn8ogh6rxZ2Z9]%nd+>V#*8U_72Lh+2Q8Cj0i:6hp&$C/:p(HK>T8Y[gHQ4`4)'$Ab(Nof%V'8hL&#<NEdtg(n'=S1A(Q1/I&4([%dM`,Iu'1:_hL>SfD07&6D<fp8dHM7/g+"
	"tlPN9J*rKaPct&?'uBCem^jn%9_K)<,C5K3s=5g&GmJb*[SYq7K;TRLGCsM-$$;S%:Y@r7AK0pprpL<Lrh,q7e/%KWK:50I^+m'vi`3?%Zp+<-d+$L-Sv:@.o19n$s0&39;kn;S%BSq*"
	"$3WoJSCLweV[aZ'MQIjO<7;X-X;&+dMLvu#^UsGEC9WEc[X(wI7#2.(F0jV*eZf<-Qv3J-c+J5AlrB#$p(H68LvEA'q3n0#m,[`*8Ft)FcYgEud]CWfm68,(aLA$@EFTgLXoBq/UPlp7"
	":d[/;r_ix=:TF`S5H-b<LI&HY(K=h#)]Lk$K14lVfm:x$H<3^Ql<M`$OhapBnkup'D#L$Pb_`N*g]2e;X/Dtg,bsj&K#2[-:iYr'_wgH)NUIR8a1n#S?Yej'h8^58UbZd+^FKD*T@;6A"
	"7aQC[K8d-(v6GI$x:T<&'Gp5Uf>@M.*J:;$-rv29'M]8qMv-tLp,'886iaC=Hb*YJoKJ,(j%K=H`K.v9HggqBIiZu'QvBT.#=)0ukruV&.)3=(^1`o*Pj4<-<aN((^7('#Z0wK#5GX@7"
	"u][`*S^43933A4rl][`*O4CgLEl]v$1Q3AeF37dbXk,.)vj#x'd`;qgbQR%FW,2(?LO=s%Sc68%NP'##Aotl8x=BE#j1UD([3$M(]UI2LX3RpKN@;/#f'f/&_mt&F)XdF<9t4)Qa.*kT"
	"LwQ'(TTB9.xH'>#MJ+gLq9-##@HuZPN0]u:h7.T..G:;$/Usj(T7`Q8tT72LnYl<-qx8;-HV7Q-&Xdx%1a,hC=0u+HlsV>nuIQL-5<N?)NBS)QN*_I,?&)2'IM%L3I)X((e/dl2&8'<M"
	":^#M*Q+[T.Xri.LYS3v%fF`68h;b-X[/En'CR.q7E)p'/kle2HM,u;^%OKC-N+Ll%F9CF<Nf'^#t2L,;27W:0O@6##U6W7:$rJfLWHj$#)woqBefIZ.PK<b*t7ed;p*_m;4ExK#h@&]>"
	"_>@kXQtMacfD.m-VAb8;IReM3$wf0''hra*so568'Ip&vRs849'MRYSp%:t:h5qSgwpEr$B>Q,;s(C#$)`svQuF$##-D,##,g68@2[T;.XSdN9Qe)rpt._K-#5wF)sP'##p#C0c%-Gb%"
	"hd+<-j'Ai*x&&HMkT]C'OSl##5RG[JXaHN;d'uA#x._U;.`PU@(Z3dt4r152@:v,'R.Sj'w#0<-;kPI)FfJ&#AYJ&#//)>-k=m=*XnK$>=)72L]0I%>.G690a:$##<,);?;72#?x9+d;"
	"^V'9;jY@;)br#q^YQpx:X#Te$Z^'=-=bGhLf:D6&bNwZ9-ZD#n^9HhLMr5G;']d&6'wYmTFmL<LD)F^%[tC'8;+9E#C$g%#5Y>q9wI>P(9mI[>kC-ekLC/R&CH+s'B;K-M6$EB%is00:"
	"+A4[7xks.LrNk0&E)wILYF@2L'0Nb$+pv<(2.768/FrY&h$^3i&@+G%JT'<-,v`3;_)I9M^AE]CN?Cl2AZg+%4iTpT3<n-&%H%b<FDj2M<hH=&Eh<2Len$b*aTX=-8QxN)k11IM1c^j%"
	"9s<L<NFSo)B?+<-(GxsF,^-Eh@$4dXhN$+#rxK8'je'D7k`e;)2pYwPA'_p9&@^18ml1^[@g4t*[JOa*[=Qp7(qJ_oOL^('7fB&Hq-:sf,sNj8xq^>$U4O]GKx'm9)b@p7YsvK3w^YR-"
	"CdQ*:Ir<($u&)#(&?L9Rg3H)4fiEp^iI9O8KnTj,]H?D*r7'M;PwZ9K0E^k&-cpI;.p/6_vwoFMV<->#%Xi.LxVnrU(4&8/P+:hLSKj$#U%]49t'I:rgMi'FL@a:0Y-uA[39',(vbma*"
	"hU%<-SRF`Tt:542R_VV$p@[p8DV[A,?1839FWdF<TddF<9Ah-6&9tWoDlh]&1SpGMq>Ti1O*H&#(AL8[_P%.M>v^-))qOT*F5Cq0`Ye%+$B6i:7@0IX<N+T+0MlMBPQ*Vj>SsD<U4JHY"
	"8kD2)2fU/M#$e.)T4,_=8hLim[&);?UkK'-x?'(:siIfL<$pFM`i<?%W(mGDHM%>iWP,##P`%/L<eXi:@Z9C.7o=@(pXdAO/NLQ8lPl+HPOQa8wD8=^GlPa8TKI1CjhsCTSLJM'/Wl>-"
	"S(qw%sf/@%#B6;/U7K]uZbi^Oc^2n<bhPmUkMw>%t<)'mEVE''n`WnJra$^TKvX5B>;_aSEK',(hwa0:i4G?.Bci.(X[?b*($,=-n<.Q%`(X=?+@Am*Js0&=3bh8K]mL<LoNs'6,'85`"
	"0?t/'_U59@]ddF<#LdF<eWdF<OuN/45rY<-L@&#+fm>69=Lb,OcZV/);TTm8VI;?%OtJ<(b4mq7M6:u?KRdF<gR@2L=FNU-<b[(9c/ML3m;Z[$oF3g)GAWqpARc=<ROu7cL5l;-[A]%/"
	"+fsd;l#SafT/f*W]0=O'$(Tb<[)*@e775R-:Yob%g*>l*:xP?Yb.5)%w_I?7uk5JC+FS(m#i'k.'a0i)9<7b'fs'59hq$*5Uhv##pi^8+hIEBF`nvo`;'l0.^S1<-wUK2/Coh58KKhLj"
	"M=SO*rfO`+qC`W-On.=AJ56>>i2@2LH6A:&5q`?9I3@@'04&p2/LVa*T-4<-i3;M9UvZd+N7>b*eIwg:CC)c<>nO&#<IGe;__.thjZl<%w(Wk2xmp4Q@I#I9,DF]u7-P=.-_:YJ]aS@V"
	"?6*C()dOp7:WL,b&3Rg/.cmM9&r^>$(>.Z-I&J(Q0Hd5Q%7Co-b`-c<N(6r@ip+AurK<m86QIth*#v;-OBqi+L7wDE-Ir8K['m+DDSLwK&/.?-V%U_%3:qKNu$_b*B-kp7NaD'QdWQPK"
	"Yq[@>P)hI;*_F]u`Rb[.j8_Q/<&>uu+VsH$sM9TA%?)(vmJ80),P7E>)tjD%2L=-t#fK[%`v=Q8<FfNkgg^oIbah*#8/Qt$F&:K*-(N/'+1vMB,u()-a.VUU*#[e%gAAO(S>WlA2);Sa"
	">gXm8YB`1d@K#n]76-a$U,mF<fX]idqd)<3,]J7JmW4`6]uks=4-72L(jEk+:bJ0M^q-8Dm_Z?0olP1C9Sa&H[d&c$ooQUj]Exd*3ZM@-WGW2%s',B-_M%>%Ul:#/'xoFM9QX-$.QN'>"
	"[%$Z$uF6pA6Ki2O5:8w*vP1<-1`[G,)-m#>0`P&#eb#.3i)rtB61(o'$?X3B</R90;eZ]%Ncq;-Tl]#F>2Qft^ae_5tKL9MUe9b*sLEQ95C&`=G?@Mj=wh*'3E>=-<)Gt*Iw)'QG:`@I"
	"wOf7&]1i'S01B+Ev/Nac#9S;=;YQpg_6U`*kVY39xK,[/6Aj7:'1Bm-_1EYfa1+o&o4hp7KN_Q(OlIo@S%;jVdn0'1<Vc52=u`3^o-n1'g4v58Hj&6_t7$##?M)c<$bgQ_'SY((-xkA#"
	"Y(,p'H9rIVY-b,'%bCPF7.J<Up^,(dU1VY*5#WkTU>h19w,WQhLI)3S#f$2(eb,jr*b;3Vw]*7NH%$c4Vs,eD9>XW8?N]o+(*pgC%/72LV-u<Hp,3@e^9UB1J+ak9-TN/mhKPg+AJYd$"
	"MlvAF_jCK*.O-^(63adMT->W%iewS8W6m2rtCpo'RS1R84=@paTKt)>=%&1[)*vp'u+x,VrwN;&]kuO9JDbg=pO$J*.jVe;u'm0dr9l,<*wMK*Oe=g8lV_KEBFkO'oU]^=[-792#ok,)"
	"i]lR8qQ2oA8wcRCZ^7w/Njh;?.stX?Q1>S1q4Bn$)K1<-rGdO'$Wr.Lc.CG)$/*JL4tNR/,SVO3,aUw'DJN:)Ss;wGn9A32ijw%FL+Z0Fn.U9;reSq)bmI32U==5ALuG&#Vf1398/pVo"
	"1*c-(aY168o<`JsSbk-,1N;$>0:OUas(3:8Z972LSfF8eb=c-;>SPw7.6hn3m`9^Xkn(r.qS[0;T%&Qc=+STRxX'q1BNk3&*eu2;&8q$&x>Q#Q7^Tf+6<(d%ZVmj2bDi%.3L2n+4W'$P"
	"iDDG)g,r%+?,$@?uou5tSe2aN_AQU*<h`e-GI7)?OK2A.d7_c)?wQ5AS@DL3r#7fSkgl6-++D:'A,uq7SvlB$pcpH'q3n0#_%dY#xCpr-l<F0NR@-##FEV6NTF6##$l84N1w?AO>'IAO"
	"URQ##V^Fv-XFbGM7Fl(N<3DhLGF%q.1rC$#:T__&Pi68%0xi_&[qFJ(77j_&JWoF.V735&T,[R*:xFR*K5>>#`bW-?4Ne_&6Ne_&6Ne_&n`kr-#GJcM6X;uM6X;uM(.a..^2TkL%oR(#"
	";u.T%fAr%4tJ8&><1=GHZ_+m9/#H1F^R#SC#*N=BA9(D?v[UiFY>>^8p,KKF.W]L29uLkLlu/+4T<XoIB&hx=T1PcDaB&;HH+-AFr?(m9HZV)FKS8JCw;SD=6[^/DZUL`EUDf]GGlG&>"
	"w$)F./^n3+rlo+DB;5sIYGNk+i1t-69Jg--0pao7Sm#K)pdHW&;LuDNH@H>#/X-TI(;P>#,Gc>#0Su>#4`1?#8lC?#<xU?#@.i?#D:%@#HF7@#LRI@#P_[@#Tkn@#Xw*A#]-=A#a9OA#"
	"d<F&#*;G##.GY##2Sl##6`($#:l:$#>xL$#B.`$#F:r$#JF.%#NR@%#R_R%#Vke%#Zww%#_-4&#3^Rh%Sflr-k'MS.o?.5/sWel/wpEM0%3'/1)K^f1-d>G21&v(35>V`39V7A4=onx4"
	"A1OY5EI0;6Ibgr6M$HS7Q<)58C5w,;WoA*#[%T*#`1g*#d=#+#hI5+#lUG+#pbY+#tnl+#x$),#&1;,#*=M,#.I`,#2Ur,#6b.-#;w[H#iQtA#m^0B#qjBB#uvTB##-hB#'9$C#+E6C#"
	"/QHC#3^ZC#7jmC#;v)D#?,<D#C8ND#GDaD#KPsD#O]/E#g1A5#KA*1#gC17#MGd;#8(02#L-d3#rWM4#Hga1#,<w0#T.j<#O#'2#CYN1#qa^:#_4m3#o@/=#eG8=#t8J5#`+78#4uI-#"
	"m3B2#SB[8#Q0@8#i[*9#iOn8#1Nm;#^sN9#qh<9#:=x-#P;K2#$%X9#bC+.#Rg;<#mN=.#MTF.#RZO.#2?)4#Y#(/#[)1/#b;L/#dAU/#0Sv;#lY$0#n`-0#sf60#(F24#wrH0#%/e0#"
	"TmD<#%JSMFove:CTBEXI:<eh2g)B,3h2^G3i;#d3jD>)4kMYD4lVu`4m`:&5niUA5@(A5BA1]PBB:xlBCC=2CDLXMCEUtiCf&0g2'tN?PGT4CPGT4CPGT4CPGT4CPGT4CPGT4CPGT4CP"
	"GT4CPGT4CPGT4CPGT4CPGT4CPGT4CP-qekC`.9kEg^+F$kwViFJTB&5KTB&5KTB&5KTB&5KTB&5KTB&5KTB&5KTB&5KTB&5KTB&5KTB&5KTB&5KTB&5KTB&5KTB&5o,^<-28ZI'O?;xp"
	"O?;xpO?;xpO?;xpO?;xpO?;xpO?;xpO?;xpO?;xpO?;xpO?;xpO?;xpO?;xpO?;xp;7q-#lLYI:xvD=#"
};

static constexpr const char* GetDefaultCompressedDataBase85()
{
	return ProggyCleanTtfCompressedDataBase85;
}

// (. = white layer, X = black layer, others are blank)
// The 2x2 white texels on the top left are the ones we'll use everywhere in GalGui to render filled shapes.
constexpr int FontAtlasDefaultTexDataW = 108; // Actual texture will be 2 times that + 1 spacing.
constexpr int FontAtlasDefaultTexDataH = 27;
static constexpr char FontAtlasDefaultTexDataPixels[FontAtlasDefaultTexDataW * FontAtlasDefaultTexDataH + 1] =
{
	"..-         -XXXXXXX-    X    -           X           -XXXXXXX          -          XXXXXXX-     XX          "
	"..-         -X.....X-   X.X   -          X.X          -X.....X          -          X.....X-    X..X         "
	"---         -XXX.XXX-  X...X  -         X...X         -X....X           -           X....X-    X..X         "
	"X           -  X.X  - X.....X -        X.....X        -X...X            -            X...X-    X..X         "
	"XX          -  X.X  -X.......X-       X.......X       -X..X.X           -           X.X..X-    X..X         "
	"X.X         -  X.X  -XXXX.XXXX-       XXXX.XXXX       -X.X X.X          -          X.X X.X-    X..XXX       "
	"X..X        -  X.X  -   X.X   -          X.X          -XX   X.X         -         X.X   XX-    X..X..XXX    "
	"X...X       -  X.X  -   X.X   -    XX    X.X    XX    -      X.X        -        X.X      -    X..X..X..XX  "
	"X....X      -  X.X  -   X.X   -   X.X    X.X    X.X   -       X.X       -       X.X       -    X..X..X..X.X "
	"X.....X     -  X.X  -   X.X   -  X..X    X.X    X..X  -        X.X      -      X.X        -XXX X..X..X..X..X"
	"X......X    -  X.X  -   X.X   - X...XXXXXX.XXXXXX...X -         X.X   XX-XX   X.X         -X..XX........X..X"
	"X.......X   -  X.X  -   X.X   -X.....................X-          X.X X.X-X.X X.X          -X...X...........X"
	"X........X  -  X.X  -   X.X   - X...XXXXXX.XXXXXX...X -           X.X..X-X..X.X           - X..............X"
	"X.........X -XXX.XXX-   X.X   -  X..X    X.X    X..X  -            X...X-X...X            -  X.............X"
	"X..........X-X.....X-   X.X   -   X.X    X.X    X.X   -           X....X-X....X           -  X.............X"
	"X......XXXXX-XXXXXXX-   X.X   -    XX    X.X    XX    -          X.....X-X.....X          -   X............X"
	"X...X..X    ---------   X.X   -          X.X          -          XXXXXXX-XXXXXXX          -   X...........X "
	"X..X X..X   -       -XXXX.XXXX-       XXXX.XXXX       -------------------------------------    X..........X "
	"X.X  X..X   -       -X.......X-       X.......X       -    XX           XX    -           -    X..........X "
	"XX    X..X  -       - X.....X -        X.....X        -   X.X           X.X   -           -     X........X  "
	"      X..X          -  X...X  -         X...X         -  X..X           X..X  -           -     X........X  "
	"       XX           -   X.X   -          X.X          - X...XXXXXXXXXXXXX...X -           -     XXXXXXXXXX  "
	"------------        -    X    -           X           -X.....................X-           ------------------"
	"                    ----------------------------------- X...XXXXXXXXXXXXX...X -                             "
	"                                                      -  X..X           X..X  -                             "
	"                                                      -   X.X           X.X   -                             "
	"                                                      -    XX           XX    -                             "
};

static constexpr GalVec2 FontAtlasDefaultTexCursorData[static_cast<GalGuiMouseCursor>(EnumGalGuiMouseCursor::GALGUI_MOUSE_CURSOR_COUNT)][3] =
{
	// pos ============= size ============= offset
	{{0, 3}, {12, 19}, {0, 0}},			// ARROW
	{{13, 0}, {7, 16}, {1, 8}},		    // TEXT_INPUT
	{{31, 0}, {23, 23}, {11, 11}},	    // RESIZE_ALL
	{{21, 0}, {9, 23}, {4, 11}},	    // RESIZE_HEIGHT
	{{55, 18}, {23, 9}, {11, 4}},	    // RESIZE_WIDTH
	{{73, 0}, {17, 17}, {8, 8}},	    // RESIZE_BL(Bottom Left)
	{{55, 0}, {17, 17}, {8, 8}},	    // RESIZE_BR(Bottom Right)
	{{91, 0}, {17, 22}, {5, 0}},	    // HAND
};

struct GalFontAtlas
{
	bool locked = false;				
	GalFontAtlasFlags flags = static_cast<GalFontAtlasFlags>(EnumGalFontAtlasFlags::GAL_FONT_ATLAS_FLAGS_NONE);
	GalTextureID texID = nullptr;	
	int texDesireWidth = 0;			
	int texGlyphPadding = 1;		

	// [Internal]
	unsigned char* texPixelsAlpha8 = nullptr;
	unsigned int* texPixelsRGBA32 = nullptr;
	int texWidth = 0;
	int texHeight = 0;
	GalVec2 texUVScale = { 0.0f, 0.0f };
	GalVec2 texUVWhitePixel = { 0.0f, 0.0f };
	GalVector<GalFont*> fonts;
	GalVector<GalFontAtlasCustomRect> customRects;
	GalVector<GalFontConfig> configData;
	GalVec4 texUVLines[GalDrawListTexLinesWidthMax + 1];

	// [Internal] Packing data
	int packIDMouseCursors = -1;	// Custom texture rectangle ID for white pixel and mouse cu
	int packIDLines = -1;		// Custom texture rectangle ID for baked anti-aliased lines

	~GalFontAtlas()
	{
		GAL_ASSERT(!locked && "Cannot modify a locked GalFontAtlas between NewFrame() and EndFrame/Render()!");
		Clear();
	}

	// 需要实现在 GalFont 有完整定义之后
	constexpr GalFont* AddFont(const GalFontConfig* config);
	constexpr GalFont* AddFontDefault(const GalFontConfig* configTemplate);
	constexpr GalFont* AddFontFromFileTTF(const char* filename, float sizePixels, const GalFontConfig* configTemplate = nullptr, const GalWChar* glyphRanges = nullptr);
	constexpr GalFont* AddFontFromMemoryTTF(void* fontData, size_t fontSize, float sizePixels, const GalFontConfig* configTemplate = nullptr, const GalWChar* glyphRanges = nullptr);
	constexpr GalFont* AddFontFromMemoryCompressedTTF(const void* compressedFontData, size_t compressedFontSize, float sizePixels, const GalFontConfig* configTemplate = nullptr, const GalWChar* glyphRanges = nullptr);
	constexpr GalFont* AddFontFromMemoryCompressedBase85TTF(const char* compressedFontDataBase85, float sizePixels, const GalFontConfig* config = nullptr, const GalWChar* glyphRanges = nullptr);

	constexpr void ClearInputData();
	constexpr void ClearTexData();
	constexpr void ClearFonts();
	constexpr void Clear();

	constexpr bool Build();
	constexpr void GetTexDataAsAlpha8(unsigned char** outPixels, int* outWidth, int* outHeight, int* outBytesPerPixels = nullptr);// 1 byte per-pixel
	constexpr void GetTexDataAsRGBA32(unsigned char** outPixels, int* outWidth, int* outHeight, int* outBytesPerPixels = nullptr);// 4 byte per-pixel

	[[nodiscard]] constexpr bool IsBuilt() const
	{
		return fonts.Size() > 0 && (texPixelsAlpha8 != nullptr || texPixelsRGBA32 != nullptr);
	}
	
	void SetTexID(const GalTextureID id)
	{
		texID = id;
	}
	
	static const GalWChar* GetGlyphRangesDefault()
	{
		static const GalWChar Ranges[] =
		{
			0x0020, 0x00FF, // Basic Latin + Latin Supplement
			0,
		};
		return &Ranges[0];
	}
	static const GalWChar* GetGlyphRangesChineseFull()
	{
		static const GalWChar Ranges[] =
		{
			0x0020, 0x00FF, // Basic Latin + Latin Supplement
			0x2000, 0x206F, // General Punctuation
			0x3000, 0x30FF, // CJK Symbols and Punctuations, Hiragana, Katakana
			0x31F0, 0x31FF, // Katakana Phonetic Extensions
			0xFF00, 0xFFEF, // Half-width characters
			0x4e00, 0x9FAF, // CJK Ideograms
			0,
		};
		return &Ranges[0];
	}
	static const GalWChar* GetGlyphRangesChineseSimplifiedCommon()
	{
		static const short AccumulativeOffsetsFrom0X4E00[] =
		{
			0,1,2,4,1,1,1,1,2,1,3,2,1,2,2,1,1,1,1,1,5,2,1,2,3,3,3,2,2,4,1,1,1,2,1,5,2,3,1,2,1,2,1,1,2,1,1,2,2,1,4,1,1,1,1,5,10,1,2,19,2,1,2,1,2,1,2,1,2,
			1,5,1,6,3,2,1,2,2,1,1,1,4,8,5,1,1,4,1,1,3,1,2,1,5,1,2,1,1,1,10,1,1,5,2,4,6,1,4,2,2,2,12,2,1,1,6,1,1,1,4,1,1,4,6,5,1,4,2,2,4,10,7,1,1,4,2,4,
			2,1,4,3,6,10,12,5,7,2,14,2,9,1,1,6,7,10,4,7,13,1,5,4,8,4,1,1,2,28,5,6,1,1,5,2,5,20,2,2,9,8,11,2,9,17,1,8,6,8,27,4,6,9,20,11,27,6,68,2,2,1,1,
			1,2,1,2,2,7,6,11,3,3,1,1,3,1,2,1,1,1,1,1,3,1,1,8,3,4,1,5,7,2,1,4,4,8,4,2,1,2,1,1,4,5,6,3,6,2,12,3,1,3,9,2,4,3,4,1,5,3,3,1,3,7,1,5,1,1,1,1,2,
			3,4,5,2,3,2,6,1,1,2,1,7,1,7,3,4,5,15,2,2,1,5,3,22,19,2,1,1,1,1,2,5,1,1,1,6,1,1,12,8,2,9,18,22,4,1,1,5,1,16,1,2,7,10,15,1,1,6,2,4,1,2,4,1,6,
			1,1,3,2,4,1,6,4,5,1,2,1,1,2,1,10,3,1,3,2,1,9,3,2,5,7,2,19,4,3,6,1,1,1,1,1,4,3,2,1,1,1,2,5,3,1,1,1,2,2,1,1,2,1,1,2,1,3,1,1,1,3,7,1,4,1,1,2,1,
			1,2,1,2,4,4,3,8,1,1,1,2,1,3,5,1,3,1,3,4,6,2,2,14,4,6,6,11,9,1,15,3,1,28,5,2,5,5,3,1,3,4,5,4,6,14,3,2,3,5,21,2,7,20,10,1,2,19,2,4,28,28,2,3,
			2,1,14,4,1,26,28,42,12,40,3,52,79,5,14,17,3,2,2,11,3,4,6,3,1,8,2,23,4,5,8,10,4,2,7,3,5,1,1,6,3,1,2,2,2,5,28,1,1,7,7,20,5,3,29,3,17,26,1,8,4,
			27,3,6,11,23,5,3,4,6,13,24,16,6,5,10,25,35,7,3,2,3,3,14,3,6,2,6,1,4,2,3,8,2,1,1,3,3,3,4,1,1,13,2,2,4,5,2,1,14,14,1,2,2,1,4,5,2,3,1,14,3,12,
			3,17,2,16,5,1,2,1,8,9,3,19,4,2,2,4,17,25,21,20,28,75,1,10,29,103,4,1,2,1,1,4,2,4,1,2,3,24,2,2,2,1,1,2,1,3,8,1,1,1,2,1,1,3,1,1,1,6,1,5,3,1,1,
			1,3,4,1,1,5,2,1,5,6,13,9,16,1,1,1,1,3,2,3,2,4,5,2,5,2,2,3,7,13,7,2,2,1,1,1,1,2,3,3,2,1,6,4,9,2,1,14,2,14,2,1,18,3,4,14,4,11,41,15,23,15,23,
			176,1,3,4,1,1,1,1,5,3,1,2,3,7,3,1,1,2,1,2,4,4,6,2,4,1,9,7,1,10,5,8,16,29,1,1,2,2,3,1,3,5,2,4,5,4,1,1,2,2,3,3,7,1,6,10,1,17,1,44,4,6,2,1,1,6,
			5,4,2,10,1,6,9,2,8,1,24,1,2,13,7,8,8,2,1,4,1,3,1,3,3,5,2,5,10,9,4,9,12,2,1,6,1,10,1,1,7,7,4,10,8,3,1,13,4,3,1,6,1,3,5,2,1,2,17,16,5,2,16,6,
			1,4,2,1,3,3,6,8,5,11,11,1,3,3,2,4,6,10,9,5,7,4,7,4,7,1,1,4,2,1,3,6,8,7,1,6,11,5,5,3,24,9,4,2,7,13,5,1,8,82,16,61,1,1,1,4,2,2,16,10,3,8,1,1,
			6,4,2,1,3,1,1,1,4,3,8,4,2,2,1,1,1,1,1,6,3,5,1,1,4,6,9,2,1,1,1,2,1,7,2,1,6,1,5,4,4,3,1,8,1,3,3,1,3,2,2,2,2,3,1,6,1,2,1,2,1,3,7,1,8,2,1,2,1,5,
			2,5,3,5,10,1,2,1,1,3,2,5,11,3,9,3,5,1,1,5,9,1,2,1,5,7,9,9,8,1,3,3,3,6,8,2,3,2,1,1,32,6,1,2,15,9,3,7,13,1,3,10,13,2,14,1,13,10,2,1,3,10,4,15,
			2,15,15,10,1,3,9,6,9,32,25,26,47,7,3,2,3,1,6,3,4,3,2,8,5,4,1,9,4,2,2,19,10,6,2,3,8,1,2,2,4,2,1,9,4,4,4,6,4,8,9,2,3,1,1,1,1,3,5,5,1,3,8,4,6,
			2,1,4,12,1,5,3,7,13,2,5,8,1,6,1,2,5,14,6,1,5,2,4,8,15,5,1,23,6,62,2,10,1,1,8,1,2,2,10,4,2,2,9,2,1,1,3,2,3,1,5,3,3,2,1,3,8,1,1,1,11,3,1,1,4,
			3,7,1,14,1,2,3,12,5,2,5,1,6,7,5,7,14,11,1,3,1,8,9,12,2,1,11,8,4,4,2,6,10,9,13,1,1,3,1,5,1,3,2,4,4,1,18,2,3,14,11,4,29,4,2,7,1,3,13,9,2,2,5,
			3,5,20,7,16,8,5,72,34,6,4,22,12,12,28,45,36,9,7,39,9,191,1,1,1,4,11,8,4,9,2,3,22,1,1,1,1,4,17,1,7,7,1,11,31,10,2,4,8,2,3,2,1,4,2,16,4,32,2,
			3,19,13,4,9,1,5,2,14,8,1,1,3,6,19,6,5,1,16,6,2,10,8,5,1,2,3,1,5,5,1,11,6,6,1,3,3,2,6,3,8,1,1,4,10,7,5,7,7,5,8,9,2,1,3,4,1,1,3,1,3,3,2,6,16,
			1,4,6,3,1,10,6,1,3,15,2,9,2,10,25,13,9,16,6,2,2,10,11,4,3,9,1,2,6,6,5,4,30,40,1,10,7,12,14,33,6,3,6,7,3,1,3,1,11,14,4,9,5,12,11,49,18,51,31,
			140,31,2,2,1,5,1,8,1,10,1,4,4,3,24,1,10,1,3,6,6,16,3,4,5,2,1,4,2,57,10,6,22,2,22,3,7,22,6,10,11,36,18,16,33,36,2,5,5,1,1,1,4,10,1,4,13,2,7,
			5,2,9,3,4,1,7,43,3,7,3,9,14,7,9,1,11,1,1,3,7,4,18,13,1,14,1,3,6,10,73,2,2,30,6,1,11,18,19,13,22,3,46,42,37,89,7,3,16,34,2,2,3,9,1,7,1,1,1,2,
			2,4,10,7,3,10,3,9,5,28,9,2,6,13,7,3,1,3,10,2,7,2,11,3,6,21,54,85,2,1,4,2,2,1,39,3,21,2,2,5,1,1,1,4,1,1,3,4,15,1,3,2,4,4,2,3,8,2,20,1,8,7,13,
			4,1,26,6,2,9,34,4,21,52,10,4,4,1,5,12,2,11,1,7,2,30,12,44,2,30,1,1,3,6,16,9,17,39,82,2,2,24,7,1,7,3,16,9,14,44,2,1,2,1,2,3,5,2,4,1,6,7,5,3,
			2,6,1,11,5,11,2,1,18,19,8,1,3,24,29,2,1,3,5,2,2,1,13,6,5,1,46,11,3,5,1,1,5,8,2,10,6,12,6,3,7,11,2,4,16,13,2,5,1,1,2,2,5,2,28,5,2,23,10,8,4,
			4,22,39,95,38,8,14,9,5,1,13,5,4,3,13,12,11,1,9,1,27,37,2,5,4,4,63,211,95,2,2,2,1,3,5,2,1,1,2,2,1,1,1,3,2,4,1,2,1,1,5,2,2,1,1,2,3,1,3,1,1,1,
			3,1,4,2,1,3,6,1,1,3,7,15,5,3,2,5,3,9,11,4,2,22,1,6,3,8,7,1,4,28,4,16,3,3,25,4,4,27,27,1,4,1,2,2,7,1,3,5,2,28,8,2,14,1,8,6,16,25,3,3,3,14,3,
			3,1,1,2,1,4,6,3,8,4,1,1,1,2,3,6,10,6,2,3,18,3,2,5,5,4,3,1,5,2,5,4,23,7,6,12,6,4,17,11,9,5,1,1,10,5,12,1,1,11,26,33,7,3,6,1,17,7,1,5,12,1,11,
			2,4,1,8,14,17,23,1,2,1,7,8,16,11,9,6,5,2,6,4,16,2,8,14,1,11,8,9,1,1,1,9,25,4,11,19,7,2,15,2,12,8,52,7,5,19,2,16,4,36,8,1,16,8,24,26,4,6,2,9,
			5,4,36,3,28,12,25,15,37,27,17,12,59,38,5,32,127,1,2,9,17,14,4,1,2,1,1,8,11,50,4,14,2,19,16,4,17,5,4,5,26,12,45,2,23,45,104,30,12,8,3,10,2,2,
			3,3,1,4,20,7,2,9,6,15,2,20,1,3,16,4,11,15,6,134,2,5,59,1,2,2,2,1,9,17,3,26,137,10,211,59,1,2,4,1,4,1,1,1,2,6,2,3,1,1,2,3,2,3,1,3,4,4,2,3,3,
			1,4,3,1,7,2,2,3,1,2,1,3,3,3,2,2,3,2,1,3,14,6,1,3,2,9,6,15,27,9,34,145,1,1,2,1,1,1,1,2,1,1,1,1,2,2,2,3,1,2,1,1,1,2,3,5,8,3,5,2,4,1,3,2,2,2,12,
			4,1,1,1,10,4,5,1,20,4,16,1,15,9,5,12,2,9,2,5,4,2,26,19,7,1,26,4,30,12,15,42,1,6,8,172,1,1,4,2,1,1,11,2,2,4,2,1,2,1,10,8,1,2,1,4,5,1,2,5,1,8,
			4,1,3,4,2,1,6,2,1,3,4,1,2,1,1,1,1,12,5,7,2,4,3,1,1,1,3,3,6,1,2,2,3,3,3,2,1,2,12,14,11,6,6,4,12,2,8,1,7,10,1,35,7,4,13,15,4,3,23,21,28,52,5,
			26,5,6,1,7,10,2,7,53,3,2,1,1,1,2,163,532,1,10,11,1,3,3,4,8,2,8,6,2,2,23,22,4,2,2,4,2,1,3,1,3,3,5,9,8,2,1,2,8,1,10,2,12,21,20,15,105,2,3,1,1,
			3,2,3,1,1,2,5,1,4,15,11,19,1,1,1,1,5,4,5,1,1,2,5,3,5,12,1,2,5,1,11,1,1,15,9,1,4,5,3,26,8,2,1,3,1,1,15,19,2,12,1,2,5,2,7,2,19,2,20,6,26,7,5,
			2,2,7,34,21,13,70,2,128,1,1,2,1,1,2,1,1,3,2,2,2,15,1,4,1,3,4,42,10,6,1,49,85,8,1,2,1,1,4,4,2,3,6,1,5,7,4,3,211,4,1,2,1,2,5,1,2,4,2,2,6,5,6,
			10,3,4,48,100,6,2,16,296,5,27,387,2,2,3,7,16,8,5,38,15,39,21,9,10,3,7,59,13,27,21,47,5,21,6
		};

		static GalWChar baseRanges[] = // not zero-terminated
		{
			0x0020, 0x00FF, // Basic Latin + Latin Supplement
			0x2000, 0x206F, // General Punctuation
			0x3000, 0x30FF, // CJK Symbols and Punctuations, Hiragana, Katakana
			0x31F0, 0x31FF, // Katakana Phonetic Extensions
			0xFF00, 0xFFEF  // Half-width characters
		};
		static GalWChar fullRanges[GalArraySize(baseRanges) + GalArraySize(AccumulativeOffsetsFrom0X4E00) * 2 + 1] = { 0 };
		if(!fullRanges[0])
		{
			GalMemCpy(fullRanges, sizeof(baseRanges), baseRanges, sizeof(baseRanges));
			[&](int baseCodePoint) -> void
			{
				auto outRanges = fullRanges + GalArraySize(baseRanges);
				for(size_t i = 0; i < GalArraySize(AccumulativeOffsetsFrom0X4E00); ++i, outRanges += 2)
				{
					outRanges[0] = outRanges[1] = static_cast<GalWChar>(baseCodePoint + AccumulativeOffsetsFrom0X4E00[i]);
					baseCodePoint += AccumulativeOffsetsFrom0X4E00[i];
				}
				outRanges[0] = '\0';
			}(0x4E00);
		}
		return &fullRanges[0];
	}

	constexpr size_t AddCustomRectRegular(const int width, const int height)
	{
		GAL_ASSERT(width > 0 && width <= 0xFFFF);
		GAL_ASSERT(height > 0 && height <= 0xFFFF);
		const GalFontAtlasCustomRect rect =
		{
			static_cast<unsigned short>(width),
			static_cast<unsigned short>(height)
		};
		customRects.PushBack(rect);
		return customRects.Size() - 1;// Return index
	}
	constexpr size_t AddCustomFontGlyph(GalFont* font, const GalWChar id, const int width, const int height, const float advanceX, const GalVec2& offset = { 0.0f, 0.0f })
	{
		GAL_ASSERT(font != nullptr);
		GAL_ASSERT(width > 0 && width <= 0xFFFF);
		GAL_ASSERT(height > 0 && height <= 0XFFFF);
		const GalFontAtlasCustomRect rect =
		{
			static_cast<unsigned short>(width),
			static_cast<unsigned short>(height),
			id,
			advanceX,
			offset,
			font
		};
		customRects.PushBack(rect);
		return customRects.Size() - 1;// Return index
	}
	GalFontAtlasCustomRect* GetCustomRectByIndex(const int index)
	{
		GAL_ASSERT(index >= 0);
		return &customRects[index];
	}

	// [Internal]
	constexpr void CalcCustomRectUV(const GalFontAtlasCustomRect* rect, GalVec2* outUVMin, GalVec2* outUVMax) const
	{
		GAL_ASSERT(texWidth > 0 && texHeight > 0);   // Font atlas needs to be built before we can calculate UV coordinates
		GAL_ASSERT(rect->IsPacked());				// Make sure the rectangle has been packed
		*outUVMin = { static_cast<float>(rect->x) * texUVScale.x, static_cast<float>(rect->y) * texUVScale.y };
		*outUVMax = { static_cast<float>(rect->x + rect->width) * texUVScale.x, static_cast<float>(rect->y + rect->height) * texUVScale.y };
	}
	
	constexpr bool GetMouseCursorTexData(GalGuiMouseCursor cursor, GalVec2* outOffset, GalVec2* outSize, GalVec2 outUVBorder[2], GalVec2 outUVFill[2])
	{
		if(cursor <= static_cast<GalGuiMouseCursor>(EnumGalGuiMouseCursor::GALGUI_MOUSE_CURSOR_NONE) || cursor >= static_cast<GalGuiMouseCursor>(EnumGalGuiMouseCursor::GALGUI_MOUSE_CURSOR_COUNT))
		{
			return false;
		}
		if(flags & EnumGalFontAtlasFlags::GAL_FONT_ATLAS_FLAGS_NO_MOUSE_CURSORS)
		{
			return false;
		}

		GAL_ASSERT(packIDMouseCursors != -1);
		auto rect = GetCustomRectByIndex(packIDMouseCursors);
		
	}
};

struct GalFont
{
	// Members: Hot ~20/24 bytes (for CalcTextSize)
	GalVector<float> indexAdvanceX;			// 12-16 // out //            // Sparse. Glyphs->AdvanceX in a directly indexable way (cache-friendly for CalcTextSize functions which only this this info, and are often bottleneck in large UI).
	float fallbackAdvanceX;					// 4     // out // = FallbackGlyph->AdvanceX
	float fontSize;							// 4     // in  //            // Height of characters/line, set during loading (don't change after loading)

	// Members: Hot ~36/48 bytes (for CalcTextSize + render loop)
	GalVector<GalWChar> indexLookup;        // 12-16 // out //            // Sparse. Index glyphs by Unicode code-point.
	GalVector<GalFontGlyph> glyphs;         // 12-16 // out //            // All glyphs.
	const GalFontGlyph* fallbackGlyph;      // 4-8   // out // = FindGlyph(FontFallbackChar)
	GalVec2 displayOffset;					// 8     // in  // = (0,0)    // Offset font rendering by xx pixels

	// Members: Cold ~32/40 bytes
	GalFontAtlas* containerAtlas;     // 4-8   // out //            // What we has been loaded into
	const GalFontConfig* configData;         // 4-8   // in  //            // Pointer within ContainerAtlas->ConfigData
	short configDataCount;    // 2     // in  // ~ 1        // Number of ImFontConfig involved in creating this font. Bigger than 1 when merging multiple font sources into one ImFont.
	GalWChar fallbackChar;       // 2     // in  // = '?'      // Replacement character if a glyph isn't found. Only set via SetFallbackChar()
	GalWChar ellipsisChar;       // 2     // out // = -1       // Character used for ellipsis rendering.
	bool dirtyLookupTables;  // 1     // out //
	float scale;              // 4     // in  // = 1.f      // Base font scale, multiplied by the per-window font scale which you can adjust with SetWindowFontScale()
	float ascent;    // 4+4   // out //            // Ascent: distance from top to bottom of e.g. 'A' [0..FontSize]
	float descent;
	int metricsTotalSurface;// 4     // out //            // Total surface in pixels to get an idea of the font rasterization/texture cost (not exact, we approximate the cost of padding between glyphs)
	GalU8 used4kPagesMap[(GalUnicodeCodePointMax + 1) / 4096 / 8]; // 2 bytes if ImWchar=ImWchar16, 34 bytes if ImWchar==ImWchar32. Store 1-bit for each block of 4K codepoints that has one active glyph. This is mainly used to facilitate iterations across all used codepoints.	
};

constexpr GalFont* GalFontAtlas::AddFont(const GalFontConfig* config)
{
	GAL_ASSERT(!locked && "Cannot modify a locked GalFontAtlas between NewFrame() and EndFrame/Render()!");
	GAL_ASSERT(config->fontData != nullptr && config->fontDataSize > 0);
	GAL_ASSERT(config->sizePixels > 0.0f);

	if (!config->mergeMode)
	{
		fonts.PushBack(GalNew<GalFont>());
	}
	else
	{
		GAL_ASSERT(!fonts.Empty() && "Cannot use mergeMode for the first font");
	}
	configData.PushBack(*config);
	auto& newConfig = configData.Back();
	if (newConfig.destFont == nullptr)
	{
		newConfig.destFont = fonts.Back();
	}
	if (!newConfig.fontDataOwnedByAtlas)
	{
		newConfig.fontData = GalMemAlloc(newConfig.fontDataSize);
		newConfig.fontDataOwnedByAtlas = true;
		GalMemCpy(newConfig.fontData, newConfig.fontDataSize, config->fontData, config->fontDataSize);
	}

	if (newConfig.destFont->ellipsisChar == static_cast<GalWChar>(-1))
	{
		newConfig.destFont->ellipsisChar = config->ellipsisChar;
	}

	// Invalidate texture
	ClearTexData();
	return newConfig.destFont;
}

constexpr GalFont* GalFontAtlas::AddFontDefault(const GalFontConfig* configTemplate)
{
	auto config = configTemplate ? *configTemplate : GalFontConfig();
	if(!configTemplate)
	{
		config.overSampleH = 1;
		config.overSampleV = 1;
		config.pixelSnapH = true;
	}
	if(config.sizePixels <= 0.0f)
	{
		config.sizePixels = 13.0f;
	}
	if(config.name[0] == '\0')
	{
		GalFormatString(config.name, GalArraySize(config.name), "ProggyClean.ttf, %dpx", config.sizePixels);
	}
	config.ellipsisChar = 0x0085;

	const auto ttfCompressedBase85 = GetDefaultCompressedDataBase85();
	const auto glyphRanges = config.glyphRanges != nullptr ? config.glyphRanges : GetGlyphRangesDefault();
	const auto font = AddFontFromMemoryCompressedBase85TTF(ttfCompressedBase85, config.sizePixels, &config, glyphRanges);
	font->displayOffset.y = 1.0f;
	return font;
}

constexpr GalFont* GalFontAtlas::AddFontFromFileTTF(
	const char* filename, const float sizePixels,
	const GalFontConfig* configTemplate, const GalWChar* glyphRanges
)
{
	GAL_ASSERT(!locked && "Cannot modify a locked GalFontAtlas between NewFrame() and EndFrame/Render()!");
	size_t dataSize = 0;
	const auto data = GalFileLoadToMemory(filename, "rb", &dataSize, 0);
	if(!data)
	{
		GAL_ASSERT(0 && "Could not load font file!");
		return nullptr;
	}
	auto config = configTemplate ? *configTemplate : GalFontConfig();
	if(config.name[0] == '\0')
	{
		// Store a short copy of filename into the font name for convenience
		const char* p;
		for(p = filename + strlen(filename); p > filename && p[-1] != '/' && p[-1] != '\\'; --p) {}
		GalFormatString(config.name, GalArraySize(config.name), "%s, %.0fpx", p, sizePixels);
	}
	return AddFontFromMemoryTTF(data, dataSize, sizePixels, &config, glyphRanges);
}

constexpr GalFont* GalFontAtlas::AddFontFromMemoryTTF(
	void* fontData, const size_t fontSize, const float sizePixels,
	const GalFontConfig* configTemplate, const GalWChar* glyphRanges
)
{
	GAL_ASSERT(!locked && "Cannot modify a locked GalFontAtlas between NewFrame() and EndFrame/Render()!");
	auto config = configTemplate ? *configTemplate : GalFontConfig();
	GAL_ASSERT(config.fontData == nullptr);
	config.fontData = fontData;
	config.fontDataSize = fontSize;
	config.sizePixels = sizePixels;
	if(glyphRanges)
	{
		config.glyphRanges = glyphRanges;
	}

	return AddFont(&config);
}

constexpr struct GalFont* GalFontAtlas::AddFontFromMemoryCompressedTTF(
	const void* compressedFontData, size_t compressedFontSize, float sizePixels, 
	const GalFontConfig* configTemplate, const GalWChar* glyphRanges
)
{
	// 计算所需大小,代替传入参数 compressedFontSize
	auto decompressLength =
		[](const unsigned char* input) -> unsigned int
	{
		return (input[8] << 24) + (input[9] << 16) + (input[10] << 8) + input[11];
	};

	const auto bufDecompressedSize = decompressLength(static_cast<const unsigned char*>(compressedFontData));
	auto bufDecompressedData = static_cast<unsigned char*>(GalMemAlloc(bufDecompressedSize));
	
	auto result = [&]() -> unsigned int
	{
		const auto output = static_cast<unsigned char*>(bufDecompressedData);
		auto input = static_cast<const unsigned char*>(compressedFontData);

		static auto int2 =
			[](const unsigned char* src, const size_t index) -> unsigned int
		{
			return (src[index] << 8) + src[index + 1];
		};
		
		static auto int3 =
			[](const unsigned char* src, const size_t index) -> unsigned int
		{
			return (src[index] << 16) + int2(src, index + 1);
		};
		
		static auto int4 =
			[](const unsigned char* src, const size_t index) -> unsigned int
		{
			return (src[index] << 24) + int3(src, index + 1);
		};

		if(int4(input, 0) != 0x57bC0000 || int4(input, 4) != 0 /* error! stream is > 4GB */)
		{
			return 0;
		}
		
		const auto len = decompressLength(input);

		auto barrierInB = input;
		auto barrierOutE = output + len;
		auto barrierOutB = output;
		auto dOut = output;
		input += 16;

		auto decompressToken =
			[&](const unsigned char* in) -> const unsigned char*
		{
			static auto match =
				[&](const unsigned char* data, unsigned int length) -> void
			{
				GAL_ASSERT(dOut + length <= barrierOutE);
				if (dOut + length > barrierOutE)
				{
					dOut += length;
					return;
				}
				if (data < barrierOutB)
				{
					dOut = barrierOutE + 1;
					return;
				}
				while (length--)
				{
					*dOut++ = *data++;
				}
			};

			static auto lit =
				[&](const unsigned char* data, const unsigned int length) -> void
			{
				GAL_ASSERT(dOut + length <= barrierOutE);
				if (dOut + length > barrierOutE)
				{
					dOut += length;
					return;
				}
				if (data < barrierInB)
				{
					dOut = barrierOutE + 1;
					return;
				}
				// 并不修改data的数据,但是我们需要去掉const修饰
				GalMemCpy(dOut, length, const_cast<unsigned char*>(data), length);
				dOut += length;
			};

			if (in[0] >= 0x20)
			{
				// use fewer if's for cases that expand small
				if (in[0] >= 0x80)
				{
					match(dOut - in[1] - 1, in[0] - 0x80 + 1);
					in += 2;
				}
				else if (in[0] >= 0x40)
				{
					match(dOut - (int2(in, 0) - 0x4000 + 1), in[2] + 1);
					in += 3;
				}
				else
				{
					lit(in + 1, in[0] - 0x20 + 1);
					in += 1 + (in[0] - 0x20 + 1);
				}
			}
			else
			{
				// more ifs for cases that expand large, since overhead is amortized
				if (in[0] >= 0x18)
				{
					match(dOut - (int3(in, 0) - 0x180000 + 1), in[3] + 1);
					in += 4;
				}
				else if (in[0] >= 0x10)
				{
					match(dOut - (int3(in, 0) - 0x100000 + 1), int2(in, 3) + 1);
					in += 5;
				}
				else if (in[0] >= 0x08)
				{
					lit(in + 2, int2(in, 0) - 0x800 + 1);
					in += 2 + (int2(in, 0) - 0x800 + 1);
				}
				else if (in[0] == 0x07)
				{
					lit(in + 3, int2(in, 1) + 1);
					in += 3 + (int2(in, 1) + 1);
				}
				else if (in[0] == 0x06)
				{
					match(dOut - (int3(in, 1) + 1), in[4] + 1);
					in += 5;
				}
				else if (in[0] == 0x04)
				{
					match(dOut - (int3(in, 1) + 1), int2(in, 4) + 1);
					in += 6;
				}
			}

			return in;
		};

		auto adler32 =
			[](const unsigned int adler, unsigned char* buffer, unsigned int bufLen) -> unsigned int
		{
			const unsigned long adlerMod = 65521;
			unsigned long s1 = adler & 0xFFFF;
			unsigned long s2 = adler >> 16;
			unsigned long blockLen = bufLen % 5552;

			while (bufLen)
			{
				unsigned long i;
				for (i = 0; i + 7 < blockLen; i += 8)
				{
					s1 += buffer[0];
					s2 += s1;
					s1 += buffer[1];
					s2 += s1;
					s1 += buffer[2];
					s2 += s1;
					s1 += buffer[3];
					s2 += s1;
					s1 += buffer[4];
					s2 += s1;
					s1 += buffer[5];
					s2 += s1;
					s1 += buffer[6];
					s2 += s1;
					s1 += buffer[7];
					s2 += s1;

					buffer += 8;
				}

				for (; i < blockLen; ++i)
				{
					s1 += *buffer++;
					s2 += s1;
				}

				s1 %= adlerMod;
				s1 %= adlerMod;
				bufLen -= blockLen;
				blockLen = 5552;
			}
			return static_cast<unsigned int>(s2 << 16) + static_cast<unsigned int>(s1);
		};
		
		for(;;)
		{
			const auto last = input;
			input = decompressToken(input);
			if(input == last)
			{
				if(input[0] == 0x05 && input[1] == 0xfa)
				{
					GAL_ASSERT(dOut == output + len);
					if(dOut != output + len || adler32(1, output, len) != int4(input, 2))
					{
						return 0;
					}
					return len;
				}
				/* NOT REACHED */
				GAL_ASSERT(0);
				return 0;
			}
			GAL_ASSERT(dOut <= output + len);
			if(dOut > output + len)
			{
				return 0;
			}
		}
	}();

	auto config = configTemplate ? *configTemplate : GalFontConfig();
	GAL_ASSERT(config.fontData == nullptr);
	config.fontDataOwnedByAtlas = true;
	return AddFontFromMemoryTTF(bufDecompressedData, bufDecompressedSize, sizePixels, &config, glyphRanges);
}

constexpr GalFont* GalFontAtlas::AddFontFromMemoryCompressedBase85TTF(
	const char* compressedFontDataBase85, const float sizePixels, 
	const GalFontConfig* config, const GalWChar* glyphRanges
)
{
	
	const auto bufDecompressedSize = (strlen(compressedFontDataBase85) + 4) / 5 * 4;
	auto compressedTtf = GalMemAlloc(bufDecompressedSize);
	[&]()
	{
		auto src = reinterpret_cast<const unsigned char*>(compressedFontDataBase85);
		auto dest = static_cast<unsigned char*>(compressedTtf);

		auto decode85Byte = [](const unsigned char c) -> unsigned int
		{
			return c >= '\\' ? c - 36 : c - 35;
		};
		
		while(*src)
		{
			const auto tmp =
				decode85Byte(src[0]) +
					85 * (decode85Byte(src[1]) +
						85 * (decode85Byte(src[2]) +
							85 * decode85Byte(src[3]) +
								85 * decode85Byte(src[4])));
			dest[0] = (tmp >> 0) & 0xFF;
			dest[1] = (tmp >> 8) & 0xFF;
			dest[2] = (tmp >> 16) & 0xFF;
			dest[3] = (tmp >> 24) & 0xFF;
			src += 5;
			dest += 4;
		}
	}();
	const auto font = AddFontFromMemoryCompressedTTF(compressedTtf, bufDecompressedSize, sizePixels, config, glyphRanges);
	GalMemFree(compressedTtf);
	return font;
}

constexpr void GalFontAtlas::ClearInputData()
{
	GAL_ASSERT(!locked && "Cannot modify a locked GalFontAtlas between NewFrame() and EndFrame/Render()!");
	for (size_t i = 0; i < configData.Size(); ++i)
	{
		if (configData[i].fontData && configData[i].fontDataOwnedByAtlas)
		{
			GalMemFree(configData[i].fontData);
			configData[i].fontData = nullptr;
		}
	}
	for (size_t i = 0; i < fonts.Size(); ++i)
	{
		if (fonts[i]->configData >= configData.Begin() && fonts[i]->configData < configData.End())
		{
			fonts[i]->configData = nullptr;
			fonts[i]->configDataCount = 0;
		}
	}
	configData.Clear();
	customRects.Clear();
	packIDMouseCursors = -1;
	packIDLines = -1;
}

constexpr void GalFontAtlas::ClearTexData()
{
	GAL_ASSERT(!locked && "Cannot modify a locked GalFontAtlas between NewFrame() and EndFrame/Render()!");
	if(texPixelsAlpha8)
	{
		GalMemFree(texPixelsAlpha8);
	}
	if(texPixelsRGBA32)
	{
		GalMemFree(texPixelsRGBA32);
	}
	texPixelsAlpha8 = nullptr;
	texPixelsRGBA32 = nullptr;
}

constexpr void GalFontAtlas::ClearFonts()
{
	GAL_ASSERT(!locked && "Cannot modify a locked GalFontAtlas between NewFrame() and EndFrame/Render()!");
	for(size_t i = 0; i < fonts.Size(); ++i)
	{
		GalDelete(fonts[i]);
	}
	fonts.Clear();
}

constexpr void GalFontAtlas::Clear()
{
	ClearInputData();
	ClearTexData();
	ClearFonts();
}

constexpr bool GalFontAtlas::Build()
{
	GAL_ASSERT(!locked && "Cannot modify a locked GalFontAtlas between NewFrame() and EndFrame/Render()!");

	// Build
	GAL_ASSERT(configData.Size() > 0);

	// Init
	if(packIDMouseCursors < 0)
	{
		if(!(flags & EnumGalFontAtlasFlags::GAL_FONT_ATLAS_FLAGS_NO_MOUSE_CURSORS))
		{
			packIDMouseCursors = AddCustomRectRegular()
		}
	}
	
}


struct GalGuiIO
{
	GalGuiConfigFlags configFlags;														// = 0              // See ImGuiConfigFlags_ enum. Set by user/application. Gamepad/keyboard navigation options, etc.
	GalGuiBackendFlags backendFlags;													// = 0              // See ImGuiBackendFlags_ enum. Set by back-end (imgui_impl_xxx files or custom back-end) to communicate features supported by the back-end.
	GalVec2 displaySize;																// <unset>          // Main display size, in pixels.
	float deltaTime;																	// = 1.0f/60.0f     // Time elapsed since last frame, in seconds.
	float iniSavingRate;																// = 5.0f           // Minimum time between saving positions/sizes to .ini file, in seconds.
	const char* iniFilename;															// = "imgui.ini"    // Path to .ini file. Set NULL to disable automatic .ini loading/saving, if e.g. you want to manually load/save from memory.
	const char* logFilename;															// = "imgui_log.txt"// Path to .log file (default parameter to ImGui::LogToFile when no file is specified).
	float mouseDoubleClickTime;															// = 0.30f          // Time for a double-click, in seconds.
	float mouseDoubleClickMaxDist;														// = 6.0f           // Distance threshold to stay in to validate a double-click, in pixels.
	float mouseDragThreshold;															// = 6.0f           // Distance threshold before considering we are dragging.
	int keyMap[static_cast<GalGuiKey>(EnumGalGuiKey::GALGUI_KEY_COUNT)];				// <unset>          // Map of indices into the KeysDown[512] entries array which represent your "native" keyboard state.
	float keyRepeatDelay;																// = 0.250f         // When holding a key/button, time before it starts repeating, in seconds (for buttons in Repeat mode, etc.).
	float keyRepeatRate;																// = 0.050f         // When holding a key/button, rate at which it repeats, in seconds.
	void* userData;																		// = NULL           // Store your own data for retrieval by callbacks.

	GalFontAtlas
};

struct GalGuiContext
{
	bool initialized;
	bool fontAtlasOwnedByContext;
	ImGuiIO
};

namespace GalGui
{
	template <typename T>
	void GalFree(T* ptr)
	{
		if (ptr)
		{
			if (auto context = g_galGuiContext)
			{
				context
			}
		}
	}









}
