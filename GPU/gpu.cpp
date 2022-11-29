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
	ZRGBA tempColor = incolor;
	if (bEnableBlend) {
		auto orgColor = mFrameBuffer->mColorBuffer[inIndex];
		float weight = static_cast<float>(incolor.zA) / 255;
		tempColor.zB = weight * incolor.zB + (1 - weight) * orgColor.zB;
		tempColor.zG = weight * incolor.zG + (1 - weight) * orgColor.zG;
		tempColor.zR = weight * incolor.zR + (1 - weight) * orgColor.zR;
		tempColor.zA = weight * incolor.zA + (1 - weight) * orgColor.zA;
	}
	mFrameBuffer->mColorBuffer[inIndex] = tempColor;
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

void ZGPU::DrawTriangle(const ZScrPoint& p1, const ZScrPoint& p2, const ZScrPoint& p3)
{
	std::vector<ZScrPoint> trianglePoints;
	Raster::RasterizeTriangle(trianglePoints, p1, p2, p3);
	for (auto tPoint : trianglePoints)
	{
		DrawPoint(tPoint.X, tPoint.Y, tPoint.color);
	}
}

void ZGPU::DrawZImage(const ZImage* inImg)
{
	for (uint32_t i = 0; i < inImg->mWidth; i++) {
		for (uint32_t j = 0; j < inImg->mHeight; j++) {
			DrawPoint(i, j, inImg->mData[j * inImg->mWidth + i]);
		}
	}
}
