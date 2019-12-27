#ifndef FLUXIONS_BASE_OBJECT_HPP
#define FLUXIONS_BASE_OBJECT_HPP

#include <fluxions_stdcxx.hpp>

namespace Fluxions
{
	class IBaseObject {
	public:
		IBaseObject() {}
		virtual ~IBaseObject() {};

		const char* name() const { return name_.c_str(); };		
		virtual const char* keyword() const = 0;
		virtual const char* type() const = 0;
		virtual const char* status() const = 0;
		virtual bool read(const std::string& keyword, std::istream& istr) = 0;
		virtual bool write(std::ostream& ostr) const = 0;
	protected:
		std::string name_;
		void setName(const std::string& name) { name_ = name; }
		void setName(const char* name) { name_ = name; }
		friend class SimpleSceneGraph;
	};
}

#endif