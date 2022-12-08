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


//当UV值超出1的边界时采用的采样的方式
#define TEXTRUE_WRAP_REPEAT 0
#define TEXTRUE_WRAP_MIRROR 1

//可供选择的Buffer类型
#define VERTEXT_ARRAY_BUFFER 0	//与顶点属性相关的VBO
#define ELEMENT_VERTEXT_ARRAY_BUFFER	1 //与顶点索引相关的EBO

//提供顶点之后的光栅化模式(比如画线或者画面)
#define DRAW_LINES 0
#define DRAW_TRIANGLES 1

#define FRONT_FACE 0	//将正方向的面剪裁掉
#define BACK_FACE 1	//将反方向的面剪裁掉
#define CW_FRONT_FACE 0	//将顺时针方向设置为正面
#define CCW_FRONT_FACE 1	//将逆时针方向设置为正面

#define CULL_FACE 1	//开启或者关闭cull Face的功能
#define DEPTH_TEST 2	//开启或者关闭深度检测的功能
#define COLORBLENDING	3	//开启或关闭颜色混合功能
#define BILINEARITY 4	//开启或关闭双线性插值读取图片

#define DEPTH_LESS 0	//深度检测的方式,绘制小于当前深度值的点
#define DEPTH_GREATER 1	//深度检测的方式,绘制大于当前深度值的点

#define TEXTURE_FILTER 0		//设置纹理采样的方式
#define TEXTURE_WRAP_U 1		//设置纹理U向的wrap方式
#define TEXTURE_WRAP_V 2		//设置纹理V向的wrap方式

#define TEXTURE_FILTER_NEAREST 0	//nearest图片采样方法
#define TEXTURE_FILTER_LINEAR 1	//双线性插值图片采样方法

#define TEXTURE_WRAP_REPEAT 0
#define TEXTURE_WRAP_MIRROR 1