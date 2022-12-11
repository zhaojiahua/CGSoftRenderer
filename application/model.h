#pragma once
#include "../math/math.h"
#include "../GPU/Shader/LightShader.h"
#include "image.h"

#include "../ThirdParty/include/assimp/Importer.hpp"
#include "../ThirdParty/include/assimp/scene.h"
#include "../ThirdParty/include/assimp/postprocess.h"


struct ZVertex
{
	math::vec3f position;
	math::vec3f normal;
	math::vec2f texCoords;
};

class ZMesh;

//Model类(一个Model包含n个Mesh)
class ZModel
{
	std::string mRootPath{};//当前模型的根目录
	ZMesh* mRootMesh{ nullptr };//根节点Mesh
	std::vector<ZMesh*> mMeshes;//存储模型的所有Mesh

	//这个模型包含的纹理贴图
	std::map<std::string, uint32_t> mTextrueCache;

public:
	ZModel();
	~ZModel();

	void Read(const std::string& path);

	void DrawModel(LightShader* shader);

	void SetModelMatrix(const math::Mat4f& m);

private:
	//处理assimp的Node
	void ProcessNode(ZMesh* parentMesh, aiNode* aninode, const aiScene* scene);
	//转换assimp的矩阵
	math::Mat4f GetMatFromAinode(aiMatrix4x4 invalue);
	//处理assimp的aiMesh得到一个ZMesh
	ZMesh* ProcessMesh(aiMesh* inaimesh, const aiScene* inScene);
	//处理assimp的aiMaterial得到纹理贴图
	uint32_t ProcessTexture(const aiMaterial* inmat, const aiTextureType& texType, const aiScene* inScene);
	//根据ZImage数据绑定Texture内存地址并生成texture句柄
	uint32_t CreateTexture(ZImage* inimage);
};

//Mesh类(一个Mesh包含n个三角面)
class ZMesh
{
	friend class ZModel;
	uint32_t mVAO{ 0 };
	uint32_t mVBO{ 0 };
	uint32_t mEBO{ 0 };
	uint32_t mTexture{ 0 };	//Mesh自身携带的一张贴图
	uint32_t mIndiceCount{ 0 };		//Mesh的顶点序号总数

	math::Mat4f mLocalMatrix;	//自身相对于父节点的空间变换矩阵
	std::vector<ZMesh*> mChildren;	//自身作为父节点,其下面的所有子节点

public:
	//构造函数(需要提供这个Mesh的所有顶点,这个Mesh的顶点序号列表,这个Mesh的纹理贴图,还有这个Mesh的相对父节点的变换矩阵)
	ZMesh(
		const std::vector<ZVertex>& invertices,
		const std::vector<uint32_t>& indices,
		uint32_t diffuseTexture,
		const math::Mat4f& localMatrix
	);
	~ZMesh();

	//添加子Mesh节点
	void AddChildMesh(ZMesh* inmesh);
	//绘制这个Mesh,需要提供它的变换矩阵和一个shader(如果不需要变换默认矩阵是单位矩阵)
	void DrawMesh(const math::Mat4f& perMatrix, LightShader* shader);

};