#include "fluxions_ssg_pch.hpp"
#include <hatchetfish_stopwatch.hpp>
#include <fluxions_ssg_scene_graph.hpp>

#ifdef __APPLE__
#define __unix__
#endif

namespace Fluxions {
	//struct SimpleRendererUniforms {
	//	/* This was this code, but g++ doesn't like it. The better solution would be
	//			 * to create a pack() function which creates a memory buffer with the
	//	   contents
	//			 * packed into it.
	//			union {
	//					struct {
	//							Matrix4f projectionMatrix;
	//							Matrix4f cameraMatrix_;
	//							Matrix4f worldMatrix;
	//							Matrix4f projectionViewMatrix;
	//							Matrix4f modelViewMatrix;
	//							Matrix4f projectionModelViewMatrix;

	//							Vector4f cameraPosition;
	//							Vector4f lightPositions[32];
	//							bool lightEnabled[32];
	//					};
	//					GLubyte data[4096];
	//			};
	//			*/

	//	Matrix4f projectionMatrix;
	//	Matrix4f cameraMatrix;
	//	Matrix4f worldMatrix;
	//	Matrix4f projectionViewMatrix;
	//	Matrix4f modelViewMatrix;
	//	Matrix4f projectionModelViewMatrix;

	//	Vector4f cameraPosition;
	//	Vector4f lightPositions[32];
	//	bool lightEnabled[32];

	//	void ComputeExtraValues() {
	//		projectionViewMatrix = projectionMatrix * cameraMatrix;
	//		modelViewMatrix = cameraMatrix * worldMatrix;
	//		projectionModelViewMatrix = projectionMatrix * cameraMatrix * worldMatrix;
	//		cameraPosition = cameraMatrix * Vector4f(0.0f, 0.0f, 0.0f, 1.0f);
	//	}

	//	void reset() {
	//		projectionMatrix.LoadIdentity();
	//		cameraMatrix.LoadIdentity();
	//		worldMatrix.LoadIdentity();
	//		projectionViewMatrix.LoadIdentity();
	//		modelViewMatrix.LoadIdentity();
	//		projectionModelViewMatrix.LoadIdentity();
	//	}
	//};

	SimpleSceneGraph::SimpleSceneGraph() {}


	SimpleSceneGraph::~SimpleSceneGraph() {
		reset();
	}


	void SimpleSceneGraph::reset() {
		boundingBox.reset();
		sceneFileLines.clear();
		//pathsToTry.clear();
		spheres.clear();
		currentTransform.LoadIdentity();
		geometryGroups.clear();
		//shaderMaps.clear();
		staticMeshes.clear();
		pointLights.clear();
		anisoLights.clear();

		if (userdata) {
			userdata->reset();
		}

		// set time to longest time of the day
		Sf::PA::CivilDateTime dtg{ 1, 7, 2017, true, 0, 16, 0, 0, 0.0f };
		environment.setCivilDateTime(dtg);
		// shaderMaps.clear();
		// geometryGroups_.clear();
	}


	std::string SimpleSceneGraph::_findPath(std::string path) const {
		std::string p;
		FilePathInfo fpi(path, pathsToTry);
		if (fpi.exists()) p = fpi.shortestPath();
		return fpi.shortestPath();
	}


	void SimpleSceneGraph::addPath(const std::string& path) {
		FilePathInfo fpi(path);
		if (!fpi.isDirectory()) return;
		if (std::find(pathsToTry.begin(), pathsToTry.end(), fpi.shortestPath()) == pathsToTry.end()) {
			pathsToTry.push_back(path);
		}
	}


