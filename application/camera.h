#pragma once
#include "../global/base.h"
#include "../math/math.h"

//定义相机的几个移动状态
#define NO_MOVE 0					//	0000 0000
#define LEFT_MOVE 0x001			// 0000 0001
#define RIGHT_MOVE 0x002		// 0000 0010
#define FRONT_MOVE 0x004		// 0000 0100
#define BACK_MOVE 0x008		// 0000 1000

//相机类
class ZCamera
{
	math::Mat4f mPerspectiveMatrix;	//相机的透视矩阵
	math::Mat4f mViewMatrix;	//相机的视图变换矩阵
	math::vec3f mTop{ 0.0f,1.0f,0.0f };	//相机的默认天空方向(天空方向的在正上方)
	math::vec3f mPosition{ 0.0f,0.0f,0.0f };	//相机的当前世界空间坐标位置(默认在原点)
	math::vec3f mFront{ 0.0f,0.0f,-1.0f };	//相机当前的朝向(默认朝向世界Z轴的负方向)

	float mMoveSpeed{ 0.01f };	//相机的移动速度
	float mSensitivity{ 0.1f };	//相机的旋转灵敏度

	float mPitch{ 0.0f };	//相机的Pitch值
	float mYaw{ -90.0f };	//相机的Yaw值(规定由X轴正方向转到Z轴负方向为-90度)

	uint32_t mMoveState{ NO_MOVE };	//标记相机移动的状态
	bool mMouseMoving{ false };	//标记鼠标移动的状态
	int mCurrentMouseX{ 0 };	//当前鼠标的屏幕坐标X
	int mCurrentMouseY{ 0 };	//当前鼠标的屏幕坐标Y

public:
	ZCamera(float fovy, float aspect, float near, float far, const math::vec3f& top);	//相机的构造函数给相机设定好参数(相机的视角,输出屏幕的宽高比,近剪裁面,远剪裁面,天空的方向),从而确定相机的透视矩阵
	~ZCamera();

	math::Mat4f GetPerspectiveMatrix() { return mPerspectiveMatrix; }
	math::Mat4f GetViewMatrix() { return mViewMatrix; }

	void OnRMouseDown(const int& x, const int& y);	//鼠标右键按下事件
	void OnRMouseUp(const int& x, const int& y);	//鼠标右键抬起事件
	void OnRMouseMove(const int& x, const int& y);	//鼠标移动事件
	void OnKeyDown(const uint32_t& key);	//键盘按下事件
	void OnKeyUp(const uint32_t& key);	//键盘抬起事件

	void Updata();//实时更新mViewMatrix

private:
	void PitchOffset(int yoffset);	//根据Pitch值的变化更新mFront
	void YawOffset(int xoffset);		//根据Yaw值的变化更新mFront

};