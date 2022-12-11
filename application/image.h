#pragma once
/*
image类用来存储从硬盘里面读取的图片数据(中间使用stb_image来读取)
*/
#include "../global/base.h"

class ZImage
{
public:
	ZImage(const uint32_t inW = 0, const uint32_t inH = 0, ZRGBA* indata = nullptr);
	~ZImage();

public:
	uint32_t mWidth{ 0 };	//图片的宽
	uint32_t mHeight{ 0 };	//图片的高
	ZRGBA* mData{ nullptr };	//存储ZRGBA类型颜色的内存区域地址

	static ZImage* CreateZImage(const std::string& imgPath);
	//从内存里读取数据创建ZImage
	static ZImage* CreateZImageFromMemory(std::string path, unsigned char* dataIn, uint32_t widthIn, uint32_t heightIn);
	static void DestroyZImage(ZImage* zimage);

};
