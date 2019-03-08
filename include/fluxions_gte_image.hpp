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
#ifndef FLUXIONS_GTE_IMAGE_HPP
#define FLUXIONS_GTE_IMAGE_HPP

// #include <algorithm>
#include <string>
#include <vector>
#include <memory.h>
#include <fluxions_gte_color3.hpp>
#include <fluxions_gte_color4.hpp>
#include <fluxions_gte_math.hpp>
#include <fluxions_gte_vector3.hpp>
#include <fluxions_gte_vector4.hpp>

namespace Fluxions
{
template <class T>
struct is_color_type : std::integral_constant<
                           bool,
                           std::is_same<TColor3<T>, T>::value || std::is_same<TColor4<T>, T>::value>
{
};

template <typename ColorType>
class TImage
{
  public:
    std::vector<ColorType> pixels;
    size_t imageWidth;
    size_t imageHeight;
    size_t imageDepth;

    TImage()
        : imageWidth(0), imageHeight(0), imageDepth(0)
    {
    }
    TImage(int width, int height, int depth = 1) { resize(width, height, depth); }

    constexpr void setBorderColor(const ColorType &color) { borderColor = color; }
    constexpr const ColorType &getBorderColor() const { return borderColor; }
    constexpr size_t width() const { return imageWidth; }
    constexpr size_t height() const { return imageHeight; }
    constexpr size_t depth() const { return imageDepth; }
    constexpr size_t addr(size_t x, size_t y, size_t z = 0)
    {
        if (x >= imageWidth || y >= imageHeight || z >= imageDepth)
            return 0;
        return z * zstride + y * imageWidth + x;
    }

    constexpr ColorType *getPixels(size_t index)
    {
        if (index >= imageDepth)
            return NULL;
        return &(pixels[index * zstride]);
    }

    void resize(size_t width, size_t height, size_t depth = 1);
    void reset() { resize(0, 0, 0); }
    void clear(const ColorType &clearcolor);
    bool empty() const noexcept { return pixels.empty(); }
    bool IsCubeMap() const noexcept { return imageWidth == imageHeight && imageDepth == 6; }
    TImage<ColorType> &ToSRGB();
    TImage<ColorType> &ReverseSRGB();
    TImage<ColorType> &ToneMap(float exposure);
    TImage<ColorType> &ReverseToneMap(float exposure);
    void scaleColors(float x);
    TImage<ColorType> ScaleImage(size_t newWidth, size_t newHeight, bool bilinear = false);

    constexpr void setPixel(size_t x, size_t y, ColorType color) noexcept
    {
        if (x >= imageWidth || y >= imageHeight)
            return;
        pixels[y * imageWidth + x] = color;
    }

    constexpr ColorType getPixel(size_t x, size_t y) const
    {
        if (x >= imageWidth || y >= imageHeight)
            return borderColor;

        return pixels[y * imageWidth + x];
    }

    // Same as setPixel(X, y, color) but without image bounds checking: unsafe!
    constexpr void setPixelUnsafe(size_t x, size_t y, ColorType color)
    {
        pixels[y * imageWidth + x] = color;
    }

    // Same as getPixel(X, y) but without image bounds checking: unsafe!
    constexpr ColorType getPixelUnsafe(size_t x, size_t y) const
    {
        return pixels[y * imageWidth + x];
    }

    constexpr void setPixel(size_t x, size_t y, size_t z, const ColorType &color)
    {
        if (x >= imageWidth || y >= imageHeight || z >= imageDepth)
            return;
        pixels[z * zstride + y * imageWidth + x] = color;
    }

    constexpr ColorType getPixel(size_t x, size_t y, size_t z) const
    {
        if (x >= imageWidth || y >= imageHeight || z >= imageDepth)
            return borderColor;

        return pixels[z * zstride + y * imageWidth + x];
    }

    constexpr typename ColorType::type maxrgb() const noexcept
    {
        typename ColorType::type maxColorFound = 0;
        for (auto &c : pixels)
        {
            maxColorFound = std::max(c.maxrgb(), maxColorFound);
        }
        return maxColorFound;
    }

    constexpr typename ColorType::type minrgb() const noexcept
    {
        typename ColorType::type minColorFound = pixels.empty() ? 0 : pixels[0].minrgb();
        for (auto &c : pixels)
        {
            minColorFound = std::min(c.minrgb(), minColorFound);
        }
        return minColorFound;
    }

    constexpr ColorType getPixelCubeMap(const Vector3f &v) const
    {
        return getPixelCubeMap(v.x, v.y, v.z);
    }

    constexpr ColorType getPixelCubeMap(float x, float y, float z) const
    {
        if (imageDepth != 6)
            return borderColor;

        float s, t;
        size_t iz;
        MakeFaceSTFromCubeVector(x, y, z, &s, &t, &iz);
        size_t ix = (int)(s * imageWidth);
        size_t iy = (int)((1.0f - t) * imageHeight);
        return getPixel(ix, iy, iz);
    }

    // Same as setPixel(X, y, color) but without image bounds checking: unsafe!
    constexpr void setPixelUnsafe(size_t x, size_t y, size_t z, const ColorType &color)
    {
        pixels[z * zstride + y * imageWidth + x] = color;
    }

    // Same as getPixel(X, y) but without image bounds checking: unsafe!
    constexpr ColorType getPixelUnsafe(size_t x, size_t y, size_t z) const
    {
        return pixels[z * zstride + y * imageWidth + x];
    }

    constexpr const void *getImageData(size_t z) const
    {
        return &pixels[z * zstride];
    }

    double getIntensity() const;
    double getMinimum() const;
    double getMaximum() const;
    double getAverage() const;
    int getNumPixels() const { return (int)pixels.size(); }

    void savePPMRaw(const std::string &filename, size_t z = 0);
    void savePPM(const std::string &filename, size_t z = 0, bool flipy = false);
    void savePPMi(const std::string &filename, float scale, int minValue, int maxValue, size_t z = 0, bool flipy = false);
    void savePPMHDRI(const std::string &filename, size_t z = 0);
    void loadPPM(const std::string &filename);
	void loadEXR(const std::string &path);
	void saveEXR(const std::string &path);

    bool flipX(int z = 0);
    bool flipY(int z = 0);
    bool rotateLeft90(int z = 0);
    bool rotateRight90(int z = 0);
    bool convertRectToCubeMap();
    bool convertCubeMapToRect();

    void setImageData(unsigned int format, unsigned int type, size_t width, size_t height, size_t depth, void *_pixels);

  private:
    void _setImageData(unsigned int fromFormat, unsigned int fromType, unsigned int toFormat, unsigned int toType, size_t width, size_t height, size_t depth, void *_pixels);

    size_t zstride;
    ColorType borderColor;
    int minColor;
    int maxColor;
};

using Image3f = TImage<Color3f>;
using Image3i = TImage<Color3i>;
using Image3ub = TImage<Color3ub>;

using Image4f = TImage<Color4f>;
using Image4i = TImage<Color4i>;
using Image4ub = TImage<Color4ub>;

#ifndef FLUXIONS_NO_EXTERN_TEMPLATES
extern template class TImage<Color3f>;
extern template class TImage<Color3i>;
extern template class TImage<Color3ub>;

extern template class TImage<Color4f>;
extern template class TImage<Color4i>;
extern template class TImage<Color4ub>;
#endif
} // namespace Fluxions

#endif
