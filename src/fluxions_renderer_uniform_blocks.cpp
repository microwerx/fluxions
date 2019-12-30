#include "pch.hpp"
#include <fluxions_renderer_uniform_blocks.hpp>
#include <fluxions_draw_gl1gl2.hpp>

namespace Fluxions
{
	const char* UniformBlockNames[(unsigned)UbBindings::MaxBindingPoints] = {
		"UnknownBlock",
		"EnviroBlock",
		"CameraBlock",
		"MaterialBlock",
		"DirToLightBlock",
		"PointLightBlock",
		"AnisoLightBlock"
	};

	RendererUniformBlock::RendererUniformBlock(GLsizeiptr size, const GLvoid* data) :
		size_(size), data_(data), ubo_(std::make_shared<unsigned>(0)) {}

	RendererUniformBlock::~RendererUniformBlock() {
		if (ubo_.use_count() == 1) {
			FxDeleteBuffer(&(*ubo_));
		}
	}

	void RendererUniformBlock::update() {
		FxCreateBuffer(GL_UNIFORM_BUFFER, *ubo_, size_, data_, GL_DYNAMIC_DRAW);
		blockBinding_ = uniformBinding();
		glBindBufferBase(GL_UNIFORM_BUFFER, blockBinding_, *ubo_);
		blockIndex_ = GL_INVALID_INDEX;
	}

	void RendererUniformBlock::use(GLuint program) {
		if (blockIndex_ == GL_INVALID_INDEX) {
			blockIndex_ = glGetUniformBlockIndex(program, uniformBlockName());
			return;
		}
		glUniformBlockBinding(program, blockIndex_, blockBinding_);
	}

	void RendererUniformBlock::SetProgramBindingPoints(GLuint program) {
		for (unsigned i = 0; i < (unsigned)UbBindings::MaxBindingPoints; i++) {
			GLuint blockIndex = glGetUniformBlockIndex(program, UniformBlockNames[i]);
			if (blockIndex == GL_INVALID_INDEX) continue;
			glUniformBlockBinding(program, blockIndex, i);
		}
	}
}