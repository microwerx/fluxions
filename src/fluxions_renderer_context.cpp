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
#include "pch.hpp"
#include <fluxions_renderer_context.hpp>
#include <fluxions_renderer_program.hpp>

namespace Fluxions
{

	RendererContext::RendererContext()
	{
	}

	RendererContext::~RendererContext()
	{
		kill();
	}

	bool RendererContext::loadConfig(const std::string& filename)
	{
		FilePathInfo fpi(filename);
		basepath = fpi.dir;
		if (fpi.DoesNotExist())
			return false;

		std::ifstream fin(filename);
		if (!fin)
			return false;

		std::string line;
		Df::TokenVector tokens;

		std::map<std::string, int> quickLookup{
			{"renderconfig", 1},
			{"path", 2},
			{"program", 3},
			{"vertshader", 4},
			{"fragshader", 5},
			{"vertattrib", 6},
			{"sampler", 7},
			{"texture", 8},
			{"fbo", 9},
			{"renderbuffer", 10},
			{"geomshader", 11}
		};

		size_t lineno = 0;
		while (fin)
		{
			line = "";
			lineno++;
			std::getline(fin, line);

			Df::lex_quick_l3_parse(line, tokens);

			if (tokens.empty())
				continue;

			size_t count = tokens.size();

			bool result = false;

			// process commands
			if (count >= 1 && tokens[0].IsString())
			{
				bool processed = true;
				int number = quickLookup[tokens[0].sval];
				switch (number)
				{
				case 0:
					processed = false;
					break;
				case 1:
					result = k_renderconfig(tokens);
					break;
				case 2:
					result = k_path(tokens);
					break;
				case 3:
					result = k_program(tokens);
					break;
				case 4:
					result = k_vertshader(tokens);
					break;
				case 5:
					result = k_fragshader(tokens);
					break;
				case 6:
					result = k_vertattrib(tokens);
					break;
				case 7:
					result = k_sampler(tokens);
					break;
				case 8:
					result = k_texture(tokens);
					break;
				case 9:
					result = k_fbo(tokens);
					break;
				case 10:
					result = k_renderbuffer(tokens);
					break;
				case 11:
					result = k_geomshader(tokens);
					break;
				default:
					processed = false;
					break;
				}

				if (!processed && count >= 2)
				{
					if (!tokens[0].sval.empty() && tokens[0].sval[0] == '@')
					{
						result = true;
						if (tokens[1].IsInteger())
							VarList.set_var(tokens[0].sval, tokens[1].ival);
						else if (tokens[1].IsDouble())
							VarList.set_var(tokens[0].sval, tokens[1].dval);
						else if (tokens[1].IsStringOrIdentifier())
							VarList.set_var(tokens[0].sval, tokens[1].sval);
						else {
							result = false;
						}
					}
				}
			}

			HFLOGDEBUG("(Line: %03i) [%s] %s", lineno, Df::TokenVectorJoin(tokens, " ").c_str(),
				result ? "" : "(FALSE)");
		}

		fin.close();
		return true;
	}

	void RendererContext::init(const std::string& name) {
		RendererObject::init(name);
		initTexUnits();
	}

	void RendererContext::kill() {
		RendererObject::kill();
		pcur_program = nullptr;
		pcur_renderconfig = nullptr;
		Programs.clear();
		RenderConfigs.clear();
		Textures.clear();
		Samplers.clear();
		Framebuffers.clear();
		Renderbuffers.clear();
		Paths.clear();
		killTexUnits();
	}

	void RendererContext::reset()
	{
		RendererObject::reset();
		kill();
		init(name_);
	}

	void RendererContext::initTexUnits()
	{
		killTexUnits();

		// Push some texture_ units to our resource manager so we can dish these out as necessary
		for (int i = 0; i < 32; i++)
		{
			GLuint id = 16 + i;
			textureUnits.Add(id);
		}
	}

	void RendererContext::killTexUnits()
	{
		textureUnits.Clear();
	}

	void RendererContext::renderGbuffer()
	{
	}

	void RendererContext::renderDeferred(Quadrant quadrant)
	{
		Recti r = deferredRect.GetQuadrant(quadrant, deferredSplitPoint);
		if (r.area() == 0)
			return;

		glBegin(GL_TRIANGLE_STRIP);
		glVertex2f((GLfloat)r.x1(), (GLfloat)r.y1());
		glVertex2f((GLfloat)r.x1(), (GLfloat)r.y2());
		glVertex2f((GLfloat)r.x2(), (GLfloat)r.y1());
		glVertex2f((GLfloat)r.x2(), (GLfloat)r.y2());
		glEnd();
	}

