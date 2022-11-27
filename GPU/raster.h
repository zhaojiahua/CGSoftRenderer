#pragma once
#include "../global/base.h"

/*
自定义一个光栅类(光栅器),专门做一些光栅化的操作
对外提供静态函数接口,传入离散化的图元点,返回光栅化后的点组
*/

class Raster
{
public:
	Raster() {};
	~Raster() {};

	static void RasterizeLine_Brensenham(std::vector<ZScrPoint>& outScreenPoints, const ZScrPoint& startPoint, const ZScrPoint& endPoint);
};