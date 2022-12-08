#pragma once
#include "../global/base.h"
#include "frameBuffer.h"
#include "../application/application.h"
#include "../application/image.h"
#include "../math/math.h"
#include "dataStruct.h"
#include "Shader/ShaderBase.h"
#include "texture.h"

class BufferObject;
class VertexArrayObject;

/*
class  GPU
这个类用来模拟GPU的行为和算法等
是个单例类
*/
class ZGPU
{
	static ZGPU* mZGPUInstance;
	FrameBuffer* mFrameBuffer{ nullptr };
	ZImage* textrue{ nullptr };

	//临近采样采样图片纹理
	ZRGBA NearestSimple(const math::vec2f& inuv);
	//双线性差值采样图片纹理
	ZRGBA BilinearitySimple(const math::vec2f& inuv);
	//check UV值
	void CheckValue(float& inValue);

	void Trim(VsOutPoint& inPt);

public:
	ZGPU();
	~ZGPU();
	static ZGPU* GetZGPUInstance();	//单例模式,获取此类的实例
	
	//接受外界传入的位图指针以及窗体的宽和高
	void InitSurface(const uint32_t& inWidth, const uint32_t& inHeight, void* bufferPtr = nullptr);

	//清除画布
	void Clear();

	//传入像素位置和颜色并绘制
	void DrawPoint(const int32_t& inx, const int32_t& iny, const ZRGBA& incolor);

	//传入两个像素点绘制直线
	void DrawLine(const VsOutPoint& startPoint, const VsOutPoint& endPoint);

	//传入纹理贴图
	void SetTextrue(ZImage* inIma);

	//传入三个像素点绘制一个三角形
	void DrawTriangle(const VsOutPoint& p1, const VsOutPoint& p2, const VsOutPoint& p3);

	//传入ZImage数据绘制图像
	void DrawZImage(const ZImage* inImg);

	//传入ZImage数据和透明度绘制半透明图像
	void DrawZImage(const ZImage* inImg, const uint32_t& ina);

	//渲染管线架构相关
	uint32_t GenerateVertexBuffer();
	void DeleteVertexBuffer(const uint32_t inBufferCounter);
	uint32_t GenerateVertexArray();
	void DeleteVertexArray(const uint32_t inBufferCounter);
	//提供VBO绑定操作接口
	void BindingVertexBuffer(const uint32_t& bufferType, const uint32_t& bufferID);		//绑定VBO的类型在global.base里面提供(有顶点属性Buffer和顶点索引Buffer)
	void BufferUpData(const uint32_t& bufferType, size_t dataSize, void* data);	//更新BufferData 数据转移到GPU缓存
	//提供VAO绑定操作接口
	void BingdingVertexArray(const uint32_t& vaoID);
	void VertexAttributePointer(const uint32_t& bindingKey, const uint32_t& itemSize, const uint32_t& stride, const uint32_t& offset);	//创建对应VBO缓存的描述 

	void PrintVAO(const uint32_t& inVAO);	//传入一个VAO句柄打印VAO信息

	uint32_t GetCurrentVBO() const { return currentVBO; }
	uint32_t GetCurrentEBO() const { return currentEBO; }
	uint32_t GetCurrentVAO() const { return currentVAO; }

	void UseShaderProgram(ShaderBase* inShader);	//指定要使用的shader
	void DrawElement(
		const uint32_t& drawMode,	//指定绘制模式
		const uint32_t& first,	//绘制的开始位置(从第几个索引开始绘制)
		const uint32_t& count	//绘制个数(绘制几个顶点)
	);

	//提供统一的开启状态的接口
	void Enable(const uint32_t& value);
	//提供统一的关闭状态的接口
	void Disable(const uint32_t& value);

	//指定正面朝向是顺时针或逆时针
	void SetFrontFace(const uint32_t& value);
	//指定剔除的是正面还是反面
	void SetCullFace(const uint32_t& value);
	//设置深度测试的方式
	void SetDepthPatter(const uint32_t& value);

	//texture
	uint32_t GenerateTexture();
	void DeleteTexture(const uint32_t& textureID);
	void BindTexture(const uint32_t& textureID);
	void TexImage2D(const uint32_t& width, const uint32_t& height, void* data);
	void TexParameter(const uint32_t& param, const uint32_t& calue);


private:
	//VBO相关/EBO也在其中
	uint32_t mBufferCounter{ 0 };		//记录VBO的数量,同时也是当前Buffer的句柄
	std::map<uint32_t, BufferObject*> mVBOMap;
	//VAO相关
	uint32_t mVAOCounter{ 0 };	//VAO的数量,同时也是当前Buffer的句柄
	std::map<uint32_t, VertexArrayObject*> mVAOMap;

	uint32_t currentVBO{ 0 };		//当前VBO句柄
	uint32_t currentEBO{ 0 };		//当前EBO句柄
	uint32_t currentVAO{ 0 };	//当前VAO句柄

	void VertexShaderStage(std::vector<VsOutPoint>& outvsPoints, const VertexArrayObject* vao, const BufferObject* vbo, const uint32_t first, const uint32_t count);
	void PerspectiveDivision(VsOutPoint& vsPoints);	//透视除法
	void PerspectiveRecover(VsOutPoint& vsPoints);	//透视校正
	void ScreenMapping(VsOutPoint& vsPoints);	//屏幕像素化
	bool DepthTest(const FsOutPoint& outpt);	//深度测试函数
	ZRGBA BlendColor(const FsOutPoint& inpt);	//颜色混合函数

	ShaderBase* mShader{ nullptr };
	math::Mat4f mScreenMatrix;	//屏幕空间矩阵,在屏幕参数已知的时候就可以确定了

	//Cull face
	bool mEnableCullFace{ true };	//默认背面剔除开启
	uint32_t mFrontFace{ CCW_FRONT_FACE };	//默认逆时针方向的面为正面
	uint32_t mCullFace{ BACK_FACE };	//默认剔除的是背面朝向的面

	//Depth test
	bool mEnableDepthTest{ true };
	uint32_t mDepthPatter{ DEPTH_LESS };

	//texture
	uint32_t mCurrentTexture{ 0 };
	uint32_t mTextureCounter{ 0 };
	std::map<uint32_t, ZTexture*> mTextureMap;


public:
	bool bEnableBlend{ true };		//是否开启图像融合模式
	bool bUseBilinearity{ false };	//是否使用双线性差值采样纹理
	uint8_t UVwrap = TEXTRUE_WRAP_REPEAT;	//uvwrap的方式
};