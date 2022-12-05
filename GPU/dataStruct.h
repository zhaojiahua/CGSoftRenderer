#pragma once 
#include "../global/base.h"
#include "../math/math.h"

//VAO数据结构,用来存储vertexBuffer的存储信息的描述
struct  BindingDescription
{
	uint32_t mVBOID{ 0 };	//相应的VertexBuffer的内存地址
	size_t mItemSize{ 0 };	//每个元素所占的空间字节大小
	size_t mStride{ 0 };	//每个顶点包含元素的个数
	size_t mOffset{ 0 };	//相应属性所在该顶点内存位置中的偏移
};

//vertex shader处理的顶点数据结构(包含顶点的世界空间位置,线性空间颜色,UV等)
struct VsOutPoint
{
	math::vec4f mPosition;
	math::vec4f mColor;		//为了方便计算这里用0-1表示颜色
	math::vec2f mUV;
};

//Fragment Shader处理的屏幕点数据结构
struct FsOutPoint
{
	math::vec2i mPosition; //像素点的屏幕空间位置
	float mDepth;	//点的深度信息
	ZRGBA mColor;	//这里直接用0-255的空间颜色为屏幕绘色
};