#include "model.h"
#include "../GPU/gpu.h"

ZMesh::ZMesh(const std::vector<ZVertex>& invertices, const std::vector<uint32_t>& indices, uint32_t diffuseTexture, const math::Mat4f& localMatrix)
{
	mLocalMatrix = localMatrix;
	mTexture = diffuseTexture;

	if (indices.empty() || invertices.empty()) return;
	mIndiceCount = indices.size();

	//绑定内存
	mVAO = Sgl->GenerateVertexArray();
	mVBO = Sgl->GenerateVertexBuffer();
	mEBO = Sgl->GenerateVertexBuffer();

	Sgl->BindingVertexBuffer(ELEMENT_VERTEXT_ARRAY_BUFFER, mEBO);
	Sgl->BufferUpData(ELEMENT_VERTEXT_ARRAY_BUFFER, mIndiceCount * sizeof(uint32_t), (void*)&indices[0]);

	Sgl->BindingVertexBuffer(VERTEXT_ARRAY_BUFFER, mVBO);
	Sgl->BufferUpData(VERTEXT_ARRAY_BUFFER, invertices.size() * sizeof(ZVertex), (void*)&invertices[0]);

	Sgl->BingdingVertexArray(mVAO);
	Sgl->VertexAttributePointer(0,3,sizeof(ZVertex),0);
	Sgl->VertexAttributePointer(1, 3, sizeof(ZVertex), 3 * sizeof(float));
	Sgl->VertexAttributePointer(2, 2, sizeof(ZVertex), 6 * sizeof(float));
	Sgl->BingdingVertexArray(0);
}

ZMesh::~ZMesh()
{
	if (mVAO) Sgl->DeleteVertexArray(mVAO);
	if (mVBO) Sgl->DeleteVertexBuffer(mVBO);
	if (mEBO) Sgl->DeleteVertexBuffer(mEBO);
}

void ZMesh::AddChildMesh(ZMesh* inmesh)
{
	mChildren.push_back(inmesh);
}

void ZMesh::DrawMesh(const math::Mat4f& perMatrix, LightShader* shader)
{
	auto modelMatrix = perMatrix * mLocalMatrix;
	if (mVAO)	{
		shader->mModeMatrix = modelMatrix;
		if (mTexture) {
			shader->mDiffuseTexture = mTexture;
		}
		Sgl->BingdingVertexArray(mVAO);
		Sgl->BindingVertexBuffer(ELEMENT_VERTEXT_ARRAY_BUFFER, mEBO);
		Sgl->DrawElement(DRAW_TRIANGLES, 0, mIndiceCount);
		Sgl->BingdingVertexArray(0);
	}
	//绘制所有的子节点Mesh
	for (auto mesh : mChildren) {
		mesh->DrawMesh(modelMatrix, shader);
	}
}

ZModel::ZModel(){}

ZModel::~ZModel()
{
	for (auto& mesh : mMeshes)delete mesh;
	for (auto& iter : mTextrueCache)Sgl->DeleteTexture(iter.second);
}

void ZModel::Read(const std::string& path)
{
	std::size_t lastIndex = path.find_last_of("//");
	mRootPath = path.substr(0, lastIndex + 1);

	//读取数据
	Assimp::Importer importer;
	const aiScene* currentScene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);
	mRootMesh = new ZMesh({}, {}, 0, math::Mat4f());//一个空节点作为所有Mesh的父节点
	if (!currentScene || currentScene->mFlags& AI_SCENE_FLAGS_INCOMPLETE || !currentScene->mRootNode) {
		std::cout << "read model failed!" << std::endl;
		return;
	}
	ProcessNode(mRootMesh, currentScene->mRootNode, currentScene);
}

void ZModel::DrawModel(LightShader* shader)
{
	mRootMesh->DrawMesh(math::Mat4f(), shader);
}

void ZModel::SetModelMatrix(const math::Mat4f& m)
{
	mRootMesh->mLocalMatrix = m;
}

void ZModel::ProcessNode(ZMesh* parentMesh, aiNode* aninode, const aiScene* scene)
{
	auto node = new ZMesh({}, {}, 0, math::Mat4f());//一个新的空节点用来存储要读取的Mesh
	parentMesh->AddChildMesh(node);
	node->mLocalMatrix = GetMatFromAinode(aninode->mTransformation);//assimp库的变换矩阵是以行为向量的,所以这里要转换一下
	for (unsigned int i = 0; i < aninode->mNumMeshes; i++) {
		aiMesh* aimesh = scene->mMeshes[aninode->mMeshes[i]];
		auto mesh = ProcessMesh(aimesh, scene);
		mMeshes.push_back(mesh);
		node->AddChildMesh(mesh);
	}
	//重复处理它的子节点
	for (unsigned int i = 0; i < aninode->mNumChildren; i++) {
		ProcessNode(node, aninode->mChildren[i], scene);
	}
}

