#include <hatchetfish_stopwatch.hpp>

namespace Hf {
	StopWatch::StopWatch() {
		start_timepoint = std::chrono::system_clock::now();
	}

	StopWatch::~StopWatch() {}

	void StopWatch::Start() {
		start_timepoint = std::chrono::system_clock::now();
	}

	void StopWatch::Stop() {
		end_timepoint = std::chrono::system_clock::now();
	}

	double StopWatch::GetSecondsElapsed() {
		auto diff = end_timepoint - start_timepoint;
		return std::chrono::duration<double>(diff).count();
	}

	double StopWatch::GetMillisecondsElapsed() {
		auto diff = end_timepoint - start_timepoint;
		return std::chrono::duration<double, std::milli>(diff).count();
	}
}
