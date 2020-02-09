#ifndef FLUXIONS_RESOURCE_MANAGER_HPP
#define FLUXIONS_RESOURCE_MANAGER_HPP

#include <fluxions_stdcxx.hpp>

namespace Fluxions {
	extern const std::string BlankString;

	template <typename T>
	class TResourceManager {
	private:
		std::map<unsigned, T> resources_;
		T defaultObject_;

		unsigned largestHandle_;
		std::vector<unsigned> availableResourceHandles_;
		std::vector<unsigned> allocatedResourceHandles_;

		std::map<std::string, unsigned> stringToHandleMap_;
		std::map<unsigned, std::vector<std::string>> handleToStringsMap_;

	public:
		using iterator = typename std::map<unsigned, T>::iterator;
		using const_iterator = typename std::map<unsigned, T>::const_iterator;
		using value_type = typename std::map<unsigned, T>::value_type;
		using mapped_type = typename std::map<unsigned, T>::mapped_type;
		using key_type = typename std::map<unsigned, T>::key_type;

		unsigned lastId{ 0 };

		TResourceManager() { init(); }
		~TResourceManager() { clear(); }

		void init() noexcept;
		void clear() noexcept;
		void reset() noexcept;

		inline iterator begin() noexcept { return resources_.begin(); }
		inline iterator end() noexcept { return resources_.end(); }
		inline const_iterator begin() const noexcept { return resources_.begin(); }
		inline const_iterator end() const noexcept { return resources_.end(); }
		inline bool empty() const noexcept { return resources_.empty(); }
		inline unsigned size() const noexcept { return (unsigned)resources_.size(); }

		unsigned count(const std::string& name) const noexcept;
		unsigned count(unsigned id) const noexcept;

		void eraseUnnamedResources();

		void setDefaultValue(const T& value);

		unsigned create();
		unsigned create(const std::string& name);
		void erase(unsigned handle);
		void erase(const std::string& name);

		bool isAHandle(unsigned handle) const;
		bool isAHandle(const std::string& name) const;
		const std::string& getNameFromHandle(unsigned handle) const;
		const std::vector<std::string> getNamesFromHandle(unsigned handle) const;
		unsigned getHandleFromName(const std::string& name) const;
		unsigned mapNameToHandle(const std::string& name, unsigned handle);
		void removeName(const std::string& name);

		T& operator[](unsigned handle);
		T& operator[](const std::string& name);

		const T& operator[](unsigned handle) const;
		const T& operator[](const std::string& name) const;

		T* getPtr(unsigned handle) {
			auto handleObjIt = resources_.find(handle);
			if (handleObjIt == resources_.end())
				return nullptr;
			return &handleObjIt->second;
		}

		T* getPtr(const std::string& name) {
			unsigned handle = getHandleFromName(name);
			if (handle == 0)
				return nullptr;
			return getPtr(handle);
		}
	};

	template <typename T>
	class TSimpleResourceManager {
	public:
		TSimpleResourceManager();
		~TSimpleResourceManager();

		// Erases all elements and clears the available and allocated lists.
		void clear();

		// Frees allocated elements and makes them available
		void reset();

		// Adds a resource to the available list
		void add(T& resource);

		// Pulls a resource from the available list
		T& create();

		// Moves a resource from the allocated list to the available list
		void erase(T& resource);

	private:
		T defaultObject_;
		std::vector<T> available_;
		std::vector<T> allocated_;
	};

	template <typename T>
	TSimpleResourceManager<T>::TSimpleResourceManager() {
		allocated_.clear();
		available_.clear();
	}

	template <typename T>
	TSimpleResourceManager<T>::~TSimpleResourceManager() {
		allocated_.clear();
		available_.clear();
	}

	template <typename T>
	void TSimpleResourceManager<T>::clear() {
		allocated_.clear();
		available_.clear();
	}

