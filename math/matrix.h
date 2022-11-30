#pragma once
/*
矩阵类,提供矩阵方面的运算
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

	};

}