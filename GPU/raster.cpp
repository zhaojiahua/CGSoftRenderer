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
	ZScrPoint p0 = startPoint;
	ZScrPoint p1 = endPoint;

	//首先把p0添加进去
	outScreenPoints.push_back(p0);

	//如果不满足第一个条件直接对调两个点的位置
	if (endPoint.X < startPoint.X) {
		auto temp = p0;
		p0 = p1;
		p1 = temp;
	}
	//如果不满足第二个条件就以x轴对称过去(也就是Y值全部取负)
	bool flipY = false;
	if (endPoint.Y < startPoint.Y) {
		p0.X = startPoint.X;
		p0.Y = -startPoint.Y;
		p1.X = endPoint.X;
		p1.Y = -endPoint.Y;
		flipY = true;
	}
	//如果不满足第三个条件就以y=x为对称轴对称过去(也就是xy值对调)
	bool swapXY = false;
	if (endPoint.X < endPoint.Y) {
		p0.X= startPoint.Y;
		p0.Y = startPoint.X;
		p1.X= endPoint.Y;
		p1.Y = endPoint.X;
		swapXY = true;
	}
	//Brensenham算法
	int32_t deltaX = p1.X - p0.X;
	int32_t deltaY = p1.Y - p0.Y;
	int32_t preX = p0.X;
	int32_t preY = p0.Y;
	int32_t curX = p0.X + 1;
	int32_t curY = p0.Y;		//主要就是计算curY,先给一个初始值
	int32_t p = 2 * deltaY - deltaX;
	while (curX <= p1.X) {
		if (p>=0) {
			curY = preY + 1;
			p = p - 2 * deltaX;
		}
		else curY = preY;
		p = p + 2 * deltaY;
		ZScrPoint tempPoint(curX, curY);
		//往数组里面添加点的时候再根据flipY和swapXY的值进行翻转(注意翻转顺序要一致)
		if (swapXY) {
			uint32_t tp = tempPoint.Y;
			tempPoint.Y = tempPoint.X;
			tempPoint.X = tp;
		}
		if (flipY) {
			tempPoint.Y = -tempPoint.Y;
		}
		outScreenPoints.push_back(tempPoint);
		preX = curX;
		preY = curY;
		curX++;
	}
}