	template <typename T>
	void TSimpleResourceManager<T>::reset() {
		available_.reserve(available_.size() + allocated_.size());
		copy(allocated_.begin(), allocated_.end(), back_inserter(available_));
		allocated_.clear();
	}

	template <typename T>
	void TSimpleResourceManager<T>::add(T& resource) {
		// std::vector<T>::iterator it;
		auto it = find(allocated_.begin(), allocated_.end(), resource);
		if (it != allocated_.end()) {
			// it's already allocated, so do nothing
			return;
		}
		it = find(available_.begin(), available_.end(), resource);
		if (it == available_.end()) {
			available_.push_back(resource);
		}
	}

	template <typename T>
	T& TSimpleResourceManager<T>::create() {
		if (available_.empty())
			return defaultObject_;

		T& item = available_.back();
		available_.pop_back();
		allocated_.push_back(item);

		return item;
	}

	template <typename T>
	void TSimpleResourceManager<T>::erase(T& item) {
		auto it = find(allocated_.begin(), allocated_.end(), item);
		if (it != allocated_.end()) {
			available_.push_back(item);
			allocated_.erase(it);
		}
	}

	template <typename T>
	void TResourceManager<T>::init() noexcept {
		clear();
	}

	template <typename T>
	void TResourceManager<T>::clear() noexcept {
		resources_.clear();
		handleToStringsMap_.clear();
		stringToHandleMap_.clear();
		allocatedResourceHandles_.clear();
		availableResourceHandles_.clear();
		largestHandle_ = 0;
	}

	template <typename T>
	void TResourceManager<T>::reset() noexcept {
		availableResourceHandles_.reserve(availableResourceHandles_.size() + allocatedResourceHandles_.size());
		move(allocatedResourceHandles_.begin(), allocatedResourceHandles_.end(), back_inserter(availableResourceHandles_));
		availableResourceHandles_.clear();
		stringToHandleMap_.clear();
		handleToStringsMap_.clear();
	}


	template <typename T>
	unsigned TResourceManager<T>::count(const std::string& id) const noexcept {
		return (unsigned)stringToHandleMap_.count(id);
	}

	template <typename T>
	unsigned TResourceManager<T>::count(unsigned id) const noexcept {
		return (unsigned)handleToStringsMap_.count(id);
	}

	template <typename T>
	void TResourceManager<T>::eraseUnnamedResources() {
		std::vector<unsigned> resourcesToRemove;
		for (auto& [handle, r] : resources_) {
			//unsigned handle = it->first;
			if (handleToStringsMap_[handle].empty())
				resourcesToRemove.push_back(handle);
		}

		for (auto& resourceToRemove : resourcesToRemove) {
			erase(resourceToRemove);
		}
	}

	template <typename T>
	void TResourceManager<T>::setDefaultValue(const T& value) {
		defaultObject_ = value;
	}

	template <typename T>
	bool TResourceManager<T>::isAHandle(unsigned handle) const {
		return (handle != 0) && (resources_.count(handle) != 0);
	}

	template <typename T>
	bool TResourceManager<T>::isAHandle(const std::string& name) const {
		unsigned handle = getHandleFromName(name);
		return isAHandle(handle);
	}

	template <typename T>
	const std::string& TResourceManager<T>::getNameFromHandle(unsigned handle) const {
		auto it = handleToStringsMap_.find(handle);
		if (it == handleToStringsMap_.end())
			return BlankString;
		if (it->second.empty())
			return BlankString;
		return it->second[0];
	}

	template <typename T>
	const std::vector<std::string> TResourceManager<T>::getNamesFromHandle(unsigned handle) const {
		auto it = handleToStringsMap_.find(handle);
		if (it == handleToStringsMap_.end())
			return std::vector<std::string>();
		return it->second;
	}

