#include "pch.hpp"
#include <fluxions_renderer_config.hpp>

namespace Fluxions
{
	RendererConfig::RendererConfig() {}

	RendererConfig::~RendererConfig() {
		kill();
	}

	void RendererConfig::init(const std::string& name) {
		RendererObject::init(name);
	}

	void RendererConfig::kill() {
		shaderProgram.reset();
		zShaderProgram.reset();

		RendererObject::kill();
	}

	GLbitfield RendererConfig::getClearBits() const {
		GLbitfield clearBits = 0;
		if (clearDepthBuffer) clearBits |= GL_DEPTH_BUFFER_BIT;
		if (clearColorBuffer) clearBits |= GL_COLOR_BUFFER_BIT;
		if (clearStencilBuffer) clearBits |= GL_STENCIL_BUFFER_BIT;
		return clearBits;
	}

	bool RendererConfig::check() const {
		if (!zShaderProgram && !shaderProgram) {
			return false;
		}

		if (zShaderProgram && !zShaderProgram->isLinked()) {
			return false;
		}

		if (shaderProgram && !shaderProgram->isLinked()) {
			return false;
		}

		return true;
	}
}