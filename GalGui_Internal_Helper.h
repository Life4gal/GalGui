#pragma once

#include <algorithm>

#include <cassert>
#include <cfloat>
#include <cstdarg>
#include <cstddef>
#include <cstring>

struct GalBitVector;                 // Store 1-bit per value
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

using GalGuiCol = int; 	// -> enum GalGuiCol_             // Enum: A color identifier for styling
enum class EnumGalGuiCol
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

using GalGuiCond = int; 	// -> enum GalGuiCond_            // Enum: A condition for many Set*() functions
enum class EnumGalGuiCond
{
	GALGUI_COND_NONE = 0,
	GALGUI_COND_ALWAYS = 1 << 0,
	GALGUI_COND_ONCE = 1 << 1,
	GALGUI_COND_FIRST_USE_EVER = 1 << 2,
	GALGUI_COND_APPEARING = 1 << 3
};

using GalGuiDataType = int; 	// -> enum GalGuiDataType_        // Enum: A primary data type

using GalS8 = signed char;
using GalU8 = unsigned char;
using GalS16 = signed short;
using GalU16 = unsigned short;
using GalS32 = signed int;
using GalU32 = unsigned int;
using GalS64 = signed   __int64;
using GalU64 = unsigned __int64;
enum class EnumGalGuiDataType
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

using GalGuiDir = int;	// -> enum GalGuiDir_             // Enum: A cardinal direction
enum class EnumGalGuiDir
{
	GALGUI_DIR_NONE = -1,
	GALGUI_DIR_LEFT = 0,
	GALGUI_DIR_RIGHT = 1,
	GALGUI_DIR_UP = 2,
	GALGUI_DIR_DOWN = 3,
	GALGUI_DIR_COUNT
};

using GalGuiKey = int;	// -> enum GalGuiKey_             // Enum: A key identifier (ImGui-side enum)
enum class EnumGalGuiKey
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

