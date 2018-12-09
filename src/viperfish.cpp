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
#include "viperfish.hpp"
#include "stdafx.h"

namespace Viperfish
{

//ObjectPtr CreateObjectFromType(const std::string &className, const std::string &instanceName)
//{
//	ObjectPtr newObj;

//	if (className == "Object") newObj = CreateObject<Object>(instanceName);
//	else if (className == "Widget") newObj = CreateObject<AWidget>(instanceName);

//	return newObj;
//}

//AWidget::AWidget()
//{

//}

//AWidget::~AWidget()
//{

//}

//void AWidget::Init()
//{
//	ResetClock();
//	if (!isInitialized) OnInit();
//	isInitialized = true;
//	for (auto child = begin(); child != end(); ++child)
//	{
//		(*child)->Init();
//	}
//}

//void AWidget::Kill()
//{
//	// Do a depth first parse of the tree
//	for (auto child = begin(); child != end(); ++child)
//	{
//		(*child)->Kill();
//	}
//	children_.clear();
//	OnKill();
//	isInitialized = false;
//}

//void AWidget::Start()
//{
//	if (!isInitialized) return;
//	if (isStopped)
//	{
//		isStopped = false;
//		isStarted = true;
//		isPaused = false;
//		timeRunning_ = 0.0;
//		OnStart();
//		if (isRecursive)
//		{
//			for (auto child = begin(); child != end(); ++child)
//			{
//				(*child)->Start();
//			}
//		}
//	}
//}

//void AWidget::Stop()
//{
//	if (!isInitialized) return;
//	if (isStarted)
//	{
//		if (isPaused)
//		{
//			Resume();
//		}
//		isStopped = true;
//		isStarted = false;
//		isPaused = false;
//		OnStop();
//		if (isRecursive)
//		{
//			for (auto child = begin(); child != end(); ++child)
//			{
//				(*child)->Stop();
//			}
//		}
//	}
//}

//void AWidget::Pause()
//{
//	if (!isInitialized) return;
//	if (isStarted)
//	{
//		isPaused = true;
//		OnPause();
//		if (isRecursive)
//		{
//			for (auto child = begin(); child != end(); ++child)
//			{
//				(*child)->Pause();
//			}
//		}
//	}

//}

//void AWidget::Resume()
//{
//	if (!isInitialized) return;
//	if (isStarted && isPaused)
//	{
//		isPaused = false;
//		OnResume();
//	}
//}

//void AWidget::Restart()
//{
//	if (!isInitialized) return;
//	if (isStarted)
//	{
//		Stop();
//	}
//	Start();
//}

//void AWidget::Update(double elapsedTime)
//{
//	if (!isInitialized) return;
//	if (isStarted && !isPaused)
//	{
//		timeRunning_ += elapsedTime;
//		OnUpdate(elapsedTime);
//		for (auto child = begin(); child != end(); ++child)
//		{
//			(*child)->Update(elapsedTime);
//		}
//	}
//}

//ViperfishApplication VfApp;

//std::function<void(WidgetPtr &decorateeWidget_)> ViperfishApplication_OnInit = [](WidgetPtr &decorateeWidget_) {
//	ViperfishApplicationPtr vfapp = dynamic_pointer_cast<ViperfishApplication>(decorateeWidget_);
//};

//// <summary>ViperfishApplication constructor</summary>
//ViperfishApplication::ViperfishApplication()
//{

//}

//// <summary>ViperfishApplication destructor</summary>
//ViperfishApplication::~ViperfishApplication()
//{

//}
} // namespace Viperfish