	void RendererContext::loadShaders()
	{
		Programs.clear();

		for (auto& [rcname, rc] : RenderConfigs)
		{
			HFLOGINFO("loading renderconfig %s", rcname.c_str());

			for (auto& p : rc->programs)
			{
				HFLOGINFO("loading program %s", p.name.c_str());

				std::string vspath = FindPathIfExists(p.vertshader, Paths);
				std::string fspath = FindPathIfExists(p.fragshader, Paths);
				std::string gspath = FindPathIfExists(p.geomshader, Paths);

				if (vspath.empty())
				{
					HFLOGERROR("vertex shader %s does not exist", p.vertshader.c_str());
				}

				if (fspath.empty())
				{
					HFLOGERROR("fragment shader %s does not exist", p.fragshader.c_str());
				}
				RendererShaderPtr spVS = CompileShaderFromFile(GL_VERTEX_SHADER, vspath);
				RendererShaderPtr spFS = CompileShaderFromFile(GL_FRAGMENT_SHADER, fspath);
				RendererShaderPtr spGS = CompileShaderFromFile(GL_GEOMETRY_SHADER, gspath);

				if (!spVS->didCompile || !spFS->didCompile)
				{
					HFLOGERROR("failed to load program %s because the vertex/fragment shaders did not compile.", p.name.c_str());
					continue;
				}

				if (!gspath.empty() && !spGS->didCompile)
				{
					HFLOGERROR("failed to load program %s because the geometry shader did not compile.", p.name.c_str());
				}

				RendererProgramPtr program = std::make_shared<RendererProgram>();
				program->init(p.name);
				program->attachShaders(spVS);
				program->attachShaders(spFS);
				if (spGS->didCompile)
				{
					program->attachShaders(spGS);
				}

				for (auto attrib : p.vertex_attribs)
				{
					program->bindAttribLocation(attrib.index, attrib.name.c_str());
				}
				program->link();
				if (program->isLinked())
				{
					for (auto attrib : p.vertex_attribs)
					{
						HFLOGDEBUG("attrib (%02d) %s",
							program->getAttribLocation(attrib.name.c_str()),
							attrib.name.c_str());
					}
					p.program = program;
				}
			}
		}
	}

	RendererConfig* RendererContext::getRendererConfig(const std::string& name) {
		auto it = rendererConfigs.find(name);
		if (it == rendererConfigs.end()) return nullptr;
		return &(it->second);
	}

	RendererGLES30* RendererContext::getRenderer(const std::string& name) {
		auto it = renderers.find(name);
		if (it == renderers.end()) return nullptr;
		return &(it->second);
	}

	RendererProgramPtr RendererContext::findProgram(const std::string& renderConfigName, const std::string& name)
	{
		RendererProgramPtr p = nullptr;

		if (RenderConfigs.count(renderConfigName))
		{
			for (RenderConfig::Program& program :
				RenderConfigs[renderConfigName]->programs)
			{
				if (program.name == name)
				{
					p = program.program;
				}
			}
		}

		return p;
	}

	bool RendererContext::new_renderconfig(const std::string& name)
	{
		RenderConfigs[name] = std::make_shared<RenderConfig>(name);
		return use_renderconfig(name);
	}

	bool RendererContext::use_renderconfig(const std::string& name)
	{
		pcur_renderconfig = nullptr;
		if (!RenderConfigs.count(name))
			return false;
		pcur_renderconfig = RenderConfigs[name];
		return true;
	}

	bool RendererContext::new_program(const std::string& name)
	{
		if (pcur_renderconfig == nullptr)
			return false;
		pcur_renderconfig->programs.push_back(RenderConfig::Program(name));
		return use_program(name);
	}

	bool RendererContext::use_program(const std::string& name)
	{
		if (pcur_renderconfig == nullptr)
			return false;
		std::vector<RenderConfig::Program>& p = pcur_renderconfig->programs;
		auto it = std::find_if(p.begin(), p.end(),
			[&name](RenderConfig::Program& rc) { return rc.name == name; });
		if (it == p.end())
			return false;
		pcur_program = &(*it);
		return true;
	}

	bool RendererContext::k_check(const Df::TokenVector& args,
		int count, const std::string& cmd) {
		if (args.size() < count || args[0].sval != cmd)
			return false;
		return true;
	}

	bool RendererContext::k_strori(const Df::TokenVector& args,
		int i, std::string& x) {
		if (args.size() <= i) return false;
		if (args[i].IsStringOrIdentifier()) {
			x = args[i].sval;
			return true;
		}
		return false;
	}

	bool RendererContext::k_renderconfig(const Df::TokenVector& args)
	{
		if (!k_check(args, 2, "renderconfig")) return false;
		std::string rc;
		if (k_strori(args, 1, rc)) return new_renderconfig(rc);
		return false;
	}

