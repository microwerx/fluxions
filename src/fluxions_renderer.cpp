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
#include "stdafx.h"
#include <regex>
#include <kasl.hpp>
#include "fluxions_renderer.hpp"
#include "fluxions_fileio.hpp"

namespace Fluxions
{

Renderer::Renderer()
{
}

Renderer::~Renderer()
{
	KillTexUnits();
}

bool Renderer::UploadCompileLink()
{
	return true;
}

void Renderer::Render(const IScene *pScene)
{
}

bool Renderer::LoadConfig(const string &filename)
{
	FilePathInfo fpi(filename);
	basepath = fpi.dir;
	if (fpi.DoesNotExist())
		return false;

	ifstream fin(filename);
	if (!fin)
		return false;

	string line;
	KASL::TokenVector tokens;

	map<string, int> quickLookup;
	quickLookup["renderconfig"] = 1;
	quickLookup["path"] = 2;
	quickLookup["program"] = 3;
	quickLookup["vertshader"] = 4;
	quickLookup["fragshader"] = 5;
	quickLookup["vertattrib"] = 6;
	quickLookup["sampler"] = 7;
	quickLookup["texture"] = 8;
	quickLookup["fbo"] = 9;
	quickLookup["renderbuffer"] = 10;
	quickLookup["geomshader"] = 11;

	size_t lineno = 0;
	while (fin)
	{
		line = "";
		lineno++;
		getline(fin, line);

		KASL::lex_quick_l3_parse(line, tokens);

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
					if (tokens[1].IsInteger())
						VarList.set_var(tokens[0].sval, tokens[1].ival);
					if (tokens[1].IsDouble())
						VarList.set_var(tokens[0].sval, tokens[1].dval);
					if (tokens[1].IsStringOrIdentifier())
						VarList.set_var(tokens[0].sval, tokens[1].sval);
				}
			}
		}

		hflog.info("%s(): (Line: %03i) [%s]", __FUNCTION__, lineno, KASL::TokenVectorJoin(tokens, " ").c_str());
	}

	fin.close();
	return true;
}

void Renderer::Reset()
{
	pcur_program = nullptr;
	pcur_renderconfig = nullptr;
	RenderConfigs.clear();
	Paths.clear();
	Samplers.clear();
	Textures.clear();
	InitTexUnits();
}

void Renderer::InitTexUnits()
{
	KillTexUnits();

	// Push some texture_ units to our resource manager so we can dish these out as necessary
	for (int i = 0; i < 32; i++)
	{
		GLuint id = 16 + i;
		textureUnits.Add(id);
	}
}

void Renderer::KillTexUnits()
{
	textureUnits.Clear();
}

void Renderer::RenderGbuffer()
{
}

void Renderer::RenderDeferred(Quadrant quadrant)
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

void Renderer::LoadShaders()
{
	Programs.clear();

	for (auto it = RenderConfigs.begin(); it != RenderConfigs.end(); it++)
	{
		hflog.info("%s(): loading renderconfig %s", __FUNCTION__, it->first.c_str());
		Renderer::RenderConfig &rc = it->second;

		for (auto &p : rc.programs)
		{
			hflog.info("%s(): loading program %s", __FUNCTION__, p.name.c_str());
			string vspath = FindPathIfExists(p.vertshader, Paths);
			string fspath = FindPathIfExists(p.fragshader, Paths);
			string gspath = FindPathIfExists(p.geomshader, Paths);
			if (vspath.empty())
				hflog.error("%s(): vertex shader %s does not exist", __FUNCTION__, p.vertshader.c_str());
			if (fspath.empty())
				hflog.error("%s(): fragment shader %s does not exist", __FUNCTION__, p.fragshader.c_str());
			shared_ptr<Fluxions::SimpleShader> spVS = CompileShaderFromFile(GL_VERTEX_SHADER, vspath);
			shared_ptr<Fluxions::SimpleShader> spFS = CompileShaderFromFile(GL_FRAGMENT_SHADER, fspath);
			shared_ptr<Fluxions::SimpleShader> spGS = CompileShaderFromFile(GL_GEOMETRY_SHADER, gspath);
			if (!spVS->didCompile || !spFS->didCompile)
			{
				hflog.info("%s(): failed to load program %s because the vertex/fragment shaders did not compile.", __FUNCTION__, p.name.c_str());
				continue;
			}
			if (!gspath.empty() && !spGS->didCompile)
			{
				hflog.info("%s(): failed to load program %s because the geometry shader did not compile.", __FUNCTION__, p.name.c_str());
			}

			SimpleProgramPtr program = make_shared<SimpleProgram>();
			program->Create();
			program->AttachShaders(spVS);
			program->AttachShaders(spFS);
			if (spGS->didCompile)
				program->AttachShaders(spGS);

			for (auto attrib : p.vertex_attribs)
			{
				program->BindAttribLocation(attrib.index, attrib.name.c_str());
			}
			program->Link();
			if (!program->IsLinked())
			{
				hflog.error("%s(): Program not linked!", __FUNCTION__);
			}
			for (auto attrib : p.vertex_attribs)
			{
				cout << "    " << attrib.name << ": " << program->GetAttribLocation(attrib.name.c_str()) << endl;
				hflog.info("%s(): attrib (%2d) %s", __FUNCTION__, program->GetAttribLocation(attrib.name.c_str()), attrib.name.c_str());
			}
			p.program = program;
		}
	}
}

