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
#include "pch.h"
#include <hatchetfish.hpp>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef __APPLE__
#define __unix__ 1
#endif

Hatchetfish hflog;

Hatchetfish::Hatchetfish()
{
	fout = stdout;
}

Hatchetfish::~Hatchetfish()
{
	setOutputFile();
}

std::string Hatchetfish::makeDTG()
{
	char msg[50];
#ifdef __STDC_SECURE_LIB__
	struct tm newtime;
	__time64_t long_time;
	_time64(&long_time);
	localtime_s(&newtime, &long_time);
	strftime(msg, 50, "%Y%m%d%H%M%S", &newtime);
#else
	std::chrono::time_point<std::chrono::system_clock> tp = std::chrono::system_clock::now();
	time_t t = std::chrono::system_clock::to_time_t(tp);
	tm *_Tm = nullptr;
#ifdef WIN32
	localtime_s(_Tm, &t);
#elif __unix__
	localtime_r(&t, _Tm);
#endif
	strftime(msg, 50, "%Y%m%d%H%M%S", _Tm);
#endif
	dtg = msg;
	return dtg;
}

std::string Hatchetfish::makeTimeStamp()
{
	char msg[50] = { 0 };
#ifdef __STDC_SECURE_LIB__
	struct tm newtime;
	__time64_t long_time;
	_time64(&long_time);
	localtime_s(&newtime, &long_time);
	strftime(msg, 50, "%T", &newtime);
#else
	std::chrono::time_point<std::chrono::system_clock> tp = std::chrono::system_clock::now();
	time_t t = std::chrono::system_clock::to_time_t(tp);
	struct tm _Tm;
	memset(&_Tm, 0, sizeof(struct tm));

#ifdef WIN32
	localtime_s(_Tm, &t);
#elif __unix__
	localtime_r(&t, &_Tm);
#endif
	strftime(msg, 50, "%T", &_Tm);
#endif
	timeStamp = msg;
	return timeStamp;
}

const std::string &Hatchetfish::makeMessage(const char *category, const char *msg, va_list args)
{
	std::ostringstream ostr;
	makeTimeStamp();

	char buffer[1024];
#ifdef __STDC_WANT_SECURE_LIB__
	_vsnprintf_s(buffer, sizeof(buffer), sizeof(buffer) - 1, msg, args);
#else
	vsnprintf(buffer, 1024, msg, args);
#endif

	ostr << "[" << timeStamp << ":" << category << "] " << buffer;
	lastMessage = ostr.str();

	//size_t lines = std::min(history.size(), maxHistoryLines);
	if (history.size() > maxHistoryLines)
		history.erase(history.begin(), history.end() - maxHistoryLines + 1);
	history.push_back(lastMessage);

	return lastMessage;
}

const std::string &Hatchetfish::makeMessagefn(const char *category, const char *fn, const char *msg, va_list args)
{
	std::ostringstream ostr;
	makeTimeStamp();

	char buffer[1024];
#ifdef __STDC_WANT_SECURE_LIB__
	_vsnprintf_s(buffer, sizeof(buffer), sizeof(buffer) - 1, msg, args);
#else
	vsnprintf(buffer, 1024, msg, args);
#endif

	ostr << "[" << timeStamp << ":" << category << "] ";
	if (fn != nullptr)
		ostr << fn << "(): ";
	ostr << buffer;
	lastMessage = ostr.str();

	//size_t lines = std::min(history.size(), maxHistoryLines);
	if (history.size() > maxHistoryLines)
		history.erase(history.begin(), history.end() - maxHistoryLines + 1);
	history.push_back(lastMessage);

	return lastMessage;
}

void Hatchetfish::log(const char *category, const char *msg, ...)
{
	va_list args;
	va_start(args, msg);
	makeMessage(category, msg, args);
	va_end(args);

	if (logEnabled)
		fprintf(fout, "%s\n", lastMessage.c_str());
}

void Hatchetfish::logfn(const char *category, const char *fn, const char *msg, ...)
{
	va_list args;
	va_start(args, msg);
	makeMessagefn(category, fn, msg, args);
	va_end(args);

	if (logEnabled)
		fprintf(fout, "%s\n", lastMessage.c_str());
}

void Hatchetfish::info(const char *msg, ...)
{
	va_list args;
	va_start(args, msg);
	makeMessage("INFO", msg, args);
	va_end(args);

	if (logEnabled)
		fprintf(fout, "%s\n", lastMessage.c_str());
}

void Hatchetfish::infofn(const char *fn, const char *msg, ...)
{
	va_list args;
	va_start(args, msg);
	makeMessagefn("INFO", fn, msg, args);
	va_end(args);

	if (logEnabled)
		fprintf(fout, "%s\n", lastMessage.c_str());
}

void Hatchetfish::warning(const char *msg, ...)
{
	va_list args;
	va_start(args, msg);
	makeMessage("WARNING", msg, args);
	va_end(args);

	if (logEnabled)
		fprintf(fout, "%s\n", lastMessage.c_str());
}

