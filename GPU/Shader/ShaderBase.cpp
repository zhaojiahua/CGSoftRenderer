#include "ShaderBase.h"

ShaderBase::ShaderBase(){}

ShaderBase::~ShaderBase(){}

math::vec4f ShaderBase::GetVector(const std::map<uint32_t, BindingDescription>& invaoMap, const std::map<uint32_t, BufferObject*>& invboMap, const uint32_t& attributeLoc, const uint32_t& index)
{
	auto iter_VAO = invaoMap.find(attributeLoc);
	if (iter_VAO == invaoMap.end()) assert(false);
	auto description = iter_VAO->second;
	auto iter_VBO = invboMap.find(description.mVBOID);
	if (iter_VBO == invboMap.end()) assert(false);
	BufferObject* vbo = iter_VBO->second;
	math::vec4f tempResult;
	auto tempsrc = vbo->GetBufferData() + description.mStride * index + description.mOffset;
	memcpy(&tempResult, tempsrc, description.mItemSize * sizeof(float));
	return tempResult;
}

ZRGBA ShaderBase::VectorToRGBA(const math::vec4f& inv)
{
	//防止颜色越界
	math::vec4f tempV;
	tempV.X = math::Clamp(inv.X, 0.0f, 1.0f);
	tempV.Y = math::Clamp(inv.Y, 0.0f, 1.0f);
	tempV.Z = math::Clamp(inv.Z, 0.0f, 1.0f);
	tempV.W = math::Clamp(inv.W, 0.0f, 1.0f);

	ZRGBA tempResult;
	tempResult.zR = static_cast<uint32_t>(tempV.X * 255);
	tempResult.zG = static_cast<uint32_t>(tempV.Y*255);
	tempResult.zB = static_cast<uint32_t>(tempV.Z*255);
	tempResult.zA = static_cast<uint32_t>(tempV.W*255);
	return tempResult;
}
