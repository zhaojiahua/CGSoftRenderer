#pragma once
/////////////////////////////这里面存储一些宏和常量和常用的库文件

#include <iostream>
#include <vector>
#include <map>
#include <cmath>
#include <assert.h>	//添加断言的宏,若表达式为真则继续,若表达式为假则终止程序

#define PI										3.14159265358979323					//定义圆周率的精度
#define DEGTORAD(theta)				(0.01745329251994329*(theta))		//角度转弧度
#define FRACTION(v)						((v)-(int)(v))										//取值浮点数的小数部分

//获取单例类的类实例
#define Sgl	ZGPU::GetZGPUInstance()
#define ZApp ZApplication::GetInstance()

using byte = unsigned char;		//定义一比特就是一字节

struct ZRGBA
{
	byte zB;
	byte zG;
	byte zR;
	byte zA;
	ZRGBA(byte r = 255, byte g = 255, byte b = 255, byte a = 255)
	{
		zB = b;
		zG = g;
		zR = r;
		zA = a;
	}
};

//屏幕上的像素点
struct ZScrPoint
{
	ZScrPoint() : X(0), Y(0), color(255, 255, 255, 255) {};
	ZScrPoint(int32_t inX, int32_t inY, ZRGBA inColor = ZRGBA(255, 255, 255, 255)) :X(inX), Y(inY), color(inColor) {};
	int32_t X;
	int32_t Y;
	ZRGBA color;
	void operator = (ZScrPoint  inPoint);
};