#pragma once
#include "ShaderBase.h"
#include "../../math/math.h"

class TextureShader : public ShaderBase
{

public:
	TextureShader();
	~TextureShader();

	VsOutPoint VertexShader(const std::map<uint32_t, BindingDescription>& invaoMap, const std::map<uint32_t, BufferObject*>& invboMap, const uint32_t& index)override;
	void FragmentShader(const VsOutPoint& inVspoints, FsOutPoint& outPoints, const std::map<uint32_t, ZTexture*>& textures)override;

	math::Mat4f mModeMatrix;
	math::Mat4f mViewMatrix;
	math::Mat4f mPerspectiveMatrix;

	uint32_t mDiffuseTexture{ 0 };		//默认的一张漫反射贴图
};