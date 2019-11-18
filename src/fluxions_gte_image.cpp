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
#include <algorithm>
#include <fstream>
#include <fluxions_gte_color_math.hpp>
#include <fluxions_gte_image.hpp>
#include <fluxions_gte_image_operations.hpp>
#include <iostream>

#define _CRT_SECURE_NO_WARNINGS
#ifdef _WIN32
#pragma warning(disable : 4996)
#endif

#ifdef FLUXIONS_GTE_USEOPENEXR
#define OPENEXR_DLL
#include <OpenEXR/ImfRgbaFile.h>
#endif

#undef _CRT_SECURE_NO_WARNINGS

namespace Fluxions
{
	// C++ Specializations Must Come First

	Color3ub RGBFloatToUint8(const Color3f& c) {
		return Color3ub(
			(unsigned char)clamp((int)(c.r * 255.0f), 0, 255),
			(unsigned char)clamp((int)(c.g * 255.0f), 0, 255),
			(unsigned char)clamp((int)(c.b * 255.0f), 0, 255));
	}

	Color3i RGBFloatToInt32(const Color3f& c) {
		return Color3i(
			(int)clamp((int)(c.r * 255.0f), 0, 65535),
			(int)clamp((int)(c.g * 255.0f), 0, 65535),
			(int)clamp((int)(c.b * 255.0f), 0, 65535));
	}

	// template <>
	// TImage<Color3ub> &TImage<Color3ub>::ToSRGB()
	// {
	//     float scaleFactor = 1.0f / 255.0f;
	//     for (Color3ub &c : pixels)
	//     {
	//         Color3f color_linear(scaleFactor * c.r, scaleFactor * c.g, scaleFactor * c.b);
	//         Color3f color_srgb = RGBtoSRGB(color_linear);
	//         c = RGBFloatToUint8(color_srgb);
	//         // c.r = (unsigned char)clamp((int)(color_srgb.r * 255.0f), 0, 255);
	//         // c.g = (unsigned char)clamp((int)(color_srgb.g * 255.0f), 0, 255);
	//         // c.b = (unsigned char)clamp((int)(color_srgb.b * 255.0f), 0, 255);
	//     }

	//     return *this;
	// }

	// template <>
	// TImage<Color3i> &TImage<Color3i>::ToSRGB()
	// {
	//     float scaleFactor = 1.0f / 255.0f;
	//     for (Color3i &c : pixels)
	//     {
	//         Color3f color_linear(scaleFactor * c.r, scaleFactor * c.g, scaleFactor * c.b);
	//         Color3f color_srgb = RGBtoSRGB(color_linear);
	//         c = RGBFloatToInt32(color_srgb);
	//         // c.r = clamp((int)(color_srgb.r * 255.0f), 0, 65535);
	//         // c.g = clamp((int)(color_srgb.g * 255.0f), 0, 65535);
	//         // c.b = clamp((int)(color_srgb.b * 255.0f), 0, 65535);
	//     }

	//     return *this;
	// }

	// template <>
	// TImage<Color4ub> &TImage<Color4ub>::ToSRGB()
	// {
	//     float scaleFactor = 1.0f / 255.0f;
	//     for (Color4ub &c : pixels)
	//     {
	//         Color3f color_linear(scaleFactor * c.r, scaleFactor * c.g, scaleFactor * c.b);
	//         Color3f color_srgb = RGBtoSRGB(color_linear);
	//         c.r = (unsigned char)clamp((int)(color_srgb.r * 255.0f), 0, 255);
	//         c.g = (unsigned char)clamp((int)(color_srgb.g * 255.0f), 0, 255);
	//         c.b = (unsigned char)clamp((int)(color_srgb.b * 255.0f), 0, 255);
	//     }

	//     return *this;
	// }

	// template <>
	// TImage<Color4i> &TImage<Color4i>::ToSRGB()
	// {
	//     float scaleFactor = 1.0f / 255.0f;
	//     for (Color4i &c : pixels)
	//     {
	//         Color3f color_linear(scaleFactor * c.r, scaleFactor * c.g, scaleFactor * c.b);
	//         Color3f color_srgb = RGBtoSRGB(color_linear);
	//         c.r = clamp((int)(color_srgb.r * 255.0f), 0, 65535);
	//         c.g = clamp((int)(color_srgb.g * 255.0f), 0, 65535);
	//         c.b = clamp((int)(color_srgb.b * 255.0f), 0, 65535);
	//     }

	//     return *this;
	// }

	// template <>
	// TImage<Color3ub> &TImage<Color3ub>::ReverseSRGB()
	// {
	//     float scaleFactor = 1.0f / 255.0f;
	//     for (Color3ub &c : pixels)
	//     {
	//         Color3f color_srgb(scaleFactor * c.r, scaleFactor * c.g, scaleFactor * c.b);
	//         Color3f color_linear = SRGBtoRGB(color_srgb);
	//         c.r = (unsigned char)clamp((int)(color_linear.r * 255.0f), 0, 255);
	//         c.g = (unsigned char)clamp((int)(color_linear.g * 255.0f), 0, 255);
	//         c.b = (unsigned char)clamp((int)(color_linear.b * 255.0f), 0, 255);
	//     }

	//     return *this;
	// }

	// template <>
	// TImage<Color3i> &TImage<Color3i>::ReverseSRGB()
	// {
	//     float scaleFactor = 1.0f / 255.0f;
	//     for (Color3i &c : pixels)
	//     {
	//         Color3f color_srgb(scaleFactor * c.r, scaleFactor * c.g, scaleFactor * c.b);
	//         Color3f color_linear = SRGBtoRGB(color_srgb);
	//         c.r = clamp((int)(color_linear.r * 255.0f), 0, 65535);
	//         c.g = clamp((int)(color_linear.g * 255.0f), 0, 65535);
	//         c.b = clamp((int)(color_linear.b * 255.0f), 0, 65535);
	//     }

	//     return *this;
	// }

