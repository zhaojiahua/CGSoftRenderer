#pragma once
#include <Windows.h>
#include "iostream"
#include "Functions/AddFun.h"
#include "application/application.h"
#include "GPU/gpu.h"

#pragma comment(linker,"/subsystem:console /entry:wWinMainCRTStartup")



//准备绘制三角的点和两张贴图(准备工作)
ZImage* image01 = ZImage::CreateZImage("assets/images/tx02.png");
ZImage* image02 = ZImage::CreateZImage("assets/images/opencv.png");
ZScrPoint point1(100, 100, ZRGBA(255, 0, 0, 255), math::vec2f(0.0f, 0.0f));
ZScrPoint point2(400, 100, ZRGBA(0, 255, 0, 255), math::vec2f(1.0f, 0.0f));
ZScrPoint point3(250, 600, ZRGBA(0, 0, 255, 255), math::vec2f(0.5f, 1.0f));
ZScrPoint bpoint1(400, 100, ZRGBA(255, 0, 0, 255), math::vec2f(0.0f, 0.0f));
ZScrPoint bpoint2(600, 100, ZRGBA(0, 255, 0, 255), math::vec2f(1.0f, 0.0f));
ZScrPoint bpoint3(500, 600, ZRGBA(0, 0, 255, 255), math::vec2f(0.5f, 1.0f));

math::vec4f ppos1(-1.5f, 0.0f, 0.0f, 1.0f);
math::vec4f ppos2(1.5f, 0.0f, 0.0f, 1.0f);
math::vec4f ppos3(0.0f, 2.0f, 0.0f, 1.0f);

math::Mat4f modelMatrix;
math::Mat4f viewMatrix;
math::Mat4f perpectiveMatrix = math::Perspective(60.0f, (float)ZApp->GetWidth() / (float)ZApp->GetHeight(), 0.1f, 100.0f);
math::Mat4f screenMatrix = math::ScreenMatrix<float>(ZApp->GetWidth(), ZApp->GetHeight());
//transform模型(实际上就是对这三个点进行操作)
float cameraPos = 5.0f;	//相机初始位置
float angle = 0.0f;	//模型开始旋转的初始角度
void transform() {
	angle += 0.01f;	//让模型每帧旋转0.01f弧度
	cameraPos += 0.01f;	//让相机每帧后退0.01单位

	//模型变换
	modelMatrix = math::Rotate(math::Mat4f(1.0f), angle, math::vec3f(0.0f, 1.0f, 0.0f));		//M矩阵
	
	//相机后退
	auto cameraModelMatrix = math::Translate(math::Mat4f(1.0f), math::vec3f(0.0f, 0.0f, cameraPos));
	viewMatrix = math::Inverse(cameraModelMatrix);

	//MVP矩阵
	auto mvpMatrix = perpectiveMatrix * viewMatrix * modelMatrix;
	
	//用MVP矩阵对三个顶点进行同时操作
	auto newppos1 = mvpMatrix * ppos1;
	auto newppos2 = mvpMatrix * ppos2;
	auto newppos3 = mvpMatrix * ppos3;
	newppos3.print();
	////透视除法到NDC空间
	newppos1 /= newppos1.W;
	newppos2 /= newppos2.W;
	newppos3 /= newppos3.W;

	//通过屏幕空间矩阵变换到屏幕空间
	auto screenppos1 = screenMatrix * newppos1;
	auto screenppos2 = screenMatrix * newppos2;
	auto screenppos3 = screenMatrix * newppos3;


	//将屏幕空间位置信息传递给原始的三个点
	point1.X = screenppos1.X;
	point1.Y = screenppos1.Y;

	point2.X = screenppos2.X;
	point2.Y = screenppos2.Y;

	point3.X = screenppos3.X;
	point3.Y = screenppos3.Y;

}


//改变UV值实现跑马灯的效果
void ChangeUV(float uvSpeed) {
	point1.uv.X += uvSpeed;
	point2.uv.X += uvSpeed;
	point3.uv.X += uvSpeed;
	bpoint1.uv.Y += uvSpeed;
	bpoint2.uv.Y += uvSpeed;
	bpoint3.uv.Y += uvSpeed;
}

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
	Sgl->DrawZImage(image01);
	Sgl->DrawZImage(image02, 150);

	ChangeUV(0.01);
	Sgl->UVwrap = TEXTRUE_WRAP_MIRROR;
	//绘制带UV信息的三角形
	Sgl->SetTextrue(image01);
	Sgl->bUseBilinearity = true;
	//旋转模型并移动相机
	transform();
	Sgl->DrawTriangle(point1, point2, point3);
	////绘制带UV信息的三角形(使用双线性差值)
	//Sgl->bUseBilinearity = true;
	//Sgl->DrawTriangle(bpoint1, bpoint2, bpoint3);
	
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

