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
#define FLUXIONS_NO_EXTERN_TEMPLATES
#include <fluxions_gte_color3.hpp>

namespace Fluxions
{
	template <>
	float TColor3<float>::to_float_factor = 1.0f;
	template <>
	float TColor3<float>::from_float_factor = 1.0f;
	template <>
	float TColor3<double>::to_float_factor = 1.0f;
	template <>
	float TColor3<double>::from_float_factor = 1.0f;
	template <>
	float TColor3<unsigned char>::to_float_factor = 255.99f;
	template <>
	float TColor3<unsigned char>::from_float_factor = 1.0f / 255.00f;
	template <>
	float TColor3<unsigned int>::to_float_factor = 255.99f;
	template <>
	float TColor3<unsigned int>::from_float_factor = 1.0f / 255.00f;
	template <>
	float TColor3<unsigned short>::to_float_factor = 255.99f;
	template <>
	float TColor3<unsigned short>::from_float_factor = 1.0f / 255.00f;
	template <>
	float TColor3<char>::to_float_factor = 127.99f;
	template <>
	float TColor3<char>::from_float_factor = 1.0f / 127.00f;
	template <>
	float TColor3<int>::to_float_factor = 255.99f;
	template <>
	float TColor3<int>::from_float_factor = 1.0f / 255.00f;
	template <>
	float TColor3<short>::to_float_factor = 255.99f;
	template <>
	float TColor3<short>::from_float_factor = 1.0f / 255.00f;

	template <>
	TColor3<float>::value_type TColor3<float>::min_value = 0.0f;
	template <>
	TColor3<float>::value_type TColor3<float>::max_value = 1.0f;
	template <>
	TColor3<double>::value_type TColor3<double>::min_value = 0.0;
	template <>
	TColor3<double>::value_type TColor3<double>::max_value = 1.0;
	template <>
	TColor3<char>::value_type TColor3<char>::min_value = 0;
	template <>
	TColor3<char>::value_type TColor3<char>::max_value = 127;
	template <>
	TColor3<short>::value_type TColor3<short>::min_value = 0;
	template <>
	TColor3<short>::value_type TColor3<short>::max_value = 255;
	template <>
	TColor3<int>::value_type TColor3<int>::min_value = 0;
	template <>
	TColor3<int>::value_type TColor3<int>::max_value = 65535;
	template <>
	TColor3<unsigned char>::value_type TColor3<unsigned char>::min_value = 0;
	template <>
	TColor3<unsigned char>::value_type TColor3<unsigned char>::max_value = 255;
	template <>
	TColor3<unsigned short>::value_type TColor3<unsigned short>::min_value = 0;
	template <>
	TColor3<unsigned short>::value_type TColor3<unsigned short>::max_value = 65535;
	template <>
	TColor3<unsigned int>::value_type TColor3<unsigned int>::min_value = 0;
	template <>
	TColor3<unsigned int>::value_type TColor3<unsigned int>::max_value = 65535;

	template <>
	unsigned int TColor3<float>::gl_type = 0x1406; // GL_FLOAT
	template <>
	unsigned int TColor3<double>::gl_type = 0x140A; // GL_DOUBLE
	template <>
	unsigned int TColor3<char>::gl_type = 0x1400; // GL_BYTE
	template <>
	unsigned int TColor3<unsigned char>::gl_type = 0x1401; // GL_UNSIGNED_BYTE
	template <>
	unsigned int TColor3<short>::gl_type = 0x1402; // GL_SHORT
	template <>
	unsigned int TColor3<unsigned short>::gl_type = 0x1403; // GL_UNSIGNED_SHORT
	template <>
	unsigned int TColor3<int>::gl_type = 0x1404; // GL_INT
	template <>
	unsigned int TColor3<unsigned int>::gl_type = 0x1405; // GL_UNSIGNED_INT
	template <>
	unsigned int TColor3<float>::gl_size = 3;
	template <>
	unsigned int TColor3<double>::gl_size = 3;
	template <>
	unsigned int TColor3<char>::gl_size = 3;
	template <>
	unsigned int TColor3<unsigned char>::gl_size = 3;
	template <>
	unsigned int TColor3<short>::gl_size = 3;
	template <>
	unsigned int TColor3<unsigned short>::gl_size = 3;
	template <>
	unsigned int TColor3<int>::gl_size = 3;
	template <>
	unsigned int TColor3<unsigned int>::gl_size = 3;

	template class TColor3<float>;
	template class TColor3<double>;
	template class TColor3<char>;
	template class TColor3<unsigned char>;
	template class TColor3<short>;
	template class TColor3<unsigned short>;
	template class TColor3<int>;
	template class TColor3<unsigned int>;

	Color3f HLSToRGBf(float h, float l, float s) noexcept {
		float q = l < 0.5f ? l * (1.0f + s) : l + s - l * s;
		float p = 2.0f * l - q;
		auto h2rgb = [&q, &p](float t) {
			if (t < 0.0f)
				t += 1.0f;
			if (t > 1.0f)
				t -= 1.0f;
			if (t < 0.16667f)
				return p + (q - p) * 6.0f * t;
			if (t < 0.5f)
				return q;
			if (t < 0.66667f)
				return p + (q - p) * (0.66667f - t) * 6.0f;
			return 0.0f;
		};

		return Color3f(h2rgb(h + 0.33333f), h2rgb(h), h2rgb(h - 0.33333f));
	}

	Color3d HLSToRGBd(double h, double l, double s) noexcept {
		double q = l < 0.5 ? l * (1.0f + s) : l + s - l * s;
		double p = 2.0 * l - q;
		auto h2rgb = [&q, &p](double t) {
			if (t < 0.0)
				t += 1.0;
			if (t > 1.0)
				t -= 1.0;
			if (t < 0.16667)
				return p + (q - p) * 6.0 * t;
			if (t < 0.5)
				return q;
			if (t < 0.66667)
				return p + (q - p) * (0.66667 - t) * 6.0;
			return 0.0;
		};

		return Color3d(h2rgb(h + 0.33333f), h2rgb(h), h2rgb(h - 0.33333f));
	}

} // namespace Fluxions