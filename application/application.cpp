#pragma once
#include "application.h"
#include "strsafe.h"
#include "../GPU/gpu.h"

#define DNUM		1000

ZApplication* ZApplication::mInstance = nullptr;

LRESULT CALLBACK Wndproc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	ZApplication::GetInstance()->HandleMessage(hWnd, message, wParam, lParam);
	return(DefWindowProc(hWnd, message, wParam, lParam));
}

BOOL ZApplication::ZCreateWindow(HINSTANCE hInstance)
{
	mWinInstance = hInstance;

	auto dwExStyle = WS_EX_APPWINDOW;		//窗口扩展样式:当窗口可见时，将一个顶层窗口放置到任务条上
	auto dwStyle = WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;		//窗口样式:拥有普通程序主窗口的所有特点，必须有标题且有边框 | 被兄弟窗口挡住区域不绘制 | 被子窗口遮挡住的区域不绘制

	windowRect.left = 0L;
	windowRect.top = 0L;
	windowRect.right = (long)mWidth;
	windowRect.bottom = (long)mHeight;
	AdjustWindowRectEx(&windowRect, dwStyle, FALSE, dwExStyle);

	mHwnd = CreateWindowW(mWinClassName, (LPCWSTR)"ComputeGraphicLearning", dwStyle, 500, 500, windowRect.right, windowRect.bottom,nullptr, nullptr, hInstance, nullptr);	//调用windowAPI创建窗口
	if (!mHwnd)return FALSE;
	ShowWindow(mHwnd, true);
	UpdateWindow(mHwnd);
	return TRUE;
}

ATOM ZApplication::ZRegisterWindowClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wndClass;
	wndClass.cbSize = sizeof(WNDCLASSEXW);
	wndClass.style= CS_OWNDC;	//水平/垂直大小发生变化不重绘窗口
	wndClass.lpfnWndProc = Wndproc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = hInstance;
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);//应用程序图标,即任务栏的大图标
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);//鼠标图标
	wndClass.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);//窗口背景色
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName= mWinClassName;//窗口类名
	wndClass.hIconSm = LoadIcon(NULL, IDI_WINLOGO);//窗口标题图标

	return RegisterClassExW(&wndClass);
}

ZApplication* ZApplication::GetInstance()
{
	if (mInstance == nullptr) {
		mInstance = new ZApplication();
	}
	return mInstance;
}

ZApplication::ZApplication(){}
ZApplication::~ZApplication(){}

bool ZApplication::InitZApplication(HINSTANCE hInstance, const uint32_t& width, const uint32_t& height)
{
	mWidth = width;
	mHeight = height;

	ZRegisterWindowClass(hInstance);
	if (!ZCreateWindow(hInstance)) return false;

	///////////////获取显示器的分辨率
	cxClient = GetSystemMetrics(SM_CXSCREEN);
	cyClient = GetSystemMetrics(SM_CYSCREEN);

	///////////////初始化hdc画布
	/*
	dc device context设备上下文对象
	每个窗口都有自己对应的设备区域映射 hdc
	在内存中创建一个与本窗口hdc兼容的CanvasDC,用于绘制图像(将绘制完的完整图像一并拷贝之hdc)
	*/
	hdc = GetDC(mHwnd);
	mCanvasDC = CreateCompatibleDC(hdc);
	//创建位图之前要先构建位图的信息
	BITMAPINFO bmpInfo{};
	bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmpInfo.bmiHeader.biWidth = mWidth;
	bmpInfo.bmiHeader.biHeight = mHeight;
	bmpInfo.bmiHeader.biPlanes = 1;
	bmpInfo.bmiHeader.biBitCount = 32;
	bmpInfo.bmiHeader.biCompression = BI_RGB;	//实际上存储的是BGRA
	//创建于mhMem兼容的位图内存,并让mCanvasBuffer指向它(DIB表示这种与设备无关的位图文件)
	mhBmp = CreateDIBSection(mCanvasDC, &bmpInfo, DIB_RGB_COLORS, (void**)&mCanvasBuffer, 0, 0);
	//一个设备可以创建多个位图,选择表示激活位图,拷贝的位图
	SelectObject(mCanvasDC, mhBmp);
	memset(mCanvasBuffer,0,mWidth*mHeight*4);	//初始化的时候清空CanvasDc的buffer
	Sgl->InitSurface(mWidth, mHeight, mCanvasBuffer);	//构造FrameBuffer
	return true;
}

void ZApplication::HandleMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	RECT clientrect;
	GetClientRect(hWnd, &clientrect);
	static TCHAR szbuffer[128];
	StringCchPrintf(szbuffer, 128, TEXT("resolution ratio: %d * %d px"), cxClient, cyClient);
	static int cxClient, cyClient;
	static POINT bserPt[4];	//这种公用的变量一定要放在全局区
	switch (message)
	{
	case WM_CLOSE: {
		//if (MessageBox(hWnd, TEXT("are you sure"), TEXT("Confirm"), MB_YESNO) == 6) DestroyWindow(hWnd);//此处销毁窗体,会自动发出WM_DESTROY
		//else	return;
		return;
	}
	case WM_SIZE: {
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);
	}
	case WM_LBUTTONDOWN: {
		StringCchPrintf(szbuffer, lstrlen(szbuffer), TEXT("left mouse button down"));
		TextOut(hdc, 0, 0, szbuffer, lstrlen(szbuffer));
		ReleaseDC(hWnd, hdc);
		break;
	}
	case WM_RBUTTONDOWN: {
		StringCchPrintf(szbuffer, lstrlen(szbuffer), TEXT("right mouse button down"));
		TextOut(hdc, 0, 0, szbuffer, lstrlen(szbuffer));
		ReleaseDC(hWnd, hdc);
		break;
	}
	case WM_PAINT: {
		{
		}
		break;
	}
	case WM_DESTROY: {
		//MessageBox(hWnd, TEXT("WM_DESTROY been touch"), TEXT("testWin"), 0);
		PostQuitMessage(0);//发出线程终止请求
		mAlive = false;
		break;
	}
	}
}

bool ZApplication::peekMessage()
{
	MSG msg;
	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return mAlive;
}

void ZApplication::Render()
{
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

	//绘制一个渐变的三角形
	ZScrPoint point1(100, 100, ZRGBA(255, 0, 0, 255));
	ZScrPoint point2(600, 100, ZRGBA(0, 255, 0, 255));
	ZScrPoint point3(350, 500, ZRGBA(0, 0, 255, 255));
	Sgl->DrawTriangle(point1, point2, point3);

	//绘制图片
	Sgl->bEnableBlend = true;
	ZImage* image01 = ZImage::CreateZImage("assets/images/tx01.png");
	ZImage* image02 = ZImage::CreateZImage("assets/images/opencv.png");
	Sgl->DrawZImage(image01);
	Sgl->DrawZImage(image02, 150);
	Show();
}

void ZApplication::Show()
{
	BitBlt(hdc, 0, 0, mWidth, mHeight, mCanvasDC, 0, 0, SRCCOPY);
}