	bool RendererContext::k_path(const Df::TokenVector& args)
	{
		if (!k_check(args, 2, "path")) return false;
		if (args[1].IsStringOrIdentifier())
		{
			FilePathInfo fpi(args[1].sval);
			if (!fpi.IsRelative() && fpi.IsDirectory())
			{
				HFLOGDEBUG("Path added \"%s\"", fpi.path.c_str());
				Paths.push_back(fpi.path);
			}
			else
			{
				std::string testpath = basepath + "/" + args[1].sval;
				fpi.Set(testpath);
				bool isdir = fpi.IsDirectory();
				if (isdir)
				{
					HFLOGDEBUG("Path added \"%s\"", fpi.path.c_str());
					Paths.push_back(fpi.path);
				}
				else
				{
					return false;
				}
			}
		}
		return true;
	}

	bool RendererContext::k_program(const Df::TokenVector& args)
	{
		if (!k_check(args, 2, "program")) return false;
		if (args[1].IsStringOrIdentifier())
			new_program(args[1].sval);
		return true;
	}

	bool RendererContext::k_vertshader(const Df::TokenVector& args)
	{
		if (!k_check(args, 2, "vertshader")) return false;
		if (args[1].IsStringOrIdentifier())
		{
			if (pcur_program != nullptr)
			{
				pcur_program->vertshader = args[1].sval;
			}
		}
		return true;
	}

	bool RendererContext::k_fragshader(const Df::TokenVector& args)
	{
		if (!k_check(args, 2, "fragshader")) return false;
		if (args[1].IsStringOrIdentifier())
		{
			if (pcur_program != nullptr)
			{
				pcur_program->fragshader = args[1].sval;
			}
		}
		return true;
	}

	bool RendererContext::k_geomshader(const Df::TokenVector& args)
	{
		if (!k_check(args, 2, "geomshader")) return false;
		if (args[1].IsStringOrIdentifier())
		{
			if (pcur_program != nullptr)
			{
				pcur_program->geomshader = args[1].sval;
			}
		}
		return true;
	}

	bool RendererContext::k_vertattrib(const Df::TokenVector& args)
	{
		if (!k_check(args, 3, "vertattrib")) return false;
		if (args[1].IsInteger() && args[2].IsStringOrIdentifier())
		{
			if (pcur_program != nullptr)
			{
				pcur_program->vertex_attribs.push_back(
					RenderConfig::Program::VertexAttrib(args[1].ival, args[2].sval));
			}
		}
		return true;
	}

	bool RendererContext::k_sampler(const Df::TokenVector& args)
	{
		int count = (int)args.size();
		if (!k_check(args, 2, "sampler")) return false;
		std::string samplerName;
		if (count == 2 && k_strori(args, 1, samplerName)) {
			Samplers[samplerName].name = samplerName;
			cur_sampler = samplerName;
		}
		else if (!cur_sampler.empty() && count >= 3 && args[1].IsString())
		{
			GLenum paramName = Fluxions::glNameTranslator.GetEnum(args[1].sval);
			GLenum paramValue;

			if (args[2].IsString())
			{
				paramValue = Fluxions::glNameTranslator.GetEnum(args[2].sval);
			}
			else if (args[2].IsInteger())
			{
				paramValue = args[2].ival;
			}

			Samplers[cur_sampler].parameters.push_back(std::make_pair(paramName, paramValue));
		}
		return true;
	}

