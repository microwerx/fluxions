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

	RendererContext::RendererContext() {}

	RendererContext::~RendererContext() {}

	void RendererContext::init(const std::string& name,
							   RendererObject* pparent) {
		RendererObject::init(name, pparent);
		initTexUnits();
	}

	void RendererContext::kill() {
		//TODO: remove old renderer code
		//pcur_program = nullptr;
		//pcur_renderconfig = nullptr;
		//Programs.clear();
		//RenderConfigs.clear();
		//Textures.clear();
		//Samplers.clear();
		//Framebuffers.clear();
		//Renderbuffers.clear();
		paths.clear();

		pcurFBO = nullptr;
		pcurProgram = nullptr;
		pcurRenderer = nullptr;
		pcurRendererConfig = nullptr;
		pcurSampler = nullptr;
		pcurTexture2D = nullptr;
		pcurTextureCube = nullptr;

		fbos.clear();
		programs.clear();
		renderers.clear();
		rendererConfigs.clear();
		samplers.clear();
		texture2Ds.clear();
		textureCubes.clear();

		killTexUnits();
		RendererObject::kill();
	}

	const char* RendererContext::type() const {
		return "RendererContext";
	}

	bool RendererContext::loadConfig(const std::string& filename) {
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
			{"renderconfig",  1},
			{"path",          2},
			{"program",       3},
			{"vertshader",    4},
			{"fragshader",    5},
			{"vertattrib",    6},
			{"sampler",       7},
			{"texture",       8},
			{"fbo",           9},
			{"renderbuffer", 10},
			{"geomshader",   11},
			{"renderer",     12},
			{"texture2D",    13},
			{"textureCube",  14}
		};

		size_t lineno = 0;
		while (fin) {
			line = "";
			lineno++;
			std::getline(fin, line);

			Df::lex_quick_l3_parse(line, tokens);

			if (tokens.empty())
				continue;

			size_t count = tokens.size();

			bool result = false;

			// process commands
			if (count >= 1 && tokens[0].IsString()) {
				bool processed = true;
				int number = quickLookup[tokens[0].sval];
				if (!number) {
					processed = false;
				}
				else {
					k_replaceVars(tokens);
				}
				switch (number) {
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
				case 12:
					result = k_renderer(tokens);
					break;
				case 13:
					result = k_texture2D(tokens);
					break;
				case 14:
					result = k_textureCube(tokens);
				default:
					processed = false;
					break;
				}

				if (!processed && count >= 2) {
					if (!tokens[0].sval.empty() && tokens[0].sval[0] == '@') {
						result = true;
						if (tokens[1].IsInteger())
							vars.set_var(tokens[0].sval, tokens[1].ival);
						else if (tokens[1].IsDouble())
							vars.set_var(tokens[0].sval, tokens[1].dval);
						else if (tokens[1].IsStringOrIdentifier())
							vars.set_var(tokens[0].sval, tokens[1].sval);
						else {
							result = false;
						}
					}
				}
			}

			HFLOGDEBUG("(Line: %03i) [%s] %s", lineno, Df::TokenVectorJoin(tokens, " ").c_str(),
					   result ? "" : "(FALSE)");
		}

		return true;
	}

	//bool RendererContext::isConfig(const std::string& name) const {
	//	return RenderConfigs.count(name);
	//}

	//bool RendererContext::useConfig(const std::string& name) {
	//	return use_renderconfig(name);
	//}

	//RenderConfigPtr RendererContext::getConfig(const std::string& name) {
	//	return isConfig(name) ? RenderConfigs[name] : nullptr;
	//}

	void RendererContext::initTexUnits() {
		killTexUnits();

		// Push some texture_ units to our resource manager so we can dish these out as necessary
		for (int i = 0; i < 32; i++) {
			GLuint id = 16 + i;
			textureUnits.Add(id);
		}
	}

	void RendererContext::killTexUnits() {
		textureUnits.Clear();
	}

	GLuint RendererContext::getTexUnit() {
		return textureUnits.Create();
	}

	void RendererContext::freeTexUnit(GLuint id) {
		textureUnits.Delete(id);
	}

	//void RendererContext::renderGbuffer() {}

	void RendererContext::setDeferredRect(const Recti& rect) {
		deferredRect = rect;
		deferredSplitPoint = deferredRect.Clamp(deferredSplitPoint);
	}

	void RendererContext::setDeferredSplit(const Vector2i& position) {
		deferredSplitPoint = deferredRect.Clamp(position);
	}

	void RendererContext::setDeferredSplitPercent(const Vector2f& pct) {
		deferredSplitPoint = deferredRect.percent(pct);
	}

	const Vector2i& RendererContext::getDeferredSplitPoint() const {
		return deferredSplitPoint;
	}

	const Recti& RendererContext::getDeferredRect() const {
		return deferredRect;
	}

	void RendererContext::renderDeferred(Quadrant quadrant) {
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

	void RendererContext::loadShaders() {
		HFLOGINFO("Loading shaders from renderconfig");
		for (auto& [k, p] : programs) {
			HFLOGINFO("Trying to load shader program '%s'",
					  k.c_str());
			p.loadShaders();
			p.link();
		}

		//Programs.clear();

		//for (auto& [rcname, rc] : RenderConfigs) {
		//	HFLOGINFO("loading renderconfig %s", rcname.c_str());

		//	for (auto& p : rc->programs) {
		//		HFLOGINFO("loading program %s", p.name.c_str());

		//		std::string vspath = FindPathIfExists(p.vertshader, paths);
		//		std::string fspath = FindPathIfExists(p.fragshader, paths);
		//		std::string gspath = FindPathIfExists(p.geomshader, paths);

		//		if (vspath.empty()) {
		//			HFLOGERROR("vertex shader %s does not exist", p.vertshader.c_str());
		//		}

		//		if (fspath.empty()) {
		//			HFLOGERROR("fragment shader %s does not exist", p.fragshader.c_str());
		//		}
		//		RendererShaderPtr spVS = CompileShaderFromFile(GL_VERTEX_SHADER, vspath);
		//		RendererShaderPtr spFS = CompileShaderFromFile(GL_FRAGMENT_SHADER, fspath);
		//		RendererShaderPtr spGS = CompileShaderFromFile(GL_GEOMETRY_SHADER, gspath);

		//		if (!spVS->didCompile || !spFS->didCompile) {
		//			HFLOGERROR("failed to load program %s because the vertex/fragment shaders did not compile.", p.name.c_str());
		//			continue;
		//		}

		//		if (!gspath.empty() && !spGS->didCompile) {
		//			HFLOGERROR("failed to load program %s because the geometry shader did not compile.", p.name.c_str());
		//		}

		//		RendererProgramPtr program = std::make_shared<RendererProgram>();
		//		program->init(p.name);
		//		program->attachShaders(spVS);
		//		program->attachShaders(spFS);
		//		if (spGS->didCompile) {
		//			program->attachShaders(spGS);
		//		}

		//		for (auto attrib : p.vertex_attribs) {
		//			program->bindAttribLocation(attrib.index, attrib.name.c_str());
		//		}
		//		program->link();
		//		if (program->isLinked()) {
		//			for (auto attrib : p.vertex_attribs) {
		//				HFLOGDEBUG("attrib (%02d) %s",
		//						   program->getAttribLocation(attrib.name.c_str()),
		//						   attrib.name.c_str());
		//			}
		//			p.program = program;
		//		}
		//	}
		//}
	}

	void RendererContext::loadTextures() {
		HFLOGINFO("Loading textures from renderconfig");
		for (auto& [k, t] : texture2Ds) {
			HFLOGINFO("Trying to load texture 2d '%s'",
					  k.c_str());
		}

		for (auto& [k, t] : textureCubes) {
			HFLOGINFO("Trying to load texture cube '%s'",
					  k.c_str());
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

	//TODO: remove findProgram which should be configured with renderconfig
	//RendererProgramPtr RendererContext::findProgram(const std::string& renderConfigName, const std::string& name) {
	//	RendererProgramPtr p = nullptr;

	//	if (RenderConfigs.count(renderConfigName)) {
	//		for (RenderConfig::Program& program :
	//			 RenderConfigs[renderConfigName]->programs) {
	//			if (program.name == name) {
	//				p = program.program;
	//			}
	//		}
	//	}

	//	return p;
	//}

	//bool RendererContext::new_renderconfig(const std::string& name) {
	//	RenderConfigs[name] = std::make_shared<RenderConfig>(name);

	//	rendererConfigs[name].init(name, this);
	//	pcurRendererConfig = &rendererConfigs[name];

	//	return use_renderconfig(name);
	//}

	//bool RendererContext::use_renderconfig(const std::string& name) {
	//	pcur_renderconfig = nullptr;
	//	if (!RenderConfigs.count(name))
	//		return false;
	//	pcur_renderconfig = RenderConfigs[name];
	//	return true;
	//}

	//bool RendererContext::new_program(const std::string& name) {
	//	if (pcur_renderconfig == nullptr)
	//		return false;
	//	pcur_renderconfig->programs.push_back(RenderConfig::Program(name));
	//	return use_program(name);
	//}

	//bool RendererContext::use_program(const std::string& name) {
	//	if (pcur_renderconfig == nullptr)
	//		return false;
	//	std::vector<RenderConfig::Program>& p = pcur_renderconfig->programs;
	//	auto it = std::find_if(p.begin(), p.end(),
	//						   [&name](RenderConfig::Program& rc) { return rc.name == name; });
	//	if (it == p.end())
	//		return false;
	//	pcur_program = &(*it);
	//	return true;
	//}

	bool RendererContext::k_replaceVars(Df::TokenVector& args) {
		bool replacedAny = false;
		for (auto& arg : args) {
			if (arg.IsStringOrIdentifier()) {
				if (vars.is_var(arg.sval)) {
					// replace variable here
					auto it = vars.get_var(arg.sval);
					if (it == vars.get_var_end()) {
						continue;
					}
					arg = it->second;
					replacedAny = true;
				}
			}
		}
		return replacedAny;
	}

	bool RendererContext::k_check(const Df::TokenVector& args,
								  int count, const std::string& cmd) {
		if (args.size() < count || args[0].sval != cmd)
			return false;
		return true;
	}

	bool RendererContext::k_sval(const Df::TokenVector& args,
								 int i, std::string& x) {
		if (args.size() <= i) return false;
		if (args[i].IsStringOrIdentifier()) {
			x = args[i].sval;
			return true;
		}
		return false;
	}

	bool RendererContext::k_ival(const Df::TokenVector& args,
								 int i, int& ival) {
		if (args.size() <= i) return false;
		if (args[i].IsInteger()) {
			ival = args[i].ival;
			return true;
		}
		return false;
	}

	bool RendererContext::k_dval(const Df::TokenVector& args,
								 int i, double& dval) {
		if (args.size() <= i) return false;
		if (args[i].IsIntegerOrDouble()) {
			dval = args[i].dval;
			return true;
		}
		return false;
	}

	bool RendererContext::k_glenum(const Df::TokenVector& args,
								   int i, GLenum& enumValue) {
		if (args.size() <= i) return false;
		if (args[i].IsStringOrIdentifier()) {
			GLenum paramName = Fluxions::glNameTranslator.getEnum(args[i].sval);
			if (paramName) {
				enumValue = paramName;
				return true;
			}
		}
		return false;
	}

	int RendererContext::k_ivalue(const Df::TokenVector& args, int i) {
		if (args.size() <= i) return 0;
		else if (args[i].IsInteger()) return args[i].ival;
		else if (args[i].IsDouble()) return (int)args[i].dval;
		else if (args[i].sval.empty()) return 0;
		else if (args[i].sval[0] == '@') {
			return vars.get_var_integer(args[i].sval);
		}
		return 0;
	}

	double RendererContext::k_dvalue(const Df::TokenVector& args, int i) {
		if (args.size() <= i) return 0;
		else if (args[i].IsDouble()) return args[i].dval;
		else if (args[i].IsInteger()) return args[i].ival;
		else if (args[i].sval.empty()) return 0;
		else if (args[i].sval[0] == '@') {
			return vars.get_var_double(args[i].sval);
		}
		else return args[i].dval;
	}

	bool RendererContext::k_renderconfig(const Df::TokenVector& args) {
		if (!k_check(args, 2, "renderconfig")) return false;
		const int count = (int)args.size();
		std::string arg1;
		std::string arg2;
		std::string arg3;
		bool svalarg1 = k_sval(args, 1, arg1);
		bool svalarg2 = k_sval(args, 2, arg2);
		bool svalarg3 = k_sval(args, 3, arg3);
		double dval;
		int argc = (int)args.size();
		static const std::string WRITEFBO{ "writefbo" };
		static const std::string READFBO{ "readfbo" };
		static const std::string PROGRAM{ "program" };
		static const std::string ZONLY{ "zonly" };
		static const std::string CUBEMAP{ "cubemap" };
		static const std::string SRGB{ "srgb" };
		static const std::string TRUESTRING{ "true" };
		static const std::string FALSESTRING{ "false" };
		static const std::string ZNEAR{ "znear" };
		static const std::string ZFAR{ "zfar" };
		static const std::string CAMERA{ "camera" };
		static const std::string PROJECTION{ "projection" };
		static const std::string CLEARCOLOR{ "clearcolor" };

		if (svalarg1) {
			if (pcurRendererConfig && count == 6 && arg1 == CLEARCOLOR) {
				pcurRendererConfig->clearColor.reset(
					(float)args[2].dval,
					(float)args[3].dval,
					(float)args[4].dval,
					(float)args[5].dval
				);
				pcurRendererConfig->clearColorBuffer = true;
				return true;
			}
			else if (pcurRendererConfig && arg1 == ZNEAR && count == 3) {
				pcurRendererConfig->znear = (float)args[2].dval;
				return true;
			}
			else if (pcurRendererConfig && arg1 == ZFAR && count == 3) {
				pcurRendererConfig->zfar = (float)args[2].dval;
				return true;
			}
			else if (pcurRendererConfig && svalarg2) {
				tolower(arg1);
				if (arg1 == WRITEFBO) {
					pcurRendererConfig->writeFBOs.push_back({ arg2, nullptr });
					HFLOGINFO("rendererconfig '%s' adding write fbo '%s'",
							  pcurRendererConfig->name(),
							  arg2.c_str());
					return true;
				}
				else if (arg1 == READFBO) {
					pcurRendererConfig->readFBOs.push_back({ arg2, nullptr });
					HFLOGINFO("rendererconfig '%s' adding read fbo '%s'",
							  pcurRendererConfig->name(),
							  arg2.c_str());
					return true;
				}
				else if (arg1 == PROGRAM) {
					if (programs.count(arg2)) {
						pcurRendererConfig->rc_program = arg2;
						HFLOGINFO("rendererconfig '%s' adding program '%s'",
								  pcurRendererConfig->name(),
								  arg2.c_str());
						return true;
					}
					else {
						HFLOGERROR("program '%s' not defined",
								   arg2.c_str());
						return false;
					}
				}
				else if (arg1 == CAMERA) {
					pcurRendererConfig->ssg_camera = arg2;
					HFLOGINFO("rendererconfig '%s' adding camera '%s'",
							  pcurRendererConfig->name(),
							  arg2.c_str());
					return true;
				}
				else if (arg1 == PROJECTION) {
					pcurRendererConfig->ssg_projection = arg2;
					HFLOGINFO("rendererconfig '%s' adding projection '%s'",
							  pcurRendererConfig->name(),
							  arg2.c_str());
					return true;
				}

				bool arg2TrueFalse = false;
				if (arg2 == TRUESTRING) arg2TrueFalse = true;
				else if (arg2 == FALSESTRING) arg2TrueFalse = false;
				else {
					HFLOGWARN("Use 'true' for flags");
				}

				if (arg1 == ZONLY) {
					pcurRendererConfig->useZOnly = arg2TrueFalse;
					return true;
				}
				else if (arg1 == CUBEMAP) {
					pcurRendererConfig->isCubeMap = arg2TrueFalse;
					return true;
				}
				else if (arg1 == SRGB) {
					pcurRendererConfig->enableSRGB = arg2TrueFalse;
					return true;
				}
			}
			else {
				rendererConfigs[arg1].init(arg1, this);
				pcurRendererConfig = &rendererConfigs[arg1];
				return true;
			}
		}
		return false;
	}

	bool RendererContext::k_path(const Df::TokenVector& args) {
		if (!k_check(args, 2, "path")) return false;
		if (args[1].IsStringOrIdentifier()) {
			FilePathInfo fpi(args[1].sval);
			if (!fpi.IsRelative() && fpi.IsDirectory()) {
				HFLOGDEBUG("Path added \"%s\"", fpi.path.c_str());
				paths.push_back(fpi.path);
			}
			else {
				std::string testpath = basepath + "/" + args[1].sval;
				fpi.Set(testpath);
				bool isdir = fpi.IsDirectory();
				if (isdir) {
					HFLOGDEBUG("Path added \"%s\"", fpi.path.c_str());
					paths.push_back(fpi.path);
				}
				else {
					return false;
				}
			}
		}
		return true;
	}

	bool RendererContext::k_program(const Df::TokenVector& args) {
		if (!k_check(args, 2, "program")) return false;
		std::string arg1;
		bool svalarg1 = k_sval(args, 1, arg1);
		if (svalarg1) {
			programs[arg1].init(arg1, this);
			pcurProgram = &programs[arg1];
		}
		return true;
	}

	bool RendererContext::k_vertshader(const Df::TokenVector& args) {
		if (!k_check(args, 2, "vertshader")) return false;
		std::string arg1;
		bool svalarg1 = k_sval(args, 1, arg1);
		if (svalarg1) {
			if (pcurProgram) {
				arg1 = Fluxions::FindPathIfExists(arg1, paths);
				pcurProgram->vertshaderpath = arg1;
				return true;
			}
		}
		return false;
	}

	bool RendererContext::k_fragshader(const Df::TokenVector& args) {
		if (!k_check(args, 2, "fragshader")) return false;
		std::string arg1;
		bool svalarg1 = k_sval(args, 1, arg1);
		if (svalarg1) {
			if (pcurProgram) {
				arg1 = Fluxions::FindPathIfExists(arg1, paths);
				pcurProgram->fragshaderpath = arg1;
				return true;
			}
		}
		return false;
	}

	bool RendererContext::k_geomshader(const Df::TokenVector& args) {
		if (!k_check(args, 2, "geomshader")) return false;
		std::string arg1;
		bool svalarg1 = k_sval(args, 1, arg1);
		if (svalarg1) {
			if (pcurProgram) {
				arg1 = Fluxions::FindPathIfExists(arg1, paths);
				pcurProgram->geomshaderpath = arg1;
				return true;
			}
		}
		return false;
	}

	bool RendererContext::k_vertattrib(const Df::TokenVector& args) {
		if (!k_check(args, 3, "vertattrib")) return false;
		int arg1;
		bool ivalarg1 = k_ival(args, 2, arg1);
		std::string arg2;
		bool svalarg2 = k_sval(args, 2, arg2);
		if (ivalarg1 && svalarg2) {
			if (pcurProgram) {
				pcurProgram->requestedVertexAttribs.push_back({ arg1, arg2 });
			}
		}
		return false;
	}

	bool RendererContext::k_sampler(const Df::TokenVector& args) {
		//int count = (int)args.size();
		//if (!k_check(args, 2, "sampler")) return false;
		//std::string samplerName;
		//if (count == 2 && k_sval(args, 1, samplerName)) {
		//	Samplers[samplerName].name = samplerName;
		//	cur_sampler = samplerName;
		//}
		//else if (!cur_sampler.empty() && count >= 3 && args[1].IsString()) {
		//	GLenum pname = Fluxions::glNameTranslator.getEnum(args[1].sval);
		//	GLenum param;

		//	if (args[2].IsString()) {
		//		param = Fluxions::glNameTranslator.getEnum(args[2].sval);
		//	}
		//	else if (args[2].IsInteger()) {
		//		param = args[2].ival;
		//	}

		//	Samplers[cur_sampler].parameters.push_back(std::make_pair(pname, param));
		//}

		//if (!k_check(args, 2, "sampler")) return false;

		if (!k_check(args, 2, "sampler")) return false;

		std::string arg1;
		bool svalarg1 = k_sval(args, 1, arg1);
		std::string arg2;
		bool svalarg2 = k_sval(args, 2, arg2);

		if (pcurSampler && svalarg1 && svalarg2) {
			GLenum pname = Fluxions::glNameTranslator.getEnum(arg1);
			GLenum param = Fluxions::glNameTranslator.getEnum(arg2);
			if (!pname || !param) return false;
			switch (pname) {
			case GL_TEXTURE_MAG_FILTER:
				pcurSampler->setMagFilter(param);
				break;
			case GL_TEXTURE_MIN_FILTER:
				pcurSampler->setMinFilter(param);
				break;
			case GL_TEXTURE_COMPARE_MODE:
				pcurSampler->setCompareMode(param);
				break;
			case GL_TEXTURE_COMPARE_FUNC:
				pcurSampler->setCompareFunction(param);
				break;
			case GL_TEXTURE_WRAP_S:
				pcurSampler->setWrapS(param);
				break;
			case GL_TEXTURE_WRAP_T:
				pcurSampler->setWrapT(param);
				break;
			case GL_TEXTURE_WRAP_R:
				pcurSampler->setWrapR(param);
				break;
			default:
				return false;
			}
			return true;
		}
		else if (svalarg1) {
			samplers[arg1].init(arg1, this);
			pcurSampler = &samplers[arg1];
			return true;
		}
		return false;
	}

	bool RendererContext::k_texture(const Df::TokenVector& args) {
		return false;
		//size_t count = args.size();
		//if (count < 2 || args[0].sval != "texture")
		//	return false;
		//if (count == 2 && args[1].IsString()) {
		//	GLenum param = Fluxions::glNameTranslator.getEnum(args[1].sval);
		//	if (args[1].sval == "generateMipmap")
		//		Textures[cur_texture].genmipmap = true;
		//	else if (param == GL_GENERATE_MIPMAP)
		//		Textures[cur_texture].genmipmap = true;
		//	else if (param == GL_TEXTURE_1D)
		//		Textures[cur_texture].target = param;
		//	else if (param == GL_TEXTURE_2D)
		//		Textures[cur_texture].target = param;
		//	else if (param == GL_TEXTURE_CUBE_MAP)
		//		Textures[cur_texture].target = param;
		//	else {
		//		Textures[args[1].sval].name = args[1].sval;
		//		cur_texture = args[1].sval;
		//	}
		//}
		//else if (!cur_texture.empty()) {
		//	if (count == 3 && args[1].IsString() && args[2].IsString()) {
		//		GLenum param = Fluxions::glNameTranslator.getEnum(args[1].sval);
		//		if (param == GL_TEXTURE_2D)
		//			Textures[cur_texture].target = GL_TEXTURE_2D;
		//		if (param >= GL_TEXTURE_CUBE_MAP_POSITIVE_X && param <= GL_TEXTURE_CUBE_MAP_NEGATIVE_Z)
		//			Textures[cur_texture].target = GL_TEXTURE_CUBE_MAP;
		//		if (param == GL_TEXTURE_2D ||
		//			(param >= GL_TEXTURE_CUBE_MAP_POSITIVE_X && param <= GL_TEXTURE_CUBE_MAP_NEGATIVE_Z)) {
		//			std::string path = FindPathIfExists(args[2].sval, paths);
		//			if (!path.empty())
		//				Textures[cur_texture].files.push_back(std::make_pair(param, path));
		//		}
		//	}
		//	else if (count == 7 && args[1].IsInteger() && args[2].IsString() &&
		//			 args[3].IsInteger() && args[4].IsInteger() &&
		//			 args[5].IsString() && args[6].IsString()) {
		//		Textures[cur_texture].level = args[1].ival;
		//		Textures[cur_texture].internalformat = Fluxions::glNameTranslator.getEnum(args[2].sval);
		//		Textures[cur_texture].width = args[3].ival;
		//		Textures[cur_texture].height = args[4].ival;
		//		Textures[cur_texture].format = Fluxions::glNameTranslator.getEnum(args[5].sval);
		//		Textures[cur_texture].type = Fluxions::glNameTranslator.getEnum(args[6].sval);
		//	}
		//}

		//return true;
	}

	bool RendererContext::k_fbo(const Df::TokenVector& args) {
		size_t count = args.size();
		if (count < 2 || args[0].sval != "fbo")
			return false;
		std::string arg1;
		bool svalarg1 = k_sval(args, 1, arg1);
		if (count == 2 && svalarg1) {
			fbos[arg1].init(arg1, this);
			pcurFBO = &fbos[arg1];
			return true;
			//TODO: remove following code
			//std::string name = args[1].sval;
			//Framebuffers[name].name = name;
			//Framebuffers[name].renderbuffers.clear();
			//Framebuffers[name].textures.clear();
			//pcur_fbo = &Framebuffers[name];
		}

		if (!pcurFBO || !svalarg1)
			return false;

		tolower(arg1);
		const std::string ATTACH{ "attach" };
		const std::string DIMENSIONS{ "dimensions" };

		if (arg1 == DIMENSIONS) {
			int width = k_ivalue(args, 2);
			int height = k_ivalue(args, 3);
			if (!width || !height) {
				HFLOGERROR("fbo '%s' dimensions must be > 0",
						   pcurFBO->name());
			}
			pcurFBO->setDimensions(width, height);
		}

		// attach target attachment internalformat mapName
		if (count >= 5 && arg1 == ATTACH) {
			bool gotTarget = false;
			bool gotAttachment = false;
			bool gotInternalformat = false;
			bool generateMipmaps = true;

			GLenum target = glNameTranslator.getEnum(args[2].sval);
			GLenum attachment = glNameTranslator.getEnum(args[3].sval);
			GLenum internalformat = glNameTranslator.getEnum(args[4].sval);
			std::string mapName;
			bool svalMapName = k_sval(args, 5, mapName);

			static const std::vector<GLenum> targets{
				GL_RENDERBUFFER,
				GL_TEXTURE_2D,
				GL_TEXTURE_CUBE_MAP
			};

			if (std::find(targets.begin(), targets.end(),
						  target) != targets.end()) {
				gotTarget = true;
			}
			else {
				HFLOGERROR("target '%s' incorrect for fbo '%s'",
						   args[2].sval.c_str(),
						   pcurFBO->name());
				return false;
			}

			static const std::vector<GLenum> attachments{
				GL_COLOR_ATTACHMENT0,
				GL_COLOR_ATTACHMENT1,
				GL_DEPTH_ATTACHMENT,
				GL_STENCIL_ATTACHMENT,
				GL_DEPTH_STENCIL_ATTACHMENT
			};

			if (std::find(attachments.begin(), attachments.end(),
						  attachment) != attachments.end()) {
				gotAttachment = true;
			}
			else {
				HFLOGERROR("attachment incorrect for fbo '%s'",
						   args[3].sval.c_str(),
						   pcurFBO->name());
				return false;
			}

			static const std::vector<GLenum> internalformats{
				GL_DEPTH24_STENCIL8,
				GL_DEPTH_COMPONENT16,
				GL_DEPTH_COMPONENT24,
				GL_DEPTH_COMPONENT32F,
				GL_RGB8,
				GL_RGB32F
			};

			if (std::find(internalformats.begin(),
						  internalformats.end(),
						  internalformat) != internalformats.end()) {
				gotInternalformat = true;
			}
			else {
				HFLOGERROR("internalformat %s incorrect for fbo '%s'",
						   args[4].sval.c_str(),
						   pcurFBO->name());
				return false;
			}

			if (gotTarget && gotAttachment && gotInternalformat) {
				switch (target) {
				case GL_RENDERBUFFER:
					pcurFBO->addRenderbuffer(attachment, internalformat);
					HFLOGINFO("attaching renderbuffer to fbo '%s'",
							  pcurFBO->name());
					break;
				case GL_TEXTURE_2D:
					pcurFBO->addTexture2D(attachment, target, internalformat, generateMipmaps);
					pcurFBO->setMapName(mapName);
					HFLOGINFO("attaching texture2D to fbo '%s' for map '%s'",
							  pcurFBO->name(),
							  mapName.c_str());
					break;
				case GL_TEXTURE_CUBE_MAP:
					pcurFBO->addTextureCubeMap(attachment, target, internalformat, generateMipmaps);
					pcurFBO->setMapName(mapName);
					HFLOGINFO("attaching textureCube to fbo '%s' for map '%s'",
							  pcurFBO->name(),
							  mapName.c_str());
					break;
				}
				return true;
			}
		}


		return false;
	}

	bool RendererContext::k_renderbuffer(const Df::TokenVector& args) {
		return false;
		//size_t count = args.size();
		//if (count < 5 || args[0].sval != "renderbuffer")
		//	return false;
		//else {
		//	Renderbuffer renderbuffer;

		//	if (args[1].IsStringOrIdentifier())
		//		renderbuffer.name = args[1].sval;
		//	if (args[2].IsStringOrIdentifier())
		//		renderbuffer.internalformat = glNameTranslator.getEnum(args[2].sval);
		//	if (args[3].IsStringOrIdentifier()) {
		//		renderbuffer.width = vars.get_var_integer(args[3].sval);
		//	}
		//	else if (args[3].IsInteger())
		//		renderbuffer.width = args[3].ival;
		//	else
		//		renderbuffer.width = 0;

		//	if (args[4].IsStringOrIdentifier()) {
		//		renderbuffer.height = vars.get_var_integer(args[4].sval);
		//	}
		//	else if (args[4].IsInteger())
		//		renderbuffer.height = args[4].ival;
		//	else
		//		renderbuffer.height = 0;

		//	if (count == 6 && args[5].IsInteger())
		//		renderbuffer.samples = args[5].ival;
		//	else
		//		renderbuffer.samples = 1;

		//	Renderbuffers[renderbuffer.name] = renderbuffer;
		//}
		//return true;
	}

	bool RendererContext::k_texture2D(const Df::TokenVector& args) {
		static const std::string CMDNAME{ "texture2D" };
		if (!k_check(args, 2, CMDNAME)) return false;
		const int count = (int)args.size();
		std::string arg1;
		std::string arg2;
		std::string arg3;
		bool svalarg1 = k_sval(args, 1, arg1);
		bool svalarg2 = k_sval(args, 2, arg2);
		bool svalarg3 = k_sval(args, 3, arg3);
		if (svalarg1 && count == 2) {
			texture2Ds[arg1].init(arg1, this);
			pcurTexture2D = &texture2Ds[arg1];
			return true;
		}
		if (pcurTexture2D && svalarg1 && svalarg2) {
			if (arg1 == "map") {
				pcurTexture2D->mappath = arg2;
				return true;
			}
			else if (arg1 == "uniform") {
				pcurTexture2D->uniformname = arg2;
				return true;
			}
			else if (arg1 == "sampler" && samplers.count(arg2)) {
				pcurTexture2D->samplername = arg2;
				return true;
			}
		}
		return false;
	}

	bool RendererContext::k_textureCube(const Df::TokenVector& args) {
		static const std::string CMDNAME{ "textureCube" };
		if (!k_check(args, 2, CMDNAME)) return false;
		const int count = (int)args.size();
		std::string arg1;
		std::string arg2;
		std::string arg3;
		bool svalarg1 = k_sval(args, 1, arg1);
		bool svalarg2 = k_sval(args, 2, arg2);
		bool svalarg3 = k_sval(args, 3, arg3);
		if (svalarg1 && count == 2) {
			textureCubes[arg1].init(arg1, this);
			pcurTextureCube = &textureCubes[arg1];
			return true;
		}
		return false;
	}

	bool RendererContext::k_renderer(const Df::TokenVector& args) {
		static const std::string CMDNAME{ "renderer" };
		if (!k_check(args, 2, CMDNAME)) return false;
		const int count = (int)args.size();
		std::string arg1;
		std::string arg2;
		std::string arg3;
		bool svalarg1 = k_sval(args, 1, arg1);
		bool svalarg2 = k_sval(args, 2, arg2);
		bool svalarg3 = k_sval(args, 3, arg3);
		if (svalarg1 && count == 2) {
			renderers[arg1].init(arg1, this);
			pcurRenderer = &renderers[arg1];
			pcurRenderer->setContext(this);
			return true;
		}
		if (pcurRenderer && svalarg1 && svalarg2) {
			if (arg1 == "renderconfig" && rendererConfigs.count(arg2)) {
				pcurRenderer->renderconfigname = arg2;
			}
		}
		return false;
	}

} // namespace Fluxions
