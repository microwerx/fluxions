#include <hatchetfish_log.hpp>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <chrono>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef __APPLE__
#define __unix__ 1
#endif

namespace Hf {
	HatchetfishLog Log;

	namespace ansi {
		const char* black = "\033[0;30m";
		const char* red = "\033[0;91m";
		const char* green = "\033[0;92m";
		const char* yellow = "\033[0;93m";
		const char* blue = "\033[0;94m";
		const char* magenta = "\033[0;95m";
		const char* cyan = "\033[0;96m";
		const char* white = "\033[0;97m";
		const char* normal = "\033[0m";
	}

	namespace hf {
		const char* info = "INFO ";
		const char* warning = "WARN ";
		const char* error = "ERROR";
		const char* debug = "DEBUG";
	}

	HatchetfishLog::HatchetfishLog() {
		fout = stdout;
	}

	HatchetfishLog::~HatchetfishLog() {
		setOutputFile();
	}

	std::string& HatchetfishLog::makeDTG() {
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
		tm* _Tm = nullptr;
#ifdef _WIN32
		localtime_s(_Tm, &t);
#elif __unix__
		localtime_r(&t, _Tm);
#endif
		strftime(msg, 50, "%Y%m%d%H%M%S", _Tm);
#endif
		dtg = msg;
		return dtg;
	}

	std::string& HatchetfishLog::makeTimeStamp() {
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

#ifdef _WIN32
		localtime_s(_Tm, &t);
#elif __unix__
		localtime_r(&t, &_Tm);
#endif
		strftime(msg, 50, "%T", &_Tm);
#endif
		timeStamp = msg;
		return timeStamp;
	}

	const std::string& HatchetfishLog::makeMessage(const char* category, const char* msg, va_list args) {
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

		_addHistory(lastMessage);

		return lastMessage;
	}

	void HatchetfishLog::_addHistory(const std::string& line) {
		_trimHistory();
		history.push_back(line);
		history_cstr.push_back(history.back().c_str());
	}

	void HatchetfishLog::_trimHistory() {
		if (maxHistoryLines == 0 || history.size() < maxHistoryLines) return;
		history.erase(history.begin(), history.end() - maxHistoryLines + 1);
		history_cstr.clear();
		for (auto& h : history) {
			history_cstr.push_back(h.c_str());
		}
	}

	const std::string& HatchetfishLog::makeMessagefn(const char* category, const char* fn, const char* msg, va_list args) {
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

		_addHistory(lastMessage);

		return lastMessage;
	}

	void HatchetfishLog::print(const char* color) {
		if (logEnabled && colorEnabled) {
			fprintf(fout, "%s%s%s\n", color, lastMessage.c_str(), ansi::normal);
		}
		else if (logEnabled) {
			fprintf(fout, "%s\n", lastMessage.c_str());
		}
	}

	void HatchetfishLog::str(std::string& output, const char* msg, ...) {
		va_list args;
		va_start(args, msg);
		static char buffer[1024];
#ifdef __STDC_WANT_SECURE_LIB__
		_vsnprintf_s(buffer, sizeof(buffer), sizeof(buffer) - 1, msg, args);
#else
		vsnprintf(buffer, 1024, msg, args);
#endif
		va_end(args);
		output = (const char*)buffer;
	}

	void HatchetfishLog::log(const char* category, const char* msg, ...) {
		va_list args;
		va_start(args, msg);
		makeMessage(category, msg, args);
		va_end(args);
		print(ansi::normal);
	}

	void HatchetfishLog::logfn(const char* category, const char* fn, const char* msg, ...) {
		va_list args;
		va_start(args, msg);
		makeMessagefn(category, fn, msg, args);
		va_end(args);
		print(ansi::normal);
	}

	void HatchetfishLog::info(const char* msg, ...) {
		va_list args;
		va_start(args, msg);
		makeMessage(hf::info, msg, args);
		va_end(args);
		print(ansi::cyan);
	}

	void HatchetfishLog::infofn(const char* fn, const char* msg, ...) {
		va_list args;
		va_start(args, msg);
		makeMessagefn(hf::info, fn, msg, args);
		va_end(args);
		print(ansi::cyan);
	}

	void HatchetfishLog::warning(const char* msg, ...) {
		va_list args;
		va_start(args, msg);
		makeMessage(hf::warning, msg, args);
		va_end(args);
		print(ansi::yellow);
	}

