#pragma once
#include "../global/base.h"
#include <Windows.h>

//自定义一个窗体类
class ZApplication
{
	//单例模式下向外部暴露一个单实例指针
	static ZApplication* mInstance;

	bool mAlive = true;		//为true表示当前窗体仍然在继续显示,程序一直在跑;为false表示窗体已经被关闭,程序需要退出

	HINSTANCE mWinInstance;		//程序实例句柄
	WCHAR mWinClassName[100] = L"ZAPPWindow";
	HWND mHwnd;		//实例窗口句柄

	int mWidth = 800;
	int mHeight = 600;

private:
	BOOL ZCreateWindow(HINSTANCE hInstance);		//创建一个窗口实例
	ATOM ZRegisterWindowClass(HINSTANCE hInstance);		//向Windows注册此窗口类

public:
	static ZApplication* GetInstance();		//获取单个实例

	ZApplication();		//构造函数
	~ZApplication();		//析构函数

	//初始化窗体(完成窗体类注册,一个窗体实例的创建以及显示)
	bool InitZApplication(HINSTANCE hInstance, const uint32_t& width = 800, const uint32_t& height = 600);

	//托管wndProc捕获的消息并进行处理
	void HandleMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	//每一帧/每一个循环都要调用,以捕获窗体的消息并分发
	bool peekMessage();
};