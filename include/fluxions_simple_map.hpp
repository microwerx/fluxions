#ifndef FLUXIONS_SIMPLE_MAP_HPP
#define FLUXIONS_SIMPLE_MAP_HPP

namespace Fluxions {

	struct SimpleMap
	{
		std::string mapName;
		GLuint mapId = 0;
		std::string pathname;
		std::string shader;

		struct SimpleMapCached {
			GLuint textureId = 0;
			GLuint samplerId = 0;
			GLint unitId = 0; // we're not using GLenum here because unitId can be < 0 and no overflow is expected.
			GLint map_loc = 0;
			GLint map_mix_loc = 0;
			RendererTextureObject textureObject;
		} cached;
	};

}

#endif