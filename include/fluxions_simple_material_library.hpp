#ifndef FLUXIONS_SIMPLE_MATERIAL_LIBRARY_HPP
#define FLUXIONS_SIMPLE_MATERIAL_LIBRARY_HPP

#include <fluxions_stdcxx.hpp>
#include <fluxions_resource_manager.hpp>
#include <fluxions_fileio.hpp>
#include <fluxions_simple_material.hpp>
#include <fluxions_simple_map.hpp>

namespace Fluxions {
	struct SimpleMaterialLibrary
	{
		TResourceManager<SimpleMap> maps;
		TResourceManager<SimpleMaterial> mtls;
		FilePathInfo fpi;
		std::string name;
	};
}

#endif