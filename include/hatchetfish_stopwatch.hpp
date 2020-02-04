#ifndef HATCHETFISH_STOPWATCH_HPP
#define HATCHETFISH_STOPWATCH_HPP

#include <chrono>

#define HFCLOCKSf(timevariable, timeexpression) { Hf::StopWatch sw; timeexpression; timevariable = sw.Stop_sf(); }
#define HFCLOCKMSf(timevariable, timeexpression) { Hf::StopWatch sw; timeexpression; timevariable = sw.Stop_msf(); }
#define HFCLOCKSd(timevariable, timeexpression) { Hf::StopWatch sw; timeexpression; timevariable = sw.Stop_s(); }
#define HFCLOCKMSd(timevariable, timeexpression) { Hf::StopWatch sw; timeexpression; timevariable = sw.Stop_ms(); }

namespace Hf {
	class StopWatch {
	public:
		StopWatch();
		~StopWatch();

		void Start();
		void Stop();
		double Stop_s() { Stop(); return GetSecondsElapsed(); }
		double Stop_ms() { Stop(); return GetMillisecondsElapsed(); }
		float Stop_sf() { Stop(); return (float)GetSecondsElapsed(); }
		float Stop_msf() { Stop(); return (float)GetMillisecondsElapsed(); }
		double GetMillisecondsElapsed();
		double GetSecondsElapsed();

	private:
		std::chrono::time_point<std::chrono::system_clock> start_timepoint;
		std::chrono::time_point<std::chrono::system_clock> end_timepoint;
	};
}

#endif
