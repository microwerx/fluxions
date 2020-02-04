#ifndef FLUXIONS_RENDERER_UTILITIES_HPP
#define FLUXIONS_RENDERER_UTILITIES_HPP

#include <fluxions_ssg_scene_graph.hpp>
#include <fluxions_renderer_context.hpp>

namespace Fluxions {
	bool RenderImage(RendererContext& rendererContext,
					 SimpleSceneGraph& ssg,
					 const std::string& rendererName,
					 const std::string& rendererConfigName);

	bool RenderCube(RendererContext& rendererContext,
					SimpleSceneGraph& ssg,
					const std::string& rendererName,
					const std::string& rendererConfigName);

	//void RenderCubeShadowMap(const SimpleSceneGraph& ssg,
	//						 RendererTextureCube& scs,
	//						 const RendererConfig& rc);

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
