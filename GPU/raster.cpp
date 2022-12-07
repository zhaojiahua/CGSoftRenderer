#pragma once
#include "raster.h"
#include "../math/math.h"

void Raster::RasterizeLine_Brensenham(std::vector<VsOutPoint>& outScreenPoints, const VsOutPoint& startPoint, const VsOutPoint& endPoint)
{
	/*
	首先要满足 0<斜率k<1且起始点位置在终止点的左侧
	也就是满足这几个条件: 
	endPoint.X>startPoint.X
	endPoint.Y>startPoint.Y
	endPoint.X>endPoint.Y
	如果不满足就要创造满足的条件
	*/
	VsOutPoint start = startPoint;
	VsOutPoint end = endPoint;

	//1 保证x方向是从小到大的
	if (start.mPosition.X > end.mPosition.X) {
		auto tmp = start;
		start = end;
		end = tmp;
	}

	outScreenPoints.push_back(start);

	//2 保证y方向也是从小到大，如果需要翻转，必须记录
	bool flipY = false;
	if (start.mPosition.Y > end.mPosition.Y) {
		start.mPosition.Y *= -1;
		end.mPosition.Y *= -1;
		flipY = true;
	}

	//3 保证斜率在0-1之间，如果需要调整，必须记录
	int32_t deltaX = end.mPosition.X - start.mPosition.X;
	int32_t deltaY = end.mPosition.Y - start.mPosition.Y;

	bool swapXY = false;
	if (deltaX < deltaY) {
		std::swap(start.mPosition.X, start.mPosition.Y);
		std::swap(end.mPosition.X, end.mPosition.Y);
		std::swap(deltaX, deltaY);
		swapXY = true;
	}

	//4 brensenham
	int32_t currentX = start.mPosition.X;
	int32_t currentY = start.mPosition.Y;

	int32_t resultX = 0;
	int32_t resultY = 0;

	VsOutPoint currentPoint;
	int32_t p = 2 * deltaY - deltaX;

	for (int32_t i = 0; i < deltaX; ++i) {
		if (p >= 0) {
			currentY += 1;
			p -= 2 * deltaX;
		}

		currentX += 1;
		p += 2 * deltaY;

		//处理新xy，flip and swap

		resultX = currentX;
		resultY = currentY;
		if (swapXY) {
			std::swap(resultX, resultY);
		}

		if (flipY) {
			resultY *= -1;
		}

		//产生新顶点
		currentPoint.mPosition.X = resultX;
		currentPoint.mPosition.Y = resultY;

		InterpolantLine(start, end, currentPoint);

		outScreenPoints.push_back(currentPoint);
	}
}

void Raster::RasterizeTriangle(std::vector<VsOutPoint>& outps, const VsOutPoint& p1, const VsOutPoint& p2, const VsOutPoint& p3)
{
	/*
	根据二维向量的叉积判断像素点是否落在三角形的区域内
	如果落在三角形内部然后用三角形重心插值法去求这个像素点的属性值
	*/
	//首先得到三角形的外接矩形
	int32_t xmin = std::min(p1.mPosition.X, std::min(p2.mPosition.X, p3.mPosition.X));
	int32_t ymin = std::min(p1.mPosition.Y, std::min(p2.mPosition.Y, p3.mPosition.Y));
	int32_t xmax = std::max(p1.mPosition.X, std::max(p2.mPosition.X, p3.mPosition.X));
	int32_t ymax = std::max(p1.mPosition.Y, std::max(p2.mPosition.Y, p3.mPosition.Y));
	//然后遍历这个矩形内的所有像素点
	for (int32_t tx = xmin; tx < xmax; ++tx) {
		for (int32_t ty = ymin; ty < ymax; ++ty) {
			math::vec2i tv1(p1.mPosition.X - tx, p1.mPosition.Y - ty);
			math::vec2i tv2(p2.mPosition.X - tx, p2.mPosition.Y - ty);
			math::vec2i tv3(p3.mPosition.X - tx, p3.mPosition.Y - ty);

			float crossresult1 = math::cross(tv1, tv2);
			float crossresult2 = math::cross(tv2, tv3);
			float crossresult3 = math::cross(tv3, tv1);

			bool allnagtive = crossresult1 < 0 && crossresult2 < 0 && crossresult3 < 0;
			bool allpositive = crossresult1 > 0 && crossresult2 > 0 && crossresult3 > 0;
			
			if (allnagtive || allpositive) {
				VsOutPoint tempPoint;
				tempPoint.mPosition.X = tx;
				tempPoint.mPosition.Y = ty;
				InterpolantTriangle(p1, p2, p3, tempPoint);
				outps.push_back(tempPoint);
			}
		}
	}
}

