#ifndef FLUXIONS_RENDERER_UNIFORM_BLOCKS_HPP
#define FLUXIONS_RENDERER_UNIFORM_BLOCKS_HPP

#include <fluxions_stdcxx.hpp>
#include <fluxions_opengl.hpp>
#include <fluxions_simple_scene_graph_base.hpp>

namespace Fluxions
{
	constexpr int MaxLights = 8;
	constexpr int MaxMaterials = 32;

	enum class UbBindings {
		Unknown = 0,
		Enviroment = 1,
		Camera = 2,
		Materials = 3,
		DirToLights = 4,
		PointLights = 5,
		AnisoLights = 6,
		MaxBindingPoints,
		UserBindingPoints
	};


	class RendererUniformBlock {
	public:
		RendererUniformBlock(GLsizeiptr size, const GLvoid* data);
		virtual ~RendererUniformBlock();

		// Maps the buffer object to new data using GL_DYNAMIC_DRAW
		virtual void update();

		// Returns "UnknownUniformBlock" if this object is not derived
		virtual const char* uniformBlockName() const { return "UnknownUniformBlock"; }

		// Returns 0 if this object has no binding (due to a badly implemented class)
		virtual unsigned uniformBinding() const { return (unsigned)UbBindings::Unknown; }

		void use(GLuint program);

		bool usable() const { return blockIndex_ != GL_INVALID_INDEX; }

		static void SetProgramBindingPoints(GLuint program);
	private:
		GLuint blockBinding_{ GL_INVALID_INDEX };
		GLuint blockIndex_{ GL_INVALID_INDEX };
		GLsizeiptr size_{ 0 };
		const GLvoid* data_{ nullptr };
		std::shared_ptr<unsigned> ubo_;
	};


	class UbEnviromentBlock : public RendererUniformBlock {
	public:
		UbEnviromentBlock() :
			RendererUniformBlock((GLsizeiptr)sizeof(uniforms), (const GLvoid*)&uniforms) {}

		const char* uniformBlockName() const override { return "EnvironmentBlock"; }
		unsigned uniformBinding() const override { return (unsigned)UbBindings::Enviroment; }

		BaseEnvironment uniforms;
	};


	class UbCameraBlock : public RendererUniformBlock {
	public:
		UbCameraBlock() :
			RendererUniformBlock((GLsizeiptr)sizeof(uniforms), (const GLvoid*)&uniforms) {}

		const char* uniformBlockName() const override { return "CameraBlock"; }
		unsigned uniformBinding() const override { return (unsigned)UbBindings::Camera; }

		struct UNIFORMS {
			Matrix4f ProjectionMatrix;
			Matrix4f CameraMatrix;
		} uniforms;
	};


	class UbMaterialBlock : public RendererUniformBlock {
	public:
		UbMaterialBlock() :
			RendererUniformBlock((GLsizeiptr)sizeof(uniforms), (const GLvoid*)&uniforms[0]) {}

		const char* uniformBlockName() const override { return "MaterialBlock"; }
		unsigned uniformBinding() const override { return (unsigned)UbBindings::Materials; }

		BaseMaterial uniforms[MaxMaterials];
	};


	class UbDirToLightBlock : public RendererUniformBlock {
	public:
		UbDirToLightBlock() :
			RendererUniformBlock((GLsizeiptr)sizeof(uniforms), (const GLvoid*)&uniforms[0]) {}

		const char* uniformBlockName() const override { return "DirToLightBlock"; }
		unsigned uniformBinding() const override { return (unsigned)UbBindings::DirToLights; }

		BaseDirToLight uniforms[MaxLights];
	};


	class UbPointLightBlock : public RendererUniformBlock {
	public:
		UbPointLightBlock() :
			RendererUniformBlock((GLsizeiptr)sizeof(uniforms), (const GLvoid*)&uniforms[0]) {}

		const char* uniformBlockName() const override { return "PointLightBlock"; }
		unsigned uniformBinding() const override { return (unsigned)UbBindings::PointLights; }

		BasePointLight uniforms[MaxLights];
	};


	class UbAnisoLightBlock : public RendererUniformBlock {
	public:
		UbAnisoLightBlock() :
			RendererUniformBlock((GLsizeiptr)sizeof(uniforms), (const GLvoid*)&uniforms[0]) {}

		const char* uniformBlockName() const override { return "AnisoLightBlock"; }
		unsigned uniformBinding() const override { return (unsigned)UbBindings::AnisoLights; }

		BaseAnisoLight uniforms[MaxLights];
	};
}

#endif