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

void ZGPU::DrawPoint(const int32_t& inx, const int32_t& iny, const ZRGBA& incolor)
{
	//限制inx和iny的值在0到mFrameBuffer->mWidth和mFrameBuffer->mHeight之间
	int32_t tempinx = max(0, inx);
	tempinx = min(mFrameBuffer->mWidth-1, tempinx);
	int32_t tempiny = max(0, iny);
	tempiny= min(mFrameBuffer->mHeight-1, tempiny);

	size_t inIndex = tempiny * mFrameBuffer->mWidth + tempinx;
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
