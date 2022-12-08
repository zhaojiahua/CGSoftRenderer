#pragma once
#include "ShaderBase.h"

//一个最简单的shader,把模型顶点数据经过矩阵变换映射到屏幕空间,仅此而已
class DefaultShader : public ShaderBase
{
public:
	DefaultShader();
	~DefaultShader();

	VsOutPoint VertexShader(const std::map<uint32_t, BindingDescription>& invaoMap, const std::map<uint32_t, BufferObject*>& invboMap, const uint32_t& index) override;
	void FragmentShader(const VsOutPoint& inVspoints, FsOutPoint& outPoints, const std::map<uint32_t, ZTexture*>& textures)override;

public:
	math::Mat4f mModeMatrix;
	math::Mat4f mViewMatrix;
	math::Mat4f mPerspectiveMatrix;
 };