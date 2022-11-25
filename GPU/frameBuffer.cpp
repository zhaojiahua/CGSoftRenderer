#include "frameBuffer.h"

FrameBuffer::FrameBuffer(uint32_t width, uint32_t height, void* buffer)
{
	mWidth = width;
	mHeight = height;
	if (buffer == nullptr) {
		buffer = new ZRGBA[width * height];
		mExternBuffer = false;
	}
	else mExternBuffer = true;
	mColorBuffer = (ZRGBA*)buffer;
}
FrameBuffer::~FrameBuffer()
{
	if (!mExternBuffer && mColorBuffer) {
		delete[] mColorBuffer;		//如果是自己申请的内存一定要自己删除
	}
}