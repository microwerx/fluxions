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
#ifndef FLUXIONS_ES2_STATE_INFO_HPP
#define FLUXIONS_ES2_STATE_INFO_HPP

#include <vector>
#include <memory>

#include <fluxions_config.hpp>
#include <fluxions_opengl.hpp>

namespace Fluxions
{

namespace ES2
{
//template <typename T>
//union TVector3 {
//	struct
//	{
//		T x, y, z;
//	};
//	struct
//	{
//		T r, g, b;
//	};
//	T ptr[3];
//
//	TVector3() : x(0), y(0), z(0)
//	{
//	}
//
//	TVector3(T xVal, T yVal, T zVal) : x(xVal), y(yVal), z(zVal)
//	{
//	}
//};

template <typename T>
struct TVector3
{
	T x, y, z;
	TVector3() : x(0), y(0), z(0)
	{
	}

	TVector3(T xVal, T yVal, T zVal) : x(xVal), y(yVal), z(zVal)
	{
	}
};

//template <typename T>
//union TVector4 {
//	struct
//	{
//		T x, y, z, w;
//	};
//	struct
//	{
//		T r, g, b, a;
//	};
//	T ptr[4];
//
//	TVector4() : x(0), y(0), z(0), w(0)
//	{
//	}
//
//	TVector4(T xVal, T yVal, T zVal, T wVal)
//		: x(xVal), y(yVal), z(zVal), w(wVal)
//	{
//	}
//};

template <typename T>
struct TVector4
{
	T x, y, z, w;

	TVector4() : x(0), y(0), z(0), w(0)
	{
	}

	TVector4(T xVal, T yVal, T zVal, T wVal)
		: x(xVal), y(yVal), z(zVal), w(wVal)
	{
	}
};

using Vector3f = TVector3<GLfloat>;
using Vector3d = TVector3<GLdouble>;
using Vector4f = TVector4<GLfloat>;
using Vector4d = TVector4<GLdouble>;

struct RasterStateInfo
{
	float lineWidth;
	GLenum frontFaceDir;
	GLenum cullFaceMode;
	GLfloat polygonOffsetFactor;
	GLfloat polygonOffsetUnits;

	RasterStateInfo() : lineWidth(1.0),
						frontFaceDir(GL_CCW),
						cullFaceMode(GL_NONE),
						polygonOffsetFactor(0.0),
						polygonOffsetUnits(0.0)
	{
	}

	void Apply()
	{
		// Rasterizer State (RS)
		glLineWidth(lineWidth);
		glFrontFace(frontFaceDir);
		glCullFace(cullFaceMode);

		if (cullFaceMode == GL_NONE)
			glDisable(GL_CULL_FACE);
		else
			glEnable(GL_CULL_FACE);

		glPolygonOffset(polygonOffsetFactor, polygonOffsetUnits);

		if (polygonOffsetFactor == 0.0 && polygonOffsetUnits == 0.0)
			glDisable(GL_POLYGON_OFFSET_FILL);
		else
			glEnable(GL_POLYGON_OFFSET_FILL);
	}
};

struct ViewportStateInfo
{
	GLint x;
	GLint y;
	GLsizei width;
	GLsizei height;
	GLclampf znear;
	GLclampf zfar;

	ViewportStateInfo() : x(0), y(0), width(0), height(0), znear(0.0), zfar(1.0)
	{
	}

	void Apply()
	{
		glDepthRangef(znear, zfar);
		glViewport(x, y, width, height);
	}
};

struct ScissorStateInfo
{
	bool enabled;
	GLint x;
	GLint y;
	GLsizei width;
	GLsizei height;

	ScissorStateInfo() : enabled(false),
						 x(0),
						 y(0),
						 width(0),
						 height(0)
	{
	}

	void Apply()
	{
		glScissor(x, y, width, height);
		if (enabled)
			glEnable(GL_SCISSOR_TEST);
		else
			glDisable(GL_SCISSOR_TEST);
	}
};

struct ColorBlendStateInfo
{
	bool enabled;
	GLenum blendEquationModeRGB;
	GLenum blendEquationModeAlpha;
	GLenum blendFuncSourceRGB;
	GLenum blendFuncSourceAlpha;
	GLenum blendFuncDestRGB;
	GLenum blendFuncDestAlpha;
	Color4f blendColor;