	bool RendererContext::k_texture(const Df::TokenVector& args)
	{
		size_t count = args.size();
		if (count < 2 || args[0].sval != "texture")
			return false;
		if (count == 2 && args[1].IsString())
		{
			GLenum param = Fluxions::glNameTranslator.GetEnum(args[1].sval);
			if (args[1].sval == "generateMipmap")
				Textures[cur_texture].genmipmap = true;
			else if (param == GL_GENERATE_MIPMAP)
				Textures[cur_texture].genmipmap = true;
			else if (param == GL_TEXTURE_1D)
				Textures[cur_texture].target = param;
			else if (param == GL_TEXTURE_2D)
				Textures[cur_texture].target = param;
			else if (param == GL_TEXTURE_CUBE_MAP)
				Textures[cur_texture].target = param;
			else
			{
				Textures[args[1].sval].name = args[1].sval;
				cur_texture = args[1].sval;
			}
		}
		else if (!cur_texture.empty())
		{
			if (count == 3 && args[1].IsString() && args[2].IsString())
			{
				GLenum param = Fluxions::glNameTranslator.GetEnum(args[1].sval);
				if (param == GL_TEXTURE_2D)
					Textures[cur_texture].target = GL_TEXTURE_2D;
				if (param >= GL_TEXTURE_CUBE_MAP_POSITIVE_X && param <= GL_TEXTURE_CUBE_MAP_NEGATIVE_Z)
					Textures[cur_texture].target = GL_TEXTURE_CUBE_MAP;
				if (param == GL_TEXTURE_2D ||
					(param >= GL_TEXTURE_CUBE_MAP_POSITIVE_X && param <= GL_TEXTURE_CUBE_MAP_NEGATIVE_Z))
				{
					std::string path = FindPathIfExists(args[2].sval, Paths);
					if (!path.empty())
						Textures[cur_texture].files.push_back(std::make_pair(param, path));
				}
			}
			else if (count == 7 && args[1].IsInteger() && args[2].IsString() &&
				args[3].IsInteger() && args[4].IsInteger() &&
				args[5].IsString() && args[6].IsString())
			{
				Textures[cur_texture].level = args[1].ival;
				Textures[cur_texture].internalformat = Fluxions::glNameTranslator.GetEnum(args[2].sval);
				Textures[cur_texture].width = args[3].ival;
				Textures[cur_texture].height = args[4].ival;
				Textures[cur_texture].format = Fluxions::glNameTranslator.GetEnum(args[5].sval);
				Textures[cur_texture].type = Fluxions::glNameTranslator.GetEnum(args[6].sval);
			}
		}

		return true;
	}

	bool RendererContext::k_fbo(const Df::TokenVector& args)
	{
		size_t count = args.size();
		if (count < 2 || args[0].sval != "fbo")
			return false;
		if (count == 2 && args[1].IsStringOrIdentifier())
		{
			std::string name = args[1].sval;
			Framebuffers[name].name = name;
			Framebuffers[name].renderbuffers.clear();
			Framebuffers[name].textures.clear();
			pcur_fbo = &Framebuffers[name];
		}
		else if (pcur_fbo != nullptr && count >= 5 && args[1].IsStringOrIdentifier() && args[1].sval == "attach")
		{
			bool gotTarget = false;
			bool gotAttachment = false;
			GLenum target = glNameTranslator.GetEnum(args[2].sval);
			GLenum attachment = glNameTranslator.GetEnum(args[3].sval);
			if (target == GL_RENDERBUFFER ||
				target == GL_TEXTURE_2D ||
				(target >= GL_TEXTURE_CUBE_MAP_POSITIVE_X && target <= GL_TEXTURE_CUBE_MAP_NEGATIVE_Z))
			{
				gotTarget = true;
			}
			if ((attachment >= GL_COLOR_ATTACHMENT0 && attachment <= GL_COLOR_ATTACHMENT15) ||
				attachment == GL_DEPTH_ATTACHMENT ||
				attachment == GL_STENCIL_ATTACHMENT ||
				attachment == GL_DEPTH_STENCIL_ATTACHMENT)
			{
				gotAttachment = true;
			}
			if (gotTarget && gotAttachment)
			{
				if (target == GL_RENDERBUFFER)
				{
					pcur_fbo->renderbuffers.push_back(std::make_tuple(attachment, args[4].sval));
				}
				else
				{
					int level = 0;
					if (count == 6 && args[5].IsInteger())
						level = args[5].ival;
					pcur_fbo->textures.push_back(std::make_tuple(attachment, target, args[4].sval, level));
				}
			}
		}
		return true;
	}

	bool RendererContext::k_renderbuffer(const Df::TokenVector& args)
	{
		size_t count = args.size();
		if (count < 5 || args[0].sval != "renderbuffer")
			return false;
		else
		{
			Renderbuffer renderbuffer;

			if (args[1].IsStringOrIdentifier())
				renderbuffer.name = args[1].sval;
			if (args[2].IsStringOrIdentifier())
				renderbuffer.internalformat = glNameTranslator.GetEnum(args[2].sval);
			if (args[3].IsStringOrIdentifier())
			{
				renderbuffer.width = VarList.get_var_integer(args[3].sval);
			}
			else if (args[3].IsInteger())
				renderbuffer.width = args[3].ival;
			else
				renderbuffer.width = 0;

			if (args[4].IsStringOrIdentifier())
			{
				renderbuffer.height = VarList.get_var_integer(args[4].sval);
			}
			else if (args[4].IsInteger())
				renderbuffer.height = args[4].ival;
			else
				renderbuffer.height = 0;

			if (count == 6 && args[5].IsInteger())
				renderbuffer.samples = args[5].ival;
			else
				renderbuffer.samples = 1;

			Renderbuffers[renderbuffer.name] = renderbuffer;
		}
		return true;
	}
} // namespace Fluxions
