// SSPHH/Fluxions/Unicornfish/Viperfish/Hatchetfish/Sunfish/KASL/GLUT Extensions
// Copyright (C) 2017 Jonathan Metzgar
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

#include <fluxions_stdcxx.hpp>
#include "fluxions_opengl.hpp"
#include "fluxions_gte.hpp"

namespace Fluxions
{
	struct RenderTarget
	{
		GLuint object;
		GLenum target;
		GLenum attachment;
		GLenum internalformat;
		GLsizei width;
		GLsizei height;
		GLint levels;
		GLenum currentCubeFace;
		bool useMultisamples;
		GLsizei samples;
		GLenum readBufferTarget;
		bool generateMipmaps;
		Matrix4f projectionViewMatrix;
		GLuint unit;
		GLuint sampler;
	};

	class FramebufferObject
	{
		GLint lastBoundUnit;
	public:
		FramebufferObject();
		~FramebufferObject();

		void Delete	();
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
		void SetProjectionViewMatrix(const Matrix4f &M);
		void SetAllProjectionViewMatrices(const Matrix4f &M);
		void SetCurrentCubeFace(GLenum face);
		void AddRenderbuffer(GLenum attachment, GLenum internalformat);
		void AddTexture2D(GLenum attachment, GLenum target, GLenum internalformat, bool generateMipmaps);
		void AddTextureCubeMap(GLenum attachment, GLenum target, GLenum internalformat, bool generateMipmaps);
		void Enable(GLenum target) { enables.push_back(target); }

		bool dirty;
		GLuint fbo;
		GLint width;
		GLint height;
		GLsizei samples;
		bool useMultisamples;
		GLenum internalformat;
		GLenum currentCubeFace;
		Matrix4f projectionViewMatrix;
		vector<pair<GLenum, RenderTarget>> renderTargets;
		vector<GLenum> enables;
	};

	string GetFramebufferStatusAsString(GLenum status);
}

#endif
