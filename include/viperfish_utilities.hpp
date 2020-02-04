#ifndef VIPERFISH_UTILITIES_HPP
#define VIPERFISH_UTILITIES_HPP

#include <viperfish_base.hpp>

namespace Vf {

	// Time stuff
	inline double SteadyClockNow() noexcept {
		std::chrono::microseconds t = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now().time_since_epoch());
		return t.count() * 1e-6;
	}

	inline time_t SystemClockNow() noexcept {
		std::chrono::time_point<std::chrono::system_clock> tp = std::chrono::system_clock::now();
		return std::chrono::system_clock::to_time_t(tp);
	}
} // namespace Vf

#endif
