// SSPHH/Fluxions/Unicornfish/Viperfish/Hatchetfish/Sunfish/Damselfish/GLUT Extensions
// Copyright (C) 2017-2020 Jonathan Metzgar
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
#ifndef FLUXIONS_STDCXX_HPP
#define FLUXIONS_STDCXX_HPP

// C++ Libraries

#include <ostream>
#include <istream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <string>
#include <memory>
#include <vector>
#include <map>
#include <functional>
#include <regex>
#include <random>
#include <future>
#include <cctype>

// C Libraries

#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h> // for free()

#ifdef __unix__
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <libgen.h> // for dirname() and basename()
#include <unistd.h>
#endif

namespace Fluxions
{
	inline std::string& tolower(std::string& str) {
		for_each(str.begin(), str.end(),
				 [](std::string::value_type c) {
					 return std::tolower(c);
				 });
		return str;
	}

	inline std::string& toupper(std::string& str) {
		for_each(str.begin(), str.end(),
				 [](std::string::value_type c) {
					 return std::toupper(c);
				 });
		return str;
	}
}

#endif
