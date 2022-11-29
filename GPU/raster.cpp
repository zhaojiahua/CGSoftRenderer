#pragma once
#include "raster.h"
#include "../math/math.h"

void Raster::RasterizeLine_Brensenham(std::vector<ZScrPoint>& outScreenPoints, const ZScrPoint& startPoint, const ZScrPoint& endPoint)
{
	/*
	首先要满足 0<斜率k<1且起始点位置在终止点的左侧
	也就是满足这几个条件: 
	endPoint.X>startPoint.X
	endPoint.Y>startPoint.Y
	endPoint.X>endPoint.Y
	如果不满足就要创造满足的条件
	*/
	ZScrPoint start = startPoint;
	ZScrPoint end = endPoint;

	//1 保证x方向是从小到大的
	if (start.X > end.X) {
		auto tmp = start;
		start = end;
		end = tmp;
	}

	outScreenPoints.push_back(start);

	//2 保证y方向也是从小到大，如果需要翻转，必须记录
	bool flipY = false;
	if (start.Y > end.Y) {
		start.Y *= -1.0f;
		end.Y *= -1.0f;
		flipY = true;
	}

	//3 保证斜率在0-1之间，如果需要调整，必须记录
	int32_t deltaX = end.X - start.X;
	int32_t deltaY = end.Y - start.Y;

	bool swapXY = false;
	if (deltaX < deltaY) {
		std::swap(start.X, start.Y);
		std::swap(end.X, end.Y);
		std::swap(deltaX, deltaY);
		swapXY = true;
	}

	//4 brensenham
	int32_t currentX = start.X;
	int32_t currentY = start.Y;

	int32_t resultX = 0;
	int32_t resultY = 0;

	ZScrPoint currentPoint;
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
		currentPoint.X = resultX;
		currentPoint.Y = resultY;

		InterpolantLine(start, end, currentPoint);

		outScreenPoints.push_back(currentPoint);
	}
}

void Raster::RasterizeTriangle(std::vector<ZScrPoint>& outps, const ZScrPoint& p1, const ZScrPoint& p2, const ZScrPoint& p3)
{
	/*
	根据二维向量的叉积判断像素点是否落在三角形的区域内
	如果落在三角形内部然后用三角形重心插值法去求这个像素点的属性值
	*/
	//首先得到三角形的外接矩形
	int32_t xmin = std::min(p1.X, std::min(p2.X, p3.X));
	int32_t ymin = std::min(p1.Y, std::min(p2.Y, p3.Y));
	int32_t xmax = std::max(p1.X, std::max(p2.X, p3.X));
	int32_t ymax = std::max(p1.Y, std::max(p2.Y, p3.Y));
	//然后遍历这个矩形内的所有像素点
	for (int32_t tx = xmin; tx < xmax; ++tx) {
		for (int32_t ty = ymin; ty < ymax; ++ty) {
			math::vec2i tv1(p1.X - tx, p1.Y - ty);
			math::vec2i tv2(p2.X - tx, p2.Y - ty);
			math::vec2i tv3(p3.X - tx, p3.Y - ty);

			float crossresult1 = math::cross(tv1, tv2);
			float crossresult2 = math::cross(tv2, tv3);
			float crossresult3 = math::cross(tv3, tv1);

			bool allnagtive = crossresult1 < 0 && crossresult2 < 0 && crossresult3 < 0;
			bool allpositive = crossresult1 > 0 && crossresult2 > 0 && crossresult3 > 0;

			if (allnagtive || allpositive) {
				ZScrPoint tempPoint(tx, ty);
				InterpolantTriangle(p1, p2, p3, tempPoint);
				outps.push_back(tempPoint);
			}
		}
	}


}

void Raster::InterpolantLine(const ZScrPoint& startPoint, const ZScrPoint& endPoint, ZScrPoint& targetPoint)
{
	float_t weight = 1.0;
	if (startPoint.X != endPoint.X) {
		weight = (float_t)(targetPoint.X - startPoint.X) / (float_t)(endPoint.X - startPoint.X);
	}
	else if (startPoint.Y != endPoint.Y) {
		weight = (float_t)(targetPoint.Y - startPoint.Y) / (float_t)(endPoint.Y - startPoint.Y);
	}
	targetPoint.color.zR = static_cast<byte>  ((1 - weight) * startPoint.color.zR + weight * endPoint.color.zR);
	targetPoint.color.zG = static_cast<byte> ((1 - weight) * startPoint.color.zG + weight * endPoint.color.zG);
	targetPoint.color.zB = static_cast<byte> ((1 - weight) * startPoint.color.zB + weight * endPoint.color.zB);
	targetPoint.color.zA = static_cast<byte> ((1 - weight) * startPoint.color.zA + weight * endPoint.color.zA);
}

void Raster::InterpolantTriangle(const ZScrPoint& p1, const ZScrPoint& p2, const ZScrPoint& p3, ZScrPoint& tp)
{
	//三角形重心差值法
	//首先求出这个三角形的面积
	math::vec2i p1p2(p2.X - p1.X, p2.Y - p1.Y);
	math::vec2i p1p3(p3.X - p1.X, p3.Y - p1.Y);
	int32_t tri_area = std::abs(math::cross(p1p2, p1p3));	//三角形面积的2倍(都是整形数据的运算)

	//再求出三个小三角形面积
	math::vec2i tpp1(p1.X - tp.X, p1.Y - tp.Y);
	math::vec2i tpp2(p2.X - tp.X, p2.Y - tp.Y);
	math::vec2i tpp3(p3.X - tp.X, p3.Y - tp.Y);

	int32_t tri_area1 = std::abs(math::cross(tpp2, tpp3));
	int32_t tri_area2 = std::abs(math::cross(tpp1, tpp3));
	int32_t tri_area3 = std::abs(math::cross(tpp1, tpp2));

	//根据三角面积比例计算出三个点的权重值(涉及到除法的要引入浮点数据运算)
	float weight1 = static_cast<float>(tri_area1) / tri_area;
	float weight2 = static_cast<float>(tri_area2) / tri_area;
	float weight3 = static_cast<float>(tri_area3) / tri_area;

	//最后根据这个权重值差值计算出中间点的属性
	tp.color.zA = static_cast<byte>(weight1 * static_cast<float>(p1.color.zA) + weight2 * static_cast<float>(p2.color.zA) + weight3 * static_cast<float>(p3.color.zA));
	tp.color.zR = static_cast<byte>(weight1 * static_cast<float>(p1.color.zR) + weight2 * static_cast<float>(p2.color.zR) + weight3 * static_cast<float>(p3.color.zR));
	tp.color.zG = static_cast<byte>(weight1 * static_cast<float>(p1.color.zG) + weight2 * static_cast<float>(p2.color.zG) + weight3 * static_cast<float>(p3.color.zG));
	tp.color.zB = static_cast<byte>(weight1 * static_cast<float>(p1.color.zB) + weight2 * static_cast<float>(p2.color.zB) + weight3 * static_cast<float>(p3.color.zB));
}
