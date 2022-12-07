#pragma once
#include "../dataStruct.h"
#include "../bufferObject.h"
#include "../../math/math.h"

//Shader的基类,后面所有的shader都继承于此
class ShaderBase
{
public:
	ShaderBase();
	~ShaderBase();

	//vertexShader需要传入VAOmap和VBOmap和一个EBO句柄
	virtual VsOutPoint VertexShader(const std::map<uint32_t, BindingDescription>& invaoMap, const std::map<uint32_t, BufferObject*>& invboMap, const uint32_t& index) = 0;

	//FragmentShader 传入来自VertexShader处理完的点,输出离散后的屏幕像素点
	virtual void FragmentShader(const VsOutPoint& inVspoints, FsOutPoint& outPoints) = 0;

//用于处理数据的公用工具
public:
	math::vec4f GetVector(
		const std::map<uint32_t, BindingDescription>& invaoMap, 
		const std::map<uint32_t, BufferObject*>& invboMap, 
		const uint32_t& attributeLoc,
		const uint32_t& index);
	ZRGBA VectorToRGBA(const math::vec4f& inv);
};