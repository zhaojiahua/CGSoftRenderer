#pragma once
#include "zvector.h"
#include "matrix.h"
#include "../global/base.h"

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

		col1.X = -(src.Get(0, 1) * D_22_33 - src.Get(2, 1) * D_02_33 + src.Get(3, 1) * D_02_23);
		col1.Y = src.Get(0, 0) * D_22_33 - src.Get(2, 0) * D_02_33 + src.Get(3, 0) * D_02_23;
		col1.Z = -(src.Get(0, 0) * D_21_33 - src.Get(2, 0) * D_01_33 + src.Get(3, 0) * D_01_23);
		col1.W = src.Get(0, 0) * D_21_32 - src.Get(2, 0) * D_01_32 + src.Get(3, 0) * D_01_22;

		col2.X = src.Get(0, 1) * D_12_33 - src.Get(1, 1) * D_02_33 + src.Get(3, 1) * D_02_13;
		col2.Y = -(src.Get(0, 0) * D_12_33 - src.Get(1, 0) * D_02_33 + src.Get(3, 0) * D_02_13);
		col2.Z = src.Get(0, 0) * D_11_33 - src.Get(1, 0) * D_01_33 + src.Get(3, 0) * D_01_13;
		col2.W = -(src.Get(0, 0) * D_11_32 - src.Get(1, 0) * D_01_32 + src.Get(3, 0) * D_01_12);

		col3.X = -(src.Get(0, 1) * D_12_23 - src.Get(1, 1) * D_02_23 + src.Get(2, 1) * D_02_13);
		col3.Y = src.Get(0, 0) * D_12_23 - src.Get(1, 0) * D_02_23 + src.Get(2, 0) * D_02_13;
		col3.Z = -(src.Get(0, 0) * D_11_23 - src.Get(1, 0) * D_01_23 + src.Get(2, 0) * D_01_13);
		col3.W = src.Get(0, 0) * D_11_22 - src.Get(1, 0) * D_01_22 + src.Get(2, 0) * D_01_12;

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

		return Amatrix * oneOverDeterminant;
	}

	/*
	构造空间变换矩阵
	scale translate rotate
	第一个参数是当前模型坐标系原矩阵,返回变换后的矩阵
	*/
	template<typename T,typename V>
	ZMatrix4X4<T> Scale(const ZMatrix4X4<T>& src, V x, V y, V z) {		//缩放矩阵
		ZMatrix4X4<T> tempResult;
		auto col0 = src.GetColum(0);
		auto col1 = src.GetColum(1);
		auto col2 = src.GetColum(2);
		auto col3 = src.GetColum(3);
		col0 *= x;
		col1 *= y;
		col2 *= z;
		tempResult.SetColum(0, col0);
		tempResult.SetColum(1, col1);
		tempResult.SetColum(2, col2);
		tempResult.SetColum(3, col3);
		return tempResult;
	}
	template<typename T, typename V>
	ZMatrix4X4<T> Translate(const ZMatrix4X4<T>& src, V x, V y, V z) {		//平移矩阵
		ZMatrix4X4<T> tempResult(src);
		ZVector4D<T> col0 = src.GetColum(0);
		ZVector4D<T> col1 = src.GetColum(1);
		ZVector4D<T> col2 = src.GetColum(2);
		ZVector4D<T> col3 = src.GetColum(3);

		ZVector4D<T> dstcol3 = col0 * x + col1 * y + col2 * z + col3;

		tempResult.SetColum(3, dstcol3);
		
		return tempResult;
	}
	template<typename T, typename V>
	ZMatrix4X4<T> Translate(const ZMatrix4X4<T>& src, const ZVector3D<V>& v) {		//平移矩阵
		return Translate(src, v.X, v.Y, v.Z);
	}
	template<typename T>
	ZMatrix4X4<T> Rotate(const ZMatrix4X4<T>& src, float angle, const ZVector3D<T>& v) {	//绕着一个轴向v旋转angle角度
		T const c = std::cos(angle);	//注意要求angle是弧度制
		T const s = std::sin(angle);

		ZVector3D<T> axis = normalize(v);
		ZVector3D<T> temp((T(1) - c) * axis);

		//根据公式构建Rotator矩阵
		ZMatrix4X4<T> Rotator;
		Rotator.Set(0, 0, c + temp[0] * axis[0]);
		Rotator.Set(1, 0, temp[0] * axis[1] + s * axis[2]);
		Rotator.Set(2, 0, temp[0] * axis[2] - s * axis[1]);

		Rotator.Set(0, 1, temp[1] * axis[0] - s * axis[2]);
		Rotator.Set(1, 1, c + temp[1] * axis[1]);
		Rotator.Set(2, 1, temp[1] * axis[2] + s * axis[0]);

		Rotator.Set(0, 2, temp[2] * axis[0] + s * axis[1]);
		Rotator.Set(1, 2, temp[2] * axis[1] - s * axis[0]);
		Rotator.Set(2, 2, c + temp[2] * axis[2]);

		//然后 src矩阵*Rotator矩阵
		auto rcol0 = Rotator.GetColum(0);
		auto rcol1 = Rotator.GetColum(1);
		auto rcol2 = Rotator.GetColum(2);
		auto rcol3 = Rotator.GetColum(3);

		auto scol0 = src.GetColum(0);
		auto scol1 = src.GetColum(1);
		auto scol2 = src.GetColum(2);
		auto scol3 = src.GetColum(3);

		auto col0 = scol0 * rcol0[0] + scol1 * rcol0[1] + scol2 * rcol0[2];
		auto col1 = scol0 * rcol1[0] + scol1 * rcol1[1] + scol2 * rcol1[2];
		auto col2 = scol0 * rcol2[0] + scol1 * rcol2[1] + scol2 * rcol2[2];
		auto col3 = scol3;

		ZMatrix4X4<T> resultM;
		resultM.SetColum(0, col0);
		resultM.SetColum(1, col1);
		resultM.SetColum(2, col2);
		resultM.SetColum(3, col3);

		return resultM;
	}

	//正交矩阵
	template<typename T>
	ZMatrix4X4<T> Orthographic(T left, T right, T bottom, T top, T near, T far) {
		ZMatrix4X4<T> result(static_cast<T>(1));

		result.Set(0, 0, static_cast<T>(2) / (right - left));
		result.Set(0, 3, -(right + left) / (right - left));
		result.Set(1, 1, static_cast<T>(2) / (top - bottom));
		result.Set(1, 3, -(top + bottom) / (top - bottom));
		result.Set(2, 2, -static_cast<T>(2) / (far - near));
		result.Set(1, 3, -(far + near) / (far - near));

		return result;
	}

	//透视矩阵
	template<typename T>
	ZMatrix4X4<T> Perspective(T fovy, T aspect, T n, T f) {
		T tanHalfFovy = std::tan(0.01745329251994329 * (fovy * 0.5f));

		ZMatrix4X4<T> result(static_cast<T>(0));
		result.Set(0, 0, static_cast<T>(1) / (aspect * tanHalfFovy));
		result.Set(1, 1, static_cast<T>(1) / (tanHalfFovy));
		result.Set(2, 2, -(f + n) / (f - n));
		result.Set(2, 3, -static_cast<T>(2) * f * n / (f - n));
		result.Set(3, 2, -static_cast<T>(1));

		return result;
	}

	//屏幕像素矩阵
	template<typename T>
	ZMatrix4X4<T> ScreenMatrix(const uint32_t& width, const uint32_t& height) {
		ZMatrix4X4<T> result(static_cast<T>(1));

		//x
		result.Set(0, 0, static_cast<T>(width) / static_cast<T>(2));
		result.Set(0, 3, static_cast<T>(width) / static_cast<T>(2));

		//y
		result.Set(1, 1, static_cast<T>(height) / static_cast<T>(2));
		result.Set(1, 3, static_cast<T>(height) / static_cast<T>(2));

		//z
		result.Set(2, 2, 0.5f);
		result.Set(2, 3, 0.5f);

		return result;
	}


	//lerp函数的系统化定义(统一风格,结果和第二值成正比和第一个值成反比)
	static float Lerp(const float& inf1, const float& inf2, const float& weight) {
		return (1 - weight) * inf1 + weight * inf2;
	}
	static vec2f Lerp(const vec2f& inf1, const vec2f& inf2, const float& weight) {
		return (1 - weight) * inf1 + weight * inf2;
	}
	static vec3f Lerp(const vec3f& inf1, const vec3f& inf2, const float& weight) {
		return (1 - weight) * inf1 + weight * inf2;
	}
	static vec4f Lerp(const vec4f& inf1, const vec4f& inf2, const float& weight) {
		return (1 - weight) * inf1 + weight * inf2;
	}
	static ZRGBA Lerp(const ZRGBA& inf1, const ZRGBA& inf2, const float& weight) {
		ZRGBA tempC;
		tempC.zA = static_cast<byte>(Lerp(static_cast<float>(inf1.zA), static_cast<float>(inf2.zA), weight));
		tempC.zR = static_cast<byte>(Lerp(static_cast<float>(inf1.zR), static_cast<float>(inf2.zR), weight));
		tempC.zG = static_cast<byte>(Lerp(static_cast<float>(inf1.zG), static_cast<float>(inf2.zG), weight));
		tempC.zB = static_cast<byte>(Lerp(static_cast<float>(inf1.zB), static_cast<float>(inf2.zB), weight));
		return tempC;
	}
	//三角形重心差值算法
	static float Lerp(const float& inf1, const float& inf2, const float& inf3, const float& weight1, const float& weight2, const float& weight3) {
		return weight1 * inf1 + weight2 * inf2 + weight3 * inf3;
	}
	static vec2f Lerp(const vec2f& inf1, const vec2f& inf2, const vec2f& inf3, const float& weight1, const float& weight2, const float& weight3) {
		return weight1 * inf1 + weight2 * inf2 + weight3 * inf3;
	}
	static vec3f Lerp(const vec3f& inf1, const vec3f& inf2, const vec3f& inf3, const float& weight1, const float& weight2, const float& weight3) {
		return weight1 * inf1 + weight2 * inf2 + weight3 * inf3;
	}
	static vec4f Lerp(const vec4f& inf1, const vec4f& inf2, const vec4f& inf3, const float& weight1, const float& weight2, const float& weight3) {
		return weight1 * inf1 + weight2 * inf2 + weight3 * inf3;	//vertex shader里面处理颜色差值可以用此函数
	}
}
