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
#ifndef FLUXIONS_SIMPLE_MATERIALS_HPP
#define FLUXIONS_SIMPLE_MATERIALS_HPP

#include <fluxions_fileio.hpp>
#include <fluxions_simple_property.hpp>
#include <fluxions_resource_manager.hpp>
#include <fluxions_renderer_texture.hpp>
#include <fluxions_simple_material.hpp>
#include <fluxions_simple_material_library.hpp>

namespace Fluxions
{

	std::ostream& WriteMaterial(std::ostream& ostr, const SimpleMaterial& mtl);
	void SetMaterialDefaults(SimpleMaterial& mtl);

	class SimpleMaterialSystem
	{
	private:
		std::string defaultName = "<INVALIDMATERIAL>";
		std::string currentMtlLibName;
		GLuint currentMtlLibId = 0;
		std::string currentMtlName;
		GLuint currentMtlId = 0;

		SimpleMaterialLibrary* currentMtlLibPtr = nullptr;
		SimpleMaterial* currentMtlPtr = nullptr;

		TResourceManager<SimpleMaterialLibrary> mtllibs;
		TResourceManager<std::string> shaderMaps;

		std::map<std::string, std::string> maps_paths;
		std::string defaultMapPath = "<UNKNOWN>";

		void SynchronizeIds();

	public:
		using iterator = typename TResourceManager<SimpleMaterialLibrary>::iterator;
		using const_iterator = typename TResourceManager<SimpleMaterialLibrary>::const_iterator;
		using value_type = typename TResourceManager<SimpleMaterialLibrary>::value_type;
		using key_type = typename TResourceManager<SimpleMaterialLibrary>::key_type;
		using mapped_type = typename TResourceManager<SimpleMaterialLibrary>::mapped_type;

		SimpleMaterialSystem();
		~SimpleMaterialSystem();

		void Clear();

		iterator begin() { return mtllibs.begin(); }
		iterator end() { return mtllibs.end(); }
		const_iterator begin() const { return mtllibs.begin(); }
		const_iterator end() const { return mtllibs.end(); }
		unsigned size() { return mtllibs.size(); }

		bool Save(const std::string& path);
		bool Load(const std::string& mtllibName, const std::string& filename);
		bool Save(const std::string& mtllibName, const std::string& filename);

		SimpleMaterialLibrary* CreateLibrary(const std::string& name);
		SimpleMaterial* CreateMaterial(const std::string& name);

		void DeleteLibrary(const std::string& name);
		void DeleteMaterial(const std::string& name);
		void DeleteLibraryMaterial(const std::string& mtllibName, const std::string& mtlName);

		SimpleMaterialLibrary* SetLibrary(const std::string& name);
		SimpleMaterial* SetMaterial(const std::string& name);
		SimpleMaterial* SetLibraryMaterial(const std::string& mtllibName, const std::string& mtlName);
		const SimpleMaterial* GetLibraryMaterial(const std::string& mtllibName, const std::string& mtlName) const;
		SimpleMaterial* GetCurrentMaterial() { return currentMtlPtr; }

		const std::string& GetLibraryName() const;
		const std::string& GetMaterialName() const;
		const std::string& GetMaterialName(GLuint id) const;

		GLuint GetLibraryId() const;
		GLuint GetMaterialId() const;
		GLuint GetLibraryId(const std::string& name) const;
		GLuint GetMaterialId(const std::string& name) const;
		GLuint GetLibraryMaterialId(const std::string& mtllibName, const std::string& mtlName) const;

		bool AddMap(const std::string& path, const std::string& filename);
		bool AddMapShader(const std::string& name, const std::string& shader);
		void LoadMaps();
		SimpleMap* GetTextureMap(const std::string& name);
		const SimpleMap* GetTextureMap(const std::string& name) const;

		const std::string& GetMapPath(const std::string& mapName) const;
	};
} // namespace Fluxions

#endif
