#pragma once
#include <Windows.h>
#include "iostream"
#include "Functions/AddFun.h"
#include "application/application.h"
#include "GPU/gpu.h"
#include "GPU/dataStruct.h"
#include "GPU/bufferObject.h"
#include "GPU/VAO.h"
#include "GPU/Shader/DefaultShader.h"

#pragma comment(linker,"/subsystem:console /entry:wWinMainCRTStartup")


DefaultShader* g_shader;
math::Mat4f g_modelMat;
math::Mat4f g_viewMat;
math::Mat4f g_perspectiveMat;
uint32_t g_vao = 0;
uint32_t g_vao1 = 0;
uint32_t g_vao2 = 0;
uint32_t g_ebo = 0;
uint32_t g_positionvbo = 0;
uint32_t g_positionvbo1 = 0;
uint32_t g_positionvbo2 = 0;
uint32_t g_colorvbo = 0;
uint32_t g_colorvbo1 = 0;
uint32_t g_colorvbo2 = 0;
uint32_t g_uvvbo = 0;
float angle = 0.0f;	//模型开始旋转的初始角度
float cameraLoc = 2.0f;	//相机初始位置
void transform() {
	angle += 0.05f;	//让模型每帧旋转0.01f弧度
	cameraLoc -= 0.005f;	//每帧把相机往前移动0.0f单位
	//模型变换
	g_modelMat = math::Rotate(math::Mat4f(1.0f), angle, math::vec3f(0.0f, 1.0f, 0.0f));		//M矩阵

	auto cameraModelMatrix = math::Translate(math::Mat4f(1.0f), math::vec3f{ 0.0f,0.0f,cameraLoc });
	g_viewMat = math::Inverse(cameraModelMatrix);
}
void Prepare() {
	g_shader = new DefaultShader();
	g_perspectiveMat= math::Perspective(60.0f, (float)ZApp->GetWidth() / (float)ZApp->GetHeight(), 0.1f, 100.0f);
	g_modelMat = math::Rotate(math::Mat4f(1.0f), angle, math::vec3f(0.0f, 1.0f, 0.0f));
	auto cameraModelMatrix = math::Translate(math::Mat4f(1.0f), math::vec3f{ 0.0f,0.0f,cameraLoc });
	g_viewMat = math::Inverse(cameraModelMatrix);
	Sgl->Enable(CULL_FACE);
	Sgl->Enable(DEPTH_TEST);
	Sgl->SetFrontFace(CCW_FRONT_FACE);
	Sgl->SetCullFace(BACK_FACE);
	//一个三角形数据缓存
	float meshPos[] = { -0.5f,0.0f,0.0f,		0.5f,0.0f,0.0f,		0.25f,0.5f,0.0f };
	float meshPos1[] = { 0.3f,0.0f,-0.3f,		0.8f,0.0f,-0.3f,		0.45f,0.5f,-0.3f };
	float meshPos2[] = { -0.1f,0.0f,0.3f,		0.5f,-0.1f,0.3f,		0.4f,0.6f,0.3f };
	float meshColor[] = { 1.0f,0.0f,0.0f,1.0f,		0.0f,1.0f,0.0f,1.0f,		0.0f,0.0f,1.0f,1.0f };
	float meshColor1[] = { 1.0f,1.0f,0.0f,1.0f,		1.0f,1.0f,0.0f,1.0f,		1.0f,1.0f,0.0f,1.0f };
	float meshColor2[] = { 1.0f,0.0f,0.0f,0.5f,		1.0f,0.0f,0.0f,0.5f,		1.0f,0.0f,0.0f,0.5f };
	float meshUV[] = { 0.0f,0.0f,		0.0f,1.0f,		1.0f,0.0f };
	uint32_t meshIndex[] = { 0,1,2 };

	//在GPU中生成相应的EBO
	//Index属性设置(EBO)
	g_ebo = Sgl->GenerateVertexBuffer();
	Sgl->BindingVertexBuffer(ELEMENT_VERTEXT_ARRAY_BUFFER, g_ebo);	//绑定EBO编号
	Sgl->BufferUpData(ELEMENT_VERTEXT_ARRAY_BUFFER, sizeof(uint32_t) * 3, meshIndex);	//将数据传入
	Sgl->BindingVertexBuffer(ELEMENT_VERTEXT_ARRAY_BUFFER, 0);	//重新将当前VBO编号设置成0(解绑)
	
	g_uvvbo = Sgl->GenerateVertexBuffer();
	Sgl->BindingVertexBuffer(VERTEXT_ARRAY_BUFFER, g_uvvbo);
	Sgl->BufferUpData(VERTEXT_ARRAY_BUFFER, sizeof(float) * 6, meshUV);
	Sgl->BindingVertexBuffer(VERTEXT_ARRAY_BUFFER, 0);

	g_vao = Sgl->GenerateVertexArray();
	Sgl->BingdingVertexArray(g_vao);

	g_positionvbo = Sgl->GenerateVertexBuffer();
	Sgl->BindingVertexBuffer(VERTEXT_ARRAY_BUFFER, g_positionvbo);
	Sgl->BufferUpData(VERTEXT_ARRAY_BUFFER, sizeof(float) * 9, meshPos);
	Sgl->VertexAttributePointer(0, 3, 3 * sizeof(float), 0);

	g_colorvbo= Sgl->GenerateVertexBuffer();
	Sgl->BindingVertexBuffer(VERTEXT_ARRAY_BUFFER, g_colorvbo);
	Sgl->BufferUpData(VERTEXT_ARRAY_BUFFER, sizeof(float) * 12, meshColor);
	Sgl->VertexAttributePointer(1, 4, 4 * sizeof(float), 0);

	Sgl->BindingVertexBuffer(VERTEXT_ARRAY_BUFFER, g_uvvbo);
	Sgl->VertexAttributePointer(2, 2, 2 * sizeof(float), 0);
		
	//解绑
	Sgl->BindingVertexBuffer(VERTEXT_ARRAY_BUFFER, 0);
	Sgl->BingdingVertexArray(0);

	//绑定另一个三角形
	g_vao1 = Sgl->GenerateVertexArray();
	Sgl->BingdingVertexArray(g_vao1);

	g_positionvbo1 = Sgl->GenerateVertexBuffer();
	Sgl->BindingVertexBuffer(VERTEXT_ARRAY_BUFFER, g_positionvbo1);
	Sgl->BufferUpData(VERTEXT_ARRAY_BUFFER, sizeof(float) * 9, meshPos1);
	Sgl->VertexAttributePointer(0, 3, 3 * sizeof(float), 0);

	g_colorvbo1 = Sgl->GenerateVertexBuffer();
	Sgl->BindingVertexBuffer(VERTEXT_ARRAY_BUFFER, g_colorvbo1);
	Sgl->BufferUpData(VERTEXT_ARRAY_BUFFER, sizeof(float) * 12, meshColor1);
	Sgl->VertexAttributePointer(1, 4, 4 * sizeof(float), 0);

	Sgl->BindingVertexBuffer(VERTEXT_ARRAY_BUFFER, g_uvvbo);
	Sgl->VertexAttributePointer(2, 2, 2 * sizeof(float), 0);

	//解绑
	Sgl->BindingVertexBuffer(VERTEXT_ARRAY_BUFFER, 0);
	Sgl->BingdingVertexArray(0);

	//第三个三角形
	g_vao2 = Sgl->GenerateVertexArray();
	Sgl->BingdingVertexArray(g_vao2);

	g_positionvbo2 = Sgl->GenerateVertexBuffer();
	Sgl->BindingVertexBuffer(VERTEXT_ARRAY_BUFFER, g_positionvbo2);
	Sgl->BufferUpData(VERTEXT_ARRAY_BUFFER, sizeof(float) * 9, meshPos2);
	Sgl->VertexAttributePointer(0, 3, 3 * sizeof(float), 0);

	g_colorvbo2 = Sgl->GenerateVertexBuffer();
	Sgl->BindingVertexBuffer(VERTEXT_ARRAY_BUFFER, g_colorvbo2);
	Sgl->BufferUpData(VERTEXT_ARRAY_BUFFER, sizeof(float) * 12, meshColor2);
	Sgl->VertexAttributePointer(1, 4, 4 * sizeof(float), 0);

	Sgl->BindingVertexBuffer(VERTEXT_ARRAY_BUFFER, g_uvvbo);
	Sgl->VertexAttributePointer(2, 2, 2 * sizeof(float), 0);

	//解绑
	Sgl->BindingVertexBuffer(VERTEXT_ARRAY_BUFFER, 0);
	Sgl->BingdingVertexArray(0);
}

