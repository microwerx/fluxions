#ifndef FLUXIONS_RENDER_UTILITIES_HPP
#define FLUXIONS_RENDER_UTILITIES_HPP

#include <fluxions_simple_scene_graph.hpp>
#include <fluxions_simple_cube_texture.hpp>

namespace Fluxions
{
	void RenderCubeShadowMap(const SimpleSceneGraph& ssg,
							 SimpleCubeTexture& scs, 
							 const SimpleRenderConfig& rc);

	bool SaveTextureMap(GLenum target,
						GLuint id,
						const std::string& path);

	void ConvertBuffer(GLenum srccomponents,
					   GLenum srctype,
					   const void* srcdata,
					   GLenum dstcomponents,
					   GLenum dsttype,
					   void* dstdata,
					   GLsizei w,
					   GLsizei h,
					   GLsizei layers = 1);
}

#endif