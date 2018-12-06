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
#ifndef VIPERFISH_STOPWATCH_HPP
#define VIPERFISH_STOPWATCH_HPP

#include <chrono>

namespace Viperfish
{
class IStopWatch
{
  public:
	IStopWatch() {}
	virtual ~IStopWatch(){};

	virtual void Start() = 0;
	virtual void Stop() = 0;
	virtual double GetMillisecondsElapsed() = 0;
	virtual double GetSecondsElapsed() = 0;
};

// class SteadyStopWatch implements IStopWatch
class StopWatch : public IStopWatch
{
  public:
	StopWatch();
	~StopWatch();

	void Start();
	void Stop();
	double GetMillisecondsElapsed();
	double GetSecondsElapsed();

  private:
	std::chrono::time_point<std::chrono::system_clock> start_timepoint;
	std::chrono::time_point<std::chrono::system_clock> end_timepoint;
};
} // namespace Viperfish

#endif