	bool SimpleSceneGraph::Load(const std::string& filename) {
		FilePathInfo scenefpi(filename);
		if (!scenefpi.exists()) return false;

		lastWriteTime = scenefpi.lastWriteTime();

		HFLOGINFO("Loading '%s'", scenefpi.shortestPathC());
		setName(scenefpi.stem());
		std::ifstream fin(filename.c_str());

		if (!fin) {
			return false;
		}

		// Loading is successful, so go ahead and add path of the scene graph
		// file so we can reference it if necessary for other referenced files
		// in the mtllibs and objs.
		reset();
		// Use the path of the scene graph as the default path, then the current
		// directory
		pathsToTry.push_back(scenefpi.parentPath());

		static constexpr int LOADED_DATETIME = 1;
		static constexpr int LOADED_CAMERA = 2;
		static constexpr int LOADED_GEOMETRY_GROUP = 4;
		static constexpr int LOADED_ENVIRONMENT = 8;
		int flags = 0;

		// proceed to parse the file and read each line.
		while (fin) {
			// read a line from the file and set up a string stream.
			std::string line;
			getline(fin, line);
			if (!fin && !fin.eof())
				break;
			if (line.empty())
				continue;
			if (line[0] == '#')
				continue;

			sceneFileLines.push_back(line);

			std::istringstream istr(line);
			std::string token;
			istr >> token;

			if (read(token, istr)) {
			}
			else if (token == "path") {
				pathsToTry.push_back(ReadString(istr));
			}
			else if (token == "transform") {
				currentTransform = ReadAffineMatrix4f(istr);
			}
			else if (token == "identity") {
				currentTransform.LoadIdentity();
			}
			else if (token == "translate") {
				currentTransform.Translate(
					ReadFloat(istr),
					ReadFloat(istr),
					ReadFloat(istr));
			}
			else if (token == "rotate") {
				currentTransform.Rotate(
					ReadFloat(istr),
					ReadFloat(istr),
					ReadFloat(istr),
					ReadFloat(istr));
			}
			else if (token == "mtllib") {
				ReadMaterialLibrary(token, istr);
			}
			else if (token == "geometryGroup") {
				flags |= LOADED_GEOMETRY_GROUP;
				ReadGeometryGroup(token, istr);
			}
			else if (token == "pathanim") {
				currentTransform.LoadIdentity();
				ReadPath(token, istr);
			}
			else if (token == "enviro") {
				ReadEnviro(token, istr);
			}
			else if (token == "pbsky") {
				flags |= LOADED_ENVIRONMENT;
				ReadEnviroPbsky(token, istr);
			}
			else if (token == "datetime") {
				flags |= LOADED_DATETIME;
				ReadEnviroDatetime(token, istr);
			}
			else if (token == "camera") {
				flags |= LOADED_CAMERA;
				currentTransform.LoadIdentity();
				ReadCamera(token, istr);
			}
			else if (token == "tonemap") {
				environment.toneMapExposure() = clamp(ReadFloat(istr), -20.0f, 20.0f);
			}
			else if (token == "gamma") {
				environment.toneMapGamma() = clamp(ReadFloat(istr), -6.0f, 6.0f);
			}
			else if (token == "filmicHighlights") {
				environment.toneMapFilmicHighlights() = clamp(ReadFloat(istr), 0.0f, 1.0f);
			}
			else if (token == "filmicShadows") {
				environment.toneMapFilmicShadows() = clamp(ReadFloat(istr), 0.0f, 1.0f);
			}
			else if (token == "sun" || token == "moon") {
				currentTransform.LoadIdentity();
				ReadOldDirectionalLight(token, istr);
			}
			else if (token == "sphere") {
				ReadSphere(token, istr);
			}
			else if (token == "pointLight" || token == "light") {
				currentTransform.LoadIdentity();
				ReadPointLight(token, istr);
			}
			else if (token == "dirtoLight" || token == "dirToLight") {
				currentTransform.LoadIdentity();
				ReadDirToLight(token, istr);
			}
			else if (token == "anisoLight") {
				currentTransform.LoadIdentity();
				ReadAnisoLight(token, istr);
			}
			else if (userdata) {
				userdata->read(token, istr);
			}
		}

		fin.close();

		if (!(flags & LOADED_DATETIME)) HFLOGWARN("Did not specify datetime");
		if (!(flags & LOADED_GEOMETRY_GROUP)) HFLOGWARN("Did not specify geometry");
		if (!(flags & LOADED_CAMERA)) HFLOGWARN("Did not specify camera");
		if (!(flags & LOADED_ENVIRONMENT)) HFLOGWARN("Did not specify environment");

		calcBounds();

		_assignIdsToMeshes();

		// Load Images
		//materialSystem.FindMapPaths(pathsToTry);

		return true;
	}


