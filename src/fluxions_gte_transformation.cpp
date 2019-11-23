
#include "pch.hpp"
#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <fluxions_common_test.hpp>
#include <fluxions_gte_transformation.hpp>

namespace Fluxions
{
template class TTransformation<float>;
template class TTransformation<double>;

template <typename T>
bool TestGteTransformations(const char* testname) {
	using std::cout;
	using std::endl;
	bool result = true;

	using T1 = TTransformation<T>::value_type;
	using PType = TTransformation<T>::position_type;
	using QType = TTransformation<T>::rotation_type;

	TTransformation<T> t1{ { 1,  1,  1}, TQuaternion<T>::makeFromAngleAxis( 10, 1, 0, 0) };
	TTransformation<T> t2{ { 2,  3,  4}, TQuaternion<T>::makeFromAngleAxis( 90, 0, 1, 0) };
	TTransformation<T> t3{ {-4, -3, -2}, TQuaternion<T>::makeFromAngleAxis(-45, 0, 0, 1) };
	FLUXIONS_DO_TEST(t1.position.length() == 0);

	cout << t1.toString() << endl;
	cout << t1.toString() << endl;
	cout << t1.toString() << endl;

	// FLUXIONS_DO_TEST(t1.rotation.length() == 0);

	//fprintf(stderr, t1.toString().c_str());

	//using keyframe = std::pair<int, Transformation>;
	//using keyframe_vector = std::vector<keyframe>;

	//Transformation t2{ PType(1, 2, 3), QType::makeFromAngleAxis(0.0, 1.0f, 0.0f, 0.0f) };

	return result;
}

void TestGteTransformations() noexcept {
	FLUXIONS_TEST(TestGteTransformations<float>);
	FLUXIONS_TEST(TestGteTransformations<double>);
}
}