using GalGuiNavInput = int;	// -> enum GalGuiNavInput_        // Enum: An input identifier for navigation
enum class EnumGalGuiNavInput
{
	// TODO 不需要
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

using GalGuiMouseButton = int;// -> enum GalGuiMouseButton_     // Enum: A mouse button identifier (0=left, 1=right, 2=middle)
enum class EnumGalGuiMouseButton
{
	GALGUI_MOUSE_BUTTON_LEFT = 0,
	GALGUI_MOUSE_BUTTON_RIGHT = 1,
	GALGUI_MOUSE_BUTTON_MIDDLE = 2,
	GALGUI_MOUSE_BUTTON_X1 = 3,
	GALGUI_MOUSE_BUTTON_X2 = 4,
	GALGUI_MOUSE_BUTTON_COUNT = 5
};

using GalGuiMouseCursor = int;// -> enum GalGuiMouseCursor_     // Enum: A mouse cursor identifier
enum class EnumGalGuiMouseCursor
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

using GalGuiStyleVar = int;// -> enum GalGuiStyleVar_        // Enum: A variable identifier for styling
enum class EnumGalGuiStyleVar
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

using GalDrawCornerFlags = int;	// -> enum GalDrawCornerFlags_    // Flags: for ImDrawList::AddRect(), AddRectFilled() etc.
using GalDrawListFlags = int;// -> enum GalDrawListFlags_      // Flags: for ImDrawList
using GalFontAtlasFlags = int;	// -> enum GalFontAtlasFlags_     // Flags: for ImFontAtlas build

using GalGuiBackendFlags = int;	// -> enum GalGuiBackendFlags_    // Flags: for io.BackendFlags
enum class EnumGalGuiBackendFlags
{
	GALGUI_BACKEND_FLAGS_NONE = 0,
	GALGUI_BACKEND_FLAGS_HAS_GAMEPAD = 1 << 0,
	GALGUI_BACKEND_FLAGS_HAS_MOUSE_CURSORS = 1 << 1,
	GALGUI_BACKEND_FLAGS_HAS_SET_MOUSE_POS = 1 << 2,
	GALGUI_BACKEND_FLAGS_RENDERER_HAS_VERTEX_OFFSET = 1 << 3
};

using GalGuiColorEditFlags = int;// -> enum GalGuiColorEditFlags_  // Flags: for ColorEdit4(), ColorPicker4() etc.
enum class EnumGalGuiColorEditFlags
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

using GalGuiConfigFlags = int;	// -> enum GalGuiConfigFlags_     // Flags: for io.ConfigFlags
enum class EnumGalGuiConfigFlags
{
	GALGUI_CONFIG_FLAGS_NONE = 0,
	GALGUI_CONFIG_FLAGS_NAV_ENABLE_KEYBOARD = 1 << 0,
	GALGUI_CONFIG_FLAGS_NAV_ENABLE_GAMEPAD = 1 << 1,
	GALGUI_CONFIG_FLAGS_NAV_ENABLE_SET_MOUSE_POS = 1 << 2,
	GALGUI_CONFIG_FLAGS_NAV_NO_CAPTURE_KEYBOARD = 1 << 3,
	GALGUI_CONFIG_FLAGS_NO_MOUSE = 1 << 4,
	GALGUI_CONFIG_FLAGS_NO_MOUSE_CURSOR_CHANGE = 1 << 5,
};

using GalGuiComboFlags = int;// -> enum GalGuiComboFlags_      // Flags: for BeginCombo()
enum class EnumGalGuiComboFlags
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

using GalGuiDragDropFlags = int;// -> enum GalGuiDragDropFlags_   // Flags: for BeginDragDropSource(), AcceptDragDropPayload()
enum class EnumGalGuiDragDropFlags
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

using GalGuiFocusedFlags = int;// -> enum GalGuiFocusedFlags_    // Flags: for IsWindowFocused()
enum class EnumGalGuiFocusedFlags
{
	GALGUI_FOCUSED_FLAGS_NONE = 0,
	GALGUI_FOCUSED_FLAGS_CHILD_WINDOWS = 1 << 0,
	GALGUI_FOCUSED_FLAGS_ROOT_WINDOW = 1 << 1,
	GALGUI_FOCUSED_FLAGS_ANY_WINDOW = 1 << 2,
	GALGUI_FOCUSED_FLAGS_ROOT_AND_CHILD_WINDOWS =
		GALGUI_FOCUSED_FLAGS_CHILD_WINDOWS |
		GALGUI_FOCUSED_FLAGS_ROOT_WINDOW
};

using GalGuiHoveredFlags = int;// -> enum GalGuiHoveredFlags_    // Flags: for IsItemHovered(), IsWindowHovered() etc.
enum class EnumGalGuiHoveredFlags
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

using GalGuiInputTextFlags = int;// -> enum GalGuiInputTextFlags_  // Flags: for InputText(), InputTextMultiline()

using GalGuiKeyModFlags = int;// -> enum GalGuiKeyModFlags_     // Flags: for io.KeyMods (Ctrl/Shift/Alt/Super)
enum class EnumGalGuiKeyModeFlags
{
	GALGUI_KEY_MOD_FLAGS_NONE = 0,
	GALGUI_KEY_MOD_FLAGS_CTRL = 1 << 0,
	GALGUI_KEY_MOD_FLAGS_SHIFT = 1 << 1,
	GALGUI_KEY_MOD_FLAGS_ALT = 1 << 2,
	GALGUI_KEY_MOD_FLAGS_SUPER = 1 << 3
};

using GalGuiPopupFlags = int;// -> enum GalGuiPopupFlags_      // Flags: for OpenPopup*(), BeginPopupContext*(), IsPopupOpen()
enum class EnumGalGuiPopupFlags
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

using GalGuiSelectableFlags = int;// -> enum GalGuiSelectableFlags_ // Flags: for Selectable()
enum class EnumGalGuiSelectableFlags
{
	GALGUI_SELECTABLE_FLAGS_NONE = 0,
	GALGUI_SELECTABLE_FLAGS_DOT_NO_CLOSE_POPUP = 1 << 0,
	GALGUI_SELECTABLE_FLAGS_SPAN_ALL_COLUMNS = 1 << 1,
	GALGUI_SELECTABLE_FLAGS_ALLOW_DOUBLE_CLICK = 1 << 2,
	GALGUI_SELECTABLE_FLAGS_DISABLE = 1 << 3,
	GALGUI_SELECTABLE_FLAGS_ALLOW_ITEM_OVERLAP = 1 << 4
};

using GalGuiTabBarFlags = int;// -> enum GalGuiTabBarFlags_     // Flags: for BeginTabBar()
enum class EnumGalGuiTabBarFlags
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

using GalGuiTabItemFlags = int;// -> enum GalGuiTabItemFlags_    // Flags: for BeginTabItem()
enum class EnumGalGuiTabItemFlags
{
	GALGUI_TAB_ITEM_FLAGS_NONE = 0,
	GALGUI_TAB_ITEM_FLAGS_UNSAVED_DOCUMENT = 1 << 0,
	GALGUI_TAB_ITEM_FLAGS_SET_SELECTED = 1 << 1,
	GALGUI_TAB_ITEM_FLAGS_NO_CLOSE_WITH_MIDDLE_MOUSE_BUTTON = 1 << 2,
	GALGUI_TAB_ITEM_FLAGS_NO_PUSH_ID = 1 << 3,
	GALGUI_TAB_ITEM_FLAGS_NO_TOOLTIP = 1 << 4
};

using GalGuiTreeNodeFlags = int;// -> enum GalGuiTreeNodeFlags_   // Flags: for TreeNode(), TreeNodeEx(), CollapsingHeader()
enum class EnumGalGuiTreeNodeFlags
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

using GalGuiWindowFlags = int;	// -> enum GalGuiWindowFlags_     // Flags: for Begin(), BeginChild()
using GalTextureID = void*;
using GalGuiID = unsigned int;// 一个独有的 ID ,通常来自字符串的哈希值


// 函数指针 GalGuiInputTextCallback
//typedef int (*GalGuiInputTextCallback)(GalGuiInputTextCallbackData* data);
// 函数指针 GalGuiSizeCallback
//typedef void (*GalGuiSizeCallback)(GalGuiSizeCallbackData* data);



using GalGuiLayoutType = int;
using GalGuiButtonFlags = int;
using GalGuiColumnsFlags = int;
using GalGuiDragFlags = int;
using GalGuiItemFlags = int;
using GalGuiItemStatusFlags = int;
using GalGuiNavHighlightFlags = int;
using GalGuiNavDirSourceFlags = int;
using GalGuiNavMoveFlags = int;
using GalGuiNextItemDataFlags = int;
using GalGuiNextWindowDataFlags = int;
using GalGuiSeparatorFlags = int;
using GalGuiSliderFlags = int;
using GalGuiTextFlags = int;
using GalGuiTooltipFlags = int;



// 字符解码
using GalWChar16 = unsigned short;
using GalWChar32 = unsigned int;
using GalWChar = GalWChar16;

constexpr auto GalUnicodeCodePointInvalid = 0xFFFD;
constexpr auto GalUnicodeCodePointMax = 0xFFFF;






template <typename Exp>
constexpr void GalAssert(Exp exp)
{
	assert(exp);
}

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

// Helpers: Hashing

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
constexpr const char* GalParseFormatTrimDecoration(const char* str, char* dest, size_t length)
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
constexpr int GalTextCharFromUtf8(unsigned int* out, const unsigned char* text, const unsigned char* textEnd)
{
	if(!(text[0] & 0x80))
	{
		*out = static_cast<unsigned int>(*text);
		return 1;
	}
	if((text[0] & 0xe0) == 0xc0)
	{
		*out = GalUnicodeCodePointInvalid;
		if(textEnd && reinterpret_cast<const char*>(textEnd) - reinterpret_cast<const char*>(text) < 2)
		{
			return 1;
		}
		if(text[0] & 0xc2)
		{
			return 2;
		}
		unsigned int c = (*text++ & 0x1f) << 6;
		if((text[0] & 0xc0) != 0x80)
		{
			return 2;
		}
		c += (*text & 0x3f);
		*out = c;
		return 2;
	}
	if((text[0] & 0xf0) == 0xe0)
	{
		*out = GalUnicodeCodePointInvalid;
		if(textEnd && reinterpret_cast<const char*>(textEnd) - reinterpret_cast<const char*>(text) < 3)
		{
			return 1;
		}
		if(text[0] == 0xe0 && (text[1] < 0xa0 || text[1] > 0xbf))
		{
			return 3;
		}
		if(text[0] == 0xed && text[1] > 0x9f)
		{
			return 3;
		}
		unsigned int c = (*text++ & 0x0f) << 12;
		if((text[0] & 0xc0) != 0x80)
		{
			return 3;
		}
		c += (*text++ & 0x3f) << 6;
		if((text[0] & 0xc0) != 0x80)
		{
			return 3;
		}
		c += (*text & 0x3f);
		*out = c;
		return 3;
	}
	if((text[0] & 0xf8) == 0xf0)
	{
		*out = GalUnicodeCodePointInvalid;
		if (textEnd && reinterpret_cast<const char*>(textEnd) - reinterpret_cast<const char*>(text) < 4)
		{
			return 1;
		}
		if(text[0] > 0xf4)
		{
			return 4;
		}
		if(text[0] == 0xf0 && (text[1] < 0x90 || text[1] > 0xbf))
		{
			return 4;
		}
		if(text[0] == 0xf4 && text[1] > 0x8f)
		{
			return 4;
		}
		unsigned int c = (*text++ & 0x07) << 18;
		if((text[0] & 0xc0) != 0x80)
		{
			return 4;
		}
		c += (*text++ & 0x3f) << 12;
		if((text[0] & 0xc0) != 0x80)
		{
			return 4;
		}
		c += (*text++ & 0x3f) << 6;
		if((text[0] & 0xc0) != 0x80)
		{
			return 4;
		}
		c += (*text & 0x3f);
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
	GalWChar* dest, size_t length, 
	const char* text, const char* textEnd, 
	const char** textRemaining)
{
	auto it = dest;
	const auto end = dest + length;
	while(it < end - 1 && (!textEnd || text < textEnd) && *text)
	{
		unsigned int c = -1;
		text += GalTextCharFromUtf8(&c, reinterpret_cast<const unsigned char*>(text), reinterpret_cast<const unsigned char*>(textEnd));
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
		text += GalTextCharFromUtf8(&c, reinterpret_cast<const unsigned char*>(text), reinterpret_cast<const unsigned char*>(textEnd));
		if(c == 0)
		{
			break;
		}
		++count;
	}

	return count;
}

constexpr int GalTextCountUtf8BytesFromChar(const char* text, const char* textEnd)
{
	unsigned int dummy = 0;
	return GalTextCharFromUtf8(&dummy, reinterpret_cast<const unsigned char*>(text), reinterpret_cast<const unsigned char*>(textEnd));
}

static constexpr int GalTextCountUtf8BytesFromChar(unsigned int c)
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
};

static constexpr GalVec2 operator+(const GalVec2& vec, const float scale)
{
	return { vec.x + scale, vec.y + scale };
}

static constexpr GalVec2 operator-(const GalVec2& vec, const float scale)
{
	return { vec.x - scale, vec.y - scale };
}

static constexpr GalVec2 operator*(const GalVec2& vec, const float scale)
{
	return { vec.x * scale, vec.y * scale };
}

static constexpr GalVec2 operator/(const GalVec2& vec, const float scale)
{
	return { vec.x / scale, vec.y / scale };
}

static constexpr GalVec2 operator+(const GalVec2& lhs, const GalVec2& rhs)
{
	return { lhs.x + rhs.x, lhs.y + rhs.y };
}

static constexpr GalVec2 operator-(const GalVec2& lhs, const GalVec2& rhs)
{
	return { lhs.x - rhs.x, lhs.y - rhs.y };
}

static constexpr GalVec2 operator*(const GalVec2& lhs, const GalVec2& rhs)
{
	return { lhs.x * rhs.x, lhs.y * rhs.y };
}

static constexpr GalVec2 operator/(const GalVec2& lhs, const GalVec2& rhs)
{
	return { lhs.x / rhs.x, lhs.y / rhs.y };
}

static constexpr GalVec2& operator+=(GalVec2& vec, const float scale)
{
	vec.x += scale;
	vec.y += scale;
	return vec;
}

static constexpr GalVec2& operator-=(GalVec2& vec, const float scale)
{
	vec.x -= scale;
	vec.y -= scale;
	return vec;
}

static constexpr GalVec2& operator*=(GalVec2& vec, const float scale)
{
	vec.x *= scale;
	vec.y *= scale;
	return vec;
}

static constexpr GalVec2& operator/=(GalVec2& vec, const float scale)
{
	vec.x /= scale;
	vec.y /= scale;
	return vec;
}

static constexpr GalVec2& operator+=(GalVec2& lhs, const GalVec2& rhs)
{
	lhs.x += rhs.x;
	lhs.y += rhs.x;
	return lhs;
}

static constexpr GalVec2& operator-=(GalVec2& lhs, const GalVec2& rhs)
{
	lhs.x -= rhs.x;
	lhs.y -= rhs.x;
	return lhs;
}

static constexpr GalVec2& operator*=(GalVec2& lhs, const GalVec2& rhs)
{
	lhs.x *= rhs.x;
	lhs.y *= rhs.x;
	return lhs;
}

static constexpr GalVec2& operator/=(GalVec2& lhs, const GalVec2& rhs)
{
	lhs.x /= rhs.x;
	lhs.y /= rhs.x;
	return lhs;
}

static constexpr GalVec4 operator+(const GalVec4& lhs, const GalVec4& rhs)
{
	return { lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w };
}

static constexpr GalVec4 operator-(const GalVec4& lhs, const GalVec4& rhs)
{
	return { lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w };
}

static constexpr GalVec4 operator*(const GalVec4& lhs, const GalVec4& rhs)
{
	return { lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z, lhs.w * rhs.w };
}

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

constexpr GalVec2 GalMul(const GalVec2& lhs, const GalVec2& rhs)
{
	return { lhs.x * rhs.x, lhs.y * rhs.y };
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
	GalAssert(numSegments > 0);
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
	GalAssert(tessTol > 0.0f);
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
	return std::abs(a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y)) * 0.5f;
}

