#include "gpu.h"
#include "raster.h"

ZGPU* ZGPU::mZGPUInstance = nullptr;

ZGPU::ZGPU(){}

ZGPU::~ZGPU()
{
	if (mFrameBuffer) delete mFrameBuffer;
}

ZGPU* ZGPU::GetZGPUInstance()
{
	if (mZGPUInstance == nullptr) {
		mZGPUInstance = new ZGPU();
	}
	return mZGPUInstance;
}

void ZGPU::InitSurface(const uint32_t& inWidth, const uint32_t& inHeight, void* bufferPtr)
{
	mFrameBuffer = new FrameBuffer(inWidth, inHeight, bufferPtr);		//这个初始化其实就是开辟一块内存给FrameBuffer使用
}

void ZGPU::Clear()
{
	//并不是清除FrameBuffer,而是给FrameBuffer填充背景色
	size_t pixelCount = mFrameBuffer->mHeight * mFrameBuffer->mWidth;
	std::fill_n(mFrameBuffer->mColorBuffer, pixelCount, ZRGBA(20, 20, 15, 15));
}

void ZGPU::DrawPoint(const uint32_t& inx, const uint32_t& iny, const ZRGBA& incolor)
{
	size_t inIndex = iny * mFrameBuffer->mWidth + inx;
	mFrameBuffer->mColorBuffer[inIndex] = incolor;
}

void ZGPU::DrawLine(const ZScrPoint& startPoint, const ZScrPoint& endPoint)
{
	std::vector<ZScrPoint> linePoints;
	Raster::RasterizeLine_Brensenham(linePoints, startPoint, endPoint);		//Brensenham算法在屏幕上绘制线段
	for (auto tPoint : linePoints)
	{
		DrawPoint(tPoint.X, tPoint.Y, tPoint.color);
	}
}
