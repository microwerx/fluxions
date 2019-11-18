// Damselfish
// Copyright (C) 2019 Jonathan Metzgar
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
#include <string>
#include <utility>

#include <codecvt>
#include <iostream>
#include <locale>
#include <string>
#include <hatchetfish.hpp>

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#endif

#pragma comment(lib, "hatchetfish.lib")

inline std::string wstring_to_string(const std::wstring &wstr) noexcept
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	return converter.to_bytes(wstr);
}

inline std::wstring string_to_wstring(const std::string &str) noexcept
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	return converter.from_bytes(str);
}

#ifdef _DEBUG
#undef _DEBUG
#include <Python.h>
#define _DEBUG
#else
#include <Python.h>
#endif

#include <damselfish_python.hpp>

namespace Df
{

	PythonInterpreter::PythonInterpreter()
	{
	}

	PythonInterpreter::~PythonInterpreter()
	{
	}

	bool PythonInterpreter::init(const std::string &programName)
	{
		if (myThread.joinable()) {
			// we don't want to initialize a thread that's already running...
			return false;
		}
		commandDeque.clear();
		addCommand(Command::Init, programName);
		currentState = State::Ready;
		return true;
	}

	void PythonInterpreter::kill()
	{
		if (myThread.joinable()) {
			// thread is already running, so send the message to kill it...
			addCommand(Command::Kill);
		}
		else {
			currentState = State::InitialState;
			commandDeque.clear();
		}
	}

	void PythonInterpreter::join()
	{
		myThread.join();
	}

	void PythonInterpreter::run()
	{
		myThread = std::thread(PythonInterpreter::thread_func, myProgramName, this);
	}

	void PythonInterpreter::start()
	{
		addCommand(Command::Start);
	}

	void PythonInterpreter::stop()
	{
		addCommand(Command::Stop);
	}

	void PythonInterpreter::flush()
	{
		while (processSingleCommand())
			;
	}

	void PythonInterpreter::lock()
	{
		myMutex.lock();
	}

	bool PythonInterpreter::try_lock()
	{
		return myMutex.try_lock();
	}

	void PythonInterpreter::unlock()
	{
		myMutex.unlock();
	}

	void PythonInterpreter::addCommand(Command cmd, const std::string &str)
	{
		lock();
		commandDeque.push_back(std::pair<Command, std::string>(cmd, str));
		unlock();
	}

	bool PythonInterpreter::processSingleCommand()
	{
		if (currentState == State::InitialState)
			return false;

		bool retval = false;
		// pop front of vector and process the single command...
		lock();
		if (!commandDeque.empty()) {
			std::pair<Command, std::string> curCmd = commandDeque.front();
			commandDeque.pop_front();
			unlock();
			switch (curCmd.first) {
			case Command::Init:
				onInit();
				break;
			case Command::Kill:
				onKill();
				break;
			case Command::Start:
				onStart();
				break;
			case Command::Stop:
				onStop();
				break;
			case Command::PyRunString:
			case Command::PyRun_SimpleString:
				onPyRun_SimpleString(curCmd.second);
				break;
			case Command::PyRunFile:
			case Command::PyRun_SimpleFile:
				onPyRun_SimpleFile(curCmd.second);
				break;
			case Command::PyRun_AnyFile:
				onPyRun_AnyFile(curCmd.second);
				break;
			case Command::PyRun_InteractiveOne:
				onPyRun_InteractiveLoop(curCmd.second);
				break;
			case Command::PyRun_InteractiveLoop:
				onPyRun_InteractiveLoop(curCmd.second);
				break;
			case Command::PyRun_File:
				onPyRun_File(curCmd.second);
				break;
			case Command::ImportModule:
				onImportModule(curCmd.second);
				break;
			}
			retval = true;
			cmdCount++;
		}
		else {
			unlock();
		}

		// false means we haven't done any command
		return retval;
	}

	bool PythonInterpreter::doThreadLoop()
	{
		if (currentState == State::InitialState)
			return false;

		flush();

		return true;
	}

	void PythonInterpreter::thread_func(const std::string &programName, PythonInterpreter *pPyInterpreter)
	{
		if (pPyInterpreter == nullptr)
			return;
#ifdef _WIN32
		BOOL result = AllocConsole();
		Hf::Log.info("%(): Opening Console: result = %d", __FUNCTION__, result);
#endif
		std::cout << "PythonInterpreter::thread_func() [" << pPyInterpreter->myThread.get_id() << "]: starting[" << programName << "]..." << std::endl;
		try {
			if (pPyInterpreter->currentState == PythonInterpreter::State::InitialState) {
				pPyInterpreter->flush();
			}

			while (pPyInterpreter->doThreadLoop()) {
				//this_thread::yield();
				//this_thread::sleep_for(1ms);
				std::this_thread::sleep_for(std::chrono::seconds(1));
			}
		}
		catch (const std::exception &e) {
			std::cerr << "EXCEPTION! PythonInterpreter::thread_func() [" << pPyInterpreter->myThread.get_id() << "]: " << std::endl;
			std::cerr << e.what() << std::endl;
		}
		catch (...) {
			std::cerr << "EXCEPTION! PythonInterpreter::thread_func() [" << pPyInterpreter->myThread.get_id() << "]" << std::endl;
		}

		std::cout << "PythonInterpreter::thread_func() [" << pPyInterpreter->myThread.get_id() << "]: stopping..." << std::endl;
	}