	// template <>
	// TImage<Color4ub> &TImage<Color4ub>::ReverseSRGB()
	// {
	//     float scaleFactor = 1.0f / 255.0f;
	//     for (Color4ub &c : pixels)
	//     {
	//         Color3f color_srgb(scaleFactor * c.r, scaleFactor * c.g, scaleFactor * c.b);
	//         Color3f color_linear = SRGBtoRGB(color_srgb);
	//         c.r = (unsigned char)clamp((int)(color_linear.r * 255.0f), 0, 255);
	//         c.g = (unsigned char)clamp((int)(color_linear.g * 255.0f), 0, 255);
	//         c.b = (unsigned char)clamp((int)(color_linear.b * 255.0f), 0, 255);
	//     }

	//     return *this;
	// }

	// template <>
	// TImage<Color4i> &TImage<Color4i>::ReverseSRGB()
	// {
	//     float scaleFactor = 1.0f / 255.0f;
	//     for (Color4i &c : pixels)
	//     {
	//         Color3f color_srgb(scaleFactor * c.r, scaleFactor * c.g, scaleFactor * c.b);
	//         Color3f color_linear = SRGBtoRGB(color_srgb);
	//         c.r = clamp((int)(color_linear.r * 255.0f), 0, 65535);
	//         c.g = clamp((int)(color_linear.g * 255.0f), 0, 65535);
	//         c.b = clamp((int)(color_linear.b * 255.0f), 0, 65535);
	//     }

	//     return *this;
	// }

	// template <>
	// TImage<Color3ub> &TImage<Color3ub>::ReverseToneMap(float exposure)
	// {
	//     float tm = 1.0f / (2.5f * powf(2.0f, exposure));
	//     for (Color3ub &c : pixels)
	//     {
	//         Color3i _c((int)(tm * (float)c.r), (int)(tm * (float)c.g), (int)(tm * (float)c.b));
	//         c.r = (unsigned char)clamp(_c.r, 0, 255);
	//         c.g = (unsigned char)clamp(_c.g, 0, 255);
	//         c.b = (unsigned char)clamp(_c.b, 0, 255);
	//     }

	//     return *this;
	// }

	// template <>
	// TImage<Color3i> &TImage<Color3i>::ReverseToneMap(float exposure)
	// {
	//     float tm = 1.0f / (2.5f * powf(2.0f, exposure));
	//     for (Color3i &c : pixels)
	//     {
	//         Color3i _c((int)(tm * (float)c.r), (int)(tm * (float)c.g), (int)(tm * (float)c.b));
	//         c.r = clamp(_c.r, 0, 65535);
	//         c.g = clamp(_c.g, 0, 65535);
	//         c.b = clamp(_c.b, 0, 65535);
	//     }

	//     return *this;
	// }

	// template <>
	// TImage<Color4ub> &TImage<Color4ub>::ReverseToneMap(float exposure)
	// {
	//     float tm = 1.0f / (2.5f * powf(2.0f, exposure));
	//     for (Color4ub &c : pixels)
	//     {
	//         Color3i _c((int)(tm * (float)c.r), (int)(tm * (float)c.g), (int)(tm * (float)c.b));
	//         c.r = (unsigned char)clamp(_c.r, 0, 255);
	//         c.g = (unsigned char)clamp(_c.g, 0, 255);
	//         c.b = (unsigned char)clamp(_c.b, 0, 255);
	//     }

	//     return *this;
	// }

	// template <>
	// TImage<Color4i> &TImage<Color4i>::ReverseToneMap(float exposure)
	// {
	//     float tm = 1.0f / (2.5f * powf(2.0f, exposure));
	//     for (Color4i &c : pixels)
	//     {
	//         Color3i _c((int)(tm * (float)c.r), (int)(tm * (float)c.g), (int)(tm * (float)c.b));
	//         c.r = clamp(_c.r, 0, 65535);
	//         c.g = clamp(_c.g, 0, 65535);
	//         c.b = clamp(_c.b, 0, 65535);
	//     }

	//     return *this;
	// }

	// template <>
	// TImage<Color3ub> &TImage<Color3ub>::ToneMap(float exposure)
	// {
	//     float tm = 2.5f * powf(2.0f, exposure);
	//     for (Color3ub &c : pixels)
	//     {
	//         Color3i _c((int)(tm * (float)c.r), (int)(tm * (float)c.g), (int)(tm * (float)c.b));
	//         c.r = (unsigned char)clamp(_c.r, 0, 255);
	//         c.g = (unsigned char)clamp(_c.g, 0, 255);
	//         c.b = (unsigned char)clamp(_c.b, 0, 255);
	//     }

	//     return *this;
	// }

	// template <>
	// TImage<Color4ub> &TImage<Color4ub>::ToneMap(float exposure)
	// {
	//     float tm = 2.5f * powf(2.0f, exposure);
	//     for (Color4ub &c : pixels)
	//     {
	//         Color3i _c((int)(tm * (float)c.r), (int)(tm * (float)c.g), (int)(tm * (float)c.b));
	//         c.r = (unsigned char)clamp(_c.r, 0, 255);
	//         c.g = (unsigned char)clamp(_c.g, 0, 255);
	//         c.b = (unsigned char)clamp(_c.b, 0, 255);
	//     }

	//     return *this;
	// }

	// template <>
	// TImage<Color3i> &TImage<Color3i>::ToneMap(float exposure)
	// {
	//     float tm = 2.5f * powf(2.0f, exposure);
	//     for (Color3i &c : pixels)
	//     {
	//         Color3i _c((int)(tm * (float)c.r), (int)(tm * (float)c.g), (int)(tm * (float)c.b));
	//         c.r = clamp(_c.r, 0, 65535);
	//         c.g = clamp(_c.g, 0, 65535);
	//         c.b = clamp(_c.b, 0, 65535);
	//     }

	//     return *this;
	// }

