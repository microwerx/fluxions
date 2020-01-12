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
#include <iostream>
#ifdef _MSC_VER
#elif defined(__unix__)
#include <byteswap.h>
#endif
#include <hatchetfish.hpp>
#include <fluxions_gte_color_math.hpp>
#include <fluxions_gte_image.hpp>
#include <fluxions_gte_image_operations.hpp>

#define _CRT_SECURE_NO_WARNINGS
#ifdef _WIN32
#pragma warning(disable : 4996)
#endif

#ifdef FLUXIONS_GTE_USEOPENEXR
#ifdef _WIN32
#define OPENEXR_DLL
#endif
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
	TImage<ColorType> TImage<ColorType>::ScaleImage(unsigned newWidth, unsigned newHeight) {
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

	template <typename ColorType>
	bool TImage<ColorType>::loadPPM(const std::string& filename) {
		std::ifstream fin(filename.c_str());
		if (!fin) return false;
		std::string magicNumber;
		unsigned width;
		unsigned height;
		int maxInt;

		fin >> magicNumber;
		while (!magicNumber.empty() && magicNumber[0] == '#') {
			// skip comments
			fin >> magicNumber;
		}
		if (magicNumber != "P3")
			return false;
		fin >> width;
		fin >> height;
		fin >> maxInt;

		// decide what to do...
		// const unsigned int size = ColorType::gl_size;
		// const unsigned int type = ColorType::gl_type;
		const float scale = ColorType::from_float_factor / 255.99f;

		resize(width, height);
		for (unsigned y = 0; y < imageHeight; y++) {
			for (unsigned x = 0; x < imageWidth; x++) {
				Color3f color;
				fin >> color.r >> color.g >> color.b;
				color *= scale;
				ColorType dst;
				FromColor3f(dst, color);
				setPixel(x, y, dst);
			}
		}

		return false;
	}

	template <typename ColorType>
	bool TImage<ColorType>::savePPM(const std::string& filename, unsigned z, bool flipy) const {
		if (pixels.empty()) return false;
		if (pixels[0].size() < 3) return false;

		float maxColorFound = (float)maxrgb();
		float minColorFound = (float)minrgb();
		constexpr float scale = color_to_float_factor<scalar_type>();

		float imageColorRange = 255;
		if (std::is_integral_v<scalar_type>) {
			imageColorRange = std::max(maxColorFound, 255.0f);
		}
		else if (std::is_floating_point_v<scalar_type>) {
			imageColorRange = std::min(65535, (int)std::floor(std::max(maxColorFound, 1.0f) * 255.99f));
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

		return true;
	}

	template <typename ColorType>
	bool TImage<ColorType>::savePPMi(const std::string& filename, float scale, int minValue, int maxValue, unsigned z, bool flipy) const {
		std::ofstream fout(filename.c_str());
		if (!fout) return false;
		if (pixels.empty()) return false;
		if (pixels[0].size() < 3) return false;

		fout << "P3" << std::endl;
		fout << imageWidth << " ";
		fout << imageHeight << " ";
		fout << maxValue << std::endl;

		int y1 = 0;
		int y2 = (int)imageHeight;
		int dy = 1;
		if (flipy) {
			y1 = (int)imageHeight - 1;
			y2 = -1;
			dy = -1;
		}
		for (int y = y1; y != y2; y += dy) {
			for (int x = 0; x < (int)imageWidth; x++) {
				ColorType c = getPixel(x, y, z);
				Color3i color(
					clamp<int>((int)(c[0] * scale), minValue, maxValue),
					clamp<int>((int)(c[1] * scale), minValue, maxValue),
					clamp<int>((int)(c[2] * scale), minValue, maxValue)
				);
				fout << color.r << " " << color.g << " " << color.b << "\n";
			}
		}

		return true;
	}
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
	bool TImage<ColorType>::saveCubePPM(const std::string& path, bool flipy) const {
		if (imageHeight * 6 == imageWidth) {
			return savePPM(path, 0, flipy);
		}

		if (imageDepth == 6) {
			TImage<ColorType> image;
			convertCubeMapToRect(image);
			return image.savePPM(path, 0, flipy);
		}

		return false;
	}

	template <typename ColorType>
	bool TImage<ColorType>::saveCubePPMi(const std::string& path, float scale, int minValue, int maxValue, bool flipy) const {
		if (imageHeight * 6 == imageWidth) {
			return savePPMi(path, scale, minValue, maxValue, 0, flipy);
		}

		if (imageDepth == 6) {
			TImage<ColorType> image;
			convertCubeMapToRect(image);
			return image.savePPMi(path, scale, minValue, maxValue, 0, flipy);
		}

		return false;
	}
#ifdef _MSC_VER
#define FX_SWAP32INT(v) _byteswap_ulong(v);
#elif defined(__APPLE__)
#define FX_SWAP32INT(v) OSSwapInt32(v);
#elif defined(__unix__)
#define FX_SWAP32INT(v) __bswap_32(v);
#endif
	float read_float_reversebytes(std::ifstream& fin) {
		unsigned v;
		fin.read((char*)&v, 4);
		v = FX_SWAP32INT(v);
		return *reinterpret_cast<float*>(&v);
	}

	inline void reverse_bytes1(float* f) {
		unsigned long u = FX_SWAP32INT(*reinterpret_cast<int*>(f));
		*f = *reinterpret_cast<float*>(&u);
	}

	inline void reverse_bytes3(float* f) {
		reverse_bytes1(f);
		reverse_bytes1(f + 1);
		reverse_bytes1(f + 2);
	}

	inline bool same_byte_order(int byte_order) {
		union {
			unsigned i;
			char c[4];
		} endianness = { 0x01020304 };
		return
			((endianness.c[0] == 1) && (byte_order > 0)) ||
			((endianness.c[0] == 4) && (byte_order < 0));
	}

	inline void reverse_donothing(float*) {}

	template <typename ColorType>
	bool TImage<ColorType>::loadPFM(const std::string& path) {
		std::ifstream fin(path, std::ios::binary);
		if (!fin) return false;
		std::string input;
		int newline_count = 0;
		while (newline_count < 3) {
			char c = 0;
			fin.read(&c, 1);
			if (c == 0x0a) newline_count++;
			input.push_back(c);
			if (!fin) {
				return false;
			}
		}
		std::istringstream istr(input);
		std::string type;
		int xres = 0;
		int yres = 0;
		double byte_order = 0;
		int stride = 0;
		istr >> type >> xres >> yres >> byte_order;
		if (type == "PF") stride = 12;
		else if (type == "Pf") stride = 4;
		else return false;
		const int count = (stride == 12 ? 3 : 1);
		resize(xres, yres, 1);

		auto rb = same_byte_order((int)byte_order) ? reverse_donothing :
			stride == 4 ? reverse_bytes1 : reverse_bytes3;

		constexpr float to_scalar_type = color_from_float_factor<scalar_type>();
		float v[3]{ 0.0f, 0.0f, 0.0f };
		for (auto& p : pixels) {
			fin.read((char*)v, stride);
			rb(v);
			p.r = (scalar_type)(v[0] * to_scalar_type);
			p.g = (scalar_type)(v[1] * to_scalar_type);
			p.b = (scalar_type)(v[2] * to_scalar_type);
		}

		//if (!same_byte_order((int)byte_order)) {
		//	if (stride == 4) {
		//		for (auto& p : pixels) {
		//			reverse_bytes1((float*)&p.r);
		//		}
		//	}
		//	else {
		//		for (auto& p : pixels) {
		//			reverse_bytes3((float*)&p.r);
		//		}
		//	}
		//}

		return true;
	}

	template <typename ColorType>
	bool TImage<ColorType>::savePFM(const std::string& path) const {
		std::ofstream fout(path, std::ios::binary);
		if (!fout) return false;
		std::ostringstream ostr;
		ostr << "PF" << char(0xa);
		ostr << imageWidth << char(0x20);
		ostr << imageHeight << char(0xa);
		ostr << (same_byte_order(-1) ? -1.0 : 1.0) << char(0xa);
		const float tofloat = color_to_float_factor<scalar_type>();
		for (const auto& p : pixels) {
			Color3f output{ p.r * tofloat, p.g * tofloat, p.b * tofloat };
			fout.write((char*)&output.r, 12);
		}
		return true;
	}

	template <typename ColorType>
	bool TImage<ColorType>::loadEXR(const std::string& path) {
#ifdef FLUXIONS_GTE_USEOPENEXR
		double t1 = HFLOG_MS_ELAPSED();
		Imf::RgbaInputFile file(path.c_str());
		Imath::Box2i dw = file.dataWindow();
		unsigned w = dw.max.x - dw.min.x + 1;
		unsigned h = dw.max.y - dw.min.y + 1;
		//Imf::Array2D<Imf::Rgba> filePixels;
		std::vector<Imf::Rgba> filePixels((unsigned)w * h);
		//filePixels.resizeErase(h, range);
		file.setFrameBuffer(&filePixels[0], 1, w);
		file.readPixels(dw.min.y, dw.max.y);

		resize(w, h);
		unsigned addr = 0;
		float minC = 1e6;
		float maxC = -1e6;
		for (unsigned j = 0; j < h; j++) {
			for (unsigned i = 0; i < w; i++) {
				Imf::Rgba rgba = filePixels[addr];
				Color3f color(float(rgba.r), float(rgba.g), float(rgba.b));
				minC = std::min(minC, color.minrgb());
				maxC = std::max(maxC, color.maxrgb());
				setPixel(i, j, color);
				addr++;
			}
		}
		t1 = HFLOG_MS_ELAPSED() - t1;
		HFLOGINFO("TImage<>::loadEXR", "Read %dx%d image from %s (%3f ms) (min, max) = (%-2.3f, %-2.3f)", w, h, path.c_str(), t1, minC, maxC);
		return true;
#else
		return false;
#endif // FLUXIONS_GTE_USEOPENEXR
	}

	template <typename ColorType>
	bool TImage<ColorType>::saveEXR(const std::string& path) const {
#ifdef FLUXIONS_GTE_USEOPENEXR
		double t1 = HFLOG_MS_ELAPSED();
		const Imf::Rgba black(0.0f, 0.0f, 0.0f, 1.0f);
		std::vector<Imf::Rgba> halfPixels((unsigned)imageWidth * imageHeight * imageDepth, black);
		const unsigned count = imageWidth * imageHeight * imageDepth;
		for (unsigned i = 0; i < count; i++) {
			constexpr float to_float = color_to_float_factor<scalar_type>();
			//Color3f color = ToColor3f(pixels[i]);
			Color3f color(pixels[i][0] * to_float,
						  pixels[i][1] * to_float,
						  pixels[i][2] * to_float);
			halfPixels[i] = Imf::Rgba(color.r, color.g, color.b);
		}
		Imf::RgbaOutputFile file(path.c_str(), (int)imageWidth, (int)imageHeight, Imf::WRITE_RGBA);
		file.setFrameBuffer(halfPixels.data(), 1, imageWidth);
		file.writePixels((int)imageHeight);
		t1 = HFLOG_MS_ELAPSED() - t1;
		HFLOGINFO("TImage<>::saveEXR", "Wrote %dx%d image to %s (%3f ms)", imageWidth, imageHeight, path.c_str(), t1);
		return true;
#else
		return false;
#endif // FLUXIONS_GTE_USEOPENEXR
	}

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
		ystride = imageWidth;
	}

	template <typename ColorType>
	void TImage<ColorType>::clear(const ColorType& clearcolor) {
		for (ColorType& c : pixels) {
			c = clearcolor;
		}
	}

	constexpr unsigned getComponentCount(unsigned format) {
		constexpr unsigned glconstant_RGB = 0x1907;
		constexpr unsigned glconstant_RGBA = 0x1908;

		return (format == glconstant_RGB ? 3 :
				format == glconstant_RGBA ? 4 :
				format);
	}

	template <typename ColorType>
	void TImage<ColorType>::setImageData(unsigned int format, unsigned int type, unsigned width, unsigned height, unsigned depth, void* _pixels) {
		_setImageData(getComponentCount(format), type, ColorType::gl_size, ColorType::gl_type, width, height, depth, _pixels);
	}

	template <typename ColorType>
	void TImage<ColorType>::_setImageData(unsigned int fromFormat, unsigned int fromType, unsigned int toFormat, unsigned int toType, unsigned width, unsigned height, unsigned depth, void* _pixels) {
		constexpr float scaleFactor_itof = 1.0f / 255.99f;
		constexpr float scaleFactor_ftoi = 255.99f;

		// constexpr unsigned glconstant_RGB = 0x1907;
		// constexpr unsigned glconstant_RGBA = 0x1908;
		constexpr unsigned glconstant_FLOAT = 0x1406;
		constexpr unsigned glconstant_UNSIGNED_BYTE = 0x1401;

		const unsigned fromstride = getComponentCount(fromFormat);
		const unsigned tostride = getComponentCount(toFormat);

		// handle case where reading RGBA image to RGB
		const unsigned components = std::min(fromstride, tostride);

		using PixelValueType = typename ColorType::value_type;

		resize(width, height, depth);
		unsigned count = width * height * depth;
		if (fromType == glconstant_UNSIGNED_BYTE && toType == glconstant_FLOAT) {
			unsigned char* data = (unsigned char*)_pixels;
			for (unsigned i = 0; i < count; i++) {
				PixelValueType* v = pixels[i].ptr();
				for (unsigned j = 0; j < components; j++) {
					v[j] = (PixelValueType)(scaleFactor_itof * data[j]);
				}
				data += fromstride;
			}
		}
		else if (fromType == glconstant_FLOAT && toType == glconstant_UNSIGNED_BYTE) {
			float* data = (float*)_pixels;
			for (unsigned i = 0; i < count; i++) {
				PixelValueType* v = pixels[i].ptr();
				for (unsigned j = 0; j < components; j++) {
					v[j] = (PixelValueType)clamp((int)(scaleFactor_ftoi * data[j]), 0, 255);
				}
				data += fromstride;
			}
		}
		else if (fromType == glconstant_UNSIGNED_BYTE && toType == glconstant_UNSIGNED_BYTE) {
			unsigned char* data = (unsigned char*)_pixels;
			for (unsigned i = 0; i < count; i++) {
				PixelValueType* v = pixels[i].ptr();
				for (unsigned j = 0; j < components; j++) {
					v[j] = (PixelValueType)data[j];
				}
				data += fromstride;
			}
		}
		else if (fromType == glconstant_FLOAT && toType == glconstant_FLOAT) {
			float* data = (float*)_pixels;
			for (unsigned i = 0; i < count; i++) {
				PixelValueType* v = pixels[i].ptr();
				for (unsigned j = 0; j < fromstride; j++) {
					v[j] = (PixelValueType)data[j];
				}
				data += fromstride;
			}
		}
	}

	// Square image operations
	template <typename ColorType>
	bool TImage<ColorType>::flipX(int z) {
		if (empty())
			return false;

		std::vector<ColorType> tmp((size_t)imageWidth * imageHeight);
		unsigned _zstride = imageWidth * imageHeight;
		unsigned zoffset = _zstride * z;
		for (unsigned x = 0; x < imageWidth; x++) {
			for (unsigned y = 0; y < imageHeight; y++) {
				unsigned sx = imageWidth - x - 1;
				unsigned sy = y;
				unsigned dst_offset = y * imageWidth + x;
				unsigned src_offset = sy * imageWidth + sx;
				tmp[dst_offset] = pixels[(size_t)zoffset + src_offset];
			}
		}
		copy(tmp.begin(), tmp.end(), pixels.begin() + zoffset);

		return true;
	}

	template <typename ColorType>
	bool TImage<ColorType>::flipY(int z) {
		if (empty())
			return false;

		std::vector<ColorType> tmp((size_t)imageWidth * imageHeight);
		unsigned _zstride = imageWidth * imageHeight;
		unsigned zoffset = _zstride * z;
		for (unsigned x = 0; x < imageWidth; x++) {
			for (unsigned y = 0; y < imageHeight; y++) {
				unsigned sx = x;
				unsigned sy = imageHeight - y - 1;
				unsigned dst_offset = y * imageWidth + x;
				unsigned src_offset = sy * imageWidth + sx;
				tmp[dst_offset] = pixels[(size_t)zoffset + src_offset];
			}
		}
		copy(tmp.begin(), tmp.end(), pixels.begin() + zoffset);

		return true;
	}

	template <typename ColorType>
	bool TImage<ColorType>::flipXY(int z) {
		if (empty())
			return false;

		std::vector<ColorType> tmp((size_t)imageWidth * imageHeight);
		unsigned _zstride = imageWidth * imageHeight;
		unsigned zoffset = _zstride * z;
		for (unsigned x = 0; x < imageWidth; x++) {
			for (unsigned y = 0; y < imageHeight; y++) {
				unsigned sx = imageWidth - x - 1;
				unsigned sy = imageHeight - y - 1;
				unsigned dst_offset = y * imageWidth + x;
				unsigned src_offset = sy * imageWidth + sx;
				tmp[dst_offset] = pixels[(size_t)zoffset + src_offset];
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

		std::vector<ColorType> tmp((size_t)imageWidth * imageHeight);
		unsigned size = imageWidth;
		//unsigned zstride = imageWidth * imageHeight;
		unsigned zoffset = zstride * z;
		for (unsigned x = 0; x < size; x++) {
			for (unsigned y = 0; y < size; y++) {
				int sx = (int)y;
				int sy = (int)(size - x - 1);
				unsigned dst_offset = y * size + x;
				unsigned src_offset = sy * size + sx;
				tmp[dst_offset] = pixels[(size_t)zoffset + src_offset];
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
	void TImage<ColorType>::blit2D(int sx, int sy, int sz,
								   int width, int height,
								   int dx, int dy, int dz,
								   TImage<ColorType>& dst) const {
		blit3D(sx, sy, sz, width, height, 1, dx, dy, dz, dst);
	}

	// determines if incoming range is clipped out of image
	inline bool range_clipped(int x, int w, unsigned rangeWidth) {
		return (x + w <= 0 || x >= (int)rangeWidth) ? true : false;
	}

	inline bool range_clipped(Vector3i x, Vector3i w, Vector3ui rangeWidth) {
		return ((x + w <= Vector3i(0)).l1dist() || (x >= rangeWidth).l1dist()) ? true : false;
	}

	inline void clamp_range(int& x1, int& x2, unsigned rangeWidth) {
		x1 = clamp<int>(x1, 0, (int)rangeWidth - 1);
		x2 = clamp<int>(x2, 0, (int)rangeWidth - 1);
	}

	inline void clamp_range(Vector3i& x1, Vector3i& x2, Vector3ui rangeWidth) {
		clamp_range(x1.x, x2.x, rangeWidth.x);
		clamp_range(x1.y, x2.y, rangeWidth.y);
		clamp_range(x1.z, x2.z, rangeWidth.z);
	}

	inline Vector3i tight_bounds(Vector3ui& range1, Vector3ui& range2) {
		return Vector3i(std::min(range1.x, range2.x),
						std::min(range1.y, range2.y),
						std::min(range1.z, range2.z));
	}

	inline bool range_empty(const Vector3ui& range) {
		return (range.x == 0 || range.y == 0 || range.z == 0);
	}

	template <typename ColorType>
	void TImage<ColorType>::blit3D(int sx, int sy, int sz,
								   int width, int height, int depth,
								   int dx, int dy, int dz,
								   TImage<ColorType>& dst) const {
		//HFLOGDEBUG("Copying (%i, %i, %i) cube from (%i, %i, %i)/(%i, %i, %i) to (%i, %i, %i)/(%i, %i, %i)",
		//		   width, height, depth,
		//		   sx, sy, sz,
		//		   imageWidth, imageHeight, imageDepth,
		//		   dx, dy, dz,
		//		   dst.imageWidth, dst.imageHeight, dst.imageDepth
		//);

		// Look for out of bounds
		Vector3ui range{
			(unsigned)std::max(0, width),
			(unsigned)std::max(0, height),
			(unsigned)std::max(0, depth) };
		Vector3ui srange{ imageWidth, imageHeight, imageDepth };
		Vector3ui drange{ dst.imageWidth, dst.imageHeight, dst.imageDepth };
		if (range_empty(srange) || range_empty(drange) || range_empty(range))
			return;

		// Clip source coordinates
		Vector3i sx1{ sx, sy, sz };
		if (range_clipped(sx1, range, srange))
			return;
		Vector3i sx2 = sx1 + range - 1;
		clamp_range(sx1, sx2, srange);

		// Clip target coordinates
		Vector3i dx1{ dx, dy, dz };
		if (range_clipped(dx1, range, drange))
			return;
		Vector3i dx2 = dx1 + range - 1;
		clamp_range(dx1, dx2, drange);

		// Determine smallest range and return if any are zero
		range = tight_bounds(srange, range);
		range = tight_bounds(drange, range);
		if (range_empty(range)) return;

		//HFLOGDEBUG("Actual->(%i, %i, %i) cube from (%i, %i, %i) to (%i, %i, %i)",
		//		   range.x, range.y, range.z,
		//		   sx1.x, sx1.y, sx1.z,
		//		   dx1.x, dx1.y, dx1.z
		//);

		size_t szdiff = (size_t)imageWidth * imageHeight;
		size_t dzdiff = (size_t)dst.imageWidth * dst.imageHeight;
		size_t sydiff = imageWidth;
		size_t dydiff = dst.imageWidth;
		size_t szoffset = sx1.z * szdiff;
		size_t dzoffset = dx1.z * dzdiff;
		size_t syoffset = sx1.y * sydiff;
		size_t dyoffset = dx1.y * dydiff;
		size_t row_size = sizeof(value_type) * range.x;

		unsigned zcount = range.z;
		while (zcount-- > 0) {
			unsigned ycount = range.y;
			while (ycount-- > 0) {
				const void* srcmem = &pixels[szoffset + syoffset + sx1.x];
				void* dstmem = &dst.pixels[dzoffset + dyoffset + dx1.x];
				memcpy(dstmem, srcmem, row_size);
				syoffset += sydiff;
				dyoffset += dydiff;
			}
			szoffset += szdiff;
			dzoffset += dzdiff;
		}
	}


	template <typename ColorType>
	bool TImage<ColorType>::convertRectToCubeMap() {
		// savePPMi("blah.ppm", 255.99f, 0, 255, 0);
		TImage<ColorType> tmp = *this;
		return tmp.convertRectToCubeMap(*this);

		//if (empty() || imageWidth != 6 * imageHeight)
		//	return false;

		//int swizzle[6] = {
		//	4, // POSITIVE Z
		//	5, // NEGATIVE Z
		//	2, // POSITIVE Y
		//	3, // NEGATIVE Y
		//	1, // POSITIVE X
		//	0, // NEGATIVE X
		//};
		//int size = (int)imageHeight;
		//std::vector<ColorType> src = pixels;
		//resize(size, size, 6);

		//for (unsigned i = 0; i < 6; i++) {
		//	unsigned k = swizzle[i];
		//	// demultiplex the data
		//	unsigned dst_offset = i * zstride;
		//	unsigned src_offset = k * size;
		//	for (int y = 0; y < size; y++) {
		//		void* pdst = &pixels[dst_offset];
		//		void* psrc = &src[src_offset];
		//		unsigned n = size * sizeof(ColorType);
		//		memcpy(pdst, psrc, n);
		//		dst_offset += size;
		//		src_offset += 6 * size;
		//	}
		//}

		//rotateRight90(3);
		//rotateLeft90(2);

		//return true;
	}

	template <typename ColorType>
	bool TImage<ColorType>::convertRectToCubeMap(TImage<ColorType>& dst) const {
		if (empty() || imageWidth != 6 * imageHeight)
			return false;

		int corona_swizzle[6] = {
			4, // POSITIVE Z
			5, // NEGATIVE Z
			2, // POSITIVE Y
			3, // NEGATIVE Y
			1, // POSITIVE X
			0, // NEGATIVE X
		};
		int normal_swizzle[6] = {
			0, // POSITIVE X
			1, // NEGATIVE X
			2, // POSITIVE Y
			3, // NEGATIVE Y
			4, // POSITIVE Z
			5, // NEGATIVE Z
		};
		constexpr bool use_corona_swizzle = false;
		int* swizzle = use_corona_swizzle ? corona_swizzle : normal_swizzle;

		int size = (int)imageHeight;
		dst.resize(size, size, 6);
		if (use_corona_swizzle) {			
			const std::vector<ColorType>& src = pixels;			

			unsigned zdiff = imageWidth * imageHeight;
			for (int i = 0; i < 6; i++) {
				int k = swizzle[i];
				// demultiplex the data
				unsigned dst_offset = i * zdiff;
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
		}
		else {
			int h[6] = { 0,1,2,3,4,5 };
			int v[6] = { 0,0,0,0,0,0 };
			for (int i = 0; i < 6; i++) {
				blit2D(h[i] * size, v[i] * size, 0, size, size, 0, 0, i, dst);
			}
		}

		if (use_corona_swizzle) {
			dst.rotateRight90(3);
			dst.rotateLeft90(2);
		}

		return true;
	}

	template <typename ColorType>
	bool TImage<ColorType>::convertCubeMapToRect() {
		TImage<ColorType> tmp(*this);
		return tmp.convertCubeMapToRect(*this);
	}

	template <typename ColorType>
	bool TImage<ColorType>::convertCubeMapToRect(TImage<ColorType>& dst) const {
		if (empty() || ((imageWidth != imageHeight) && (imageDepth != 6)))
			return false;

		constexpr bool use_corona_swizzle = false;
		if (use_corona_swizzle) {
			//rotateLeft90(2);
			//rotateRight90(3);
		}

		unsigned size = imageWidth;
		std::vector<ColorType> tmp = pixels;
		dst.resize(size * 6, size, 1);

		int corona_swizzle[6] = {
			4, // POSITIVE Z
			5, // NEGATIVE Z
			2, // POSITIVE Y
			3, // NEGATIVE Y
			1, // POSITIVE X
			0, // NEGATIVE X
		};
		int normal_swizzle[6] = {
			0, // POSITIVE X
			1, // NEGATIVE X
			2, // POSITIVE Y
			3, // NEGATIVE Y
			4, // POSITIVE Z
			5, // NEGATIVE Z
		};
		int* swizzle = use_corona_swizzle ? corona_swizzle : normal_swizzle;

		if (use_corona_swizzle) {
			for (unsigned i = 0; i < 6; i++) {
			unsigned k = swizzle[i];
			// demultiplex the data
			unsigned src_offset = i * size * size;
			unsigned dst_offset = k * size;
			for (unsigned y = 0; y < size; y++) {
				void* pdst = &dst.pixels[dst_offset];
				const void* psrc = &pixels[src_offset];
				unsigned n = size * sizeof(ColorType);
				memcpy(pdst, psrc, n);
				src_offset += size;
				dst_offset += 6 * size;
			}
		}
		}
		else {
			int h[6] = { 0,1,2,3,4,5 };
			int v[6] = { 0,0,0,0,0,0 };
			for (int i = 0; i < 6; i++) {
				blit2D(0, 0, i, size, size, h[i] * size, v[i] * size, 0, dst);
			}
		}

		return true;
		//if (empty() || ((imageWidth != imageHeight) && (imageDepth != 6)))
		//	return false;

		//TImage<ColorType> src(*this);
		//src.rotateLeft90(2);
		//src.rotateRight90(3);

		//int size = (int)imageWidth;
		//dst.resize(size * 6, size, 1);

		//int swizzle[6] = {
		//	4, // POSITIVE Z
		//	5, // NEGATIVE Z
		//	2, // POSITIVE Y
		//	3, // NEGATIVE Y
		//	1, // POSITIVE X
		//	0, // NEGATIVE X
		//};

		//for (int i = 0; i < 6; i++) {
		//	int k = swizzle[i];
		//	// demultiplex the data
		//	unsigned src_offset = i * size * size;
		//	unsigned dst_offset = k * size;
		//	for (int y = 0; y < size; y++) {
		//		auto srcit = src.pixels.cbegin() + src_offset;
		//		auto dstit = dst.pixels.begin() + dst_offset;
		//		std::copy_n(srcit, size, dstit);
		//		src_offset += size;
		//		dst_offset += 6 * size;
		//	}
		//}
		//src.resize(1, 1, 1);

		//return true;
	}

	template <typename ColorType>
	bool TImage<ColorType>::convertCrossToCubeMap() {
		TImage<ColorType> tmp = *this;
		return tmp.convertCrossToCubeMap(*this);
	}


	template <typename ColorType>
	bool TImage<ColorType>::convertCubeMapToCross(bool horizontal) {
		TImage<ColorType> tmp = *this;
		return tmp.convertCubeMapToCross(*this, horizontal);
		return true;
	}


	template <typename ColorType>
	bool TImage<ColorType>::convertCrossToCubeMap(TImage<ColorType>& dst) const {
		// check for cross orientation
		// HORIZONTAL is 4x3 aspect ratio
		bool horizontal = (imageWidth * 3 == imageHeight * 4 && imageDepth == 1);
		// VERTICAL is 3x4 aspect ratio
		bool vertical = (imageWidth * 4 == imageHeight * 3 && imageDepth == 1);

		if (!horizontal && !vertical) return false;

		int size = (int)(vertical ? (imageHeight >> 2) : (imageWidth >> 2));
		dst.resize(size, size, 6);

		int hh[6] = { 2,0,1,1,1,3 };
		int hv[6] = { 1,1,0,2,1,1 };
		int vh[6] = { 2,0,1,1,1,1 };
		int vv[6] = { 1,1,0,2,1,3 };

		int* h = horizontal ? hh : vh;
		int* v = horizontal ? hv : vv;

		for (int i = 0; i < 6; i++) {
			blit2D(h[i] * size, v[i] * size, 0, size, size, 0, 0, i, dst);
		}

		if (!horizontal) dst.flipXY(5);

		return true;
	}

	template <typename ColorType>
	bool TImage<ColorType>::convertCubeMapToCross(TImage<ColorType>& dst, bool horizontal) const {
		if (empty() || ((imageWidth != imageHeight) && (imageDepth != 6)))
			return false;

		TImage<ColorType> src(*this);
		//src.rotateLeft90(2);
		//src.rotateRight90(3);

		int size = (int)imageWidth;
		if (horizontal)
			dst.resize(size * 4, size * 3, 1);
		else
			dst.resize(size * 3, size * 4, 1);

		//int hh[6] = { 2,0,3,1,1,1 };
		//int hv[6] = { 1,1,1,1,2,0 };
		int hh[6] = { 2,0,1,1,1,3 };
		int hv[6] = { 1,1,0,2,1,1 };
		//int vh[6] = { 2,0,1,1,1,1 };
		//int vv[6] = { 1,1,3,1,2,0 };
		int vh[6] = { 2,0,1,1,1,1 };
		int vv[6] = { 1,1,0,2,1,3 };

		int* h = horizontal ? hh : vh;
		int* v = horizontal ? hv : vv;

		if (!horizontal) src.flipXY(5);
		for (int i = 0; i < 6; i++) {
			src.blit2D(0, 0, i, size, size, h[i] * size, v[i] * size, 0, dst);
		}

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
	bool TestTImage(const char* testbase) noexcept {
		constexpr int width = 8;
		constexpr int height = 8;
		using value_type = typename ColorType::value_type;
		value_type black = 0;
		value_type white = color_max_value<value_type>();
		value_type grey1 = (black + white) / 4;
		value_type grey2 = (black + white) / 2;
		value_type grey3 = grey2 + grey1;

#define COLORTYPENAME(X) #X
		using std::cerr;
		cerr << testbase << "  ";
		cerr << "black " << (double)black;
		cerr << "|<-- " << (double)grey1;
		cerr << "-->| " << (double)white << "\n";

		{	// Write rectangular image
			ColorType blackPixel{ grey1, grey2, grey3 };
			ColorType whitePixel{ grey3, grey1, grey2 };
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

	void TestImage() noexcept {
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
