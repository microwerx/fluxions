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
#define FLUXIONS_NO_EXTERN_TEMPLATES
#include "pch.h"
#include <fluxions_gte_math.hpp>
#include <random>

namespace Fluxions
{
	float randomSampler(float _min0, float _max0)
	{
		static std::mt19937 s;
		std::uniform_real_distribution<float> urd(_min0, _max0);

		return urd(s);
	}
} // namespace Fluxions