void Render() {
	//transform();
	g_shader->mModeMatrix = g_modelMat;
	g_shader->mViewMatrix = g_viewMat;
	g_shader->mPerspectiveMatrix = g_perspectiveMat;
	Sgl->Clear();
	Sgl->UseShaderProgram(g_shader);
	Sgl->BingdingVertexArray(g_vao);
	Sgl->BindingVertexBuffer(ELEMENT_VERTEXT_ARRAY_BUFFER, g_ebo);
	Sgl->DrawElement(DRAW_TRIANGLES, 0, 3);

	//再绘制第二个三角形
	Sgl->BingdingVertexArray(g_vao1);
	Sgl->BindingVertexBuffer(ELEMENT_VERTEXT_ARRAY_BUFFER, g_ebo);
	Sgl->DrawElement(DRAW_TRIANGLES, 0, 3);

	//绘制第三个三角形
	Sgl->BingdingVertexArray(g_vao2);
	Sgl->BindingVertexBuffer(ELEMENT_VERTEXT_ARRAY_BUFFER, g_ebo);
	Sgl->DrawElement(DRAW_TRIANGLES, 0, 3);
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
	Sgl->InitSurface(ZApp->GetWidth(), ZApp->GetHeight(), ZApp->GetCanvasBuffer());
	Prepare();
	bool alive = true;
	while (alive) {
		alive = ZApp->peekMessage();
		Render();
		ZApp->Show();
	}
	delete g_shader;
	return 0;
}