constexpr GalGuiDir GalGetDirQuadrantFromDelta(float dx, float dy)
{
	if(std::abs(dx) > std::abs(dy))
	{
		return dx > 0.0f > : Enum
	}
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
	
	constexpr GalVec2 GetCenter() const
	{
		return { (min.x + max.x) * 0.5f, (min.y + max.y) * 0.5f };
	}

	constexpr GalVec2 GetSize() const
	{
		return { max.x - min.x, max.y - min.y };
	}

	constexpr float GetWidth() const
	{
		return max.x - min.x;
	}

	constexpr float GetHeight() const
	{
		return max.y - min.y;
	}

	constexpr GalVec2 GetTopLeft() const
	{
		return min;
	}

	constexpr GalVec2 GetTopRight() const
	{
		return { max.x, min.y };
	}

	constexpr GalVec2 GetBottomLeft() const
	{
		return { min.x, max.y };
	}

	constexpr GalVec2 GetBottomRight() const
	{
		return max;
	}

	constexpr bool Contains(const GalVec2& point) const
	{
		return point.x >= min.x && point.y >= min.y && point.x < max.x && point.y < max.y;
	}
	
	constexpr bool Contains(const GalRect& rect) const
	{
		return rect.min.x >= min.x && rect.min.y >= min.y && rect.max.x <= max.x && rect.max.y <= max.y;
	}
	
	constexpr bool Overlaps(const GalRect& rect) const
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
		min = std::max(min, rect.min);
		max = std::min(max, rect.max);
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

	constexpr bool IsInverted() const
	{
		return min.x > max.x || min.y > max.y;
	}
};

