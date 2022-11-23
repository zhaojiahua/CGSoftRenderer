#include "application.h"

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

	RECT windowRect;
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
	wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);//窗口背景色
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
	if (ZCreateWindow(hInstance)) return true;
	return false;
}

void ZApplication::HandleMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CLOSE: {
		DestroyWindow(hWnd);//此处销毁窗体,会自动发出WM_DESTROY
		break;
	}
	case WM_PAINT: {
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
	}
		break;
	case WM_DESTROY: {
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
