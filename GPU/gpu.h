#pragma once
#include "../global/base.h"
#include "frameBuffer.h"
#include "../application/application.h"
#include "../application/image.h"


/*
class  GPU
这个类用来模拟GPU的行为和算法等
是个单例类
*/
class ZGPU
{
	static ZGPU* mZGPUInstance;
	FrameBuffer* mFrameBuffer{ nullptr };

public:
	ZGPU();
	~ZGPU();
	static ZGPU* GetZGPUInstance();	//单例模式,获取此类的实例
	
	//接受外界传入的位图指针以及窗体的宽和高
	void InitSurface(const uint32_t& inWidth, const uint32_t& inHeight, void* bufferPtr = nullptr);

	//清除画布
	void Clear();

	//传入像素位置和颜色并绘制
	void DrawPoint(const int32_t& inx, const int32_t& iny, const ZRGBA& incolor);

	//传入两个像素点绘制直线
	void DrawLine(const ZScrPoint& startPoint, const ZScrPoint& endPoint);

	//传入三个像素点绘制一个三角形
	void DrawTriangle(const ZScrPoint& p1, const ZScrPoint& p2, const ZScrPoint& p3);

	//传入ZImage数据绘制图像
	void DrawZImage(const ZImage* inImg);

};