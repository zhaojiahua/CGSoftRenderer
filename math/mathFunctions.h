#pragma once
#include "zvector.h"
#include "matrix.h"
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

	/*
	matrix
	m0, m3, m6
	m1, m4, m7
	m2, m5, m8
	*/
	template<typename T>
	ZMatrix3X3<T> Transpose(const ZMatrix3X3<T>& inm) {	//矩阵转置
		ZMatrix3X3<T> tempM;
		tempM.m[0] = inm.m[0]; tempM.m[1] = inm.m[3]; tempM.m[2] = inm.m[6];
		tempM.m[3] = inm.m[1]; tempM.m[4] = inm.m[4]; tempM.m[5] = inm.m[7];
		tempM.m[6] = inm.m[2]; tempM.m[7] = inm.m[5]; tempM.m[8] = inm.m[8];
		return tempM;
	}
	template<typename T>
	ZMatrix4X4<T> Transpose(const ZMatrix4X4<T>& inm) {	//矩阵转置
		ZMatrix4X4<T> tempM;
		tempM.m[0] = inm.m[0]; tempM.m[1] = inm.m[4]; tempM.m[2] = inm.m[8]; tempM.m[3] = inm.m[12];
		tempM.m[4] = inm.m[1]; tempM.m[5] = inm.m[5]; tempM.m[6] = inm.m[9]; tempM.m[7] = inm.m[13];
		tempM.m[8] = inm.m[2]; tempM.m[9] = inm.m[6]; tempM.m[10] = inm.m[10]; tempM.m[11] = inm.m[14];
		tempM.m[12] = inm.m[3]; tempM.m[13] = inm.m[7]; tempM.m[14] = inm.m[11]; tempM.m[15] = inm.m[15];
		return tempM;
	}
	template<typename T>
	ZMatrix3X3<T> operator*(const ZMatrix3X3<T>& inm1, const ZMatrix3X3<T>& inm2) {		//矩阵乘法(各元素分别相乘然后分别相加)
		auto m1col0 = inm1.GetColum(0); auto m1col1 = inm1.GetColum(1); auto m1col2 = inm1.GetColum(2);
		auto m2col0 = inm2.GetColum(0); auto m2col1 = inm2.GetColum(1); auto m2col2 = inm2.GetColum(2);

		//按照矩阵乘法的结果,在这里可以理解为第一个矩阵的各向量按照不同的权重相加组成一个新向量,这个权重信息放在第二矩阵的各向量里面,然后逐个遍历矩阵2的各向量,生成的结果放在对应的向量位置
		auto col0 = m1col0 * m2col0[0] + m1col1 * m2col0[1] + m1col2 * m2col0[2];
		auto col1 = m1col0 * m2col1[0] + m1col1 * m2col1[1] + m1col2 * m2col1[2];
		auto col2 = m1col0 * m2col2[0] + m1col1 * m2col2[1] + m1col2 * m2col2[2];

		ZMatrix3X3<T> tempResult;
		tempResult.SetColum(0, col0); tempResult.SetColum(1, col1); tempResult.SetColum(2, col2);
		return tempResult;
	}

	template<typename T>
	ZMatrix4X4<T> operator*(const ZMatrix4X4<T>& inm1, const ZMatrix4X4<T>& inm2) {		//矩阵乘法(各元素分别相乘然后分别相加)
		auto m1col0 = inm1.GetColum(0); auto m1col1 = inm1.GetColum(1); auto m1col2 = inm1.GetColum(2); auto m1col3 = inm1.GetColum(3);
		auto m2col0 = inm2.GetColum(0); auto m2col1 = inm2.GetColum(1); auto m2col2 = inm2.GetColum(2); auto m2col3 = inm2.GetColum(3);

		//按照矩阵乘法的结果,在这里可以理解为第一个矩阵的各向量按照不同的权重相加组成一个新向量,这个权重信息放在第二矩阵的各向量里面,然后逐个遍历矩阵2的各向量,生成的结果放在对应的向量位置
		ZVector4D<T> col0, col1, col2, col3;
		col0 = m1col0 * m2col0[0] + m1col1 * m2col0[1] + m1col2 * m2col0[2]+m1col3 * m2col0[3];
		col1 = m1col0 * m2col1[0] + m1col1 * m2col1[1] + m1col2 * m2col1[2]+m1col3 * m2col1[3];
		col2 = m1col0 * m2col2[0] + m1col1 * m2col2[1] + m1col2 * m2col2[2]+m1col3 * m2col2[3];
		col3 = m1col0 * m2col3[0] + m1col1 * m2col3[1] + m1col2 * m2col3[2]+m1col3 * m2col3[3];

		ZMatrix4X4<T> tempResult;
		tempResult.SetColum(0, col0); tempResult.SetColum(1, col1); tempResult.SetColum(2, col2); tempResult.SetColum(3, col3);
		return tempResult;
	}

	template<typename T>
	ZMatrix4X4<T> Inverse(const ZMatrix4X4<T>& src) {		//求矩阵的逆矩阵
		//这里使用伴随矩阵法求逆矩阵:先求出此矩阵的伴随矩阵inm*,再求出矩阵的行列式|inm|,然后根据公式 inm*/|inm| 即可算出

		/*
		* m0  m4  m8	   m12
		* m1  m5  m9 	   m13
		* m2  m6  m10   m14
		* m3  m7  m11   m15
		*/

		//求伴随矩阵
		ZMatrix4X4<T> Amatrix(static_cast<T>(1));
		Amatrix.PrintM();
		//����ÿ�������2*2��������ʽ,�±������Ͻǵ����½�
		T D_22_33 = src.Get(2, 2) * src.Get(3, 3) - src.Get(2, 3) * src.Get(3, 2);

		T D_12_23 = src.Get(1, 2) * src.Get(2, 3) - src.Get(1, 3) * src.Get(2, 2);
		T D_12_33 = src.Get(1, 2) * src.Get(3, 3) - src.Get(1, 3) * src.Get(3, 2);

		T D_21_32 = src.Get(2, 1) * src.Get(3, 2) - src.Get(2, 2) * src.Get(3, 1);
		T D_21_33 = src.Get(2, 1) * src.Get(3, 3) - src.Get(2, 3) * src.Get(3, 1);

		T D_11_22 = src.Get(1, 1) * src.Get(2, 2) - src.Get(1, 2) * src.Get(2, 1);
		T D_11_23 = src.Get(1, 1) * src.Get(2, 3) - src.Get(1, 3) * src.Get(2, 1);
		T D_11_32 = src.Get(1, 1) * src.Get(3, 2) - src.Get(1, 2) * src.Get(3, 1);
		T D_11_33 = src.Get(1, 1) * src.Get(3, 3) - src.Get(1, 3) * src.Get(3, 1);

		T D_02_13 = src.Get(0, 2) * src.Get(1, 3) - src.Get(0, 3) * src.Get(1, 2);
		T D_02_23 = src.Get(0, 2) * src.Get(2, 3) - src.Get(0, 3) * src.Get(2, 2);
		T D_02_33 = src.Get(0, 2) * src.Get(3, 3) - src.Get(0, 3) * src.Get(3, 2);

		T D_01_12 = src.Get(0, 1) * src.Get(1, 2) - src.Get(0, 2) * src.Get(1, 1);
		T D_01_13 = src.Get(0, 1) * src.Get(1, 3) - src.Get(0, 3) * src.Get(1, 1);
		T D_01_22 = src.Get(0, 1) * src.Get(2, 2) - src.Get(0, 2) * src.Get(2, 1);
		T D_01_23 = src.Get(0, 1) * src.Get(2, 3) - src.Get(0, 3) * src.Get(2, 1);
		T D_01_32 = src.Get(0, 1) * src.Get(3, 2) - src.Get(0, 2) * src.Get(3, 1);
		T D_01_33 = src.Get(0, 1) * src.Get(3, 3) - src.Get(0, 3) * src.Get(3, 1);

		//����������ÿ������
		ZVector4D<T> col0, col1, col2, col3;

		/*
		* 
		* m5 m9	 m13
		* m6 m10 m14
		* m7 m11 m15
		*/
		col0.X = src.Get(1, 1) * D_22_33 - src.Get(2, 1) * D_12_33 + src.Get(3, 1) * D_12_23;
		col0.Y = -(src.Get(1, 0) * D_22_33 - src.Get(2, 0) * D_12_33 + src.Get(3, 0) * D_12_23);
		col0.Z = src.Get(1, 0) * D_21_33 - src.Get(2, 0) * D_11_33 + src.Get(3, 0) * D_11_23;
		col0.W = -(src.Get(1, 0) * D_21_32 - src.Get(2, 0) * D_11_32 + src.Get(3, 0) * D_11_22);
		col0.print();
		col1.X = -(src.Get(0, 1) * D_22_33 - src.Get(2, 1) * D_02_33 + src.Get(3, 1) * D_02_23);
		col1.Y = src.Get(0, 0) * D_22_33 - src.Get(2, 0) * D_02_33 + src.Get(3, 0) * D_02_23;
		col1.Z = -(src.Get(0, 0) * D_21_33 - src.Get(2, 0) * D_01_33 + src.Get(3, 0) * D_01_23);
		col1.W = src.Get(0, 0) * D_21_32 - src.Get(2, 0) * D_01_32 + src.Get(3, 0) * D_01_22;
		col1.print();
		col2.X = src.Get(0, 1) * D_12_33 - src.Get(1, 1) * D_02_33 + src.Get(3, 1) * D_02_13;
		col2.Y = -(src.Get(0, 0) * D_12_33 - src.Get(1, 0) * D_02_33 + src.Get(3, 0) * D_02_13);
		col2.Z = src.Get(0, 0) * D_11_33 - src.Get(1, 0) * D_01_33 + src.Get(3, 0) * D_01_13;
		col2.W = -(src.Get(0, 0) * D_11_32 - src.Get(1, 0) * D_01_32 + src.Get(3, 0) * D_01_12);
		col2.print();
		col3.X = -(src.Get(0, 1) * D_12_23 - src.Get(1, 1) * D_02_23 + src.Get(2, 1) * D_02_13);
		col3.Y = src.Get(0, 0) * D_12_23 - src.Get(1, 0) * D_02_23 + src.Get(2, 0) * D_02_13;
		col3.Z = -(src.Get(0, 0) * D_11_23 - src.Get(1, 0) * D_01_23 + src.Get(2, 0) * D_01_13);
		col3.W = src.Get(0, 0) * D_11_22 - src.Get(1, 0) * D_01_22 + src.Get(2, 0) * D_01_12;
		col3.print();
		Amatrix.SetColum(0,col0);
		Amatrix.SetColum(1,col1);
		Amatrix.SetColum(2,col2);
		Amatrix.SetColum(3,col3);

		//��������ʽ
		ZVector4D<T> row0(Amatrix.Get(0, 0), Amatrix.Get(0, 1), Amatrix.Get(0, 2), Amatrix.Get(0, 3));
		ZVector4D<T> colum0 = src.GetColum(0);
		T determinant = dot(row0, colum0);
		
		assert(determinant != 0);

		T oneOverDeterminant = static_cast<T>(1) / determinant;
		Amatrix.PrintM();
		return Amatrix * oneOverDeterminant;
	}
}

/*

|m1_00,m1_04,m1_08,m1_12|		|m2_00,m2_04,m2_08,m2_12|	
|											  |		|											  |
|m1_01,m1_05,m1_09,m1_13|		|m2_01,m2_05,m2_09,m2_13|
|											  |	    |											  |
|m1_02,m1_06,m1_10,m1_14|		|m2_02,m2_06,m2_10,m2_14|
|											  |		|											  |
|m1_03,m1_07,m1_11,m1_15|		|m2_03,m2_07,m2_11,m2_15|

*/