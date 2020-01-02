#ifndef FLUXIONS_SIMPLE_MATERIAL_LIBRARY_HPP
#define FLUXIONS_SIMPLE_MATERIAL_LIBRARY_HPP

#include <fluxions_stdcxx.hpp>
#include <fluxions_resource_manager.hpp>
#include <fluxions_file_path_info.hpp>
#include <fluxions_simple_material.hpp>
#include <fluxions_simple_map.hpp>

namespace Fluxions
{
	struct SimpleMaterialLibrary {
		// texture maps [k, v] = [map name, path to image]
		std::map<std::string, std::string> maps;

		// material libraries [k, v] = [mtllib name, path to MTL]
		std::map<std::string, std::string> mtllibs;

		// materials
		std::vector<SimpleMaterial> mtls;

		static constexpr int WRITE_MATERIALS = 0x0001;
		static constexpr int WRITE_MAPS = 0x0002;
		static constexpr int WRITE_SPECULAR = 0x0004;

		void clear();
		bool loadMtls(const std::map<std::string, std::string>& mtlsToLoad);
		bool load(const std::string& path);
		bool saveMTL(const std::string& path) const;
		bool saveXml(const std::string& path, int flags) const;
	private:
		SimpleMaterial* set_mtl(std::string&& name);
		// Looks for map location, modifies mapname to be user friendly
		bool readMap(std::istream& istr, std::string& mapname, std::string& mtlpath);
		std::ostream& printMap(std::ostream& ostr, const std::string& maptype, const std::string& mapname) const;
	};
}

#endif