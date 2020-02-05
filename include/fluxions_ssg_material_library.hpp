#ifndef FLUXIONS_SSG_MATERIAL_LIBRARY_HPP
#define FLUXIONS_SSG_MATERIAL_LIBRARY_HPP

#include <fluxions_ssg_base.hpp>
#include <fluxions_ssg_material.hpp>
#include <fluxions_ssg_map.hpp>

namespace Fluxions {
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

		int getMaterialIndex(const std::string& materialName) const;
	private:
		SimpleMaterial* set_mtl(std::string&& name);
		// Looks for map location, modifies mapname to be user friendly
		bool readMap(std::string& pathToMap, std::string& mapname, const std::string& mtlpath);
		std::ostream& printMap(std::ostream& ostr, const std::string& maptype, const std::string& mapname) const;
	};
}

#endif
