// SSPHH/Fluxions/Unicornfish/Viperfish/Hatchetfish/Sunfish/Damselfish/GLUT Extensions
// Copyright (C) 2017-2019 Jonathan Metzgar
// All rights reserved.
//
// This program is free software : you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program.If not, see <https://www.gnu.org/licenses/>.
//
// For any other type of licensing, please contact me at jmetzgar@outlook.com
#ifndef FLUXIONS_MULTIPLE_RENDER_TARGETS_HPP
#define FLUXIONS_MULTIPLE_RENDER_TARGETS_HPP

#include <string>
#include <fluxions_opengl.hpp>
#include <fluxions_gte.hpp>

namespace Fluxions
{
	struct RenderTarget
	{
		GLuint object = 0;
		GLenum target = 0;
		GLenum attachment = 0;
		GLenum internalformat = 0;
		GLsizei width = 0;
		GLsizei height = 0;
		GLint levels = 0;
		GLenum currentCubeFace = 0;
		bool useMultisamples = false;
		GLsizei samples = 0;
		GLenum readBufferTarget = 0;
		bool generateMipmaps = false;
		Matrix4f projectionViewMatrix;
		GLuint unit = 0;
		GLuint sampler = 0;
	};

	class FramebufferObject
	{
		GLint lastBoundUnit = 0;

	public:
		FramebufferObject();
		~FramebufferObject();

		void Delete();
		void DeleteBuffers();
		bool Make();
		void UseForWriting();
		void UseForReading();
		void Use();
		void GenerateMipmaps();
		void RestoreGLState();
		void BindTextures(int unit);
		void UnbindTextures();
		void SetDimensions(GLsizei newWidth, GLsizei newHeight);
		void SetMultisamples(GLsizei newSamples, bool newUseMultisamples);
		void SetProjectionViewMatrix(const Matrix4f& M);
		void SetAllProjectionViewMatrices(const Matrix4f& M);
		void SetCurrentCubeFace(GLenum face);
		void AddRenderbuffer(GLenum attachment, GLenum internalformat);
		void AddTexture2D(GLenum attachment, GLenum target, GLenum internalformat, bool generateMipmaps);
		void AddTextureCubeMap(GLenum attachment, GLenum target, GLenum internalformat, bool generateMipmaps);
		void Enable(GLenum target) noexcept { enables.push_back(target); }

		bool dirty = false;
		GLuint fbo = 0;
		GLint width = 0;
		GLint height = 0;
		GLsizei samples = 0;
		bool useMultisamples = false;
		GLenum internalformat = 0;
		GLenum currentCubeFace = 0;
		Matrix4f projectionViewMatrix;
		std::vector<std::pair<GLenum, RenderTarget>> renderTargets;
		std::vector<GLenum> enables;
	};

	std::string GetFramebufferStatusAsString(GLenum status);
} // namespace Fluxions

#endif
