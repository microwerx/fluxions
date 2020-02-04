#include "fluxions_pch.hpp"
#include <fluxions_base.hpp>
#include <fluxions_file_system.hpp>

namespace Fluxions {
	bool debugging = false;
	const std::string BlankString{ "" };
	bool __checkedGLInfo = false;
	std::string gl_version;
	std::string gl_renderer;
	std::string gl_vendor;

	std::string FindFileIfExists(const std::string& filename, const std::vector<std::string>& pathsToTry) {
		std::string output;

		// Is there a file name to test?
		FilePathInfo fpi(filename);
		if (fpi.fullfname.empty())
			return output;

		if (TestIfFileExists(filename)) {
			output = filename;
		}
		else {
			for (auto testPathIt : pathsToTry) {
				std::string testPath = testPathIt + fpi.fullfname;
				if (TestIfFileExists(testPath)) {
					output = testPath;
					break;
				}
			}
		}

		return output;
	}

	std::map<std::string, std::string> MakeOptionsFromArgs(int argc, const char** argv) {
		std::map<std::string, std::string> argv_options;
		for (int i = 0; i < argc; i++) {
			std::string option = argv[i];
			HFLOGINFO("Processing '%s'", option.c_str());
			std::regex dashequals("(^-+|=)",
								  std::regex_constants::ECMAScript);
			std::sregex_token_iterator it(option.begin(),
										  option.end(),
										  dashequals,
										  -1);
			std::sregex_token_iterator end;
			size_t count = 0;

			std::string key = "";
			std::string value = "";
			for (; it != end; it++) {
				if (count == 1) {
					key = *it;
				}
				else if (count == 2) {
					value = *it;
				}
				std::string token = *it;
				HFLOGDEBUG("token %d '%s'", count, token.c_str());
				count++;
			}
			if (key.length() > 0) {
				argv_options.emplace(std::make_pair(key, value));
				HFLOGINFO("argv adding key '%s' = '%s'", key.c_str(), value.c_str());
			}
		}
		return argv_options;
	}

	void ReadGLInfo() {
		__checkedGLInfo = true;
		gl_version = (const char*)glGetString(GL_VERSION);
		gl_vendor = (const char*)glGetString(GL_VENDOR);
		gl_renderer = (const char*)glGetString(GL_RENDERER);

		if (glewIsExtensionSupported("GL_ARB_ES2_compatibility")) {
			HFLOGINFO("GL_EXTENSIONS: GL_ARB_ES2_compatibility supported");
		}

		if (glewIsExtensionSupported("GL_ARB_ES3_compatibility")) {
			HFLOGINFO("GL_EXTENSIONS: GL_ARB_ES3_compatibility supported");
		}

		if (glewIsExtensionSupported("GL_ARB_ES3_1_compatibility")) {
			HFLOGINFO("GL_EXTENSIONS: GL_ARB_ES3_1_compatibility supported");
		}

		if (glewIsExtensionSupported("GL_ARB_ES3_2_compatibility")) {
			HFLOGINFO("GL_EXTENSIONS: GL_ARB_ES3_2_compatibility supported");
		}

		HFLOGINFO("GL_RENDERER:   %s", gl_renderer.c_str());
		HFLOGINFO("GL_VERSION:    %s", gl_version.c_str());
		HFLOGINFO("GL_VENDOR:     %s", gl_vendor.c_str());
	}

	const std::string& GetRenderer() {
		if (!__checkedGLInfo) ReadGLInfo();
		return gl_renderer;
	}

	const std::string& GetVendor() {
		if (!__checkedGLInfo) ReadGLInfo();
		return gl_vendor;
	}

	const std::string& GetVersion() {
		if (!__checkedGLInfo) ReadGLInfo();
		return gl_version;
	}

	void Init() {
		// Perform any necessary initialization steps here
		HFLOGINFO("Initializing Fluxions");

		HFLOGINFO("Initializing GLEW");
		GLenum err = glewInit();
		if (err != GLEW_OK) {
			HFLOGERROR("glewInit() -> %s", glewGetErrorString(err));
		}
		ReadGLInfo();

		glHint(GL_FRAGMENT_SHADER_DERIVATIVE_HINT_ARB, GL_NICEST);

#ifndef FLUXIONS_NO_SDL
		HFLOGINFO("Initializing SDL2");
		if (SDL_Init(SDL_INIT_EVERYTHING)) {
			HFLOGERROR("SDL Error! %s", SDL_GetError());
		}

		HFLOGINFO("Initializing SDL2_image for JPG/PNG/WEBP support");
		int flags = IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_WEBP;
		flags = IMG_Init(flags);
		if ((flags & IMG_INIT_JPG) == 0) HFLOGWARN("SDL2_image: No JPG support!");
		if ((flags & IMG_INIT_PNG) == 0) HFLOGWARN("SDL2_image: No PNG support!");
		if ((flags & IMG_INIT_WEBP) == 0) HFLOGWARN("SDL2_image: No WEBP support!");
#endif
	}

	void Kill() {}

	void YieldThread() {
#ifdef _WIN32
		INPUT input;
		ZeroMemory(&input, sizeof(INPUT));
		input.type = INPUT_MOUSE;
		input.mi.dwFlags = MOUSEEVENTF_MOVE;
		SendInput(1, &input, sizeof(INPUT));
		Sleep(0);
#elif _POSIX_VERSION
		usleep(0);
#endif
	}

} // namespace Fluxions
