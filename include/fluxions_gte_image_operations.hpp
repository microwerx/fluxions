#ifndef FLUXIONS_GTE_IMAGE_OPERATIONS_HPP
#define FLUXIONS_GTE_IMAGE_OPERATIONS_HPP

#include <fluxions_gte_image.hpp>

namespace Fluxions {
	template <typename ColorType>
	ColorType maxof(const TImage<ColorType>& image) noexcept {
		ColorType found = *image.cbegin();
		for (const auto& p : image) {
			found = std::max(found, maxof(p));
		}
		return found;
	}

	template <typename ColorType>
	ColorType minof(const TImage<ColorType>& image) noexcept {
		ColorType found = *image.cbegin();
		for (const auto& p : image) {
			found = std::min(found, minof(p));
		}
		return found;
	}
}

#endif