	// template <>
	// TImage<Color4i> &TImage<Color4i>::ToneMap(float exposure)
	// {
	//     float tm = 2.5f * powf(2.0f, exposure);
	//     for (Color4i &c : pixels)
	//     {
	//         Color3i _c((int)(tm * (float)c.r), (int)(tm * (float)c.g), (int)(tm * (float)c.b));
	//         c.r = clamp(_c.r, 0, 65535);
	//         c.g = clamp(_c.g, 0, 65535);
	//         c.b = clamp(_c.b, 0, 65535);
	//     }

	//     return *this;
	// }

	// template <>
	// void TImage<Color3i>::loadPPM(const std::string &filename)
	// {
	//     std::ifstream fin(filename.c_str());

	//     std::string magicNumber;
	//     int width;
	//     int height;
	//     int maxInt;

	//     fin >> magicNumber;
	//     if (magicNumber != "P3")
	//         return;
	//     fin >> width;
	//     fin >> height;
	//     fin >> maxInt;

	//     resize(width, height);
	//     for (int y = 0; y < imageHeight; y++)
	//     {
	//         for (int x = 0; x < imageWidth; x++)
	//         {
	//             Color3i color;
	//             fin >> color.r >> color.g >> color.b;
	//             setPixel(x, y, color);
	//         }
	//     }

	//     fin.close();
	// }

	// template <>
	// void TImage<Color4i>::loadPPM(const std::string &filename)
	// {
	//     std::ifstream fin(filename.c_str());

	//     std::string magicNumber;
	//     int width;
	//     int height;
	//     int maxInt;

	//     fin >> magicNumber;
	//     if (magicNumber != "P3")
	//         return;
	//     fin >> width;
	//     fin >> height;
	//     fin >> maxInt;

	//     resize(width, height);
	//     for (int y = 0; y < imageHeight; y++)
	//     {
	//         for (int x = 0; x < imageWidth; x++)
	//         {
	//             Color4i color;
	//             fin >> color.r >> color.g >> color.b;
	//             setPixel(x, y, color);
	//         }
	//     }

	//     fin.close();
	// }

	// template <>
	// void TImage<Color3ub>::loadPPM(const std::string &filename)
	// {
	//     std::ifstream fin(filename.c_str());

	//     std::string magicNumber;
	//     int width;
	//     int height;
	//     int maxInt;

	//     fin >> magicNumber;
	//     if (magicNumber != "P3")
	//         return;
	//     fin >> width;
	//     fin >> height;
	//     fin >> maxInt;

	//     resize(width, height);
	//     for (int y = 0; y < imageHeight; y++)
	//     {
	//         for (int x = 0; x < imageWidth; x++)
	//         {
	//             Color3ub color;
	//             fin >> color.r >> color.g >> color.b;
	//             setPixel(x, y, color);
	//         }
	//     }

	//     fin.close();
	// }

	// template <>
	// void TImage<Color4ub>::loadPPM(const std::string &filename)
	// {
	//     std::ifstream fin(filename.c_str());

	//     std::string magicNumber;
	//     int width;
	//     int height;
	//     int maxInt;

	//     fin >> magicNumber;
	//     if (magicNumber != "P3")
	//         return;
	//     fin >> width;
	//     fin >> height;
	//     fin >> maxInt;

	//     resize(width, height);
	//     for (int y = 0; y < imageHeight; y++)
	//     {
	//         for (int x = 0; x < imageWidth; x++)
	//         {
	//             Color4ub color;
	//             fin >> color.r >> color.g >> color.b;
	//             setPixel(x, y, color);
	//         }
	//     }

	//     fin.close();
	// }

	// template <>
	// void TImage<Color3f>::loadPPM(const std::string &filename)
	// {
	//     std::ifstream fin(filename.c_str());

	//     std::string magicNumber;
	//     int width;
	//     int height;
	//     int maxInt;

	//     fin >> magicNumber;
	//     if (magicNumber != "P3")
	//         return;
	//     fin >> width;
	//     fin >> height;
	//     fin >> maxInt;

	//     float scaleFactor = 1.0f / maxInt;
	//     resize(width, height);
	//     for (int y = 0; y < imageHeight; y++)
	//     {
	//         for (int x = 0; x < imageWidth; x++)
	//         {
	//             Color3i color;
	//             fin >> color.r >> color.g >> color.b;
	//             Color3f color_srgb(color.r * scaleFactor, color.g * scaleFactor, color.b * scaleFactor);
	//             setPixel(x, y, color_srgb);
	//         }
	//     }

	//     fin.close();
	// }

	// template <>
	// void TImage<Color4f>::loadPPM(const std::string &filename)
	// {
	//     std::ifstream fin(filename.c_str());

	//     std::string magicNumber;
	//     int width;
	//     int height;
	//     int maxInt;

	//     fin >> magicNumber;
	//     if (magicNumber != "P3")
	//         return;
	//     fin >> width;
	//     fin >> height;
	//     fin >> maxInt;

	//     float scaleFactor = 1.0f / maxInt;

	//     resize(width, height);
	//     for (int y = 0; y < imageHeight; y++)
	//     {
	//         for (int x = 0; x < imageWidth; x++)
	//         {
	//             Color3i color;
	//             fin >> color.r >> color.g >> color.b;
	//             Color4f color_srgb(color.r * scaleFactor, color.g * scaleFactor, color.b * scaleFactor, 1.0f);
	//             setPixel(x, y, color_srgb);
	//         }
	//     }

	//     fin.close();
	// }

	template <typename ColorType>
	TImage<ColorType>& TImage<ColorType>::ToSRGB() {
		for (ColorType& c : pixels) {
			Color3f color_linear = ToColor3f(c);
			Color3f color_srgb = RGBtoSRGB(color_linear);
			FromColor3f(c, color_srgb.clamp());
		}

		return *this;
	}

	template <typename ColorType>
	TImage<ColorType>& TImage<ColorType>::ReverseSRGB() {
		for (ColorType& c : pixels) {
			Color3f color_srgb = ToColor3f(c);
			Color3f color_linear = SRGBtoRGB(color_srgb);
			FromColor3f(c, color_linear.clamp());
		}

		return *this;
	}

