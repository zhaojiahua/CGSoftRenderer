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
#include "application/model.h"

#pragma comment(linker,"/subsystem:console /entry:wWinMainCRTStartup")

ZCamera* g_camera = nullptr;
LightShader* g_shader;
DirectionLight* g_light;
math::vec3f g_envLight;

ZModel* g_model = nullptr;

void Prepare() {
	g_camera = new ZCamera(60.0f, (float)ZApp->GetWidth() / (float)ZApp->GetHeight(), 0.1f, 1000.0f, { 0.0f,1.0f,0.0f });
	ZApp->SetCamera(g_camera);

	g_shader = new LightShader();
	g_light = new DirectionLight();
	g_light->SetLightColor({ 1.0f,1.0f,1.0f });
	g_light->SetLightDirection(math::vec3f(-1.0f, -0.5f, -0.7f));
	g_envLight = { 0.5f,0.5f,0.5f };

	Sgl->Enable(CULL_FACE);
	Sgl->Enable(DEPTH_TEST);
	Sgl->SetFrontFace(CCW_FRONT_FACE);
	Sgl->SetCullFace(BACK_FACE);
	
	g_model = new ZModel();
	//g_model->Read("assets/modelsData/testBox/testBox2.obj");
	g_model->Read("assets/modelsData/bag/backpack.obj");
	//g_model->Read("assets/modelsData/bag/box.obj");
	//g_model->Read("assets/modelsData/dinosaur/source/RampagingT-Rex.glb");
	
}

float angle = 0.0f;
void Transform() {
	angle -= 0.02f;
	auto rotateM = math::Rotate(math::Mat4f(), angle, { 0.0f,1.0f,0.0f });
	auto tranformM = math::Translate(math::Mat4f(), 0.0f, 0.0f, -5.0f);
	auto modelM = tranformM * rotateM;
	modelM = math::Scale(modelM, 0.1f, 0.1f, 0.1f);
	g_model->SetModelMatrix(modelM);
}

void Render() {
	Transform();
	g_shader->mViewMatrix = g_camera->GetViewMatrix();
	g_shader->mPerspectiveMatrix = g_camera->GetPerspectiveMatrix();
	g_shader->mDirectionLight = g_light;
	g_shader->mEnvLight = g_envLight;
	Sgl->Clear();
	Sgl->UseShaderProgram(g_shader);
	g_model->DrawModel(g_shader);
}

int APIENTRY wWinMain(
	_In_ HINSTANCE hInstance,
	_In_ HINSTANCE hPreInstance,
	_In_ LPWSTR lpCmdLine,
	_In_ int nCmdShow)
{
	if (!ZApp->InitZApplication(hInstance, 1200, 900))return -1;
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
	delete g_camera;
	delete g_light;

	return 0;
}

