#pragma once
#include "zvector.h"
namespace math {
	//标量*向量的运算符重载
	template<typename T,typename S>
	inline ZVector2D<T> operator*(S s, const ZVector2D<T>& v) {
		return v * s;
	}
	template<typename T, typename S>
	inline ZVector3D<T> operator*(S s, const ZVector3D<T>& v) {
		return v * s;
	}
	template<typename T, typename S>
	inline ZVector4D<T> operator*(S s, const ZVector4D<T>& v) {
		return v * s;
	}
	//向量*向量的运算符重载
	template<typename T>
	inline ZVector2D<T> operator*(const ZVector2D<T>& v1, const ZVector2D<T>& v2) {
		return ZVector2D(v1.X * v2.X, v1.Y * v2.Y);
	}
	template<typename T>
	inline ZVector3D<T> operator*(const ZVector3D<T>& v1, const ZVector3D<T>& v2) {
		return ZVector3D(v1.X * v2.X, v1.Y * v2.Y,v1.Z*v2.Z);
	}
	template<typename T>
	inline ZVector4D<T> operator*(const ZVector4D<T>& v1, const ZVector4D<T>& v2) {
		return ZVector4D(v1.X * v2.X, v1.Y * v2.Y, v1.Z * v2.Z, v1.W * v2.W);
	}
	//取绝对值运算
	template<typename T>
	inline ZVector2D<T> abs(const ZVector2D<T>& v) {
		return ZVector2D(std::abs(v.X), std::abs(v.Y));
	}
	template<typename T>
	inline ZVector3D<T> abs(const ZVector3D<T>& v) {
		return ZVector3D(std::abs(v.X), std::abs(v.Y), std::abs(v.Z));
	}
	template<typename T>
	inline ZVector4D<T> abs(const ZVector4D<T>& v) {
		return ZVector4D(std::abs(v.X), std::abs(v.Y), std::abs(v.Z), std::abs(v.W));
	}
	//向量的点乘运算
	template<typename T>
	inline T dot(const ZVector2D<T>& v1, const ZVector2D<T>& v2) {
		return v1.X * v2.X + v1.Y * v2.Y;
	}
	template<typename T>
	inline T dot(const ZVector3D<T>& v1, const ZVector3D<T>& v2) {
		return v1.X * v2.X + v1.Y * v2.Y + v1.Z * v2.Z;
	}
	template<typename T>
	inline T dot(const ZVector4D<T>& v1, const ZVector4D<T>& v2) {
		return v1.X * v2.X + v1.Y * v2.Y + v1.Z * v2.Z + v1.W * v2.W;
	}
	//向量的叉乘运算
	template<typename T>
	inline T cross(const ZVector2D<T>& v1, const ZVector2D<T>& v2) {
		return v1.X * v2.Y - v1.Y * v2.X;
	}
	template<typename T>
	inline ZVector3D<T>cross(const ZVector3D<T>& v1, const ZVector3D<T>& v2) {
		return ZVector3D(
			v1.Y * v2.Z - v1.Z * v2.Y,
			v1.Z * v2.X - v1.X * v2.Z,
			v1.X * v2.Y - v1.Y * v2.X
		);
	}
	//向量模长的平方
	template<typename T>
	inline float lenghtSquared(const ZVector2D<T>& v) {
		return v.X * v.X + v.Y * v.Y;
	}
	template<typename T>
	inline float lenghtSquared(const ZVector3D<T>& v) {
		return v.X * v.X + v.Y * v.Y + v.Z * v.Z;
	}
	template<typename T>
	inline float lenghtSquared(const ZVector4D<T>& v) {
		return v.X * v.X + v.Y * v.Y + v.Z * v.Z + v.W * v.W;
	}
	//向量的模长
	template<typename T>
	inline float length(const ZVector2D<T>& v) {
		return std::sqrt(lenghtSquared(v));
	}
	template<typename T>
	inline float length(const ZVector3D<T>& v) {
		return std::sqrt(lenghtSquared(v));
	}
	template<typename T>
	inline float length(const ZVector4D<T>& v) {
		return std::sqrt(lenghtSquared(v));
	}
	//向量的归一化
	template<typename T>
	inline ZVector2D<T>normalize(const ZVector2D<T>& v) {
		return v / length(v);
	}
	template<typename T>
	inline ZVector3D<T>normalize(const ZVector3D<T>& v) {
		return v / length(v);
	}
	template<typename T>
	inline ZVector4D<T>normalize(const ZVector4D<T>& v) {
		return v / length(v);
	}
}