	void PythonInterpreter::onInit()
	{
		//cout << "OnInit" << std::endl;
		std::wstring wstr = string_to_wstring(myProgramName);
		wchar_t *wstrPtr = &wstr[0];
		Py_SetProgramName(wstrPtr);
		Py_Initialize();
		PySys_SetArgv(1, &wstrPtr);
		currentState = State::Ready;
		cmdCount = 0;
	}

	void PythonInterpreter::onKill()
	{
		//cout << "OnKill" << std::endl;
		Py_Finalize();
		currentState = State::InitialState;
		commandDeque.clear();
	}

	void PythonInterpreter::onStart()
	{
		if (currentState == State::InitialState)
			return;
		currentState = State::Started;
		//cout << "OnStart" << std::endl;
	}

	void PythonInterpreter::onStop()
	{
		if (currentState == State::InitialState)
			return;
		//cout << "OnStop" << std::endl;
		currentState = State::Ready;
	}

	void PythonInterpreter::onImportModule(const std::string &param)
	{
		PyObject *pName = Py_BuildValue("s", param.c_str());
		if (pName == nullptr)
			return;
		PyObject *pModule = PyImport_Import(pName);

		if (pName)
			Py_DECREF(pName);
		if (pModule)
			Py_DECREF(pName);
	}

	void PythonInterpreter::onCallFunction(const std::string &param)
	{
	}

	void PythonInterpreter::onPyRun_SimpleString(const std::string &param)
	{
		if (currentState != State::Started)
			return;
		//cout << "OnPyRun_SimpleString: " << param << std::endl;
		PyRun_SimpleString(param.c_str());
	}

	void PythonInterpreter::onPyRun_AnyFile(const std::string &param)
	{
		if (currentState != State::Started)
			return;
		//cout << "OnPyRun_AnyFile: " << param << std::endl;
	}

	void PythonInterpreter::onPyRun_SimpleFile(const std::string &param)
	{
		if (currentState != State::Started)
			return;
		//cout << "OnPyRun_SimpleFile: " << param << std::endl;
		FILE *fin = _Py_fopen(param.c_str(), "r+");
		if (fin) {
			try {
				int retval = PyRun_SimpleFileExFlags(fin, param.c_str(), 1, nullptr);
				std::cout << "retval: " << retval << std::endl;
			}
			catch (...) {
				// ergh...
			}
		}
	}

	void PythonInterpreter::onPyRun_InteractiveOne(const std::string &param)
	{
		if (currentState != State::Started)
			return;
		//cout << "OnPyRun_InteractiveOne: " << param << std::endl;
	}

	void PythonInterpreter::onPyRun_InteractiveLoop(const std::string &param)
	{
		if (currentState != State::Started)
			return;
		//cout << "OnPyRun_InteractiveLoop: " << param << std::endl;
	}

	void PythonInterpreter::onPyRun_File(const std::string &param)
	{
		if (currentState != State::Started)
			return;
		//cout << "OnPyRun_File: " << param << std::endl;
	}

	int test_PythonInterpreter(int argc, char **argv)
	{
		std::cout << "PythonInterpreter TEST" << std::endl;

		PythonInterpreter python;

		python.init(std::string("TestPythonInterpreter"));
		python.run();
		python.start();
		bool done = false;
		size_t cmdcount = python.getCommandCount();
		while (!done) {
			while (cmdcount == python.getCommandCount())
				std::this_thread::yield();
			std::cout << "Enter a command: " << std::flush;
			std::string cmdline = "";
			//cin.clear();
			//cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			getline(std::cin, cmdline);
			if (cmdline.size() == 0) {
				done = true;
			}
			else if (cmdline == "quit") {
				done = true;
			}
			else {
				cmdcount = python.getCommandCount();
				// python.addCommand(PythonInterpreter::Command::PyRun_SimpleString, cmdline);
				python.addCommand(PythonInterpreter::Command::PyRun_SimpleFile, cmdline);
			}
		}
		python.kill();
		python.join();
		return 0;
	}
} // namespace Df
