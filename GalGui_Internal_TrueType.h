#pragma once

using GalS8 = signed char;
using GalU8 = unsigned char;
using GalS16 = signed short;
using GalU16 = unsigned short;
using GalS32 = signed int;
using GalU32 = unsigned int;
using GalS64 = signed   __int64;
using GalU64 = unsigned __int64;

struct GalttBuffer
{
	unsigned char* data;
	int cursor;
	int size;
};

struct GalttBackedChar
{
	// coordinates of bbox in bitmap
	unsigned short x0;
	unsigned short y0;
	unsigned short x1;
	unsigned short y1;

	float xOffset;
	float yOffset;
	float xAdvance;
};

static int Galtt_BakeFontBitmap(
	const unsigned char* data, int offset,		// 字体的位置(普通的 .ttf 文件 offset == 0)
	float pixelHeight,							// 字体的高度(像素)
	unsigned char* pixels, int pw, int ph,		// 要填充的 bitmap
	int firstChar, int numChars,				// 烘培的字符
	GalttBackedChar* charData					// 目的地, num_chars 大小
);             

struct GalttAlignedQuad
{
	// top-left
	float x0;
	float y0;
	float s0;
	float t0;
	// bottom-right
	float x1;
	float y1;
	float s1;
	float t1;
};

static void GalttGetBakedQuad(
	const GalttBackedChar* charData,
	int pw, int ph,
	int charIndex,								// character to display
	float* xPos, float* yPos,					// pointers to current position in screen pixel space
	GalttAlignedQuad* quad,						// output: quad to draw
	bool isOpenGL								// true if opengl fill rule; false if DX9 or earlier
);













