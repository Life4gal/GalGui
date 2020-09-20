#pragma once

#include <type_traits>
#include <cassert>

#define GALTT_ASSERT assert

using GalttS8 = signed char;
using GalttU8 = unsigned char;
using GalttS16 = signed short;
using GalttU16 = unsigned short;
using GalttS32 = signed int;
using GalttU32 = unsigned int;

struct GalttBuffer
{
	GalttU8* data;
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

struct GalttPackedChar
{
	unsigned short x0;
	unsigned short y0;
	unsigned short x1;
	unsigned short y1;
	float xOffset;
	float yOffset;
	float xAdvance;
	float xOffset2;
	float yOffset2;
};

struct GalttPackContext
{
	void* userAllocatorContext;
	void* packInfo;
	int width;
	int height;
	int strideInBytes;
	int skipMissing;
	unsigned int overSampleH;
	unsigned int overSampleV;
	unsigned char* pixel;
	void* nodes;
};

struct GalttFontInfo
{
	void* userData;
	unsigned char* data;	// pointer to .ttf file
	int fontStart;			// offset of start of font

	int numGlyphs;			// number of glyphs, needed for range checking

	// table locations as offset from start of .ttf
	int loca;
	int head;
	int glyf;
	int hhea;
	int hmtx;
	int kern;
	int gpos;

	int indexMap;			// a cmap mapping for our chosen character encoding
	int indexToLocFormat;	// format needed to map from glyph index to glyph

	GalttBuffer cff;		// cff font data
	GalttBuffer charStrings;// the charstring index
	GalttBuffer gsubrs;		// global charstring subroutines index
	GalttBuffer subrs;		// private charstring subroutines index
	GalttBuffer fontDicts;	// array of font dicts
	GalttBuffer fdSelect;	// map from glyph to fontDict
};


// GalttMaxOverSample cannot be > 255
constexpr unsigned short GalttMaxOverSample = 8;

constexpr static GalttU8 BufferGet8()

static int Galtt_BakeFontBitmap(
	unsigned char* data, int offset,		// 字体的位置(普通的 .ttf 文件 offset == 0)
	float pixelHeight,							// 字体的高度(像素)
	unsigned char* pixels, int pw, int ph,		// 要填充的 bitmap
	int firstChar, int numChars,				// 烘培的字符
	GalttBackedChar* charData					// 目的地, num_chars 大小
)
{
	GalttFontInfo fontInfo
}



static void GalttGetBakedQuad(
	const GalttBackedChar* charData,
	int pw, int ph,
	int charIndex,								// character to display
	float* xPos, float* yPos,					// pointers to current position in screen pixel space
	GalttAlignedQuad* quad,						// output: quad to draw
	bool isOpenGL								// true if opengl fill rule; false if DX9 or earlier
);

static void GalttGetScaleFontVMetrics(
	const unsigned char* fontData,
	int index, int size,
	float* ascent,
	float* descent,
	float* lineGap
);


int GalttPackBegin(
	GalttPackContext* packContext, unsigned char* pixels, int width, int height,
	int strideInBytes, int padding, void* allocContext);

void GalttPackEnd(GalttPackContext* packContext);

int GalttPackFontRange(
	GalttPackContext* packContext, const unsigned char* fontData, int fontSize,
	int firstUnicodeCharInRange, int numCharsInRange, GalttPackedChar* charDataForRange
);

struct GalttPackRange
{
	float fontSize;
	int firstUnicodeCodepointInRange;
	int* arrayOfUnicodeCodepoint;
	int numChars;
	GalttPackedChar* charDataForRange;
	unsigned char overSampleH;
	unsigned char overSampleV;
};

int GalttPackFontRange(
	GalttPackContext* packContext, const unsigned char* fontData, int fontSize,
	GalttPackRange* ranges, int numRanges
);

void GalttPackSetOverSampling(GalttPackContext* packContext, unsigned int overSampleH, unsigned int overSampleV);

void GalttPackSetSkipMissingCodepoint(GalttPackContext* packContext, int skip);



constexpr int GetFontOffsetForIndex(unsigned char* font, int index);

constexpr bool InitFont(GalttFontInfo* info, unsigned char* data, int fontStart);
