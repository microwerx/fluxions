#include "fluxions_ssg_pch.hpp"
#include <fluxions_ssg_reader.hpp>

namespace Fluxions {
	bool SceneGraphReader::open(const char* path) {
		fin.open(path);
		return (bool)fin;
	}

	void SceneGraphReader::close() {
		fin.close();
	}

	bool SceneGraphReader::read(SimpleSceneGraph* ssg) {
		if (!ssg) return false;
		return true;
	}

	bool XmlSceneGraphReader::read(SimpleSceneGraph* ssg) {
		if (!ssg) return false;
		return true;
	}
}
