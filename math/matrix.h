#pragma once
/*
矩阵类,提供矩阵方面的运算(主要是方阵)
*/
#include <iostream>
#include <assert.h>
#include "zvector.h"

namespace math {
	template<typename T>
	class ZMatrix4X4;
	/*
	m0 m3 m6
	m1 m4 m7
	m2 m5 m8
	*/
	template<typename T>
	class ZMatrix3X3
	{
	public:
		T m[9] = { 1,0,0,0,1,0,0,0,1 };
		//构造函数
		ZMatrix3X3() {}
		ZMatrix3X3(T invalue) { m[0] = m[4] = m[8] = invalue; }
		ZMatrix3X3<T>(const ZMatrix3X3<T>& inmatrix33) {
			memccpy((void*)m, (void*)inmatrix33.m, sizeof(T) * 9);
		}
		ZMatrix3X3<T>(const ZMatrix4X4<T>& inmatrix44) {
			m[0] = inmatrix44.m[0]; m[3] = inmatrix44.m[4]; m[6] = inmatrix44.m[8];
			m[1] = inmatrix44.m[1]; m[4] = inmatrix44.m[5]; m[7] = inmatrix44.m[9];
			m[2] = inmatrix44.m[2]; m[5] = inmatrix44.m[6]; m[8] = inmatrix44.m[10];
		}

		//常用的矩阵运算函数
		ZVector3D<T> GetColum(const uint8_t& col) {
			assert(col < 3);
			return ZVector3D<T>(m[col * 3], m[col * 3 + 1], m[col * 3 + 2]);
		}
		void SetColum(const uint8_t& col, const ZVector3D<T>& colV) {
			assert(col < 3);
			m[col * 3] = colV.X;
			m[col * 3 + 1]= colV.Y;
			m[col * 3 + 2] = colV.Z;
		}
		ZMatrix3X3<T> operator*(const T& s) {
			ZMatrix3X3<T> tempMatrix33;
			tempMatrix33.SetColum(0, this->GetColum(0) * s);
			tempMatrix33.SetColum(1, this->GetColum(1) * s);
			tempMatrix33.SetColum(2, this->GetColum(2) * s);
			return tempMatrix33;
		}
		ZVector3D<T> operator*(const ZVector3D<T>& inV) {
			return ZVector3D<T>(
				inV.X * m[0] + inV.Y * m[3] + inV.Z * m[6],
				inV.X * m[1] + inV.Y * m[4] + inV.Z * m[7],
				inV.X * m[2] + inV.Y * m[5] + inV.Z * m[8]
				);
		}
		void Set(const uint8_t& row, const uint8_t& col, T t) {
			assert(row < 3 && col < 3);
			m[col * 3 + row] = t;
		}
		void Set(
			T t00, T t01, T t02, 
			T t03, T t04, T t05, 
			T t06, T t07, T t08) {
			m[0] = t00; m[3] = t01; m[6] = t02;
			m[1] = t03; m[4] = t04; m[7] = t05;
			m[2] = t06; m[5] = t07; m[8] = t08;
		}
		ZMatrix3X3<T> Identity() {
			Set(
				1, 0, 0,
				0, 1, 0,
				0, 0, 1
			); 
			return *this;
		}
		void PrintM() {
			std::cout << "|" << m[0] << "," << m[3] << "," << m[6] << "|" << std::endl;
			std::cout << "|" << m[1] << "," << m[4] << "," << m[7] << "|" << std::endl;
			std::cout << "|" << m[2] << "," << m[5] << "," << m[8] << "|" << std::endl;
			std::cout << std::endl;
		}
	};

	template<typename T>
	class ZMatrix4X4
	{
	public:
		T m[16] = { 1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1 };
		//构造函数
		ZMatrix4X4() {}
		ZMatrix4X4(T invalue) { m[0] = m[5] = m[10] = m[15] = invalue; }
		ZMatrix4X4<T>(const ZMatrix4X4<T>& inmatrix44) {
			memcpy((void*)m, (void*)inmatrix44.m, sizeof(T) * 16);
		}

		//常用的矩阵运算函数
		ZVector4D<T> GetColum(const uint8_t& col) const {
			assert(col < 4);
			return ZVector4D<T>(m[col * 4], m[col * 4 + 1], m[col * 4 + 2], m[col * 4 + 3]);
		}
		void SetColum(const uint8_t& col, const ZVector4D<T>& colV) {
			assert(col < 4);
			m[col * 4] = colV.X;
			m[col * 4 + 1] = colV.Y;
			m[col * 4 + 2] = colV.Z;
			m[col * 4 + 3] = colV.W;
		}
		ZMatrix4X4<T> operator*(const T& s) {
			ZMatrix4X4<T> tempMatrix44;
			tempMatrix44.SetColum(0, this->GetColum(0) * s);
			tempMatrix44.SetColum(1, this->GetColum(1) * s);
			tempMatrix44.SetColum(2, this->GetColum(2) * s);
			tempMatrix44.SetColum(3, this->GetColum(3) * s);
			return tempMatrix44;
		}
		ZVector4D<T> operator*(const ZVector4D<T>& inV) {
			return ZVector4D<T>(
			inV.X * m[0] + inV.Y * m[4] + inV.Z * m[8] + inV.W * m[12],
			inV.X * m[1] + inV.Y * m[5] + inV.Z * m[9] + inV.W * m[13],
			inV.X * m[2] + inV.Y * m[6] + inV.Z * m[10] + inV.W * m[14],
			inV.X * m[3] + inV.Y * m[7] + inV.Z * m[11] + inV.W * m[15]
			);
		}
		T Get(const uint8_t& row, const uint8_t& col) const {
			assert(row < 4 && col < 4);
			return m[col * 4 + row];
		}
		void Set(const uint8_t& row, const uint8_t& col, T t) {
			assert(row < 4 && col < 4);
			m[col * 4 + row] = t;
		}
		void Set(
			T t00, T t01, T t02, T t03,
			T t04, T t05, T t06, T t07,
			T t08, T t09, T t10, T t11,
			T t12, T t13, T t14, T t15) {
			m[0] = t00; m[4] = t01; m[8] = t02; m[12] = t03;
			m[1] = t04; m[5] = t05; m[9] = t06; m[13] = t07;
			m[2] = t08; m[6] = t09; m[10] = t10; m[14] = t11;
			m[3] = t12; m[7] = t13; m[11] = t14; m[15] = t15;
		}
		ZMatrix4X4<T> Identity() {
			Set(
				1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				0, 0, 0, 1
			);
			return *this;
		}
		void PrintM() {
			std::cout << "|" << m[0] << "," << m[4] << "," << m[8]  << "," << m[12] << "|" << std::endl;
			std::cout << "|" << m[1] << "," << m[5] << "," << m[9]  << "," << m[13] << "|" << std::endl;
			std::cout << "|" << m[2] << "," << m[6] << "," << m[10]<< ","  << m[14] << "|" << std::endl;
			std::cout << "|" << m[3] << "," << m[7] << "," << m[11]<< ","  << m[15] << "|" << std::endl;
			std::cout << std::endl;
		}
	};

	using Mat3f = ZMatrix3X3<float>;
	using Mat4f = ZMatrix4X4<float>;
}