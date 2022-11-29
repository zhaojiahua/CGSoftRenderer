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
	static void RasterizeTriangle(std::vector<ZScrPoint>& outps, const ZScrPoint& p1, const ZScrPoint& p2, const ZScrPoint& p3);
	// 差值计算两点中间点的属性
	static void InterpolantLine(const ZScrPoint& startPoint, const ZScrPoint& endPoint, ZScrPoint& targetPoint);
	//差值计算三个点中间点的属性
	static void InterpolantTriangle(const ZScrPoint& p1, const ZScrPoint& p2, const ZScrPoint& p3, ZScrPoint& tp);
};