/* =====================================================
 *		GalVector & GalBitVector
 *		GalVector & GalBitVector
 *		GalVector & GalBitVector
 *		GalVector & GalBitVector
 *		GalVector & GalBitVector
 *		GalVector & GalBitVector
 *		GalVector & GalBitVector
 *		GalVector & GalBitVector
 *		GalVector & GalBitVector
 *		GalVector & GalBitVector
 *		GalVector & GalBitVector
 *		GalVector & GalBitVector
 *		GalVector & GalBitVector
 *		GalVector & GalBitVector
 *		GalVector & GalBitVector
 *		GalVector & GalBitVector
 *		GalVector & GalBitVector
 *		GalVector & GalBitVector
 *		GalVector & GalBitVector
 * ======================================================
 */

template <typename T>
struct GalVector
{
	size_t size;
	size_t capacity;
	T* data;

	GalVector() : size(0), capacity(0), data(nullptr) {}
	
	GalVector(const GalVector<T>& src) : size(0), capacity(0), data(nullptr)
	{
		this->operator=(src);
	}

	GalVector<T>& operator=(const GalVector<T>& src)
	{
		if(data == src.data && size == src.size && capacity == src.capacity)
		{
			return *this;
		}
		
		this->Clear();
		this->Resize(src.size);
		::GalMemCpy(data, size * sizeof(T), src.data, size * sizeof(T));
		return *this;
	}

