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
#include <fluxions_opengl.hpp>
#include <fluxions_es2_state_info.hpp>
#include <fluxions_es2_renderer.hpp>

namespace Fluxions
{
namespace ES2
{
// RESOURCES ////////////////////////////////////////////////////////
enum class ResourceType
{
	NONE,
	IMAGE_2D,
	IMAGE_CUBE_MAP,
	ARRAY_BUFFER,
	ELEMENT_ARRAY_BUFFER,
};

class Resource
{
  protected:
	GLuint resourceId;

  public:
	Resource() : resourceId(0)
	{
		Create();
	}
	virtual ~Resource() { Delete(); }

	virtual ResourceType GetType() { return ResourceType::NONE; }
	virtual GLuint GetHandle() { return resourceId; }
	virtual GLenum GetTarget() { return 0; }
	virtual void Create() {}
	virtual void Delete() { resourceId = 0; }
	virtual void Bind() {}
};

class TextureResource : public Resource
{
  public:
	TextureResource() {}
	virtual ~TextureResource() {}

	virtual void Create() { glGenTextures(1, &resourceId); }
	virtual void Delete()
	{
		glDeleteTextures(1, &resourceId);
		Resource::Delete();
	}
};

class Image2DResource : public TextureResource
{
  public:
	Image2DResource() {}
	virtual ~Image2DResource() {}

	virtual ResourceType GetType() { return ResourceType::IMAGE_2D; }
	virtual GLenum GetTarget() { return GL_TEXTURE_2D; }

	void SetImageData(int width, int height, int internalformat, GLenum format, GLenum type, void *data)
	{
		glBindTexture(GL_TEXTURE_2D, resourceId);
		glTexImage2D(GL_TEXTURE_2D, 0, internalformat, width, height, 0, format, type, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void Bind() { glBindTexture(GL_TEXTURE_2D, resourceId); }
};

class ImageCubeMapResource : public TextureResource
{
  public:
	ImageCubeMapResource() {}
	virtual ~ImageCubeMapResource() {}

	virtual ResourceType GetType() { return ResourceType::IMAGE_CUBE_MAP; }
	virtual GLenum GetTarget() { return GL_TEXTURE_CUBE_MAP; }

	void SetImageData(int width, int height, int internalformat, GLenum format, GLenum type,
					  void *pPosX, void *pPosY, void *pPosZ, void *pNegX, void *pNegY, void *pNegZ)
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP, resourceId);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, internalformat, width, height, 0, format, type, pPosX);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, internalformat, width, height, 0, format, type, pPosY);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, internalformat, width, height, 0, format, type, pPosZ);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, internalformat, width, height, 0, format, type, pNegX);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, internalformat, width, height, 0, format, type, pNegY);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, internalformat, width, height, 0, format, type, pNegZ);
		glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}

	void Bind() { glBindTexture(GL_TEXTURE_CUBE_MAP, resourceId); }
};

class BufferResource : public Resource
{
  public:
	BufferResource() {}
	virtual ~BufferResource() {}

	virtual void Create() { glGenBuffers(1, &resourceId); }
	virtual void Delete()
	{
		glDeleteBuffers(1, &resourceId);
		resourceId = 0;
	}
	virtual void SetBufferData(GLsizeiptr size, const void *data, GLenum usage) {}
};

class ArrayBufferResource : public BufferResource
{
  public:
	ArrayBufferResource() {}
	virtual ~ArrayBufferResource() {}

	virtual ResourceType GetType() { return ResourceType::ARRAY_BUFFER; }

	virtual void SetBufferData(GLsizeiptr size, const void *data, GLenum usage)
	{
		glBindBuffer(GL_ARRAY_BUFFER, resourceId);
		glBufferData(GL_ARRAY_BUFFER, size, data, usage);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	virtual void Bind() { glBindBuffer(GL_ARRAY_BUFFER, resourceId); }
};

class ElementArrayBuffer : public BufferResource
{
  public:
	ElementArrayBuffer() {}
	virtual ~ElementArrayBuffer() {}

