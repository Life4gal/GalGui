#include "GalGui_Internal_TrueType.h"

namespace
{
	// ReSharper disable once CppInconsistentNaming
	enum class PlatformID
	{
		PLATFORM_ID_UNICODE = 0,
		PLATFORM_ID_MAC = 1,
		PLATFORM_ID_ISO = 2,
		PLATFORM_ID_MICROSOFT = 3
	};

	// ReSharper disable once CppInconsistentNaming
	enum class EncodingIDUnicode
	{
		UNICODE_EID_UNICODE_1_0 = 0,
		UNICODE_EID_UNICODE_1_1 = 1,
		UNICODE_EID_ISO_10646 = 2,
		UNICODE_EID_UNICODE_2_0_BMP = 3,
		UNICODE_EID_UNICODE_2_0_FULL = 4
	};

	// ReSharper disable once CppInconsistentNaming
	enum class EncodingIDMicrosoft
	{
		MS_EID_SYMBOL = 0,
		MS_EID_UNICODE_BMP = 1,
		MS_EID_SHIFT_JIS = 2,
		MS_EID_UNICODE_FULL = 10
	};

	// ReSharper disable once CppInconsistentNaming
	enum class EncodingIDMac
	{
		MAC_EID_ROMAN = 0,
		MAC_EID_JAPANESE = 1,
		MAC_EID_CHINESE_TRAD = 2,
		MAC_EID_KOREAN = 3,
		MAC_EID_ARABIC = 4,
		MAC_EID_HEBREW = 5,
		MAC_EID_GREEK = 6,
		MAC_EID_RUSSIAN = 7
	};

	// ReSharper disable once CppInconsistentNaming
	enum class LangIDMicrosoft
	{
		MS_LID_ENGLISH = 0x0409,
		MS_LID_ITALIAN = 0x0410,
		MS_LID_CHINESE = 0x0804,
		MS_LID_JAPANESE = 0x0411,
		MS_LID_DUTCH = 0x0413,
		MS_LID_KOREAN = 0x0412,
		MS_LID_FRENCH = 0x040c,
		MS_LID_RUSSIAN = 0x0419,
		MS_LID_GERMAN = 0x0407,
		MS_LID_SPANISH = 0x0409,
		MS_LID_HEBREW = 0x040d,
		MS_LID_SWEDISH = 0x041D
	};

	// ReSharper disable once CppInconsistentNaming
	enum class LangIDMac
	{
		MAC_LID_ENGLISH = 0,
		MAC_LID_FRENCH = 1,
		MAC_LID_GERMAN = 2,
		MAC_LID_ITALIAN = 3,
		MAC_LID_DUTCH = 4,
		MAC_LID_SWEDISH = 5,
		MAC_LID_SPANISH = 6,
		MAC_LID_HEBREW = 10,
		MAC_LID_JAPANESE = 11,
		MAC_LID_ARABIC = 12,
		MAC_LID_CHINESE_TRAD = 19,
		MAC_LID_KOREAN = 23,
		MAC_LID_RUSSIAN = 32,
		MAC_LID_CHINESE_SIMPLIFIED = 33	
	};
	
	// 如早点认识到模版是洪荒猛兽就不会浪费那么多时间了...
	
	/*
	// N = 1 -> 8, N = 2 -> 16, N = 4 -> 32
	template <int N>
	constexpr auto BufferGet(GalttBuffer* buffer)
	{
		static_assert(N == 1 || N == 2 || N == 4);

		using Type = std::conditional_t<N == 1, GalttU8, std::conditional_t<N == 2, GalttU16, GalttU32>>;

		auto get8 = [](GalttBuffer* buffer) -> GalttU8
		{
			if (buffer->cursor >= buffer->size)
			{
				return 0;
			}
			return buffer->data[buffer->cursor++];
		};

		Type v = 0;
		for (auto i = 0; i < N; ++i)
		{
			v = (v << 8) | get8(buffer);
		}
		return v;
	}
	*/
	
	constexpr GalttU8 BufferGet8(GalttBuffer* buffer)
	{
		if (buffer->cursor >= buffer->size)
		{
			return 0;
		}
		return buffer->data[buffer->cursor++];
	}

	// 2 -> 16, 4 -> 32
	constexpr GalttU32 BufferGet(GalttBuffer* buffer, const int n)
	{
		GALTT_ASSERT(n >= 1 && n <= 4);
		GalttU16 v = 0;
		for(auto i = 0; i < n; ++i)
		{
			v = (v << 8) | BufferGet8(buffer);
		}
		return v;
	}

	constexpr GalttU8 BufferPeek8(GalttBuffer* buffer)
	{
		if(buffer->cursor >= buffer->size)
		{
			return 0;
		}
		return buffer->data[buffer->cursor];
	}

