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

#ifndef FLUXIONS_GTE_IMAGE_OPERATIONS_HPP
#define FLUXIONS_GTE_IMAGE_OPERATIONS_HPP

#include <fluxions_gte_image.hpp>

namespace Fluxions
{
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