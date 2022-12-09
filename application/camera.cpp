#include "camera.h"

ZCamera::ZCamera(float fovy, float aspect, float near, float far, const math::vec3f& top)
{
	mTop = top;
	mPerspectiveMatrix = math::Perspective(fovy, aspect, near, far);
	Updata();
}

ZCamera::~ZCamera(){}

void ZCamera::OnRMouseDown(const int& x, const int& y)
{
	mMouseMoving = true;
	mCurrentMouseX = x;
	mCurrentMouseY = y;
}

void ZCamera::OnRMouseUp(const int& x, const int& y)
{
	mMouseMoving = false;
}

void ZCamera::OnRMouseMove(const int& x, const int& y)
{
	if (mMouseMoving) {
		int xoffset = x - mCurrentMouseX;
		int yoffset = y - mCurrentMouseY;
	
		mCurrentMouseX = x;
		mCurrentMouseY = y;

		//更新mFront
		PitchOffset(-yoffset);
		YawOffset(xoffset);
	}
}

void ZCamera::OnKeyDown(const uint32_t& key)
{
	switch (key)
	{
	case KEY_W:
		mMoveState |= FRONT_MOVE;
		break;
	case KEY_A:
		mMoveState |= LEFT_MOVE;
		break;
	case KEY_S:
		mMoveState |= BACK_MOVE;
		break;
	case KEY_D:
		mMoveState |= RIGHT_MOVE;
		break;
	default:
		break;
	}
}

void ZCamera::OnKeyUp(const uint32_t& key)
{
	switch (key)
	{
	case KEY_W:
		mMoveState &= ~FRONT_MOVE;		//先按位取反,再按位与操作,就把此状态给去掉了
		break;
	case KEY_A:
		mMoveState &= ~LEFT_MOVE;
		break;
	case KEY_S:
		mMoveState &= ~BACK_MOVE;
		break;
	case KEY_D:
		mMoveState &= ~RIGHT_MOVE;
		break;
	default:
		break;
	}
}

void ZCamera::Updata()
{
	math::vec3f moveDirection = { 0.0f,0.0f,0.0f };
	math::vec3f rightV = math::normalize(math::cross(mFront, mTop));

	if (mMoveState & FRONT_MOVE) moveDirection += mFront;
	if (mMoveState & BACK_MOVE) moveDirection += -mFront;
	if (mMoveState & LEFT_MOVE) moveDirection += -rightV;
	if (mMoveState & RIGHT_MOVE) moveDirection += rightV;

	if (math::lenghtSquared(moveDirection) != 0) {
		moveDirection = math::normalize(moveDirection);
		mPosition += mMoveSpeed * moveDirection;
	}

	mViewMatrix = math::GetViewMatrixFromLookAt<float>(mPosition, mPosition + mFront, mTop);
}

void ZCamera::PitchOffset(int yoffset)
{
	mPitch += yoffset * mSensitivity;
	
	//限制旋转的极限值
	if (mPitch >= 89.0f) mPitch = 89.0f;
	if (mPitch <= -89.0f) mPitch = -89.0f;

	//更新mFront
	mFront.Y = sin(DEGTORAD(mPitch));
	mFront.X = cos(DEGTORAD(mYaw)) * cos(DEGTORAD(mPitch));
	mFront.Z = sin(DEGTORAD(mYaw)) * cos(DEGTORAD(mPitch));
	mFront = math::normalize(mFront);
}

void ZCamera::YawOffset(int xoffset)
{
	mYaw += xoffset * mSensitivity;

	//更新mFront
	mFront.Y = sin(DEGTORAD(mPitch));
	mFront.X = cos(DEGTORAD(mYaw)) * cos(DEGTORAD(mPitch));
	mFront.Z = sin(DEGTORAD(mYaw)) * cos(DEGTORAD(mPitch));
	mFront = math::normalize(mFront);
}