	/*
	template <int Origin>
	constexpr void BufferSeek(GalttBuffer* buffer)
	{
		static_assert(Origin < buffer->size && Origin > 0);
		buffer->cursor = Origin;
	}
	*/

	constexpr void BufferSeek(GalttBuffer* buffer, const int origin)
	{
		GALTT_ASSERT(origin > 0 && origin < buffer->size);
		buffer->cursor = origin;
	}

	/*
	template <int Origin>
	constexpr void BufferSkip(GalttBuffer* buffer)
	{
		BufferSeek<buffer->cursor + Origin>(buffer);
	}
	*/

	constexpr void BufferSkip(GalttBuffer* buffer, const int origin)
	{
		BufferSeek(buffer, buffer->cursor + origin);
	}

	/*
	template <int Size>
	constexpr GalttBuffer BufferNew(const void* p)
	{
		static_assert(Size < 0x40000000);
		return { static_cast<GalttU8*>(const_cast<void*>(p)), 0, Size };
		
	}
	*/

	constexpr GalttBuffer BufferNew(const void* p, const int size)
	{
		GALTT_ASSERT(size < 0x40000000);
		return { static_cast<GalttU8*>(const_cast<void*>(p)), 0, size };
	}

	/*
	template <int Origin, int Size>
	constexpr GalttBuffer BufferRange(const GalttBuffer* buffer)
	{
		if (Origin < 0 || Size < 0 || Origin > buffer->size || Size > buffer->size)
		{
			return BufferNew<0>(nullptr);
		}
		return BufferNew<Size>(buffer->data + Origin);
	}
	*/

	constexpr GalttBuffer BufferRange(const GalttBuffer* buffer, const int origin, const int size)
	{
		if (origin < 0 || size < 0 || origin > buffer->size || size > buffer->size)
		{
			return BufferNew(nullptr, 0);
		}
		return BufferNew(buffer->data + origin, size);
	}

	constexpr GalttBuffer CffGetIndex(GalttBuffer* buffer)
	{
		const auto start = buffer->cursor;
		if (const auto count = BufferGet(buffer, 2))
		{
			const auto offSize = BufferGet8(buffer);
			GALTT_ASSERT(offSize >= 1 && offSize <= 4);
			BufferSkip(buffer, static_cast<int>(offSize * count));
			BufferSkip(buffer, static_cast<int>(BufferGet(buffer, offSize)) - 1);
		}
		return BufferRange(buffer, start, buffer->cursor - start);
	}
	
	constexpr GalttU32 CffInt(GalttBuffer* buffer)
	{
		if(const auto b = BufferGet8(buffer); b >= 32 && b <= 246)
		{
			return b - 139;
		}
		else
		{
			if(b >= 247 && b <= 250)
			{
				return (b - 247) * 256 + BufferGet8(buffer) + 108;
			}
			if(b >= 251 && b < 254)
			{
				return -(b - 251) * 256 - BufferGet8(buffer) - 108;
			}
			if(b == 28)
			{
				return BufferGet(buffer, 2);
			}
			if(b == 29)
			{
				return BufferGet(buffer, 4);
			}
			GALTT_ASSERT(0);
			return 0;
		}
	}

	constexpr void CffSkipOperand(GalttBuffer* buffer)
	{
		const auto b = BufferPeek8(buffer);
		GALTT_ASSERT(b >= 28);
		if(b == 30)
		{
			BufferSkip(buffer, 1);
			while (buffer->cursor < buffer->size)
			{
				const auto v = BufferGet8(buffer);
				if((v & 0xf) == 0xf || (v >> 4) == 0xf)
				{
					break;
				}
			}
		}
		else
		{
			CffInt(buffer);
		}
	}

	constexpr GalttBuffer DictGet(GalttBuffer* buffer, const int key)
	{
		BufferSeek(buffer, 0);
		while (buffer->cursor < buffer->size)
		{
			const auto start = buffer->cursor;
			while(BufferPeek8(buffer) >= 28)
			{
				CffSkipOperand(buffer);
			}
			const auto end = buffer->cursor;
			auto op = BufferGet8(buffer);
			if(op == 12)
			{
				op = BufferGet8(buffer) | 0x100;
			}
			if(op == key)
			{
				return BufferRange(buffer, start, end - start);
			}
		}
		return BufferRange(buffer, 0, 0);
	}

	constexpr void DictGetInt(GalttBuffer* buffer, const int key, const int outCount, GalttU32* out)
	{
		auto operands = DictGet(buffer, key);
		for(auto i = 0; i < outCount && operands.cursor < operands.size; ++i)
		{
			out[i] = CffInt(&operands);
		}

	}

	constexpr int CffIndexCount(GalttBuffer* buffer)
	{
		BufferSeek(buffer, 0);
		return BufferGet(buffer, 2);
	}

