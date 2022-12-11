#pragma once 
#include "../math/math.h"

class ZLightBase
{
public:
	ZLightBase() {}
	~ZLightBase() {}

private:

};

//平行光
class DirectionLight : public ZLightBase
{
	math::vec3f lightColor;	//默认白色光源
	math::vec3f lightDirection;	//默认朝向Z轴负方向
	
public:
	DirectionLight() {}
	~DirectionLight() {}

	math::vec3f GetLightColor() { return lightColor; }
	math::vec3f GetLightDirection() { return lightDirection; }
	void SetLightColor(const math::vec3f& incolor) { lightColor = incolor; }
	void SetLightDirection(const math::vec3f& indirection) { lightDirection = indirection; }
};