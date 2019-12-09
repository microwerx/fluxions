#include "pch.hpp"
#include <fluxions_scene_graph_reader.hpp>
#include <fluxions_simple_scene_graph.hpp>

namespace Fluxions
{

bool SceneGraphReader::open(const char* path) {
	fin.open(path);
	return (bool)fin;
}

void SceneGraphReader::close() {
	fin.close();
}

bool SceneGraphReader::read(SimpleSceneGraph* ssg) {
	return true;
}

bool XmlSceneGraphReader::read(SimpleSceneGraph* ssg) {
	return true;
}

}