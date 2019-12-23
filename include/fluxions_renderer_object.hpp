#ifndef FLUXIONS_RENDERER_OBJECT_HPP
#define FLUXIONS_RENDERER_OBJECT_HPP

#include <fluxions_stdcxx.hpp>
#include <fluxions_opengl.hpp>

namespace Fluxions {
	class RendererObject {
	public:
		RendererObject();
		virtual ~RendererObject();

		// RendererObject::init() should be called before other initialization code
		virtual void init(const std::string& newname, RendererObject* pparent = nullptr);

		// RendererObject::kill() should be called after other deinitialization code
		virtual void kill();

		// RendererObject::reset() should only be called if this object has been initialized
		virtual void reset();

		// RendererObject::set_default_parameters() can do a "soft reset" for class parameters
		virtual void set_default_parameters();

		// type() returns an RTTI string of the type of object
		virtual const char* type() const;

		// status() returns some message about the current state of the object
		virtual const char* status() const;

		// usable() returns true or false (default is true) if the current object is usable for rendering
		virtual bool usable() const;

		const char* name() const noexcept { return name_.c_str(); };
		const bool initialized() const noexcept { return initialized_; }

		void setParent(RendererObject* pparent);
		RendererObject* parent();
		const RendererObject* parent() const;
	protected:
		bool usable_{ false };
	private:
		bool initialized_ = false;
		std::string name_;
		RendererObject* parent_{ nullptr };
	};
}

#endif
