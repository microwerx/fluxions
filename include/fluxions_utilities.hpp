#ifndef FLUXIONS_UTILITIES_HPP
#define FLUXIONS_UTILITIES_HPP

#include <fluxions_base.hpp>

namespace Fluxions {
	inline std::vector<std::string> argc_argv_to_vector_string(int argc, char** argv) {
		std::vector<std::string> args;
		for (int i = 0; i < argc; i++) {
			args.push_back(argv[i]);
		}
		return args;
	}
} // namespace Fluxions

#endif