	void HatchetfishLog::warningfn(const char* fn, const char* msg, ...) {
		va_list args;
		va_start(args, msg);
		makeMessagefn(hf::warning, fn, msg, args);
		va_end(args);
		print(ansi::yellow);
	}

	void HatchetfishLog::error(const char* msg, ...) {
		va_list args;
		va_start(args, msg);
		makeMessage(hf::error, msg, args);
		va_end(args);
		print(ansi::red);
	}

	void HatchetfishLog::errorfn(const char* fn, const char* msg, ...) {
		va_list args;
		va_start(args, msg);
		makeMessagefn(hf::error, fn, msg, args);
		va_end(args);
		print(ansi::red);
	}

	void HatchetfishLog::debug(const char* msg, ...) {
		va_list args;
		va_start(args, msg);
		makeMessage(hf::debug, msg, args);
		va_end(args);
		print(ansi::magenta);
	}

	void HatchetfishLog::debugfn(const char* fn, const char* msg, ...) {
		va_list args;
		va_start(args, msg);
		makeMessagefn(hf::debug, fn, msg, args);
		va_end(args);
		print(ansi::magenta);
	}

	void HatchetfishLog::flush() {
		fflush(fout);
	}

	void HatchetfishLog::setOutputFile(FILE* fileStream) {
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

	double HatchetfishLog::getSecondsElapsed() {
		return getMicrosecondsElapsed() / 1.0e6;
	}

	double HatchetfishLog::getMillisecondsElapsed() {
		return getMicrosecondsElapsed() / 1.0e3;
	}

	double HatchetfishLog::getMicrosecondsElapsed() {
		t1 = std::chrono::high_resolution_clock::now();
		return (double)std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count();
	}

	void HatchetfishLog::resetClock() {
		t0 = t1 = std::chrono::high_resolution_clock::now();
	}

	void HatchetfishLog::saveStats(const std::string& filenameprefix) {
		for (auto& stat : stats) {
			computeStat(stat.first);
			std::ofstream fout_(filenameprefix + stat.first + ".csv", std::ios::app);

			fout_ << "scene,i,time,x,mr,xbar,mrbar,lcl,ucl" << std::endl;
			fout_ << std::fixed << std::setprecision(6);

			int i = 0;
			for (auto& datapoint : stat.second.X) {
				if (i > 300)
					break;
				fout_ << filenameprefix << "," << i++ << "," << datapoint.timeMeasured << ",";
				fout_ << std::setprecision(4) << datapoint.x << "," << datapoint.r << "," << stat.second.xbar << "," << stat.second.rbar << "," << stat.second.lcl << "," << stat.second.ucl << std::endl;
			}

			fout_.close();
		}
	}

	void HatchetfishLog::takeStat(const std::string& name) {
		double elapsedTime = getSecondsElapsed();
		auto& X = stats[name].X;
		double deltaTime = 0.0;
		double r = 0.0;
		if (!X.empty()) {
			deltaTime = elapsedTime - X.back().timeMeasured;
			r = deltaTime - X.back().x;
		}

		X.push_back({ elapsedTime, deltaTime, r });
	}

	void HatchetfishLog::takeStat(const std::string& name, double xval) {
		double elapsedTime = getSecondsElapsed();
		auto& X = stats[name].X;
		double r = 0.0;
		if (!X.empty()) {
			r = xval - X.back().x;
		}

		X.push_back({ elapsedTime, xval, r });
	}

	void HatchetfishLog::resetStat(const std::string& name) {
		auto& stat = stats[name];
		stat.lcl = 0.0;
		stat.ucl = 0.0;
		stat.xbar = 0.0;
		stat.rbar = 0.0;
		stat.X.clear();
	}

	void HatchetfishLog::computeStat(const std::string& name, bool filter) {
		TimeDataPoints& stat = stats[name];
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
			for (TimeDataPoint& datapoint : stat.X) {
				lastx = (i == 0) ? stat.xbar : stat.X[i - 1].x;
				nextx = (i < size - 1) ? stat.X[i + 1].x : stat.xbar;
				if (std::abs(datapoint.x - stat.xbar) > abs(10.0 * stat.xbar)) {
					datapoint.x = 0.5 * (nextx + lastx);
				}
				i++;
			}
			stat.computeStats();
		}
	}

	const HatchetfishLog::TimeDataPoints& HatchetfishLog::getStat(const std::string& name) {
		TimeDataPoints& stat = stats[name];
		return stat;
	}
}
