#pragma once

#include <cfloat>
#include <cstdarg>
#include <cstddef>
#include <cstring>
#include <cassert>

#ifndef GALGUI_API
#define GALGUI_API
#endif

template <typename Exp>
constexpr void Gal_Assert(Exp exp)
{
	assert(exp);
}

struct GalDrawChannel;					// 临时储存用于无序输出的绘图命令,使用于 GalDrawListDataSplitter 和 GalDrawList::ChannelsSplit()
struct GalDrawCmd;						// GalDrawList中一个单独的子draw命令(通常映射到1个GPU draw调用，除非它是回调)
struct GalDrawData;						// 渲染要用于投影矩阵的帧+位置/大小坐标所需的所有绘制命令列表
struct GalDrawList;						// 单个绘图命令列表(通常每个窗口一个，从概念上讲，您可以将其视为动态"网格"(mesh)生成器)
struct GalDrawListSharedData;			// 在多个绘图列表之间共享数据(通常由 GalGui Context 拥有，但您可以自己创建一个)
struct GalDrawListDataSplitter;			// 将 draw list拆分为不同的层次,无序绘制,之后再平铺回来
struct GalDrawVertex;					// 单个顶点(默认情况下pos+uv+col=20字节)

struct GalFront;						// GalFontAtlas 中单个字体的运行时数据
struct GalFontAtlas;					// 多个字体的运行时数据，将多个字体烘焙成一个纹理，TTF/OTF字体加载器
struct GalFontConfig;					// 添加字体或合并字体时的配置数据
struct GalFontGlyph;					// 单个字体字形(code point + GalFontAtlas中的coordinates + offset)
struct GalFontGlyphBuilder;				// 从文本/字符串数据生成字形范围的构造器
struct GalColor;						// 创建可转换为u32或float4的颜色的助手函数(*过时*请避免使用)

struct GalGuiContext;					// GalGui 上下文(不透明的结构,除非包含 GalGui_internal.h)
struct GalGuiIO;						// 应用程序与 GalGui 之间的 IO
struct GalGuiInputTextCallbackData;		// 使用自定的 GalGuiInputTextCallback() 来共享 InputText() 的状态(很少使用)
struct GalGuiListClipper;				// 用于切分一个大列表中的物件
struct GalGuiOnceUponAFrame;			// 用于在每帧运行一次的代码块
struct GalGuiPayload;					// 用于拖放操作时的用户数据负载
struct GalGuiSizeCallbackData;			// 使用 SetNextWindowSizeConstraints() 的回调数据(很少使用)
struct GalGuiStorage;					// 键值对的储存
struct GalGuiStyle;						// 样式/颜色的运行时数据
struct GalGuiTextBuffer;				// 用于追加字符
struct GalGuiTextFilter;				// 用于解析文本并进行过滤

typedef int GalGuiCol;					// -> enum GalGuiCol_             // Enum: A color identifier for styling
typedef int GalGuiCond;					// -> enum GalGuiCond_            // Enum: A condition for many Set*() functions
typedef int GalGuiDataType;				// -> enum GalGuiDataType_        // Enum: A primary data type
typedef int GalGuiDir;					// -> enum GalGuiDir_             // Enum: A cardinal direction
typedef int GalGuiKey;					// -> enum GalGuiKey_             // Enum: A key identifier (ImGui-side enum)
typedef int GalGuiNavInput;				// -> enum GalGuiNavInput_        // Enum: An input identifier for navigation
typedef int GalGuiMouseButton;			// -> enum GalGuiMouseButton_     // Enum: A mouse button identifier (0=left, 1=right, 2=middle)
typedef int GalGuiMouseCursor;			// -> enum GalGuiMouseCursor_     // Enum: A mouse cursor identifier
typedef int GalGuiStyleVar;				// -> enum GalGuiStyleVar_        // Enum: A variable identifier for styling
typedef int GalDrawCornerFlags;			// -> enum GalDrawCornerFlags_    // Flags: for ImDrawList::AddRect(), AddRectFilled() etc.
typedef int GalDrawListFlags;			// -> enum GalDrawListFlags_      // Flags: for ImDrawList
typedef int GalFontAtlasFlags;			// -> enum GalFontAtlasFlags_     // Flags: for ImFontAtlas build
typedef int GalGuiBackendFlags;			// -> enum GalGuiBackendFlags_    // Flags: for io.BackendFlags
typedef int GalGuiColorEditFlags;		// -> enum GalGuiColorEditFlags_  // Flags: for ColorEdit4(), ColorPicker4() etc.
typedef int GalGuiConfigFlags;			// -> enum GalGuiConfigFlags_     // Flags: for io.ConfigFlags
typedef int GalGuiComboFlags;			// -> enum GalGuiComboFlags_      // Flags: for BeginCombo()
typedef int GalGuiDragDropFlags;		// -> enum GalGuiDragDropFlags_   // Flags: for BeginDragDropSource(), AcceptDragDropPayload()
typedef int GalGuiFocusedFlags;			// -> enum GalGuiFocusedFlags_    // Flags: for IsWindowFocused()
typedef int GalGuiHoveredFlags;			// -> enum GalGuiHoveredFlags_    // Flags: for IsItemHovered(), IsWindowHovered() etc.
typedef int GalGuiInputTextFlags;		// -> enum GalGuiInputTextFlags_  // Flags: for InputText(), InputTextMultiline()
typedef int GalGuiKeyModFlags;			// -> enum GalGuiKeyModFlags_     // Flags: for io.KeyMods (Ctrl/Shift/Alt/Super)
typedef int GalGuiPopupFlags;			// -> enum GalGuiPopupFlags_      // Flags: for OpenPopup*(), BeginPopupContext*(), IsPopupOpen()
typedef int GalGuiSelectableFlags;		// -> enum GalGuiSelectableFlags_ // Flags: for Selectable()
typedef int GalGuiTabBarFlags;			// -> enum GalGuiTabBarFlags_     // Flags: for BeginTabBar()
typedef int GalGuiTabItemFlags;			// -> enum GalGuiTabItemFlags_    // Flags: for BeginTabItem()
typedef int GalGuiTreeNodeFlags;		// -> enum GalGuiTreeNodeFlags_   // Flags: for TreeNode(), TreeNodeEx(), CollapsingHeader()
typedef int GalGuiWindowFlags;			// -> enum GalGuiWindowFlags_     // Flags: for Begin(), BeginChild()

