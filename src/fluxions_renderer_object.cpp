#include "pch.hpp"
#include <fluxions_renderer_object.hpp>
#include <hatchetfish.hpp>

namespace Fluxions
{
	RendererObject::RendererObject() {}

	RendererObject::~RendererObject() {
		HFLOGDEBUG("Destroying renderer object '%s' [%s]",
				   name_.c_str(),
				   type());
		kill();
	}

	void RendererObject::init(const std::string& newname,
							  RendererObject* pparent) {
		name_ = newname;
		setParent(pparent);
		HFLOGDEBUG("Initting renderer object '%s' [%s] with parent '%s'",
				   name_.c_str(),
				   type(),
				   parent_ ? parent_->name_.c_str() : "(noparent)");
		initialized_ = true;
	}

	void RendererObject::kill() {
		HFLOGDEBUG("Killing renderer object '%s' [%s] with parent '%s'",
				   name_.c_str(),
				   type(),
				   parent_ ? parent_->name_.c_str() : "(noparent)");
		initialized_ = false;
	}

	void RendererObject::reset() {
		if (!initialized_) {
			HFLOGERROR("Renderer object never initialized");
			return;
		}

		HFLOGDEBUG("Resetting renderer object '%s' [%s]",
				   name_.c_str(),
				   type());
		kill();
		init(name());
	}

	const char* RendererObject::type() const {
		return "RendererObject";
	}

	void RendererObject::setParent(RendererObject* pparent) {
		if (!pparent) {
			HFLOGWARN("This object '%s' does not have a parent", name());
		}
		parent_ = pparent;
	}

	RendererObject* RendererObject::parent() {
		return parent_;
	}

	const RendererObject* RendererObject::parent() const {
		return parent_;
	}
}