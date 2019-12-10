#include "pch.hpp"
#include <fluxions_scene_graph_writer.hpp>
#include <fluxions_fileio_iostream.hpp>
#include <fluxions_xml.hpp>

//#include <fluxions_simple_scene_graph.hpp>

namespace Fluxions
{

bool SceneGraphWriter::open(const char* path) {
	fout.open(path);
	if (!fout) return false;
	return true;
}

void SceneGraphWriter::close() {
	fout.close();
}

bool SceneGraphWriter::write(const SimpleSceneGraph* pssg) {
	const SimpleSceneGraph& ssg = *pssg;

	// 1. Camera
	fout << "camera ";
	if (ssg.camera.isOrtho) {
		fout << "ortho_tmw ";
		WriteAffineMatrix4f(fout, ssg.camera.viewMatrix);
		fout << ssg.camera.width;
	}
	else {
		fout << "perspective_tmf ";
		WriteAffineMatrix4f(fout, ssg.camera.viewMatrix);
		fout << ssg.camera.fov;
	}
	fout << "\n";

	// 2. Environment
	if (ssg.environment.hasColor) {
		fout << "enviro color ";
		WriteColor3f(fout, ssg.environment.color);
		fout << "\n";
	}
	if (ssg.environment.hasTexmap) {
		fout << "enviro texmap ";
		WriteString(fout, ssg.environment.texmap);
		fout << "\n";
	}
	if (ssg.environment.hasSun) {
		fout << "sun dirTo ";
		WriteVector3f(fout, ssg.environment.curSunDirTo);
		fout << "color ";
		WriteColor3f(fout, ssg.environment.sunColor);
		fout << "sizeMult ";
		WriteDouble(fout, ssg.environment.sunSize);
		fout << "\n";
	}

	// 3. Newmaps
	for (const auto& shaderMap : ssg.shaderMaps) {
		fout << "newmap " << shaderMap.first << " ";
		WriteString(fout, shaderMap.second);
		fout << "\n";
	}

	// 4. Spheres
	for (auto& [i, sphere] : ssg.spheres) {
		if (ssg.materials.GetLibraryId() != sphere.mtllibId) {
			//ssg.materials.SetLibrary(sphere.mtllibName);
			fout << "mtllib " << sphere.mtllibName << "\n";
		}
		fout << "transform ";
		WriteAffineMatrix4f(fout, sphere.transform);
		fout << "\n";
		fout << "sphere ";
		WriteString(fout, sphere.mtlName);
		fout << "\n";
	}

	// 5. Geometry Groups
	for (const auto& [i, geo] : ssg.geometry) {
		if (ssg.materials.GetLibraryId() != geo.mtllibId) {
			// ssg.materials.SetLibrary(geo.mtllibName);
			fout << "mtllib " << geo.mtllibName << "\n";
		}
		fout << "transform ";
		WriteAffineMatrix4f(fout, geo.transform);
		fout << "\n";
		fout << "geometryGroup " << geo.objectName << "\n";
	}
	fout.close();

	// 6. Output geometry files
	for (const auto& [i, geo] : ssg.geometry) {
		ssg.geometryObjects[geo.objectId].SaveOBJ(geo.objectName);
	}

	// 7. Output mtllib files
	// TODO:
	// ssg.materials.Save(fpi.dir);

	return true;
}


// XML SCENE GRAPH WRITER

bool XmlSceneGraphWriter::write(const SimpleSceneGraph* ssg) {
	if (!fout) return false;

	XmlBeginTag(fout, "scene") << "\n";

	for (string_string& ss : extra_tags) {
		if (ss.first == "") {
			fout << ss.second << "\n";
		}
		else {
			XmlString(fout, ss.first, ss.second, 1) << "\n";
		}
	}

	if (!camera_specified_) {
		setPerspectiveCamera(ssg->camera.origin(),
							 ssg->camera.target(),
							 ssg->camera.roll(),
							 ssg->camera.fov);
	}

	// Camera
	writeCamera(fout);

	// Sun
	writeSun(fout, ssg);

	// Geometry Groups
	writeGeometryGroups(fout, ssg);


	XmlEndTag(fout, "scene") << "\n";

	return true;
}


void XmlSceneGraphWriter::setPerspectiveCamera(const Vector3f& origin,
											   const Vector3f& target,
											   const Vector3f& roll,
											   const float horizontalFieldOfViewInDegrees) {
	cameraOrigin = origin;
	cameraTarget = target;
	cameraRoll = roll;
	cameraHorizontalFieldOfViewInDegrees = horizontalFieldOfViewInDegrees;
	cameraType = "perspective";
	camera_specified_ = true;
}

void XmlSceneGraphWriter::setCubeMapCamera(const Vector3f& origin,
										   const Vector3f& target,
										   const Vector3f& roll) {
	cameraOrigin = origin;
	cameraTarget = target;
	cameraRoll = roll;
	cameraType = "cubemap";
	camera_specified_ = true;
}

void XmlSceneGraphWriter::writeCamera(std::ostream& ostr) {
	Fluxions::Matrix4f fixMatrix(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, -1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
	XmlBeginTag(ostr, "camera", cameraType, 1) << "\n";
	if (cameraType == "perspective") {
		Fluxions::Matrix4f fixed = cameraMatrix * fixMatrix;
		// Vector4f position = fixed.col4();
		XmlMatrix4f(ostr, "tm", fixed, 2) << "\n";
		XmlFloat(ostr, "fov", cameraHorizontalFieldOfViewInDegrees, 2) << "\n";
	}
	if (cameraType == "cubemap") {
		XmlVector3f(ostr, "origin", cameraOrigin, 2) << "\n";
		XmlVector3f(ostr, "target", cameraTarget, 2) << "\n";
		XmlVector3f(ostr, "roll", cameraRoll, 2) << "\n";
	}
	XmlEndTag(ostr, "camera", 1) << "\n"
		<< "\n";
}

void XmlSceneGraphWriter::writeSun(std::ostream& ostr,
								   const Fluxions::SimpleSceneGraph* ssg) {
	if (!write_sun) return;
	XmlString(ostr, "mtllib", lights_mtllib, 1) << "\n";
	XmlBeginTag(ostr, "sun", 1) << "\n";
	Fluxions::Vector3f sunDirTo(ssg->environment.curSunDirTo.x,
								-ssg->environment.curSunDirTo.z,
								ssg->environment.curSunDirTo.y);
	XmlVector3f(ostr, "dirTo", sunDirTo, 2) << "\n";
	//XmlVector3f(ostr, "color", ssg->environment.color, 2) << "\n";
	//XmlFloat(ostr, "sizeMult", 1.0f, 2) << "\n";
	// Corona 1.6+
	XmlFloat(ostr, "turbidity", ssg->environment.pbsky.GetTurbidity(), 2) << "\n";
	XmlEndTag(ostr, "sun", 1) << "\n"
		<< "\n";

	ostr << "\t<environment>1 1 1<map class=\"Reference\">Skylight_environment</map></environment>" << "\n"
		<< "\n";

	// <mtlLib>
	// <mapDefinition name="Skylight_environment">
	// 	<map class="Sky" originalMapClass="SkyShader">
	// 		<groundColor>0.2 0.2 0.2</groundColor>
	// 	</map>
	// </mapDefinition>
	// </mtlLib>

	// XmlString(ostr, "mtllib", "ssphh_lights.mtl", 1) << "\n";

	const float size = 0.1f;
	for (unsigned i = 0; i < ssg->pointLights.size(); i++) {
		const SimplePointLight& pointLight = ssg->pointLights[i];
		Fluxions::Matrix4f lightMatrix(
			size, 0.0f, 0.0f, pointLight.position.x,
			0.0f, size, 0.0f, -pointLight.position.z,
			0.0f, 0.0f, size, pointLight.position.y,
			0.0f, 0.0f, 0.0f, 1.0f);
		// int tab = 0;
		std::ostringstream name;
		name << "PointLight" << i;

		XmlComment(ostr, name.str(), 1);
		XmlBeginTag(ostr, "geometryGroup", 1) << "\n";
		XmlBeginTag(ostr, "object", "sphere", 2) << "\n";
		XmlSize(ostr, "materialId", i, 3) << "\n";
		XmlEndTag(ostr, "object", 2) << "\n";
		XmlBeginTag(ostr, "instance", 2) << "\n";
		XmlBeginTag(ostr, "material", "Reference", 3) << "PointLight" << i;
		XmlEndTag(ostr, "material") << "\n";
		XmlMatrix4f(ostr, "transform", lightMatrix, 3) << "\n";
		XmlEndTag(ostr, "instance", 2) << "\n";
		XmlEndTag(ostr, "geometryGroup", 1) << "\n"
			<< "\n";
	}

	// Generate lights MTLLIB
	writeLightMtllib(export_path_prefix + lights_mtllib, ssg);
}

void XmlSceneGraphWriter::writeGeometryGroups(std::ostream& ostr,
											  const Fluxions::SimpleSceneGraph* ssg) {
	if (!write_geometry) return;
	writeCache(ssg);
	XmlBeginTag(ostr, "mtllib", 1);
	ostr << "materials_corona.mtl";
	XmlEndTag(ostr, "mtllib");
	ostr << "\n";
	for (string_string_Matrix4f& gg : geometryGroups) {
		XmlBeginTag(ostr, "geometryGroup", 1) << "\n";
		XmlBeginTag(ostr, "object", "file", 2) << std::get<0>(gg);
		XmlEndTag(ostr, "object") << "\n";
		XmlBeginTag(ostr, "instance", 2) << "\n";
		XmlBeginTag(ostr, "material", "Reference", 3) << std::get<1>(gg);
		XmlEndTag(ostr, "material", 0) << "\n";
		XmlMatrix4f(ostr, "transform", std::get<2>(gg), 3) << "\n";
		XmlEndTag(ostr, "instance", 2) << "\n";
		XmlEndTag(ostr, "geometryGroup", 1) << "\n";
	}
}

void XmlSceneGraphWriter::writeCache(const SimpleSceneGraph* ssg) {
	if (geometryGroups.empty()) {
		std::string MTLpath = export_path_prefix + "materials.mtl";
		std::string CoronaMTLpath = export_path_prefix + "materials_corona.mtl";
		HFLOGINFO("Writing out %s", MTLpath.c_str());
		HFLOGINFO("Writing out %s", CoronaMTLpath.c_str());
		std::ofstream mtl_fout(MTLpath);
		std::ofstream mtlxml_fout(CoronaMTLpath);
		XmlBeginTag(mtlxml_fout, "mtlLib") << "\n";

		std::string last_mtllib;
		int obj_count = 0;
		auto it = ssg->geometry.begin();
		for (unsigned i = 0; i < ssg->geometry.size(); i++, it++) {
			const SimpleGeometryGroup& sgo = it->second;
			const OBJStaticModel& obj = ssg->geometryObjects[sgo.objectId];

			if (obj.Surfaces.empty())
				continue;

			for (auto& surface : obj.Surfaces) {
				std::ostringstream obj_pathname;
				obj_pathname << "object_" << std::setw(3) << std::setfill('0') << obj_count << "_" << sgo.objectName << ".obj";
				std::string OBJpath = export_path_prefix + obj_pathname.str();

				mtl_name = sgo.objectName + "_" + surface.materialName;
				for (auto& c : mtl_name) {
					if (c == ' ')
						c = '_';
				}

				if (written_materials[mtl_name] == false) {
					const SimpleMaterial* mtl = ssg->materials.GetLibraryMaterial(sgo.mtllibName, surface.materialName);
					if (mtl) {
						writeMtl(mtl_fout, mtlxml_fout, mtl, ssg);
					}
				}

				FilePathInfo objfpi(OBJpath);
				if (objfpi.DoesNotExist()) {
					HFLOGINFO("Writing out %s", OBJpath.c_str());
					std::ofstream obj_fout(OBJpath);
					writeObj(obj_fout, obj, surface);
				}

				Fluxions::Matrix4f fixMatrix(
					1.0f, 0.0f, 0.0f, 0.0f,
					0.0f, 0.0f, -1.0f, 0.0f,
					0.0f, 1.0f, 0.0f, 0.0f,
					0.0f, 0.0f, 0.0f, 1.0f);

				std::string objectPath = obj_pathname.str();
				Fluxions::Matrix4f transform = fixMatrix * sgo.transform;

				geometryGroups.push_back(string_string_Matrix4f(objectPath, mtl_name, transform));
				obj_count++;
			}
		}

		mtl_fout.close();

		for (auto& mapIt : written_maps) {
			XmlCoronaMapTexture(mtlxml_fout, "mapDefinition", mapIt.first, mapIt.first, 1, 2.2f) << "\n";

			std::string map_path = export_path_prefix + mapIt.first;
			FilePathInfo mtlfpi(map_path);
			if (mtlfpi.DoesNotExist()) {
#ifdef _WIN32
				// CopyFile(dst, src, bOverWrite?)
				CopyFile(mapIt.second.c_str(), map_path.c_str(), TRUE);
#elif __unix__
				// POSIX version does not exist
				// So we will call system instead
				std::string cp_args = mapIt.second + " " + map_path;
				execl("/bin/cp", cp_args.c_str());
#else
#error "unsupported platform"
#endif
				//copy_file(mapIt.second, corona_export_prefix + mapIt.first);
			}
		}
		XmlEndTag(mtlxml_fout, "mtlLib");
		mtlxml_fout.close();
	}
}

void XmlSceneGraphWriter::writeLightMtllib(const std::string& path,
										   const SimpleSceneGraph* ssg) {
	HFLOGINFO("Writing lights_mtllib %s", path.c_str());
	std::ofstream enviro_fout(path);
	XmlBeginTag(enviro_fout, "mtlLib") << "\n";
	XmlBeginTag(enviro_fout, "mapDefinition", "name", "Skylight_environment", 1) << "\n";
	enviro_fout << "\t<map class = \"Sky\" originalMapClass = \"SkyShader\">\n";
	XmlVector3f(enviro_fout, "groundColor", ssg->environment.pbsky.GetGroundAlbedo().ToVector3(), 2) << "\n";
	XmlEndTag(enviro_fout, "map", 1) << "\n";
	XmlEndTag(enviro_fout, "mapDefinition", 1) << "\n"
		<< "\n";

	const float size = 0.1f;
	for (unsigned i = 0; i < ssg->pointLights.size(); i++) {
		std::ostringstream name;
		name << "PointLight" << i;
		XmlBeginTag(enviro_fout, "materialDefinition", "name", name.str(), 1) << "\n";
		XmlBeginTag(enviro_fout, "material", "Native", 2) << "\n";
		XmlBeginTag(enviro_fout, "emission", 3) << "\n";
		XmlVector3f(enviro_fout, "color", Fluxions::Vector3f(ssg->pointLights[i].E0 / size), 4) << "\n";
		XmlEndTag(enviro_fout, "emission", 3) << "\n";
		XmlEndTag(enviro_fout, "material", 2) << "\n";
		XmlEndTag(enviro_fout, "materialDefinition", 1) << "\n"
			<< "\n";
	}

	XmlEndTag(enviro_fout, "mtlLib") << "\n";
	enviro_fout.close();
}

void XmlSceneGraphWriter::writeMtl(std::ostream& mtl_fout,
								   std::ostream& mtlxml_fout,
								   const SimpleMaterial* mtl,
								   const SimpleSceneGraph* ssg) {
	written_materials[mtl_name] = (int)written_materials.size();
	mtl_fout << "newmtl " << mtl_name << "\n";

	mtl_fout << "Kd ";
	WriteColor3f(mtl_fout, mtl->Kd);
	mtl_fout << "\n";
	if (enableKs) {
		mtl_fout << "Ks ";
		WriteColor3f(mtl_fout, mtl->Ks);
		mtl_fout << "\n";
	}
	else {
		mtl_fout << "Ks 0.0 0.0 0.0" << "\n";
	}
	mtl_fout << "\n";

	//ostream & fout = mtlxml_fout;

	if (!mtl->map_Kd.empty()) {
		written_maps[mtl->map_Kd] = ssg->materials.GetMapPath(mtl->map_Kd);
	}
	if (!mtl->map_bump.empty()) {
		written_maps[mtl->map_bump] = ssg->materials.GetMapPath(mtl->map_bump);
	}

	XmlBeginTag(mtlxml_fout, "materialDefinition", "name", mtl_name, 1) << "\n";
	XmlBeginTag(mtlxml_fout, "material", "Native", 2) << "\n";
	if (!mtl->map_Kd.empty()) {
		XmlBeginTag(mtlxml_fout, "diffuse", 3) << "\n";
		XmlBeginTag(mtlxml_fout, "map", "Reference", 4);
		mtlxml_fout << mtl->map_Kd;
		XmlEndTag(mtlxml_fout, "map") << "\n";
		XmlEndTag(mtlxml_fout, "diffuse", 3) << "\n";
	}
	else {
		if (mtl->PBk > 0.0)
			XmlColor3f(mtlxml_fout, "diffuse", Color3f(), 3) << "\n";
		else
			XmlColor3f(mtlxml_fout, "diffuse", mtl->Kd, 3) << "\n";
	}

	if (enableKs) {
		XmlBeginTag(mtlxml_fout, "reflect", 3) << "\n";
		XmlColor3f(mtlxml_fout, "color", mtl->Ks, 4) << "\n";

		if (mtl->PBk > 0.0) {
			XmlFloat(mtlxml_fout, "ior", 999, 4) << "\n";
		}
		else {
			XmlFloat(mtlxml_fout, "ior", mtl->PBior, 4) << "\n";
		}
		// Corona uses glossiness which is the reverse of roughness
		// A surface is smooth if roughness is 0 or glossiness is 1
		// A surface is rough if roughness is 1 or glossiness is 0
		if (mtl->PBKsm < 0)
			XmlFloat(mtlxml_fout, "glossiness", clamp(1.0f + mtl->PBKsm, 0.0f, 1.0f), 4) << "\n";
		else
			XmlFloat(mtlxml_fout, "glossiness", clamp(1.0f - mtl->PBKsm, 0.0f, 1.0f), 4) << "\n";
		XmlEndTag(mtlxml_fout, "reflect", 3) << "\n";
	}

	//if (!mtl->map_bump.empty())
	//{
	//	XmlString(mtlxml_fout, "bump", mtl->map_bump, 3) << "\n";
	//}
	XmlEndTag(mtlxml_fout, "material", 2) << "\n";
	XmlEndTag(mtlxml_fout, "materialDefinition", 1) << "\n";
}

void XmlSceneGraphWriter::writeObj(std::ostream& obj_fout,
								   const OBJStaticModel& obj,
								   const OBJStaticModel::Surface& surface) {
	obj_fout << "usemtl " << written_materials[mtl_name] << "\n";

	// output vertices
	obj_fout << "# surface(" << surface.first << ", " << surface.count << ") \"" << surface.surfaceName << "\"" << "\n";
	for (unsigned index = surface.first; index < surface.first + surface.count; index++) {
		obj_fout << "v ";
		WriteVector3f(obj_fout, obj.Vertices[index].position) << "\n";
	}

	// normals
	for (unsigned index = surface.first; index < surface.first + surface.count; index++) {
		obj_fout << "vn ";
		WriteVector3f(obj_fout, obj.Vertices[index].normal) << "\n";
	}

	// texcoords
	for (unsigned index = surface.first; index < surface.first + surface.count; index++) {
		obj_fout << "vt ";
		WriteVector2f(obj_fout, obj.Vertices[index].texcoord) << "\n";
	}

	// output triangle faces
	unsigned count = 0;
	for (unsigned index = surface.first; index < surface.first + surface.count; index++, count++) {
		if (count % 3 == 0) obj_fout << "\nf ";
		int v_index = index - surface.first + 1;
		obj_fout << v_index << "/" << v_index << "/" << v_index << " ";
	}
}

} // namespace Fluxions