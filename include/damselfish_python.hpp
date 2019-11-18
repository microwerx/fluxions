// Damselfish
// Copyright (C) 2017-2019 Jonathan Metzgar
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
#ifndef DAMSELFISH_PYTHON_HPP
#define DAMSELFISH_PYTHON_HPP

#include <thread>
#include <mutex>
#include <deque>
#include <vector>
#include <string>

namespace Df
{
class PythonInterpreter
{
  public:
	enum class Command
	{
		Init,
		Kill,
		Start,
		Stop,
		ImportModule,
		PyRunString,
		PyRunFile,
		PyRun_SimpleString,
		PyRun_AnyFile,
		PyRun_SimpleFile,
		PyRun_InteractiveOne,
		PyRun_InteractiveLoop,
		PyRun_File
	};

	PythonInterpreter();
	~PythonInterpreter();

	/// <summary>init(programName) Initializes the Python code in its own thread.
	/// <code>programName</code> is the reference name used by the Python library.</summary>
	bool init(const std::string &programName);
	/// <summary>Removes any resources and resets the class to its initial conditions.</summary>
	void kill();
	/// <summary>run() causes the thread to start and the command queue to begin executing.</summary>
	void run();
	/// <summary>join() waits for the Python thread to finish.</summary>
	void join();
	/// <summary>start() Starts the Python thread.</summary>
	void start();
	/// <summary>stop() stops the Python thread from executing.</summary>
	void stop();
	/// <summary>flush() causes all the commands in the command queue to execute and finish.</summary>
	void flush();
	/// <summary>addCommand(cmd) runs a non string command. Is Equivalent to <code>addCommand(cmd, string())</code>.</summary>
	void addCommand(Command cmd) { addCommand(cmd, std::string()); }
	/// <summary>addCommand(cmd, str) adds a command string to the command deque.</summary>
	void addCommand(Command cmd, const std::string &str);

	constexpr bool isInitialized() const { return currentState != State::InitialState; }
	constexpr bool isStarted() const { return currentState == State::Started; }
	constexpr bool isReady() const { return currentState == State::Ready; }

	// Mutex control of the Python Interpreter thread

	/// <summary>lock() locks the mutex for the thread, blocking the current thread if necessary.
	/// This is for issuing modifications to internal data structures</summary>
	void lock();
	/// <summary>try_lock() locks the mutex for the thread. If this cannot be done immediately,
	/// it returns false, otherwise true.</summary>
	bool try_lock();
	/// <summary>unlock() unlocks the mutex for the thread. It returns immediately.</summary>
	void unlock();
	/// <summary>getCommandCount() returns the number of commands dequeued in the thread loop.</summary>
	size_t getCommandCount() const { return cmdCount; }

  private:
	enum class State
	{
		InitialState,
		Ready,
		Started
	};
	std::thread myThread;
	std::mutex myMutex;
	std::string myProgramName;
	size_t cmdCount;

	std::deque<std::pair<Command, std::string>> commandDeque;

	State currentState = State::InitialState;

	bool processSingleCommand();
	bool doThreadLoop();
	static void thread_func(const std::string &programName, PythonInterpreter *pPyInterpreter);

	void onInit();
	void onKill();
	void onStart();
	void onStop();
	void onImportModule(const std::string &param);
	void onCallFunction(const std::string &param);
	void onPyRun_SimpleString(const std::string &param);
	void onPyRun_AnyFile(const std::string &param);
	void onPyRun_SimpleFile(const std::string &param);
	void onPyRun_InteractiveOne(const std::string &param);
	void onPyRun_InteractiveLoop(const std::string &param);
	void onPyRun_File(const std::string &param);
};
} // namespace Df

#endif