	bool SimpleSceneGraph::Save(const std::string& filename) {
		FilePathInfo fpi(filename);

		std::ofstream fout(filename.c_str());
		if (!fout)
			return false;
		write(fout);
		fout.close();

		// 6. Output geometryGroups_ files
		for (auto& [k, mesh] : staticMeshes) {
			mesh.saveOBJ(fpi.parentPath() + mesh.name + ".obj");
		}

		// 7. Output mtllib files
		materials.saveMTL(fpi.parentPath() + name_str() + ".mtl");

		return true;

		// output SCN information...

		//// 1. Camera
		//fout << "camera ";
		//if (camera.isOrtho) {
		//	fout << "ortho_tmw ";
		//	WriteAffineMatrix4f(fout, camera.viewMatrix_);
		//	fout << camera.width;
		//}
		//else {
		//	fout << "perspective_tmf ";
		//	WriteAffineMatrix4f(fout, camera.viewMatrix_);
		//	fout << camera.fov;
		//}
		//fout << "\n";

		//for (auto& [k, n] : cameras) {
		//	n.write(fout);
		//}

		//// 2. Environment
		//if (environment.hasColor) {
		//	fout << "enviro color ";
		//	WriteColor3f(fout, environment.color);
		//	fout << "\n";
		//}
		//if (environment.hasTexmap) {
		//	fout << "enviro texmap ";
		//	WriteString(fout, environment.texmap);
		//	fout << "\n";
		//}
		//if (environment.hasSun) {
		//	fout << "sun dirTo ";
		//	WriteVector3f(fout, environment.curSunDirTo);
		//	fout << "color ";
		//	WriteColor3f(fout, environment.sunColor);
		//	fout << "sizeMult ";
		//	WriteDouble(fout, environment.sunSize);
		//	fout << "\n";
		//}

		//// 3. Newmaps
		//for (auto it = shaderMaps.begin(); it != shaderMaps.end(); it++) {
		//	fout << "newmap " << it->first << " ";
		//	WriteString(fout, it->second);
		//	fout << "\n";
		//}

		//// 4. Spheres
		//for (auto it = spheres.begin(); it != spheres.end(); it++) {
		//	SimpleSphere& sphere = it->second;
		//	if (materialSystem.GetLibraryId() != sphere.mtllibId) {
		//		materialSystem.SetLibrary(sphere.mtllibName);

		//		fout << "mtllib " << sphere.mtllibName << "\n";
		//	}
		//	fout << "transform ";
		//	WriteAffineMatrix4f(fout, sphere.transform);
		//	fout << "\n";
		//	fout << "sphere ";
		//	WriteString(fout, sphere.mtlName);
		//	fout << "\n";
		//}

		//// 5. Geometry Groups
		//for (auto it = geometryGroups_.begin(); it != geometryGroups_.end(); it++) {
		//	SimpleGeometryGroup& geo = it->second;
		//	if (materialSystem.GetLibraryId() != geo.mtllibId) {
		//		materialSystem.SetLibrary(geo.mtllibName);

		//		fout << "mtllib " << geo.mtllibName << "\n";
		//	}
		//	fout << "transform ";
		//	WriteAffineMatrix4f(fout, geo.transform);
		//	fout << "\n";
		//	fout << "geometryGroup " << geo.objectName << "\n";
		//}
		//fout.close();

		//// 6. Output geometryGroups_ files
		//for (auto it = geometryGroups_.begin(); it != geometryGroups_.end(); it++) {
		//	SimpleGeometryGroup& geo = it->second;
		//	staticMeshes[geo.objectId].saveOBJ(geo.objectName);
		//}

		//// 7. Output mtllib files
		//materialSystem.Save(fpi.dir);

		//return true;
	}


	bool SimpleSceneGraph::Load(const char* path, SceneGraphReader* reader) {
		if (!reader)
			return false;
		bool result = true;
		if (result && !reader->open(path))
			result = false;
		if (result && !reader->read(this))
			result = false;
		reader->close();
		return result;
	}


	bool SimpleSceneGraph::Save(const char* path, SceneGraphWriter* writer) const {
		if (!writer)
			return false;
		bool result = true;
		if (result && !writer->open(path))
			result = false;
		if (result && !writer->write(this))
			result = false;
		writer->close();
		return result;
	}


