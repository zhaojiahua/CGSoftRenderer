#include "VAO.h"

VertexArrayObject::VertexArrayObject(){}

VertexArrayObject::~VertexArrayObject(){}

void VertexArrayObject::Set(uint32_t bindingKey, uint32_t vboID, size_t itemSize, size_t stride, size_t offset)
{
	auto iter = mBindingMap.find(bindingKey);
	if (iter == mBindingMap.end()) {
		iter = mBindingMap.insert(std::make_pair(bindingKey, BindingDescription())).first;
	}
	auto& des = iter->second;
	des.mVBOID = vboID;
	des.mItemSize = itemSize;
	des.mStride = stride;
	des.mOffset = offset;
}

std::map<uint32_t, BindingDescription> VertexArrayObject::GetBindingMap() const
{
	return mBindingMap;
}