	ColorBlendStateInfo() : enabled(false),
							blendEquationModeRGB(GL_FUNC_ADD),
							blendEquationModeAlpha(GL_FUNC_ADD),
							blendFuncSourceRGB(GL_ONE),
							blendFuncSourceAlpha(GL_ONE),
							blendFuncDestRGB(GL_ZERO),
							blendFuncDestAlpha(GL_ZERO),
							blendColor(0, 0, 0, 0)
	{
	}

	void Apply()
	{
		glBlendEquationSeparate(
			blendEquationModeRGB,
			blendEquationModeAlpha);
		glBlendFuncSeparate(
			blendFuncSourceRGB,
			blendFuncDestRGB,
			blendFuncSourceAlpha,
			blendFuncDestAlpha);
		glBlendColor(
			blendColor.r,
			blendColor.g,
			blendColor.b,
			blendColor.a);

		if (enabled)
			glEnable(GL_BLEND);
		else
			glDisable(GL_BLEND);
	}
};

struct StencilOpStateInfo
{
	GLenum sfailOp;
	GLenum dpfailOp;
	GLenum dppassOp;
	GLenum func;
	GLint ref;
	GLuint mask;

	StencilOpStateInfo() : sfailOp(GL_KEEP),
						   dpfailOp(GL_KEEP),
						   dppassOp(GL_KEEP),
						   func(GL_ALWAYS),
						   ref(0),
						   mask(0xFFFFFFFF)
	{
	}

	void Apply(GLenum face)
	{
		glStencilFuncSeparate(face, func, ref, mask);
		glStencilOpSeparate(face, sfailOp, dpfailOp, dppassOp);
	}
};

struct DepthStencilStateInfo
{
	bool depthTestEnable;
	bool depthWriteEnable;
	GLenum depthFunc;
	GLfloat minDepth;
	GLfloat maxDepth;
	bool stencilTestEnable;
	GLuint readMask;
	GLuint writeMaskFront;
	GLuint writeMaskBack;
	StencilOpStateInfo front;
	StencilOpStateInfo back;

	void Apply()
	{
		glDepthMask(depthWriteEnable ? GL_TRUE : GL_FALSE);
		glDepthRangef(minDepth, maxDepth);
		glDepthFunc(depthFunc);

		if (depthTestEnable)
			glEnable(GL_DEPTH_TEST);
		else
			glDisable(GL_DEPTH_TEST);

		glStencilMaskSeparate(GL_FRONT, writeMaskFront);
		glStencilMaskSeparate(GL_BACK, writeMaskBack);
		front.Apply(GL_FRONT);
		back.Apply(GL_BACK);

		if (stencilTestEnable)
			glEnable(GL_STENCIL_TEST);
		else
			glDisable(GL_STENCIL_TEST);
	}
};

struct MsaaStateInfo
{
	bool sampleAlphaToCoverageEnabled;
	bool sampleCoverageEnabled;
	float sampleCoverageValue;
	bool sampleCoverageInvert;

	MsaaStateInfo() : sampleAlphaToCoverageEnabled(false),
					  sampleCoverageEnabled(false),
					  sampleCoverageValue(1.0),
					  sampleCoverageInvert(false)
	{
	}

	void Apply()
	{
		glSampleCoverage(sampleCoverageValue, sampleCoverageInvert ? GL_TRUE : GL_FALSE);
		if (sampleAlphaToCoverageEnabled)
			glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);
		else
			glDisable(GL_SAMPLE_ALPHA_TO_COVERAGE);
		if (sampleCoverageEnabled)
			glEnable(GL_SAMPLE_COVERAGE);
		else
			glDisable(GL_SAMPLE_COVERAGE);
	}
};

struct DynamicPipelineState
{
	ES2::RasterStateInfo rasterState;
	ES2::ViewportStateInfo viewportState;
	ES2::ScissorStateInfo scissorState;
	ES2::ColorBlendStateInfo blendState;
	ES2::MsaaStateInfo msaaState;

	DynamicPipelineState()
	{
	}

	void Apply()
	{
		rasterState.Apply();
		viewportState.Apply();
		scissorState.Apply();
		blendState.Apply();
		msaaState.Apply();
	}
};

namespace Pipeline
{
struct InputAssemblerStateInfo
{
	// This is not really configurable for OpenGL ES 2.0.
	// This is really here for Mantle/Vulkan potential usage.
	// I think this is in reference to glPolygonMode to draw points, lines, or triangles.

