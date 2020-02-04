#include "fluxions_pch.hpp"
#include <fluxions_opengl.hpp>
#include <fluxions_es2_state_info.hpp>
#include <fluxions_es2_renderer.hpp>

namespace Fluxions {
	namespace ES2 {
		// RESOURCES ////////////////////////////////////////////////////////
		enum class ResourceType {
			NONE,
			IMAGE_2D,
			IMAGE_CUBE_MAP,
			ARRAY_BUFFER,
			ELEMENT_ARRAY_BUFFER,
		};

		class Resource {
		protected:
			GLuint resourceId{ 0 };

		public:
			Resource() {
				Create();
			}

			virtual ~Resource() {
				Delete();
			}

			virtual ResourceType GetType() {
				return ResourceType::NONE;
			}

			virtual GLuint GetHandle() {
				return resourceId;
			}

			virtual GLenum GetTarget() {
				return 0;
			}

			virtual void Create() {}

			virtual void Delete() {
				resourceId = 0;
			}

			virtual void Bind() {}
		};

		class TextureResource : public Resource {
		public:
			TextureResource() {}

			virtual ~TextureResource() override {}

			virtual void Create() override {
			}

			virtual void Delete() override {
				FxDeleteTexture(&resourceId);
				Resource::Delete();
			}
		};

		class Image2DResource : public TextureResource {
		public:
			Image2DResource() {}

			virtual ~Image2DResource() override {}

			virtual ResourceType GetType() override {
				return ResourceType::IMAGE_2D;
			}

			virtual GLenum GetTarget() override {
				return GL_TEXTURE_2D;
			}

			void SetImageData(int width, int height, int internalformat, GLenum format, GLenum type, void* data) {
				FxCreateTexture(GL_TEXTURE_2D, &resourceId);
				glTexImage2D(GL_TEXTURE_2D, 0, internalformat, width, height, 0, format, type, data);
				FxGenerateMipmap(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, 0);
			}

			virtual void Bind() override {
				glBindTexture(GL_TEXTURE_2D, resourceId);
			}
		};

		class ImageCubeMapResource : public TextureResource {
		public:
			ImageCubeMapResource() {}

			virtual ~ImageCubeMapResource() override {}

			virtual ResourceType GetType() override {
				return ResourceType::IMAGE_CUBE_MAP;
			}

			virtual GLenum GetTarget() override {
				return GL_TEXTURE_CUBE_MAP;
			}

			void SetImageData(int width, int height,
							  int internalformat,
							  GLenum format,
							  GLenum type,
							  void* pPosX, void* pPosY, void* pPosZ,
							  void* pNegX, void* pNegY, void* pNegZ) {
				FxCreateTexture(GL_TEXTURE_CUBE_MAP, &resourceId);
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, internalformat, width, height, 0, format, type, pPosX);
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, internalformat, width, height, 0, format, type, pPosY);
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, internalformat, width, height, 0, format, type, pPosZ);
				glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, internalformat, width, height, 0, format, type, pNegX);
				glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, internalformat, width, height, 0, format, type, pNegY);
				glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, internalformat, width, height, 0, format, type, pNegZ);
				FxGenerateMipmap(GL_TEXTURE_CUBE_MAP);
				glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
			}

			virtual void Bind() override {
				glBindTexture(GL_TEXTURE_CUBE_MAP, resourceId);
			}
		};

		class BufferResource : public Resource {
		private:
			GLenum target;

		public:
			BufferResource(GLenum target)
				: target(target) {}

			virtual ~BufferResource() override {}

			virtual void Create() override {
			}

			virtual void Delete() override {
				FxDeleteBuffer(&resourceId);
			}

			virtual void SetBufferData(GLsizeiptr size, const void* data, GLenum usage) {
				FxCreateBuffer(target, &resourceId, size, data, usage);
			}

			virtual void Bind() override {
				glBindBuffer(target, resourceId);
			}
		};

		class ArrayBufferResource : public BufferResource {
		public:
			ArrayBufferResource()
				: BufferResource(GL_ARRAY_BUFFER) {}

			virtual ~ArrayBufferResource() override {}

			virtual ResourceType GetType() override {
				return ResourceType::ARRAY_BUFFER;
			}
		};

		class ElementArrayBuffer : public BufferResource {
		public:
			ElementArrayBuffer()
				: BufferResource(GL_ELEMENT_ARRAY_BUFFER) {}

			virtual ~ElementArrayBuffer() override {}

			virtual ResourceType GetType() override {
				return ResourceType::ELEMENT_ARRAY_BUFFER;
			}
		};

		// SHADERS //////////////////////////////////////////////////////////

		// UNIFORM SETS /////////////////////////////////////////////////////

		// DESCRIPTOR SETS //////////////////////////////////////////////////

		// PIPELINES ////////////////////////////////////////////////////////

		// COMMAND BUFFERS ////////////////////////////////////////////

		// RENDER PASSES ////////////////////////////////////////////////////
	} // namespace ES2

	struct ES2Renderer::Impl {
		std::vector<std::shared_ptr<ES2::Resource>> resources; // i.e. used to store memory buffers and images
		//	std::vector<std::shared_ptr<Shader>> shaders;				// i.e. used to store the shaders
		//	std::vector<std::shared_ptr<StateSet>> stateSets;			// i.e. what's the depth test, etc. set to?
		//	std::vector<std::shared_ptr<UniformSet>> uniformSets;		// i.e. what are the uniforms set to for the shader?
		//	std::vector<std::shared_ptr<DescriptorSet>> descriptorSets;	// i.e. where are the textures bound?, parameters set to?
		//	std::vector<std::shared_ptr<Pipeline>> pipelines;			// i.e. what's the currently bound program? default states? default descriptors? default uniforms?
		//	std::vector<std::shared_ptr<CommandBuffer>> commandBuffers;	// i.e. a list of commands to execute
		//	std::vector<std::shared_ptr<RenderPass>> renderPasses;		// i.e. framebuffer objects and a list of commands to execute

		void Clear() {
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

	ES2Renderer::ES2Renderer() : pImpl(std::make_unique<Impl>()) {}

	ES2Renderer::~ES2Renderer() {
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
	ES2Renderer::ES2Renderer(ES2Renderer&& rhs) = default;			  // move ctr
	ES2Renderer& ES2Renderer::operator=(ES2Renderer&& rhs) = default; // move
																	  //ES2Renderer::ES2Renderer(const ES2Renderer& rhs) = default;				// copy ctr
																	  //ES2Renderer & ES2Renderer::operator=(const ES2Renderer& rhs) = default;	// copy
#endif

	void ES2Renderer::Render(const IScene* pScene) {
		if (!pScene)
			return;
	}
} // namespace Fluxions
