#include "pch.hpp"
#include <fluxions_renderer_object.hpp>
#include <hatchetfish.hpp>

namespace Fluxions
{
	RendererObject::RendererObject() {}

	RendererObject::~RendererObject() {
		if (initialized_) kill();
	}

	void RendererObject::init(const std::string& newname, RendererObject* pparent) {
		if (initialized_) kill();
		name_ = newname;
		setParent(pparent);
		HFLOGDEBUG("Initializing renderer object (%s) --> [%s] '%s'",
				   parent_ ? parent_->name() : "noparent",
				   type(), 
				   name());
		initialized_ = true;
	}

	void RendererObject::kill() {
		if (!initialized_) {
			HFLOGERROR("NOT KILLING renderer object (%s) --> [%s] '%s'",
					   parent_ ? parent_->name() : "noparent",
					   type(),
					   name());
			return;
		}
		HFLOGDEBUG("Killing renderer object (%s) --> [%s] '%s'",
				   parent_ ? parent_->name() : "noparent",
				   type(),
				   name());
		initialized_ = false;
	}

	void RendererObject::reset() {
		HFLOGDEBUG("Resetting renderer object (%s) --> [%s] '%s'",
				   parent_ ? parent_->name() : "noparent",
				   type(),
				   name());
		kill();
		init(name(), parent_);
	}

	const char* RendererObject::type() const {
		return "RendererObject";
	}

	void RendererObject::setParent(RendererObject* pparent) {
		if (!pparent) {
			HFLOGWARN("Renderer object [%s] '%s' does not have a parent", type(), name());
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