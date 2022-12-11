#pragma once
#include <assert.h>
#include <iostream>

namespace math {
	template <typename T>
	class ZVector3D;
	template <typename T>
	class ZVector4D;
	template <typename T>
	class ZVector2D
	{
	public:
		T X;
		T Y;
		ZVector2D() : X(0), Y(0) {}
		ZVector2D(T inx, T iny) : X(inx), Y(iny) {}
		ZVector2D(const ZVector2D<T>& inVector2D) :X(inVector2D.X), Y(inVector2D.Y) {}
		ZVector2D(const ZVector3D<T>& inVector3D) :X(inVector3D.X), Y(inVector3D.Y) {}
		ZVector2D(const ZVector4D<T>& inVector4D) : X(inVector4D.X), Y(inVector4D.Y) {}
		//运算符重载
		T operator[](int index) const {
			assert(index>=0&& index<2);
			if (index == 0)return X;
			return Y;
		}
		T& operator[](int index) {
			assert(index >= 0 && index < 2);
			if (index == 0)return X;
			return Y;
		}
		ZVector2D<T> operator=(const ZVector3D<T>& inV) {
			X = inV.X; Y = inV.Y;
			return *this;
		}
		ZVector2D<T> operator=(const ZVector4D<T>& inV) {
			X = inV.X; Y = inV.Y;
			return *this;
		}
		ZVector2D<T> operator+(const ZVector2D<T>& inV) {
			return ZVector2D(X + inV.X, Y + inV.Y);
		}
		ZVector2D<T> operator+=(const ZVector2D<T>& inV) {
			X += inV.X; Y += inV.Y;
			return *this;
		}
		ZVector2D<T> operator*(T inV) const {
			return ZVector2D(X * inV, Y * inV);
		}
		ZVector2D<T> operator*=(const T& inV) {
			X *= inV; Y *= inV;
			return *this;
		}
		ZVector2D<T> operator/(T inV) const {
			assert(inV != 0);
			float f = static_cast<float> (1) / inV;
			return ZVector2D(X * f, Y * f);
		}
		ZVector2D<T> operator/=(T inV) {
			assert(inV != 0);
			float f = static_cast<T> (1) / inV;
			X *= f; Y *= f;
			return *this;
		}
		ZVector2D<T> operator-() {
			return ZVector2D(-X, -Y);
		}
		ZVector2D<T> operator-(const ZVector2D<T>& inV) const {
			return ZVector2D(X - inV.X, Y - inV.Y);
		}
		ZVector2D<T> operator-=(const ZVector2D<T>& inV) {
			X -= inV.X; Y -= inV.Y;
			return *this;
		}
		bool operator==(const ZVector2D<T>& inV) {
			if (X == inV.X && Y == inV.Y)return true;
			return false;
		}
		void print() {
			std::cout << "(" << X << "," << Y << ")" << std::endl;
		}
	};

	template <typename T>
	class ZVector3D
	{
	public:
		T X;
		T Y;
		T Z;
		ZVector3D() : X(0), Y(0), Z(0) {}
		ZVector3D(T inx, T iny, T inz) : X(inx), Y(iny), Z(inz) {}
		ZVector3D(const ZVector2D<T>& inVector2D) : X(inVector2D.X), Y(inVector2D.Y), Z(0) {}
		ZVector3D(const ZVector3D<T>& inVector3D) : X(inVector3D.X), Y(inVector3D.Y), Z(inVector3D.Z) {}
		ZVector3D(const ZVector4D<T>& inVector4D) : X(inVector4D.X), Y(inVector4D.Y), Z(inVector4D.Z) {}
		//运算符重载
		T operator[](int index) const {
			assert(index >= 0 && index < 3);
			if (index == 0)return X;
			else if (index == 1) return Y;
			return Z;
		}
		T& operator[](int index) {
			assert(index >= 0 && index < 3);
			if (index == 0)return X;
			else if (index == 1) return Y;
			return Z;
		}
		ZVector3D<T> operator=(const ZVector2D<T>& inV) {
			X = inV.X; Y = inV.Y;
			return *this;
		}
		ZVector3D<T> operator=(const ZVector4D<T>& inV) {
			X = inV.X; Y = inV.Y; Z = inV.Z;
			return *this;
		}
		ZVector3D<T> operator+(const ZVector3D<T>& inV) {
			return ZVector3D(X + inV.X, Y + inV.Y, Z + inV.Z);
		}
		ZVector3D<T> operator+=(const ZVector3D<T>& inV) {
			X += inV.X; Y += inV.Y; Z += inV.Z;
			return *this;
		}
		ZVector3D<T> operator*(const T& inV) const {
			return ZVector3D(X * inV, Y * inV, Z * inV);
		}
		ZVector3D<T> operator*=(const T& inV) {
			X *= inV; Y *= inV; Z *= inV;
			return *this;
		}
		ZVector3D<T> operator/(T inV) const {
			assert(inV != 0);
			float f = static_cast<float> (1) / inV;
			return ZVector3D(X * f, Y * f, Z * f);
		}
		ZVector3D<T> operator/=(T inV) {
			assert(inV != 0);
			float f = static_cast<float> (1) / inV;
			X *= f; Y *= f; Z *= f;
			return *this;
		}
		ZVector3D<T> operator-() {
			return ZVector3D(-X, -Y, -Z);
		}
		ZVector3D<T> operator-(const ZVector3D<T>& inV) const {
			return ZVector3D(X - inV.X, Y - inV.Y, Z - inV.Z);
		}
		ZVector3D<T> operator-=(const ZVector3D<T>& inV) {
			X -= inV.X; Y -= inV.Y; Z -= inV.Z;
			return *this;
		}
		bool operator==(const ZVector3D<T>& inV) const {
			if (X == inV.X && Y == inV.Y && Z == inV.Z)return true;
			return false;
		}
		void print() {
			std::cout << "(" << X << "," << Y << "," << Z << ")" << std::endl;
		}
	};

