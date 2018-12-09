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
#ifndef FLUXIONS_RESOURCE_MANAGER_HPP
#define FLUXIONS_RESOURCE_MANAGER_HPP

#include <string>
#include <memory>
#include <algorithm>
#include <vector>
#include <map>

namespace Fluxions
{

extern const std::string BlankString;

template <typename T>
class TResourceManager
{
  private:
	std::map<unsigned, T> resources;
	T defaultObject;

	unsigned largestHandle;
	std::vector<unsigned> availableResourceHandles;
	std::vector<unsigned> allocatedResourceHandles;

	std::map<std::string, unsigned> stringToHandleMap;
	std::map<unsigned, std::vector<std::string>> handleToStringsMap;

  public:
	TResourceManager() { Init(); }
	~TResourceManager() { Clear(); }

	void Init();
	void Clear();
	void Reset();

	inline void clear() { Clear(); }
	inline auto begin() -> decltype(resources.begin()) { return resources.begin(); }
	inline auto end() -> decltype(resources.end()) { return resources.end(); }
	inline auto cbegin() const -> decltype(resources.cbegin()) { return resources.cbegin(); }
	inline auto cend() const -> decltype(resources.cend()) { return resources.cend(); }
	inline auto empty() const -> decltype(resources.empty()) { return resources.empty(); }
	inline auto size() const -> decltype(resources.size()) { return resources.size(); }

	void DeleteUnnamedResources();

	void SetDefaultValue(const T &value);

	unsigned Create();
	unsigned Create(const std::string &name);
	void Delete(unsigned handle);
	void Delete(const std::string &name);

	bool IsAHandle(unsigned handle) const;
	bool IsAHandle(const std::string &name) const;
	const std::string &GetNameFromHandle(unsigned handle);
	const std::vector<std::string> &GetNamesFromHandle(unsigned handle);
	unsigned GetHandleFromName(const std::string &name) const;
	unsigned MapNameToHandle(const std::string &name, unsigned handle);
	void RemoveName(const std::string &name);

	T &operator[](unsigned handle);
	T &operator[](const std::string &name);

	const T &operator[](unsigned handle) const;
	const T &operator[](const std::string &name) const;
};

template <typename T>
class TSimpleResourceManager
{
  public:
	TSimpleResourceManager();
	~TSimpleResourceManager();

	// Erases all elements and clears the available and allocated lists.
	void Clear();

	// Frees allocated elements and makes them available
	void Reset();

	// Adds a resource to the available list
	void Add(T &resource);

	// Pulls a resource from the available list
	T &Create();

	// Moves a resource from the allocated list to the available list
	void Delete(T &resource);

