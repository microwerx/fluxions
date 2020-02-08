#ifndef FLUXIONS_IMAGE_LOADER_HPP
#define FLUXIONS_IMAGE_LOADER_HPP

#include <fluxions_gte_image.hpp>

namespace Fluxions {
	bool LoadImage3f(const std::string& path, Image3f& image);
	bool LoadImage4f(const std::string& path, Image4f& image);
}

#endif