	GalVector(GalVector&& other) noexcept = default;
	GalVector& operator=(GalVector&& other) noexcept = default;

	~GalVector()
	{
		if(data)
		{
			GalGui::Free(data);
		}
	}

	T& operator[](size_t index)
	{
		GalAssert(index < size);
		return data[index];
	}

	const T& operator[](size_t index) const
	{
		GalAssert(index < size);
		return data[index];
	}
	
	constexpr bool Empty() const
	{
		return size == 0;
	}

	constexpr size_t Size() const
	{
		return size;
	}

	constexpr size_t SizeInBytes() const
	{
		return size * sizeof(T);
	}

	constexpr size_t Capacity() const
	{
		return capacity;
	}

	constexpr void Clear()
	{
		if(data)
		{
			size = 0;
			capacity = 0;
			GalGui::Free(data);
			data = nullptr;
		}
	}

	constexpr T* Begin()
	{
		return data;
	}

	constexpr const T* Begin() const
	{
		return data;
	}

	constexpr T* End()
	{
		return data + size;
	}

	constexpr const T* End() const
	{
		return data + size;
	}

	constexpr T& Front()
	{
		GalAssert(size > 0);
		return data[0];
	}

	constexpr const T& Front() const
	{
		GalAssert(size > 0);
		return data[0];
	}

