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
	wndClass.style= CS_HREDRAW | CS_VREDRAW;	//水平/垂直大小发生变化重绘窗口
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
	PAINTSTRUCT ps;
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
			//hdc = BeginPaint(hWnd, &ps);
			////绘制文本
			//StringCchPrintf(szbuffer, lstrlen(szbuffer), TEXT("hello renderer"));
			//TextOut(hdc, 0, 0, szbuffer, lstrlen(szbuffer));
			///*for (int i = clientrect.left; i < clientrect.right; i++) {
			//	SetPixel(hdc, i, 100, RGB(255, 0, 0));
			//}*/
			////MoveToEx(hdc, 100, 100, nullptr);
			////LineTo(hdc, 200, 300);		//画线默认起点是0,0
			////LineTo(hdc, 300, 300);
			///*//绘制网格
			//for (int i = clientrect.left; i < clientrect.right / 50; i++) {
			//	MoveToEx(hdc, i * 50, clientrect.top, nullptr);
			//	LineTo(hdc, i * 50, clientrect.bottom);
			//}
			//for (int i = clientrect.top; i < clientrect.bottom / 50; i++) {
			//	MoveToEx(hdc, clientrect.left, i * 50, nullptr);
			//	LineTo(hdc, clientrect.right, i * 50 );
			//}*/
			////绘制多个多边形
			//POINT points[] = { 50,20,20,60,80,60,50,20,70,20,100,60,130,20,70,20,150,20,150,60 };	//一共十个点
			//DWORD pointsSeprate[] = { 4,4,2 };		//将这10个点分成3个组
			//PolyPolyline(hdc, points, pointsSeprate,3);

			////绘制sin曲线
			//MoveToEx(hdc, 0, cyClient / 2, nullptr);
			//LineTo(hdc, cxClient, cyClient / 2);
			//MoveToEx(hdc, 0, cyClient / 2, nullptr);
			//POINT sinPoints[DNUM];
			//for (int i = 0; i < DNUM; i++) {
			//	sinPoints[i].x = cxClient * i / DNUM;
			//	sinPoints[i].y = cyClient * 0.5 * (sin(2 * PI * i / DNUM) + 1);
			//}
			//PolylineTo(hdc, sinPoints, DNUM);

			////绘制贝塞尔曲线
			//PolyBezier(hdc, bserPt, 4);

			////绘制边框(即绘制边框又填充内容)
			//Rectangle(hdc, 100, 100, 200, 300);
			//Ellipse(hdc,100,100,200,300);
			//EndPaint(hWnd, &ps);
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
	//绘制一条横线
	for (uint32_t i = 0; i < mWidth; ++i) {
		Sgl->DrawPoint(i, 200, ZRGBA(0, 255, 255, 255));
	}
	//绘制雪花噪点
	for (uint32_t i = 0; i < mWidth; i++)
	{
		for (uint32_t j = 0; j < mHeight; j++)
		{
			uint32_t temp = std::rand() % 255;
			Sgl->DrawPoint(i, j, ZRGBA(temp, temp, temp, temp));
		}
	}
	Show();
}

void ZApplication::Show()
{
	BitBlt(hdc, 0, 0, mWidth, mHeight, mCanvasDC, 0, 0, SRCCOPY);
}

