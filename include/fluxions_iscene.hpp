#ifndef FLUXIONS_ISCENE_HPP
#define FLUXIONS_ISCENE_HPP

#include <vector>

namespace Fluxions {
	// SCENE INTERFACE
	class IScene {
	public:
		virtual bool Load(const char* filename) = 0;
		virtual bool Save(const char* filename) = 0;

		struct Light {};

		struct Camera {};

		struct Mesh {};

		struct MtlLib {};

		std::vector<Camera> Cameras;
		std::vector<Mesh> Meshes;
		std::vector<MtlLib> MtlLibs;
	};
}

#endif
