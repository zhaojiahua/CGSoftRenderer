#pragma once
#include "../global/base.h"
#include "dataStruct.h"

/*
自定义一个光栅类(光栅器),专门做一些光栅化的操作
对外提供静态函数接口,传入离散化的图元点,返回光栅化后的点组
*/

class Raster
{
public:
	Raster() {};
	~Raster() {};

	static void RasterizeLine_Brensenham(std::vector<VsOutPoint>& outScreenPoints, const VsOutPoint& startPoint, const VsOutPoint& endPoint);
	static void RasterizeTriangle(std::vector<VsOutPoint>& outps, const VsOutPoint& p1, const VsOutPoint& p2, const VsOutPoint& p3);
	// 差值计算两点中间点的属性
	static void InterpolantLine(const VsOutPoint& startPoint, const VsOutPoint& endPoint, VsOutPoint& targetPoint);
	//差值计算三个点中间点的属性
	static void InterpolantTriangle(const VsOutPoint& p1, const VsOutPoint& p2, const VsOutPoint& p3, VsOutPoint& tp);

	//lerp函数(weight值越小越靠近color1,越大越靠近color2)
	static ZRGBA LerpRGBA(const ZRGBA& inc1, const ZRGBA& inc2, float inweight);
};