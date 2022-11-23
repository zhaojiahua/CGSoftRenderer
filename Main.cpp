#include <Windows.h>
#include "iostream"
#include "Functions/AddFun.h"

#pragma comment(linker,"/subsystem:console /entry:wWinMainCRTStartup")

int main()
{
	std::cout << AddFun(1.0, 5.3) << std::endl;
	return 0;
}
int APIENTRY wWinMain(
	_In_ HINSTANCE hInstance,
	_In_ HINSTANCE hPreInstance,
	_In_ LPWSTR lpCmdLine,
	_In_ int nCmdShow)
{
	std::cout << "hello windows" << std::endl;
	return 0;
}

