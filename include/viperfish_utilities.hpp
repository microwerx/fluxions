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
#ifndef VIPERFISH_UTILITIES_HPP
#define VIPERFISH_UTILITIES_HPP

#include <chrono>


#include <viperfish_stopwatch.hpp>


namespace Viperfish
{

	using namespace std;


	// Time stuff
	inline double SteadyClockNow() {
		using namespace std;
		chrono::microseconds t = chrono::duration_cast<chrono::microseconds>(chrono::steady_clock::now().time_since_epoch());
		return t.count() * 1e-6;
	}

	inline time_t SystemClockNow() {
		std::chrono::time_point<std::chrono::system_clock> tp = std::chrono::system_clock::now();
		return std::chrono::system_clock::to_time_t(tp);
	}
}

#endif
