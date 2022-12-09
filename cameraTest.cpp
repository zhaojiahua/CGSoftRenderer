#pragma once
#include <Windows.h>
#include "iostream"
#include "Functions/AddFun.h"
#include "application/application.h"
#include "GPU/gpu.h"
#include "GPU/dataStruct.h"
#include "GPU/bufferObject.h"
#include "GPU/VAO.h"
#include "GPU/Shader/TextureShader.h"
#include "application/camera.h"

#pragma comment(linker,"/subsystem:console /entry:wWinMainCRTStartup")


ZCamera* g_camera = nullptr;
math::Mat4f g_modelMat;

TextureShader* g_shader;
uint32_t g_vao = 0;
uint32_t g_ebo = 0;
uint32_t g_positionvbo = 0;
uint32_t g_colorvbo = 0;
uint32_t g_uvvbo = 0;

uint32_t g_texture = 0;
ZImage* g_image = nullptr;

void Prepare() {
	g_camera = new ZCamera(60.0f, (float)ZApp->GetWidth() / (float)ZApp->GetHeight(), 0.1f, 200.0f, { 0.0,1.0,0.0 });
	ZApp->SetCamera(g_camera);

	g_shader = new TextureShader();
	g_image = ZImage::CreateZImage("assets/images/tx01.png");
	g_texture = Sgl->GenerateTexture();
	Sgl->BindTexture(g_texture);
	Sgl->TexImage2D(g_image->mWidth, g_image->mHeight, g_image->mData);
	Sgl->TexParameter(TEXTURE_FILTER, TEXTURE_FILTER_LINEAR);
	Sgl->TexParameter(TEXTURE_WRAP_U, TEXTURE_WRAP_REPEAT);
	Sgl->TexParameter(TEXTURE_WRAP_V, TEXTURE_WRAP_REPEAT);
	Sgl->BindTexture(0);

	Sgl->Enable(CULL_FACE);
	Sgl->Enable(DEPTH_TEST);
	Sgl->SetFrontFace(CCW_FRONT_FACE);
	Sgl->SetCullFace(BACK_FACE);
	//一个三角形数据缓存
	float meshPos[] = { -0.5f,0.0f,-1.0f,		0.5f,0.0f,-1.0f,		0.0f,0.5f,-1.0f };
	float meshColor[] = { 1.0f,0.0f,0.0f,1.0f,		0.0f,1.0f,0.0f,1.0f,		0.0f,0.0f,1.0f,1.0f };
	float meshUV[] = { 0.0f,0.0f,		1.0f,0.0f,		0.5f,1.0f };
	uint32_t meshIndex[] = { 0,1,2 };

	//在GPU中生成相应的EBO
	//Index属性设置(EBO)
	g_ebo = Sgl->GenerateVertexBuffer();
	Sgl->BindingVertexBuffer(ELEMENT_VERTEXT_ARRAY_BUFFER, g_ebo);	//绑定EBO编号
	Sgl->BufferUpData(ELEMENT_VERTEXT_ARRAY_BUFFER, sizeof(uint32_t) * 3, meshIndex);	//将数据传入
	Sgl->BindingVertexBuffer(ELEMENT_VERTEXT_ARRAY_BUFFER, 0);	//重新将当前VBO编号设置成0(解绑)

	g_vao = Sgl->GenerateVertexArray();
	Sgl->BingdingVertexArray(g_vao);

	g_positionvbo = Sgl->GenerateVertexBuffer();
	Sgl->BindingVertexBuffer(VERTEXT_ARRAY_BUFFER, g_positionvbo);
	Sgl->BufferUpData(VERTEXT_ARRAY_BUFFER, sizeof(float) * 9, meshPos);
	Sgl->VertexAttributePointer(0, 3, 3 * sizeof(float), 0);

	g_colorvbo = Sgl->GenerateVertexBuffer();
	Sgl->BindingVertexBuffer(VERTEXT_ARRAY_BUFFER, g_colorvbo);
	Sgl->BufferUpData(VERTEXT_ARRAY_BUFFER, sizeof(float) * 12, meshColor);
	Sgl->VertexAttributePointer(1, 4, 4 * sizeof(float), 0);

	g_uvvbo = Sgl->GenerateVertexBuffer();
	Sgl->BindingVertexBuffer(VERTEXT_ARRAY_BUFFER, g_uvvbo);
	Sgl->BufferUpData(VERTEXT_ARRAY_BUFFER, sizeof(float) * 6, meshUV);
	Sgl->VertexAttributePointer(2, 2, 2 * sizeof(float), 0);

	//解绑
	Sgl->BindingVertexBuffer(VERTEXT_ARRAY_BUFFER, 0);
	Sgl->BingdingVertexArray(0);

}

void Render() {
	g_shader->mModeMatrix = g_modelMat;
	g_shader->mViewMatrix = g_camera->GetViewMatrix();
	g_shader->mPerspectiveMatrix = g_camera->GetPerspectiveMatrix();
	g_shader->mDiffuseTexture = g_texture;
	Sgl->Clear();
	Sgl->UseShaderProgram(g_shader);
	Sgl->BingdingVertexArray(g_vao);
	Sgl->BindingVertexBuffer(ELEMENT_VERTEXT_ARRAY_BUFFER, g_ebo);
	Sgl->DrawElement(DRAW_TRIANGLES, 0, 3);

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
		g_camera->Updata();
		Render();
		ZApp->Show();
	}
	delete g_shader;
	ZImage::DestroyZImage(g_image);
	Sgl->DeleteTexture(g_texture);
	delete g_camera;

	return 0;
}

