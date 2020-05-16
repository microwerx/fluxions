#include "fluxions_renderer_pch.hpp"
#include <fluxions_fileio_iostream.hpp>
#include <fluxions_renderer_object.hpp>
#include <hatchetfish.hpp>

namespace Fluxions {
	namespace {
		bool ro_debugging = false;
	}

	RendererObject::RendererObject() {}

	RendererObject::~RendererObject() {
		if (initialized_) kill();
	}

	void RendererObject::init(const std::string& newname, RendererObject* pparent) {
		if (initialized_) kill();
		name_ = newname;
		setParent(pparent);
		if (ro_debugging) HFLOGDEBUG("Initializing renderer object (%s) --> [%s] '%s'",
									 parent_ ? parent_->name() : "noparent",
									 type(),
									 name());
		initialized_ = true;
	}

	void RendererObject::kill() {
		if (!initialized_) {
			if (ro_debugging) HFLOGERROR("NOT KILLING renderer object (%s) --> [%s] '%s'",
										 parent_ ? parent_->name() : "noparent",
										 type(),
										 name());
			return;
		}
		if (ro_debugging) HFLOGDEBUG("Killing renderer object (%s) --> [%s] '%s'",
									 parent_ ? parent_->name() : "noparent",
									 type(),
									 name());
		initialized_ = false;
	}

	void RendererObject::reset() {
		if (ro_debugging) HFLOGDEBUG("Resetting renderer object (%s) --> [%s] '%s'",
									 parent_ ? parent_->name() : "noparent",
									 type(),
									 name());
		kill();
		init(name(), parent_);
	}

	void RendererObject::set_default_parameters() {}

	void RendererObject::invalidate_caches() {}

	const char* RendererObject::type() const {
		return "RendererObject";
	}

	const char* RendererObject::status() const {
		return "no status";
	}

	bool RendererObject::usable() const {
		return usable_;
	}

	bool RendererObject::read(const std::string& keyword, std::istream& istr) {
		if (keyword == this->keyword()) {
			setName(ReadString(istr));
			return true;
		}
		return false;
	}

	bool RendererObject::write(std::ostream& ostr) const {
		ostr << keyword() << " ";
		WriteString(ostr, name()) << "\n";
		return true;
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
