#pragma once
#include <Windows.h>
#include "iostream"
#include "Functions/AddFun.h"
#include "application/application.h"
#include "GPU/gpu.h"

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
	if (!ZApp->InitZApplication(hInstance, 800, 600))return -1;
	//Sgl->InitSurface(ZApp->GetWidth(), ZApp->GetHeight(), ZApp->GetCanvasBuffer());

	bool alive = true;
	while (alive) {
		alive = ZApp->peekMessage();
		ZApp->Render();
		/*Render();
		ZApp->Show();*/
	}
	return 0;
}

