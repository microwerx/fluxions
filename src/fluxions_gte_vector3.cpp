// SSPHH/Fluxions/Unicornfish/Viperfish/Hatchetfish/Sunfish/Damselfish/GLUT Extensions
// Copyright (C) 2017 Jonathan Metzgar
// All rights reserved.
//
// This program is free software : you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program.If not, see <https://www.gnu.org/licenses/>.
//
// For any other type of licensing, please contact me at jmetzgar@outlook.com
#include <stdio.h>
#include <type_traits>
#define FLUXIONS_NO_EXTERN_TEMPLATES
#include <fluxions_gte_vector3.hpp>

namespace Fluxions
{
	// template specializations
	template class TVector3<float>;
	template class TVector3<double>;
	template class TVector3<signed char>;
	template class TVector3<unsigned char>;
	template class TVector3<int>;
	template class TVector3<unsigned int>;
	template class TVector3<short>;
	template class TVector3<unsigned short>;

	template <typename T>
	bool TestTVector3() {
		using vtype = TVector3<T>;
#define TEST(test) if (!(test)) result = false;
		bool result = true;

		// Test 0 default constructor
		TVector3<T> a;
		if (a.x != 0 && a.y != 0 && a.z != 0) result = false;

		// Test parameter constructor
		TVector3<T> b(1, 2, 3);
		if (b.x != 1 && b.y != 2 && b.z != 3) result = false;

		// Test copy constructor
		TVector3<T> c(b);
		if (b.x != c.x && b.y != c.y && b.z != c.z) result = false;

		// Test equality operator
		if (b != c) result = false;
		if (!(b == c)) result = false;

		// Test signed comparison operators
		// compare (1, 2, 3) to (2, 1, 3)
		// <  => (1, 0, 1)
		// <= => (1, 0, 1)
		// >  => (0, 1, 0)
		// >= => (0, 1, 1)
		b.reset(1, 2, 3);
		c.reset(2, 1, 3);
		TEST(b < c == TVector3<T>(1, 0, 0));
		TEST(b <= c == TVector3<T>(1, 0, 1));
		TEST(b > c == TVector3<T>(0, 1, 0));
		TEST(b >= c == TVector3<T>(0, 1, 1));

		// Test reset method
		b.reset(4, 5, 6);
		if (b != TVector3<T>(4, 5, 6)) result = false;

		// Test constants
		if (TVector3<T>::one != 1) result = false;
		if (TVector3<T>::zero != 0) result = false;
		if (TVector3<T>::is_signed && TVector3<T>::minusone != -1) {
			result = false;
		}

		// Linear Algebra Test

		// Test += operator (scalar)
		a.reset(1, 2, 3);
		a += 1;
		if (a != TVector3<T>(2, 3, 4)) result = false;

		// Test += operator (vector)
		a.reset(1, 2, 3);
		a += TVector3<T>(3, 2, 1);
		if (a != TVector3<T>(4, 4, 4)) result = false;

		// Test -= operator (scalar)
		a.reset(2, 3, 4);
		a -= 1;
		TEST(a == TVector3<T>(1, 2, 3));

		// Test -= operator (vector)
		a.reset(2, 3, 4);
		a -= TVector3<T>(1, 2, 3);
		TEST(a == TVector3<T>(1, 1, 1));

		// Test unary operators

		a.reset(2, 3, 4);
		if (TVector3<T>::is_signed) {
			TEST(-a == TVector3<T>(-2, -3, -4));
		}
		else {
			TEST(-a == a);
		}

		// Test range-based for
		a.reset(4, 5, 6);
		unsigned i = 0;
		for (T x : a) {
			TEST(a[i++] == x);
		}

		i = 0;
		for (const T& x : a) {
			TEST(a[i++] == x);
		}

		i = 0;
		for (T& x : a) {
			x = 1;
			TEST(a[i++] == 1);
		}

		bool trivially_copy_constructible = 0;
		fprintf(stderr, "trivially_copy_constructible: %d", std::is_trivially_copy_constructible_v<vtype>);

		return result;
	}

	void TestVector3() {
#define FLUXIONS_TEST(test) fprintf(stderr, "%s(): Test " #test " was %s\n", __FUNCTION__, ((test) ? "successful" : "unsuccessful"));

		FLUXIONS_TEST(TestTVector3<float>());
		FLUXIONS_TEST(TestTVector3<double>());
		FLUXIONS_TEST(TestTVector3<unsigned char>());
		FLUXIONS_TEST(TestTVector3<signed char>());
		FLUXIONS_TEST(TestTVector3<unsigned short>());
		FLUXIONS_TEST(TestTVector3<short>());
		FLUXIONS_TEST(TestTVector3<int>());
		FLUXIONS_TEST(TestTVector3<unsigned int>());
	}
} // namespace Fluxions
