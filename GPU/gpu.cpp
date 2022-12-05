#include "gpu.h"
#include "gpu.h"
#include "raster.h"
#include "bufferObject.h"
#include "VAO.h"

ZGPU* ZGPU::mZGPUInstance = nullptr;

ZRGBA ZGPU::NearestSimple(const math::vec2f& inuv)
{
	//查看UV的值是否在0-1之间,如果不是会根据uvwrap的方式处理uv
	math::vec2f tempuv = inuv;
	CheckValue(tempuv.X);
	CheckValue(tempuv.Y);
	int32_t tx = std::round(tempuv.X * (textrue->mWidth - 1));
	int32_t ty = std::round(tempuv.Y * (textrue->mHeight - 1));
	if (textrue->mData)	return textrue->mData[tx + ty * textrue->mWidth];
	return ZRGBA();
}

ZRGBA ZGPU::BilinearitySimple(const math::vec2f& inuv)
{
	math::vec2f tempuv = inuv;
	CheckValue(tempuv.X);
	CheckValue(tempuv.Y);
	//先根据这个点的四个临近像素点分别纵向插值和横向插值算出两个lerp值,再计算出这个像素点的最终颜色值
	float tx = tempuv.X * (textrue->mWidth - 1);
	float ty = tempuv.Y * (textrue->mHeight - 1);

	int floorx = std::floor(tx);
	int floory = std::floor(ty);
	int ceilx = std::ceil(tx);
	int ceily = std::ceil(ty);
	//计算两个lerp值
	float weightH = (tx - floorx);
	float weightV = (ty - floory);
	//对tx和ty分别向上和向下取整得到四个临近像素点
	ZRGBA blcolor = textrue->mData[floorx + floory * textrue->mWidth];		//左下角的颜色值
	ZRGBA trcolor = textrue->mData[ceilx + ceily * textrue->mWidth];		//右上角的颜色值
	ZRGBA tlcolor = textrue->mData[floorx + ceily * textrue->mWidth];		//左上角的颜色值
	ZRGBA brcolor = textrue->mData[ceilx + floory * textrue->mWidth];		//右下角的颜色值

	ZRGBA lcolorH = Raster::LerpRGBA(blcolor, tlcolor, weightV);
	ZRGBA rcolorH = Raster::LerpRGBA(brcolor, trcolor, weightV);

	return Raster::LerpRGBA(lcolorH, rcolorH, weightH);
}

void ZGPU::CheckValue(float& inValue)
{
	if (inValue > 1.0f || inValue < 0.0f) {
		inValue = FRACTION(inValue);
		switch (UVwrap)
		{
		case TEXTRUE_WRAP_REPEAT:
			inValue = FRACTION(1 + inValue);
			break;
		case TEXTRUE_WRAP_MIRROR:
			inValue = 1.0f - FRACTION(1 + inValue);
			break;
		default:
			break;
		}
	}
}

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
	uint32_t tempinx = max(0, inx);
	tempinx = min(mFrameBuffer->mWidth-1, tempinx);
	uint32_t tempiny = max(0, iny);
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

void ZGPU::DrawLine(const VsOutPoint& startPoint, const VsOutPoint& endPoint)
{
	std::vector<VsOutPoint> linePoints;
	Raster::RasterizeLine_Brensenham(linePoints, startPoint, endPoint);		//Brensenham算法在屏幕上绘制线段
	for (const auto& tPoint : linePoints)
	{
		DrawPoint(tPoint.mPosition.X, tPoint.mPosition.Y, ZRGBA(255, 255, 255, 255));
	}
}

void ZGPU::SetTextrue(ZImage* inIma)
{
	if (inIma) textrue = inIma;
}

void ZGPU::DrawTriangle(const VsOutPoint& p1, const VsOutPoint& p2, const VsOutPoint& p3)
{
	std::vector<VsOutPoint> interPoints;
	Raster::RasterizeTriangle(interPoints, p1, p2, p3);
	for (const auto& tPoint : interPoints)
	{
		//如果设置了贴图就从贴图采样颜色,否则就直接使用点的颜色
		if (textrue) {
			if (bUseBilinearity) DrawPoint(tPoint.mPosition.X, tPoint.mPosition.Y, BilinearitySimple(tPoint.mUV));
			else DrawPoint(tPoint.mPosition.X, tPoint.mPosition.Y, NearestSimple(tPoint.mUV));
		}
		else DrawPoint(tPoint.mPosition.X, tPoint.mPosition.Y, ZRGBA(255, 255, 255, 255));
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

void ZGPU::DrawZImage(const ZImage* inImg, const uint32_t& ina)
{
	float talpha = static_cast<float>(ina) / 255;
	for (uint32_t i = 0; i < inImg->mWidth; i++) {
		for (uint32_t j = 0; j < inImg->mHeight; j++) {
			ZRGBA tempC = inImg->mData[j * inImg->mWidth + i];
			tempC.zA *= talpha;
			DrawPoint(i, j, tempC);
		}
	}
}

uint32_t ZGPU::GenerateVertexBuffer()
{
	mBufferCounter++;
	mVBOMap.insert(std::make_pair(mBufferCounter, new BufferObject()));
	return mBufferCounter;
}

void ZGPU::DeleteVertexBuffer(const uint32_t inBufferCounter)
{
	auto iter = mVBOMap.find(inBufferCounter);
	if (iter != mVBOMap.end()) {
		delete iter->second;
	}
	else return;
	mVBOMap.erase(iter);
}

uint32_t ZGPU::GenerateVertexArray()
{
	mVAOCounter++;
	mVAOMap.insert(std::make_pair(mVAOCounter, new VertexArrayObject()));
	return mVAOCounter;
}

void ZGPU::DeleteVertexArray(const uint32_t inCounter)
{
	auto iter = mVAOMap.find(inCounter);
	if (iter != mVAOMap.end()) {
		delete iter->second;
	}
	else return;
	mVAOMap.erase(iter);
}
