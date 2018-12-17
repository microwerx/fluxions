// SSPHH/Fluxions/Unicornfish/Viperfish/Hatchetfish/Sunfish/KASL/GLUT Extensions
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
#include "stdafx.h"
#include <algorithm>
#include <fluxions_gte_image.hpp>
#include <fstream>

namespace Fluxions
{
using namespace std;

// C++ Specializations Must Come First

Color3ub RGBFloatToUint8(const Color3f &c)
{
    return Color3ub(
        (unsigned char)clamp((int)(c.r * 255.0f), 0, 255),
        (unsigned char)clamp((int)(c.g * 255.0f), 0, 255),
        (unsigned char)clamp((int)(c.b * 255.0f), 0, 255));
}

Color3i RGBFloatToInt32(const Color3f &c)
{
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
TImage<ColorType> &TImage<ColorType>::ToSRGB()
{
    for (ColorType &c : pixels)
    {
        Color3f color_linear = ToColor3f(c);
        Color3f color_srgb = RGBtoSRGB(color_linear);
        FromColor3f(c, color_srgb.clamp());
    }

    return *this;
}

template <typename ColorType>
TImage<ColorType> &TImage<ColorType>::ReverseSRGB()
{
    for (ColorType &c : pixels)
    {
        Color3f color_srgb = ToColor3f(c);
        Color3f color_linear = SRGBtoRGB(color_srgb);
        FromColor3f(c, color_linear.clamp());
    }

    return *this;
}

template <typename ColorType>
TImage<ColorType> &TImage<ColorType>::ReverseToneMap(float exposure)
{
    float tm = 1.0f / (2.5f * powf(2.0f, exposure));
    for (ColorType &c : pixels)
    {
        Color3f color = ToColor3f(c);
        color *= tm;
        FromColor3f(c, color.clamp());
    }

    return *this;
}

template <typename ColorType>
TImage<ColorType> &TImage<ColorType>::ToneMap(float exposure)
{
    float tm = 2.5f * powf(2.0f, exposure);
    for (ColorType &c : pixels)
    {
        Color3f color = ToColor3f(c);
        color *= tm;
        FromColor3f(c, color.clamp());
    }

    return *this;
}

template <typename ColorType>
void TImage<ColorType>::scaleColors(const float x)
{
    for (ColorType &c : pixels)
    {
        c *= (typename ColorType::type)x;
    }
}

template <typename ColorType>
TImage<ColorType> TImage<ColorType>::ScaleImage(size_t newWidth, size_t newHeight, bool bilinear)
{
    TImage<ColorType> out;
    out.resize(newWidth, newHeight, imageDepth);
    size_t newDepth = imageDepth;
    Vector3d d(imageWidth / (double)newWidth, imageHeight / (double)newHeight, imageDepth / (double)newDepth);
    Vector3d src;
    for (size_t z = 0; z < newDepth; z++)
    {
        src.y = 0.0;
        for (size_t y = 0; y < newHeight; y++)
        {
            src.x = 0.0;
            for (size_t x = 0; x < newWidth; x++)
            {
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

template <typename ColorType>
void TImage<ColorType>::savePPMRaw(const std::string &filename, size_t z)
{
    const float scale = ColorType::to_float_factor * 255.0f;
    float maxColorFound = maxrgb() * scale;
    if (maxColorFound < 255.0f)
        maxColorFound = 255.0f;

    std::ofstream fout(filename.c_str());
    fout << "P3" << std::endl;
    fout << imageWidth << " ";
    fout << imageHeight << " ";
    fout << maxColorFound << std::endl;

    for (size_t y = 0; y < imageHeight; y++)
    {
        for (size_t x = 0; x < imageWidth; x++)
        {
            Color3f color = getPixel(x, y, z) * scale;
            int ir = (int)color.r;
            int ig = (int)color.g;
            int ib = (int)color.b;
            fout << ir << " " << ig << " " << ib << std::endl;
        }
    }

    fout.close();
}

template <typename ColorType>
void TImage<ColorType>::loadPPM(const std::string &filename)
{
    std::ifstream fin(filename.c_str());

    std::string magicNumber;
    size_t width;
    size_t height;
    int maxInt;

    fin >> magicNumber;
    while (!magicNumber.empty() && magicNumber[0] == '#')
    {
        // skip comments
        fin >> magicNumber;
    }
    if (magicNumber != "P3")
        return;
    fin >> width;
    fin >> height;
    fin >> maxInt;

    // decide what to do...
    // const unsigned int size = ColorType::gl_size;
    // const unsigned int type = ColorType::gl_type;
    const float scale = ColorType::from_float_factor / 255.99f;

    resize(width, height);
    for (size_t y = 0; y < imageHeight; y++)
    {
        for (size_t x = 0; x < imageWidth; x++)
        {
            Color3f color;
            fin >> color.r >> color.g >> color.b;
            color *= scale;
            ColorType dst;
            FromColor3f(dst, color);
            setPixel(x, y, dst);
        }
    }

    fin.close();
}

template <typename ColorType>
void TImage<ColorType>::savePPM(const std::string &filename, size_t z, bool flipy)
{
    const float scale = ColorType::to_float_factor * 255.99f;
    float maxColorFound = maxrgb() * scale;
    if (maxColorFound < 255.0f)
        maxColorFound = 255.0f;

    std::ofstream fout(filename.c_str());
    fout << "P3" << std::endl;
    fout << imageWidth << " ";
    fout << imageHeight << " ";
    fout << maxColorFound << std::endl;

    size_t y1 = 0;
    size_t y2 = imageHeight;
    int dy = 1;
    if (flipy)
    {
        y1 = imageHeight - 1;
        y2 = -1;
        dy = -1;
    }
    for (size_t y = y1; y != y2; y += dy)
    {
        for (size_t x = 0; x < imageWidth; x++)
        {
            ColorType c = getPixel(x, y, z);
            Color3i color(
                clamp((int)(c.r * scale), 0, 255),
                clamp((int)(c.g * scale), 0, 255),
                clamp((int)(c.b * scale), 0, 255));
            fout << color.r << " " << color.g << " " << color.b << std::endl;
        }
    }

    fout.close();
}

template <typename ColorType>
void TImage<ColorType>::savePPMi(const std::string &filename, float scale, int minValue, int maxValue, size_t z, bool flipy)
{
    if (maxValue <= 0)
    {
        maxValue = (int)(scale * maxrgb());
    }

    std::ofstream fout(filename.c_str());

    fout << "P3" << std::endl;
    fout << imageWidth << " ";
    fout << imageHeight << " ";
    fout << maxValue << std::endl;

    size_t y1 = 0;
    size_t y2 = imageHeight;
    int dy = 1;
    if (flipy)
    {
        y1 = imageHeight - 1;
        y2 = -1;
        dy = -1;
    }
    for (size_t y = y1; y != y2; y += dy)
    {
        for (size_t x = 0; x < imageWidth; x++)
        {
            Color4i color = ToColor4i(getPixel(x, y, z), scale, minValue, maxValue);
            fout << color.r << " " << color.g << " " << color.b << std::endl;
        }
    }

    fout.close();
}

template <typename ColorType>
void TImage<ColorType>::savePPMHDRI(const std::string &filename, size_t z)
{
    std::ofstream fout(filename.c_str());

    fout << "P3" << std::endl;
    fout << imageWidth << " ";
    fout << imageHeight << " ";
    fout << "1.0" << std::endl;

    for (size_t y = 0; y < imageHeight; y++)
    {
        for (size_t x = 0; x < imageWidth; x++)
        {
            ColorType c = getPixel(x, y, z);
            Color3f color = ToColor3f(c);
            fout << color.r << " " << color.g << " " << color.b << std::endl;
        }
    }

    fout.close();
}

template <typename ColorType>
void TImage<ColorType>::resize(size_t width, size_t height, size_t depth)
{
    imageWidth = width;
    imageHeight = height;
    imageDepth = depth;
    if (width * height * depth == 0)
        pixels.clear();
    else
        pixels.resize(width * height * depth);
    zstride = imageWidth * imageHeight;
}

template <typename ColorType>
void TImage<ColorType>::clear(const ColorType &clearcolor)
{
    for (ColorType &c : pixels)
    {
        c = clearcolor;
    }
}

template <typename ColorType>
void TImage<ColorType>::setImageData(unsigned int format, unsigned int type, size_t width, size_t height, size_t depth, void *_pixels)
{
    _setImageData(format, type, ColorType::gl_type, ColorType::gl_size, width, height, depth, _pixels);
}

template <typename ColorType>
void TImage<ColorType>::_setImageData(unsigned int fromFormat, unsigned int fromType, unsigned int toFormat, unsigned int toType, size_t width, size_t height, size_t depth, void *_pixels)
{
    float scaleFactor_itof = 1.0f / 255.99f;
    float scaleFactor_ftoi = 255.99f;

    const unsigned glconstant_RGB = 0x1907;
    const unsigned glconstant_RGBA = 0x1908;
    const unsigned glconstant_FLOAT = 0x1406;
    const unsigned glconstant_UNSIGNED_BYTE = 0x1401;

    size_t stride;
    if (fromFormat == 3 || fromFormat == glconstant_RGB)
        stride = 3;
    else if (fromFormat == 4 || fromFormat == glconstant_RGBA)
        stride = 4;
    else
        return;

    resize(width, height, depth);
    size_t count = width * height * depth;
    if (fromType == glconstant_UNSIGNED_BYTE && toType == glconstant_FLOAT)
    {
        unsigned char *data = (unsigned char *)_pixels;
        for (size_t i = 0; i < count; i++)
        {
            typename ColorType::type *v = pixels[i].ptr();
            for (size_t j = 0; j < stride; j++)
            {
                v[j] = (typename ColorType::type)clamp((int)(scaleFactor_itof * data[j]), 0, 255);
            }
            data += stride;
        }
    }
    else if (fromType == glconstant_FLOAT && toType == glconstant_UNSIGNED_BYTE)
    {
        float *data = (float *)_pixels;
        for (size_t i = 0; i < count; i++)
        {
            typename ColorType::type *v = pixels[i].ptr();
            for (size_t j = 0; j < stride; j++)
            {
                v[j] = (typename ColorType::type)clamp((int)(scaleFactor_ftoi * data[j]), 0, 255);
            }
            data += stride;
        }
    }
    else if (fromType == glconstant_UNSIGNED_BYTE && toType == glconstant_UNSIGNED_BYTE)
    {
        unsigned char *data = (unsigned char *)_pixels;
        for (size_t i = 0; i < count; i++)
        {
            typename ColorType::type *v = pixels[i].ptr();
            for (size_t j = 0; j < stride; j++)
            {
                v[j] = (typename ColorType::type)data[j];
            }
            data += stride;
        }
    }
    else if (fromType == glconstant_FLOAT && toType == glconstant_FLOAT)
    {
        float *data = (float *)_pixels;
        for (size_t i = 0; i < count; i++)
        {
            typename ColorType::type *v = pixels[i].ptr();
            for (size_t j = 0; j < stride; j++)
            {
                v[j] = (typename ColorType::type)data[j];
            }
            data += stride;
        }
    }
}

// Square image operations
template <typename ColorType>
bool TImage<ColorType>::flipX(int z)
{
    if (empty() || imageWidth != imageHeight)
        return false;

    std::vector<ColorType> tmp(imageWidth * imageHeight);
    size_t size = imageWidth;
    //size_t zstride = imageWidth * imageHeight;
    size_t zoffset = zstride * z;
    for (size_t x = 0; x < size; x++)
    {
        for (size_t y = 0; y < size; y++)
        {
            size_t sx = size - x - 1;
            size_t sy = y;
            size_t dst_offset = y * size + x;
            size_t src_offset = sy * size + sx;
            tmp[dst_offset] = pixels[zoffset + src_offset];
        }
    }
    copy(tmp.begin(), tmp.end(), pixels.begin() + zoffset);

    return true;
}

template <typename ColorType>
bool TImage<ColorType>::flipY(int z)
{
    if (empty() || imageWidth != imageHeight)
        return false;

    std::vector<ColorType> tmp(imageWidth * imageHeight);
    size_t size = imageWidth;
    //size_t zstride = imageWidth * imageHeight;
    size_t zoffset = zstride * z;
    for (size_t x = 0; x < size; x++)
    {
        for (size_t y = 0; y < size; y++)
        {
            size_t sx = x;
            size_t sy = size - y - 1;
            size_t dst_offset = y * size + x;
            size_t src_offset = sy * size + sx;
            tmp[dst_offset] = pixels[zoffset + src_offset];
        }
    }
    copy(tmp.begin(), tmp.end(), pixels.begin() + zoffset);

    return true;
}

template <typename ColorType>
bool TImage<ColorType>::rotateLeft90(int z)
{
    if (empty() || (imageWidth != imageHeight))
        return false;

    std::vector<ColorType> tmp(imageWidth * imageHeight);
    size_t size = imageWidth;
    //size_t zstride = imageWidth * imageHeight;
    size_t zoffset = zstride * z;
    for (size_t x = 0; x < size; x++)
    {
        for (size_t y = 0; y < size; y++)
        {
            int sx = (int)(size - y - 1);
            int sy = (int)x;
            size_t dst_offset = y * size + x;
            size_t src_offset = sy * size + sx;
            tmp[dst_offset] = pixels[zoffset + src_offset];
        }
    }
    //copy(tmp.begin(), tmp.end(), pixels.begin() + zoffset);
    void *pdst = &pixels[zoffset];
    void *psrc = &tmp[0];
    size_t n = zstride * sizeof(ColorType);
    memcpy(pdst, psrc, n);
    // memcpy(&pixels[zoffset], &tmp[0], zstride * sizeof(ColorType));

    return true;
}

template <typename ColorType>
bool TImage<ColorType>::rotateRight90(int z)
{
    if (empty() || imageWidth != imageHeight)
        return false;

    std::vector<ColorType> tmp(imageWidth * imageHeight);
    size_t size = imageWidth;
    //size_t zstride = imageWidth * imageHeight;
    size_t zoffset = zstride * z;
    for (size_t x = 0; x < size; x++)
    {
        for (size_t y = 0; y < size; y++)
        {
            int sx = (int)y;
            int sy = (int)(size - x - 1);
            size_t dst_offset = y * size + x;
            size_t src_offset = sy * size + sx;
            tmp[dst_offset] = pixels[zoffset + src_offset];
        }
    }

    void *dst = &pixels[zoffset];
    void *src = &tmp[0];
    size_t n = zstride * sizeof(ColorType);
    memcpy(dst, src, n);
    //copy(tmp.begin(), tmp.end(), pixels.begin() + zoffset);

    return true;
}

template <typename ColorType>
bool TImage<ColorType>::convertRectToCubeMap()
{
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
    int size = imageHeight;
    std::vector<ColorType> src = pixels;
    resize(size, size, 6);

    for (int i = 0; i < 6; i++)
    {
        int k = swizzle[i];
        // demultiplex the data
        size_t dst_offset = i * zstride;
        size_t src_offset = k * size;
        for (int y = 0; y < size; y++)
        {
            void *pdst = &pixels[dst_offset];
            void *psrc = &src[src_offset];
            size_t n = size * sizeof(ColorType);
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
bool TImage<ColorType>::convertCubeMapToRect()
{
    if (empty() || ((imageWidth != imageHeight) && (imageDepth != 6)))
        return false;

    rotateLeft90(2);
    rotateRight90(3);

    int size = imageWidth;
    std::vector<ColorType> tmp = pixels;
    resize(size * 6, size, 1);

    int swizzle[6] = {
        4, // POSITIVE Z
        5, // NEGATIVE Z
        2, // POSITIVE Y
        3, // NEGATIVE Y
        1, // POSITIVE X
        0, // NEGATIVE X
    };

    for (int i = 0; i < 6; i++)
    {
        int k = swizzle[i];
        // demultiplex the data
        size_t src_offset = i * size * size;
        size_t dst_offset = k * size;
        for (int y = 0; y < size; y++)
        {
            void *pdst = &pixels[dst_offset];
            void *psrc = &tmp[src_offset];
            size_t n = size * sizeof(ColorType);
            memcpy(pdst, psrc, n);
            src_offset += size;
            dst_offset += 6 * size;
        }
    }

    return true;
}

template <typename ColorType>
double TImage<ColorType>::getIntensity() const
{
    double I = 0.0;
    for (auto &c : pixels)
    {
        I += c.Intensity();
    }
    return I;
}

template <typename ColorType>
double TImage<ColorType>::getMinimum() const
{
    double I = 1e6;
    for (auto &c : pixels)
    {
        if (c.Intensity() < I)
            I = c.Intensity();
    }
    return I;
}

template <typename ColorType>
double TImage<ColorType>::getMaximum() const
{
    double I = -1e6;
    for (auto &c : pixels)
    {
        if (c.Intensity() > I)
            I = c.Intensity();
    }
    return I;
}

template <typename ColorType>
double TImage<ColorType>::getAverage() const
{
    double I = 0.0;
    for (auto &c : pixels)
    {
        I += c.Intensity();
    }
    I /= (double)getNumPixels();
    return I;
}

// EXPLICIT INSTANTIATION //////////////////////////////////////////////////////

template class TImage<Color3i>;
template class TImage<Color3f>;
template class TImage<Color3ub>;

template class TImage<Color4i>;
template class TImage<Color4f>;
template class TImage<Color4ub>;

} // namespace Fluxions
