#pragma once

#include "GalGui_Internal_Helper.h"

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


enum class EnumGalGuiCol;
enum class EnumGalGuiDir;

// 一些其他类型
#ifndef GalTextureID
typedef void* GalTextureID;				// 后台缓冲区纹理的类型
#endif
typedef unsigned int GalGuiID;			// 一个独有的 ID ,通常来自字符串的哈希值
// 函数指针 GalGuiInputTextCallback
typedef int (*GalGuiInputTextCallback)(GalGuiInputTextCallbackData* data);
// 函数指针 GalGuiSizeCallback
typedef void (*GalGuiSizeCallback)(GalGuiSizeCallbackData* data);


namespace GalGui
{
    // 上下文的创建与访问
	// 默认情况下,每个上下文都会创建自己的 GalFontAtlas
	// 你可以自己实例化一个并传给 CreateContext() 以在上下文中共享该字符图集
	// 这些函数不依赖于当前的上下文
    GalGuiContext*           CreateContext(GalFontAtlas* sharedFontAtlas = nullptr);
    void                     DestroyContext(GalGuiContext* context = nullptr);               // 传入空指针则销毁当前上下文
    GalGuiContext*           GetCurrentContext();
    void                     SetCurrentContext(GalGuiContext* context);

	// 主函数
    GalGuiIO&                GetIO();
    GalGuiStyle&             GetStyle();
    void                     NewFrame();
    void                     EndFrame();
    void                     Render();
    GalDrawData*             GetDrawData();

	// 用于演示,测试,获取信息
    void                     ShowDemoWindow(bool& open);
    void                     ShowAboutWindow(bool& open);
    void                     ShowMetricsWindow(bool& open);
    void                     ShowStyleEditor(GalGuiStyle& style);
    bool                     ShowStyleSelector(const char* label);
    void                     ShowFontSelector(const char* label);
    void                     ShowUserGuide();
    const char*              GetVersion();

	// 样式
    void                     StyleColorsDark(GalGuiStyle& style);
	void                     StyleColorsClassic(GalGuiStyle& style);
	void                     StyleColorsLight(GalGuiStyle& style);









	
}

















