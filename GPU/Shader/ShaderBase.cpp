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
	ZRGBA tempResult;
	tempResult.zR = static_cast<uint32_t>(inv.X * 255);
	tempResult.zG = static_cast<uint32_t>(inv.Y*255);
	tempResult.zB = static_cast<uint32_t>(inv.Z*255);
	tempResult.zA = static_cast<uint32_t>(inv.W*255);
	return tempResult;
}