SimpleProgramPtr Renderer::FindProgram(const string &renderConfigName, const string &name)
{
	SimpleProgramPtr p = nullptr;

	auto it = RenderConfigs.find(renderConfigName);
	if (it != RenderConfigs.end())
	{
		for (auto &program : it->second.programs)
		{
			if (program.name == name)
			{
				p = program.program;
			}
		}
	}

	return p;
}

bool Renderer::new_renderconfig(const string &name)
{
	RenderConfigs[name] = RenderConfig(name);
	return use_renderconfig(name);
}

bool Renderer::use_renderconfig(const string &name)
{
	auto it = RenderConfigs.find(name);
	pcur_renderconfig = nullptr;
	if (it == RenderConfigs.end())
		return false;
	pcur_renderconfig = &it->second;
	return true;
}

bool Renderer::new_program(const string &name)
{
	if (pcur_renderconfig == nullptr)
		return false;
	pcur_renderconfig->programs.push_back(RenderConfig::Program(name));
	return use_program(name);
}

bool Renderer::use_program(const string &name)
{
	if (pcur_renderconfig == nullptr)
		return false;
	vector<RenderConfig::Program> &p = pcur_renderconfig->programs;
	auto it = find_if(p.begin(), p.end(),
					  [&name](RenderConfig::Program &rc) { return rc.name == name; });
	if (it == p.end())
		return false;
	pcur_program = &(*it);
	return true;
}

bool Renderer::k_renderconfig(const KASL::TokenVector &args)
{
	size_t count = args.size();
	if (count < 2 || args[0].sval != "renderconfig")
		return false;
	if (args[1].IsStringOrIdentifier())
		return new_renderconfig(args[1].sval);
	return false;
}

bool Renderer::k_path(const KASL::TokenVector &args)
{
	size_t count = args.size();
	if (count < 2 || args[0].sval != "path")
		return false;
	if (args[1].IsStringOrIdentifier())
	{
		FilePathInfo fpi(args[1].sval);
		if (fpi.Exists() && fpi.IsDirectory())
		{
			hflog.info("%s(): Path added \"%s\"", __FUNCTION__, fpi.path.c_str());
			Paths.push_back(fpi.path);
		}
		else
		{
			fpi.Set(basepath + args[1].sval);
			if (fpi.Exists() && fpi.IsDirectory())
			{
				hflog.info("%s(): Path added \"%s\"", __FUNCTION__, fpi.path.c_str());
				Paths.push_back(fpi.path);
			}
			else
				return false;
		}
	}
	return true;
}

bool Renderer::k_program(const KASL::TokenVector &args)
{
	size_t count = args.size();
	if (count < 2 || args[0].sval != "program")
		return false;
	if (args[1].IsStringOrIdentifier())
		new_program(args[1].sval);
	return true;
}

bool Renderer::k_vertshader(const KASL::TokenVector &args)
{
	size_t count = args.size();
	if (count < 2 || args[0].sval != "vertshader")
		return false;
	if (args[1].IsStringOrIdentifier())
	{
		if (pcur_program != nullptr)
		{
			pcur_program->vertshader = args[1].sval;
		}
	}
	return true;
}

bool Renderer::k_fragshader(const KASL::TokenVector &args)
{
	size_t count = args.size();
	if (count < 2 || args[0].sval != "fragshader")
		return false;
	if (args[1].IsStringOrIdentifier())
	{
		if (pcur_program != nullptr)
		{
			pcur_program->fragshader = args[1].sval;
		}
	}
	return true;
}

bool Renderer::k_geomshader(const KASL::TokenVector &args)
{
	size_t count = args.size();
	if (count < 2 || args[0].sval != "geomshader")
		return false;
	if (args[1].IsStringOrIdentifier())
	{
		if (pcur_program != nullptr)
		{
			pcur_program->geomshader = args[1].sval;
		}
	}
	return true;
}

bool Renderer::k_vertattrib(const KASL::TokenVector &args)
{
	size_t count = args.size();
	if (count < 3 || args[0].sval != "vertattrib")
		return false;
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

bool Renderer::k_sampler(const KASL::TokenVector &args)
{
	size_t count = args.size();
	if (count < 2 || args[0].sval != "sampler")
		return false;
	if (count == 2 && args[1].IsString())
	{
		Samplers[args[1].sval].name = args[1].sval;
		cur_sampler = args[1].sval;
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

		Samplers[cur_sampler].parameters.push_back(make_pair(paramName, paramValue));
	}
	return true;
}

bool Renderer::k_texture(const KASL::TokenVector &args)
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
				string path = FindPathIfExists(args[2].sval, Paths);
				if (!path.empty())
					Textures[cur_texture].files.push_back(make_pair(param, path));
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

bool Renderer::k_fbo(const KASL::TokenVector &args)
{
	size_t count = args.size();
	if (count < 2 || args[0].sval != "fbo")
		return false;
	if (count == 2 && args[1].IsStringOrIdentifier())
	{
		string name = args[1].sval;
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
				pcur_fbo->renderbuffers.push_back(make_tuple(attachment, args[4].sval));
			}
			else
			{
				int level = 0;
				if (count == 6 && args[5].IsInteger())
					level = args[5].ival;
				pcur_fbo->textures.push_back(make_tuple(attachment, target, args[4].sval, level));
			}
		}
	}
	return true;
}

bool Renderer::k_renderbuffer(const KASL::TokenVector &args)
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
