// SSPHH/Fluxions/Unicornfish/Viperfish/Hatchetfish/Sunfish/Damselfish/GLUT Extensions
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
#include "pch.h"
#include <fluxions_simple_geometry_mesh.hpp>

template class std::vector<Fluxions::SimpleGeometryMesh::AttribInfo>;
template class std::vector<Fluxions::SimpleGeometryMesh::Index>;
template class std::vector<Fluxions::SimpleGeometryMesh::Vertex>;
template class std::vector<Fluxions::SimpleGeometryMesh::Surface>;

namespace Fluxions
{
void SimpleGeometryMesh::Reset()
{
	vertices.clear();
	surfaces.clear();
	indices.clear();
	dirty = true;
	//vector<AttribInfo> ai = {
	attribInfo = {
		{0, "aPosition", false, true},
		{1, "aNormal", false, true},
		{2, "aColor", false, true},
		{3, "aTexCoord", false, true},
		{4, "aGenericAttribute1", false, false},
		{5, "aGenericAttribute2", false, false},
		{6, "aGenericAttribute3", false, false},
		{7, "aGenericAttribute4", false, false}};

	//attribInfo = ai;
}

void SimpleGeometryMesh::BeginSurface(SurfaceType type)
{
	Surface newSurface;
	newSurface.type = type;
	newSurface.first = (Index)indices.size();
	newSurface.count = 0;
	newSurface.materialName = curMaterialName;
	surfaces.push_back(newSurface);
}

bool SimpleGeometryMesh::SaveOBJ(const std::string &path, bool output_normals, bool output_texcoords)
{
	std::ofstream objout(path + ".obj");
	std::ofstream mtlout(path + ".mtl");

	objout << "mtllib " << path << ".mtl";

	for (auto &v : vertices)
	{
		objout << "v " << v.attribs[0].x << " " << v.attribs[0].y << " " << v.attribs[0].z << std::endl;
	}
	objout << std::endl;

	if (output_normals)
	{
		for (auto &v : vertices)
		{
			objout << "v " << v.attribs[1].x << " " << v.attribs[1].y << " " << v.attribs[1].z << std::endl;
		}
		objout << std::endl;
	}

	if (output_texcoords)
	{
		for (auto &v : vertices)
		{
			objout << "v " << v.attribs[3].x << " " << v.attribs[3].y << " " << v.attribs[3].z << std::endl;
		}
		objout << std::endl;
	}

	// Write out each surface as a new object in the OBJ file

	int surfacenum = 0;
	for (auto &surface : surfaces)
	{
		objout << "g surface" << surfacenum << std::endl;
		objout << "s 1" << std::endl;
		if (!surface.materialName.empty())
		{
			objout << "usemtl " << surface.materialName << std::endl;
			mtlout << "newmtl " << surface.materialName << std::endl;
		}
		else
		{
			objout << "usemtl surface" << surfacenum << std::endl;
			mtlout << "newmtl surface" << surfacenum << std::endl;
		}
		objout << std::endl;

		mtlout << "\tKd   1.0 1.0 1.0" << std::endl;
		mtlout << "\tKs   1.0 1.0 1.0" << std::endl;
		mtlout << "\tKa   0.2 0.2 0.2" << std::endl;
		mtlout << "\tNs   250.0" << std::endl;
		mtlout << std::endl;

		if (surface.type == GL_TRIANGLES)
		{
			for (int i = surface.first; i < surface.first + surface.count; i += 3)
			{
				int i1 = indices[i];
				int i2 = indices[i + 1];
				int i3 = indices[i + 2];

				objout << "f ";
				if (output_texcoords && output_normals)
				{
					objout << i1 << "/" << i1 << "/" << i1 << "  ";
					objout << i2 << "/" << i2 << "/" << i2 << "  ";
					objout << i3 << "/" << i3 << "/" << i3 << std::endl;
				}
				else if (output_normals)
				{
					objout << i1 << "//" << i1 << "  ";
					objout << i2 << "//" << i2 << "  ";
					objout << i3 << "//" << i3 << std::endl;
				}
				else if (output_texcoords)
				{
					objout << i1 << "/" << i1 << "  ";
					objout << i2 << "/" << i2 << "  ";
					objout << i3 << "/" << i3 << std::endl;
				}
				else
				{
					objout << i1 << "  " << i2 << "  " << i3 << std::endl;
				}
			}
			objout << std::endl;
		}
		surfacenum++;
	}

	objout.close();
	mtlout.close();

	return true;
}
} // namespace Fluxions