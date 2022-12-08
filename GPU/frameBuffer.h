#pragma once
#include "../global/base.h"

/*
Class FrameBuffer
存储当前画布对应的bmp内存指针,作为当前绘图画板
*/

class FrameBuffer
{
public:
	FrameBuffer(uint32_t width, uint32_t height, void* buffer = nullptr);
	~FrameBuffer();
	FrameBuffer(const FrameBuffer&) = delete;	//不允许拷贝复制

	uint32_t mWidth{ 0 };		//buffer的宽
	uint32_t mHeight{ 0 };		//buffer的高
	ZRGBA* mColorBuffer{ nullptr };		//存储来自拷贝mCanvasDC的位图的数据
	float* mDepthBuffer{ nullptr };	//图片的深度缓存数据

	bool mExternBuffer{ false };		//如果是自己创建的内存就为false,如果是外部传入的内存就是true,为析构函数调用
};