	template <typename ColorType>
	TImage<ColorType>& TImage<ColorType>::ReverseToneMap(float exposure) {
		float tm = 1.0f / (2.5f * powf(2.0f, exposure));
		for (ColorType& c : pixels) {
			Color3f color = ToColor3f(c);
			color *= tm;
			FromColor3f(c, color.clamp());
		}

		return *this;
	}

	template <typename ColorType>
	TImage<ColorType>& TImage<ColorType>::ToneMap(float exposure) {
		float tm = 2.5f * powf(2.0f, exposure);
		for (ColorType& c : pixels) {
			Color3f color = ToColor3f(c);
			color *= tm;
			FromColor3f(c, color.clamp());
		}

		return *this;
	}

	template <typename ColorType>
	void TImage<ColorType>::scaleColors(const float x) {
		for (vector_type& c : pixels) {
			c *= (typename ColorType::value_type)x;
		}
	}

	template <typename ColorType>
	TImage<ColorType> TImage<ColorType>::ScaleImage(unsigned newWidth, unsigned newHeight, bool bilinear) {
		TImage<ColorType> out;
		out.resize(newWidth, newHeight, imageDepth);
		unsigned newDepth = imageDepth;
		Vector3d d(imageWidth / (double)newWidth, imageHeight / (double)newHeight, imageDepth / (double)newDepth);
		Vector3d src;
		for (unsigned z = 0; z < newDepth; z++) {
			src.y = 0.0;
			for (unsigned y = 0; y < newHeight; y++) {
				src.x = 0.0;
				for (unsigned x = 0; x < newWidth; x++) {
					// truncate coordinates
					Vector3i pint((int)src.x, (int)src.y, (int)src.z);
					ColorType closestPixel = getPixelUnsafe(pint.x, pint.y, pint.z);
					out.setPixelUnsafe(x, y, z, closestPixel);
					src.x += d.x;
				}
				src.y += d.y;
			}
			src.z += d.z;
		}

		return out;
	}

	//template <typename ColorType>
	//void TImage<ColorType>::savePPMRaw(const std::string& filename, unsigned z) const {
	//	float maxColorFound = maxrgb() * scale;
	//	const float scale = ColorType::to_float_factor * 255.99f / maxColorFound;
	//	if (maxColorFound < 255.0f)
	//		maxColorFound = 255.0f;

	//	std::ofstream fout(filename.c_str());
	//	fout << "P3" << std::endl;
	//	fout << imageWidth << " ";
	//	fout << imageHeight << " ";
	//	fout << maxColorFound << std::endl;

	//	for (unsigned y = 0; y < imageHeight; y++) {
	//		for (unsigned x = 0; x < imageWidth; x++) {
	//			Color3f color = getPixel(x, y, z) * scale;
	//			int ir = (int)color.r;
	//			int ig = (int)color.g;
	//			int ib = (int)color.b;
	//			fout << ir << " " << ig << " " << ib << std::endl;
	//		}
	//	}

	//	fout.close();
	//}

	//template <typename ColorType>
	//void TImage<ColorType>::loadPPM(const std::string& filename) {
	//	std::ifstream fin(filename.c_str());

	//	std::string magicNumber;
	//	unsigned width;
	//	unsigned height;
	//	int maxInt;

	//	fin >> magicNumber;
	//	while (!magicNumber.empty() && magicNumber[0] == '#') {
	//		// skip comments
	//		fin >> magicNumber;
	//	}
	//	if (magicNumber != "P3")
	//		return;
	//	fin >> width;
	//	fin >> height;
	//	fin >> maxInt;

	//	// decide what to do...
	//	// const unsigned int size = ColorType::gl_size;
	//	// const unsigned int type = ColorType::gl_type;
	//	const float scale = ColorType::from_float_factor / 255.99f;

	//	resize(width, height);
	//	for (unsigned y = 0; y < imageHeight; y++) {
	//		for (unsigned x = 0; x < imageWidth; x++) {
	//			Color3f color;
	//			fin >> color.r >> color.g >> color.b;
	//			color *= scale;
	//			ColorType dst;
	//			FromColor3f(dst, color);
	//			setPixel(x, y, dst);
	//		}
	//	}

	//	fin.close();
	//}

