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
#include "GPU/Shader/LightShader.h"
#include "application/camera.h"
#include "application/light.h"

#pragma comment(linker,"/subsystem:console /entry:wWinMainCRTStartup")


ZCamera* g_camera = nullptr;
math::Mat4f g_modelMat;

LightShader* g_shader;
DirectionLight* g_light;

uint32_t g_vao = 0;
uint32_t g_ebo = 0;
uint32_t g_vbo = 0;

uint32_t g_texture = 0;
ZImage* g_image = nullptr;

void Prepare() {
	g_camera = new ZCamera(60.0f, (float)ZApp->GetWidth() / (float)ZApp->GetHeight(), 0.1f, 200.0f, { 0.0,1.0,0.0 });
	ZApp->SetCamera(g_camera);

	g_shader = new LightShader();
	g_light = new DirectionLight();
	g_light->SetLightDirection({ -1.0f,-0.3f,-0.7f });
	g_shader->mDirectionLight = g_light;
	g_shader->mEnvLight = { 0.1f,0.1f,0.1f };

	g_image = ZImage::CreateZImage("assets/images/tx01.png");
	g_texture = Sgl->GenerateTexture();
	Sgl->BindTexture(g_texture);
	Sgl->TexImage2D(g_image->mWidth, g_image->mHeight, g_image->mData);
	Sgl->SetTexParameter(TEXTURE_FILTER, TEXTURE_FILTER_LINEAR);
	Sgl->SetTexParameter(TEXTURE_WRAP_U, TEXTURE_WRAP_REPEAT);
	Sgl->SetTexParameter(TEXTURE_WRAP_V, TEXTURE_WRAP_REPEAT);
	Sgl->SetTexParameter(TEXTURE_TRANSPARENCYORNOT, TEXTURE_NOTTRANSPARENCY);
	Sgl->BindTexture(0);

	Sgl->Enable(CULL_FACE);
	Sgl->Enable(DEPTH_TEST);
	Sgl->SetFrontFace(CCW_FRONT_FACE);
	Sgl->SetCullFace(BACK_FACE);
	//一个三角形数据缓存
	float mesh[] = { 
		//位置								//法线							//uv
		//prim0
		-0.500000,-0.500000,-2.500000,		0.000000 ,0.000000 ,1.000000	,	0.000000,0.000000,
		0.500000 ,-0.500000,-2.500000,		0.000000 ,0.000000 ,1.000000	,	1.000000,0.000000,
		-0.500000,0.500000 ,-2.500000,		0.000000 ,0.000000 ,1.000000	,	0.000000,1.000000,
		//prim1
		-0.500000 ,0.500000,-2.500000,		0.000000 ,0.000000 ,1.000000	,	0.000000,1.000000,
		0.500000,-0.500000 ,-2.500000,		0.000000 ,0.000000 ,1.000000	,	1.000000,0.000000,
		0.500000 ,0.500000 ,-2.500000,		0.000000 ,0.000000 ,1.000000	,	1.000000,1.000000,
		//prim2
		0.500000 ,-0.500000,-3.500000,		0.000000 ,0.000000 ,-1.000000	,	0.000000,0.000000,
		-0.500000,-0.500000,-3.500000,		0.000000 ,0.000000 ,-1.000000	,	1.000000,0.000000,
		0.500000 ,0.500000 ,-3.500000,		0.000000 ,0.000000 ,-1.000000	,	0.000000,1.000000,
		//prim3
		0.500000,0.500000,-3.500000,			0.000000 ,0.000000 ,-1.000000	,	0.000000,1.000000,
		-0.500000 ,-0.500000 ,-3.500000,	0.000000 ,0.000000 ,-1.000000	,	1.000000,0.000000,
		-0.500000,0.500000 ,-3.500000,		0.000000 ,0.000000 ,-1.000000	,	1.000000,1.000000,
		//prim4
		-0.500000,0.500000 ,-2.500000,		0.000000 ,1.000000 ,0.000000	,	0.000000,0.000000,
		0.500000 ,0.500000 ,-2.500000,		0.000000 ,1.000000 ,0.000000	,	1.000000,0.000000,
		-0.500000,0.500000 ,-3.500000,		0.000000 ,1.000000 ,0.000000	,	0.000000,1.000000,
		//prim5
		0.500000 ,0.500000 ,-2.500000,		0.000000 ,1.000000 ,0.000000	,	1.000000,0.000000,
		0.500000 ,0.500000 ,-3.500000,		0.000000 ,1.000000 ,0.000000	,	1.000000,1.000000,
		-0.500000,0.500000 ,-3.500000,		0.000000 ,1.000000 ,0.000000	,	0.000000,1.000000,
		
		//prim6
		-0.500000,-0.500000,-3.500000,		-1.000000,0.000000 ,0.000000	,	0.000000,0.000000,
		-0.500000,-0.500000,-2.500000,		-1.000000,0.000000 ,0.000000	,	1.000000,0.000000,
		-0.500000,0.500000 ,-3.500000,		-1.000000,0.000000 ,0.000000	,	0.000000,1.000000,
		//prim7
		-0.500000,0.500000,-2.500000,		-1.000000,0.000000 ,0.000000	,	1.000000,0.000000,
		-0.500000,0.500000 ,-3.500000,		-1.000000,0.000000 ,0.000000	,	1.000000,1.000000,
		-0.500000,-0.500000 ,-2.500000,		-1.000000,0.000000 ,0.000000	,	0.000000,1.000000,
		//prim8
		0.500000 ,-0.500000,-2.500000,		1.000000 ,0.000000 ,0.000000	,	0.000000,0.000000,
		0.500000 ,-0.500000,-3.500000,		1.000000 ,0.000000 ,0.000000	,	1.000000,0.000000,
		0.500000 ,0.500000 ,-2.500000,		1.000000 ,0.000000 ,0.000000	,	0.000000,1.000000,
		//prim9
		0.500000 ,0.500000,-3.500000,		1.000000 ,0.000000 ,0.000000	,	1.000000,0.000000,
		0.500000 ,0.500000 ,-2.500000,		1.000000 ,0.000000 ,0.000000	,	1.000000,1.000000,
		0.500000 ,-0.500000 ,-3.500000,		1.000000 ,0.000000 ,0.000000	,	0.000000,1.000000,
		//prim10
		0.500000 ,-0.500000,-2.500000,		0.000000 ,-1.000000,0.000000	,	0.000000,0.000000,
		-0.500000,-0.500000,-2.500000,		0.000000 ,-1.000000,0.000000	,	1.000000,0.000000,
		0.500000 ,-0.500000,-3.500000,		0.000000 ,-1.000000,0.000000	,	0.000000,1.000000,
		//prim11
		-0.500000,-0.500000,-3.500000,		0.000000 ,-1.000000,0.000000	,	1.000000,0.000000,
		0.500000 ,-0.500000,-3.500000,		0.000000 ,-1.000000,0.000000	,	1.000000,1.000000,
		-0.500000,-0.500000,-2.500000,		0.000000 ,-1.000000,0.000000	,	0.000000,1.000000
	};
	uint32_t meshIndex[] = { 
		 0,		 1,		 2,		 3,		 4,		 5,
		 6,		 7,		 8,		 9,		 10,		 11,
		 12,		 13,		 14,		 15,		 16,		 17,
		 18,		 19,		 20,		 21,		 22,		 23,
		 24,		 25,		 26,		 27,		 28,		 29,
		 30,		 31,		 32,		 33,		 34,		 35 };

	//在GPU中生成相应的EBO
	//Index属性设置(EBO)
	g_ebo = Sgl->GenerateVertexBuffer();
	Sgl->BindingVertexBuffer(ELEMENT_VERTEXT_ARRAY_BUFFER, g_ebo);	//绑定EBO编号
	Sgl->BufferUpData(ELEMENT_VERTEXT_ARRAY_BUFFER, sizeof(uint32_t) * 36, meshIndex);	//将数据传入
	Sgl->BindingVertexBuffer(ELEMENT_VERTEXT_ARRAY_BUFFER, 0);	//重新将当前VBO编号设置成0(解绑)

	g_vao = Sgl->GenerateVertexArray();
	Sgl->BingdingVertexArray(g_vao);

	g_vbo = Sgl->GenerateVertexBuffer();
	Sgl->BindingVertexBuffer(VERTEXT_ARRAY_BUFFER, g_vbo);
	Sgl->BufferUpData(VERTEXT_ARRAY_BUFFER, sizeof(float) * 288, mesh);

	//position
	Sgl->VertexAttributePointer(0, 3, 8 * sizeof(float), 0);

	//normal
	Sgl->VertexAttributePointer(1, 3, 8 * sizeof(float), 3 * sizeof(float));

	//uv
	Sgl->VertexAttributePointer(2, 2, 8 * sizeof(float), 6 * sizeof(float));

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
	Sgl->DrawElement(DRAW_TRIANGLES, 0, 36);

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
	delete g_light;
	delete g_shader;
	ZImage::DestroyZImage(g_image);
	Sgl->DeleteTexture(g_texture);
	delete g_camera;

	return 0;
}

