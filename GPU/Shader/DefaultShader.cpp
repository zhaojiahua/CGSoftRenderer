#include "DefaultShader.h"

DefaultShader::DefaultShader(){}

DefaultShader::~DefaultShader(){}

VsOutPoint DefaultShader::VertexShader(const std::map<uint32_t, BindingDescription>& invaoMap, const std::map<uint32_t, BufferObject*>& invboMap, const uint32_t& index)
{
	math::vec4f positiondata = GetVector(invaoMap, invboMap, 0, index);	//直接调用父类函数读取index位置的position属性数据
	positiondata.W = 1;
	math::vec4f colordata = GetVector(invaoMap, invboMap, 1, index);	//直接调用父类函数读取index位置的color属性数据
	math::vec4f uvdata = GetVector(invaoMap, invboMap, 2, index);	//直接调用父类函数读取index位置的uv属性数据
	VsOutPoint tempResult;
	tempResult.mPosition = mPerspectiveMatrix * mViewMatrix * mModeMatrix * positiondata;
	tempResult.mColor = colordata;
	tempResult.mUV = uvdata;
	return tempResult;
}

void DefaultShader::FragmentShader(const VsOutPoint& inVspoints, FsOutPoint& outPoints, const std::map<uint32_t, ZTexture*>& textures)
{
	outPoints.mPosition.X = inVspoints.mPosition.X;
	outPoints.mPosition.Y = inVspoints.mPosition.Y;
	outPoints.mDepth = inVspoints.mPosition.Z;
	outPoints.mColor = VectorToRGBA(inVspoints.mColor);
}
