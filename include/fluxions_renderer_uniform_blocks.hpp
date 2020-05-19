#ifndef FLUXIONS_RENDERER_UNIFORM_BLOCKS_HPP
#define FLUXIONS_RENDERER_UNIFORM_BLOCKS_HPP

#include <fluxions_ssg_base.hpp>

namespace Fluxions {
	constexpr int MaxLights = 8;
	constexpr int MaxMaterials = 32;

	enum class UbBindings {
		Unknown = 0,
		Enviroment = 1,
		Camera = 2,
		Materials = 3,
		DirToLights = 4,
		DirtoLightShadowMatrices = 5,
		PointLights = 6,
		PointLightShadowMatrices = 7,
		AnisoLights = 8,
		AnisoLightShadowMatrices = 9,
		MaxBindingPoints,
		UserBindingPoints
	};


	class RendererUniformBlock {
	public:
		RendererUniformBlock(GLsizeiptr size, const GLvoid* data);
		virtual ~RendererUniformBlock();

		// Maps the buffer object to new data using GL_DYNAMIC_DRAW
		virtual void update();

		// Causes the uniform block to be rebound to the program
		virtual void invalidate_cache();

		// Returns "UnknownUniformBlock" if this object is not derived
		virtual const char* uniformBlockName() const { return "UnknownUniformBlock"; }

		// Returns 0 if this object has no binding (due to a badly implemented class)
		virtual unsigned uniformBinding() const { return (unsigned)UbBindings::Unknown; }

		// Returns the number of uniforms in this block
		virtual unsigned size() const { return 0; }

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

		unsigned size() const override { return 1; }
		BaseEnvironment uniforms;
	};


	class UbCameraBlock : public RendererUniformBlock {
	public:
		UbCameraBlock() :
			RendererUniformBlock((GLsizeiptr)sizeof(ublock), (const GLvoid*)&ublock) {}

		const char* uniformBlockName() const override { return "CameraBlock"; }
		unsigned uniformBinding() const override { return (unsigned)UbBindings::Camera; }

		unsigned size() const override { return 1; }

		BaseCamera ublock;
	};


	class UbMaterialBlock : public RendererUniformBlock {
	public:
		UbMaterialBlock() :
			RendererUniformBlock((GLsizeiptr)sizeof(uniforms), (const GLvoid*)&uniforms[0]) {}

		const char* uniformBlockName() const override { return "MaterialBlock"; }
		unsigned uniformBinding() const override { return (unsigned)UbBindings::Materials; }

		unsigned size() const override { return MaxMaterials; }
		BaseMaterial uniforms[MaxMaterials];
	};


	class UbDirToLightBlock : public RendererUniformBlock {
	public:
		UbDirToLightBlock() :
			RendererUniformBlock((GLsizeiptr)sizeof(uniforms), (const GLvoid*)&uniforms[0]) {}

		const char* uniformBlockName() const override { return "DirToLightBlock"; }
		unsigned uniformBinding() const override { return (unsigned)UbBindings::DirToLights; }

		unsigned size() const override { return MaxLights; }
		BaseDirToLight uniforms[MaxLights];
	};


	class UbDirToShadowMatrixBlock : public RendererUniformBlock {
	public:
		UbDirToShadowMatrixBlock() :
			RendererUniformBlock((GLsizeiptr)sizeof(uniforms), (const GLvoid*)&uniforms[0]) {}

		const char* uniformBlockName() const override { return "DirToShadowMatrixBlock"; }
		unsigned uniformBinding() const override { return (unsigned)UbBindings::DirtoLightShadowMatrices; }

		unsigned size() const override { return MaxLights; }
		Matrix4f uniforms[MaxLights];
	};


	class UbPointLightBlock : public RendererUniformBlock {
	public:
		UbPointLightBlock() :
			RendererUniformBlock((GLsizeiptr)sizeof(uniforms), (const GLvoid*)&uniforms[0]) {}

		const char* uniformBlockName() const override { return "PointLightBlock"; }
		unsigned uniformBinding() const override { return (unsigned)UbBindings::PointLights; }

		unsigned size() const override { return MaxLights; }
		BasePointLight uniforms[MaxLights];
	};


	class UbPointShadowMatrixBlock : public RendererUniformBlock {
	public:
		UbPointShadowMatrixBlock() :
			RendererUniformBlock((GLsizeiptr)sizeof(uniforms), (const GLvoid*)&uniforms[0]) {}

		const char* uniformBlockName() const override { return "PointLightShadowMatrixBlock"; }
		unsigned uniformBinding() const override { return (unsigned)UbBindings::AnisoLightShadowMatrices; }

		unsigned size() const override { return MaxLights; }
		Matrix4f uniforms[MaxLights];
	};


	class UbAnisoLightBlock : public RendererUniformBlock {
	public:
		UbAnisoLightBlock() :
			RendererUniformBlock((GLsizeiptr)sizeof(uniforms), (const GLvoid*)&uniforms[0]) {}

		const char* uniformBlockName() const override { return "AnisoLightBlock"; }
		unsigned uniformBinding() const override { return (unsigned)UbBindings::AnisoLights; }

		unsigned size() const override { return MaxLights; }
		BaseAnisoLight uniforms[MaxLights];
	};


	class UbAnisoShadowMatrixBlock : public RendererUniformBlock {
	public:
		UbAnisoShadowMatrixBlock() :
			RendererUniformBlock((GLsizeiptr)sizeof(uniforms), (const GLvoid*)&uniforms[0]) {}

		const char* uniformBlockName() const override { return "AnisoLightShadowMatrixBlock"; }
		unsigned uniformBinding() const override { return (unsigned)UbBindings::AnisoLightShadowMatrices; }

		unsigned size() const override { return MaxLights; }
		Matrix4f uniforms[MaxLights];
	};
}

#endif
