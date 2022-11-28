#include "raster.h"

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