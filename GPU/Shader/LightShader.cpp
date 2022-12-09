#include "LightShader.h"

LightShader::LightShader() {}

LightShader::~LightShader() {}

VsOutPoint LightShader::VertexShader(const std::map<uint32_t, BindingDescription>& invaoMap, const std::map<uint32_t, BufferObject*>& invboMap, const uint32_t& index)
{
	math::vec4f positiondata = GetVector(invaoMap, invboMap, 0, index);	//直接调用父类函数读取index位置的position属性数据
	positiondata.W = 1.0f;
	math::vec4f normaldata = GetVector(invaoMap, invboMap, 1, index);	//直接调用父类函数读取index位置的normal属性数据
	math::vec4f uvdata = GetVector(invaoMap, invboMap, 2, index);	//直接调用父类函数读取index位置的uv属性数据

	VsOutPoint tempResult;
	tempResult.mPosition = mPerspectiveMatrix * mViewMatrix * mModeMatrix * positiondata;
	tempResult.mNormal = normaldata;
	tempResult.mUV = uvdata;
	return tempResult;
}

void LightShader::FragmentShader(const VsOutPoint& inVspoints, FsOutPoint& outPoints, const std::map<uint32_t, ZTexture*>& textures)
{
	outPoints.mPosition.X = inVspoints.mPosition.X;
	outPoints.mPosition.Y = inVspoints.mPosition.Y;
	outPoints.mDepth = inVspoints.mPosition.Z;

	math::vec4f diffuseColor;
	auto normal = math::normalize(inVspoints.mNormal);
	auto lightDirection = math::normalize(mDirectionLight.GetLightDirection());
	float cosTheta = math::dot(normal, -lightDirection);
	cosTheta = math::Clamp(cosTheta, 0.0f, 1.0f);

	auto iter = textures.find(mDiffuseTexture); 
	ZTexture* texture{ nullptr };
	if (iter != textures.end())	texture = iter->second;
	math::vec4f texColor;
	if (texture) { texColor = texture->GetColor(inVspoints.mUV.X, inVspoints.mUV.Y); }

	diffuseColor = texColor * cosTheta * math::vec4f(mDirectionLight.GetLightColor(), 1.0f);

	math::vec4f envColor;
	envColor = texColor * math::vec4f(mEnvLight, 1.0f);

	outPoints.mColor = VectorToRGBA(diffuseColor + envColor);
}