	virtual ResourceType GetType() { return ResourceType::ELEMENT_ARRAY_BUFFER; }

	virtual void SetBufferData(GLsizeiptr size, const void *data, GLenum usage)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, resourceId);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, usage);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	virtual void Bind() { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, resourceId); }
};

// SHADERS //////////////////////////////////////////////////////////

// UNIFORM SETS /////////////////////////////////////////////////////

// DESCRIPTOR SETS //////////////////////////////////////////////////

// PIPELINES ////////////////////////////////////////////////////////

// COMMAND BUFFERS ////////////////////////////////////////////

// RENDER PASSES ////////////////////////////////////////////////////
} // namespace ES2

struct ES2Renderer::Impl
{
	std::vector<std::shared_ptr<ES2::Resource>> resources; // i.e. used to store memory buffers and images
	//	std::vector<std::shared_ptr<Shader>> shaders;				// i.e. used to store the shaders
	//	std::vector<std::shared_ptr<StateSet>> stateSets;			// i.e. what's the depth test, etc. set to?
	//	std::vector<std::shared_ptr<UniformSet>> uniformSets;		// i.e. what are the uniforms set to for the shader?
	//	std::vector<std::shared_ptr<DescriptorSet>> descriptorSets;	// i.e. where are the textures bound?, parameters set to?
	//	std::vector<std::shared_ptr<Pipeline>> pipelines;			// i.e. what's the currently bound program? default states? default descriptors? default uniforms?
	//	std::vector<std::shared_ptr<CommandBuffer>> commandBuffers;	// i.e. a list of commands to execute
	//	std::vector<std::shared_ptr<RenderPass>> renderPasses;		// i.e. framebuffer objects and a list of commands to execute

	void Clear()
	{
		//renderPasses.clear();
		//commandBuffers.clear();
		//pipelines.clear();
		//descriptorSets.clear();
		//uniformSets.clear();
		//stateSets.clear();
		//shaders.clear();
		resources.clear();
	}
};

ES2Renderer::ES2Renderer() : pImpl(std::make_unique<Impl>())
{
}

ES2Renderer::~ES2Renderer()
{
	// Delete these in reverse order...
	//	std::vector<std::shared_ptr<Resource>> resources;			// i.e. used to store memory buffers and images
	//	std::vector<std::shared_ptr<Shader>> shaders;				// i.e. used to store the shaders
	//	std::vector<std::shared_ptr<StateSet>> stateSets;			// i.e. what's the depth test, etc. set to?
	//	std::vector<std::shared_ptr<UniformSet>> uniformSets;		// i.e. what are the uniforms set to for the shader?
	//	std::vector<std::shared_ptr<DescriptorSet>> descriptorSets;	// i.e. where are the textures bound?, parameters set to?
	//	std::vector<std::shared_ptr<Pipeline>> pipelines;			// i.e. what's the currently bound program? default states? default descriptors? default uniforms?
	//	std::vector<std::shared_ptr<CommandBuffer>> commandBuffers;	// i.e. a list of commands to execute
	//	std::vector<std::shared_ptr<RenderPass>> renderPasses;		// i.e. framebuffer objects and a list of commands to execute

	pImpl->Clear();
}

// Let the compiler make a destructor and move constructors for us
#ifdef FLUXIONS_CPP14
ES2Renderer::ES2Renderer(ES2Renderer &&rhs) = default;			  // move ctr
ES2Renderer &ES2Renderer::operator=(ES2Renderer &&rhs) = default; // move
																  //ES2Renderer::ES2Renderer(const ES2Renderer& rhs) = default;				// copy ctr
																  //ES2Renderer & ES2Renderer::operator=(const ES2Renderer& rhs) = default;	// copy
#endif

void ES2Renderer::Render(const IScene *pScene)
{
}
} // namespace Fluxions
