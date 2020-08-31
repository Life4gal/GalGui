#pragma once

#include "GalGui.h"

#include "GalGui_Internal_RectPack.h"
#include "GalGui_Internal_TrueType.h"

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
	GalVec4 colors[static_cast<GalGuiCol>(EnumGalGuiCol::GALGUI_COL_COUNT)]{};

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
