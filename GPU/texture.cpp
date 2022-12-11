#include "texture.h"

ZTexture::ZTexture(){}

ZTexture::~ZTexture()
{
	if (mBuffer) {
		delete[] mBuffer;
		mBuffer = nullptr;
	}
}

void ZTexture::SetBufferData(const uint32_t& inwidth, const uint32_t& inheight, void* data)
{
	if (mBuffer) {
		delete[] mBuffer;
		mBuffer = nullptr;
	}
	mWidth = inwidth; mHeight = inheight;

	if (mBuffer == nullptr) {
		mBuffer = new ZRGBA[mWidth * mHeight];
	}

	memcpy(mBuffer, data, mWidth * mHeight * sizeof(ZRGBA));
}

math::vec4f ZTexture::GetColor(float u, float v)
{
	math::vec4f tempResult;
	ZRGBA tempColor;
	CheckWrap(u,mWrapU);
	CheckWrap(v,mWrapV);

	if (mFilter == TEXTURE_FILTER_NEAREST) {
		int x = std::round(u * (mWidth - 1));
		int y = std::round(v * (mHeight - 1));

		if (x > mWidth || x < 0)
		{
			//std::cout << "u error!" << std::endl;
			x = 0;
		}
		if (y > mHeight || y < 0)
		{
			//std::cout << "v error!" << std::endl;
			y = 0;
		}

		tempColor = mBuffer[y * mWidth + x];
	}
	else if (mFilter == TEXTURE_FILTER_LINEAR) {
		float x = u * static_cast<float>(mWidth - 1);
		float y = v * static_cast<float>(mHeight - 1);

		int l = std::floor(x);
		int r = std::ceil(x);
		int b = std::floor(y);
		int t = std::ceil(y);

		if (l > mWidth || l < 0 || r > mWidth || r < 0)
		{
			//std::cout << "u error!" << std::endl;
			l = r = 0;
		}
		if (b > mHeight || b < 0 || t > mHeight || t < 0)
		{
			//std::cout << "v error!" << std::endl;
			b = t = 0;
		}

		float scale_V = 0.0f;
		if (t == b) scale_V = 1.0;
		else scale_V = (y - static_cast<float>(b)) / static_cast<float>(t - b);

		int lt = t * mWidth + l;
		int lb = b * mWidth + l;
		int rt = t * mWidth + r;
		int rb = b * mWidth + r;

		ZRGBA leftColor = math::Lerp(mBuffer[lb], mBuffer[lt], scale_V);
		ZRGBA rightColor = math::Lerp(mBuffer[rb], mBuffer[rt], scale_V);

		float scale_H = 0.0f;
		if (l == r) scale_H = 1.0f;
		else scale_H= (x - static_cast<float>(l)) / static_cast<float>(r - l);

		tempColor = math::Lerp(leftColor, rightColor, scale_H);
	}
	//color 转换成四维向量输出
	tempResult.X = static_cast<float>(tempColor.zR) / 255.0f;
	tempResult.Y = static_cast<float>(tempColor.zG) / 255.0f;
	tempResult.Z = static_cast<float>(tempColor.zB) / 255.0f;
	if (mTransparency == TEXTURE_TRANSPARENCY)	tempResult.W = static_cast<float>(tempColor.zA) / 255.0f;
	else tempResult.W = 1.0f;

	return tempResult;
}

void ZTexture::SetParameter(const uint32_t& type, const uint32_t& value)
{
	switch (type)
	{
	case TEXTURE_FILTER:
		mFilter = value;
		break;
	case TEXTURE_WRAP_U:
		mWrapU = value;
		break;
	case TEXTURE_WRAP_V:
		mWrapV = value;
		break;
	case TEXTURE_TRANSPARENCYORNOT:
		mTransparency = value;
		break;
	default:
		break;
	}
}

void ZTexture::CheckWrap(float& n, const uint32_t& type)
{
	if (n > 1.0f || n < 0.0f) {
		n = FRACTION(n);
		switch (type)
		{
		case TEXTRUE_WRAP_REPEAT:
			n = FRACTION(n + 1);
			break;
		case TEXTRUE_WRAP_MIRROR:
			n = 1.0f - FRACTION(n + 1);
			break;
		default:
			break;
		}
	}
}