	constexpr GalttBuffer CffIndexGet(GalttBuffer buffer, const int i)
	{
		BufferSeek(&buffer, 0);
		const auto count = BufferGet(&buffer, 2);
		const auto offSize = BufferGet8(&buffer);
		GALTT_ASSERT(i >= 0 && i < count);
		GALTT_ASSERT(offSize >= 1 && offSize <= 4);
		BufferSkip(&buffer, i * offSize);
		const auto start = BufferGet(&buffer, offSize);
		const auto end = BufferGet(&buffer, offSize);
		return BufferRange(&buffer, static_cast<int>(2 + (count + 1) * offSize + start), static_cast<int>(end - start));
	}

	// unsigned short
	constexpr GalttU16 U8ToU16(GalttU8* p)
	{
		return p[0] * 256 + p[1];
	}

	// short
	constexpr GalttS16 U8ToS16(GalttU8* p)
	{
		return p[0] * 256 + p[1];
	}

	// unsigned int
	constexpr GalttU32 U8ToU32(GalttU8* p)
	{
		return (p[0] << 24) + (p[1] << 16) + (p[2] << 8) + p[3];
	}

	// int
	constexpr GalttS32 U8ToS32(GalttU8* p)
	{
		return (p[0] << 24) + (p[1] << 16) + (p[2] << 8) + p[3];
	}

	template <typename Ptr>
	constexpr auto ToByte(Ptr p)
	{
		return *static_cast<GalttU8*>(p);
	}

	template <typename Ptr>
	constexpr auto ToChar(Ptr p)
	{
		return *static_cast<GalttS8*>(p);
	}

	constexpr bool Tag4(const GalttU8* font, const char c0, const char c1, const char c2, const char c3)
	{
		return font[0] == c0 && font[1] == c1 && font[2] == c2 && font[3] == c3;
	}

	constexpr bool Tag(const GalttU8* font, const char* str)
	{
		return font[0] == str[0] && font[1] == str[1] && font[2] == str[2] && font[3] == str[3];
	};
	
	constexpr bool IsFont(GalttU8* font)
	{
		if(
			Tag4(font, '1', 0, 0, 0) ||		// TrueType 1
			Tag(font, "typ1") ||						// TrueType with type 1 font -- we don't support this!
			Tag(font, "OTTO") ||						// OpenType with CFF
			Tag4(font, 0, 1, 0, 0) ||		// OpenType 1.0
			Tag(font, "true"))						// Apple specification for TrueType fonts
		{
			return true;
		}
		return false;
	}

	// OPTIMIZE: binary search
	constexpr GalttU32 FindTable(GalttU8* data, const GalttU32 fontStart, const char* tag)
	{
		const auto numTable = U8ToU16(data + fontStart + 4);
		const auto tableDir = fontStart + 12;
		for(auto i = 0; i < numTable; ++i)
		{
			const auto loc = tableDir + 16 * i;
			if (Tag(data + loc + 0, tag))
			{
				return U8ToU32(data + loc + 8);
			}
		}
		return 0;
	}

	constexpr int GetNumberOfFont(unsigned char* font)
	{
		// if it's just a font, there's only one valid font
		if(IsFont(font))
		{
			return 1;
		}

		// check if it's a TTC
		if(Tag(font, "ttcf"))
		{
			// version 1?
			if(U8ToU32(font + 4) == 0x00010000 || U8ToU32(font + 4) == 0x00020000)
			{
				return U8ToS32(font + 8);
			}
		}

		return 0;
	}

	constexpr GalttBuffer GetSubrs(GalttBuffer cff, GalttBuffer fontDict)
	{
		GalttU32 subrs = 0;
		GalttU32 privateLoc[2] = { 0, 0 };
		DictGetInt(&fontDict, 18, 2, privateLoc);
		if (privateLoc[1] != 0 || privateLoc[0] != 0)
		{
			return BufferNew(nullptr, 0);
		}
		auto dict = BufferRange(&cff, privateLoc[1], privateLoc[0]);
		DictGetInt(&dict, 19, 1, &subrs);
		if(!subrs)
		{
			return BufferNew(nullptr, 0);
		}
		BufferSeek(&cff, static_cast<int>(privateLoc[1] + subrs));
		return CffGetIndex(&cff);
	}

	

	
}




constexpr int GetFontOffsetForIndex(unsigned char* font, const int index)
{
	// if it's just a font, there's only one valid index
	if (IsFont(font))
	{
		return index == 0 ? 0 : -1;
	}

	// check if it's a TTC
	if (Tag(font, "ttcf"))
	{
		// version 1?
		if (U8ToU32(font + 4) == 0x00010000 || U8ToU32(font + 4) == 0x00020000)
		{
			if (index >= U8ToS32(font + 8))
			{
				return -1;
			}
			return U8ToU32(font + 12 + index * 4);
		}
	}

	return -1;
}

