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
uint32_t g_ebo = 0;
uint32_t g_positionvbo = 0;
uint32_t g_colorvbo = 0;
uint32_t g_uvvbo = 0;
float angle = 90.0f;	//模型开始旋转的初始角度
float cameraLoc = 3.0f;	//相机初始位置
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
	//一个三角形数据缓存
	float meshPos[] = { -0.5f,-0.5f,0.0f,		-0.5f,0.5f,0.0f,		0.5f,-0.5f,0.0f };	
	float meshColor[] = { 1.0f,0.0f,0.0f,1.0f,		0.0f,1.0f,0.0f,1.0f,		0.0f,0.0f,1.0f,1.0f };
	float meshUV[] = { 0.0f,0.0f,		0.0f,1.0f,		1.0f,0.0f };
	uint32_t meshIndex[] = { 0,1,2 };

	//在GPU中生成相应的EBO
	//Index属性设置(EBO)
	g_ebo = Sgl->GenerateVertexBuffer();
	Sgl->BindingVertexBuffer(ELEMENT_VERTEXT_ARRAY_BUFFER, g_ebo);	//绑定EBO编号
	Sgl->BufferUpData(ELEMENT_VERTEXT_ARRAY_BUFFER, sizeof(uint32_t) * 3, meshIndex);	//将数据传入
	Sgl->BindingVertexBuffer(ELEMENT_VERTEXT_ARRAY_BUFFER, 0);	//重新将当前VBO编号设置成0(解绑)
	
	//生成VAO且绑定
	g_vao = Sgl->GenerateVertexArray();
	Sgl->BingdingVertexArray(g_vao);
	//生成每个属性VBO并绑定设置其属性信息
	g_positionvbo = Sgl->GenerateVertexBuffer();
	Sgl->BindingVertexBuffer(VERTEXT_ARRAY_BUFFER, g_positionvbo);	//绑定VBO编号
	Sgl->BufferUpData(VERTEXT_ARRAY_BUFFER, sizeof(float) * 9, meshPos);	//将数据传入
	Sgl->VertexAttributePointer(0, 3, 3 * sizeof(float), 0);	//设置描述信息
	g_colorvbo = Sgl->GenerateVertexBuffer();
	Sgl->BindingVertexBuffer(VERTEXT_ARRAY_BUFFER, g_colorvbo);	//绑定VBO编号
	Sgl->BufferUpData(VERTEXT_ARRAY_BUFFER, sizeof(float) * 12, meshColor);	//将数据传入
	Sgl->VertexAttributePointer(1, 4, 4 * sizeof(float), 0);	//设置描述信息
	g_uvvbo = Sgl->GenerateVertexBuffer();
	Sgl->BindingVertexBuffer(VERTEXT_ARRAY_BUFFER, g_uvvbo);	//绑定VBO编号
	Sgl->BufferUpData(VERTEXT_ARRAY_BUFFER, sizeof(float) * 6, meshUV);	//将数据传入
	Sgl->VertexAttributePointer(2, 2, 2 * sizeof(float), 0);	//设置描述信息
	//解绑
	Sgl->BindingVertexBuffer(VERTEXT_ARRAY_BUFFER, 0);
	Sgl->BingdingVertexArray(0);

}

void Render() {
	transform();
	g_shader->mModeMatrix = g_modelMat;
	g_shader->mViewMatrix = g_viewMat;
	g_shader->mPerspectiveMatrix = g_perspectiveMat;
	Sgl->Clear();
	Sgl->UseShaderProgram(g_shader);
	Sgl->BingdingVertexArray(g_vao);
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
	//Sgl->InitSurface(ZApp->GetWidth(), ZApp->GetHeight(), ZApp->GetCanvasBuffer());
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

