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
#ifndef FLUXIONS_SIMPLE_TEXTURE_HPP
#define FLUXIONS_SIMPLE_TEXTURE_HPP

#include <fluxions_gte_image.hpp>
#include <fluxions_opengl.hpp>

namespace Fluxions
{
	class RendererTextureObject
	{
	public:
		RendererTextureObject(GLenum whichTarget = GL_TEXTURE_2D);
		~RendererTextureObject();

		void Create();
		void Delete();
		void Bind(GLuint unit, bool applySamplerObjectSettings = false);
		void Unbind();
		void Enable(GLuint unit);
		void Disable(GLuint unit);
		void GenerateMipmaps();

		bool LoadTexture1D(const std::string& filename, bool genMipMap = true);
		bool LoadTexture2D(const std::string& filename, bool genMipMap = true);
		bool LoadTexture1DArray(const std::string& filename, bool genMipMap = true);
		bool LoadTexture2DArray(const std::vector<std::string>& filenames, bool genMipMap = true);
		bool LoadTexture2DArray(const std::string& filename, bool genMipMap = true);
		bool LoadTexture3D(const std::string& filename, int count, bool genMipMap = true);
		bool LoadTextureCubeMap(
			const std::string& filename_pos_x,
			const std::string& filename_pos_y,
			const std::string& filename_pos_z,
			const std::string& filename_neg_x,
			const std::string& filename_neg_y,
			const std::string& filename_neg_z,
			bool genMipMap = true);
		bool LoadTextureCubeMap(const std::vector<std::string>& filenames, bool genMipMap = true);
		bool LoadTextureCoronaCubeMap(const std::string& filename, bool genMipMap = true);

		void SetTexture1D(GLenum format, GLenum type, int width, void* data, bool genMipMap = true);
		void SetTexture2D(GLenum format, GLenum type, int width, int height, void* data, bool genMipMap = true);
		void SetTexture3D(GLenum format, GLenum type, int width, int height, int depth, void* data, bool genMipMap = true);
		void SetTexture1DArray(GLenum format, GLenum type, int width, int count, void* data, bool genMipMap = true);
		void SetTexture2DArray(GLenum format, GLenum type, int width, int height, int count, void* data, bool genMipMap = true);
		void SetTextureCubeMap(GLenum format, GLenum type, int width, int height, void** data, bool genMipMap = true);
		void SetTextureCubeMap(GLenum format, GLenum type, int width, int height, void* posxData, void* posyData, void* poszData, void* negxData, void* negyData, void* negzData, bool genMipMap = true);

		constexpr GLuint GetTextureId() const { return id; }
		constexpr GLenum GetTarget() const { return target; }

		inline void FreeMemory() noexcept { image.reset(); }
		inline Image4f& GetImage() noexcept { return image; }

		RendererSamplerObject samplerObject;

	private:
		GLuint id;
		GLenum target;

		Image4f image;

		bool didApplySamplerSettings = false;
		GLuint lastBoundUnit = 0;
	};

	extern RendererSamplerObject DefaultSamplerObject;
	extern RendererTextureObject DefaultTextureObject;
} // namespace Fluxions

#endif // FLUXIONS_TEXTURE_HPP