void Raster::InterpolantLine(const VsOutPoint& startPoint, const VsOutPoint& endPoint, VsOutPoint& targetPoint)
{
	float_t weight = 1.0;
	if (startPoint.mPosition.X != endPoint.mPosition.X) {
		weight = (float_t)(targetPoint.mPosition.X - startPoint.mPosition.X) / (float_t)(endPoint.mPosition.X - startPoint.mPosition.X);
	}
	else if (startPoint.mPosition.Y != endPoint.mPosition.Y) {
		weight = (float_t)(targetPoint.mPosition.Y - startPoint.mPosition.Y) / (float_t)(endPoint.mPosition.Y - startPoint.mPosition.Y);
	}
	//颜色属性差值
	targetPoint.mColor = math::Lerp(startPoint.mColor, endPoint.mColor, weight);
	//uv属性差值
	targetPoint.mUV = math::Lerp(startPoint.mUV, endPoint.mUV, weight);
}

void Raster::InterpolantTriangle(const VsOutPoint& p1, const VsOutPoint& p2, const VsOutPoint& p3, VsOutPoint& tp)
{
	//三角形重心差值法
	//首先求出这个三角形的面积
	math::vec2i p1p2(p2.mPosition.X - p1.mPosition.X, p2.mPosition.Y - p1.mPosition.Y);
	math::vec2i p1p3(p3.mPosition.X - p1.mPosition.X, p3.mPosition.Y - p1.mPosition.Y);
	int32_t tri_area = std::abs(math::cross(p1p2, p1p3));	//三角形面积的2倍(都是整形数据的运算)

	//再求出三个小三角形面积
	math::vec2i tpp1(p1.mPosition.X - tp.mPosition.X, p1.mPosition.Y - tp.mPosition.Y);
	math::vec2i tpp2(p2.mPosition.X - tp.mPosition.X, p2.mPosition.Y - tp.mPosition.Y);
	math::vec2i tpp3(p3.mPosition.X - tp.mPosition.X, p3.mPosition.Y - tp.mPosition.Y);

	int32_t tri_area1 = std::abs(math::cross(tpp2, tpp3));
	int32_t tri_area2 = std::abs(math::cross(tpp1, tpp3));
	//int32_t tri_area3 = std::abs(math::cross(tpp1, tpp2));

	//根据三角面积比例计算出三个点的权重值(涉及到除法的要引入浮点数据运算)
	float weight1 = static_cast<float>(tri_area1) / tri_area;
	float weight2 = static_cast<float>(tri_area2) / tri_area;
	float weight3 = 1.0f - weight1 - weight2;

	//最后根据这个权重值差值计算出中间点的颜色属性
	tp.mColor = math::Lerp(p1.mColor,p2.mColor,p3.mColor,weight1,weight2,weight3);
	//最后根据这个权重值差值计算出中间点的UV属性
	tp.mUV = math::Lerp(p1.mUV, p2.mUV, p3.mUV, weight1, weight2, weight3);
}

ZRGBA Raster::LerpRGBA(const ZRGBA& inc1, const ZRGBA& inc2, float inweight)
{
	ZRGBA tempresult;
	tempresult.zA = (1 - inweight) * inc1.zA + inweight * inc2.zA;
	tempresult.zR = (1 - inweight) * inc1.zR + inweight * inc2.zR;
	tempresult.zG = (1 - inweight) * inc1.zG + inweight * inc2.zG;
	tempresult.zB = (1 - inweight) * inc1.zB + inweight * inc2.zB;
	return tempresult;
}

void Raster::Rasterize(std::vector<VsOutPoint>& outputPoints, const uint32_t& drawMode, const std::vector<VsOutPoint>& invsPoints)
{
	if (drawMode == DRAW_LINES) {
		for (uint32_t i = 0; i < invsPoints.size(); i+=2) {
			RasterizeLine_Brensenham(outputPoints, invsPoints[i], invsPoints[i + 1]);
		}
	}
	if (drawMode == DRAW_TRIANGLES) {
		for (uint32_t i = 0; i < invsPoints.size(); i += 3) {
			RasterizeTriangle(outputPoints, invsPoints[i], invsPoints[i + 1], invsPoints[i + 2]);
		}
	}
}
