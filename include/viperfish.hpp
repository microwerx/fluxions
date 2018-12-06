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
#ifndef VIPERFISH_HPP
#define VIPERFISH_HPP


#include <map>
#include <vector>
#include <string>
#include <memory>
#include <typeinfo>
#include <functional>
#include <chrono>

#include <viperfish_keyboard.hpp>
#include <viperfish_mouse.hpp>
#include <viperfish_gamepad.hpp>
#include <viperfish_xinput.hpp>
#include <viperfish_utilities.hpp>
#include <viperfish_stopwatch.hpp>
#include <viperfish_glut.hpp>
#include <viperfish_widget.hpp>
#include <viperfish_imgui.hpp>


namespace Viperfish
{
	using namespace std;

	template <class T, class... _Types> static shared_ptr<T> MakeShared(_Types&&... _Args) { return shared_ptr<T>(new T(forward<_Types>(_Args)...)); }
	template <class T, class... _Types> static unique_ptr<T> MakeUnique(_Types&&... _Args) { return unique_ptr<T>(new T(forward<_Types>(_Args)...)); }

	///// <summary>shared_ptr<T> CreateObject(name) returns a smart pointer to a new Object class with the name set.</summary>
	//template<typename T>
	//shared_ptr<T> CreateObject(const string &name)
	//{
	//	shared_ptr<T> newT;

	//	newT = make_shared<T>();
	//	newT->SetClassName(typeid(T).name());
	//	newT->SetName(name);
	//	newT->Seal();

	//	return newT;
	//}


	///// <summary>class Object is the base class for all Viperfish classes that require a name.</summary>
	//class Object
	//{
	//private:
	//	string className_;
	//	string name_;
	//	bool isSealed = false;
	//public:
	//	Object() { }
	//	~Object() { }
	//	const string & GetName() const { return name_; }
	//	void SetName(const string &name) { if (!isSealed) name_ = name; }
	//	const string & GetClassName() const { return className_; }
	//	void SetClassName(const string &className) { if (!isSealed) className_ = className; }
	//	void Seal() { isSealed = true; }
	//};


	//using ObjectPtr = shared_ptr<Object>;
	//using WeakObjectPtr = shared_ptr<Object>;


	///// <summary>class Widget is the base class for all Viperfish widget classes.</summary>
	//class AWidget : public enable_shared_from_this<AWidget>, public Object
	//{
	//private:
	//	bool isPaused = false;
	//	bool isStopped = true;
	//	bool isStarted = false;
	//	bool isRecursive = false;
	//	bool isInitialized = false;

	//	double timeRunning_;

	//	bool isEnabled = true;

	//	using WidgetPtr = shared_ptr<AWidget>;

	//	vector<WidgetPtr> children_;
	//	WidgetPtr parent;

	//public:
	//	AWidget();
	//	virtual ~AWidget();

	//	WidgetPtr GetParent() { return parent; }
	//	WidgetPtr operator[](int i) { return children_.at(i); }
	//	vector<WidgetPtr>::iterator begin() { return children_.begin(); }
	//	vector<WidgetPtr>::iterator end() { return children_.end(); }
	//	void push_back(WidgetPtr ptr) { children_.push_back(ptr); }
	//	void pop_back() { children_.pop_back(); }
	//	const WidgetPtr & front() { return children_.front(); }
	//	const WidgetPtr & back() { return children_.back(); }
	//	size_t size() { return children_.size(); }
	//	bool empty() { return children_.empty(); }
	//	WidgetPtr *data() { return children_.data(); }

	//	void Init();
	//	void Kill();
	//	void Start();
	//	void Stop();
	//	void Pause();
	//	void Resume();
	//	void Restart();
	//	void Update(double elapsedTime);

	//	void Enable() { isEnabled = true; }
	//	void Disable() { isEnabled = false; }
	//	void ToggledEnabled() { isEnabled = !isEnabled; }
	//	bool IsEnabled() { return isEnabled; }

	//	const double GetRunningTime() const { return timeRunning_; }
	//	void ResetClock() { timeRunning_ = 0.0; }
	//	const bool IsStarted() const { return isStarted; }
	//	const bool IsStopped() const { return isStopped; }
	//	const bool IsPaused() const { return isPaused; }
	//	const bool IsRecursive() const { return isRecursive; }
	//	const bool IsInitialized() const { return isInitialized; }
	//	void SetRecursion(bool value) { isRecursive = value; }

	//	/* This was the old code. I think it's too complicated.
	//	 *
	//	std::function<void(shared_ptr<Widget> &)> OnInitOverride;
	//	std::function<void(shared_ptr<Widget> &)> OnKillOverride;
	//	std::function<void(shared_ptr<Widget> &)> OnStartOverride;
	//	std::function<void(shared_ptr<Widget> &)> OnStopOverride;
	//	std::function<void(shared_ptr<Widget> &)> OnPauseOverride;
	//	std::function<void(shared_ptr<Widget> &)> OnResumeOverride;
	//	std::function<void(shared_ptr<Widget> &, double)> OnUpdateOverride;
	//	std::function<void(shared_ptr<Widget> &, void *, unsigned, int, int, int, void *)> OnHandleMessageOverride;

	//	virtual void OnInit() { if (isEnabled && OnInitOverride) OnInitOverride(shared_from_this()); }
	//	virtual void OnKill() { if (isEnabled && OnKillOverride) OnKillOverride(shared_from_this()); }
	//	virtual void OnStart() { if (isEnabled && OnStartOverride) OnStartOverride(shared_from_this()); }
	//	virtual void OnStop() { if (isEnabled && OnStopOverride) OnStopOverride(shared_from_this()); }
	//	virtual void OnPause() { if (isEnabled && OnPauseOverride) OnPauseOverride(shared_from_this()); }
	//	virtual void OnResume() { if (isEnabled && OnResumeOverride) OnResumeOverride(shared_from_this()); }
	//	virtual void OnUpdate(double deltaTime) { if (isEnabled && OnUpdateOverride) OnUpdateOverride(shared_from_this(), deltaTime); }
	//	virtual void HandleMessage(void *handle, unsigned msg, int param1, int param2, int param3, void *param4) {
	//		if (isEnabled && OnHandleMessageOverride)
	//			OnHandleMessageOverride(shared_from_this(), handle, msg, param1, param2, param3, param4);
	//	}
	//	*/

	//	virtual void OnInit() { }
	//	virtual void OnKill() { }
	//	virtual void OnStart() { }
	//	virtual void OnStop() { }
	//	virtual void OnPause() { }
	//	virtual void OnResume() { }
	//	virtual void OnUpdate(double deltaTime) { }
	//	virtual void HandleMessage(void *handle, unsigned msg, int param1, int param2, int param3, void *param4) { }
	//};


	//using WidgetPtr = shared_ptr<Widget>;
	//using WeakWidgetPtr = weak_ptr<Widget>;


	//ObjectPtr CreateObjectFromType(const string &className, const string &instanceName);


	//class ViperfishApplication : public AWidget
	//{
	//public:
	//	ViperfishApplication();
	//	virtual ~ViperfishApplication();
	//};

	//using ViperfishApplicationPtr = shared_ptr<ViperfishApplication>;

	//extern ViperfishApplication VfApp;
}


#endif
