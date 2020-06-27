#ifndef FLUXIONS_RENDERER_CONTEXT_HPP
#define FLUXIONS_RENDERER_CONTEXT_HPP

#include <fluxions_renderer_base.hpp>
#include <fluxions_renderer_object.hpp>
#include <fluxions_renderer_config.hpp>
#include <fluxions_renderer_sampler.hpp>
#include <fluxions_renderer_texture_object.hpp>
#include <fluxions_renderer_gles30.hpp>
#include <fluxions_renderer_texture_2d.hpp>

namespace Fluxions {
	class RendererContext : public RendererObject {
	public:
		RendererContext();
		virtual ~RendererContext() override;

		void init(const std::string& name, RendererObject* pparent = nullptr) override;
		void kill() override;
		const char* type() const override;
		const char* keyword() const override { return "rendercontext"; }
		void set_default_parameters() override;
		void invalidate_caches() override;

		// Resizes matching WxH render configurations to new sizes
		void resize(int width, int height);

		std::vector<std::string> paths;
		bool findPath(std::string& path);

		void loadShaders();
		void loadTextures();
		void loadMaps(const std::map<std::string, std::string>& maps_paths);
		void makeFramebuffers();

		// Render Configurations
		bool loadConfig(const std::string& filename);

		using Quadrant = Recti::Quadrant;

		virtual void setDeferredRect(const Recti& rect);
		virtual void setDeferredSplit(const Vector2i& position);
		virtual void setDeferredSplitPercent(const Vector2f& pct);
		virtual const Vector2i& getDeferredSplitPoint() const;
		virtual const Recti& getDeferredRect() const;
		virtual void renderDeferred(Quadrant quadrant = Recti::UpperLeft);

		// DEBUG FLAGS/DEFAULTS FOR EXTERNAL PURPOSES /////////////////
		bool debugClearScreen{ false };
		int defaultScreenWidth{ 1280 };
		int defaultScreenHeight{ 720 };
		///////////////////////////////////////////////////////////////

		//std::vector<RendererProgramPtr> Programs;
		//std::map<std::string, RenderConfigPtr> RenderConfigs;
		//std::map<std::string, Texture> Textures;
		//std::map<std::string, Sampler> Samplers;
		//std::map<std::string, Framebuffer> Framebuffers;
		//std::map<std::string, Renderbuffer> Renderbuffers;

		Df::VariableList vars;

		std::map<std::string, RendererConfig> rendererConfigs;
		std::map<std::string, RendererProgram> programs;
		std::map<std::string, RendererFramebuffer> fbos;
		std::map<std::string, RendererSampler> samplers;
		//std::map<std::string, RendererTextureObject> textures;
		std::map<std::string, RendererTexture2D> texture2Ds;
		std::map<std::string, RendererTextureCube> textureCubes;
		std::map<std::string, RendererGLES30> renderers;

		RendererProgram* getRendererProgram(const std::string& name);
		RendererConfig* getRendererConfig(const std::string& name);
		RendererGLES30* getRenderer(const std::string& name);
		RendererFramebuffer* getFramebuffer(const std::string& name);

		//RendererProgramPtr findProgram(const std::string& renderConfigName, const std::string& name);

	private:
		TSimpleResourceManager<GLuint> textureUnits;
		void initTexUnits();
		void killTexUnits();

	public:
		GLuint getTexUnit();
		void freeTexUnit(GLuint id);

	private:
		//RenderConfigPtr gbufferConfig = nullptr;
		//RendererProgramPtr gbufferProgram = nullptr;
		//RenderConfigPtr deferredConfigs[4] = { nullptr, nullptr, nullptr, nullptr };
		//RendererProgramPtr deferredPrograms[4] = { nullptr, nullptr, nullptr, nullptr };
		Vector2i deferredSplitPoint{ 0, 0 };
		Recti deferredRect{ 0, 0, 0, 0 };

		std::string name_{ "noname RendererContext" };

		std::string basepath;
		//RenderConfigPtr pcur_renderconfig = nullptr;
		//RenderConfig::ProgramPtr pcur_program = nullptr;
		//std::string cur_sampler;
		//std::string cur_texture;
		//Framebuffer* pcur_fbo = nullptr;

		RendererConfig* pcurRendererConfig{ nullptr };
		RendererProgram* pcurProgram{ nullptr };
		RendererGLES30* pcurRenderer{ nullptr };
		RendererSampler* pcurSampler{ nullptr };
		RendererTexture2D* pcurTexture2D{ nullptr };
		RendererTextureCube* pcurTextureCube{ nullptr };
		RendererFramebuffer* pcurFBO{ nullptr };

		bool _addPath(const std::string& path);

		//virtual bool new_renderconfig(const std::string& name);
		//virtual bool use_renderconfig(const std::string& name);
		//virtual bool new_program(const std::string& name);
		//virtual bool use_program(const std::string& name);

		// k_replaceVars changes all of the @XYZ variables
		bool k_replaceVars(Df::TokenVector& args);

		// k_check returns if the command contains count items
		// and the appropriate command
		bool k_check(const Df::TokenVector& args,
					 int count, const std::string& cmd);

		// k_sval returns true if the argument is a string and
		// within the range of args
		bool k_sval(const Df::TokenVector& args,
					int i, std::string& x);

		// k_ival returns true if the argument is an int
		// and within the range of args
		bool k_ival(const Df::TokenVector& args,
					int i, int& ival);

		// k_dval returns true if the argument is a double
		// and within the range of args
		bool k_dval(const Df::TokenVector& args,
					int i, double& dval);

		// k_ivalue returns the integer value of the argument
		// or 0 otherwise
		int k_ivalue(const Df::TokenVector& args, int i);

		// k_dvalue returns the double value of the argument
		// or 0.0 otherwise
		double k_dvalue(const Df::TokenVector& args, int i);

		// k_glenum returns the GLenum value of the string
		// argument or 0 otherwise
		bool k_glenum(const Df::TokenVector& args,
					  int i, GLenum& enumValue);

		bool k_debug(const Df::TokenVector& args);
		bool k_renderconfig(const Df::TokenVector& args);
		bool k_path(const Df::TokenVector& args);
		bool k_program(const Df::TokenVector& args);
		bool k_vertshader(const Df::TokenVector& args);
		bool k_fragshader(const Df::TokenVector& args);
		bool k_geomshader(const Df::TokenVector& args);
		bool k_vertattrib(const Df::TokenVector& args);
		bool k_sampler(const Df::TokenVector& args);
		bool k_fbo(const Df::TokenVector& args);
		bool k_texture2D(const Df::TokenVector& args);
		bool k_textureCube(const Df::TokenVector& args);
		bool k_renderer(const Df::TokenVector& args);
	};

} // namespace Fluxions

#endif
