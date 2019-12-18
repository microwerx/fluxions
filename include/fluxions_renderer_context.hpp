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
#ifndef FLUXIONS_RENDERER_HPP
#define FLUXIONS_RENDERER_HPP

#include <fluxions_renderer_object.hpp>
#include <fluxions_renderer_config.hpp>
#include <damselfish.hpp>
#include <fluxions_gte.hpp>
#include <fluxions_renderer_sampler_object.hpp>
#include <fluxions_renderer_texture_object.hpp>
#include <fluxions_renderer_gles30.hpp>

namespace Fluxions
{
	class RendererContext : public RendererObject
	{
	public:
		struct RenderConfig
		{
			struct Program
			{
				struct VertexAttrib
				{
					int index = 0;
					std::string name;

					VertexAttrib() {}
					VertexAttrib(int _index, const std::string& _name) : index(_index), name(_name) {}
				};

				std::string name;
				std::string vertshader;
				std::string geomshader;
				std::string fragshader;
				std::vector<VertexAttrib> vertex_attribs;

				RendererProgramPtr program;

				Program() {}
				Program(const std::string& _name) : name(_name) {}
				~Program() { program.reset(); }
			};

			using ProgramPtr = Program*;

			std::string name;
			std::vector<Program> programs;

			RenderConfig() {};
			RenderConfig(const std::string& _name) : name(_name) {}
			~RenderConfig() { programs.clear(); }
		};

		using RenderConfigPtr = std::shared_ptr<RenderConfig>;

		struct Texture
		{
			std::string name;
			std::vector<std::pair<GLenum, std::string>> files;
			GLenum target = 0;
			int level = 0;
			GLenum internalformat = 0;
			int width = 0;
			int height = 0;
			GLenum format = 0;
			GLenum type = 0;
			bool genmipmap = true;
		};

		struct Sampler
		{
			std::string name;
			std::vector<std::pair<GLenum, GLenum>> parameters;
		};

		struct Renderbuffer
		{
			std::string name;
			GLenum internalformat = 0;
			int width = 0;
			int height = 0;
			int samples = 0;
		};

		struct Framebuffer
		{
			std::string name;
			// attachment, renderbuffer
			std::vector<std::tuple<GLenum, std::string>> renderbuffers;
			// attachment, target, texture_, level
			std::vector<std::tuple<GLenum, GLenum, std::string, int>> textures;
		};

		RendererContext();
		virtual ~RendererContext() override;

		void init(const std::string& name) override;
		void kill() override;
		void reset() override;

		void loadShaders();

		// Render Configurations
		bool loadConfig(const std::string& filename);
		bool isConfig(const std::string& name) const { return RenderConfigs.count(name); }
		bool useConfig(const std::string& name) { return use_renderconfig(name); }
		RenderConfigPtr getConfig(const std::string& name) { return isConfig(name) ? RenderConfigs[name] : nullptr; }

		// G-Buffer and Deferred Renderers
		virtual bool setGbufferRenderConfig(const std::string& name) {
			gbufferConfig = getConfig(name);
			return gbufferConfig != nullptr;
		}
		virtual RenderConfigPtr getGbufferRenderConfig() { return gbufferConfig; }
		virtual RendererProgramPtr getGbufferProgram() { return gbufferProgram; }
		virtual void renderGbuffer();

		using Quadrant = Recti::Quadrant;

		bool setDeferredRenderConfig(Quadrant quadrant,
			const std::string& rc,
			const std::string& program) {
			deferredConfigs[quadrant] = getConfig(rc);
			deferredPrograms[quadrant] = findProgram(rc, program);
			return deferredConfigs[quadrant] != nullptr && deferredPrograms[quadrant] != nullptr;
		}

		RenderConfigPtr getDeferredRenderConfig(Quadrant quadrant) { return deferredConfigs[quadrant]; }
		RendererProgramPtr getDeferredProgram(Quadrant quadrant) { return deferredPrograms[quadrant]; }

		virtual void setDeferredRect(const Recti& rect) {
			deferredRect = rect;
			deferredSplitPoint = deferredRect.Clamp(deferredSplitPoint);
		}

		virtual void setDeferredSplit(const Vector2i& position) { deferredSplitPoint = deferredRect.Clamp(position); }
		virtual void setDeferredSplitPercent(const Vector2f& pct) { deferredSplitPoint = deferredRect.percent(pct); }
		virtual const Vector2i& getDeferredSplitPoint() const { return deferredSplitPoint; }
		virtual const Recti& getDeferredRect() const { return deferredRect; }
		virtual void renderDeferred(Quadrant quadrant = Recti::UpperLeft);

		std::vector<RendererProgramPtr> Programs;
		std::map<std::string, RenderConfigPtr> RenderConfigs;
		std::map<std::string, Texture> Textures;
		std::map<std::string, Sampler> Samplers;
		std::map<std::string, Framebuffer> Framebuffers;
		std::map<std::string, Renderbuffer> Renderbuffers;
		std::vector<std::string> Paths;
		Df::VariableList VarList;

		std::map<std::string, RendererConfig> rendererConfigs;
		std::map<std::string, RendererFramebufferObject> fbos;
		std::map<std::string, RendererSamplerObject> samplers;
		std::map<std::string, RendererTextureObject> textures;
		std::map<std::string, RendererTextureCube> textureCubes;
		std::map<std::string, RendererGLES30> renderers;

		RendererConfig* getRendererConfig(const std::string& name);
		RendererGLES30* getRenderer(const std::string& name);

		RendererProgramPtr findProgram(const std::string& renderConfigName, const std::string& name);

	private:
		TSimpleResourceManager<GLuint> textureUnits;
		void initTexUnits();
		void killTexUnits();

	public:
		GLuint getTexUnit() { return textureUnits.Create(); }
		void freeTexUnit(GLuint id) { textureUnits.Delete(id); }

	private:
		RenderConfigPtr gbufferConfig = nullptr;
		RendererProgramPtr gbufferProgram = nullptr;
		RenderConfigPtr deferredConfigs[4] = { nullptr, nullptr, nullptr, nullptr };
		RendererProgramPtr deferredPrograms[4] = { nullptr, nullptr, nullptr, nullptr };
		Vector2i deferredSplitPoint = Vector2i(0, 0);
		Recti deferredRect = Recti(0, 0, 0, 0);

		std::string name_{ "noname RendererContext" };

		std::string basepath;
		RenderConfigPtr pcur_renderconfig = nullptr;
		RenderConfig::ProgramPtr pcur_program = nullptr;
		std::string cur_sampler;
		std::string cur_texture;
		Framebuffer* pcur_fbo = nullptr;

		virtual bool new_renderconfig(const std::string& name);
		virtual bool use_renderconfig(const std::string& name);
		virtual bool new_program(const std::string& name);
		virtual bool use_program(const std::string& name);

		bool k_check(const Df::TokenVector& args,
			int count, const std::string& cmd);
		bool k_strori(const Df::TokenVector& args,
			int i, std::string& x);

		bool k_renderconfig(const Df::TokenVector& args);
		bool k_path(const Df::TokenVector& args);
		bool k_program(const Df::TokenVector& args);
		bool k_vertshader(const Df::TokenVector& args);
		bool k_fragshader(const Df::TokenVector& args);
		bool k_geomshader(const Df::TokenVector& args);
		bool k_vertattrib(const Df::TokenVector& args);
		bool k_sampler(const Df::TokenVector& args);
		bool k_texture(const Df::TokenVector& args);
		bool k_fbo(const Df::TokenVector& args);
		bool k_renderbuffer(const Df::TokenVector& args);
	};

} // namespace Fluxions

#endif