math::Mat4f ZModel::GetMatFromAinode(aiMatrix4x4 invalue)
{
	math::Mat4f tempResult;
	tempResult.Set(
		invalue.a1, invalue.a2, invalue.a3, invalue.a4,
		invalue.b1, invalue.b2, invalue.b3, invalue.b4,
		invalue.c1, invalue.c2, invalue.c3, invalue.c4,
		invalue.d1, invalue.d2, invalue.d3, invalue.d4
	);
	return tempResult;
}

ZMesh* ZModel::ProcessMesh(aiMesh* inaimesh, const aiScene* inScene)
{
	std::vector<ZVertex>vertices;
	std::vector<uint32_t>indices;
	uint32_t diffuseTexture;
	//处理顶点位置 法线和uv
	for (unsigned int i = 0; i < inaimesh->mNumVertices; i++) {
		ZVertex tempVex;
		math::vec3f tempVec;

		tempVec.X = inaimesh->mVertices[i].x;
		tempVec.Y = inaimesh->mVertices[i].y;
		tempVec.Z = inaimesh->mVertices[i].z;
		tempVex.position = tempVec;

		tempVec.X = inaimesh->mNormals[i].x;
		tempVec.Y = inaimesh->mNormals[i].y;
		tempVec.Z = inaimesh->mNormals[i].z;
		tempVex.normal = tempVec;

		if (inaimesh->mTextureCoords[0])//如果有纹理坐标(每个Mesh可能会有多套UV坐标)
		{
			math::vec2f tempVec;
			tempVec.X = inaimesh->mTextureCoords[0][i].x;
			tempVec.Y = inaimesh->mTextureCoords[0][i].y;
			tempVex.texCoords = tempVec;
		}
		else tempVex.texCoords = math::vec2f(0.0f, 0.0f);
		vertices.push_back(tempVex);
	}
	//处理索引
	for (unsigned int i = 0; i < inaimesh->mNumFaces; i++) {
		aiFace face = inaimesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++) {
			indices.push_back(face.mIndices[j]);
		}
	}
	//取出材质
	if (inaimesh->mMaterialIndex >= 0) {//如果有材质
		aiMaterial* mat = inScene->mMaterials[inaimesh->mMaterialIndex];
		diffuseTexture = ProcessTexture(mat, aiTextureType_DIFFUSE, inScene);
	}

	return new ZMesh(vertices, indices, diffuseTexture, math::Mat4f());
}

uint32_t ZModel::ProcessTexture(const aiMaterial* inmat, const aiTextureType& texType, const aiScene* inScene)
{
	uint32_t tempResult;
	aiString aiPath;
	ZImage* zimage = nullptr;
	inmat->Get(AI_MATKEY_TEXTURE(texType, 0), aiPath);//拿到texType类型的第0张贴图
	if (!aiPath.length)return 0;
	//先检查缓存里面是否有这个纹理贴图
	auto iter = mTextrueCache.find(std::string(aiPath.C_Str()));
	if (iter != mTextrueCache.end()) return iter->second;//如果在缓存里面找到了直接将这个贴图返回出去就行了
	//如果没有找到就要考虑生成一张贴图
	//首先看看Scene里面是否有EmbeddedTexture(有的贴图可能会被嵌入到资产里面)
	const aiTexture* assimpTex = inScene->GetEmbeddedTexture(aiPath.C_Str());
	if (assimpTex) {
		unsigned char* dataIn = reinterpret_cast<unsigned char*>(assimpTex->pcData);//把里面的数据转换成char*处理
		uint32_t widthIn = assimpTex->mWidth;
		uint32_t heightIn = assimpTex->mHeight;
		std::string path = aiPath.C_Str();
		ZImage* tempImage = ZImage::CreateZImageFromMemory(path, dataIn, widthIn, heightIn);
		tempResult = CreateTexture(tempImage);
		ZImage::DestroyZImage(tempImage);
	}
	else {//如果Scene里面没有嵌入的纹理贴图,那么就要按照路径去到硬盘里面读取了
		std::string fullPath = mRootPath + aiPath.C_Str();
		ZImage* tempimage = ZImage::CreateZImage(fullPath);
		tempResult = CreateTexture(tempimage);
		ZImage::DestroyZImage(tempimage);
	}
	mTextrueCache.insert(std::make_pair(aiPath.C_Str(), tempResult));
	return tempResult;
}

uint32_t ZModel::CreateTexture(ZImage* inimage)
{
	uint32_t texture = Sgl->GenerateTexture();
	Sgl->BindTexture(texture);
	Sgl->TexImage2D(inimage->mWidth, inimage->mHeight, inimage->mData);
	Sgl->SetTexParameter(TEXTURE_FILTER,TEXTURE_FILTER_LINEAR);
	Sgl->SetTexParameter(TEXTURE_WRAP_U,TEXTRUE_WRAP_REPEAT);
	Sgl->SetTexParameter(TEXTURE_WRAP_V,TEXTRUE_WRAP_REPEAT);
	Sgl->BindTexture(0);
	return texture;
}
