#include "bufferObject.h"

BufferObject::BufferObject()
{}

BufferObject::~BufferObject()
{
	if (mBufferData) {
		delete[] mBufferData;
	}
}

void BufferObject::SetBufferData(size_t dataSize, void* data)
{
	if (mBufferData != nullptr && mBufferSize < dataSize) {
		delete[] mBufferData;
		mBufferData = nullptr;
	}
	if (mBufferData == nullptr) {
		mBufferData = new byte[dataSize];
		mBufferSize = dataSize;
	}
	memcpy(mBufferData, data, dataSize);
}

byte* BufferObject::GetBufferData() const
{
	return mBufferData;
}
