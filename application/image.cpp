#include "image.h"
#define STB_IMAGE_IMPLEMENTATION	//预处理器会修改头文件,让其只包含相关的函数定义源码
#include "stb_image.h"
#include "image.h"

ZImage::ZImage(const uint32_t inW, const uint32_t inH, ZRGBA* indata)
{
	mWidth = inW;
	mHeight = inH;
	if (indata) {
		mData = new ZRGBA[mWidth * mHeight];
		memcpy(mData, indata, sizeof(ZRGBA) * mWidth * mHeight);
	}
}

ZImage::~ZImage()
{
	if (mData) {
		delete[] mData;
	}
}

ZImage* ZImage::CreateZImage(const std::string & imgPath)
{
	int picType = 0;
	int imgWidth = 0, imgHeight = 0;	//用来存储图片的宽高和类型信息
	stbi_set_flip_vertically_on_load(true);		//设置stb读取的时候翻转Y轴信息
	unsigned char* bits = stbi_load(imgPath.c_str(), &imgWidth, &imgHeight, &picType, STBI_rgb_alpha);		//返回一个带颜色信息的char数组
	//windows GDI画布的颜色是BGRA格式的,所以这里要交换B和R的值
	for (unsigned int i = 0; i < imgWidth * imgHeight*4; i+=4) {		//计算机的乘法运算要比加法运算更消耗性能,所以设计循环的时候尽量规避循环过程中使用乘法而多使用加法
		byte tbit = bits[i];
		bits[i] = bits[i + 2];
		bits[i + 2] = tbit;
	}
	ZImage* tImg = new ZImage(imgWidth, imgHeight, (ZRGBA*)bits);
	stbi_image_free(bits);	//释放stb所占用的内存
	return tImg;
}

ZImage* ZImage::CreateZImageFromMemory(std::string path, unsigned char* dataIn, uint32_t widthIn, uint32_t heightIn)
{
	int picType = 0;
	int width{ 0 }, height{ 0 };
	uint32_t dataInSize = 0;
	if (!heightIn)dataInSize = widthIn;
	else dataInSize = widthIn * heightIn;
	unsigned char* bits = stbi_load_from_memory(dataIn, dataInSize, &width, &height, &picType, STBI_rgb_alpha);
	//交换RGB成BGR
	for (int i = 0; i < width * height * 4; i++) {
		byte tmp = bits[i];
		bits[i] = bits[i + 2];
		bits[i + 2] = tmp;
	}
	ZImage* image = new ZImage(width, height, (ZRGBA*)bits);
	return image;
}

void ZImage::DestroyZImage(ZImage* zimage)
{
	if (zimage) {
		delete zimage;
	}
}