// 一些其他类型
#ifndef GalTextureID
typedef void* GalTextureID;				// 后台缓冲区纹理的类型
#endif
typedef unsigned int GalGuiID;			// 一个独有的 ID ,通常来自字符串的哈希值
// 函数指针 GalGuiInputTextCallback
typedef int (*GalGuiInputTextCallback)(GalGuiInputTextCallbackData* data);
// 函数指针 GalGuiSizeCallback
typedef void (*GalGuiSizeCallback)(GalGuiSizeCallbackData* data);

// 字符解码
typedef unsigned short GalWChar16;
typedef unsigned int GalWChar32;
#ifdef GALGUI_USE_WCHAR32
typedef GalWChar32 GalWChar;
#else
typedef GalWChar16 GalWChar;
#endif

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

	explicit operator float*()
    {
        return reinterpret_cast<float*>(this);
    }
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
    explicit GalVec4(_In_reads_(4) const float* pArray) : x(pArray[0]), y(pArray[1]), z(pArray[2]), w(pArray[3]) {}

    explicit operator float*()
    {
        return reinterpret_cast<float*>(this);
    }
};

namespace GalGui
{
    // 上下文的创建与访问
	// 默认情况下,每个上下文都会创建自己的 GalFontAtlas
	// 你可以自己实例化一个并传给 CreateContext() 以在上下文中共享该字符图集
	// 这些函数不依赖于当前的上下文
    GALGUI_API GalGuiContext*           CreateContext(GalFontAtlas* sharedFontAtlas = nullptr);
    GALGUI_API void                     DestroyContext(GalGuiContext* context = nullptr);               // 传入空指针则销毁当前上下文
    GALGUI_API GalGuiContext*           GetCurrentContext();
    GALGUI_API void                     SetCurrentContext(GalGuiContext* context);

	// 主函数
    GALGUI_API GalGuiIO&                GetIO();
    GALGUI_API GalGuiStyle&             GetStyle();
    GALGUI_API void                     NewFrame();
    GALGUI_API void                     EndFrame();
    GALGUI_API void                     Render();
    GALGUI_API GalDrawData*             GetDrawData();

	// 用于演示,测试,获取信息
    GALGUI_API void                     ShowDemoWindow(bool& open);
    GALGUI_API void                     ShowAboutWindow(bool& open);
    GALGUI_API void                     ShowMetricsWindow(bool& open);
    GALGUI_API void                     ShowStyleEditor(GalGuiStyle& style);
    GALGUI_API bool                     ShowStyleSelector(const char* label);
    GALGUI_API void                     ShowFontSelector(const char* label);
    GALGUI_API void                     ShowUserGuide();
    GALGUI_API const char*              GetVersion();

	// 样式
    GALGUI_API void                     StyleColorsDark(GalGuiStyle& style);
	GALGUI_API void                     StyleColorsClassic(GalGuiStyle& style);
	GALGUI_API void                     StyleColorsLight(GalGuiStyle& style);









	
}

















