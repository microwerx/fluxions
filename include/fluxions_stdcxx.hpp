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
#include <set>
#include <functional>
#include <regex>
#include <random>
#include <future>
#include <cctype>
#include <cfloat>

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

namespace Fluxions {
	inline std::string& tolower(std::string& str) {
		for (auto& c : str) {
			c = (char)std::tolower(c);
		}
		return str;
	}

	inline std::string tolower(const std::string& str) {
		std::string s = str;
		return tolower(s);
	}

	inline std::string& toupper(std::string& str) {
		for (auto& c : str) {
			c = (char)std::toupper(c);
		}
		return str;
	}

	inline std::string toupper(const std::string& str) {
		std::string s = str;
		return toupper(s);
	}

	inline std::string& toidentifier(std::string& str) {
		for (auto& c : str) {
			if (!isalnum(c)) c = '_';
		}
		return str;
	}

	inline std::string toidentifier(const std::string& str) {
		std::string s = str;
		return toidentifier(s);
	}

	inline std::string& toloweridentifier(std::string& str) {
		for (auto& c : str) {
			if (!isalnum(c)) c = '_';
			c = (char)std::tolower(c);
		}
		return str;
	}

	inline std::string toloweridentifier(const std::string& str) {
		std::string s = str;
		return toloweridentifier(s);
	}

	inline std::string& toupperidentifier(std::string& str) {
		for (auto& c : str) {
			if (!isalnum(c)) c = '_';
			c = (char)std::toupper(c);
		}
		return str;
	}

	inline std::string toupperidentifier(const std::string& str) {
		std::string s = str;
		return toupperidentifier(s);
	}

	using string_string = std::pair<std::string, std::string>;
	using string_vector = std::vector<std::string>;
	using string_string_vector = std::vector<string_string>;
	using string_string_map = std::map<std::string, std::string>;
	using byte_array = std::vector<uint8_t>;
}

#endif