	constexpr T& Back()
	{
		GalAssert(size > 0);
		return data[size - 1];
	}

	constexpr const T& Back() const
	{
		GalAssert(size > 0);
		return data[size - 1];
	}

	constexpr void Swap(GalVector<T>& rhs)
	{
		using std::swap;
		swap(size, rhs.size);
		swap(capacity, rhs.capacity);
		swap(data, rhs.data);
	}

	constexpr void Reserve(size_t newCapacity)
	{
		if (newCapacity <= capacity) return;
		auto newData = GalGui::Alloc(newCapacity * sizeof(T));
		if (data)
		{
			::GalMemCpy(newData, size * sizeof(T), data, size * sizeof(T));
			GalGui::Free(data);
		}
		data = newData;
		capacity = newCapacity;
	}
	
	constexpr size_t GrowCapacity(size_t newSize) const
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
		GalAssert(newSize <= size);
		size = newSize;
	}

	constexpr void PushBack(const T& v)
	{
		if(size == capacity)
		{
			this->Reserve(this->GrowCapacity(size + 1));
		}
		::GalMemCpy(&data[size], sizeof(v), &v, sizeof(v));
		++size;
	}

	constexpr void PopBack()
	{
		GalAssert(size > 0);
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
		::GalAssert(it >= data && it < data + size);
		const std::ptrdiff_t off = it - data;
		const auto s = (size - off - 1) * sizeof(T);
		::GalMemMove(data + off, s, data + off + 1, s);
		--size;
		return data + off;
	}

	constexpr T* Erase(const T* it, const T* last)
	{
		::GalAssert(it >= data && it < data + size && last > it && last <= data + size);
		const std::ptrdiff_t count = last - it;
		const std::ptrdiff_t off = it - data;
		const auto s = (size - off - count) * sizeof(T);
		::GalMemMove(data + off, s, data + off + count, s);
		size -= count;
		return data + off;
	}

	constexpr T* EraseUnsorted(const T* it)
	{
		::GalAssert(it >= data && it < data + size);
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
		::GalAssert(it >= data && it <= data + size);
		const std::ptrdiff_t off = it - data;
		if(size == capacity)
		{
			this->Reserve(this->GrowCapacity(size + 1));
		}
		if(off < size)
		{
			const auto s = (size - off) * sizeof(T);
			::GalMemMove(data + off + 1, s, data + off, s);
		}
		::GalMemCpy(&data[off], sizeof(v), &v, sizeof(v));
		++size;
		return data + off;
	}

	constexpr bool Contains(const T& v) const
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
		::GalAssert(it >= data && it < data + size);
		const std::ptrdiff_t off = it - data;
		return off;
	}
};