	template <typename T >
	class ZVector4D
	{
	public:
		T X;
		T Y;
		T Z;
		T W;
		ZVector4D() : X(0), Y(0), Z(0), W(0) {}
		ZVector4D(T inx, T iny, T inz, T inw) : X(inx), Y(iny), Z(inz), W(inw) {};
		ZVector4D(const ZVector2D<T>& inVector2D) :X(inVector2D.X), Y(inVector2D.Y), Z(0), W(0) {}
		ZVector4D(const ZVector3D<T>& inVector3D) :X(inVector3D.X), Y(inVector3D.Y), Z(inVector3D.Z), W(0) {}
		ZVector4D(const ZVector3D<T>& inVector3D,const T& insigle) :X(inVector3D.X), Y(inVector3D.Y), Z(inVector3D.Z), W(insigle) {}
		ZVector4D(const ZVector4D<T>& inVector4D) :X(inVector4D.X), Y(inVector4D.Y), Z(inVector4D.Z), W(inVector4D.W) {}

		//运算符重载
		T operator[](int index) const {
			assert(index >= 0 && index < 4);
			if (index == 0)return X;
			else if (index == 1) return Y;
			else if (index == 2) return Z;
			return W;
		}
		T& operator[](int index) {
			assert(index >= 0 && index < 4);
			if (index == 0)return X;
			else if (index == 1) return Y;
			else if (index == 2) return Z;
			return W;
		}
		ZVector4D<T> operator=(const ZVector4D<T>& inV) {
			X = inV.X; Y = inV.Y; Z = inV.Z; W = inV.W;
			return *this;
		}
		ZVector4D<T> operator+(const ZVector4D<T>& inV) const {
			return ZVector4D(X + inV.X, Y + inV.Y, Z + inV.Z, W + inV.W);
		}
		ZVector4D<T> operator+=(const ZVector4D<T>& inV) {
			X += inV.X; Y += inV.Y; Z += inV.Z; W = inV.W;
			return *this;
		}
		ZVector4D<T> operator*(T inV) const {
			return ZVector4D(X * inV, Y * inV, Z * inV, W * inV);
		}
		ZVector4D<T> operator*=(const T& inV) {
			X *= inV; Y *= inV; Z *= inV; W *= inV;
			return *this;
		}
		ZVector4D<T> operator/(T inV) const {
			assert(inV != 0);
			float f = static_cast<float> (1) / inV;
			return ZVector4D(X * f, Y * f, Z * f, W * f);
		}
		ZVector4D<T> operator/=(T inV) {
			assert(inV != 0);
			float f = static_cast<float> (1) / inV;
			X *= f; Y *= f; Z *= f; W *= f;
			return *this;
		}
		ZVector4D<T> operator-() {
			return ZVector4D(-X, -Y, -Z,-W);
		}
		ZVector4D<T> operator-(const ZVector4D<T>& inV) const {
			return ZVector4D(X - inV.X, Y - inV.Y, Z - inV.Z, W - inV.W);
		}
		ZVector4D<T> operator-=(const ZVector4D<T>& inV) {
			X -= inV.X; Y -= inV.Y; Z -= inV.Z; W -= inV.W;
			return *this;
		}
		bool operator==(const ZVector4D<T>& inV) const {
			if (X == inV.X && Y == inV.Y && Z == inV.Z && W == inV.W)return true;
			return false;
		}
		void print() {
			std::cout << "(" << X << "," << Y << "," << Z << "," << W << ")" << std::endl;
		}
	};

	using vec2f = ZVector2D<float>;
	using vec2i = ZVector2D<int>;
	using vec3f = ZVector3D<float>;
	using vec3i = ZVector3D<int>;
	using vec4f = ZVector4D<float>;
	using vec4i = ZVector4D<int>;
}