	template <typename ColorType>
	void TImage<ColorType>::savePPM(const std::string& filename, unsigned z, bool flipy) const {
		if (pixels.empty()) return;
		if (pixels[0].size() < 3) return;

		float maxColorFound = (float)maxrgb();
		float minColorFound = (float)minrgb();
		constexpr float scale = color_to_float_factor<scalar_type>();

		float imageColorRange = 255;
		if (std::is_integral_v<scalar_type>) {
			imageColorRange = std::max(maxColorFound, 255.0f);
		}
		else if (std::is_floating_point_v<scalar_type>) {
			imageColorRange = std::floor(std::max(maxColorFound, 1.0f) * 255.99f);
		}

		// Typical cases:
		// Float image range is [0, 1]
		// Float image range is [0, ?] which we want to scale
		// UBYTE image range is [0, 255]
		// INT image range is [0, 65535]
		// INT image range needs to be clamped

		std::string comment = "# ";
		comment += filename;
		comment += " with range [";
		comment += std::to_string(minColorFound);
		comment += ", ";
		comment += std::to_string(maxColorFound);
		comment += "] scaled by " + std::to_string(scale);

		std::ofstream fout(filename.c_str());
		fout << "P3" << "\n";
		fout << comment << "\n";
		fout << imageWidth << " ";
		fout << imageHeight << " ";
		fout << imageColorRange << "\n";

		int y1 = 0;
		int y2 = (int)imageHeight;
		int dy = 1;
		if (flipy) {
			y1 = (int)imageHeight - 1;
			y2 = -1;
			dy = -1;
		}
		constexpr unsigned count = 3;
		for (int y = y1; y != y2; y += dy) {
			for (int x = 0; x < (int)imageWidth; x++) {
				ColorType c = getPixel(x, y, z);
				if (std::is_floating_point_v<scalar_type>) {
					constexpr int high = 65535;
					fout << (int)clamp(int(c[0] * 255.99), 0, high) << " ";
					fout << (int)clamp(int(c[1] * 255.99), 0, high) << " ";
					fout << (int)clamp(int(c[2] * 255.99), 0, high) << " ";
				}
				else if (std::is_integral_v<scalar_type>) {
					constexpr int high =
						sizeof(scalar_type) == 1 ? 255 : 65535;
					fout << (int)clamp(int(c[0]), 0, high) << " ";
					fout << (int)clamp(int(c[1]), 0, high) << " ";
					fout << (int)clamp(int(c[2]), 0, high) << " ";
				}
				else {
					fout << c[0] << " ";
					fout << c[1] << " ";
					fout << c[2];
				}
				fout << "\n";
			}
		}

		fout.close();
	}
	//
	//	template <typename ColorType>
	//	void TImage<ColorType>::savePPMi(const std::string& filename, float scale, int minValue, int maxValue, unsigned z, bool flipy) const {
	//		//if (maxValue <= 0) {
	//		//	maxValue = (int)(scale * maxrgb());
	//		//}
	//
	//		std::ofstream fout(filename.c_str());
	//
	//		fout << "P3" << std::endl;
	//		fout << imageWidth << " ";
	//		fout << imageHeight << " ";
	//		fout << maxValue << std::endl;
	//
	//		int y1 = 0;
	//		int y2 = (int)imageHeight;
	//		int dy = 1;
	//		if (flipy) {
	//			y1 = (int)imageHeight - 1;
	//			y2 = -1;
	//			dy = -1;
	//		}
	//		for (int y = y1; y != y2; y += dy) {
	//			for (int x = 0; x < (int)imageWidth; x++) {
	//				Color4i color = ToColor4i(getPixel(x, y, z), scale, minValue, maxValue);
	//				fout << color.r << " " << color.g << " " << color.b << std::endl;
	//			}
	//		}
	//
	//		fout.close();
	//	}
	//
	//	template <typename ColorType>
	//	void TImage<ColorType>::savePPMHDRI(const std::string& filename, unsigned z) const {
	//		std::ofstream fout(filename.c_str());
	//		float scale = ColorType::to_float_factor;
	//		fout << "P3" << std::endl;
	//		fout << imageWidth << " ";
	//		fout << imageHeight << " ";
	//		fout << "1.0" << std::endl;
	//
	//		for (unsigned y = 0; y < imageHeight; y++) {
	//			for (unsigned x = 0; x < imageWidth; x++) {
	//				ColorType c = getPixel(x, y, z);
	//				Color3f color(clamp(c.r * scale, 0.0f, 1.0f),
	//							  clamp(c.g * scale, 0.0f, 1.0f),
	//							  clamp(c.b * scale, 0.0f, 1.0f)
	//				);
	//				fout << color.r << " " << color.g << " " << color.b << std::endl;
	//			}
	//		}
	//
	//		fout.close();
	//	}
	//
	template <typename ColorType>
	void TImage<ColorType>::saveCubePPM(const std::string& path, bool flipy) const {
		if (imageHeight * 6 == imageWidth) {
			savePPM(path, 0, flipy);
			return;
		}

		if (imageDepth == 6) {
			TImage<ColorType> image;
			convertCubeMapToRect(image);
			image.savePPM(path, 0, flipy);
		}

		return;
	}
	//
	//	template <typename ColorType>
	//	void TImage<ColorType>::loadEXR(const std::string& path) {
	//#ifdef FLUXIONS_GTE_USEOPENEXR
	//		double t1 = Hf::Log.getMillisecondsElapsed();
	//		Imf::RgbaInputFile file(path.c_str());
	//		Imath::Box2i dw = file.dataWindow();
	//		unsigned w = dw.max.x - dw.min.x + 1;
	//		unsigned h = dw.max.y - dw.min.y + 1;
	//		//Imf::Array2D<Imf::Rgba> filePixels;
	//		std::vector<Imf::Rgba> filePixels(w * h);
	//		//filePixels.resizeErase(h, w);
	//		file.setFrameBuffer(&filePixels[0], 1, w);
	//		file.readPixels(dw.min.y, dw.max.y);
	//
	//		resize(w, h);
	//		unsigned addr = 0;
	//		float minC = 1e6;
	//		float maxC = -1e6;
	//		for (unsigned j = 0; j < h; j++) {
	//			for (unsigned i = 0; i < w; i++) {
	//				Imf::Rgba rgba = filePixels[addr];
	//				Color3f color(float(rgba.r), float(rgba.g), float(rgba.b));
	//				minC = std::min(minC, color.minrgb());
	//				maxC = std::max(maxC, color.maxrgb());
	//				setPixel(i, j, color);
	//				addr++;
	//			}
	//		}
	//		t1 = Hf::Log.getMillisecondsElapsed() - t1;
	//		Hf::Log.infofn("TImage<>::loadEXR", "Read %dx%d image from %s (%3f ms) (min, max) = (%-2.3f, %-2.3f)", w, h, path.c_str(), t1, minC, maxC);
	//#endif
	//	}
	//
	//	template <typename ColorType>
	//	void TImage<ColorType>::saveEXR(const std::string& path) const {
	//#ifdef FLUXIONS_GTE_USEOPENEXR
	//		double t1 = Hf::Log.getMillisecondsElapsed();
	//		const Imf::Rgba black(0.0f, 0.0f, 0.0f, 1.0f);
	//		std::vector<Imf::Rgba> halfPixels(imageWidth * imageHeight * imageDepth, black);
	//		const unsigned count = imageWidth * imageHeight * imageDepth;
	//		for (unsigned i = 0; i < count; i++) {
	//			Color3f color = ToColor3f(pixels[i]);
	//			halfPixels[i] = Imf::Rgba(color.r, color.g, color.b);
	//		}
	//		Imf::RgbaOutputFile file(path.c_str(), (int)imageWidth, (int)imageHeight, Imf::WRITE_RGBA);
	//		file.setFrameBuffer(halfPixels.data(), 1, imageWidth);
	//		file.writePixels((int)imageHeight);
	//		t1 = Hf::Log.getMillisecondsElapsed() - t1;
	//		Hf::Log.infofn("TImage<>::saveEXR", "Wrote %dx%d image to %s (%3f ms)", imageWidth, imageHeight, path.c_str(), t1);
	//#endif
	//	}

