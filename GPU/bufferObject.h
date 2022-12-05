#pragma once
#include "../global/base.h"

class BufferObject
{
public:
	BufferObject();
	~BufferObject();
	BufferObject(const BufferObject& bufferObject2) = delete;	//VBO的不允许操作,不允许拷贝

	void SetBufferData(size_t dataSize, void* data);
	byte* GetBufferData()const;

private:
	size_t mBufferSize{ 0 };	//buffer的大小,单位是byte
	byte* mBufferData{ nullptr };	//buffer空间指针
};