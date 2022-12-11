#pragma once 
#include "../global/base.h"
#include "../math/math.h"

class ZTexture
{
	ZRGBA* mBuffer{ nullptr };	//贴图的颜色数据
	uint32_t mWidth{ 0 };	//贴图的宽
	uint32_t mHeight{ 0 };	//贴图的高

	uint32_t mFilter{ TEXTURE_FILTER_LINEAR };	//默认bilinear采样纹理
	uint32_t mWrapU{ TEXTURE_WRAP_REPEAT };	//U向默认重复采样
	uint32_t mWrapV{ TEXTURE_WRAP_REPEAT };	//V向默认重复采样
	uint32_t mTransparency{ TEXTURE_TRANSPARENCY };	//采样纹理的时候默认读取透明通道

public:
	ZTexture();
	~ZTexture();

	void SetBufferData(const uint32_t& inwidth, const uint32_t& inheight, void* data);

	math::vec4f GetColor(float u, float v);

	void SetParameter(const uint32_t& type, const uint32_t& value);

private:
	void CheckWrap(float& n, const uint32_t& type);


};