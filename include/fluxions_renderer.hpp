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

#include <map>
#include <vector>
#include <string>
#include <damselfish.hpp>
#include <fluxions_opengl.hpp>
#include <fluxions_gte.hpp>
#include <fluxions_simple_renderer.hpp>

namespace Fluxions
{
	// SCENE INTERFACE

	class IScene
	{
	public:
		virtual bool Load(const char* filename) = 0;
		virtual bool Save(const char* filename) = 0;

		struct Light
		{
		};

		struct Camera
		{
		};

		struct Mesh
		{
		};

		struct MtlLib
		{
		};

		std::vector<Camera> Cameras;
		std::vector<Mesh> Meshes;
		std::vector<MtlLib> MtlLibs;
	};

	// RENDERER INTERFACE

	class IRenderer
	{
	public:
		virtual bool UploadCompileLink() = 0;
		virtual void Render(const IScene* pScene) = 0;
		virtual void Reset() = 0;

		virtual bool IsConfig(const std::string& name) const = 0;
		virtual bool UseConfig(const std::string& name) = 0;
		virtual bool LoadConfig(const std::string& filename) = 0;
	};

	class Renderer : public IRenderer
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

				SimpleProgramPtr program;

				Program() {}
				Program(const std::string& _name) : name(_name) {}
			};

			std::string name;
			std::vector<Program> programs;

			RenderConfig() {};
			RenderConfig(const std::string& _name) : name(_name) {}
		};

		using RenderConfigPtr = RenderConfig *;

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

		Renderer();
		virtual ~Renderer();

		virtual bool UploadCompileLink();
		virtual void Render(const IScene* pScene);
		virtual void Reset();

		virtual void LoadShaders();

		// Render Configurations
		virtual bool LoadConfig(const std::string& filename);
		virtual bool IsConfig(const std::string& name) const { return RenderConfigs.find(name) != RenderConfigs.end(); }
		virtual bool UseConfig(const std::string& name) { return use_renderconfig(name); }
		virtual RenderConfigPtr GetConfig(const std::string& name) { return IsConfig(name) ? &RenderConfigs.find(name)->second : nullptr; }

		// G-Buffer and Deferred Renderers
		virtual bool SetGbufferRenderConfig(const std::string& name)
		{
			gbufferConfig = GetConfig(name);
			return gbufferConfig != nullptr;
		}
		virtual RenderConfigPtr GetGbufferRenderConfig() { return gbufferConfig; }
		virtual SimpleProgramPtr GetGbufferProgram() { return gbufferProgram; }
		virtual void RenderGbuffer();

		using Quadrant = Recti::Quadrant;

		virtual bool SetDeferredRenderConfig(Quadrant quadrant,
											 const std::string& rc,
											 const std::string& program)
		{
			deferredConfigs[quadrant] = GetConfig(rc);
			deferredPrograms[quadrant] = FindProgram(rc, program);
			return deferredConfigs[quadrant] != nullptr && deferredPrograms[quadrant] != nullptr;
		}

		virtual RenderConfigPtr GetDeferredRenderConfig(Quadrant quadrant) { return deferredConfigs[quadrant]; }
		virtual SimpleProgramPtr GetDeferredProgram(Quadrant quadrant) { return deferredPrograms[quadrant]; }
		
		virtual void SetDeferredRect(const Recti& rect)
		{
			deferredRect = rect;
			deferredSplitPoint = deferredRect.Clamp(deferredSplitPoint);
		}
		
		virtual void SetDeferredSplit(const Vector2i& position) { deferredSplitPoint = deferredRect.Clamp(position); }
		virtual void SetDeferredSplitPercent(const Vector2f& pct) { deferredSplitPoint = deferredRect.percent(pct); }
		virtual const Vector2i& GetDeferredSplitPoint() const { return deferredSplitPoint; }
		virtual const Recti& GetDeferredRect() const { return deferredRect; }
		virtual void RenderDeferred(Quadrant quadrant = Recti::UpperLeft);

		std::vector<SimpleProgramPtr> Programs;

		std::map<std::string, RenderConfig> RenderConfigs;
		std::map<std::string, Texture> Textures;
		std::map<std::string, Sampler> Samplers;
		std::map<std::string, Framebuffer> Framebuffers;
		std::map<std::string, Renderbuffer> Renderbuffers;
		std::vector<std::string> Paths;
		Df::VariableList VarList;

		SimpleProgramPtr FindProgram(const std::string& renderConfigName, const std::string& name);

	private:
		TSimpleResourceManager<GLuint> textureUnits;
		void InitTexUnits();
		void KillTexUnits();

	public:
		GLuint GetTexUnit() { return textureUnits.Create(); }
		void FreeTexUnit(GLuint id) { textureUnits.Delete(id); }

	private:
		RenderConfigPtr gbufferConfig = nullptr;
		SimpleProgramPtr gbufferProgram = nullptr;
		RenderConfigPtr deferredConfigs[4] = { nullptr, nullptr, nullptr, nullptr };
		SimpleProgramPtr deferredPrograms[4] = { nullptr, nullptr, nullptr, nullptr };
		Vector2i deferredSplitPoint = Vector2i(0, 0);
		Recti deferredRect = Recti(0, 0, 0, 0);

		std::string basepath;
		RenderConfig* pcur_renderconfig = nullptr;
		RenderConfig::Program* pcur_program = nullptr;
		std::string cur_sampler;
		std::string cur_texture;
		Framebuffer* pcur_fbo = nullptr;

		virtual bool new_renderconfig(const std::string& name);
		virtual bool use_renderconfig(const std::string& name);
		virtual bool new_program(const std::string& name);
		virtual bool use_program(const std::string& name);

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
