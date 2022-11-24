#include "application.h"
#include "strsafe.h"

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

	///////////////获取显示器的分辨率
	cxClient = GetSystemMetrics(SM_CXSCREEN);
	cyClient = GetSystemMetrics(SM_CYSCREEN);

	ZRegisterWindowClass(hInstance);
	if (ZCreateWindow(hInstance)) return true;
	return false;
}

void ZApplication::HandleMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	RECT clientrect;
	GetClientRect(hWnd, &clientrect);
	HDC hdc;
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
		bserPt[0].x = cxClient / 4;
		bserPt[0].y = cyClient / 2;

		bserPt[1].x = cxClient / 2;
		bserPt[1].y = cyClient / 4;

		bserPt[2].x = cxClient / 2;
		bserPt[2].y = cyClient * 3 / 4;

		bserPt[3].x = cxClient * 3 / 4;
		bserPt[3].y = cyClient / 2;
	}
	case WM_LBUTTONDOWN: {
		hdc = GetDC(hWnd);
		StringCchPrintf(szbuffer, lstrlen(szbuffer), TEXT("left mouse button down"));
		TextOut(hdc, 0, 0, szbuffer, lstrlen(szbuffer));
		SelectObject(hdc, GetStockObject(WHITE_PEN));
		PolyBezier(hdc, bserPt, 4);
		//MessageBox(hWnd,TEXT("WM_LBUTTONUP"),TEXT("clickUpWindow"),MB_OK);
		bserPt[1].x = LOWORD(lParam);		//低位是鼠标的横坐标
		bserPt[1].y = HIWORD(lParam);		//高位是鼠标的纵坐标
		SelectObject(hdc, GetStockObject(BLACK_PEN));
		PolyBezier(hdc, bserPt, 4);
		ReleaseDC(hWnd, hdc);
		return;
	}
	case WM_RBUTTONDOWN: {
		hdc = GetDC(hWnd);
		StringCchPrintf(szbuffer, lstrlen(szbuffer), TEXT("right mouse button down"));
		TextOut(hdc, 0, 0, szbuffer, lstrlen(szbuffer));
		SelectObject(hdc, GetStockObject(WHITE_PEN));
		PolyBezier(hdc, bserPt, 4);
		//MessageBox(hWnd,TEXT("WM_LBUTTONUP"),TEXT("clickUpWindow"),MB_OK);
		bserPt[2].x = LOWORD(lParam);		//低位是鼠标的横坐标
		bserPt[2].y = HIWORD(lParam);		//高位是鼠标的纵坐标
		SelectObject(hdc, GetStockObject(BLACK_PEN));
		PolyBezier(hdc, bserPt, 4);
		ReleaseDC(hWnd, hdc);
		return;
	}
	case WM_PAINT: {
		hdc = BeginPaint(hWnd, &ps);
		//绘制文本
		StringCchPrintf(szbuffer, lstrlen(szbuffer), TEXT("hello renderer"));
		TextOut(hdc, 0, 0, szbuffer, lstrlen(szbuffer));
		/*for (int i = clientrect.left; i < clientrect.right; i++) {
			SetPixel(hdc, i, 100, RGB(255, 0, 0));
		}*/
		//MoveToEx(hdc, 100, 100, nullptr);
		//LineTo(hdc, 200, 300);		//画线默认起点是0,0
		//LineTo(hdc, 300, 300);
		/*//绘制网格
		for (int i = clientrect.left; i < clientrect.right / 50; i++) {
			MoveToEx(hdc, i * 50, clientrect.top, nullptr);
			LineTo(hdc, i * 50, clientrect.bottom);
		}
		for (int i = clientrect.top; i < clientrect.bottom / 50; i++) {
			MoveToEx(hdc, clientrect.left, i * 50, nullptr);
			LineTo(hdc, clientrect.right, i * 50 );
		}*/
		//绘制多个多边形
		POINT points[] = { 50,20,20,60,80,60,50,20,70,20,100,60,130,20,70,20,150,20,150,60 };	//一共十个点
		DWORD pointsSeprate[] = { 4,4,2 };		//将这10个点分成3个组
		PolyPolyline(hdc, points, pointsSeprate,3);

		//绘制sin曲线
		MoveToEx(hdc, 0, cyClient / 2, nullptr);
		LineTo(hdc, cxClient, cyClient / 2);
		MoveToEx(hdc, 0, cyClient / 2, nullptr);
		POINT sinPoints[DNUM];
		for (int i = 0; i < DNUM; i++) {
			sinPoints[i].x = cxClient * i / DNUM;
			sinPoints[i].y = cyClient * 0.5 * (sin(2 * PI * i / DNUM) + 1);
		}
		PolylineTo(hdc, sinPoints, DNUM);

		//绘制贝塞尔曲线
		PolyBezier(hdc, bserPt, 4);

		//绘制边框(即绘制边框又填充内容)
		Rectangle(hdc, 100, 100, 200, 300);
		Ellipse(hdc,100,100,200,300);


		EndPaint(hWnd, &ps);
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
