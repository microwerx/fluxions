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
#ifndef HATCHETFISH_HPP
#define HATCHETFISH_HPP

#include <vector>
#include <tuple>
#include <map>
#include <string>
#include <chrono>
#include <cmath>

#define HFLOGINFO(...) Hf::Log.infofn(__FUNCTION__, __VA_ARGS__)
#define HFLOGWARN(...) Hf::Log.warningfn(__FUNCTION__, __VA_ARGS__)
#define HFLOGERROR(...) Hf::Log.errorfn(__FUNCTION__, __VA_ARGS__)
#define HFLOGDEBUG(...) Hf::Log.debugfn(__FUNCTION__, __VA_ARGS__)
#define HFLOG(...) Hf::Log.logfn(__FUNCTION__, __VA_ARGS)
#ifndef HFLOGDEBUGFIRSTRUN_COUNT
#define HFLOGDEBUGFIRSTRUN_COUNT 1
#endif
#define HFLOGDEBUGFIRSTRUN() { static unsigned firstRun = HFLOGDEBUGFIRSTRUN_COUNT; if (firstRun) { firstRun--; HFLOGDEBUG("run %i", HFLOGDEBUGFIRSTRUN_COUNT - firstRun); } }

namespace Hf
{
	class Hatchetfish
	{
	private:
		FILE* fout = stdout;
		std::string timeStamp;
		std::string dtg;
		std::string lastMessage;

		using SteadyClock = std::chrono::steady_clock;
		using TimePoint = std::chrono::time_point<SteadyClock>;
		using Duration = std::chrono::duration<double>;

		bool colorEnabled = true;
		bool logEnabled = true;
		bool logInfoEnabled = true;
		bool logWarningEnabled = true;
		bool logErrorEnabled = true;
		bool logDebugEnabled = true;

		const std::string& makeMessage(const char* category, const char* msg, va_list args);
		const std::string& makeMessagefn(const char* category, const char* fn, const char* msg, va_list args);

		mutable std::chrono::time_point<std::chrono::high_resolution_clock> t0;
		mutable std::chrono::time_point<std::chrono::high_resolution_clock> t1;

		std::vector<std::string> history;
		size_t maxHistoryLines = 10;

	public:
		struct TimeDataPoint
		{
			double timeMeasured = 0.0;
			double x = 0.0;
			double r = 0.0;
		};

		struct TimeDataPoints
		{
			std::vector<TimeDataPoint> X;
			double xbar;
			double rbar;
			double lcl;
			double ucl;

			void computeStats()
			{
				xbar = 0.0;
				rbar = 0.0;
				size_t i = 0;
				for (auto& datapoint : X)
				{
					if (i == 0)
						X[0].r = 0.0;
					xbar += datapoint.x;
					if (i > 0)
					{
						X[i].r = std::fabs(X[i].x - X[i - 1].x);
						rbar += datapoint.r;
					}
					i++;
				}
				double N = (double)X.size();
				xbar /= N;
				rbar /= N - 1.0;
				lcl = xbar - 2.66 * rbar;
				ucl = xbar + 2.66 * rbar;
			}
		};

		void print(const char* color);

	private:
		std::map<std::string, TimeDataPoints> stats;

	public:
		Hatchetfish();
		~Hatchetfish();

		void log(const char* category, const char* msg, ...);
		void logfn(const char* category, const char* fn, const char* msg, ...);
		void info(const char* msg, ...);
		void infofn(const char* fn, const char* msg, ...);
		void warning(const char* msg, ...);
		void warningfn(const char* fn, const char* msg, ...);
		void error(const char* msg, ...);
		void errorfn(const char* fn, const char* msg, ...);
		void debug(const char* msg, ...);
		void debugfn(const char* fn, const char* msg, ...);
		void flush();

		std::string makeTimeStamp();
		std::string makeDTG();
		const std::vector<std::string>& getHistory() const { return history; }
		void setMaxHistory(size_t lines) { maxHistoryLines = lines > 100 ? 100 : lines; }

		void setOutputFile(FILE* fileStream = NULL);

		void enableLog() { logEnabled = true; }
		void disableLog() { logEnabled = false; }
		void toggleLog() { logEnabled = !logEnabled; }
		bool isLogEnabled() { return logEnabled; }

		void enableColor() { colorEnabled = true; }
		void disableColor() { colorEnabled = false; }
		void toggleColor() { colorEnabled = !colorEnabled; }
		bool isColorEnabled() { return colorEnabled; }

		void enableInfoLog() { logInfoEnabled = true; }
		void disableInfoLog() { logInfoEnabled = false; }
		void toggleInfoLog() { logInfoEnabled = !logInfoEnabled; }
		bool isInfoLogEnabled() { return logInfoEnabled; }

		void enableWarningLog() { logWarningEnabled = true; }
		void disableWarningLog() { logWarningEnabled = false; }
		void toggleWarningLog() { logWarningEnabled = !logWarningEnabled; }
		bool isWarningLogEnabled() { return logWarningEnabled; }

		void enableErrorLog() { logErrorEnabled = true; }
		void disableErrorLog() { logErrorEnabled = false; }
		void toggleErrorLog() { logErrorEnabled = !logErrorEnabled; }
		bool isErrorLogEnabled() { return logErrorEnabled; }

		void enableDebugLog() { logDebugEnabled = true; }
		void disableDebugLog() { logDebugEnabled = false; }
		void toggleDebugLog() { logDebugEnabled = !logDebugEnabled; }
		bool isLogDebugEnabled() { return logDebugEnabled; }

		double getSecondsElapsed();
		double getMillisecondsElapsed();
		double getMicrosecondsElapsed();
		void resetClock();

		void saveStats(const std::string& filenameprefix);
		void takeStat(const std::string& name);
		void takeStat(const std::string& name, double xval);
		void resetStat(const std::string& name);
		void computeStat(const std::string& name, bool filter = true);
		const TimeDataPoints& getStat(const std::string& name);
	};

	extern Hatchetfish Log;
}

#endif
