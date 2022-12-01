#include "math/math.h"

using namespace math;
int main() {
	Mat4f m01(1.0f);
	m01.PrintM();
	m01.Set(
		10,15,10,10.2,
		11.6,11,12.5,11,
		15,16,17,12.8,
		19,20,10.6,12.6
		);
	m01.PrintM();
	Mat4f m01Iverse = Inverse(m01);
	m01Iverse.PrintM();
	(m01Iverse * m01).PrintM();
	/*vec3f v3df1(1.0f, 2.0f, 3.0f);
	vec3f v3df2(3.0f, 2.0f, 1.0f);
	vec3i v3d2(3, 2, 1);
	(3.5 * v3df1).print();
	normalize(v3df1).print();
	normalize(v3df2).print();
	normalize(cross(v3df1, v3df2)).print();
	(v3df1 * v3df2).print();*/
	/*ZVector2D<int> testV2d1(10, 15);
	ZVector2D<int> testV2d2(13, 20);
	ZVector2D<int>testV2d3(testV2d1);
	ZVector2D<int>testV2d4;
	testV2d1.print();
	testV2d2.print();
	testV2d3.print();
	testV2d4.print();
	testV2d4 = testV2d2;
	testV2d4.print();
	testV2d4 += testV2d1;
	testV2d4.print();
	ZVector2D<int> tempv(2, 2);
	ZVector2D<int> testV2d5 = testV2d4 - tempv;
	testV2d5.print();
	testV2d4 -= tempv;
	testV2d4.print();
	if (testV2d5 == testV2d4) std::cout << "==" << std::endl;
	testV2d5 *= 10;
	testV2d5.print();
	ZVector2D<int> testV2d6 = testV2d5 * 10;
	testV2d6.print();
	testV2d6 /= 10;
	testV2d6.print();
	ZVector2D<int> testV2d7 = testV2d6 / 100;
	testV2d7.print();

	std::cout << "------------Vector3D------------" << std::endl;
	/// /////
	ZVector3D<int> testV3d1(10, 15,18);
	ZVector3D<int> testV3d2(13, 20, 22);
	ZVector3D<int>testV3d3(testV3d1);
	ZVector3D<int>testV3d4;
	testV3d1.print();
	testV3d2.print();
	testV3d3.print();
	testV3d4.print();
	testV3d4 = testV3d2;
	testV3d4.print();
	testV3d4 += testV3d1;
	testV3d4.print();
	ZVector3D<int> tempv3(2, 2, 3);
	ZVector3D<int> testV3d5 = testV3d4 - tempv3;
	testV3d5.print();
	testV3d4 -= tempv3;
	testV3d4.print();
	if (testV3d5 == testV3d4) std::cout << "==" << std::endl;
	testV3d5 *= 10;
	testV3d5.print();
	ZVector3D<int> testV3d6 = testV3d5 * 10;
	testV3d6.print();
	testV3d6 /= 10;
	testV3d6.print();
	ZVector3D<int> testV3d7 = testV3d6 / 100;
	testV3d7.print();

	std::cout << "------------Vector4D------------" << std::endl;
	ZVector4D<int> testV4d1(10, 15, 18,100);
	ZVector4D<int> testV4d2(13, 20, 22,100);
	ZVector4D<int>testV4d3(testV4d1);
	ZVector4D<int>testV4d4;
	testV4d1.print();
	testV4d2.print();
	testV4d3.print();
	testV4d4.print();
	testV4d4 = testV4d2;
	testV4d4.print();
	testV4d4 += testV4d1;
	testV4d4.print();
	ZVector4D<int> tempv4(2, 2, 3,10);
	ZVector4D<int> testV4d5 = testV4d4 - tempv4;
	testV4d5.print();
	testV4d4 -= tempv4;
	testV4d4.print();
	if (testV4d5 == testV4d4) std::cout << "==" << std::endl;
	testV4d5 *= 10;
	testV4d5.print();
	ZVector4D<int> testV4d6 = testV4d5 * 10;
	testV4d6.print();
	testV4d6 /= 10;
	testV4d6.print();
	ZVector4D<int> testV4d7 = testV4d6 / 100;
	testV4d7.print();

	std::cout << "---------------------------------------------float------------------------------------------------" << std::endl;
	ZVector2D<float> testfV2d1(10.5, 15.5);
	ZVector2D<float> testfV2d2(13.3, 20.8);
	ZVector2D<float>testfV2d3(testfV2d1);
	ZVector2D<float>testfV2d4;
	testfV2d1.print();
	testfV2d2.print();
	testfV2d3.print();
	testfV2d4.print();
	testfV2d4 = testfV2d2;
	testfV2d4.print();
	testfV2d4 += testfV2d1;
	testfV2d4.print();
	ZVector2D<float> tempfv(2.6, 2.6);
	ZVector2D<float> testfV2d5 = testfV2d4 - tempfv;
	testfV2d5.print();
	testfV2d4 -= tempfv;
	testfV2d4.print();
	if (testfV2d5 == testfV2d4) std::cout << "==" << std::endl;
	testfV2d5 *= 10;
	testfV2d5.print();
	ZVector2D<float> testfV2d6 = testfV2d5 * 10;
	testfV2d6.print();
	testfV2d6 /= 10;
	testfV2d6.print();
	ZVector2D<float> testfV2d7 = testfV2d6 / 100;
	testfV2d7.print();

	std::cout << "------------Vector3D------------" << std::endl;
	/// /////
	ZVector3D<float> testfV3d1(10.5, 15.5, 18.5);
	ZVector3D<float> testfV3d2(13.3, 20.8, 22.8);
	ZVector3D<float>testfV3d3(testfV3d1);
	ZVector3D<float>testfV3d4;
	testfV3d1.print();
	testfV3d2.print();
	testfV3d3.print();
	testfV3d4.print();
	testfV3d4 = testfV3d2;
	testfV3d4.print();
	testfV3d4 += testfV3d1;
	testfV3d4.print();
	ZVector3D<float> tempfv3(2.6, 2.6, 3.6);
	ZVector3D<float> testfV3d5 = testfV3d4 - tempfv3;
	testfV3d5.print();
	testfV3d4 -= tempfv3;
	testfV3d4.print();
	if (testfV3d5 == testfV3d4) std::cout << "==" << std::endl;
	testfV3d5 *= 10;
	testfV3d5.print();
	ZVector3D<float> testfV3d6 = testfV3d5 * 10;
	testfV3d6.print();
	testfV3d6 /= 10;
	testfV3d6.print();
	ZVector3D<float> testfV3d7 = testfV3d6 / 100;
	testfV3d7.print();

	std::cout << "------------Vector4D------------" << std::endl;
	ZVector4D<float> testfV4d1(10.5, 15.5, 18.5, 100.5);
	ZVector4D<float> testfV4d2(13.3, 20.38, 22.8, 100.4);
	ZVector4D<float>testfV4d3(testfV4d1);
	ZVector4D<float>testfV4d4;
	testfV4d1.print();
	testfV4d2.print();
	testfV4d3.print();
	testfV4d4.print();
	testfV4d4 = testfV4d2;
	testfV4d4.print();
	testfV4d4 += testfV4d1;
	testfV4d4.print();
	ZVector4D<float> tempfv4(2.6, 2.6, 3.6, 10.6);
	ZVector4D<float> testfV4d5 = testfV4d4 - tempfv4;
	testfV4d5.print();
	testfV4d4 -= tempfv4;
	testfV4d4.print();
	if (testfV4d5 == testfV4d4) std::cout << "==" << std::endl;
	testfV4d5 *= 10;
	testfV4d5.print();
	ZVector4D<float> testfV4d6 = testfV4d5 * 10;
	testfV4d6.print();
	testfV4d6 /= 10;
	testfV4d6.print();
	ZVector4D<float> testfV4d7 = testfV4d6 / 100;
	testfV4d7.print();*/

	return 0;
}