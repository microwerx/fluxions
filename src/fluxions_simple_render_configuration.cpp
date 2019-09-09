// SSPHH/Fluxions/Unicornfish/Viperfish/Hatchetfish/Sunfish/Damselfish/GLUT Extensions
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
#include "pch.h"
#include <fluxions_simple_scene_graph.hpp>


namespace Fluxions
{
	GLbitfield SimpleRenderConfiguration::GetClearBits() const
	{
		GLbitfield clearBits = 0;
		if (clearDepthBuffer) clearBits |= GL_DEPTH_BUFFER_BIT;
		if (clearColorBuffer) clearBits |= GL_COLOR_BUFFER_BIT;
		if (clearStencilBuffer) clearBits |= GL_STENCIL_BUFFER_BIT;
		return clearBits;
	}

	bool SimpleRenderConfiguration::check() const
	{
		if (!zShaderProgram && !shaderProgram) {
			return false;
		}

		if (zShaderProgram && !zShaderProgram->IsLinked()) {
			return false;
		}

		if (shaderProgram && !shaderProgram->IsLinked()) {
			return false;
		}
	}
}
