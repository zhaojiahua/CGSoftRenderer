#pragma once
#include "../global/base.h"
#include "frameBuffer.h"
#include "../application/application.h"


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
	void DrawPoint(const uint32_t& inx, const uint32_t& iny, const ZRGBA& incolor);

};