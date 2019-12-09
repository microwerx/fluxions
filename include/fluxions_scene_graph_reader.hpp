#ifndef FLUXIONS_SCENE_GRAPH_READER_HPP
#define FLUXIONS_SCENE_GRAPH_READER_HPP

#include <fluxions_stdcxx.hpp>
#include <fluxions_file_reader_writer.hpp>

namespace Fluxions
{
class SimpleSceneGraph;

// Older Corona Inspired Plain Text Scene Graph Reader
struct SceneGraphReader : public TFileReader<SimpleSceneGraph> {
	virtual bool open(const char* path) override;
	virtual void close() override;
	virtual bool read(SimpleSceneGraph* ssg) override;
private:
	std::ifstream fin;
};

// Newer Corona Inspired Xml Based Scene Graph Reader
struct XmlSceneGraphReader : public SceneGraphReader {
	virtual bool read(SimpleSceneGraph* ssg) override;
};

}

#endif
