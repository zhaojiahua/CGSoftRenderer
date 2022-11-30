#pragma once
#include <Windows.h>
#include "iostream"
#include "Functions/AddFun.h"
#include "application/application.h"
#include "GPU/gpu.h"

#pragma comment(linker,"/subsystem:console /entry:wWinMainCRTStartup")

//准备绘制三角的点
ZScrPoint point1(100, 100, ZRGBA(255, 0, 0, 255), math::vec2f(0.0f, 0.0f));
ZScrPoint point2(400, 100, ZRGBA(0, 255, 0, 255), math::vec2f(1.0f, 0.0f));
ZScrPoint point3(250, 600, ZRGBA(0, 0, 255, 255), math::vec2f(0.5f, 1.0f));
ZScrPoint bpoint1(400, 100, ZRGBA(255, 0, 0, 255), math::vec2f(0.0f, 0.0f));
ZScrPoint bpoint2(600, 100, ZRGBA(0, 255, 0, 255), math::vec2f(1.0f, 0.0f));
ZScrPoint bpoint3(500, 600, ZRGBA(0, 0, 255, 255), math::vec2f(0.5f, 1.0f));

void Render() {
	Sgl->Clear();
	////绘制一条横线
	/*for (uint32_t i = 0; i < mWidth; ++i) {
		Sgl->DrawPoint(i, 200, ZRGBA(0, 200, 255, 255));
	}
	for (uint32_t i = 0; i < mWidth; ++i) {
		Sgl->DrawPoint(i, 100, ZRGBA(0, 255, 255, 255));
	}
	for (uint32_t i = 0; i < mWidth; ++i) {
		Sgl->DrawPoint(i, 590, ZRGBA(0, 255, 255, 255));
	}
	for (uint32_t i = 0; i < mHeight; ++i) {
		Sgl->DrawPoint(100, i, ZRGBA(0, 255, 255, 255));
	}
	for (uint32_t i = 0; i < mHeight; ++i) {
		Sgl->DrawPoint(500, i, ZRGBA(0, 255, 255, 255));
	}
	for (uint32_t i = 0; i <= mHeight; ++i) {
		Sgl->DrawPoint(500, i, ZRGBA(0, 255, 255, 255));
	}*/

	//绘制雪花噪点
	/*for (uint32_t i = 0; i < mWidth; i++)
	{
		for (uint32_t j = 0; j < mHeight; j++)
		{
			uint32_t temp = std::rand() % 255;
			Sgl->DrawPoint(i, j, ZRGBA(temp, temp, temp, temp));
		}
	}*/
	//Brensenham算法绘制直线(圆心和半径画一圈))
	/*ZScrPoint centerPoint(400, 400, ZRGBA(255, 0, 0, 255));
	int32_t raduis = 100;
	for (uint32_t i = 0; i < 36; i++) {
		double temprand = DEGTORAD(10 * i);
		int32_t tx = cos(temprand) * raduis+ centerPoint.X;
		int32_t ty = sin(temprand) * raduis + centerPoint.Y;
		Sgl->DrawLine(centerPoint, ZScrPoint(tx, ty, ZRGBA(rand() % 256, rand() % 256, rand() % 256)));
	}*/

	////绘制一个渐变的三角形
	//ZScrPoint point1(100, 100, ZRGBA(255, 0, 0, 255));
	//ZScrPoint point2(600, 100, ZRGBA(0, 255, 0, 255));
	//ZScrPoint point3(350, 500, ZRGBA(0, 0, 255, 255));
	//Sgl->DrawTriangle(point1, point2, point3);

	//绘制图片
	Sgl->bEnableBlend = true;
	ZImage* image01 = ZImage::CreateZImage("assets/images/tx02.png");
	ZImage* image02 = ZImage::CreateZImage("assets/images/opencv.png");
	Sgl->DrawZImage(image01);
	Sgl->DrawZImage(image02, 150);

	//绘制带UV信息的三角形
	Sgl->SetTextrue(image01);
	Sgl->bUseBilinearity = false;
	Sgl->DrawTriangle(point1, point2, point3);
	//绘制带UV信息的三角形(使用双线性差值)
	Sgl->bUseBilinearity = true;
	Sgl->DrawTriangle(bpoint1, bpoint2, bpoint3);
}

int main()
{
	std::cout << AddFun(1.0f, 5.3f) << std::endl;
	return 0;
}
int APIENTRY wWinMain(
	_In_ HINSTANCE hInstance,
	_In_ HINSTANCE hPreInstance,
	_In_ LPWSTR lpCmdLine,
	_In_ int nCmdShow)
{
	if (!ZApp->InitZApplication(hInstance, 800, 600))return -1;
	//Sgl->InitSurface(ZApp->GetWidth(), ZApp->GetHeight(), ZApp->GetCanvasBuffer());

	bool alive = true;
	while (alive) {
		alive = ZApp->peekMessage();
		Render();
		ZApp->Show();
	}
	return 0;
}

