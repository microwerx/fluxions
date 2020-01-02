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
#include <fluxions_stdcxx.hpp>
#include <fluxions_simple_geometry_mesh.hpp>
#include <unicornfish_corona_database.hpp>

template class std::vector<Fluxions::SimpleGeometryMesh::Vertex>;
//template class std::vector<unsigned>;
template class std::vector<Fluxions::SimpleGeometryMesh::Surface>;
template class std::map<std::string, Fluxions::SimpleGeometryMesh>;

namespace Uf
{

	CoronaDatabase::CoronaDatabase() {}

	CoronaDatabase::~CoronaDatabase() {}

	void CoronaDatabase::loadOBJ(const std::string& objname, const std::string& filename) {
		models[objname].loadOBJ(filename);
	}

	//void CoronaDatabase::BuildBuffers() {
	//	renderer.reset();
	//	std::map<std::string, Fluxions::SimpleGeometryMesh>::iterator modelIt;
	//	for (modelIt = models.begin(); modelIt != models.end(); modelIt++) {
	//		modelIt->second.Render(renderer);
	//	}
	//	renderer.BuildBuffers();
	//}

	//void CoronaDatabase::Render() {
	//	renderer.Render();
	//}

	//void CoronaDatabase::RenderZOnly() {
	//	renderer.RenderZOnly();
	//}

} // namespace Uf