constexpr bool GalBitArrayTestBit(const GalU32* arr, const int n)
{
	const GalU32 mask = 1 << (n & 31);
	return (arr[n >> 5] & mask) != 0;
}

constexpr void GalBitArrayClearBit(GalU32* arr, const int n)
{
	const GalU32 mask = 1 << (n & 31);
	arr[n >> 5] &= ~mask;
}

constexpr void GalBitArraySetBit(GalU32* arr, const int n)
{
	const GalU32 mask = 1 << (n & 31);
	arr[n >> 5] |= mask;
}

constexpr void GalBitArrayBitRange(GalU32* arr, int n, const int n2)
{
	while(n <= n2)
	{
		const auto aMod = n & 31;
		const auto bMod = ((n2 >= n + 31) ? 31 : (n2 & 31)) + 1;
		const auto mask = 
			static_cast<GalU32>((static_cast<GalU64>(1) << bMod) - 1)
			& 
			~static_cast<GalU32>((static_cast<GalU64>(1) << aMod) - 1);
		arr[n >> 5] |= mask;
		n = (n + 32) & ~31;
	}
}

struct GalBitVec
{
	GalVector<GalU32> storage;

	constexpr void Create(const size_t size)
	{
		storage.Resize((size + 31) >> 5);
		GalMemSet(storage.data, 0, storage.size * sizeof(storage.data[0]));
	}

	constexpr void Clear()
	{
		storage.Clear();
	}

	constexpr bool TestBit(const int n) const
	{
		GalAssert(n < (storage.size << 5));
		return GalBitArrayTestBit(storage.data, n);
	}

	constexpr void SetBit(const int n) const
	{
		GalAssert(n < (storage.size << 5));
		GalBitArraySetBit(storage.data, n);
	}