	template <typename ColorType>
	void TImage<ColorType>::resize(unsigned width, unsigned height, unsigned depth) {
		imageWidth = width;
		imageHeight = height;
		imageDepth = depth;
		unsigned pixelCount = width * height * depth;
		if (pixelCount == 0)
			pixels.clear();
		else
			pixels.resize(pixelCount);
		zstride = imageWidth * imageHeight;
	}

	template <typename ColorType>
	void TImage<ColorType>::clear(const ColorType& clearcolor) {
		for (ColorType& c : pixels) {
			c = clearcolor;
		}
	}

	template <typename ColorType>
	void TImage<ColorType>::setImageData(unsigned int format, unsigned int type, unsigned width, unsigned height, unsigned depth, void* _pixels) {
		_setImageData(format, type, ColorType::gl_type, ColorType::gl_size, width, height, depth, _pixels);
	}

	template <typename ColorType>
	void TImage<ColorType>::_setImageData(unsigned int fromFormat, unsigned int fromType, unsigned int toFormat, unsigned int toType, unsigned width, unsigned height, unsigned depth, void* _pixels) {
		float scaleFactor_itof = 1.0f / 255.99f;
		float scaleFactor_ftoi = 255.99f;

		const unsigned glconstant_RGB = 0x1907;
		const unsigned glconstant_RGBA = 0x1908;
		const unsigned glconstant_FLOAT = 0x1406;
		const unsigned glconstant_UNSIGNED_BYTE = 0x1401;

		unsigned stride;
		if (fromFormat == 3 || fromFormat == glconstant_RGB)
			stride = 3;
		else if (fromFormat == 4 || fromFormat == glconstant_RGBA)
			stride = 4;
		else
			return;

		resize(width, height, depth);
		unsigned count = width * height * depth;
		if (fromType == glconstant_UNSIGNED_BYTE && toType == glconstant_FLOAT) {
			unsigned char* data = (unsigned char*)_pixels;
			for (unsigned i = 0; i < count; i++) {
				typename ColorType::value_type* v = pixels[i].ptr();
				for (unsigned j = 0; j < stride; j++) {
					v[j] = (typename ColorType::value_type)clamp((int)(scaleFactor_itof * data[j]), 0, 255);
				}
				data += stride;
			}
		}
		else if (fromType == glconstant_FLOAT && toType == glconstant_UNSIGNED_BYTE) {
			float* data = (float*)_pixels;
			for (unsigned i = 0; i < count; i++) {
				typename ColorType::value_type* v = pixels[i].ptr();
				for (unsigned j = 0; j < stride; j++) {
					v[j] = (typename ColorType::value_type)clamp((int)(scaleFactor_ftoi * data[j]), 0, 255);
				}
				data += stride;
			}
		}
		else if (fromType == glconstant_UNSIGNED_BYTE && toType == glconstant_UNSIGNED_BYTE) {
			unsigned char* data = (unsigned char*)_pixels;
			for (unsigned i = 0; i < count; i++) {
				typename ColorType::value_type* v = pixels[i].ptr();
				for (unsigned j = 0; j < stride; j++) {
					v[j] = (typename ColorType::value_type)data[j];
				}
				data += stride;
			}
		}
		else if (fromType == glconstant_FLOAT && toType == glconstant_FLOAT) {
			float* data = (float*)_pixels;
			for (unsigned i = 0; i < count; i++) {
				typename ColorType::value_type* v = pixels[i].ptr();
				for (unsigned j = 0; j < stride; j++) {
					v[j] = (typename ColorType::value_type)data[j];
				}
				data += stride;
			}
		}
	}

	// Square image operations
	template <typename ColorType>
	bool TImage<ColorType>::flipX(int z) {
		if (empty() || imageWidth != imageHeight)
			return false;

		std::vector<ColorType> tmp(imageWidth * imageHeight);
		unsigned size = imageWidth;
		//unsigned zstride = imageWidth * imageHeight;
		unsigned zoffset = zstride * z;
		for (unsigned x = 0; x < size; x++) {
			for (unsigned y = 0; y < size; y++) {
				unsigned sx = size - x - 1;
				unsigned sy = y;
				unsigned dst_offset = y * size + x;
				unsigned src_offset = sy * size + sx;
				tmp[dst_offset] = pixels[zoffset + src_offset];
			}
		}
		copy(tmp.begin(), tmp.end(), pixels.begin() + zoffset);

		return true;
	}

	template <typename ColorType>
	bool TImage<ColorType>::flipY(int z) {
		if (empty() || imageWidth != imageHeight)
			return false;

		std::vector<ColorType> tmp(imageWidth * imageHeight);
		unsigned size = imageWidth;
		//unsigned zstride = imageWidth * imageHeight;
		unsigned zoffset = zstride * z;
		for (unsigned x = 0; x < size; x++) {
			for (unsigned y = 0; y < size; y++) {
				unsigned sx = x;
				unsigned sy = size - y - 1;
				unsigned dst_offset = y * size + x;
				unsigned src_offset = sy * size + sx;
				tmp[dst_offset] = pixels[zoffset + src_offset];
			}
		}
		copy(tmp.begin(), tmp.end(), pixels.begin() + zoffset);

		return true;
	}

