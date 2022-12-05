#pragma once
#include "../global/base.h"
#include "dataStruct.h"

class VertexArrayObject
{
public:
	VertexArrayObject();
	~VertexArrayObject();

	void Set(uint32_t bindingKey, uint32_t vboID, size_t itemSize, size_t stride, size_t offset);
	std::map<uint32_t, BindingDescription> GetBindingMap()const;

private:
	std::map<uint32_t, BindingDescription> mBindingMap;
};