	InputAssemblerStateInfo()
	{
	}

	void Apply()
	{
	}
};

struct ShaderStateInfo
{
	GLint shaderObject;

	ShaderStateInfo() : shaderObject(0)
	{
	}

	void Apply()
	{
	}
};

struct ProgramStateInfo
{
	GLint programObject;

	ProgramStateInfo() : programObject(0)
	{
	}

	void Apply()
	{
		glUseProgram(programObject);
	}
};

struct RasterizerStateInfo
{
	// This is to configure whether clipping happens or not,
	// so this probably won't get used.

	RasterizerStateInfo()
	{
	}

	void Apply()
	{
	}
};

struct DepthStencilBufferStateInfo
{
	GLenum format;

	DepthStencilBufferStateInfo() : format(GL_DEPTH_COMPONENT16)
	{
	}

	void Apply()
	{
	}
};

struct ColorBufferStateInfo
{
	GLenum format;

	ColorBufferStateInfo() : format(GL_RGB8)
	{
	}

	void Apply()
	{
	}
};
} // namespace Pipeline

struct StaticPipelineState
{
	static const int MAX_COLOR_BUFFERS = 1;
	Pipeline::InputAssemblerStateInfo IA;
	Pipeline::ShaderStateInfo VS;
	Pipeline::RasterizerStateInfo RS;
	Pipeline::ShaderStateInfo PS;
	Pipeline::DepthStencilBufferStateInfo DB;
	Pipeline::ColorBufferStateInfo CB[MAX_COLOR_BUFFERS];

	Pipeline::ProgramStateInfo program;

	StaticPipelineState()
	{
	}

	void Create()
	{
	}

	void Delete()
	{
	}

	void Apply()
	{
		IA.Apply();
		VS.Apply();
		RS.Apply();
		PS.Apply();
		DB.Apply();
		for (int i = 0; i < StaticPipelineState::MAX_COLOR_BUFFERS; i++)
			CB[i].Apply();
		program.Apply();
	}
};

// things you can do in a command buffer:
// BEGIN
// END
// BIND PIPELINE
// BIND STATE OBJECT
// BIND DESCRIPTOR SET
// BIND DYNAMIC MEMORY VIEW
// BIND INDEX DATA
// BIND COLOR AND DEPTH STENCIL TARGETS
// DRAW VERTICES
// DRAW INDEXED VERTICES
// COPY MEMORY
// COPY IMAGES
// COPY MEMORY TO IMAGES (glDrawPixels?)
// COPY IMAGE TO MEMORY (glReadPixels?)
// UPDATE MEMORY
// FILL MEMORY (glClear?)
// CLEAR COLOR IMAGE
// CLEAR DEPTH IMAGE
// CLEAR STENCIL IMAGE

// ORDER OF OPERATIONS:
// INITIALIZE RESOURCES (upload memory blobs for indices, vertices, textures, etc.)
//
// BIND PIPELINE
// BIND MUTABLE STATE
// BIND DESCRIPTORS
// BIND RENDER PASS
// DRAW

//class Renderer
//{
//public:
//	vector<shared_ptr<Resource>> resources;			// i.e. used to store memory buffers and images
//	vector<shared_ptr<Shader>> shaders;				// i.e. used to store the shaders
//	vector<shared_ptr<StateSet>> stateSets;			// i.e. what's the depth test, etc. set to?
//	vector<shared_ptr<UniformSet>> uniformSets;		// i.e. what are the uniforms set to for the shader?
//	vector<shared_ptr<DescriptorSet>> descriptorSets;	// i.e. where are the textures bound?, parameters set to?
//	vector<shared_ptr<Pipeline>> pipelines;			// i.e. what's the currently bound program? default states? default descriptors? default uniforms?
//	vector<shared_ptr<CommandBuffer>> commandBuffers;	// i.e. a list of commands to execute
//	vector<shared_ptr<RenderPass>> renderPasses;		// i.e. framebuffer objects and a list of commands to execute
//};

//RenderPass.Begin();
//myPipeline.Use();
//myDescriptors.Use();
//myStateSet.Use();
//myUniforms.Use();
//myGeometry.Draw();
//myOtherPipeline.Use();
//myOtherGeometry.Draw();
//myOtherDescriptors.Use();
//myOtherGeometry.Draw();
//RenderPass.End();

} // namespace ES2

} // namespace Fluxions

#endif
