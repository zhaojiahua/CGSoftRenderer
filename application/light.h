#pragma once 
#include "../math/math.h"

class ZLightBase
{
	math::vec3f lightColor{ 1.0f,1.0f,1.0f };	//默认白色光源
	math::vec3f lightDirection{ 0.0f,0.0f,-1.0f };	//默认朝向Z轴负方向
public:
	ZLightBase() {}
	~ZLightBase() {}

	math::vec3f GetLightColor() { return lightColor; }
	math::vec3f GetLightDirection() { return lightDirection; }

	void SetLightColor(const math::vec3f& incolor) { lightColor = incolor; }
	void SetLightDirection(const math::vec3f& indirection) { lightDirection = indirection; }

private:

};

//平行光
class DirectionLight : public ZLightBase
{
public:
	DirectionLight() {}
	~DirectionLight() {}
};