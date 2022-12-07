#include "gpu.h"
#include "gpu.h"
#include "raster.h"
#include "bufferObject.h"
#include "VAO.h"
#include "clipper.h"

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
	mScreenMatrix = math::ScreenMatrix<float>(inWidth - 1, inHeight - 1);
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
	std::vector<VsOutPoint> inPoints;
	inPoints.push_back(startPoint);
	inPoints.push_back(endPoint);
	Raster::Rasterize(linePoints, DRAW_LINES, inPoints);		//Brensenham算法在屏幕上绘制线段
	
}

void ZGPU::SetTextrue(ZImage* inIma)
{
	if (inIma) textrue = inIma;
}

void ZGPU::DrawTriangle(const VsOutPoint& p1, const VsOutPoint& p2, const VsOutPoint& p3)
{
	std::vector<VsOutPoint> interPoints;
	std::vector<VsOutPoint> inPoints;
	inPoints.push_back(p1);
	inPoints.push_back(p2);
	inPoints.push_back(p3);
	Raster::Rasterize(interPoints, DRAW_TRIANGLES, inPoints);
	
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

void ZGPU::BindingVertexBuffer(const uint32_t& bufferType, const uint32_t& bufferID)
{
	if (bufferType == VERTEXT_ARRAY_BUFFER) {
		currentVBO = bufferID;
	}
	else if (bufferType == ELEMENT_VERTEXT_ARRAY_BUFFER) {
		currentEBO = bufferID;
	}
	else assert(false);
}

void ZGPU::BufferUpData(const uint32_t& bufferType, size_t dataSize, void* data)
{
	uint32_t bufferID = 0;
	if (bufferType == VERTEXT_ARRAY_BUFFER) {
		bufferID = currentVBO;
	}
	else if (bufferType == ELEMENT_VERTEXT_ARRAY_BUFFER) {
		bufferID = currentEBO;
	}
	else assert(false);
	auto iter = mVBOMap.find(bufferID);
	if (iter == mVBOMap.end()) assert(false);	//如果所对应的ID是空的直接报错
	//把数据塞到对应的BufferObject里面
	iter->second->SetBufferData(dataSize, data);
}

void ZGPU::BingdingVertexArray(const uint32_t& vaoID)
{
	currentVAO = vaoID;
}

void ZGPU::VertexAttributePointer(const uint32_t& bindingKey, const uint32_t& itemSize, const uint32_t& stride, const uint32_t& offset)
{
	auto iter = mVAOMap.find(currentVAO);
	if (iter == mVAOMap.end()) assert(false);	//如果与ID对应的内存是空的直接报错
	iter->second->Set(bindingKey, currentVBO, itemSize, stride, offset);
}

void ZGPU::PrintVAO(const uint32_t& inVAO)
{
	auto iter = mVAOMap.find(inVAO);
	if (iter == mVAOMap.end()) assert(false);	//如果与ID对应的内存是空的直接报错
	auto vaoMap = iter->second->GetBindingMap();
	auto iter2 = vaoMap.begin();
	for (; iter2 != vaoMap.end(); iter2++) {
		std::cout << "VBOID: " << iter2->second.mVBOID << std::endl;
		std::cout << "mItemSize: " << iter2->second.mItemSize << std::endl;
		std::cout << "mStride: " << iter2->second.mStride << std::endl;
		std::cout << "mOffset: " << iter2->second.mOffset << std::endl;
		std::cout << std::endl;
	}
}

void ZGPU::UseShaderProgram(ShaderBase* inShader)
{
	mShader = inShader;
}

void ZGPU::DrawElement(const uint32_t& drawMode, const uint32_t& first, const uint32_t& count)
{
	if (currentVAO == 0 || mShader == nullptr || count == 0) return;
	auto vao_iter = mVAOMap.find(currentVAO);
	if (vao_iter == mVAOMap.end()) {
		std::cerr << "current VAO is not valid" << std::endl;
		return;
	}
	VertexArrayObject* vao = vao_iter->second;
	auto bindingMap = vao->GetBindingMap();

	auto ebo_iter = mVBOMap.find(currentEBO);
	if (ebo_iter == mVBOMap.end()) {
		std::cerr << "current EBO is not valid" << std::endl;
		return;
	}
	const BufferObject* ebo = ebo_iter->second;
	std::vector<VsOutPoint> outvsPoints{};
	VertexShaderStage(outvsPoints, vao, ebo, first, count);
	if (outvsPoints.empty()) {
		//std::cerr << "VertexShaderStage outPoints is empty" << std::endl;
		return;
	}

	//进入NDC之前进行剪裁
	std::vector<VsOutPoint> clipOutputs{};
	Clipper::DoClipSpace(drawMode, outvsPoints, clipOutputs);
	if (clipOutputs.empty()) {
		//std::cerr << "DoClipSpace clipOutputs is empty" << std::endl;
		return;
	}

	//进入NDC处理阶段
	for (auto& pt : clipOutputs) {
		PerspectiveDivision(pt);
	}

	//屏幕映射处理阶段
	for (auto& pt : clipOutputs) {
		ScreenMapping(pt);
	}

	//光栅化处理阶段(离散处理)
	std::vector<VsOutPoint> rasterOutPoints;
	Raster::Rasterize(rasterOutPoints, drawMode, clipOutputs);
	if (rasterOutPoints.empty())return;

	//颜色输出阶段
	FsOutPoint fsPoints;
	uint32_t pixelPos = 0;
	for (uint32_t i = 0; i < rasterOutPoints.size(); i++) {
		mShader->FragmentShader(rasterOutPoints[i], fsPoints);
		pixelPos = fsPoints.mPosition.Y * mFrameBuffer->mWidth + fsPoints.mPosition.X;
		mFrameBuffer->mColorBuffer[pixelPos] = fsPoints.mColor;
	}
}

void ZGPU::VertexShaderStage(std::vector<VsOutPoint>& outvsPoints, const VertexArrayObject* vao, const BufferObject* vbo, const uint32_t first, const uint32_t count)
{
	auto bindingmap = vao->GetBindingMap();
	byte* indicesdata = vbo->GetBufferData();
	uint32_t index = 0;
	for (uint32_t i = first; i < first + count; i++) {
		size_t indicesOffset = i * sizeof(uint32_t);
		memcpy(&index, indicesdata + indicesOffset, sizeof(uint32_t));

		VsOutPoint outPt = mShader->VertexShader(bindingmap, mVBOMap, index);
		outvsPoints.push_back(outPt);
	}
}

void ZGPU::PerspectiveDivision(VsOutPoint& vsPoints)
{
	float oneOverlie = 1.0f / vsPoints.mPosition.W;
	vsPoints.mPosition *= oneOverlie;
	vsPoints.mPosition.W = 1.0f;
	//修剪毛刺
}

void ZGPU::ScreenMapping(VsOutPoint& vsPoints)
{
	vsPoints.mPosition = mScreenMatrix * vsPoints.mPosition;
}
