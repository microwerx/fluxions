#include "fluxions_renderer_pch.hpp"
#include <fluxions_renderer_config.hpp>

namespace Fluxions {
	RendererConfig::RendererConfig() {}

	RendererConfig::~RendererConfig() {
		kill();
	}

	void RendererConfig::init(const std::string& name,
							  RendererObject* pparent) {
		RendererObject::init(name, pparent);
	}

	void RendererConfig::kill() {
		//shaderProgram.reset();
		//zShaderProgram.reset();
		readFBOs.clear();
		writeFBO = nullptr;
		textures.clear();
		rc_program = "";
		rc_program_ptr = nullptr;

		RendererObject::kill();
	}

	const char* RendererConfig::type() const {
		return "RendererConfig";
	}

	void RendererConfig::invalidate_caches() {}

	GLbitfield RendererConfig::getClearBits() const {
		GLbitfield clearBits = 0;
		if (clearDepthBuffer) clearBits |= GL_DEPTH_BUFFER_BIT;
		if (clearColorBuffer) clearBits |= GL_COLOR_BUFFER_BIT;
		if (clearStencilBuffer) clearBits |= GL_STENCIL_BUFFER_BIT;
		return clearBits;
	}

	bool RendererConfig::check() const {
		//if (!zShaderProgram && !shaderProgram) {
		//	return false;
		//}

		//if (zShaderProgram && !zShaderProgram->isLinked()) {
		//	return false;
		//}

		//if (shaderProgram && !shaderProgram->isLinked()) {
		//	return false;
		//}

		return true;
	}

	void RendererConfig::updateViewport() {
		viewportProjectionMatrix.LoadIdentity();
		if (viewportFovInDegrees > 0) {
			viewportProjectionMatrix.PerspectiveY(viewportFovInDegrees,
												  viewportRect.aspectRatiof(),
												  viewportZNear,
												  viewportZFar);
		}
		else {
			viewportProjectionMatrix.Ortho2D(0, (float)viewportRect.w,
											 0, (float)viewportRect.h);
		}
	}
}