	bool SimpleSceneGraph::ReadObjFile(const std::string& filename,
									   const std::string& geometryName) {
		if (staticMeshes.isAHandle(filename))
			return true;

		SimpleGeometryMesh model;
		bool result = model.loadOBJ(filename);
		if (!result) {
			return false;
		}

		materials.loadMtls(model.mtllibs);

		staticMeshes[geometryName] = model;
		return true;
	}


	bool SimpleSceneGraph::ReadMaterialLibrary(const std::string& type, std::istream& istr) {
		if (type != "mtllib")
			return false;
		std::string path = ReadString(istr);
		FilePathInfo fpi(path, pathsToTry);
		if (fpi.notFound()) {
			HFLOGERROR("MTLLIB '%s' was not found.", path.c_str());
			return false;
		}
		if (materials.load(fpi.shortestPath())) {
			return true;
		}
		return false;
	}


	bool SimpleSceneGraph::ReadGeometryGroup(const std::string& type, std::istream& istr) {
		if (type != "geometryGroup")
			return false;

		std::string path = ReadString(istr);
		FilePathInfo fpi(path, pathsToTry);
		if (fpi.notFound()) {
			HFLOGERROR("OBJ file '%s' was not found.", path.c_str());
			return false;
		}
		return addGeometryGroup(fpi.stem(), fpi.shortestPath());
	}


	void SimpleSceneGraph::calcBounds() {
		boundingBox.reset();

		for (auto& [id, group] : geometryGroups) {
			// Transform the bounding box of the model into world coordinates
			Vector3f tminBound = group.worldMatrix() * group.bbox.minBounds;
			Vector3f tmaxBound = group.worldMatrix() * group.bbox.maxBounds;

			boundingBox += tminBound;
			boundingBox += tmaxBound;
		}
	}


	bool SimpleSceneGraph::ReadEnviro(const std::string& type, std::istream& istr) {
		if (type != "enviro")
			return false;
		std::string envirotype;
		istr >> envirotype;
		if (envirotype == "color") {
			environment.hasColor = true;
			environment.color = ReadVector3f(istr);
		}
		else if (envirotype == "texmap") {
			environment.hasTexmap = true;
			environment.texmap = ReadString(istr);
		}
		return true;
	}


	bool SimpleSceneGraph::ReadEnviroPbsky(const std::string& type, std::istream& istr) {
		if (type != "pbsky")
			return false;
		Color3f groundAlbedo;
		float turbidity;
		float latitude;
		float longitude;
		int samples;
		istr >> turbidity;
		istr >> latitude;
		istr >> longitude;
		istr >> samples;
		istr >> groundAlbedo.r;
		istr >> groundAlbedo.g;
		istr >> groundAlbedo.b;

		environment.setNumSamples(samples);
		environment.setLocation(latitude, longitude);
		environment.setTurbidity(turbidity);
		environment.setGroundAlbedo(groundAlbedo);

		if (groundAlbedo.maxrgb() == 0) {
			// TODO: Test if this gets the value according to the month
			groundAlbedo = environment.computeModisAlbedo();
		}

		HFLOGINFO("PBSKY: numSamples=%f, lat/lon=(% 3.2f, % 3.2f), T=%2.1f, A=(%1.2f, %1.2f, %1.2f)",
				  samples,
				  latitude,
				  longitude,
				  turbidity,
				  groundAlbedo.r,
				  groundAlbedo.g,
				  groundAlbedo.b);
		return true;
	}


	bool SimpleSceneGraph::ReadEnviroDatetime(const std::string& type, std::istream& istr) {
		if (type != "datetime")
			return false;
		int month = 7;
		int day = 1;
		int year = 2017;
		int hours = 12;
		int minutes = 0;
		int seconds = 0;
		int isdst = 1;
		istr >> month;
		istr >> day;
		istr >> year;
		istr >> hours;
		istr >> minutes;
		istr >> seconds;
		istr >> isdst;

		Sf::PA::CivilDateTime dtg{ day,month, year, isdst, 0, hours, minutes, seconds, 0.0f };
		environment.setCivilDateTime(dtg);
		HFLOGINFO("Setting date to %04d-%02d-%02dT%02d:%02d:%02d",
				  year, month, day,
				  hours, minutes, seconds);
		return true;
	}