	template <typename ColorType>
	bool TImage<ColorType>::rotateLeft90(int z) {
		if (empty() || (imageWidth != imageHeight))
			return false;

		std::vector<ColorType> tmp(imageWidth * imageHeight);
		unsigned size = imageWidth;
		//unsigned zstride = imageWidth * imageHeight;
		unsigned zoffset = zstride * z;
		for (unsigned x = 0; x < size; x++) {
			for (unsigned y = 0; y < size; y++) {
				int sx = (int)(size - y - 1);
				int sy = (int)x;
				unsigned dst_offset = y * size + x;
				unsigned src_offset = sy * size + sx;
				tmp[dst_offset] = pixels[zoffset + src_offset];
			}
		}
		//copy(tmp.begin(), tmp.end(), pixels.begin() + zoffset);
		void* pdst = &pixels[zoffset];
		void* psrc = &tmp[0];
		unsigned n = zstride * sizeof(ColorType);
		memcpy(pdst, psrc, n);
		// memcpy(&pixels[zoffset], &tmp[0], zstride * sizeof(ColorType));

		return true;
	}

	template <typename ColorType>
	bool TImage<ColorType>::rotateRight90(int z) {
		if (empty() || imageWidth != imageHeight)
			return false;

		std::vector<ColorType> tmp(imageWidth * imageHeight);
		unsigned size = imageWidth;
		//unsigned zstride = imageWidth * imageHeight;
		unsigned zoffset = zstride * z;
		for (unsigned x = 0; x < size; x++) {
			for (unsigned y = 0; y < size; y++) {
				int sx = (int)y;
				int sy = (int)(size - x - 1);
				unsigned dst_offset = y * size + x;
				unsigned src_offset = sy * size + sx;
				tmp[dst_offset] = pixels[zoffset + src_offset];
			}
		}

		void* dst = &pixels[zoffset];
		void* src = &tmp[0];
		unsigned n = zstride * sizeof(ColorType);
		memcpy(dst, src, n);
		//copy(tmp.begin(), tmp.end(), pixels.begin() + zoffset);

		return true;
	}

	template <typename ColorType>
	bool TImage<ColorType>::convertRectToCubeMap() {
		if (empty() || imageWidth != 6 * imageHeight)
			return false;

		// savePPMi("blah.ppm", 255.99f, 0, 255, 0);

		int swizzle[6] = {
			4, // POSITIVE Z
			5, // NEGATIVE Z
			2, // POSITIVE Y
			3, // NEGATIVE Y
			1, // POSITIVE X
			0, // NEGATIVE X
		};
		int size = (int)imageHeight;
		std::vector<ColorType> src = pixels;
		resize(size, size, 6);

		for (unsigned i = 0; i < 6; i++) {
			unsigned k = swizzle[i];
			// demultiplex the data
			unsigned dst_offset = i * zstride;
			unsigned src_offset = k * size;
			for (int y = 0; y < size; y++) {
				void* pdst = &pixels[dst_offset];
				void* psrc = &src[src_offset];
				unsigned n = size * sizeof(ColorType);
				memcpy(pdst, psrc, n);
				dst_offset += size;
				src_offset += 6 * size;
			}
		}

		rotateRight90(3);
		rotateLeft90(2);

		return true;
	}

	template <typename ColorType>
	bool TImage<ColorType>::convertRectToCubeMap(TImage<ColorType>& dst) const {
		if (empty() || imageWidth != 6 * imageHeight)
			return false;

		int swizzle[6] = {
			4, // POSITIVE Z
			5, // NEGATIVE Z
			2, // POSITIVE Y
			3, // NEGATIVE Y
			1, // POSITIVE X
			0, // NEGATIVE X
		};
		int size = (int)imageHeight;
		const std::vector<ColorType>& src = pixels;
		dst.resize(size, size, 6);

		for (int i = 0; i < 6; i++) {
			int k = swizzle[i];
			// demultiplex the data
			unsigned dst_offset = i * zstride;
			unsigned src_offset = k * size;
			for (int y = 0; y < size; y++) {
				void* pdst = &dst.pixels[dst_offset];
				const void* psrc = &src[src_offset];
				unsigned n = size * sizeof(ColorType);
				memcpy(pdst, psrc, n);
				dst_offset += size;
				src_offset += 6 * size;
			}
		}

		dst.rotateRight90(3);
		dst.rotateLeft90(2);

		return true;
	}

	template <typename ColorType>
	bool TImage<ColorType>::convertCubeMapToRect() {
		if (empty() || ((imageWidth != imageHeight) && (imageDepth != 6)))
			return false;

		rotateLeft90(2);
		rotateRight90(3);

		unsigned size = imageWidth;
		std::vector<ColorType> tmp = pixels;
		resize(size * 6, size, 1);

		unsigned swizzle[6] = {
			4, // POSITIVE Z
			5, // NEGATIVE Z
			2, // POSITIVE Y
			3, // NEGATIVE Y
			1, // POSITIVE X
			0, // NEGATIVE X
		};

		for (unsigned i = 0; i < 6; i++) {
			unsigned k = swizzle[i];
			// demultiplex the data
			unsigned src_offset = i * size * size;
			unsigned dst_offset = k * size;
			for (unsigned y = 0; y < size; y++) {
				void* pdst = &pixels[dst_offset];
				void* psrc = &tmp[src_offset];
				unsigned n = size * sizeof(ColorType);
				memcpy(pdst, psrc, n);
				src_offset += size;
				dst_offset += 6 * size;
			}
		}

		return true;
	}

	template <typename ColorType>
	bool TImage<ColorType>::convertCubeMapToRect(TImage<ColorType>& dst) const {
		if (empty() || ((imageWidth != imageHeight) && (imageDepth != 6)))
			return false;

		TImage<ColorType> src(*this);
		src.rotateLeft90(2);
		src.rotateRight90(3);

		int size = (int)imageWidth;
		dst.resize(size * 6, size, 1);

		int swizzle[6] = {
			4, // POSITIVE Z
			5, // NEGATIVE Z
			2, // POSITIVE Y
			3, // NEGATIVE Y
			1, // POSITIVE X
			0, // NEGATIVE X
		};

		for (int i = 0; i < 6; i++) {
			int k = swizzle[i];
			// demultiplex the data
			unsigned src_offset = i * size * size;
			unsigned dst_offset = k * size;
			for (int y = 0; y < size; y++) {
				auto srcit = src.pixels.cbegin() + src_offset;
				auto dstit = dst.pixels.begin() + dst_offset;
				std::copy_n(srcit, size, dstit);
				src_offset += size;
				dst_offset += 6 * size;
			}
		}
		src.resize(1,1,1);

		return true;
	}