  private:
	T defaultObject;
	std::vector<T> available;
	std::vector<T> allocated;
};

template <typename T>
TSimpleResourceManager<T>::TSimpleResourceManager()
{
	allocated.clear();
	available.clear();
}

template <typename T>
TSimpleResourceManager<T>::~TSimpleResourceManager()
{
	allocated.clear();
	available.clear();
}

template <typename T>
void TSimpleResourceManager<T>::Clear()
{
	allocated.clear();
	available.clear();
}

template <typename T>
void TSimpleResourceManager<T>::Reset()
{
	available.reserve(available.size() + allocated.size());
	copy(allocated.begin(), allocated.end(), back_inserter(available));
	allocated.clear();
}

template <typename T>
void TSimpleResourceManager<T>::Add(T &resource)
{
	// std::vector<T>::iterator it;
	auto it = find(allocated.begin(), allocated.end(), resource);
	if (it != allocated.end())
	{
		// it's already allocated, so do nothing
		return;
	}
	it = find(available.begin(), available.end(), resource);
	if (it == available.end())
	{
		available.push_back(resource);
	}
}

template <typename T>
T &TSimpleResourceManager<T>::Create()
{
	if (available.empty())
		return defaultObject;

	T &item = available.back();
	available.pop_back();
	allocated.push_back(item);

	return item;
}

template <typename T>
void TSimpleResourceManager<T>::Delete(T &item)
{
	auto it = find(allocated.begin(), allocated.end(), item);
	if (it != allocated.end())
	{
		available.push_back(item);
		allocated.erase(it);
	}
}

template <typename T>
void TResourceManager<T>::Init()
{
	Clear();
}

template <typename T>
void TResourceManager<T>::Clear()
{
	resources.clear();
	handleToStringsMap.clear();
	stringToHandleMap.clear();
	allocatedResourceHandles.clear();
	availableResourceHandles.clear();
	largestHandle = 0;
}

template <typename T>
void TResourceManager<T>::Reset()
{
	availableResourceHandles.reserve(availableResourceHandles.size() + allocatedResourceHandles.size());
	move(allocatedResourceHandles.begin(), allocatedResourceHandles.end(), back_inserter(availableResourceHandles));
	availableResourceHandles.clear();
	stringToHandleMap.clear();
	handleToStringsMap.clear();
}

template <typename T>
void TResourceManager<T>::DeleteUnnamedResources()
{
	std::vector<unsigned> resourcesToRemove;
	for (auto it = resources.begin(); it != resources.end(); it++)
	{
		unsigned handle = it->first;
		if (handleToStringsMap[handle].empty())
			resourcesToRemove.push_back(handle);
	}
	for (auto it = resourcesToRemove.begin(); it != resourcesToRemove.end(); it++)
	{
		Delete(*it);
	}
}

template <typename T>
void TResourceManager<T>::SetDefaultValue(const T &value)
{
	defaultObject = value;
}

template <typename T>
bool TResourceManager<T>::IsAHandle(unsigned handle) const
{
	if (handle == 0)
		return false;

	auto handleObjIt = resources.find(handle);
	if (handleObjIt == resources.end())
		return false;

	return true;
}

template <typename T>
bool TResourceManager<T>::IsAHandle(const std::string &name) const
{
	unsigned handle = GetHandleFromName(name);
	return IsAHandle(handle);
}

template <typename T>
const std::string &TResourceManager<T>::GetNameFromHandle(unsigned handle)
{
	auto it = handleToStringsMap.find(handle);
	if (it == handleToStringsMap.end())
		return BlankString;
	if (it->second.empty())
		return BlankString;
	return it->second[0];
}

template <typename T>
const std::vector<std::string> &TResourceManager<T>::GetNamesFromHandle(unsigned handle)
{
	auto it = handleToStringsMap.find(handle);
	if (it == handleToStringsMap.end())
		return std::vector<std::string>();
	return it->second;
}

template <typename T>
unsigned TResourceManager<T>::Create()
{
	// are we out of available ones?
	if (availableResourceHandles.empty())
	{
		largestHandle++;
		allocatedResourceHandles.push_back(largestHandle);
		resources.emplace(largestHandle, T());
		return largestHandle;
	}

	unsigned handle = availableResourceHandles.back();
	availableResourceHandles.pop_back();
	allocatedResourceHandles.push_back(handle);
	resources.emplace(largestHandle, T());
	return handle;
}

template <typename T>
unsigned TResourceManager<T>::Create(const std::string &name)
{
	// Don't create duplicate handles
	unsigned handle = GetHandleFromName(name);
	if (handle != 0)
		return handle;

	handle = Create();
	MapNameToHandle(name, handle);
	return handle;
}

template <typename T>
void TResourceManager<T>::Delete(unsigned handle)
{
	// does this object even exist?
	auto handleObjIt = resources.find(handle);
	if (handleObjIt == resources.end())
		return;

	// remove actual object
	resources.erase(handle);

	// remove string names allocated to refer to this handle
	std::vector<std::string> &strings = handleToStringsMap[handle];
	for (auto it = strings.begin(); it != strings.end(); it++)
	{
		stringToHandleMap.erase(*it);
	}

	// remove the handle from the strings map
	handleToStringsMap.erase(handle);

	// move handle to the available handles list
	remove(allocatedResourceHandles.begin(), allocatedResourceHandles.end(), handle);
	availableResourceHandles.push_back(handle);
}

template <typename T>
void TResourceManager<T>::Delete(const std::string &name)
{
	unsigned handle = GetHandleFromName(name);
	Delete(handle);
}

template <typename T>
unsigned TResourceManager<T>::GetHandleFromName(const std::string &name) const
{
	auto it = stringToHandleMap.find(name);
	if (it == stringToHandleMap.end())
		return 0;
	return it->second;
}

template <typename T>
unsigned TResourceManager<T>::MapNameToHandle(const std::string &name, unsigned handle)
{
	// do nothing if this is the default object
	if (handle == 0)
		return 0;

	RemoveName(name);

	// okay, go ahead and add it to the appropriate handle's string list
	handleToStringsMap[handle].push_back(name);

	// make this string point to this handle
	stringToHandleMap[name] = handle;

	return handle;
}

template <typename T>
void TResourceManager<T>::RemoveName(const std::string &name)
{
	// is this string already mapped to a handle?
	auto it = stringToHandleMap.find(name);
	if (it != stringToHandleMap.end())
	{
		unsigned handle = it->second;
		std::vector<std::string> &container = handleToStringsMap[handle];
		// yep! so remove previous reference to it
		auto pos = find(container.begin(), container.end(), name);
		if (pos != container.end())
			container.erase(pos);
		stringToHandleMap.erase(it);
	}
}

template <typename T>
T &TResourceManager<T>::operator[](unsigned handle)
{
	// does this object even exist?
	auto handleObjIt = resources.find(handle);
	if (handleObjIt == resources.end())
		return defaultObject;
	return handleObjIt->second;
}

template <typename T>
T &TResourceManager<T>::operator[](const std::string &name)
{
	unsigned handle = GetHandleFromName(name);
	if (handle == 0)
	{
		handle = Create();
		MapNameToHandle(name, handle);
	}
	return this->operator[](handle);
}

template <typename T>
const T &TResourceManager<T>::operator[](unsigned handle) const
{
	// does this object even exist?
	auto handleObjIt = resources.find(handle);
	if (handleObjIt == resources.end())
		return defaultObject;
	return handleObjIt->second;
}

template <typename T>
const T &TResourceManager<T>::operator[](const std::string &name) const
{
	unsigned handle = GetHandleFromName(name);
	if (handle == 0)
	{
		return defaultObject;
	}
	return this->operator[](handle);
}
} // namespace Fluxions

#endif