void Hatchetfish::warningfn(const char *fn, const char *msg, ...)
{
	va_list args;
	va_start(args, msg);
	makeMessagefn("WARNING", fn, msg, args);
	va_end(args);

	if (logEnabled)
		fprintf(fout, "%s\n", lastMessage.c_str());
}

void Hatchetfish::error(const char *msg, ...)
{
	va_list args;
	va_start(args, msg);
	makeMessage("ERROR", msg, args);
	va_end(args);

	if (logEnabled)
		fprintf(fout, "%s\n", lastMessage.c_str());
}

void Hatchetfish::errorfn(const char *fn, const char *msg, ...)
{
	va_list args;
	va_start(args, msg);
	makeMessagefn("ERROR", fn, msg, args);
	va_end(args);

	if (logEnabled)
		fprintf(fout, "%s\n", lastMessage.c_str());
}

void Hatchetfish::debug(const char *msg, ...)
{
	va_list args;
	va_start(args, msg);
	makeMessage("DEBUG", msg, args);
	va_end(args);

	if (logEnabled)
		fprintf(fout, "%s\n", lastMessage.c_str());
}

void Hatchetfish::debugfn(const char *fn, const char *msg, ...)
{
	va_list args;
	va_start(args, msg);
	makeMessagefn("DEBUG", fn, msg, args);
	va_end(args);

	if (logEnabled)
		fprintf(fout, "%s\n", lastMessage.c_str());
}

void Hatchetfish::flush()
{
	fflush(fout);
}

void Hatchetfish::setOutputFile(FILE *fileStream)
{
	if (fileStream == stdin)
		return;
	if (fout != NULL && fout != stderr && fout != stdout) {
		fclose(fout);
	}
	if (fileStream == NULL)
		fout = stdout;
	else
		fout = fileStream;
}

double Hatchetfish::getSecondsElapsed()
{
	return getMicrosecondsElapsed() / 1.0e6;
}

double Hatchetfish::getMillisecondsElapsed()
{
	return getMicrosecondsElapsed() / 1.0e3;
}

double Hatchetfish::getMicrosecondsElapsed()
{
	t1 = std::chrono::high_resolution_clock::now();
	return (double)std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count();
}

void Hatchetfish::resetClock()
{
	t0 = t1 = std::chrono::high_resolution_clock::now();
}

void Hatchetfish::saveStats(const std::string &filenameprefix)
{
	for (auto &stat : stats) {
		computeStat(stat.first);
		std::ofstream fout_(filenameprefix + stat.first + ".csv", std::ios::app);

		fout_ << "scene,i,time,x,mr,xbar,mrbar,lcl,ucl" << std::endl;
		fout_ << std::fixed << std::setprecision(6);

		int i = 0;
		for (auto &datapoint : stat.second.X) {
			if (i > 300)
				break;
			fout_ << filenameprefix << "," << i++ << "," << datapoint.timeMeasured << ",";
			fout_ << std::setprecision(4) << datapoint.x << "," << datapoint.r << "," << stat.second.xbar << "," << stat.second.rbar << "," << stat.second.lcl << "," << stat.second.ucl << std::endl;
		}

		fout_.close();
	}
}

void Hatchetfish::takeStat(const std::string &name)
{
	double elapsedTime = getSecondsElapsed();
	auto &X = stats[name].X;
	double deltaTime = 0.0;
	double r = 0.0;
	if (!X.empty()) {
		deltaTime = elapsedTime - X.back().timeMeasured;
		r = deltaTime - X.back().x;
	}

	X.push_back({ elapsedTime, deltaTime, r });
}

void Hatchetfish::takeStat(const std::string &name, double xval)
{
	double elapsedTime = getSecondsElapsed();
	auto &X = stats[name].X;
	double r = 0.0;
	if (!X.empty()) {
		r = xval - X.back().x;
	}

	X.push_back({ elapsedTime, xval, r });
}

void Hatchetfish::resetStat(const std::string &name)
{
	auto &stat = stats[name];
	stat.lcl = 0.0;
	stat.ucl = 0.0;
	stat.xbar = 0.0;
	stat.rbar = 0.0;
	stat.X.clear();
}

void Hatchetfish::computeStat(const std::string &name, bool filter)
{
	TimeDataPoints &stat = stats[name];
	stat.lcl = 0.0;
	stat.ucl = 0.0;
	stat.xbar = 0.0;
	stat.rbar = 0.0;

	stat.computeStats();
	if (filter) {
		size_t size = stat.X.size();
		size_t i = 0;
		double lastx = 0.0;
		double nextx = stat.xbar;
		// filter out obviously bad data points
		for (TimeDataPoint &datapoint : stat.X) {
			lastx = (i == 0) ? stat.xbar : stat.X[i - 1].x;
			nextx = (i < size - 1) ? stat.X[i + 1].x : stat.xbar;
			if (abs(datapoint.x - stat.xbar) > abs(10.0 * stat.xbar)) {
				datapoint.x = 0.5 * (nextx + lastx);
			}
			i++;
		}
		stat.computeStats();
	}
}