	template <typename ColorType>
	double TImage<ColorType>::getTotalIntensity() const {
		double I = 0.0;
		for (auto& c : pixels) {
			I += c.Intensity();
		}
		return I;
	}

	template <typename ColorType>
	double TImage<ColorType>::getMinIntensity() const {
		double I = 1e6;
		for (auto& c : pixels) {
			if (c.Intensity() < I)
				I = c.Intensity();
		}
		return I;
	}

	template <typename ColorType>
	double TImage<ColorType>::getMaxIntensity() const {
		double I = -1e6;
		for (auto& c : pixels) {
			if (c.Intensity() > I)
				I = c.Intensity();
		}
		return I;
	}

	template <typename ColorType>
	double TImage<ColorType>::getMeanIntensity() const {
		double I = 0.0;
		for (auto& c : pixels) {
			I += c.Intensity();
		}
		I /= (double)getNumPixels();
		return I;
	}

	// EXPLICIT INSTANTIATION /////////////////////////////////////////////////////

	template class TImage<Color3i>;
	template class TImage<Color3f>;
	template class TImage<Color3us>;
	template class TImage<Color3ub>;

	template class TImage<Color4i>;
	template class TImage<Color4f>;
	template class TImage<Color4us>;
	template class TImage<Color4ub>;

	// TESTS //////////////////////////////////////////////////////////////////////


	template <typename ColorType>
	bool TestTImage(const char* testbase) {
		constexpr int width = 8;
		constexpr int height = 8;
		using value_type = typename ColorType::value_type;
		value_type black = 0;
		value_type white = color_max_value<value_type>();
		value_type grey1 = (black + white) / 4;
		value_type grey2 = (black + white) / 2;
		value_type grey3 = grey2 + grey1;

		ColorType blackPixel{ grey1, grey2, grey3 };
		ColorType whitePixel{ grey3, grey1, grey2 };

#define COLORTYPENAME(X) #X
		using std::cerr;
		cerr << testbase << "  ";
		cerr << "black " << (double)black;
		cerr << "|<-- " << (double)grey1;
		cerr << "-->| " << (double)white << "\n";

		{	// Write rectangular image
			TImage<ColorType> image(width, height, 1);
			bool rowColorChoice = false;
			for (unsigned y = 0; y < height; y++) {
				bool colColorChoice = rowColorChoice;
				for (unsigned x = 0; x < width; x++) {
					image.setPixel(x, y, colColorChoice ? blackPixel : whitePixel);
					colColorChoice = !colColorChoice;
				}
				rowColorChoice = !rowColorChoice;
			}
			image.savePPM(testbase + std::string("-rect-test.ppm"));
			image.savePPM(testbase + std::string("-rect-test-flipped.ppm"), 0, true);
		}

		{	// Write out Cube image
			std::vector<ColorType> pixelColors[2]{
				{
					{ grey3, grey1, grey1 }, // +X
					{ grey1, grey3, grey3 }, // -X
					{ grey1, grey3, grey1 }, // +Y
					{ grey3, grey1, grey3 }, // -Y
					{ grey1, grey1, grey3 }, // +Z
					{ grey3, grey3, grey1 }, // -Z
				},{
					{ grey3, grey2, grey2 }, // +X
					{ grey2, grey3, grey3 }, // -X
					{ grey2, grey3, grey2 }, // +Y
					{ grey3, grey2, grey3 }, // -Y
					{ grey2, grey2, grey3 }, // +Z
					{ grey3, grey3, grey2 }, // -Z
				}
			};
			TImage<ColorType> cubeimage(width, height, 6);
			bool colorChoice = false;
			for (unsigned z = 0; z < 6; z++) {
				bool rowColorChoice = false;
				ColorType blackPixel = pixelColors[0][z];
				ColorType whitePixel = pixelColors[1][z];
				for (unsigned y = 0; y < height; y++) {
					bool colColorChoice = rowColorChoice;
					for (unsigned x = 0; x < width; x++) {
						cubeimage.setPixel(x, y, z, colColorChoice ?
										   blackPixel : whitePixel);
						colColorChoice = !colColorChoice;
					}
					rowColorChoice = !rowColorChoice;
				}
			}
			cubeimage.saveCubePPM(testbase + std::string("-cube-test.ppm"));
			cubeimage.saveCubePPM(testbase + std::string("-cube-test-flipped.ppm"), true);
		}

		return true;
	}

	void TestImage() {
#define FLUXIONS_TEST(test) fprintf(stderr, "%s(): Test " #test " was %s\n", __FUNCTION__, ((test) ? "successful" : "unsuccessful"));
		FLUXIONS_TEST(TestTImage<Color4f>("Color4f"));
		FLUXIONS_TEST(TestTImage<Color4i>("Color4i"));
		FLUXIONS_TEST(TestTImage<Color4us>("Color4us"));
		FLUXIONS_TEST(TestTImage<Color4ub>("Color4ub"));
		FLUXIONS_TEST(TestTImage<Color3f>("Color3f"));
		FLUXIONS_TEST(TestTImage<Color3i>("Color3i"));
		FLUXIONS_TEST(TestTImage<Color3us>("Color3us"));
		FLUXIONS_TEST(TestTImage<Color3ub>("Color3ub"));
		FLUXIONS_TEST(TestTImage<std::vector<float>>("vector-float"));
		FLUXIONS_TEST(TestTImage<std::vector<unsigned short>>("vector-ushort"));
		FLUXIONS_TEST(TestTImage<std::vector<unsigned char>>("vector-ubyte"));
	}
} // namespace Fluxions
