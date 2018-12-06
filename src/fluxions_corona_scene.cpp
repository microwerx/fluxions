// SSPHH/Fluxions/Unicornfish/Viperfish/Hatchetfish/Sunfish/KASL/GLUT Extensions
// Copyright (C) 2017 Jonathan Metzgar
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
#include "stdafx.h"
#include <fluxions_corona_scene.hpp>
#include <fluxions_fileio.hpp>
#include <fluxions_xml.hpp>
#include <filesystem>

namespace Fluxions
{
CoronaSceneFile::CoronaSceneFile()
{
}

CoronaSceneFile::~CoronaSceneFile()
{
}

void CoronaSceneFile::SetCameraType(const string &newCameraType)
{
	if (newCameraType == "perspective")
		cameraType = newCameraType;
	if (newCameraType == "cubemap")
		cameraType = newCameraType;
}

void CoronaSceneFile::SetPerspectiveCamera(const Vector3f &origin, const Vector3f &target, const Vector3f &roll, const float horizontalFieldOfViewInDegrees)
{
	cameraOrigin = origin;
	cameraTarget = target;
	cameraRoll = roll;
	cameraHorizontalFieldOfViewInDegrees = horizontalFieldOfViewInDegrees;
	cameraType = "perspective";
}

void CoronaSceneFile::SetCubeMapCamera(const Vector3f &origin, const Vector3f &target, const Vector3f &roll)
{
	cameraOrigin = origin;
	cameraTarget = target;
	cameraRoll = roll;
	cameraType = "cubemap";
}

void CoronaSceneFile::WriteSCN(const string &filename, const SimpleSceneGraph &ssg)
{
	SetCameraType("perspective");
	Vector3f origin = ssg.camera.viewMatrix.col4().xyz();
	Vector3f target = ssg.camera.viewMatrix.col4().xyz() + ssg.camera.viewMatrix.col3().xyz();
	Vector3f roll = ssg.camera.viewMatrix.col4().xyz() + ssg.camera.viewMatrix.col2().xyz();
	float hfovDegrees = (float)ssg.camera.fov;
	SetPerspectiveCamera(origin, target, roll, hfovDegrees);
	Matrix4f coronaMatrix(
		1.0, 0.0, 0.0, 0.0,
		0.0, 0.0, -1.0, 0.0,
		0.0, -1.0, 0.0, 0.0,
		0.0, 0.0, 0.0, 1.0);
	cameraMatrix = ssg.camera.actualViewMatrix;

	ofstream fout(filename);
	if (!fout)
		return;

	XmlBeginTag(fout, "scene") << endl;

	//fout << "<renderElement class=\"Components\" instanceName=\"CESSENTIAL_Direct\"><componentName>diffuseDirect</componentName></renderElement>" << endl;
	//fout << "<renderElement class=\"Components\" instanceName=\"CESSENTIAL_Reflect\"><componentName>diffuseDirect</componentName></renderElement>" << endl;
	//fout << "<renderElement class=\"Components\" instanceName=\"CESSENTIAL_Indirect\"><componentName>diffuseIndirect</componentName></renderElement>" << endl << endl;

	XmlString(fout, "conffile", "export_corona_ground_truth.conf") << endl
																   << endl;

	// Camera
	writeCamera(fout);

	// Sun
	writeSun(fout, ssg);

	// Geometry Groups
	writeGeometryGroups(fout, ssg);

	XmlEndTag(fout, "scene") << endl;

	fout.close();
}

void CoronaSceneFile::WriteCubeMapSCN(const string &filename, const SimpleSceneGraph &ssg)
{
	Vector3f cameraPosition = ssg.camera.actualViewMatrix.col4().xyz();
	WriteCubeMapSCN(filename, ssg, cameraPosition);
}

void CoronaSceneFile::WriteCubeMapSCN(const string &filename, const SimpleSceneGraph &ssg, const Vector3f &cameraPosition)
{
	SetCubeMapCamera(cameraPosition, cameraPosition + Vector3f(0.0f, 0.0f, -1.0f), Vector3f(0.0f, 1.0f, 0.0f));

	ofstream fout(filename);
	if (!fout)
		return;

	XmlBeginTag(fout, "scene") << endl;

	// Camera
	writeCamera(fout);

	// Sun
	writeSun(fout, ssg);

	// Geometry Groups
	writeGeometryGroups(fout, ssg);

	XmlEndTag(fout, "scene") << endl;

	fout.close();
}

void CoronaSceneFile::WriteSkySCN(const string &filename, const SimpleSceneGraph &ssg)
{
	ofstream fout(filename);
	if (!fout)
		return;

	XmlBeginTag(fout, "scene") << endl;
	XmlString(fout, "conffile", "ssphh_sky.conf", 1) << endl
													 << endl;
	SetCubeMapCamera(Vector3f(0.0f, 0.0f, 0.0f), Vector3f(0.0f, 1.0f, 0.0f), Vector3f(0.0f, 0.0f, 1.0f));
	writeCamera(fout);
	writeSun(fout, ssg);
	XmlEndTag(fout, "scene") << endl;

	fout.close();
}

bool CoronaSceneFile::WriteSphlVizSCN(const string &filename, const SimpleSceneGraph &ssg, int sourceLightIndex, int receivingLightIndex)
{
	int lastIndex = (int)ssg.ssphhLights.size() - 1;
	if (!within(sourceLightIndex, -1, lastIndex) ||
		!within(receivingLightIndex, -1, lastIndex))
	{
		return false;
	}

	ofstream fout(filename);
	if (!fout)
		return false;

	XmlBeginTag(fout, "scene") << endl;
	XmlString(fout, "conffile", "sphlviz.conf", 1) << endl
												   << endl;

	// Camera
	if (receivingLightIndex < 0)
	{
		Vector3f position = ssg.camera.actualViewMatrix.col4().xyz();
		SetCubeMapCamera(position, Vector3f(0.0f, 0.0f, 0.0f), Vector3f(0.0f, 1.0f, 0.0f));
	}
	else
	{
		const SimpleSSPHHLight &sphl2 = ssg.ssphhLights[receivingLightIndex];
		Vector3f tmp = sphl2.position.xyz();
		Vector3f position(tmp.x, -tmp.z, tmp.y);
		Vector3f target(position.x, position.y - 1.0f, position.z);
		SetCubeMapCamera(position, target, Vector3f(0.0f, 0.0f, 1.0f));
	}
	writeCamera(fout);

	// Geometry Groups
	writeGeometryGroups(fout, ssg);

	if (sourceLightIndex < 0)
	{
		writeSun(fout, ssg);
	}
	else
	{
		const SimpleSSPHHLight &sphl1 = ssg.ssphhLights[sourceLightIndex];
		Matrix4f lightMatrix(
			0.1f, 0.0f, 0.0f, sphl1.position.x,
			0.0f, 0.1f, 0.0f, -sphl1.position.z,
			0.0f, 0.0f, 0.1f, sphl1.position.y,
			0.0f, 0.0f, 0.0f, 1.0f);
		// int tab = 0;
		XmlString(fout, "mtllib", "sphlviz.mtl", 1) << endl;
		XmlBeginTag(fout, "geometryGroup", 1) << endl;
		XmlBeginTag(fout, "object", "sphere", 2) << endl;
		XmlInt(fout, "materialId", 0, 3) << endl;
		XmlEndTag(fout, "object", 2) << endl;
		XmlBeginTag(fout, "instance", 2) << endl;
		XmlBeginTag(fout, "material", "Reference", 3) << "sphlSphereLight";
		XmlEndTag(fout, "material") << endl;
		XmlMatrix4f(fout, "transform", lightMatrix, 3) << endl;
		XmlEndTag(fout, "instance", 2) << endl;
		XmlEndTag(fout, "geometryGroup", 1) << endl;
	}

	XmlEndTag(fout, "scene");
	fout.close();
	return true;
}

void CoronaSceneFile::writeCamera(ostream &ostr)
{
	Matrix4f fixMatrix(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, -1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
	XmlBeginTag(ostr, "camera", cameraType, 1) << endl;
	if (cameraType == "perspective")
	{
		Matrix4f fixed = cameraMatrix * fixMatrix;
		// Vector4f position = fixed.col4();
		XmlMatrix4f(ostr, "tm", fixed, 2) << endl;
		XmlFloat(ostr, "fov", cameraHorizontalFieldOfViewInDegrees, 2) << endl;
	}
	if (cameraType == "cubemap")
	{
		XmlVector3f(ostr, "origin", cameraOrigin, 2) << endl;
		XmlVector3f(ostr, "target", cameraTarget, 2) << endl;
		XmlVector3f(ostr, "roll", cameraRoll, 2) << endl;
	}
	XmlEndTag(ostr, "camera", 1) << endl
								 << endl;
}

void CoronaSceneFile::writeSun(ostream &ostr, const SimpleSceneGraph &ssg)
{
	string lights_mtllib = "ssphh_lights.mtl";
	XmlString(ostr, "mtllib", lights_mtllib, 1) << endl;
	XmlBeginTag(ostr, "sun", 1) << endl;
	Vector3f sunDirTo(ssg.environment.curSunDirTo.x, -ssg.environment.curSunDirTo.z, ssg.environment.curSunDirTo.y);
	XmlVector3f(ostr, "dirTo", sunDirTo, 2) << endl;
	//XmlVector3f(ostr, "color", ssg.environment.color, 2) << endl;
	//XmlFloat(ostr, "sizeMult", 1.0f, 2) << endl;
	// Corona 1.6+
	XmlFloat(ostr, "turbidity", ssg.environment.pbsky.GetTurbidity(), 2) << endl;
	XmlEndTag(ostr, "sun", 1) << endl
							  << endl;

	ostr << "\t<environment>1 1 1<map class=\"Reference\">Skylight_environment</map></environment>" << endl
		 << endl;

	// <mtlLib>
	// <mapDefinition name="Skylight_environment">
	// 	<map class="Sky" originalMapClass="SkyShader">
	// 		<groundColor>0.2 0.2 0.2</groundColor>
	// 	</map>
	// </mapDefinition>
	// </mtlLib>

	// XmlString(ostr, "mtllib", "ssphh_lights.mtl", 1) << endl;

	const float size = 0.1f;
	for (size_t i = 0; i < ssg.pointLights.size(); i++)
	{
		const SimplePointLight &pointLight = ssg.pointLights[i];
		Matrix4f lightMatrix(
			size, 0.0f, 0.0f, pointLight.position.x,
			0.0f, size, 0.0f, -pointLight.position.z,
			0.0f, 0.0f, size, pointLight.position.y,
			0.0f, 0.0f, 0.0f, 1.0f);
		// int tab = 0;
		ostringstream name;
		name << "PointLight" << i;

		XmlComment(ostr, name.str(), 1);
		XmlBeginTag(ostr, "geometryGroup", 1) << endl;
		XmlBeginTag(ostr, "object", "sphere", 2) << endl;
		XmlSize(ostr, "materialId", i, 3) << endl;
		XmlEndTag(ostr, "object", 2) << endl;
		XmlBeginTag(ostr, "instance", 2) << endl;
		XmlBeginTag(ostr, "material", "Reference", 3) << "PointLight" << i;
		XmlEndTag(ostr, "material") << endl;
		XmlMatrix4f(ostr, "transform", lightMatrix, 3) << endl;
		XmlEndTag(ostr, "instance", 2) << endl;
		XmlEndTag(ostr, "geometryGroup", 1) << endl
											<< endl;
	}

	// Generate lights MTLLIB

	ofstream fout(lights_mtllib);
	XmlBeginTag(fout, "mtlLib") << endl;
	XmlBeginTag(fout, "mapDefinition", "name", "Skylight_environment", 1) << endl;
	fout << "\t<map class = \"Sky\" originalMapClass = \"SkyShader\">\n";
	XmlVector3f(fout, "groundColor", ssg.environment.pbsky.GetGroundAlbedo().ToVector3(), 2) << endl;
	XmlEndTag(fout, "map", 1) << endl;
	XmlEndTag(fout, "mapDefinition", 1) << endl
										<< endl;

	for (int i = 0; i < ssg.pointLights.size(); i++)
	{
		ostringstream name;
		name << "PointLight" << i;
		XmlBeginTag(fout, "materialDefinition", "name", name.str(), 1) << endl;
		XmlBeginTag(fout, "material", "Native", 2) << endl;
		XmlBeginTag(fout, "emission", 3) << endl;
		XmlVector3f(fout, "color", Vector3f(ssg.pointLights[i].E0 / size), 4) << endl;
		XmlEndTag(fout, "emission", 3) << endl;
		XmlEndTag(fout, "material", 2) << endl;
		XmlEndTag(fout, "materialDefinition", 1) << endl
												 << endl;
	}

	XmlEndTag(fout, "mtlLib") << endl;
	fout.close();
}

void CoronaSceneFile::ClearCache()
{
	geometryGroups.clear();
}

void CoronaSceneFile::WriteCache(const SimpleSceneGraph &ssg)
{
	if (geometryGroups.empty())
	{
		map<string, int> written_materials;
		map<string, string> written_maps;

		ofstream mtl_fout("corona_export/materials.mtl");
		ofstream mtlxml_fout("corona_export/materials_corona.mtl");
		XmlBeginTag(mtlxml_fout, "mtlLib") << endl;

		string last_mtllib;
		string mtl_name;
		int obj_count = 0;
		auto it = ssg.geometry.cbegin();
		for (size_t i = 0; i < ssg.geometry.size(); i++, it++)
		{
			const SimpleGeometryGroup &sgo = it->second;
			const OBJStaticModel &obj = ssg.geometryObjects[sgo.objectId];

			if (obj.Surfaces.empty())
				continue;

			for (auto &surface : obj.Surfaces)
			{
				ostringstream obj_pathname;
				obj_pathname << "corona_export/object_" << setw(3) << setfill('0') << obj_count << "_" << sgo.objectName << ".obj";
				ofstream obj_fout(obj_pathname.str());

				mtl_name = sgo.objectName + "_" + surface.materialName;
				for (auto &c : mtl_name)
				{
					if (c == ' ')
						c = '_';
				}

				if (written_materials[mtl_name] == false)
				{
					const SimpleMaterial *mtl = ssg.materials.GetLibraryMaterial(sgo.mtllibName, surface.materialName);
					if (mtl)
					{
						written_materials[mtl_name] = (int)written_materials.size();
						mtl_fout << "newmtl " << mtl_name << endl;

						mtl_fout << "Kd ";
						WriteVector3f(mtl_fout, mtl->Kd);
						mtl_fout << endl;
						if (enableKs)
						{
							mtl_fout << "Ks ";
							WriteVector3f(mtl_fout, mtl->Ks);
							mtl_fout << endl;
						}
						else
						{
							mtl_fout << "Ks 0.0 0.0 0.0" << endl;
						}
						mtl_fout << endl;

						//ostream & fout = mtlxml_fout;

						if (!mtl->map_Kd.empty())
						{
							written_maps[mtl->map_Kd] = ssg.materials.GetMapPath(mtl->map_Kd);
						}
						if (!mtl->map_bump.empty())
						{
							written_maps[mtl->map_bump] = ssg.materials.GetMapPath(mtl->map_bump);
						}

						XmlBeginTag(mtlxml_fout, "materialDefinition", "name", mtl_name, 1) << endl;
						XmlBeginTag(mtlxml_fout, "material", "Native", 2) << endl;
						if (!mtl->map_Kd.empty())
						{
							XmlBeginTag(mtlxml_fout, "diffuse", 3) << endl;
							XmlBeginTag(mtlxml_fout, "map", "Reference", 4);
							mtlxml_fout << mtl->map_Kd;
							XmlEndTag(mtlxml_fout, "map") << endl;
							XmlEndTag(mtlxml_fout, "diffuse", 3) << endl;
						}
						else
						{
							if (mtl->PBk > 0.0)
								XmlVector3f(mtlxml_fout, "diffuse", Vector3f(), 3) << endl;
							else
								XmlVector3f(mtlxml_fout, "diffuse", mtl->Kd, 3) << endl;
						}

						if (enableKs)
						{
							XmlBeginTag(mtlxml_fout, "reflect", 3) << endl;
							XmlVector3f(mtlxml_fout, "color", mtl->Ks, 4) << endl;

							if (mtl->PBk > 0.0)
							{
								XmlFloat(mtlxml_fout, "ior", 999, 4) << endl;
							}
							else
							{
								XmlFloat(mtlxml_fout, "ior", mtl->PBior, 4) << endl;
							}
							// Corona uses glossiness which is the reverse of roughness
							// A surface is smooth if roughness is 0 or glossiness is 1
							// A surface is rough if roughness is 1 or glossiness is 0
							if (mtl->PBKsm < 0)
								XmlFloat(mtlxml_fout, "glossiness", clamp(1.0f + mtl->PBKsm, 0.0f, 1.0f), 4) << endl;
							else
								XmlFloat(mtlxml_fout, "glossiness", clamp(1.0f - mtl->PBKsm, 0.0f, 1.0f), 4) << endl;
							XmlEndTag(mtlxml_fout, "reflect", 3) << endl;
						}

						//if (!mtl->map_bump.empty())
						//{
						//	XmlString(mtlxml_fout, "bump", mtl->map_bump, 3) << endl;
						//}
						XmlEndTag(mtlxml_fout, "material", 2) << endl;
						XmlEndTag(mtlxml_fout, "materialDefinition", 1) << endl;
					}
				}
				if (0)
				{
					obj_fout << "mtllib "
							 << "materials_corona.mtl" << endl;
					obj_fout << "usemtl " << mtl_name << endl;
				}
				else
				{
					obj_fout << "usemtl " << written_materials[mtl_name] << endl;
				}
				// output vertices
				obj_fout << "# surface(" << surface.first << ", " << surface.count << ") \"" << surface.surfaceName << "\"" << endl;
				for (int index = surface.first; index < surface.first + surface.count; index++)
				{
					obj_fout << "v ";
					WriteVector3f(obj_fout, obj.Vertices[index].position) << endl;
				}
				for (int index = surface.first; index < surface.first + surface.count; index++)
				{
					obj_fout << "vn ";
					WriteVector3f(obj_fout, obj.Vertices[index].normal) << endl;
				}
				for (int index = surface.first; index < surface.first + surface.count; index++)
				{
					obj_fout << "vt ";
					WriteVector2f(obj_fout, obj.Vertices[index].texcoord) << endl;
				}

				// output triangle faces
				int count = 0;
				for (int index = surface.first; index < surface.first + surface.count; index++, count++)
				{
					if (count % 3 == 0)
						obj_fout << endl
								 << "f ";
					int v_index = index - surface.first + 1;
					obj_fout << v_index << "/" << v_index << "/" << v_index << " ";
				}
				obj_fout.close();

				Matrix4f fixMatrix(
					1.0f, 0.0f, 0.0f, 0.0f,
					0.0f, 0.0f, -1.0f, 0.0f,
					0.0f, 1.0f, 0.0f, 0.0f,
					0.0f, 0.0f, 0.0f, 1.0f);

				string objectPath = obj_pathname.str();
				Matrix4f transform = fixMatrix * sgo.transform;

				//XmlBeginTag(ostr, "geometryGroup", 1) << endl;
				//XmlBeginTag(ostr, "object", "file", 2);
				//ostr << obj_pathname.str();
				//XmlEndTag(ostr, "object") << endl;
				//XmlBeginTag(ostr, "instance", 2) << endl;
				//XmlBeginTag(ostr, "material", "Reference", 3);
				//ostr << mtl_name;
				//XmlEndTag(ostr, "material", 0) << endl;
				//XmlMatrix4f(ostr, "transform", fixMatrix * sgo.transform, 3) << endl;
				//XmlEndTag(ostr, "instance", 2) << endl;
				//XmlEndTag(ostr, "geometryGroup", 1) << endl;

				geometryGroups.push_back(tuple<string, string, Matrix4f>(objectPath, mtl_name, transform));

				obj_count++;
			}
		}

		mtl_fout.close();

		for (auto &mapIt : written_maps)
		{
			XmlCoronaMapTexture(mtlxml_fout, "mapDefinition", mapIt.first, mapIt.first, 1, 2.2f) << endl;

#ifdef WIN32
			// CopyFile(dst, src, bOverWrite?)
			CopyFile(mapIt.second.c_str(), ("corona_export/" + mapIt.first).c_str(), TRUE);
#else
			// POSIX version does not exist
			// So we will call system instead
			string cp_args = mapIt.second + " corona_export/" + mapIt.first;
			execl("/bin/cp", cp_args.c_str());
#endif
			//copy_file(mapIt.second, "corona_export/" + mapIt.first);
		}
		XmlEndTag(mtlxml_fout, "mtlLib");
		mtlxml_fout.close();
	}
}

void CoronaSceneFile::WriteMaterials(const SimpleSceneGraph &ssg, bool enableKs_)
{
	map<string, int> written_materials;
	map<string, string> written_maps;

	ofstream mtl_fout("corona_export/materials.mtl");
	ofstream mtlxml_fout("corona_export/materials_corona.mtl");
	XmlBeginTag(mtlxml_fout, "mtlLib") << endl;

	string last_mtllib;
	string mtl_name;
	// size_t obj_count = 0;
	auto it = ssg.geometry.cbegin();
	for (size_t i = 0; i < ssg.geometry.size(); i++, it++)
	{
		const SimpleGeometryGroup &sgo = it->second;
		const OBJStaticModel &obj = ssg.geometryObjects[sgo.objectId];

		if (obj.Surfaces.empty())
			continue;

		for (auto &surface : obj.Surfaces)
		{
			//ostringstream obj_pathname;
			//obj_pathname << "corona_export/object_" << setw(3) << setfill('0') << obj_count << "_" << sgo.objectName << ".obj";
			//ofstream obj_fout(obj_pathname.str());

			mtl_name = sgo.objectName + "_" + surface.materialName;
			for (auto &c : mtl_name)
			{
				if (c == ' ')
					c = '_';
			}

			if (written_materials[mtl_name] == false)
			{
				const SimpleMaterial *mtl = ssg.materials.GetLibraryMaterial(sgo.mtllibName, surface.materialName);
				if (mtl)
				{
					written_materials[mtl_name] = (int)written_materials.size();
					mtl_fout << "newmtl " << mtl_name << endl;

					mtl_fout << "Kd ";
					WriteVector3f(mtl_fout, mtl->Kd);
					mtl_fout << endl;
					if (enableKs_)
					{
						mtl_fout << "Ks ";
						WriteVector3f(mtl_fout, mtl->Ks);
						mtl_fout << endl;
					}
					else
					{
						mtl_fout << "Ks 0.0 0.0 0.0" << endl;
					}
					mtl_fout << endl;

					//ostream & fout = mtlxml_fout;

					if (!mtl->map_Kd.empty())
					{
						written_maps[mtl->map_Kd] = ssg.materials.GetMapPath(mtl->map_Kd);
					}
					if (!mtl->map_bump.empty())
					{
						written_maps[mtl->map_bump] = ssg.materials.GetMapPath(mtl->map_bump);
					}

					XmlBeginTag(mtlxml_fout, "materialDefinition", "name", mtl_name, 1) << endl;
					XmlBeginTag(mtlxml_fout, "material", "Native", 2) << endl;
					if (!mtl->map_Kd.empty())
					{
						XmlBeginTag(mtlxml_fout, "diffuse", 3) << endl;
						XmlBeginTag(mtlxml_fout, "map", "Reference", 4);
						mtlxml_fout << mtl->map_Kd;
						XmlEndTag(mtlxml_fout, "map") << endl;
						XmlEndTag(mtlxml_fout, "diffuse", 3) << endl;
					}
					else
					{
						if (mtl->PBk > 0.0)
							XmlVector3f(mtlxml_fout, "diffuse", Vector3f(), 3) << endl;
						else
							XmlVector3f(mtlxml_fout, "diffuse", mtl->Kd, 3) << endl;
					}

					if (enableKs_)
					{
						XmlBeginTag(mtlxml_fout, "reflect", 3) << endl;
						XmlVector3f(mtlxml_fout, "color", mtl->Ks, 4) << endl;

						if (mtl->PBk > 0.0)
						{
							XmlFloat(mtlxml_fout, "ior", 999, 4) << endl;
						}
						else
						{
							XmlFloat(mtlxml_fout, "ior", mtl->PBior, 4) << endl;
						}
						// Corona uses glossiness which is the reverse of roughness
						// A surface is smooth if roughness is 0 or glossiness is 1
						// A surface is rough if roughness is 1 or glossiness is 0
						if (mtl->PBKsm < 0)
							XmlFloat(mtlxml_fout, "glossiness", clamp(1.0f + mtl->PBKsm, 0.0f, 1.0f), 4) << endl;
						else
							XmlFloat(mtlxml_fout, "glossiness", clamp(1.0f - mtl->PBKsm, 0.0f, 1.0f), 4) << endl;
						XmlEndTag(mtlxml_fout, "reflect", 3) << endl;
					}

					XmlEndTag(mtlxml_fout, "material", 2) << endl;
					XmlEndTag(mtlxml_fout, "materialDefinition", 1) << endl;
				}
			}
		}
	}

	mtl_fout.close();

	for (auto &mapIt : written_maps)
	{
		XmlCoronaMapTexture(mtlxml_fout, "mapDefinition", mapIt.first, mapIt.first, 1, 2.2f) << endl;

#ifdef WIN32
		// CopyFile(dst, src, bOverWrite?)
		CopyFile(mapIt.second.c_str(), ("corona_export/" + mapIt.first).c_str(), TRUE);
#else
		// POSIX version does not exist
		// So we will call system instead
		string cp_args = mapIt.second + " corona_export/" + mapIt.first;
		execl("/bin/cp", cp_args.c_str());
#endif

		//copy_file(mapIt.second, "corona_export/" + mapIt.first);
	}
	XmlEndTag(mtlxml_fout, "mtlLib");
	mtlxml_fout.close();
}

void CoronaSceneFile::writeGeometryGroups(ostream &ostr, const SimpleSceneGraph &ssg)
{
	WriteCache(ssg);
	XmlBeginTag(ostr, "mtllib", 1);
	ostr << "corona_export/materials_corona.mtl";
	XmlEndTag(ostr, "mtllib");
	ostr << endl;
	for (auto &gg : geometryGroups)
	{
		XmlBeginTag(ostr, "geometryGroup", 1) << endl;
		XmlBeginTag(ostr, "object", "file", 2) << get<0>(gg);
		XmlEndTag(ostr, "object") << endl;
		XmlBeginTag(ostr, "instance", 2) << endl;
		XmlBeginTag(ostr, "material", "Reference", 3) << get<1>(gg);
		XmlEndTag(ostr, "material", 0) << endl;
		XmlMatrix4f(ostr, "transform", get<2>(gg), 3) << endl;
		XmlEndTag(ostr, "instance", 2) << endl;
		XmlEndTag(ostr, "geometryGroup", 1) << endl;
	}
}

/////////////////////////////////////////////////////////////////////
// CoronaJob ////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

CoronaJob::CoronaJob(const string &name, Type jobtype, int arg1, int arg2)
{
	scene_name = name;
	scene_path = name + ".scn";
	output_path_exr = name + ".exr";
	output_path_ppm = name + ".ppm";
	output_path_png = name + ".png";
	conf_path = name + ".conf";
	hq_output_path_exr = name + "_hq.exr";
	hq_output_path_ppm = name + "_hq.ppm";
	hq_conf_path = name + "_hq.conf";

	type = jobtype;

	switch (jobtype)
	{
	case Type::GEN:
		sendLight = SphlSunIndex;
		recvLight = arg1;
		conf_path = "sphlgen.conf";
		hq_conf_path = "sphlgen_hq.conf";
		break;
	case Type::VIZ:
		sendLight = arg1;
		recvLight = arg2;
		conf_path = "sphlviz.conf";
		hq_conf_path = "sphlviz_hq.conf";
		break;
	case Type::REF:
		conf_path = "ssphh_ground_truth.conf";
		hq_conf_path = "ssphh_ground_truth_hq.conf";
		break;
	case Type::REF_CubeMap:
		break;
	case Type::Sky:
		break;
	}
}

CoronaJob::~CoronaJob()
{
}

void CoronaJob::Start(CoronaSceneFile &coronaScene, SimpleSceneGraph &ssg)
{
	if (state != State::Ready)
	{
		return;
	}

	if (!ignoreCache)
	{
		FilePathInfo fpi(isHQ ? hq_output_path_exr : output_path_exr);
		if (fpi.Exists())
		{
			state = State::Finished;
			return;
		}
	}

	if (1)
	{
		float tonemap = ssg.environment.toneMapExposure;
		if (type == Type::VIZ)
		{
			tonemap = 0.0f;
		}
		if (type == Type::GEN)
		{
			tonemap = 0.0f;
		}
		ofstream fout(scene_name + "_tonemap.conf");
		fout << "Float colorMap.simpleExposure = " << tonemap << endl;
		if (type == Type::REF)
		{
			fout << "Int image.width = " << imageWidth << endl;
			fout << "Int image.height = " << imageHeight << endl;
		}
		else
		{
			fout << "Int image.width = " << 6 * 128 << endl;
			fout << "Int image.height = " << 128 << endl;
		}
		if (!isHQ)
		{
			fout << "Int shading.maxRayDepth = " << maxRayDepth << endl;
			fout << "Int progressive.passLimit = " << passLimit << endl;
		}
		fout.close();
	}

	double t0 = hflog.getSecondsElapsed();
	state = State::Running;
	bool result = true;
	switch (type)
	{
	case Type::REF:
		coronaScene.WriteSCN(scene_path, ssg);
		result = Run();
		break;
	case Type::REF_CubeMap:
		coronaScene.WriteCubeMapSCN(scene_path, ssg);
		result = Run();
		break;
	case Type::Sky:
		coronaScene.WriteSkySCN(scene_path, ssg);
		result = Run();
		break;
	case Type::GEN:
		coronaScene.WriteSphlVizSCN(scene_path, ssg, -1, recvLight);
		result = Run();
		break;
	case Type::VIZ:
		coronaScene.WriteSphlVizSCN(scene_path, ssg, sendLight, recvLight);
		result = Run();
		break;
	default:
		break;
	}
	elapsedTime = hflog.getSecondsElapsed() - t0;
	state = result ? State::Finished : State::Error;

	string tonemapconf = scene_name + "_tonemap.conf";
#ifdef WIN32
	DeleteFile(tonemapconf.c_str());
	DeleteFile(scene_path.c_str());
#else
	execl("rm", tonemapconf.c_str());
	execl("rm", scene_path.c_str());
#endif
}

void CoronaJob::CopySPH(const Sph4f &sph_)
{
	if (!IsFinished() && !IsGEN() && !IsVIZ())
		return;
	memset(sph, 0, sizeof(float) * 484);
	for (size_t i = 0; i < sph_.size(); i++)
	{
		sph[121 * 0 + i] = sph_.r().getCoefficient(i);
		sph[121 * 1 + i] = sph_.g().getCoefficient(i);
		sph[121 * 2 + i] = sph_.g().getCoefficient(i);
		sph[121 * 3 + i] = sph_.a().getCoefficient(i);
	}
}

void CoronaJob::CopySPHToSph4f(Sph4f &sph_)
{
	sph_.resize(MaxSphlDegree);
	for (size_t i = 0; i < sph_.size(); i++)
	{
		sph_.r().setCoefficient(i, sph[121 * 0 + i]);
		sph_.g().setCoefficient(i, sph[121 * 1 + i]);
		sph_.b().setCoefficient(i, sph[121 * 2 + i]);
		sph_.a().setCoefficient(i, sph[121 * 3 + i]);
	}
}

string CoronaJob::MakeCoronaCommandLine()
{
	ostringstream cmd;
	cmd << "\"C:\\Program Files\\Corona\\Corona.exe\" " << scene_path << " -silent";

	if (isHDR)
	{
		cmd << " -oR " << (isHQ ? hq_output_path_exr : output_path_exr);
	}
	else
	{
		cmd << " -o " << (isHQ ? hq_output_path_exr : output_path_exr);
	}

	if (isHQ)
	{
		cmd << " -c " << hq_conf_path;
	}
	else
	{
		cmd << " -c " << conf_path;
	}

	cmd << " -c " << scene_name << "_tonemap.conf";

	return cmd.str();
}

string CoronaJob::MakeConvertCommandLine()
{
	ostringstream cmd;
	if (isHQ)
	{
		cmd << "magick " << hq_output_path_exr << " -compress none " << hq_output_path_ppm;
	}
	else
	{
		cmd << "magick " << output_path_exr << " -compress none " << output_path_ppm;
	}
	return cmd.str();
}

bool CoronaJob::Run()
{
	int retval = 0;
	retval = lastCoronaRetval = system(MakeCoronaCommandLine().c_str());
	if (retval != 0)
	{
		hflog.errorfn(__FUNCTION__, "unable to run corona");
		return false;
	}
	//retval = lastConvertRetval = system(MakeConvertCommandLine().c_str());
	//if (retval != 0) return false;

	{
		ostringstream cmd;
		cmd << "magick " << output_path_exr << " " << output_path_png;
		retval = lastConvertRetval = system(cmd.str().c_str());
		if (retval != 0)
		{
			hflog.errorfn(__FUNCTION__, "unable to convert EXR to PNG");
			return false;
		}
	}
	{
		ostringstream cmd;
		cmd << "magick " << output_path_png << " -compress none " << output_path_ppm;
		retval = lastConvertRetval = system(cmd.str().c_str());
		if (retval != 0)
		{
			hflog.errorfn(__FUNCTION__, "unable to convert PNG to PPM");
			return false;
		}
	}
	return true;
}

string CoronaJob::ToString() const
{
	ostringstream ostr;
	//ostr << imageWidth << endl;
	//ostr << imageHeight << endl;
	//ostr << ignoreCache << endl;
	//ostr << maxRayDepth << endl;
	//ostr << passLimit << endl;
	//ostr << elapsedTime << endl;
	//ostr << finished << endl;
	//ostr << working << endl;
	//ostr << isHQ << endl;
	//ostr << isHDR << endl;
	//ostr << sendLight << endl;
	//ostr << recvLight << endl;
	//ostr << (type == Type::VIZ ? "VIZ" : (type == Type::GEN ? "GEN" : "REF")) << endl;
	//ostr << scene_name << endl;
	//ostr << output_path_ppm << endl;

	return ostr.str();
}

void CoronaJob::FromString(const string &str)
{
}

//////////////////////////////////////////////////////////////////
// Static CoronaJob methods //////////////////////////////////////
//////////////////////////////////////////////////////////////////

string CoronaJob::MakeREFName(const string &prefix, bool isCubeMap, bool isHDR, bool isHQ, bool ks, int MaxRayDepth, int PassLimit)
{
	ostringstream ostr;

	if (!prefix.empty())
		ostr << prefix << "_";
	ostr << "REF";
	if (isCubeMap)
		ostr << "_cubemap";
	if (isHDR)
		ostr << "_hdr";
	if (isHQ)
		ostr << "_hq";
	else
	{
		ostr << "_" << setw(2) << setfill('0') << MaxRayDepth;
		ostr << "_" << setw(2) << setfill('0') << PassLimit;
		if (ks)
			ostr << "_Ks";
	}
	return ostr.str();
}

string CoronaJob::MakeVIZName(const string &prefix, int srcLightIndex, int recvLightIndex, bool isHDR, bool isHQ, bool ks, int MaxRayDepth, int PassLimit)
{
	ostringstream ostr;
	if (!prefix.empty())
		ostr << prefix << "_";
	ostr << "VIZ" << setfill('0') << setw(2) << srcLightIndex + 1 << setw(2) << recvLightIndex + 1;
	if (isHDR)
		ostr << "_hdr";
	if (isHQ)
		ostr << "_hq";
	else
	{
		ostr << "_" << setw(2) << setfill('0') << MaxRayDepth;
		ostr << "_" << setw(2) << setfill('0') << PassLimit;
		if (ks)
			ostr << "_Ks";
	}
	return ostr.str();
}

string CoronaJob::MakeGENName(const string &prefix, int recvLightIndex, bool isHDR, bool isHQ, bool ks, int MaxRayDepth, int PassLimit)
{
	ostringstream ostr;
	if (!prefix.empty())
		ostr << prefix << "_";
	ostr << "GEN" << setfill('0') << setw(2) << recvLightIndex + 1;
	if (isHDR)
		ostr << "_hdr";
	if (isHQ)
		ostr << "_hq";
	else
	{
		if (MaxRayDepth >= 0)
			ostr << "_" << setw(2) << setfill('0') << MaxRayDepth;
		if (PassLimit >= 0)
			ostr << "_" << setw(2) << setfill('0') << PassLimit;
		if (ks)
			ostr << "_Ks";
	}
	return ostr.str();
}

string CoronaJob::MakeHIERName(const string &prefix, int sendLightIndex, int MaxDegrees)
{
	ostringstream ostr;
	if (!prefix.empty())
		ostr << prefix << "_";
	ostr << "HIER" << setfill('0') << setw(2) << sendLightIndex + 1;
	if (MaxDegrees >= 0)
		ostr << "_" << MaxDegrees;
	return ostr.str();
}

} // namespace Fluxions