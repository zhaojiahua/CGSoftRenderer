#pragma once
#include "ShaderBase.h"
#include "../../math/math.h"
#include "../../application/light.h"

class LightShader : public ShaderBase
{
public:
	LightShader();
	~LightShader();

	VsOutPoint VertexShader(const std::map<uint32_t, BindingDescription>& invaoMap, const std::map<uint32_t, BufferObject*>& invboMap, const uint32_t& index)override;
	void FragmentShader(const VsOutPoint& inVspoints, FsOutPoint& outPoints, const std::map<uint32_t, ZTexture*>& textures)override;

	math::Mat4f mModeMatrix;
	math::Mat4f mViewMatrix;
	math::Mat4f mPerspectiveMatrix;

	uint32_t mDiffuseTexture{ 0 };		//默认的一张漫反射贴图
	DirectionLight* mDirectionLight;		//默认一束平行光 
	math::vec3f mEnvLight{ 0.1,0.1,0.1 };	//环境光默认暗淡的白色光

};