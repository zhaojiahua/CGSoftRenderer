#include "clipper.h"
#include "../math/math.h"

void Clipper::DoClipSpace(const uint32_t& drawMode, const std::vector<VsOutPoint>& primtives, std::vector<VsOutPoint>& outPuts)
{
	outPuts.clear();
	if (drawMode == DRAW_TRIANGLES) {
		std::vector<VsOutPoint> tempPrim;
		std::vector<VsOutPoint> tempResult;
		
		//遍历每一个三角面
		for (uint32_t i = 0; i < primtives.size(); i += 3) {
			tempPrim.clear();
			tempResult.clear();
			auto startP = primtives.begin() + i;		//primtive的第一个点的起始位
			auto endP = primtives.begin() + i + 3;	//primtive的最后一个点的结束位
			tempPrim.assign(startP, endP);
			SutherlandHodgman(drawMode, tempPrim, tempResult);
			if (tempResult.empty()) { continue; }
				
			for (uint32_t j = 0; j < tempResult.size() - 2; ++j) {
				outPuts.push_back(tempResult[0]);
				outPuts.push_back(tempResult[j+1]);
				outPuts.push_back(tempResult[j+2]);
			}
		}
	}
}

bool Clipper::CullFace(const uint32_t& frontFace, const uint32_t& cullface, const VsOutPoint& p0, const VsOutPoint& p1, const VsOutPoint& p2)
{
	math::vec3f v1 = p1.mPosition - p0.mPosition;
	math::vec3f v2 = p2.mPosition - p0.mPosition;
	math::vec3f Dir = math::cross(v1, v2);

	//注意 NDC空间下的是左手坐标系,要用左手定则来判定
	if (cullface == BACK_FACE) {
		if (frontFace == CCW_FRONT_FACE) return Dir.Z > 0;
		else return Dir.Z < 0;
	}
	else {
		if (frontFace == CCW_FRONT_FACE) return Dir.Z < 0;
		else return Dir.Z > 0;
	}
}


void Clipper::SutherlandHodgman(const uint32_t& drawMode, const std::vector<VsOutPoint>& primtive, std::vector<VsOutPoint>& outPuts)
{
	assert(outPuts.size() == 0);		//outPuts一开始要被初始化成未被剪裁之间的三个点(primtive默认是三角形)
	//根据NDC空间坐标值都在-1到1之间得出7个平面限制条件
	std::vector<math::vec4f> clipPlanes = {
		//judge w > 0
		math::vec4f(0.0f, 0.0f, 0.0f, 1.0f),
		//near
		math::vec4f(0.0f, 0.0f, 1.0f, 1.0f),
		//far
		math::vec4f(0.0f, 0.0f, -1.0f, 1.0f),
		//left
		math::vec4f(1.0f, 0.0f, 0.0f, 1.0f),
		//right
		math::vec4f(-1.0f, 0.0f, 0.0f, 1.0f),
		//top
		math::vec4f(0.0f, -1.0f, 0.0f, 1.0f),
		//bottom
		math::vec4f(0.0f, 1.0f, 0.0f, 1.0f)
	};

	outPuts = primtive;		//每次计算出来的结果作为下一个裁剪平面的输入
	std::vector<VsOutPoint> inputs;
	for (uint32_t i = 0; i < clipPlanes.size(); ++i) {
		inputs = outPuts;
		outPuts.clear();
		for (uint32_t p = 0; p < inputs.size(); ++p) {
			auto currentP = inputs[p];
			auto lastP = inputs[(p + inputs.size() - 1) % inputs.size()];			///注意uint32_t是无符号整数,一定要先加再减,如果先减可能会小于0就直接截断成0,从而造成结果出错!!!!!!!!
			if (Inside(currentP.mPosition, clipPlanes[i])) {	//如果当前点在内部
				if (drawMode != DRAW_LINES || p != inputs.size() - 1) {	//如果是画直线的时候且p的取值是最后一个点时是不需要截断差值计算的
					//且前一个点不在内部就进行差值计算并且加入到数组中
					if (!Inside(lastP.mPosition, clipPlanes[i])) {
						auto insertP = Intersect(lastP, currentP, clipPlanes[i]);
						outPuts.push_back(insertP);
					}
				}
				outPuts.push_back(currentP);		//在内部的点时必须的
			}
			else {	//如果当前点在外部
				if (drawMode != DRAW_LINES || p != inputs.size() - 1) {
					if (Inside(lastP.mPosition, clipPlanes[i])) {	//且前一个点在内部
						auto tempinsertP = Intersect(lastP, currentP, clipPlanes[i]);
						outPuts.push_back(tempinsertP);
					}
				}
			}
		}
	}
}

bool Clipper::Inside(const math::vec4f& point, const math::vec4f plane)
{
	return math::dot(point, plane) >= 0.0f;
}

VsOutPoint Clipper::Intersect(const VsOutPoint& lastP, const VsOutPoint& currentP, const math::vec4f plane)
{
	VsOutPoint tempresult;
	float distanceLast = math::dot(lastP.mPosition, plane);
	float distanceCurrent = math::dot(currentP.mPosition, plane);
	float weight = distanceLast / (distanceLast - distanceCurrent);

	tempresult.mPosition = math::Lerp(lastP.mPosition, currentP.mPosition, weight);
	tempresult.mColor= math::Lerp(lastP.mColor, currentP.mColor, weight);
	tempresult.mUV= math::Lerp(lastP.mUV, currentP.mUV, weight);
	tempresult.mNormal = math::Lerp(lastP.mNormal, currentP.mNormal, weight);

	return tempresult;
}
