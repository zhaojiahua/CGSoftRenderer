#pragma once
#include "dataStruct.h"

class Clipper
{
public:
	//提供绘制方式,要绘制的面(的所有点),根据剪裁算法剪去视域外的点并添加截断需要差值的点
	static void DoClipSpace(const uint32_t& drawMode, const std::vector<VsOutPoint>& primtives, std::vector<VsOutPoint>& outPuts);

	//背面剔除算法
	static bool CullFace(const uint32_t& frontFace, const uint32_t& cullface, const VsOutPoint& p0, const VsOutPoint& p1, const VsOutPoint& p2);

private:
	//SutherlandHodgman剪裁算法
	static void SutherlandHodgman(const uint32_t& drawMode, const std::vector<VsOutPoint>& primtive, std::vector<VsOutPoint>& outPuts);

	//判断一个点是否在一个平面的内部
	static bool Inside(const math::vec4f& point, const math::vec4f plane);

	//提供当前点和上一个点还有一个平面,然后根据这个裁剪平面计算要插入的新点
	static VsOutPoint Intersect(const VsOutPoint& lastP, const VsOutPoint& currentP, const math::vec4f plane);
};