	bool SimpleSceneGraph::ReadCamera(const std::string& keyword, std::istream& istr) {
		if (keyword == "camera") {
			auto node = createCamera(ReadString(istr));
			return node->read(keyword, istr);
		}
		return false;
	}


	bool SimpleSceneGraph::ReadOldDirectionalLight(const std::string& type, std::istream& istr) {
		if (type != "dirTo")
			return false;

		return true;
		// THIS comment code needs to be handled differently
		//std::string sunopt;
		//environment.hasSun = true;
		//sunopt = ReadString(istr);
		//if (sunopt == "sunDirTo") {
		//	environment.sunDirTo = ReadVector3f(istr);
		//}
		//if (sunopt == "moonDirTo") {
		//	environment.moonDirTo = ReadVector3f(istr);
		//}
		//sunopt = ReadString(istr);
		//if (sunopt == "color") {
		//	environment.sunColor = ReadVector3f(istr);
		//}
		//sunopt = ReadString(istr);
		//if (sunopt == "sizeMult") {
		//	environment.sunSize = ReadFloat(istr);
		//}
		//environment.sunDirTo.normalize();
		//environment.moonDirTo.normalize();
		//environment.curSunDirTo = environment.sunDirTo;
		//environment.curMoonDirTo = environment.moonDirTo;

		//environment.sunShadowBiasMatrix.LoadIdentity();
		//environment.sunShadowBiasMatrix.ShadowBias();
		//environment.sunShadowProjectionMatrix.LoadIdentity();
		//environment.sunShadowProjectionMatrix.Perspective(90.0, 1.0, 1.0, 100.0);
		//// environment.sunShadowProjectionMatrix.Ortho(-200, 200, -200, 200, -200,
		//// 200);
		//environment.sunShadowViewMatrix.LoadIdentity();
		//environment.sunShadowViewMatrix.LookAt(
		//	environment.curSunDirTo * environment.sunSize, Vector3f(0, 0, 0),
		//	Vector3f(0, 1, 0));
		//environment.sunShadowInverseViewMatrix =
		//	environment.sunShadowViewMatrix.AsInverse();
		return true;
	}


	bool SimpleSceneGraph::ReadPointLight(const std::string& keyword, std::istream& istr) {
		if (keyword == "pointLight") {
			auto node = createPointLight(ReadString(istr));
			return node->read(keyword, istr);
		}
		return false;
	}


	bool SimpleSceneGraph::ReadAnisoLight(const std::string& keyword, std::istream& istr) {
		if (keyword == "anisoLight") {
			auto node = createAnisoLight(ReadString(istr));
			return node->read(keyword, istr);
		}
		return false;
	}


	bool SimpleSceneGraph::ReadDirToLight(const std::string& keyword, std::istream& istr) {
		if (keyword == "dirtoLight") {
			auto node = createDirToLight(ReadString(istr));
			return node->read(keyword, istr);
		}
		return false;
	}


	bool SimpleSceneGraph::ReadSphere(const std::string& keyword, std::istream& istr) {
		if (keyword == "sphere") {
			auto node = createSphere(ReadString(istr));
			return node->read(keyword, istr);
		}
		return false;
	}


	bool SimpleSceneGraph::ReadPath(const std::string& keyword, std::istream& istr) {
		if (keyword == "pathanim") {
			auto node = createPathAnim(ReadString(istr));
			return node->read(keyword, istr);
		}
		else if (pathanims.lastId > 0) {
			return pathanims[pathanims.lastId].read(keyword, istr);
		}
		return false;
	}


	bool SimpleSceneGraph::read(const std::string& keyword, std::istream& istr) {
		if (keyword.empty() || !istr) return false;
		return false;
	}