	template <typename T>
	unsigned TResourceManager<T>::create() {
		// are we out of available ones?
		if (availableResourceHandles_.empty()) {
			largestHandle_++;
			allocatedResourceHandles_.push_back(largestHandle_);
			resources_.emplace(largestHandle_, T());
			return largestHandle_;
		}

		unsigned handle = availableResourceHandles_.back();
		availableResourceHandles_.pop_back();
		allocatedResourceHandles_.push_back(handle);
		resources_.emplace(largestHandle_, T());
		return handle;
	}

	template <typename T>
	unsigned TResourceManager<T>::create(const std::string& name) {
		// Don't create duplicate handles
		unsigned handle = getHandleFromName(name);
		if (handle != 0)
			return handle;

		handle = create();
		mapNameToHandle(name, handle);
		return handle;
	}

	template <typename T>
	void TResourceManager<T>::erase(unsigned handle) {
		// does this object even exist?
		if (resources_.count(handle) == 0) return;

		// remove actual object
		resources_.erase(handle);

		// remove string names allocated to refer to this handle
		const std::vector<std::string>& strings = handleToStringsMap_[handle];
		for (const std::string& name : strings) {
			stringToHandleMap_.erase(name);
		}

		// remove the handle from the strings map
		handleToStringsMap_.erase(handle);

		// move handle to the available handles list
		allocatedResourceHandles_.erase(remove(allocatedResourceHandles_.begin(),
											   allocatedResourceHandles_.end(),
											   handle),
										allocatedResourceHandles_.end());

		availableResourceHandles_.push_back(handle);
	}

	template <typename T>
	void TResourceManager<T>::erase(const std::string& name) {
		unsigned handle = getHandleFromName(name);
		erase(handle);
	}

	template <typename T>
	unsigned TResourceManager<T>::getHandleFromName(const std::string& name) const {
		auto it = stringToHandleMap_.find(name);
		if (it == stringToHandleMap_.end())
			return 0;
		return it->second;
	}

	template <typename T>
	unsigned TResourceManager<T>::mapNameToHandle(const std::string& name, unsigned handle) {
		// do nothing if this is the default object
		if (handle == 0)
			return 0;

		removeName(name);

		// okay, go ahead and add it to the appropriate handle's string list
		handleToStringsMap_[handle].push_back(name);

		// make this string point to this handle
		stringToHandleMap_[name] = handle;

		return handle;
	}

	template <typename T>
	void TResourceManager<T>::removeName(const std::string& name) {
		// is this string already mapped to a handle?
		auto it = stringToHandleMap_.find(name);
		if (it != stringToHandleMap_.end()) {
			unsigned handle = it->second;
			std::vector<std::string>& container = handleToStringsMap_[handle];
			// yep! so remove previous reference to it
			auto pos = find(container.begin(), container.end(), name);
			if (pos != container.end())
				container.erase(pos);
			stringToHandleMap_.erase(it);
		}
	}

	template <typename T>
	T& TResourceManager<T>::operator[](unsigned handle) {
		// does this object even exist?
		auto handleObjIt = resources_.find(handle);
		if (handleObjIt == resources_.end())
			return defaultObject_;
		return handleObjIt->second;
	}

	template <typename T>
	T& TResourceManager<T>::operator[](const std::string& name) {
		unsigned handle = getHandleFromName(name);
		if (handle == 0) {
			handle = create();
			mapNameToHandle(name, handle);
		}
		return this->operator[](handle);
	}

	template <typename T>
	const T& TResourceManager<T>::operator[](unsigned handle) const {
		// does this object even exist?
		auto handleObjIt = resources_.find(handle);
		if (handleObjIt == resources_.end())
			return defaultObject_;
		return handleObjIt->second;
	}

	template <typename T>
	const T& TResourceManager<T>::operator[](const std::string& name) const {
		unsigned handle = getHandleFromName(name);
		if (handle == 0) {
			return defaultObject_;
		}
		return this->operator[](handle);
	}
} // namespace Fluxions

#endif
