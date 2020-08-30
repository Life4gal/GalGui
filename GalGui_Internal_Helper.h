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

using GalS8 = signed char;
using GalU8 = unsigned char;
using GalS16 = signed short;
using GalU16 = unsigned short;
using GalS32 = signed int;
using GalU32 = unsigned int;
using GalS64 = signed   __int64;
using GalU64 = unsigned __int64;

// 字符解码
using GalWChar16 = unsigned short;
using GalWChar32 = unsigned int;
using GalWChar = GalWChar16;

constexpr auto GalUnicodeCodePointInvalid = 0xFFFD;
constexpr auto GalUnicodeCodePointMax = 0xFFFF;


















using GalGuiCol = int; 	// -> enum GalGuiCol_             // Enum: A color identifier for styling
using GalGuiCond = int; 	// -> enum GalGuiCond_            // Enum: A condition for many Set*() functions
using GalGuiDataType = int; 	// -> enum GalGuiDataType_        // Enum: A primary data type
using GalGuiDir = int;	// -> enum GalGuiDir_             // Enum: A cardinal direction
using GalGuiKey = int;	// -> enum GalGuiKey_             // Enum: A key identifier (ImGui-side enum)
using GalGuiNavInput = int;	// -> enum GalGuiNavInput_        // Enum: An input identifier for navigation
using GalGuiMouseButton = int;// -> enum GalGuiMouseButton_     // Enum: A mouse button identifier (0=left, 1=right, 2=middle)
using GalGuiMouseCursor = int;// -> enum GalGuiMouseCursor_     // Enum: A mouse cursor identifier
using GalGuiStyleVar = int;// -> enum GalGuiStyleVar_        // Enum: A variable identifier for styling
using GalDrawCornerFlags = int;	// -> enum GalDrawCornerFlags_    // Flags: for ImDrawList::AddRect(), AddRectFilled() etc.
using GalDrawListFlags = int;// -> enum GalDrawListFlags_      // Flags: for ImDrawList
using GalFontAtlasFlags = int;	// -> enum GalFontAtlasFlags_     // Flags: for ImFontAtlas build
using GalGuiBackendFlags = int;	// -> enum GalGuiBackendFlags_    // Flags: for io.BackendFlags
using GalGuiColorEditFlags = int;// -> enum GalGuiColorEditFlags_  // Flags: for ColorEdit4(), ColorPicker4() etc.
using GalGuiConfigFlags = int;	// -> enum GalGuiConfigFlags_     // Flags: for io.ConfigFlags
using GalGuiComboFlags = int;// -> enum GalGuiComboFlags_      // Flags: for BeginCombo()
using GalGuiDragDropFlags = int;// -> enum GalGuiDragDropFlags_   // Flags: for BeginDragDropSource(), AcceptDragDropPayload()
using GalGuiFocusedFlags = int;// -> enum GalGuiFocusedFlags_    // Flags: for IsWindowFocused()
using GalGuiHoveredFlags = int;// -> enum GalGuiHoveredFlags_    // Flags: for IsItemHovered(), IsWindowHovered() etc.
using GalGuiInputTextFlags = int;// -> enum GalGuiInputTextFlags_  // Flags: for InputText(), InputTextMultiline()
using GalGuiKeyModFlags = int;// -> enum GalGuiKeyModFlags_     // Flags: for io.KeyMods (Ctrl/Shift/Alt/Super)
using GalGuiPopupFlags = int;// -> enum GalGuiPopupFlags_      // Flags: for OpenPopup*(), BeginPopupContext*(), IsPopupOpen()
using GalGuiSelectableFlags = int;// -> enum GalGuiSelectableFlags_ // Flags: for Selectable()
using GalGuiTabBarFlags = int;// -> enum GalGuiTabBarFlags_     // Flags: for BeginTabBar()
using GalGuiTabItemFlags = int;// -> enum GalGuiTabItemFlags_    // Flags: for BeginTabItem()
using GalGuiTreeNodeFlags = int;// -> enum GalGuiTreeNodeFlags_   // Flags: for TreeNode(), TreeNodeEx(), CollapsingHeader()
using GalGuiWindowFlags = int;	// -> enum GalGuiWindowFlags_     // Flags: for Begin(), BeginChild()

extern GalGuiContext* g_galGuiContext;

struct GalGuiIO
{
	GalGuiConfigFlags configFlags;
	GalGuiBackendFlags backendFlags;
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
		if(ptr)
		{
			if(auto context = g_galGuiContext)
			{
				context
			}
		}
	}








	
}

template <typename Exp>
constexpr void Gal_Assert(Exp exp)
{
	assert(exp);
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
static constexpr T GalClamp(T v, T mn, T mx)
{
	return (v < mn) ? mn : (v > mx) ? mx : v;
}

template <typename T>
static constexpr T GalLerp(T a, T b, float t)
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
static constexpr T GalAddClampOverflow(T a, T b, T mn, T mx)
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
static constexpr T GalSubClampOverflow(T a, T b, T mn, T mx)
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
		min.x = static_cast<float>(static_cast<int>(min.x));			// floorf(min.x);
		min.y = static_cast<float>(static_cast<int>(min.y));			// floorf(min.y);
		max.x = static_cast<float>(static_cast<int>(max.x));			// floorf(max.x);
		max.y = static_cast<float>(static_cast<int>(max.y));			// floorf(max.y);
	}

	constexpr bool IsInverted() const
	{
		return min.x > max.x || min.y > max.y;
	}
};

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

template <typename T>
struct GalVector
{
	size_t size;
	size_t capacity;
	T* data;

	GalVector() : size(0), capacity(0), data(nullptr) {}

	GalVector<T>& operator=(const GalVector<T>& src)
	{
		clear();
		resize(src.size);
		memcpy_s(data, size * sizeof(T), src.data, size * sizeof(T));
		return *this;
	}
	
	GalVector(const GalVector<T>& src) : size(0), capacity(0), data(nullptr)
	{
		this = src;
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

	constexpr void clear()
	{
		if(data)
		{
			size = 0;
			capacity = 0;
			
		}
	}
};

struct GalBitVec
{
	GalV
};



