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
#include <future>
#include "fluxions.hpp"
#include "fluxions_fileio.hpp"
#include "fluxions_simple_scene_graph.hpp"


namespace Fluxions
{
	using namespace std;


	void async_pbsky_compute(Fluxions::PhysicallyBasedSky *pbsky, bool genCubeMap, bool genCylMap, bool *completed, double *timeElapsed = nullptr)
	{
		Viperfish::StopWatch stopwatch;

		pbsky->ComputeCubeMap(64, false, 1.0f);
		//pbsky.ComputeCylinderMap(128, 32);
		//pbsky.generatedCylMap.savePPM("pbsky.ppm");

		stopwatch.Stop();
		if (timeElapsed != nullptr)
		{
			*timeElapsed = stopwatch.GetMillisecondsElapsed();
		}
		if (completed != nullptr)
		{
			*completed = true;
		}
	}


	void SimpleEnvironment::Update(const BoundingBoxf & bbox)
	{
		pbsky.ComputeSunGroundRadiances();
		curSunDirTo = pbsky.GetSunVector();
		curGroundRadiance = pbsky.GetGroundRadiance().ToVector4();
		curSunDiskRadiance = pbsky.GetSunDiskRadiance().ToVector4();

		if (within(bbox.MaxSize(), 0.0f, 1000.0f))
		{
			//const float padding = 1.0f;
			//const float size = bbox.MaxSize();
			//float correctRadius = sqrtf(0.75f) * boundingBox.MaxSize();
			//float correctBase = correctRadius;
			//float correctHeight = sqrt(1.0f - powf(sinf(fov / 2.0f), 2.0f)) * 4.0f * correctRadius;
			//float fov = 45.0f * FX_F32_DEGREES_TO_RADIANS;

			const float radius = bbox.RadiusRounded();
			sunShadowMapTarget = bbox.Center();
			sunShadowMapDistance = radius;	// calculation for perspective: sqrt(1.0f - powf(sinf(fov / 2.0f), 2.0f)) * 4.0f * radius;
			sunShadowMapNearZ = sunShadowMapDistance - radius;
			sunShadowMapFarZ = sunShadowMapDistance + radius;
			sunShadowMapOrigin = curSunDirTo.norm() * sunShadowMapDistance + sunShadowMapTarget;
			sunShadowMapUp = ((curSunDirTo.cross(Vector3f(0.0f, 1.0f, 0.0f))).cross(curSunDirTo)).norm();
		}
		else
		{
			sunShadowMapUp.reset(0.0f, 1.0f, 0.0f);
			sunShadowMapOrigin = curSunDirTo.norm() * 20.0f;
			sunShadowMapTarget.reset(0.0f, 0.0f, 0.0f);
			sunShadowMapUp = ((Vector3f(0.0f, 1.0f, 0.0f).cross(curSunDirTo)).cross(curSunDirTo)).norm();
			sunShadowMapNearZ = 1.0f;
			sunShadowMapFarZ = 100.0f;
		}
		sunShadowProjectionMatrix.LoadIdentity();
		//sunShadowProjectionMatrix.Perspective(45.0f, 1.0f, sunShadowMapNearZ, sunShadowMapFarZ);
		sunShadowProjectionMatrix.Ortho(
			sunShadowMapZoom * (-sunShadowMapDistance + sunShadowMapOffset.x), sunShadowMapZoom * (sunShadowMapDistance + sunShadowMapOffset.x),
			sunShadowMapZoom * (-sunShadowMapDistance + sunShadowMapOffset.y), sunShadowMapZoom * (sunShadowMapDistance + sunShadowMapOffset.y),
			sunShadowMapNearZ, sunShadowMapFarZ);
		sunShadowViewMatrix.LoadIdentity();
		sunShadowViewMatrix.LookAt(sunShadowMapOrigin, sunShadowMapTarget, sunShadowMapUp);
		sunShadowInverseViewMatrix = sunShadowViewMatrix.AsInverse();

		if (isSkyComputed && pbskyColorMapId != 0)
		{
			glutSetErrorMessage(__FILE__, __LINE__, "%s", __FUNCTION__);
			glBindTexture(GL_TEXTURE_CUBE_MAP, pbskyColorMapId);
			for (int i = 0; i < 6; i++)
			{
				GLsizei size = pbsky.generatedCubeMap.width();
				void *pixels = pbsky.generatedCubeMap.getPixels(i);
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA32F, size, size, 0, GL_RGBA, GL_FLOAT, pixels);
			}
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
			glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
			glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
			isSkyComputed = false;
			isSkyComputing = false;
		}
	}


	void SimpleEnvironment::ComputePBSky()
	{
		if (isSkyComputing) return;

		isSkyComputed = false;
		isSkyComputing = true;
		lastSkyGenTime = 0.0;
		auto handle = async(std::launch::async, async_pbsky_compute, &pbsky, pbskyGenCubeMap, pbskyGenCylMap, &isSkyComputed, &lastSkyGenTime);
	}


	struct SimpleRendererUniforms
	{
		/* This was this code, but g++ doesn't like it. The better solution would be
		 * to create a pack() function which creates a memory buffer with the contents
		 * packed into it.
		union {
			struct {
				Matrix4f projectionMatrix;
				Matrix4f cameraMatrix;
				Matrix4f worldMatrix;
				Matrix4f projectionViewMatrix;
				Matrix4f modelViewMatrix;
				Matrix4f projectionModelViewMatrix;

				Vector4f cameraPosition;
				Vector4f lightPositions[32];
				bool lightEnabled[32];
			};
			GLubyte data[4096];
		};
		*/

		Matrix4f projectionMatrix;
		Matrix4f cameraMatrix;
		Matrix4f worldMatrix;
		Matrix4f projectionViewMatrix;
		Matrix4f modelViewMatrix;
		Matrix4f projectionModelViewMatrix;

		Vector4f cameraPosition;
		Vector4f lightPositions[32];
		bool lightEnabled[32];


		void ComputeExtraValues() {
			projectionViewMatrix = projectionMatrix * cameraMatrix;
			modelViewMatrix = cameraMatrix * worldMatrix;
			projectionModelViewMatrix = projectionMatrix * cameraMatrix * worldMatrix;
			cameraPosition = cameraMatrix * Vector4f(0.0f, 0.0f, 0.0f, 1.0f);
		}


		void Reset() {
			projectionMatrix.LoadIdentity();
			cameraMatrix.LoadIdentity();
			worldMatrix.LoadIdentity();
			projectionViewMatrix.LoadIdentity();
			modelViewMatrix.LoadIdentity();
			projectionModelViewMatrix.LoadIdentity();
		}
	};



	struct SimpleUniformBuffer
	{
		string uniformBufferName;
		GLuint blockBinding;

		SimpleUniformBuffer(const string &name, GLuint blockBinding) : uniformBufferName(name), blockBinding(blockBinding)
		{

		}

		virtual GLint GetSize() const { return 0; }
	};


	SimpleSceneGraph::SimpleSceneGraph()
	{
	}


	SimpleSceneGraph::~SimpleSceneGraph()
	{
	}


	void SimpleSceneGraph::Reset()
	{
		boundingBox.Reset();
		sceneFileLines.clear();
		pathsToTry.clear();
		spheres.Clear();
		currentTransform.LoadIdentity();
		ssphhLights.clear();
		ssphhLights.reserve(16);
		geometry.clear();
		shaderMaps.clear();
		geometryObjects.clear();
		pointLights.clear();


		// set time to longest time of the day
		environment.pbsky.SetLocalDate(1, 7, 2017, true, 0);
		environment.pbsky.SetLocalTime(16, 0, 0, 0.0f);
		environment.pbsky_dtg = environment.pbsky.GetCivilDateTime();
		//shaderMaps.Clear();
		//geometry.Clear();
	}


	bool SimpleSceneGraph::Load(const string &filename)
	{
		// Use this as a template
		//ifstream fin(filename.c_str());

		//int linecount = 0;
		//int totallinecount = 0;
		//string str;
		//istringstream istr;
		//while (1)
		//{
		//	getline(fin, str);
		//	linecount++;
		//	totallinecount++;
		//	if (!fin)
		//	{
		//		break;
		//	}
		//	istr.clear();
		//	istr.str(str);
		//}

		//fin.close();

		// Try loading file. If we fail, return false
		FilePathInfo scenefpi(filename);
		name = scenefpi.fname;
		ifstream fin(filename.c_str());

		if (!fin)
		{
			return false;
		}

		// Loading is successful, so go ahead and add path of the scene graph
		// file so we can reference it if necessary for other referenced files
		// in the mtllibs and objs.
		Reset();
		// Use the path of the scene graph as the default path, then the current directory
		pathsToTry.push_back(scenefpi.dir);
		pathsToTry.push_back("./");

		// proceed to parse the file and read each line.
		int linecount = 0;
		int totallinecount = 0;
		string str;
		istringstream istr;
		while (1)
		{
			// read a line from the file and set up a string stream.
			str.clear();
			getline(fin, str);
			sceneFileLines.push_back(str);
			linecount++;
			totallinecount++;

			if (!fin)
			{
				break;
			}

			istr.clear();
			istr.str(str);
			str.clear();
			istr >> str;

			if (str == "transform")
			{
				currentTransform = ReadAffineMatrix4f(istr);
			}
			else if (str == "mtllib")
			{
				string mtllibFilename = ReadString(istr);
				FilePathInfo fpi(mtllibFilename);
				string filenameToTry = fpi.FindFileIfExists(pathsToTry);
				if (filenameToTry.empty())
				{
					hflog.error("%s(): MTLLIB %s was not found.", __FUNCTION__, mtllibFilename.c_str());
				}
				else
				{
					if (materials.Load(fpi.fname, filenameToTry))
					{
						hflog.info("%s(): MTLLIB %s loaded.", __FUNCTION__, mtllibFilename.c_str());
					}
					else
					{
						hflog.error("%s(): MTLLIB %s had an error while loading.", __FUNCTION__, mtllibFilename.c_str());
					}
				}
			}
			else if (str == "conffile")
			{
				string confFilename = ReadString(istr);
				FilePathInfo fpi(confFilename);
				string filenameToTry = fpi.FindFileIfExists(pathsToTry);
				if (filenameToTry.empty())
				{
					hflog.error("%s(): CONF file %s was not found.", __FUNCTION__, confFilename.c_str());
				}
				else
				{
					if (ReadConfFile(filenameToTry))
						hflog.info("%s(): CONF file %s loaded.", __FUNCTION__, confFilename.c_str());
					else
						hflog.error("%s(): CONF file %s had an error while loading.", __FUNCTION__, confFilename.c_str());
				}
			}
			else if (str == "geometryGroup")
			{
				string geoFilename = ReadString(istr);
				FilePathInfo fpi(geoFilename);
				string filenametoTry = fpi.FindFileIfExists(pathsToTry);
				if (filenametoTry.empty())
				{
					hflog.error("%s(): OBJ file %s was not found.", __FUNCTION__, geoFilename.c_str()); 
				}
				else
				{
					if (ReadObjFile(filenametoTry, fpi.fname))
					{
						GLuint id = geometry.Create(fpi.fname);

						SimpleGeometryGroup geometryGroup;
						geometryGroup.transform = currentTransform;
						geometryGroup.mtllibName = materials.GetLibraryName();
						geometryGroup.mtllibId = materials.GetLibraryId();
						geometryGroup.objectName = fpi.fname;
						geometryGroup.objectId = id;
						geometryGroup.bbox = geometryObjects[fpi.fname].BoundingBox;

						geometry[id] = geometryGroup;
						hflog.info("%s(): OBJ file %s loaded.", __FUNCTION__, fpi.fname.c_str());

						// Transform the bounding box of the model into world coordinates
						Vector4f tminBound = geometryGroup.transform * Vector4f(geometryGroup.bbox.minBounds, 1.0f);
						Vector4f tmaxBound = geometryGroup.transform * Vector4f(geometryGroup.bbox.maxBounds, 1.0f);

						boundingBox += tminBound.xyz();
						boundingBox += tmaxBound.xyz();
					}
					else
					{
						hflog.error("%s(): OBJ file %s had an error while loading", __FUNCTION__, geoFilename.c_str());
					}
				}
			}
			else if (str == "enviro")
			{
				istr >> str;
				if (str == "color")
				{
					environment.hasColor = true;
					environment.color = ReadVector3f(istr);
				}
				else if (str == "texmap")
				{
					environment.hasTexmap = true;
					environment.texmap = ReadString(istr);
				}
			}
			else if (str == "pbsky")
			{
				Color4f groundAlbedo;
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

				environment.pbsky.SetNumSamples(samples);
				environment.pbsky.SetLocation(latitude, longitude);
				environment.pbsky.SetTurbidity(turbidity);
				environment.pbsky.SetGroundAlbedo(
					groundAlbedo.r,
					groundAlbedo.g,
					groundAlbedo.b);
			}
			else if (str == "datetime")
			{
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
				
				environment.pbsky.SetLocalDate(day, month, year, isdst != 0, 0);
				environment.pbsky.SetLocalTime(hours, minutes, seconds, 0.0f);
			}
			else if (str == "camera")
			{
				string type = ReadString(istr);
				bool isBadCamera = true;
				camera.projectionMatrix.LoadIdentity();
				camera.viewMatrix.LoadIdentity();
				if (type == "perspective_otrf")
				{
					Vector3f origin = ReadVector3f(istr);
					Vector3f target = ReadVector3f(istr);
					Vector3f roll = ReadVector3f(istr);
					float fov = ReadFloat(istr);
					isBadCamera = false;
					camera.isPerspective = true;
					camera.isOrtho = false;
					camera.fov = fov;
					camera.projectionMatrix.LoadIdentity();
					camera.projectionMatrix.PerspectiveY(camera.fov,
						camera.imageAspect,
						camera.imageNearZ,
						camera.imageFarZ);
					camera.viewMatrix.LookAt(origin, target, roll);
				}
				else if (type == "perspective_tmf")
				{
					Matrix4f tm = ReadAffineMatrix4f(istr);
					float fov = ReadFloat(istr);
					isBadCamera = false;
					camera.isPerspective = true;
					camera.isOrtho = false;
					camera.fov = fov;
					camera.projectionMatrix.LoadIdentity();
					camera.projectionMatrix.Perspective(camera.fov,
						camera.imageAspect,
						camera.imageNearZ,
						camera.imageFarZ);
					camera.viewMatrix = tm;
				}
				else if (type == "ortho_otrw")
				{
					Vector3f origin = ReadVector3f(istr);
					Vector3f target = ReadVector3f(istr);
					Vector3f roll = ReadVector3f(istr);
					float width = ReadFloat(istr);
					isBadCamera = false;
					camera.isPerspective = false;
					camera.isOrtho = true;
					camera.width = width;
					// TODO (projection and view matrix)
				}
				else if (type == "ortho_tmw")
				{
					Matrix4f tm = ReadMatrix4f(istr);
					float width = ReadFloat(istr);
					isBadCamera = false;
					camera.isPerspective = false;
					camera.isOrtho = true;
					camera.width = (float)width;
					camera.viewMatrix = tm;
					// TODO (projection matrix)
				}
				else {
					hflog.error("%s(): unsupported camera format %s.", __FUNCTION__, type.c_str());
				}
				if (!isBadCamera)
				{
					float fstop = 16.0f;
					float filmWidth = 35.0f;
					float focalDist = 100.0f;
					float bokehPolygonalBlades = 3.0f;
					float bokehPolygonalRotation = 0.0f;
					float regionStartX = 0.0f;
					float regionStartY = 0.0f;
					float regionEndX = 1.0f;
					float regionEndY = 1.0f;
					string bokehImg;

					// read optional components
					while (istr)
					{
						istr >> str;
						if (str == "fstop")
						{
							fstop = ReadFloat(istr);
						}
						else if (str == "filmWidth")
						{
							filmWidth = ReadFloat(istr);
						}
						else if (str == "focalDist")
						{
							focalDist = ReadFloat(istr);
						}
						else if (str == "bokehPolygonal")
						{
							bokehPolygonalBlades = ReadFloat(istr);
							bokehPolygonalRotation = ReadFloat(istr);
						}
						else if (str == "region")
						{
							regionStartX = ReadFloat(istr);
							regionStartY = ReadFloat(istr);
							regionEndX = ReadFloat(istr);
							regionEndY = ReadFloat(istr);
						}
						else if (str == "bokehImg")
						{
							bokehImg = ReadString(istr);
						}
					}

					camera.fstop = fstop;
					camera.filmWidth = filmWidth;
					camera.focalDist = focalDist;
					camera.bokehPolygonalBlades = bokehPolygonalBlades;
					camera.bokehPolygonalRotation = bokehPolygonalRotation;
					camera.bokehImg = bokehImg;
					camera.regionStartX = regionStartX;
					camera.regionStartY = regionStartY;
					camera.regionEndX = regionEndX;
					camera.regionEndY = regionEndY;
				}
			}
			else if (str == "tonemap")
			{
				environment.toneMapScale = clamp(ReadFloat(istr), -12.0f, 12.0f);
				environment.toneMapExposure = environment.toneMapScale;
			}
			else if (str == "gamma")
			{
				environment.toneMapGamma = clamp(ReadFloat(istr), -6.0f, 6.0f);
			}
			else if (str == "sun")
			{
				environment.hasSun = true;
				str = ReadString(istr);
				if (str == "dirTo")
				{
					environment.sunDirTo = ReadVector3f(istr);
				}
				str = ReadString(istr);
				if (str == "color")
				{
					environment.sunColor = ReadVector3f(istr);
				}
				str = ReadString(istr);
				if (str == "sizeMult")
				{
					environment.sunSize = ReadFloat(istr);
				}
				environment.sunDirTo.normalize();
				environment.curSunDirTo = environment.sunDirTo;
				environment.sunShadowBiasMatrix.LoadIdentity();
				environment.sunShadowBiasMatrix.ShadowBias();
				environment.sunShadowProjectionMatrix.LoadIdentity();
				environment.sunShadowProjectionMatrix.Perspective(90.0, 1.0, 1.0, 100.0);
				//environment.sunShadowProjectionMatrix.Ortho(-200, 200, -200, 200, -200, 200);
				environment.sunShadowViewMatrix.LoadIdentity();
				environment.sunShadowViewMatrix.LookAt(environment.curSunDirTo * environment.sunSize, Vector3f(0, 0, 0), Vector3f(0, 1, 0));
				environment.sunShadowInverseViewMatrix = environment.sunShadowViewMatrix.AsInverse();
			}
			else if (str == "newmap")
			{
				string mapname = ReadString(istr);
				string texmap = ReadString(istr);
				bool result = ReadTexmap(mapname, texmap);
			}
			else if (str == "sphere")
			{
				string mtlName = ReadString(istr);

				GLuint id = spheres.Create();
				SimpleSphere sphere;
				sphere.transform = currentTransform;
				sphere.mtllibName = materials.GetLibraryName();
				sphere.mtllibId = materials.GetLibraryId();
				sphere.mtlName = mtlName;
				sphere.mtlId = materials.GetMaterialId(mtlName);
				sphere.objectId = id;
				spheres[id] = sphere;
			}
			else if (str == "pointLight" || str == "light")
			{
				SimplePointLight spl;
				spl.name = ReadString(istr);
				spl.index = pointLights.size();
				spl.E0 = ReadFloat(istr);
				spl.falloffRadius = ReadFloat(istr);
				spl.position = ReadVector3f(istr);

				pointLights.push_back(spl);
			}
			else if (str == "sphLight")
			{
				SimpleSSPHHLight sphl;
				sphl.name = ReadString(istr);
				sphl.index = (int)ssphhLights.size();
				sphl.E0 = ReadFloat(istr);
				sphl.falloffRadius = ReadFloat(istr);
				sphl.position = ReadVector3f(istr);
				sphl.orientation = ReadQuaternionf(istr);
				sphl.msph[0] = ReadSphericalHarmonicf(istr);
				sphl.msph[1] = sphl.msph[0];
				sphl.msph[2] = sphl.msph[0];
				sphl.Standardize();
				ssphhLights.push_back(sphl);
			}
			else if (str == "sphl")
			{
				ssphhLights.resize(ssphhLights.size() + 1);
				SimpleSSPHHLight & sphl = ssphhLights.back();
				sphl.name = ReadString(istr);
				sphl.index = (int)ssphhLights.size() - 1;
				sphl.E0 = ReadFloat(istr);
				sphl.falloffRadius = ReadFloat(istr);
				sphl.position = ReadVector3f(istr);
				sphl.ChangeDegrees(MaxSphlDegree);
			}
		}

		fin.close();

		// Load Images
		materials.LoadMaps();
		renderer.AssignMaterialIds(materials);

		return false;
	}


	bool SimpleSceneGraph::Save(const string &filename)
	{
		FilePathInfo fpi(filename);

		ofstream fout(filename.c_str());
		if (!fout)
			return false;

		// output SCN information...
		
		// 1. Camera
		fout << "camera ";
		if (camera.isOrtho)
		{
			fout << "ortho_tmw ";
			WriteAffineMatrix4f(fout, camera.viewMatrix);
			fout << camera.width;
		}
		else
		{
			fout << "perspective_tmf ";
			WriteAffineMatrix4f(fout, camera.viewMatrix);
			fout << camera.fov;
		}
		fout << endl;

		// 2. Environment
		if (environment.hasColor)
		{
			fout << "enviro color ";
			WriteVector3f(fout, environment.color);
			fout << endl;
		}
		if (environment.hasTexmap)
		{
			fout << "enviro texmap ";
			WriteString(fout, environment.texmap);
			fout << endl;
		}
		if (environment.hasSun)
		{
			fout << "sun dirTo ";
			WriteVector3f(fout, environment.curSunDirTo);
			fout << "color ";
			WriteVector3f(fout, environment.sunColor);
			fout << "sizeMult ";
			WriteDouble(fout, environment.sunSize);
			fout << endl;
		}

		// 3. Newmaps
		for (auto it = shaderMaps.begin(); it != shaderMaps.end(); it++)
		{
			fout << "newmap " << it->first << " ";
			WriteString(fout, it->second);
			fout << endl;
		}

		// 4. Spheres
		for (auto it = spheres.begin(); it != spheres.end(); it++)
		{
			SimpleSphere &sphere = it->second;
			if (materials.GetLibraryId() != sphere.mtllibId)
			{
				materials.SetLibrary(sphere.mtllibName);

				fout << "mtllib " << sphere.mtllibName << endl;
			}
			fout << "transform ";
			WriteAffineMatrix4f(fout, sphere.transform);
			fout << endl;
			fout << "sphere ";
			WriteString(fout, sphere.mtlName);
			fout << endl;
		}

		// 5. Geometry Groups
		for (auto it = geometry.begin(); it != geometry.end(); it++)
		{
			SimpleGeometryGroup &geo = it->second;
			if (materials.GetLibraryId() != geo.mtllibId)
			{
				materials.SetLibrary(geo.mtllibName);

				fout << "mtllib " << geo.mtllibName << endl;
			}
			fout << "transform ";
			WriteAffineMatrix4f(fout, geo.transform);
			fout << endl;
			fout << "geometryGroup " << geo.objectName << endl;
		}
		fout.close();

		// 6. Output geometry files
		for (auto it = geometry.begin(); it != geometry.end(); it++)
		{
			SimpleGeometryGroup &geo = it->second;
			geometryObjects[geo.objectId].SaveOBJ(geo.objectName);
		}

		// 7. Output mtllib files
		materials.Save(fpi.dir);

		return true;
	}


	const BoundingBoxf & SimpleSceneGraph::GetBoundingBox()
	{
		return boundingBox;
	}


	bool SimpleSceneGraph::ReadMtlLibFile(const string &filename)
	{
		// Use this as a template
		//ifstream fin(filename.c_str());
		//if (!fin)
		//	return false;

		//vector<string> lines;
		//string str;
		//istringstream istr;
		//while (1)
		//{
		//	if (!fin)
		//		break;
		//	getline(fin, str);
		//	lines.push_back(str);

		//}
		//fin.close();

		//for (auto line = lines.begin(); line != lines.end(); line++)
		//{
		//	istr.clear();
		//	istr.str(*line);
		//}
		//return true;
		return true;
	}


	bool SimpleSceneGraph::ReadConfFile(const string &filename)
	{
		return false;
	}


	bool SimpleSceneGraph::ReadObjFile(const string &filename, const string &geometryName)
	{
		if (geometryObjects.IsAHandle(filename))
			return true;

		OBJStaticModel model;
		bool result = model.LoadOBJ(filename);
		if (!result) {
			return false;
		}

		geometryObjects[geometryName] = model;		
		return true;
	}


	bool SimpleSceneGraph::ReadTexmap(const string &texmapName, const string &texmap)
	{
		return false;
	}


	bool SimpleSceneGraph::ReadCamera(const istream &istr)
	{
		return false;
	}


	void SimpleSceneGraph::InitTexUnits()
	{
		KillTexUnits();

		// Push some texture_ units to our resource manager so we can dish these out as necessary
		for (int i = 0; i < 32; i++)
		{
			GLuint id = 16 + i;
			textureUnits.Add(id);
		}
	}


	void SimpleSceneGraph::KillTexUnits()
	{
		textureUnits.Clear();
	}


	void SimpleSceneGraph::BuildBuffers()
	{
		InitTexUnits();
		renderer.Reset();
		for (auto it = geometry.begin(); it != geometry.end(); it++)
		{
			SimpleGeometryGroup &geo = it->second;
			renderer.SetCurrentObjectId(geo.objectId);
			renderer.SetCurrentMtlLibId(geo.mtllibId);
			renderer.SetCurrentObjectName(geo.objectName);
			renderer.SetCurrentMtlLibName(geo.mtllibName);
			renderer.NewObject();
			geometryObjects[geo.objectId].Render(renderer);
		}
		renderer.AssignMaterialIds(materials);
		renderer.SetCurrentMtlLibName("");
		renderer.SetCurrentMtlLibId(0);

		hflog.infofn(__FUNCTION__, "number of vertices: %d  -- number of triangles: %d", renderer.vertexCount, renderer.triangleCount);

		environment.enviroColorMapUnit = GetTexUnit();
		environment.sunColorMapUnit = GetTexUnit();
		environment.sunDepthMapUnit = GetTexUnit();
		environment.pbskyColorMapUnit = GetTexUnit();

		if (environment.enviroColorMapId == 0) {
			glGenTextures(1, &environment.enviroColorMapId);
		}
		if (environment.sunColorMapId == 0) {
			glGenTextures(1, &environment.sunColorMapId);
		}
		if (environment.sunDepthMapId == 0) {
			glGenTextures(1, &environment.sunDepthMapId);
		}
		if (environment.pbskyColorMapId == 0) {
			glGenTextures(1, &environment.pbskyColorMapId);
		}
	}


	void SimpleSceneGraph::Render(SimpleProgram &program)
	{
		SimpleUniform InverseCameraMatrix;
		SimpleUniform CameraMatrix;
		SimpleUniform ProjectionXMatrix;
		SimpleUniform ProjectionYMatrix;

		camera.projectionMatrix.LoadIdentity();
		camera.projectionMatrix.Perspective(camera.fov, camera.imageAspect, camera.imageNearZ, camera.imageFarZ);

		ProjectionXMatrix = camera.projectionMatrix;

		camera.projectionMatrix.LoadIdentity();
		camera.projectionMatrix.PerspectiveY(camera.fov, camera.imageAspect, camera.imageNearZ, camera.imageFarZ);

		ProjectionYMatrix = camera.projectionMatrix;

		CameraMatrix = camera.viewMatrix;
		InverseCameraMatrix = camera.viewMatrix.AsInverse();

		program.Use();
		program.ApplyUniform("NewProjectionXMatrix", ProjectionXMatrix);
		program.ApplyUniform("NewProjectionYMatrix", ProjectionYMatrix);
		program.ApplyUniform("NewInverseCameraMatrix", InverseCameraMatrix);
		program.ApplyUniform("NewCameraMatrix", CameraMatrix);

		if (debugging)
			hflog.info("%s(): Starting Render", __FUNCTION__);

		GLuint objectId = 0;
		GLuint groupId = 0;
		GLuint mtlId = 0;
		GLuint mtllibId = 0;
		string objectName;
		string groupName;
		string mtllibName;
		string mtlName;

		GLint program_loc_Ka = program.GetUniformLocation("Ka");
		GLint program_loc_Kd = program.GetUniformLocation("Kd");
		GLint program_loc_Ks = program.GetUniformLocation("Ks");
		GLint program_loc_Ke = program.GetUniformLocation("Ke");
		GLint program_loc_Tr = program.GetUniformLocation("Tr");
		GLint program_loc_Tf = program.GetUniformLocation("Tf");
		GLint program_loc_Ns = program.GetUniformLocation("Ns");
		GLint program_loc_Ni = program.GetUniformLocation("Ni");
		GLint program_loc_map_Ka = program.GetUniformLocation("map_Ka");
		GLint program_loc_map_Kd = program.GetUniformLocation("map_Kd");
		GLint program_loc_map_Ks = program.GetUniformLocation("map_Ks");
		GLint program_loc_map_Ke = program.GetUniformLocation("map_Ke");
		GLint program_loc_map_Tr = program.GetUniformLocation("map_Tr");
		GLint program_loc_map_Tf = program.GetUniformLocation("map_Tf");
		GLint program_loc_map_Ni = program.GetUniformLocation("map_Ni");
		GLint program_loc_map_Ns = program.GetUniformLocation("map_Ns");
		GLint program_loc_map_bump = program.GetUniformLocation("map_bump");
		GLint program_loc_map_normal = program.GetUniformLocation("map_normal");
		GLint program_loc_map_Ka_mix = program.GetUniformLocation("map_Ka_mix");
		GLint program_loc_map_Kd_mix = program.GetUniformLocation("map_Kd_mix");
		GLint program_loc_map_Ks_mix = program.GetUniformLocation("map_Ks_mix");
		GLint program_loc_map_Ke_mix = program.GetUniformLocation("map_Ke_mix");
		GLint program_loc_map_Tf_mix = program.GetUniformLocation("map_Tf_mix");
		GLint program_loc_map_Tr_mix = program.GetUniformLocation("map_Tr_mix");
		GLint program_loc_map_Ni_mix = program.GetUniformLocation("map_Ni_mix");
		GLint program_loc_map_Ns_mix = program.GetUniformLocation("map_Ns_mix");

		GLint program_loc_sphere_array = program.GetUniformLocation("Spheres");
		GLint program_loc_sphere_count = program.GetUniformLocation("SpheresCount");
		GLint program_loc_sphere_Ke = program.GetUniformLocation("SpheresKe");

		vector<float> spherePositions;
		vector<float> sphereKe;
		for (auto sphIt = spheres.begin(); sphIt != spheres.end(); sphIt++)
		{
			if (spherePositions.size() > 8)
				break;
			Vector4f pos(0, 0, 0, 1);
			Vector4f radius(1, 0, 0, 1);
			pos = sphIt->second.transform * pos;
			radius = sphIt->second.transform * radius;
			radius = radius - pos;
			float length = radius.length();
			pos.w = length;
			spherePositions.push_back(pos.x);
			spherePositions.push_back(pos.y);
			spherePositions.push_back(pos.z);
			spherePositions.push_back(pos.w);

			Vector3f Ke = materials.SetLibraryMaterial(sphIt->second.mtllibName, sphIt->second.mtlName)->Ke;
			sphereKe.push_back(Ke.r);
			sphereKe.push_back(Ke.g);
			sphereKe.push_back(Ke.b);
			sphereKe.push_back(1.0f);
		}
		glUniform4fv(program_loc_sphere_array, (int)spherePositions.size(), &spherePositions[0]);
		glUniform4fv(program_loc_sphere_Ke, (int)spherePositions.size(), &sphereKe[0]);
		glUniform1i(program_loc_sphere_count, (int)spherePositions.size());

		// apply each material separately
		for (auto libIt = materials.begin(); libIt != materials.end(); libIt++)
		{
			SimpleMaterialLibrary &mtllib = libIt->second;
			mtllibName = mtllib.name;
			mtllibId = materials.GetLibraryId(mtllib.name);
			materials.SetLibrary(mtllib.name);
			
			if (debugging) cout << "SimpleSceneGraph::Render() -- using mtllib " << mtllib.name << endl;
			for (auto mtlIt = mtllib.mtls.begin(); mtlIt != mtllib.mtls.end(); mtlIt++)
			{
				mtlId = mtlIt->first;
				mtlName = materials.GetMaterialName(mtlId);
				while (mtlName.back() == '\0')
					mtlName.resize(mtlName.size() - 1);
				SimpleMaterial &mtl = mtlIt->second;
				materials.SetMaterial(mtlName);
				
				if (debugging) cout << "SimpleSceneGraph::Render() -- using mtl " << mtlId << endl;

				map<string, SimpleMap*> textures;
				GLuint unit = 0;
				if (!mtl.map_Ka.empty()) textures["map_Ka"] = materials.GetTextureMap(mtl.map_Ka);
				if (!mtl.map_Kd.empty()) textures["map_Kd"] = materials.GetTextureMap(mtl.map_Kd);
				if (!mtl.map_Ks.empty()) textures["map_Ks"] = materials.GetTextureMap(mtl.map_Ks);
				if (!mtl.map_Ke.empty()) textures["map_Ke"] = materials.GetTextureMap(mtl.map_Ke);
				if (!mtl.map_Ns.empty()) textures["map_Ns"] = materials.GetTextureMap(mtl.map_Ns);
				if (!mtl.map_Tf.empty()) textures["map_Tf"] = materials.GetTextureMap(mtl.map_Tf);
				if (!mtl.map_Tr.empty()) textures["map_Tr"] = materials.GetTextureMap(mtl.map_Tr);
				if (!mtl.map_bump.empty()) textures["map_bump"] = materials.GetTextureMap(mtl.map_bump);
				if (!mtl.map_normal.empty()) textures["map_normal"] = materials.GetTextureMap(mtl.map_normal);

				if (mtl.map_Ka.empty()) glUniform1f(program_loc_map_Ka_mix, 0.0f);
				if (mtl.map_Kd.empty()) glUniform1f(program_loc_map_Kd_mix, 0.0f);
				if (mtl.map_Ks.empty()) glUniform1f(program_loc_map_Ks_mix, 0.0f);
				if (mtl.map_Ke.empty()) glUniform1f(program_loc_map_Ke_mix, 0.0f);
				if (mtl.map_Ns.empty()) glUniform1f(program_loc_map_Ns_mix, 0.0f);
				if (mtl.map_Tf.empty()) glUniform1f(program_loc_map_Tf_mix, 0.0f);
				if (mtl.map_Tr.empty()) glUniform1f(program_loc_map_Tr_mix, 0.0f);

				for (auto tmapIt = textures.begin(); tmapIt != textures.end(); tmapIt++)
				{
					SimpleMap *pMap = tmapIt->second;
					if (pMap) {
						pMap->unitId = 0;// unit;
						pMap->samplerId = pMap->textureObject.samplerObject.GetId();
						pMap->textureId = pMap->textureObject.GetTextureId();
						pMap->textureObject.Bind(unit, false);
						glTexParameterf(pMap->textureObject.GetTarget(), GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0);
						textures[mtlName] = pMap;
						unit++;
						GLint program_loc = program.GetUniformLocation(tmapIt->first.c_str());
						if (program_loc >= 0)
							glUniform1i(program_loc, pMap->unitId);
						string tmp = tmapIt->first + "_mix";
						program_loc = program.GetUniformLocation(tmp.c_str());
						glUniform1f(program_loc, 1.0f);
					}
				}

				// Apply Material Uniforms to the program shader
				glUniform3fv(program_loc_Kd, 1, mtl.Kd.v);
				glUniform3fv(program_loc_Ks, 1, mtl.Ks.v);
				glUniform3fv(program_loc_Ke, 1, mtl.Ke.v);
				glUniform3fv(program_loc_Ka, 1, mtl.Ka.v);
				glUniform1fv(program_loc_Ni, 1, &mtl.Ni);
				glUniform1fv(program_loc_Ns, 1, &mtl.Ns);
				glUniform3fv(program_loc_Tr, 1, mtl.Tf.v);
				glUniform1fv(program_loc_Tf, 1, &mtl.Tr);

				for (auto geoIt = geometry.begin(); geoIt != geometry.end(); geoIt++)
				{
					SimpleGeometryGroup &geo = geoIt->second;
					if (debugging) cout << "SimpleSceneGraph::Render() -- using OBJ " << geo.objectName << endl;
					objectId = geo.objectId;
					groupId = 0;

					renderer.ApplyIdToMtlNames(mtlName, mtlIt->first);

					// Apply object specific uniforms like transformation matrices
					SimpleUniform ModelViewMatrix;
					ModelViewMatrix = geo.transform;
					program.ApplyUniform("NewModelViewMatrix", ModelViewMatrix);

					// No iterate through each object and render it with this material
					//renderer.RenderIf(objectId, groupId, mtllibId, mtlId, false);
					renderer.RenderIf(geo.objectName, "", geo.mtllibName, mtlName, false);
				}

				// Turn off textures
				for (auto tmapIt = textures.begin(); tmapIt != textures.end(); tmapIt++)
				{
					SimpleMap *pMap = tmapIt->second;
					if (pMap) {
						glutBindTexture(pMap->unitId, pMap->textureObject.GetTarget(), 0);
						
						GLint program_loc = program.GetUniformLocation(tmapIt->first.c_str());
						if (program_loc >= 0)
							glUniform1i(program_loc, pMap->unitId);
						string tmp = tmapIt->first + "_mix";
						program_loc = program.GetUniformLocation(tmp.c_str());
						glUniform1f(program_loc, 0.0f);
					}
				}
				glutSetActiveTexture(GL_TEXTURE0);
			}
		}

		if (debugging) cout << "SimpleSceneGraph::Render() -- END\n";
	}


	void SimpleSceneGraph::RenderZOnly(SimpleProgram &program)
	{
		// Render a bare bones, basic Z only version of this scene
		SimpleUniform InverseCameraMatrix;
		SimpleUniform CameraMatrix;
		SimpleUniform ProjectionMatrix;

		camera.projectionMatrix.LoadIdentity();
		camera.projectionMatrix.PerspectiveY(camera.fov, camera.imageAspect, camera.imageNearZ, camera.imageFarZ);

		ProjectionMatrix = camera.projectionMatrix;
		CameraMatrix = camera.viewMatrix;
		InverseCameraMatrix = camera.viewMatrix.AsInverse();

		program.Use();
		program.ApplyUniform("SSGProjectionMatrix", ProjectionMatrix);
		program.ApplyUniform("SSGInverseCameraMatrix", InverseCameraMatrix);
		program.ApplyUniform("SSGCameraMatrix", CameraMatrix);

		GLuint objectId = 0;
		GLuint groupId = 0;
		GLuint mtlId = 0;
		GLuint mtllibId = 0;
		string objectName;
		string groupName;
		string mtllibName;
		string mtlName;

		GLint program_loc_Kd = program.GetUniformLocation("Kd");

		// apply each material separately
		for (auto libIt = materials.begin(); libIt != materials.end(); libIt++)
		{
			SimpleMaterialLibrary &mtllib = libIt->second;
			mtllibName = mtllib.name;
			mtllibId = materials.GetLibraryId(mtllib.name);
			materials.SetLibrary(mtllib.name);

			for (auto mtlIt = mtllib.mtls.begin(); mtlIt != mtllib.mtls.end(); mtlIt++)
			{
				mtlId = mtlIt->first;
				mtlName = materials.GetMaterialName(mtlId);
				while (mtlName.back() == '\0') mtlName.resize(mtlName.size() - 1);
				SimpleMaterial &mtl = mtlIt->second;
				materials.SetMaterial(mtlName);

				// Apply Diffuse Uniforms to the program shader
				glUniform3fv(program_loc_Kd, 1, mtl.Kd.v);

				for (auto geoIt = geometry.begin(); geoIt != geometry.end(); geoIt++)
				{
					SimpleGeometryGroup &geo = geoIt->second;
					objectId = geo.objectId;
					groupId = 0;

					renderer.ApplyIdToMtlNames(mtlName, mtlIt->first);

					// Apply object specific uniforms like transformation matrices
					SimpleUniform ModelViewMatrix;
					ModelViewMatrix = geo.transform;
					program.ApplyUniform("SSGModelViewMatrix", ModelViewMatrix);

					// Now iterate through each object and render it with this material
					renderer.RenderIf(geo.objectName, "", geo.mtllibName, mtlName, false);
				}
			}
		}
	}


	void SimpleSceneGraph::Render(SimpleProgram &program, bool useMaterials, bool useMaps, bool useZOnly, Matrix4f &projectionMatrix, Matrix4f &cameraMatrix)
	{
		Matrix4f inverseCameraMatrix = cameraMatrix.AsInverse();
		Vector4f cameraPosition(0, 0, 0, 1);
		//cameraPosition = camera.viewMatrix * cameraPosition;
		cameraPosition = inverseCameraMatrix * cameraPosition;
		//cameraPosition = cameraMatrix * cameraPosition;

		SimpleUniform InverseCameraMatrix = cameraMatrix.AsInverse();
		SimpleUniform CameraMatrix = cameraMatrix;
		SimpleUniform ProjectionMatrix = projectionMatrix;
		SimpleUniform CameraPosition = cameraPosition;

		glutBindTextureAndSampler(environment.enviroColorMapUnit, GL_TEXTURE_CUBE_MAP, environment.enviroColorMapId, environment.enviroColorMapSamplerId);
		glutBindTextureAndSampler(environment.pbskyColorMapUnit, GL_TEXTURE_CUBE_MAP, environment.pbskyColorMapId, environment.pbskyColorMapSamplerId);

		program.ApplyUniform("UScreenWidth", SimpleUniform((GLfloat)camera.imageWidth));
		program.ApplyUniform("UScreenHeight", SimpleUniform((GLfloat)camera.imageHeight));
		//program.ApplyUniform("ProjectionMatrix", ProjectionMatrix);
		//program.ApplyUniform("CameraPosition", CameraPosition);
		//program.ApplyUniform("CameraMatrix", CameraMatrix);
		program.ApplyUniform("InverseCameraMatrix", (SimpleUniform)cameraMatrix.AsInverse());
		program.ApplyUniform("SunE0", SimpleUniform(environment.curSunDiskRadiance));// .ssg.SimpleUniform(sunDiskRadiance.toVector4f()));
		program.ApplyUniform("GroundE0", SimpleUniform(environment.curGroundRadiance));// SimpleUniform(groundRadiance.toVector4f()));
		program.ApplyUniform("SunShadowBiasMatrix", SimpleUniform(environment.sunShadowBiasMatrix));
		program.ApplyUniform("SunShadowProjectionMatrix", SimpleUniform(environment.sunShadowProjectionMatrix));
		program.ApplyUniform("SunShadowViewMatrix", SimpleUniform(environment.sunShadowViewMatrix));
		program.ApplyUniform("SunShadowInverseViewMatrix", SimpleUniform(environment.sunShadowInverseViewMatrix));



		// apply each material separately (use the idea that material state changes are worse than geometry ones
		for (auto libIt = materials.begin(); libIt != materials.end(); libIt++)
		{
			SimpleMaterialLibrary &mtllib = libIt->second;
			string mtllibName = mtllib.name;
			GLuint mtllibId = materials.GetLibraryId(mtllib.name);
			materials.SetLibrary(mtllib.name);

			// loop through each material
			for (auto mtlIt = mtllib.mtls.begin(); mtlIt != mtllib.mtls.end(); mtlIt++)
			{
				GLuint mtlId = mtlIt->first;
				string mtlName = materials.GetMaterialName(mtlId);
				SimpleMaterial &mtl = mtlIt->second;
				materials.SetMaterial(mtlName);

				if (useMaterials)
					ApplyMaterialToCurrentProgram(mtl, useMaps);

				// loop through each geometry object
				for (auto geoIt = geometry.begin(); geoIt != geometry.end(); geoIt++)
				{
					SimpleGeometryGroup &geo = geoIt->second;
					GLuint objectId = geo.objectId;
					GLuint groupId = 0;
					renderer.ApplyIdToMtlNames(mtlName, mtlIt->first);

					// Apply object specific uniforms like transformation matrices
					SimpleUniform ModelViewMatrix = geo.transform * geo.addlTransform;
					program.ApplyUniform("ModelViewMatrix", ModelViewMatrix);

					// Now iterate through each object and render it with this material
					if (objectId && groupId && mtllibId && mtlId)
						renderer.RenderIf(objectId, groupId, mtllibId, mtlId, useZOnly);
					else
						renderer.RenderIf(geo.objectName, "", geo.mtllibName, mtlName, useZOnly);
				}

				DisableCurrentTextures();
			}
		}

		glutBindTextureAndSampler(environment.enviroColorMapUnit, GL_TEXTURE_CUBE_MAP, 0, 0);
		glutBindTextureAndSampler(environment.pbskyColorMapUnit, GL_TEXTURE_CUBE_MAP, 0, 0);
	}


	void SimpleSceneGraph::AdvancedRender(SimpleRenderConfiguration &rc)
	{
		//double sceneMaxSize = boundingBox.MaxSize();
		//double sceneDiagonal = ceil(sqrtf(2.0 * sceneMaxSize * sceneMaxSize));
		//double centerDistance2 = ceil((rt.cameraPosition.xyz() - boundingBox.Center()).length());
		//// the closest point is between the uesr's specified near plane
		//// or the distance from the sphere surrounding the scene
		//double znear = max(0.001, centerDistance2 - sceneDiagonal);
		//// the furthest point is our distance to the sphere surrounding the scene
		//double zfar = centerDistance2 + sceneDiagonal;
		//rt.znear = (float)znear;
		//rt.zfar = (float)zfar;

		camera.SetImageParameters((float)rc.viewportRect.w, (float)rc.viewportRect.h, rc.znear, rc.zfar);

		if (!rc.useZOnly && rc.shaderProgram == nullptr)
			return;
		if (rc.useZOnly && rc.zShaderProgram == nullptr)
			return;

		SimpleProgramPtr program;

		if (rc.useZOnly)
		{
			program = rc.zShaderProgram;
		}
		else
		{
			program = rc.shaderProgram;
		}

		if (program->IsLinked() == false)
			return;

		if (rc.renderToFBO)
		{
			rc.fbo.Use();
		}

		GLbitfield clearBits = rc.GetClearBits();

		if (rc.enableDepthTest)
		{
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(rc.depthComparisonTest);
		}

		if (rc.enableCullFace)
		{
			glEnable(GL_CULL_FACE);
			glCullFace(rc.cullFaceMode);
		}

		if (rc.clearColorBuffer)
		{
			glClearColor(rc.clearColor.r, rc.clearColor.g, rc.clearColor.b, rc.clearColor.a);
		}

		if (clearBits)
			glClear(clearBits);

		if (!rc.useZOnly)
		{
			if (rc.enableSRGB)
			{
				glEnable(GL_FRAMEBUFFER_SRGB);
			}

			program->Use();
			locs.GetMaterialProgramLocations(*program);
			ApplyGlobalSettingsToCurrentProgram();
			ApplySpheresToCurrentProgram();
			if (locs.shaderDebugChoice >= 0) glUniform1i(locs.shaderDebugChoice, rc.shaderDebugChoice);
			if (locs.shaderDebugLight >= 0) glUniform1i(locs.shaderDebugLight, rc.shaderDebugLight);
			if (locs.shaderDebugSphl >= 0) glUniform1i(locs.shaderDebugSphl, rc.shaderDebugSphl);

			GLbitfield clearBits = rc.GetClearBits();

			if (rc.enableDepthTest)
			{
				glEnable(GL_DEPTH_TEST);
				glDepthFunc(rc.depthComparisonTest);
			}

			if (rc.clearColorBuffer)
			{
				glClearColor(rc.clearColor.r, rc.clearColor.g, rc.clearColor.b, rc.clearColor.a);
			}

			if (clearBits)
				glClear(clearBits);

			Matrix4f projectionMatrix;
			Matrix4f cameraMatrix;

			if (rc.useSceneCamera)
			{
				projectionMatrix.LoadIdentity();
				projectionMatrix.PerspectiveY(camera.fov, camera.imageAspect, camera.imageNearZ, camera.imageFarZ);
				cameraMatrix = rc.preCameraMatrix * camera.viewMatrix * rc.postCameraMatrix;
				//glViewport(0, 0, camera.imageWidth, camera.imageHeight);
			}
			else
			{
				projectionMatrix.LoadIdentity();
				projectionMatrix.PerspectiveY(rc.fov, (float)rc.viewportRect.aspectRatio(), rc.znear, rc.zfar);
				cameraMatrix = rc.preCameraMatrix * rc.postCameraMatrix;
				//glViewport(rt.X, rt.y, rt.width, rt.height);
			}
			rc.cameraMatrix = cameraMatrix;
			rc.cameraPosition = cameraMatrix.AsInverse().col4();

			//program->ApplyUniform("CameraPosition", (SimpleUniform)(cameraMatrix.AsInverse()).col4());
			program->ApplyUniform("CameraPosition", (SimpleUniform)rc.cameraPosition);
			program->ApplyUniform("CameraMatrix", (SimpleUniform)rc.cameraMatrix);
			program->ApplyUniform("ProjectionMatrix", (SimpleUniform)rc.projectionMatrix);

			glViewport(rc.viewportRect.x, rc.viewportRect.y, rc.viewportRect.w, rc.viewportRect.h);

			if (rc.recomputeProjectionMatrix)
			{
				rc.projectionMatrix.LoadIdentity();
				rc.projectionMatrix.PerspectiveY(rc.fov, (float)rc.viewportRect.aspectRatio(), rc.znear, rc.zfar);
			}
			projectionMatrix = rc.projectionMatrix;

			if (rc.isCubeMap)
			{
				projectionMatrix.LoadIdentity();
				projectionMatrix.Perspective(90.0f, 1.0f, rc.znear, rc.zfar);
				Vector4f cameraPosition(0, 0, 0, 1);
				cameraPosition = cameraMatrix * cameraPosition;
				if (rc.defaultCubeFace >= 0 && rc.defaultCubeFace < 6)
				{
					cameraMatrix.LoadIdentity();
					cameraMatrix.CubeMatrixPosition(rc.defaultCubeFace, -cameraPosition.xyz());
					Render(*program, rc.useMaterials, rc.useMaps, rc.useZOnly, projectionMatrix, cameraMatrix);
				}
				else
				{
					for (int i = 0; i < 6; i++)
					{
						cameraMatrix.LoadIdentity();
						cameraMatrix.Translate(cameraPosition.x, cameraPosition.y, cameraPosition.z);
						cameraMatrix.CubeMatrix(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i);
						Render(*program, rc.useMaterials, rc.useMaps, rc.useZOnly, projectionMatrix, cameraMatrix);
					}
				}
			}
			else
			{
				glutBindTextureAndSampler(environment.sunColorMapUnit, GL_TEXTURE_2D, environment.sunColorMapId, environment.sunColorMapSamplerId);
				glutBindTextureAndSampler(environment.sunDepthMapUnit, GL_TEXTURE_2D, environment.sunDepthMapId, environment.sunDepthMapSamplerId);
				Render(*program, rc.useMaterials, rc.useMaps, rc.useZOnly, projectionMatrix, cameraMatrix);
				glutBindTextureAndSampler(environment.sunColorMapUnit, GL_TEXTURE_2D, 0, 0);
				glutBindTextureAndSampler(environment.sunDepthMapUnit, GL_TEXTURE_2D, 0, 0);
			}
			if (rc.enableSRGB)
			{
				glDisable(GL_FRAMEBUFFER_SRGB);
			}
		}
		else
		{
			if (rc.isCubeMap)
			{
				// vec3 LightPosition
				// float ZFar
				// mat4 CubeShadowMatrices[6]

				Matrix4f cubeShadowMatrices[6];
				for (int face = 0; face < 6; face++)
				{
					cubeShadowMatrices[face] = Matrix4f::MakePerspective(90.0f, 1.0f, rc.znear, rc.zfar) *
						Matrix4f::MakeCubeMatrixPosition(face, rc.cameraPosition.xyz());
				}
				SimpleUniform shadowmats(GL_FLOAT_MAT4, 6, 0, cubeShadowMatrices[0].v);

				program->Use();
				program->ApplyUniform("LightPosition", (SimpleUniform)rc.cameraPosition.xyz());
				program->ApplyUniform("ZNear", (SimpleUniform)rc.znear);
				program->ApplyUniform("ZFar", (SimpleUniform)rc.zfar);
				program->ApplyUniform("CubeShadowMatrices[0]", shadowmats);


				glViewport(rc.viewportRect.x, rc.viewportRect.y, rc.viewportRect.w, rc.viewportRect.h);
				RenderZOnly(program);
			}
			else
			{
				program->Use();
				program->ApplyUniform("ZNear", (SimpleUniform)rc.znear);
				program->ApplyUniform("ZFar", (SimpleUniform)rc.zfar);
				program->ApplyUniform("CameraPosition", (SimpleUniform)rc.cameraPosition);
				program->ApplyUniform("CameraMatrix", (SimpleUniform)rc.cameraMatrix);
				program->ApplyUniform("ProjectionMatrix", (SimpleUniform)rc.projectionMatrix);

				glViewport(rc.viewportRect.x, rc.viewportRect.y, rc.viewportRect.w, rc.viewportRect.h);
				RenderZOnly(program);
			}
		}


		if (rc.renderToFBO)
		{
			rc.fbo.RestoreGLState();
		}


		if (rc.enableDebugView)
		{
			glUseProgram(0);
			glMatrixMode(GL_PROJECTION);
			glPushMatrix();
			glLoadMatrixf(rc.projectionMatrix.m);
			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
			glLoadMatrixf(rc.cameraMatrix.m);

			for (int i = 0; i < spheres.size(); i++)
			{
				glPushMatrix();
				glMultMatrixf(spheres[i].transform.m);
				glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
				glutSolidSphere(0.5, 16, 16);
				glPopMatrix();
			}

			for (int i = 0; i < pointLights.size(); i++)
			{
				glPushMatrix();
				float *v = pointLights[i].position.v;
				glTranslatef(v[0], v[1], v[2]);
				float E0 = pointLights[i].E0;
				glColor4f(E0, E0, E0, 1.0f);
				glutSolidSphere(0.1, 16, 8);
				glPopMatrix();
			}

			glutCamera(camera.fov, 1.0f, 1.0f, 1.0f, camera.imageFarZ, camera.viewMatrix.AsInverse().m, rc.projectionMatrix.m);

			glLineWidth(4.0f);
			glBegin(GL_LINES);
			glColor3f(1.0f, 1.0f, 0.0f);
			Vector3f v1 = environment.sunShadowMapOrigin;
			Vector3f v2 = environment.sunShadowMapTarget;
			Vector3f v3 = v2 - environment.sunShadowMapNearZ * environment.curSunDirTo;
			Vector3f v4 = v2 - environment.sunShadowMapFarZ * environment.curSunDirTo;
			glVertex3f(v1.x, v1.y, v1.z);
			glVertex3f(v2.x, v2.y, v2.z);
			glVertex3f(v2.x, v2.y, v2.z);
			glVertex3f(v4.x, v4.y, v4.z);
			glEnd();
			glLineWidth(1.0f);

			glPushMatrix();
			glTranslatef(v2.x, v2.y, v2.z);
			float fov = 45.0f * FX_F32_DEGREES_TO_RADIANS;
			float height = (environment.sunShadowMapOrigin - environment.sunShadowMapTarget).length();
			float base = sqrt(1.0f - powf(cosf(fov/2.0f), 2.0f)) * height; // sqrt(1 - powf(cos(22.5 degrees)))

			float correctRadius = sqrtf(0.75f) * boundingBox.MaxSize();
			float correctBase = correctRadius;
			float correctHeight = sqrt(1.0f - powf(sinf(fov / 2.0f), 2.0f)) * 4.0f * correctRadius;

			//glutWireSphere(correctRadius, 32, 16);
			//glutWireCone(correctBase, correctHeight, 16, 1);

			float radius = 0.5f * (environment.sunShadowMapFarZ - environment.sunShadowMapNearZ);
			glutWireSphere(radius, 32, 16);
			glutWireCone(base, height, 16, 1);
			glutWireCube(boundingBox.MaxSize());
			glPopMatrix();

			glPushMatrix();
			//glMultMatrixf(environment.sunShadowInverseViewMatrix.m);
			glTranslatef(v2.x, v2.y, v2.z);
			glPopMatrix();

			glPushMatrix();
			glTranslatef(v3.x, v3.y, v3.z);
			glutSolidCube(0.25);
			glPopMatrix();
			glPushMatrix();
			glTranslatef(v4.x, v4.y, v4.z);
			glutSolidCube(0.25);
			glPopMatrix();


			glColor3f(1.0f, 1.0f, 1.0f);

			glMatrixMode(GL_PROJECTION);
			glPopMatrix();
			glMatrixMode(GL_MODELVIEW);
			glPopMatrix();
		}


		for (int i = 0; i < 16; i++)
		{
			if (ssphhLights.size() > i && ssphhLights[i].colorSphlMap.unit > 0)
			{
				FreeTexUnit(ssphhLights[i].colorSphlMap.unit);
				FreeTexUnit(ssphhLights[i].depthSphlMap.unit);
				FreeTexUnit(ssphhLights[i].environmentLightProbeMap.unit);
			}
		}


		if (rc.enableDepthTest)
		{
			glDisable(GL_DEPTH_TEST);
			glDepthFunc(GL_LESS);
		}

		if (rc.enableCullFace)
		{
			glDisable(GL_CULL_FACE);
			glCullFace(GL_BACK);
		}

		glUseProgram(0);
	}


	void SimpleSceneGraph::AdvancedRenderZOnly(const SimpleRenderConfiguration &rc) const
	{
		if (!rc.useZOnly)
			return;
		if (rc.useZOnly && rc.zShaderProgram == nullptr)
			return;

		SimpleProgramPtr program = rc.zShaderProgram;
		if (program->IsLinked() == false)
			return;

		GLbitfield clearBits = rc.GetClearBits();

		if (rc.enableDepthTest)
		{
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(rc.depthComparisonTest);
		}

		if (rc.enableCullFace)
		{
			glEnable(GL_CULL_FACE);
			glCullFace(rc.cullFaceMode);
		}

		if (rc.clearColorBuffer)
		{
			glClearColor(rc.clearColor.r, rc.clearColor.g, rc.clearColor.b, rc.clearColor.a);
		}

		if (clearBits)
			glClear(clearBits);

		if (rc.isCubeMap)
		{
			Matrix4f cubeShadowMatrices[6];
			for (int face = 0; face < 6; face++)
			{
				cubeShadowMatrices[face] = Matrix4f::MakePerspective(90.0f, 1.0f, rc.znear, rc.zfar) *
					Matrix4f::MakeCubeMatrixPosition(face, rc.cameraPosition.xyz());
			}
			SimpleUniform shadowmats;
			shadowmats.UniformMatrix2x4fv(6, 0, cubeShadowMatrices[0].v);
			// upload cube shadow matrices
			program->Use();
			program->ApplyUniform("LightPosition", (SimpleUniform)rc.cameraPosition.xyz());
			program->ApplyUniform("ZNear", (SimpleUniform)rc.znear);
			program->ApplyUniform("ZFar", (SimpleUniform)rc.zfar);
			GLint csmLoc = program->GetUniformLocation("CubeShadowMatrices[0]");
			if (csmLoc >= 0)
			{
				glUniformMatrix4fv(csmLoc, 6, GL_FALSE, cubeShadowMatrices[0].m);
			}

			glViewport(rc.viewportRect.x, rc.viewportRect.y, rc.viewportRect.w, rc.viewportRect.h);
			RenderZOnly(program);
		}
		else
		{
			// 2D Only
			Matrix4f projectionMatrix;
			Matrix4f cameraMatrix;
			Vector4f cameraPosition;

			projectionMatrix.LoadIdentity();
			projectionMatrix.PerspectiveY(rc.fov, (float)rc.viewportRect.aspectRatio(), rc.znear, rc.zfar);
			cameraMatrix = rc.preCameraMatrix * camera.viewMatrix * rc.postCameraMatrix;
			cameraPosition = cameraMatrix.AsInverse().col4();

			program->Use();
			program->ApplyUniform("CameraPosition", (SimpleUniform)cameraPosition);
			program->ApplyUniform("CameraMatrix", (SimpleUniform)cameraMatrix);
			program->ApplyUniform("ProjectionMatrix", (SimpleUniform)projectionMatrix);

			glViewport(rc.viewportRect.x, rc.viewportRect.y, rc.viewportRect.w, rc.viewportRect.h);
			RenderZOnly(program);
		}
		
		if (rc.enableDepthTest)
		{
			glDisable(GL_DEPTH_TEST);
			glDepthFunc(GL_LESS);
		}

		if (rc.enableCullFace)
		{
			glDisable(GL_CULL_FACE);
			glCullFace(GL_BACK);
		}

		glUseProgram(0);
	}


	void SimpleSceneGraph::RenderZOnly(SimpleProgramPtr & program) const
	{
		if (!program)
			return;

		// loop through each geometry object
		for (auto geoIt = geometry.cbegin(); geoIt != geometry.cend(); geoIt++)
		{
			const SimpleGeometryGroup &geo = geoIt->second;

			// Apply object specific uniforms like transformation matrices
			SimpleUniform ModelViewMatrix = geo.transform * geo.addlTransform;
			program->ApplyUniform("ModelViewMatrix", ModelViewMatrix);
			program->ApplyUniform("WorldMatrix", ModelViewMatrix);
			renderer.RenderIf(geo.objectId, 0, 0, 0, true);
		}
	}



	void SimpleSceneGraph::ApplyGlobalSettingsToCurrentProgram()
	{
		if (locs.enviroCubeMap >= 0) glUniform1i(locs.enviroCubeMap, environment.enviroColorMapUnit);
		if (locs.enviroCubeMapAmount >= 0) glUniform1f(locs.enviroCubeMapAmount, 0.0);
		if (locs.pbskyCubeMap >= 0) glUniform1i(locs.pbskyCubeMap, environment.pbskyColorMapUnit);
		if (locs.sunDirTo >= 0) glUniform3fv(locs.sunDirTo, 1, environment.curSunDirTo.v);
		if (locs.sunColor >= 0) glUniform3fv(locs.sunColor, 1, environment.sunColor.v);
		if (locs.sunSize >= 0) glUniform1f(locs.sunSize, environment.sunSize);
		if (locs.sunShadowMap >= 0) glUniform1i(locs.sunShadowMap, environment.sunDepthMapUnit);
		if (locs.sunColorMap >= 0) glUniform1i(locs.sunColorMap, environment.sunColorMapUnit);
		if (locs.toneMapScale >= 0) glUniform1f(locs.toneMapScale, 2.5f * powf(2.0f, environment.toneMapScale));
		if (locs.toneMapExposure >= 0) glUniform1f(locs.toneMapExposure, 2.5f * powf(2.0f, environment.toneMapExposure));
		if (locs.toneMapGamma >= 0) glUniform1f(locs.toneMapGamma, environment.toneMapGamma);
	}


	void SimpleSceneGraph::SetUniforms(SimpleProgramPtr &program)
	{
		if (!program)
			return;
		program->Use();
		GLint enviroCubeMap = program->GetUniformLocation("enviroCube");
		
		program->ApplyUniform("EnviroColorMap", (SimpleUniform)environment.enviroColorMapUnit);

		__ShaderProgramLocations spl;
		spl.GetMaterialProgramLocations(*program);
		if (locs.enviroCubeMap >= 0) glUniform1i(spl.enviroCubeMap, environment.enviroColorMapUnit);
		if (spl.enviroCubeMapAmount >= 0) glUniform1f(spl.enviroCubeMapAmount, 0.0);
		if (spl.pbskyCubeMap >= 0) glUniform1i(spl.pbskyCubeMap, environment.pbskyColorMapUnit);
		if (spl.sunDirTo >= 0) glUniform3fv(spl.sunDirTo, 1, environment.curSunDirTo.v);
		if (spl.sunColor >= 0) glUniform3fv(spl.sunColor, 1, environment.sunColor.v);
		if (spl.sunSize >= 0) glUniform1f(spl.sunSize, environment.sunSize);
		if (spl.sunShadowMap >= 0) glUniform1i(spl.sunShadowMap, environment.sunDepthMapUnit);
		if (spl.sunColorMap >= 0) glUniform1i(spl.sunColorMap, environment.sunColorMapUnit);
		if (spl.sunE0 >= 0) glUniform4fv(spl.sunE0, 1, environment.curSunDiskRadiance.v);
		if (spl.groundE0 >= 0) glUniform4fv(spl.groundE0, 1, environment.curGroundRadiance.v);
	}


	void SimpleSceneGraph::ApplySpheresToCurrentProgram()
	{
		vector<float> spherePositions;
		vector<float> sphereKe;
		int numSpheres = 0;
		for (auto sphIt = spheres.begin(); sphIt != spheres.end(); sphIt++)
		{
			if (spherePositions.size() > 8)
				break;
			Vector4f pos(0, 0, 0, 1);
			Vector4f radius(1, 0, 0, 1);
			pos = sphIt->second.transform * pos;
			radius = sphIt->second.transform * radius;
			radius = radius - pos;
			float length = radius.length();
			pos.w = length;
			SimpleMaterial *mtl = materials.SetLibraryMaterial(sphIt->second.mtllibName, sphIt->second.mtlName);
			// only push spheres that are emissive...
			if (mtl)
			{
				spherePositions.push_back(pos.x);
				spherePositions.push_back(pos.y);
				spherePositions.push_back(pos.z);
				spherePositions.push_back(pos.w);

				Vector3f Ke = mtl->Ke;
				sphereKe.push_back(Ke.r);
				sphereKe.push_back(Ke.g);
				sphereKe.push_back(Ke.b);
				sphereKe.push_back(1.0f);
				numSpheres++;
			}
		}
		if (numSpheres > 0)
		{
			if (locs.sphere_array >= 0) glUniform4fv(locs.sphere_array, numSpheres, &spherePositions[0]);
			if (locs.sphere_Ke >= 0) glUniform4fv(locs.sphere_Ke, numSpheres, &sphereKe[0]);
		}
		if (locs.sphere_count >= 0) glUniform1i(locs.sphere_count, numSpheres);

		int v_Enabled[16] = { 0 };
		Vector3f v_E0[16];
		Vector3f v_Position[16];
		int v_LightProbeCubeMapUnit[16] = { 0 };
		int v_EnvironmentCubeMapUnit[16] = { 0 };
		int v_DepthShadowCubeMapUnit[16] = { 0 };
		GLuint v_LightProbeCubeMapTexId[16] = { 0 };
		GLuint v_EnvironmentCubeMapTexId[16] = { 0 };
		GLuint v_DepthShadowCubeMapTexId[16] = { 0 };
		GLfloat v_DepthShadowZFar[16] = { 0.0 };
		Vector3f v_sph[16 * 9] = { 0.0 };
		int v_MaxIndex = -1;

		for (int i = 0; i < 16; i++)
		{
			if (locs.sphl_lights_enabled[i] < 0)
				continue;

			if (ssphhLights.size() > i && ssphhLights[i].enabled)
			{
				v_MaxIndex = i;
				v_Enabled[i] = 1;
				v_E0[i] = Vector3f(ssphhLights[i].E0);
				v_Position[i] = ssphhLights[i].position.xyz();
				v_LightProbeCubeMapTexId[i] = ssphhLights[i].hierLightProbeTexture.GetTexture();
				v_LightProbeCubeMapUnit[i] = ssphhLights[i].colorSphlMap.unit = GetTexUnit();
				glutBindTexture(v_LightProbeCubeMapUnit[i], GL_TEXTURE_CUBE_MAP, v_LightProbeCubeMapTexId[i]);
				v_EnvironmentCubeMapTexId[i] = ssphhLights[i].coronaLightProbeTexture.GetTexture();
				v_EnvironmentCubeMapUnit[i] = ssphhLights[i].environmentLightProbeMap.unit = GetTexUnit();
				glutBindTexture(v_EnvironmentCubeMapUnit[i], GL_TEXTURE_CUBE_MAP, v_EnvironmentCubeMapTexId[i]);
				v_DepthShadowCubeMapTexId[i] = ssphhLights[i].depthSphlMap.texture.GetTexture();
				v_DepthShadowCubeMapUnit[i] = ssphhLights[i].depthSphlMap.unit = GetTexUnit();
				glutBindTexture(v_DepthShadowCubeMapUnit[i], GL_TEXTURE_CUBE_MAP, v_DepthShadowCubeMapTexId[i]);
				v_DepthShadowZFar[i] = ssphhLights[i].depthSphlMap.zfar;
				for (int sph = 0; sph < 9; sph++)
				{
					v_sph[i * 9 + sph].x = ssphhLights[i].msph[0].getCoefficient(sph);
					v_sph[i * 9 + sph].y = ssphhLights[i].msph[1].getCoefficient(sph);
					v_sph[i * 9 + sph].z = ssphhLights[i].msph[2].getCoefficient(sph);
				}
			}
		}
		if (v_MaxIndex >= 0)
		{
			if (locs.sphl_light_count >= 0) glUniform1i(locs.sphl_light_count, v_MaxIndex + 1);

			for (int i = 0; i < 16; i++)
			{
				glUniform1iv(locs.sphl_lights_enabled[i], 1, &v_Enabled[i]);
				glUniform3fv(locs.sphl_lights_E0[i], 1, v_E0[i].v);
				glUniform3fv(locs.sphl_lights_position[i], 1, v_Position[i].v);
				glUniform1iv(locs.sphl_lights_lightProbeCubeMap[i], 1, &v_LightProbeCubeMapUnit[i]);
				glUniform1iv(locs.sphl_lights_environmentCubeMap[i], 1, &v_EnvironmentCubeMapUnit[i]);
				glUniform1iv(locs.sphl_lights_depthShadowCubeMap[i], 1, &v_DepthShadowCubeMapUnit[i]);
				glUniform1fv(locs.sphl_lights_depthShadowZFar[i], 1, &v_DepthShadowZFar[i]);
				for (int j = 0; j < 9; j++)
				{
					glUniform3fv(locs.sphl_lights_sph[i][j], 9, v_sph[i * 9].v);
				}
			}

			glActiveTexture(GL_TEXTURE0);
		}
		else
		{
			if (locs.sphl_light_count >= 0) glUniform1i(locs.sphl_light_count, 0);
		}
	}


	void SimpleSceneGraph::ApplyMaterialToCurrentProgram(SimpleMaterial &mtl, bool useMaps)
	{
		GLuint unit = 1;

		if (useMaps)
		{
			if (!mtl.map_Ka.empty()) currentTextures["map_Ka"] = materials.GetTextureMap(mtl.map_Ka);
			if (!mtl.map_Kd.empty()) currentTextures["map_Kd"] = materials.GetTextureMap(mtl.map_Kd);
			if (!mtl.map_Ks.empty()) currentTextures["map_Ks"] = materials.GetTextureMap(mtl.map_Ks);
			if (!mtl.map_Ke.empty()) currentTextures["map_Ke"] = materials.GetTextureMap(mtl.map_Ke);
			if (!mtl.map_Ns.empty()) currentTextures["map_Ns"] = materials.GetTextureMap(mtl.map_Ns);
			if (!mtl.map_Ns.empty()) currentTextures["map_Ni"] = materials.GetTextureMap(mtl.map_Ni);
			if (!mtl.map_Tf.empty()) currentTextures["map_Tf"] = materials.GetTextureMap(mtl.map_Tf);
			if (!mtl.map_Tr.empty()) currentTextures["map_Tr"] = materials.GetTextureMap(mtl.map_Tr);
			if (!mtl.map_bump.empty()) currentTextures["map_bump"] = materials.GetTextureMap(mtl.map_bump);
			if (!mtl.map_normal.empty()) currentTextures["map_normal"] = materials.GetTextureMap(mtl.map_normal);
			if (!mtl.PBmap.empty()) currentTextures["PBmap"] = materials.GetTextureMap(mtl.PBmap);

			// Set mixes to 0 if the map is not there (should default to 0.0 if we don't change it when applying it)
			//if (mtl.map_Ka.empty()) glUniform1f(locs.map_Ka_mix, 0.0f);
			//if (mtl.map_Kd.empty()) glUniform1f(locs.map_Kd_mix, 0.0f);
			//if (mtl.map_Ks.empty()) glUniform1f(locs.map_Ks_mix, 0.0f);
			//if (mtl.map_Ke.empty()) glUniform1f(locs.map_Ke_mix, 0.0f);
			//if (mtl.map_Ni.empty()) glUniform1f(locs.map_Ni_mix, 0.0f);
			//if (mtl.map_Ns.empty()) glUniform1f(locs.map_Ns_mix, 0.0f);
			//if (mtl.map_Tf.empty()) glUniform1f(locs.map_Tf_mix, 0.0f);
			//if (mtl.map_Tr.empty()) glUniform1f(locs.map_Tr_mix, 0.0f);
			//if (mtl.map_bump.empty()) glUniform1f(locs.map_bump_mix, 0.0f);
			//if (mtl.map_normal.empty()) glUniform1f(locs.map_normal_mix, 0.0f);

			for (auto tmapIt = currentTextures.begin(); tmapIt != currentTextures.end(); tmapIt++)
			{
				SimpleMap *pMap = tmapIt->second;
				if (pMap) {
					if (pMap->unitId <= 0)
						pMap->unitId = GetTexUnit();
					pMap->samplerId = pMap->textureObject.samplerObject.GetId();
					pMap->textureId = pMap->textureObject.GetTextureId();
					pMap->textureObject.Bind(pMap->unitId, false);
					glTexParameterf(pMap->textureObject.GetTarget(), GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0);

					if (pMap->samplerId == 0)
					{
						glTexParameterf(pMap->textureObject.GetTarget(), GL_TEXTURE_WRAP_S, GL_REPEAT);
						glTexParameterf(pMap->textureObject.GetTarget(), GL_TEXTURE_WRAP_T, GL_REPEAT);
						glTexParameterf(pMap->textureObject.GetTarget(), GL_TEXTURE_WRAP_R, GL_REPEAT);
						glTexParameterf(pMap->textureObject.GetTarget(), GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
						glTexParameterf(pMap->textureObject.GetTarget(), GL_TEXTURE_MAG_FILTER, GL_LINEAR);
						
					}

					pMap->map_loc = -1;
					pMap->map_mix_loc = -1;
					if (tmapIt->first == "map_Ka") { pMap->map_loc = locs.map_Ka; pMap->map_mix_loc = locs.map_Ka_mix; }
					else if (tmapIt->first == "map_Kd") { pMap->map_loc = locs.map_Kd; pMap->map_mix_loc = locs.map_Kd_mix; }
					else if (tmapIt->first == "map_Ks") { pMap->map_loc = locs.map_Ks; pMap->map_mix_loc = locs.map_Ks_mix; }
					else if (tmapIt->first == "map_Ke") { pMap->map_loc = locs.map_Ke; pMap->map_mix_loc = locs.map_Ke_mix; }
					else if (tmapIt->first == "map_Ni") { pMap->map_loc = locs.map_Ni; pMap->map_mix_loc = locs.map_Ni_mix; }
					else if (tmapIt->first == "map_Ns") { pMap->map_loc = locs.map_Ns; pMap->map_mix_loc = locs.map_Ns_mix; }
					else if (tmapIt->first == "map_Tf") { pMap->map_loc = locs.map_Tf; pMap->map_mix_loc = locs.map_Tf_mix; }
					else if (tmapIt->first == "map_Tr") { pMap->map_loc = locs.map_Tr; pMap->map_mix_loc = locs.map_Tr_mix; }
					else if (tmapIt->first == "map_bump") { pMap->map_loc = locs.map_bump; pMap->map_mix_loc = locs.map_bump_mix; }
					else if (tmapIt->first == "map_normal") { pMap->map_loc = locs.map_normal; pMap->map_mix_loc = locs.map_normal_mix; }
					else if (tmapIt->first == "PBmap") { pMap->map_loc = locs.PBmap; pMap->map_mix_loc = locs.PBmap_mix; }

					if (pMap->unitId < 0 || pMap->unitId >= g_MaxCombinedTextureUnits)
					{
						hflog.error("%s(): pMap->unitId (%d) is out of range.", __FUNCTION__, pMap->unitId);
					}
					else
					{
						if (pMap->map_loc >= 0) glUniform1i(pMap->map_loc, pMap->unitId);
						if (pMap->map_mix_loc >= 0) glUniform1f(pMap->map_mix_loc, 1.0f);
					}

					unit++;
				}
			}
		}

		// Apply Material Uniforms to the program shader
		if (locs.Ka >= 0) glUniform3fv(locs.Ka, 1, mtl.Ka.v);
		if (locs.Kd >= 0) glUniform3fv(locs.Kd, 1, mtl.Kd.v);
		if (locs.Ks >= 0) glUniform3fv(locs.Ks, 1, mtl.Ks.v);
		if (locs.Ke >= 0) glUniform3fv(locs.Ke, 1, mtl.Ke.v);
		if (locs.Ni >= 0) glUniform1fv(locs.Ni, 1, &mtl.Ni);
		if (locs.Ns >= 0) glUniform1fv(locs.Ns, 1, &mtl.Ns);
		if (locs.Tf >= 0) glUniform3fv(locs.Tf, 1, mtl.Tf.v);
		if (locs.Tr >= 0) glUniform1fv(locs.Tr, 1, &mtl.Tr);
		if (locs.PBKdm >= 0) glUniform1fv(locs.PBKdm, 1, &mtl.PBKdm);
		if (locs.PBKsm >= 0) glUniform1fv(locs.PBKsm, 1, &mtl.PBKsm);
		if (locs.PBior >= 0) glUniform1fv(locs.PBior, 1, &mtl.PBior);
		if (locs.PBGGXgamma >= 0) glUniform1fv(locs.PBGGXgamma, 1, &mtl.PBGGXgamma);
		if (locs.PBm >= 0) glUniform1fv(locs.PBm, 1, &mtl.PBm);
		if (locs.PBk >= 0) glUniform1fv(locs.PBk, 1, &mtl.PBk);

		// The better way to do this is to first find all the locations for the material, then apply them...
		for (auto uniform : locs.newLocationList)
		{
			// uniform.first = mapname
			// uniform.second = details
		}
	}


	void SimpleSceneGraph::DisableCurrentTextures()
	{
		// Turn off textures and reset program unit bindings to 0
		for (auto tmapIt = currentTextures.begin(); tmapIt != currentTextures.end(); tmapIt++)
		{
			SimpleMap *pMap = tmapIt->second;
			if (pMap) {
				glutBindTexture(pMap->unitId, pMap->textureObject.GetTarget(), 0);

				glUniform1i(pMap->map_loc, 0);
				glUniform1f(pMap->map_mix_loc, 0.0f);

				if (pMap->unitId != 0)
				{
					FreeTexUnit(pMap->unitId);
				}
				
			}
		}
		glutSetActiveTexture(GL_TEXTURE0);

		currentTextures.clear();
	}



	//



	void __ShaderProgramLocations::GetMaterialProgramLocations(SimpleProgram &program)
	{
		locations.clear();
		locations["Ka"] = Ka = program.GetUniformLocation("Ka");
		locations["Kd"] = Kd = program.GetUniformLocation("Kd");
		locations["Ks"] = Ks = program.GetUniformLocation("Ks");
		locations["Ke"] = Ke = program.GetUniformLocation("Ke");
		locations["Tr"] = Tr = program.GetUniformLocation("Tr");
		locations["Tf"] = Tf = program.GetUniformLocation("Tf");
		locations["Ns"] = Ns = program.GetUniformLocation("Ns");
		locations["Ni"] = Ni = program.GetUniformLocation("Ni");

		locations["PBk"] = PBk = program.GetUniformLocation("PBk");
		locations["PBm"] = PBm = program.GetUniformLocation("PBm");
		locations["PBKdm"] = PBKdm = program.GetUniformLocation("PBKdm");
		locations["PBKsm"] = PBKsm = program.GetUniformLocation("PBKsm");
		locations["PBior"] = PBior = program.GetUniformLocation("PBior");
		locations["PBGGXgamma"] = PBGGXgamma = program.GetUniformLocation("PBGGXgamma");
		locations["PBmap"] = PBmap = program.GetUniformLocation("PBmap");
		/// 
		locations["map_Ka"] = map_Ka = program.GetUniformLocation("map_Ka");
		locations["map_Kd"] = map_Kd = program.GetUniformLocation("map_Kd");
		locations["map_Ks"] = map_Ks = program.GetUniformLocation("map_Ks");
		locations["map_Ke"] = map_Ke = program.GetUniformLocation("map_Ke");
		locations["map_Tr"] = map_Tr = program.GetUniformLocation("map_Tr");
		locations["map_Tf"] = map_Tf = program.GetUniformLocation("map_Tf");
		locations["map_Ni"] = map_Ni = program.GetUniformLocation("map_Ni");
		locations["map_Ns"] = map_Ns = program.GetUniformLocation("map_Ns");
		locations["map_bump"] = map_bump = program.GetUniformLocation("map_bump");
		locations["map_normal"] = map_normal = program.GetUniformLocation("map_normal");
		locations["map_Ka_mix"] = map_Ka_mix = program.GetUniformLocation("map_Ka_mix");
		locations["map_Kd_mix"] = map_Kd_mix = program.GetUniformLocation("map_Kd_mix");
		locations["map_Ks_mix"] = map_Ks_mix = program.GetUniformLocation("map_Ks_mix");
		locations["map_Ke_mix"] = map_Ke_mix = program.GetUniformLocation("map_Ke_mix");
		locations["map_Tf_mix"] = map_Tf_mix = program.GetUniformLocation("map_Tf_mix");
		locations["map_Tr_mix"] = map_Tr_mix = program.GetUniformLocation("map_Tr_mix");
		locations["map_Ni_mix"] = map_Ni_mix = program.GetUniformLocation("map_Ni_mix");
		locations["map_Ns_mix"] = map_Ns_mix = program.GetUniformLocation("map_Ns_mix");
		locations["map_bump_mix"] = map_bump_mix = program.GetUniformLocation("map_bump_mix");
		locations["map_normal_mix"] = map_normal_mix = program.GetUniformLocation("map_normal_mix");

		locations["Spheres"] = sphere_array = program.GetUniformLocation("Spheres[0]");
		locations["SpheresCount"] = sphere_count = program.GetUniformLocation("SpheresCount");
		locations["SpheresKe"] = sphere_Ke = program.GetUniformLocation("SpheresKe[0]");

		for (int i = 0; i < 16; i++)
		{
			char name[64];
			sprintf_s(name, "SphlLights[%d].Enabled", i);  locations[name] = sphl_lights_enabled[i] = program.GetUniformLocation(name);
			sprintf_s(name, "SphlLights[%d].E0", i);       locations[name] = sphl_lights_E0[i] = program.GetUniformLocation(name);
			sprintf_s(name, "SphlLights[%d].Position", i); locations[name] = sphl_lights_position[i] = program.GetUniformLocation(name);
			sprintf_s(name, "SphlLights[%d].LightProbeCubeMap", i); locations[name] = sphl_lights_lightProbeCubeMap[i] = program.GetUniformLocation(name);
			sprintf_s(name, "SphlLights[%d].EnvironmentCubeMap", i); locations[name] = sphl_lights_environmentCubeMap[i] = program.GetUniformLocation(name);
			sprintf_s(name, "SphlLights[%d].DepthShadowCubeMap", i); locations[name] = sphl_lights_depthShadowCubeMap[i] = program.GetUniformLocation(name);
			sprintf_s(name, "SphlLights[%d].DepthShadowZFar", i); locations[name] = sphl_lights_depthShadowZFar[i] = program.GetUniformLocation(name);
			for (int j = 0; j < 9; j++)
			{
				sphl_lights_sph[i][j] = -1;
				sprintf_s(name, "SphlLights[%d].sph[%d]", i, j); locations[name] = sphl_lights_sph[i][j] = program.GetUniformLocation(name);
			}
		}

		locations["ToneMapScale"] = toneMapScale = program.GetUniformLocation("ToneMapScale");
		locations["ToneMapExposure"] = toneMapExposure = program.GetUniformLocation("ToneMapExposure");
		locations["ToneMapGamma"] = toneMapGamma = program.GetUniformLocation("ToneMapGamma");
		locations["SunDirTo"] = sunDirTo = program.GetUniformLocation("SunDirTo");
		locations["SunColor"] = sunColor = program.GetUniformLocation("SunColor");
		locations["SunSize"] = sunSize = program.GetUniformLocation("SunSize");
		locations["SunShadowMap"] = sunShadowMap = program.GetUniformLocation("SunShadowMap");
		locations["SunColorMap"] = sunColorMap = program.GetUniformLocation("SunColorMap");
		locations["SunShadowMatrix"] = sunShadowMatrix = program.GetUniformLocation("SunShadowMatrix");
		locations["SunE0"] = sunE0 = program.GetUniformLocation("SunE0");
		locations["GroundE0"] = groundE0 = program.GetUniformLocation("GroundE0");
	
		locations["EnviroCubeMap"] = enviroCubeMap = program.GetUniformLocation("EnviroCubeMap");
		locations["EnviroCubeMapAmount"] = enviroCubeMapAmount = program.GetUniformLocation("EnviroCubeMapAmount");

		locations["PBSkyCubeMap"] = pbskyCubeMap = program.GetUniformLocation("PBSkyCubeMap");

		locations["ShaderDebugChoice"] = shaderDebugChoice = program.GetUniformLocation("ShaderDebugChoice");
		locations["ShaderDebugLight"] = shaderDebugLight = program.GetUniformLocation("ShaderDebugLight");
		locations["ShaderDebugSphl"] = shaderDebugSphl = program.GetUniformLocation("ShaderDebugSphl");

		// Do the new way by finding out what uniforms the program actually has, or is looking for...
		newLocationList.Clear();

		for (auto uniform : program.activeUniforms)
		{
			// uniform.first is the mapname of the uniform...
			// uniform.second has the details...
	
			newLocationList[uniform.first] = MakeSharedIntProperty(uniform.second.index);
		}
	}


	void SimpleSceneGraph::InitSphls()
	{
		ssphhLights.clear();
		ssphhLights.resize(MaxSphlLights);
		for (int i = 0; i < MaxSphlLights; i++)
		{
			ssphhLights[i].ChangeDegrees(MaxSphlDegree);
			ssphhLights[i].enabled = false;
		}
	}


	void SimpleSceneGraph::MakeSphlsUnclean()
	{
		for (int i = 0; i < ssphhLights.size(); i++)
		{
			ssphhLights[i].dirty = true;
			ssphhLights[i].depthSphlMap.dirty = true;
		}
	}


	void RenderCubeShadowMap(const SimpleSceneGraph & ssg, SimpleCubeTexture & scs, const SimpleRenderConfiguration &rc)
	{
		glutSetErrorMessage("fluxions_simple_scene_graph.cpp", __LINE__, __FUNCTION__);

		if (!scs.dirty)
			return;

		double t0 = hflog.getSecondsElapsed();

		scs.texture.CreateTextureShadowCube(rc.viewportRect.w);

		GLuint cubeShadowFbo = 0;
		glGenFramebuffers(1, &cubeShadowFbo);
		glBindFramebuffer(GL_FRAMEBUFFER, cubeShadowFbo);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, scs.texture.GetTexture(), 0);
		if (rc.fbo_gen_color)
		{
			glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, rc.fbo_color_map, 0);
		}
		else
		{
			glDrawBuffer(GL_NONE);
			glReadBuffer(GL_NONE);
		}
		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		string msg = GetFramebufferStatusAsString(status);
		bool fboComplete;

		if (status != GL_FRAMEBUFFER_COMPLETE)
			fboComplete = false;
		else
			fboComplete = true;

		if (!fboComplete)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			hflog.error("%s(): Framebuffer not complete!", __FUNCTION__);
			scs.buildTime = -1.0;
			scs.dirty = false;
			return;
		}

		ssg.AdvancedRenderZOnly(rc);

		// remove texture from fbo
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, 0, 0);
		if (rc.fbo_gen_color)
		{
			glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, 0, 0);
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDeleteFramebuffers(1, &cubeShadowFbo);

		double dt = hflog.getSecondsElapsed() - t0;
		scs.buildTime = dt * 1000.0;
		scs.dirty = false;
	}
} // namespace Fluxions
