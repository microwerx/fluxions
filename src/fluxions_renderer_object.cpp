#include "pch.hpp"
#include <fluxions_renderer_object.hpp>
#include <hatchetfish.hpp>

namespace Fluxions {
	RendererObject::RendererObject() {
	}

	RendererObject::~RendererObject() {
		HFLOGDEBUG("Destroying renderer object '%s'", name_.c_str());
		kill();
	}

	void RendererObject::init(const std::string& name) {
		name_ = name;
		HFLOGDEBUG("Initting renderer object '%s'", name_.c_str());
	}

	void RendererObject::kill() {
		HFLOGDEBUG("Killing renderer object '%s'", name_.c_str());
	}

	void RendererObject::reset() {
		HFLOGDEBUG("Resetting renderer object '%s'", name_.c_str());
		kill();
		init(getName());
	}
}