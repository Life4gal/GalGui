#pragma once

#include "GalGui.h"

#include "GalGui_Internal_RectPack.h"
#include "GalGui_Internal_TrueType.h"

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

enum class EnumGalGuiDir
{
	GALGUI_DIR_NONE = -1,
	GALGUI_DIR_LEFT = 0,
	GALGUI_DIR_RIGHT = 1,
	GALGUI_DIR_UP = 2,
	GALGUI_DIR_DOWN = 3,
	GALGUI_DIR_COUNT
};

struct GalGuiStyle
{
	float alpha;
	GalVec2 windowPadding{};
	float windowRounding;
	float windowBorderSize;
	GalVec2 windowMinSize{};
	GalVec2 windowTitleAlign{};
	GalGuiDir windowMenuButtonPosition;
	float childRounding;
	float childBorderSize;
	float popupRounding;
	float popupBorderSize;
	GalVec2 framePadding{};
	float frameRounding;
	float frameBorderSize;
	GalVec2 itemSpacing{};
	GalVec2 itemInnerSpacing{};
	GalVec2 touchExtraPadding{};
	float indentSpacing;
	float columnsMinSpacing;
	float scrollbarSize;
	float scrollbarRounding;
	float grabMinSize;
	float grabRounding;
	float tabRounding;
	float tabBorderSize;
	float tabMinWidthForUnselectedCloseButton;
	GalGuiDir colorButtonPosition;
	GalVec2 buttonTextAlign{};
	GalVec2 selectableTextAlign{};
	GalVec2 displayWindowPadding{};
	GalVec2 displaySafeAreaPadding{};
	float mouseCursorScale;
	bool antiAliasedLine;
	bool antiAliasedLineUseTex;
	bool antiAliasedFill;
	float curveTessellationTol;
	float circleSegmentMaxError;
	GalVec4 colors[static_cast<size_t>(EnumGalGuiCol::GALGUI_COL_COUNT)]{};

	GalGuiStyle()
	{
		alpha = 1.0f;
		windowPadding = { 8, 8 };
		windowRounding = 7.0f;
		windowBorderSize = 1.0f;
		windowMinSize = { 32, 32 };
		windowTitleAlign = { 0.0f, 0.5f };
		windowMenuButtonPosition = static_cast<GalGuiDir>(EnumGalGuiDir::GALGUI_DIR_LEFT);
		childRounding = 0.0f;
		childBorderSize = 1.0f;
		popupRounding = 0.0f;
		popupBorderSize = 1.0f;
		framePadding = { 4, 3 };
		frameRounding = 0.0f;
		frameBorderSize = 0.0f;
		itemSpacing = { 8, 4 };
		itemInnerSpacing = { 4, 4 };
		touchExtraPadding = { 0, 0 };
		indentSpacing = 21.0f;
		columnsMinSpacing = 6.0f;
		scrollbarSize = 14.0f;
		scrollbarRounding = 9.0f;
		grabMinSize = 10.0f;
		grabRounding = 0.0f;
		tabRounding = 4.0f;
		tabBorderSize = 0.0f;
		tabMinWidthForUnselectedCloseButton = 0.0f;
		colorButtonPosition = static_cast<GalGuiDir>(EnumGalGuiDir::GALGUI_DIR_RIGHT);
		buttonTextAlign = { 0.5f, 0.5f };
		selectableTextAlign = { 0.0f,0.0f };
		displayWindowPadding = { 19,19 };
		displaySafeAreaPadding = { 3,3 };
		mouseCursorScale = 1.0f;
		antiAliasedLine = true;
		antiAliasedLineUseTex = true;
		antiAliasedFill = true;
		curveTessellationTol = 1.25f;
		circleSegmentMaxError = 1.6f;

		// Default theme
		GalGui::StyleColorsDark(*this);
	}

	void ScaleAllSizes(float scale)
	{
		floorf()
	}
};


namespace GalGui
{

void StyleColorsDark(GalGuiStyle& style)
{
	auto colors = style.Colors
}






	


	
}