	constexpr void ClearBit(const int n) const
	{
		GalAssert(n < (storage.size << 5));
		GalBitArrayClearBit(storage.data, n);
	}
};























extern GalGuiContext* g_galGuiContext;













struct GalFontConfig;
struct GalFontGlyph;
struct GalFontAtlasCustomRect;
struct GalFontGlyphRangesBuilder;
struct GalFontAtlas;
struct GalFont;

enum class EnumGalFontAtlas
{
	GALGUI_FONT_ATLAS_NONE = 0,
	GALGUI_FONT_ATLAS_NO_POWER_OF_TWO_HEIGHT = 1 << 0,
	GALGUI_FONT_ATLAS_NO_MOUSE_CURSORS = 1 << 1,
	GALGUI_FONT_ATLAS_NO_BACKED_LINES = 1 << 2
};

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

	constexpr bool IsPacked() const
	{
		return x != 0xffff;
	}
};

struct GalFontGlyphRangesBuilder
{
	GalVector<GalU32> usedChars;

	constexpr void Clear()
	{
		auto sizeInBytes = (GalUnicodeCodePointMax + 1) / 8;
		usedChars.resize(sizeInBytes / sizeof(GalU32));
		memset(usedChars.data, 0, sizeInBytes);
	}
};

struct GalFontAtlas
{
	bool locked;				 // Marked as Locked by ImGui::NewFrame() so attempt to modify the atlas will assert.
	GalFontAtlasFlags flags;	 // Build flags (see ImFontAtlasFlags_)
	GalTextureID texID;			 // User data to refer to the texture once it has been uploaded to user's graphic systems. It is passed back to you during rendering via the ImDrawCmd structure.
	int texDesireWidth;			 // Texture width desired by user before Build(). Must be a power-of-two. If have many glyphs your graphics API have texture size restrictions you may want to increase texture width to decrease height.
	int texGlyphPadding;		 // Padding between glyphs within texture in pixels. Defaults to 1. If your rendering method doesn't rely on bilinear filtering you may set this to 0.

	// [Internal]
	unsigned char* texPixelsAlpha8;
	unsigned int* texPixelsRGBA32;
	int texWidth;
	int texHeight;
	GalVec2 texUVScale;
	GalVec2 texUVWhitePixel;
	GalVector<GalFont> fonts;
	GalVector<GalFib>
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
	GalFontAtlas* ContainerAtlas;     // 4-8   // out //            // What we has been loaded into
	const GalFontConfig* ConfigData;         // 4-8   // in  //            // Pointer within ContainerAtlas->ConfigData
	short                       ConfigDataCount;    // 2     // in  // ~ 1        // Number of ImFontConfig involved in creating this font. Bigger than 1 when merging multiple font sources into one ImFont.
	GalWChar                     FallbackChar;       // 2     // in  // = '?'      // Replacement character if a glyph isn't found. Only set via SetFallbackChar()
	GalWChar                   EllipsisChar;       // 2     // out // = -1       // Character used for ellipsis rendering.
	bool                        DirtyLookupTables;  // 1     // out //
	float                       Scale;              // 4     // in  // = 1.f      // Base font scale, multiplied by the per-window font scale which you can adjust with SetWindowFontScale()
	float                       Ascent, Descent;    // 4+4   // out //            // Ascent: distance from top to bottom of e.g. 'A' [0..FontSize]
	int                         MetricsTotalSurface;// 4     // out //            // Total surface in pixels to get an idea of the font rasterization/texture cost (not exact, we approximate the cost of padding between glyphs)
	GalU8                        Used4kPagesMap[(IM_UNICODE_CODEPOINT_MAX + 1) / 4096 / 8]; // 2 bytes if ImWchar=ImWchar16, 34 bytes if ImWchar==ImWchar32. Store 1-bit for each block of 4K codepoints that has one active glyph. This is mainly used to facilitate iterations across all used codepoints.


	
};


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