	bool SimpleSceneGraph::write(std::ostream& ostr) const {
		WriteLabel(ostr, "mtllib");
		WriteString(ostr, name_str() + ".mtl") << "\n";

		for (auto& [k, n] : cameras) {
			n.write(ostr);
		}

		// 2. Environment
		if (environment.hasColor) {
			ostr << "enviro color ";
			WriteColor3f(ostr, environment.color);
			ostr << "\n";
		}
		if (environment.hasTexmap) {
			ostr << "enviro texmap ";
			WriteString(ostr, environment.texmap);
			ostr << "\n";
		}
		if (environment.hasSun) {
			//environment.write(ostr);
			ostr << "sun dirTo ";
			WriteVector3f(ostr, environment.curSunDirTo);
			ostr << "color ";
			WriteColor3f(ostr, environment.sunColor);
			ostr << "sizeMult ";
			WriteDouble(ostr, environment.sunSize);
			ostr << "\n";
		}

		// 3. Spheres
		for (auto& [k, sphere] : spheres) {
			sphere.write(ostr);
		}

		// 4. Geometry Groups
		for (auto& [k, geo] : geometryGroups) {
			geo.write(ostr);
		}

		return true;
	}


	bool SimpleSceneGraph::addGeometryGroup(const std::string name, const std::string& path) {
		if (!ReadObjFile(path, name)) {
			HFLOGERROR("OBJ file %s had an error while loading", path.c_str());
			return false;
		}
		createGeometry(name);
		unsigned id = geometryGroups.lastId;

		SimpleGeometryGroup& geometryGroup = geometryGroups[id];
		geometryGroup.transform = currentTransform;
		geometryGroup.bbox = staticMeshes[name].BoundingBox;
		geometryGroup.objectId = 0;
		geometryGroup.objectName = name;

		HFLOGINFO("OBJ file '%s' loaded.", path.c_str());
		_assignIdsToMeshes();
		return true;
	}


	bool SimpleSceneGraph::addDirToLight(const std::string& name) {
		if (dirToLights.count(name)) return true;
		auto node = createDirToLight(name);
		return node != nullptr;
	}


	SimpleSceneGraphNode* SimpleSceneGraph::createNode(const std::string& nodename_, SimpleSceneGraphNode* node) {
		node->setName(nodename_);
		node->transform = currentTransform;
		nodes[nodename_] = node;
		return node;
	}


	SimpleSceneGraphNode* SimpleSceneGraph::createCamera(const std::string& nodename_) {
		unsigned id = cameras.create(nodename_);
		cameras.lastId = id;
		return createNode(nodename_, &cameras[id]);
	}


	SimpleSceneGraphNode* SimpleSceneGraph::createSphere(const std::string& nodename_) {
		unsigned id = spheres.create(nodename_);
		spheres.lastId = id;
		return createNode(nodename_, &spheres[id]);
	}


	SimpleSceneGraphNode* SimpleSceneGraph::createDirToLight(const std::string& nodename_) {
		unsigned id = dirToLights.create(nodename_);
		dirToLights.lastId = id;
		return createNode(nodename_, &dirToLights[id]);
	}


	SimpleSceneGraphNode* SimpleSceneGraph::createPointLight(const std::string& nodename_) {
		unsigned id = pointLights.create(nodename_);
		pointLights.lastId = id;
		return createNode(nodename_, &pointLights[id]);
	}


	SimpleSceneGraphNode* SimpleSceneGraph::createAnisoLight(const std::string& nodename_) {
		unsigned id = anisoLights.create(nodename_);
		anisoLights.lastId = id;
		return createNode(nodename_, &anisoLights[id]);
	}


	SimpleSceneGraphNode* SimpleSceneGraph::createPathAnim(const std::string& nodename_) {
		unsigned id = pathanims.create(nodename_);
		pathanims.lastId = id;
		return createNode(nodename_, &pathanims[id]);
	}


	SimpleSceneGraphNode* SimpleSceneGraph::createGeometry(const std::string& nodename_) {
		unsigned id = geometryGroups.create(nodename_);
		geometryGroups.lastId = id;
		return createNode(nodename_, &geometryGroups[id]);
	}


	void SimpleSceneGraph::_assignIdsToMeshes() {
		for (auto& [mid, mesh] : staticMeshes) {
			for (auto& [gid, group] : geometryGroups) {
				if (group.objectId == 0 && group.objectName == mesh.name) {
					group.objectId = mid;
				}
			}
			for (auto& surface : mesh.Surfaces) {
				surface.materialId = materials.getMaterialIndex(surface.materialName);
			}
		}
	}
} // namespace Fluxions