constexpr bool InitFont(GalttFontInfo* info, unsigned char* data, const int fontStart)
{
	info->data = data;
	info->fontStart = fontStart;
	info->cff = BufferNew(nullptr, 0);

	const auto cmap = FindTable(data, fontStart, "cmap");
	info->loca = FindTable(data, fontStart, "loca");
	info->head = FindTable(data, fontStart, "head"); // required
	info->glyf = FindTable(data, fontStart, "glyf"); // required
	info->hhea = FindTable(data, fontStart, "hhea"); // required
	info->hmtx = FindTable(data, fontStart, "hmtx"); // required
	info->kern = FindTable(data, fontStart, "kern"); // not required
	info->gpos = FindTable(data, fontStart, "GPOS"); // not required

	if (!cmap || !info->head || !info->hhea || !info->hmtx)
	{
		return false;
	}
	if (info->glyf)
	{
		// required for truetype
		if (!info->loca) return false;
	}
	else
	{
		// initialization for CFF / Type2 fonts (OTF)
		GalttU32 csType = 2;
		GalttU32 charString = 0;
		GalttU32 fdArrayOff = 0;
		GalttU32 fdSelectOff = 0;

		const auto cff = FindTable(data, fontStart, "CFF ");
		if (!cff) return false;

		info->fontDicts = BufferNew(nullptr, 0);
		info->fdSelect = BufferNew(nullptr, 0);

		// @TODO this should use size from table (not 512MB)
		info->cff = BufferNew(data + cff, 512 * 1024 * 1024);
		auto buffer = info->cff;

		// read the header
		BufferSkip(&buffer, 2);
		BufferSeek(&buffer, BufferGet8(&buffer));	// hdrsize

		// @TODO the name INDEX could list multiple fonts,
		// but we just use the first one.
		CffGetIndex(&buffer);		// name INDEX
		const auto topDictIndex = CffGetIndex(&buffer);
		auto topDict = CffIndexGet(topDictIndex, 0);
		CffGetIndex(&buffer);		// string INDEX
		info->gsubrs = CffGetIndex(&buffer);

		DictGetInt(&topDict, 17, 1, &charString);
		DictGetInt(&topDict, 0x100 | 6, 1, &csType);
		DictGetInt(&topDict, 0x100 | 36, 1, &fdArrayOff);
		DictGetInt(&topDict, 0x100 | 37, 1, &fdSelectOff);
		info->subrs = GetSubrs(buffer, topDict);

		// we only support Type 2 charStrings
		if (csType != 2) return false;
		if (charString == 0) return false;

		if (fdArrayOff)
		{
			// looks like a CID font
			if (!fdSelectOff) return false;

			BufferSeek(&buffer, fdArrayOff);
			info->fontDicts = CffGetIndex(&buffer);
			info->fdSelect = BufferRange(&buffer, fdSelectOff, buffer.size - fdSelectOff);
		}

		BufferSeek(&buffer, charString);
		info->charStrings = CffGetIndex(&buffer);
	}

	const auto t = FindTable(data, fontStart, "maxp");
	if (t)
	{
		info->numGlyphs = U8ToU16(data + t + 4);
	}
	else
	{
		info->numGlyphs = 0xffff;
	}

	// find a cmap encoding table we understand *now* to avoid searching
	// later. (TODO: could make this installable)
	// the same regardless of glyph.
	const auto numTables = U8ToU16(data + cmap + 2);
	info->indexMap = 0;
	for (auto i = 0; i < numTables; ++i)
	{
		const GalttU32 encodingRecord = cmap + 4 + 8 * i;
		// find an encoding we understand:
		switch (U8ToU16(data + encodingRecord))
		{
			case static_cast<GalttU16>(PlatformID::PLATFORM_ID_MICROSOFT) :
			{
				switch (U8ToU16(data + encodingRecord + 2))
				{
					case static_cast<GalttU16>(EncodingIDMicrosoft::MS_EID_UNICODE_BMP) :
						case static_cast<GalttU16>(EncodingIDMicrosoft::MS_EID_UNICODE_FULL) :
						// MS/Unicode
						info->indexMap = static_cast<int>(cmap) + U8ToU32(data + encodingRecord + 4);
						break;
						default:;
				}
				break;
			}
			case static_cast<GalttU16>(PlatformID::PLATFORM_ID_UNICODE) :
			{
				// Mac/IOS
				info->indexMap = static_cast<int>(cmap) + U8ToU32(data + encodingRecord + 4);
				break;
			}
			default:;
		}
	}
	if (info->indexMap == 0) return false;
	info->indexToLocFormat = U8ToU16(data + info->head + 50);
	return true;
}
