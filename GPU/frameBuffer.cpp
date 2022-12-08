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
	mDepthBuffer = new float[width * height];	//大小与colorBuffer对齐
	std::fill_n(mDepthBuffer, width * height, 1.0f);	//全部填充为1.0
}
FrameBuffer::~FrameBuffer()
{
	if (!mExternBuffer && mColorBuffer) {
		delete[] mColorBuffer;		//如果是自己申请的内存一定要自己删除
	}
	if (mDepthBuffer) delete[] mDepthBuffer;
}