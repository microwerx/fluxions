// SSPHH/Fluxions/Unicornfish/Viperfish/Hatchetfish/Sunfish/Damselfish/GLUT Extensions
// Copyright (C) 2017-2019 Jonathan Metzgar
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
#include "pch.hpp"
#include <fstream>
#include <fluxions_simple_renderer.hpp>
#include <fluxions_simple_materials.hpp>

namespace Fluxions
{

	// explicit template instantiation is after the implementation

	template <typename IndexType, GLenum GLIndexType>
	SimpleRenderer<IndexType, GLIndexType>::SimpleRenderer() {
		memset(&currentFastVertex, 0, sizeof(SimpleFastVertex));
		memset(&currentSlowVertex, 0, sizeof(SimpleSlowVertex));
		memset(&bufferInfo, 0, sizeof(BUFFERINFO));
	}

	template <typename IndexType, GLenum GLIndexType>
	SimpleRenderer<IndexType, GLIndexType>::~SimpleRenderer() {
		Reset();
	}

	template <typename IndexType, GLenum GLIndexType>
	void SimpleRenderer<IndexType, GLIndexType>::Begin(GLenum mode, bool isIndexed) {
		// Starts a new surface
		if (isMakingSurface) {
			End();
		}

		isMakingSurface = true;
		if (surfaces.empty() || surfaces[currentSurface].count > 0) {
			surfaces.push_back(SimpleSurface());
		}
		currentSurface = (int)surfaces.size() - 1;

		if (surfaces[currentSurface].vertexType == VertexType::UNDECIDED) {
			surfaces[currentSurface].vertexType = lastVertexType;
		}

		surfaces[currentSurface].mode = mode;
		surfaces[currentSurface].isIndexed = isIndexed;
		surfaces[currentSurface].programId = currentProgramId;
		surfaces[currentSurface].first = 0;
		surfaces[currentSurface].firstIndex = (int)Indices.size();
		surfaces[currentSurface].firstZIndex = (int)zIndices.size();

		// OBJECT, GROUP, MTLLIB, MTL
		surfaces[currentSurface].objectName = currentObjectName;
		surfaces[currentSurface].groupName = currentGroupName;
		surfaces[currentSurface].mtllibName = currentMtlLibName;
		surfaces[currentSurface].mtlName = currentMtlName;

		surfaces[currentSurface].objectId = currentObjectId;
		surfaces[currentSurface].groupId = currentGroupId;
		surfaces[currentSurface].mtllibId = currentMtlLibId;
		surfaces[currentSurface].mtlId = currentMtlId;
	}

	template <typename IndexType, GLenum GLIndexType>
	void SimpleRenderer<IndexType, GLIndexType>::End() {
		// End a surface
		if (isMakingSurface) {
			isMakingSurface = false;
		}
	}

	template <typename IndexType, GLenum GLIndexType>
	void SimpleRenderer<IndexType, GLIndexType>::NewObject() {
		// this creates a break in the vertex buffer
		if (isMakingSurface)
			return;

		baseZIndex = (IndexType)zVertices.size();
		baseFastIndex = (IndexType)fastVertices.size();
		baseSlowIndex = (IndexType)slowVertices.size();
	}

	template <typename IndexType, GLenum GLIndexType>
	void SimpleRenderer<IndexType, GLIndexType>::ApplyIdToObjectNames(const std::string& objectName, GLuint id) {
		for (auto surface = surfaces.begin(); surface != surfaces.end(); surface++) {
			if (surface->objectName == objectName) {
				surface->objectId = id;
			}
		}
	}

	template <typename IndexType, GLenum GLIndexType>
	void SimpleRenderer<IndexType, GLIndexType>::ApplyIdToGroupNames(const std::string& groupName, GLuint id) {
		for (auto surface = surfaces.begin(); surface != surfaces.end(); surface++) {
			if (surface->groupName == groupName) {
				surface->groupId = id;
			}
		}
	}

	template <typename IndexType, GLenum GLIndexType>
	void SimpleRenderer<IndexType, GLIndexType>::ApplyIdToMtlLibNames(const std::string& mtllibName, GLuint id) {
		for (auto surface = surfaces.begin(); surface != surfaces.end(); surface++) {
			if (surface->mtllibName == mtllibName) {
				surface->mtllibId = id;
			}
		}
	}

	template <typename IndexType, GLenum GLIndexType>
	void SimpleRenderer<IndexType, GLIndexType>::ApplyIdToMtlNames(const std::string& mtlName, GLuint id) {
		for (auto surface = surfaces.begin(); surface != surfaces.end(); surface++) {
			if (surface->mtlName == mtlName) {
				surface->mtlId = id;
			}
		}
	}

	template <typename IndexType, GLenum GLIndexType>
	void SimpleRenderer<IndexType, GLIndexType>::AssignUniqueGroupIds() {
		GLuint groupId = 0;
		std::map<std::string, GLuint> groups;
		for (auto surface = surfaces.begin(); surface != surfaces.end(); surface++) {
			std::string autoGenName = surface->objectName + " " + surface->mtllibName + " " + surface->mtlName;
			auto it = groups.find(autoGenName);
			if (it == groups.end()) {
				groupId++;
				groups[autoGenName] = groupId;
				surface->groupId = groupId;
			}
			else {
				surface->groupId = it->second;
			}
		}
	}

	template <typename IndexType, GLenum GLIndexType>
	void SimpleRenderer<IndexType, GLIndexType>::AssignMaterialIds(SimpleMaterialSystem& materials) {
		for (auto surface = surfaces.begin(); surface != surfaces.end(); surface++) {
			materials.SetLibraryMaterial(surface->mtllibName, surface->mtlName);
			surface->mtllibId = materials.GetLibraryId();
			surface->mtlId = materials.GetMaterialId();
		}
	}

	template <typename IndexType, GLenum GLIndexType>
	void SimpleRenderer<IndexType, GLIndexType>::Index(IndexType index) {
		if (!isMakingSurface)
			return;

		GLsizei maxIndex = 0;
		switch (GLIndexType) {
		case GL_BYTE:
			maxIndex = 127;
			break;
		case GL_UNSIGNED_BYTE:
			maxIndex = 255;
			break;
		case GL_SHORT:
			maxIndex = 32767;
			break;
		case GL_UNSIGNED_SHORT:
			maxIndex = 65535;
			break;
		case GL_INT:
			maxIndex = 2147483647;
			break;
		case GL_UNSIGNED_INT:
			maxIndex = 4294967295;
			break;
		}

		if (index < (IndexType)0 || index >= (IndexType)maxIndex)
			return;

		// Handle the case if this is the first index
		if (!surfaces[currentSurface].isIndexed && surfaces[currentSurface].count == 0) {
			Begin(surfaces[currentSurface].mode, true);
		}

		if (surfaces[currentSurface].vertexType == VertexType::FAST_VERTEX) {
			Indices.push_back(baseFastIndex + index);
			surfaces[currentSurface].count++;
		}
		else if (surfaces[currentSurface].vertexType == VertexType::SLOW_VERTEX) {
			Indices.push_back(baseSlowIndex + index);
			surfaces[currentSurface].count++;
		}

		// Either way, emit a Z vertex
		zIndices.push_back(baseZIndex + index);
		surfaces[currentSurface].zCount++;
	}

	template <typename IndexType, GLenum GLIndexType>
	void SimpleRenderer<IndexType, GLIndexType>::Index(std::vector<IndexType> indices) {
		// a baseIndex of < 0 means to use the current surface first vertex as 0
		for_each(indices.begin(), indices.end(), [&](IndexType i) {
			Index(i);
			});
	}

	template <typename IndexType, GLenum GLIndexType>
	void SimpleRenderer<IndexType, GLIndexType>::HandleVertexTypeChange(VertexType vertexType) {
		if (!isMakingSurface) {
			return;
		}
		if (lastVertexType != vertexType) {
			// reset all the offsets because they're going to be completely different.
			baseZIndex = (IndexType)zVertices.size();
			baseFastIndex = (IndexType)fastVertices.size();
			baseSlowIndex = (IndexType)slowVertices.size();
		}
		if (surfaces[currentSurface].vertexType != vertexType &&
			surfaces[currentSurface].count == 0) {
			// change the surface vertex mode if we've not emitted any vertices
			surfaces[currentSurface].vertexType = vertexType;
			lastVertexType = vertexType;
		}
		else if (surfaces[currentSurface].vertexType != vertexType) {
			// force a change in surfaces (by creating a new one)
			End();

			Begin(surfaces[currentSurface].mode);
			surfaces[currentSurface].vertexType = vertexType;
			lastVertexType = vertexType;
		}
	}

	template <typename IndexType, GLenum GLIndexType>
	void SimpleRenderer<IndexType, GLIndexType>::EmitVertex() {
		if (!isMakingSurface)
			return;

		if (surfaces[currentSurface].vertexType == VertexType::FAST_VERTEX) {
			fastVertices.push_back(currentFastVertex);
			ZVertex(currentFastVertex.position[0], currentFastVertex.position[1], currentFastVertex.position[2]);
		}
		else if (surfaces[currentSurface].vertexType == VertexType::SLOW_VERTEX) {
			slowVertices.push_back(currentSlowVertex);
			ZVertex(currentSlowVertex.attrib[0][0], currentSlowVertex.attrib[0][1], currentSlowVertex.attrib[0][2]);
		}

		if (!surfaces[currentSurface].isIndexed)
			surfaces[currentSurface].count++;
	}

	template <typename IndexType, GLenum GLIndexType>
	void SimpleRenderer<IndexType, GLIndexType>::ZVertex(GLfloat x, GLfloat y, GLfloat z) {
		if (!isMakingSurface)
			return;

		zVertices.push_back(SimpleZVertex(x, y, z));
		if (!surfaces[currentSurface].isIndexed)
			surfaces[currentSurface].zCount++;
	}

	template <typename IndexType, GLenum GLIndexType>
	void SimpleRenderer<IndexType, GLIndexType>::VertexAttrib4f(GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w) {
		if (index < 0 && index >= 8)
			return;
		HandleVertexTypeChange(VertexType::SLOW_VERTEX);

		currentSlowVertex.attrib[index][0] = x;
		currentSlowVertex.attrib[index][1] = y;
		currentSlowVertex.attrib[index][2] = z;
		currentSlowVertex.attrib[index][3] = w;

		if (index == 0) {
			EmitVertex();
		}
	}

	template <typename IndexType, GLenum GLIndexType>
	void SimpleRenderer<IndexType, GLIndexType>::Vertex4s(GLshort x, GLshort y, GLshort z, GLshort w) {
		HandleVertexTypeChange(VertexType::FAST_VERTEX);

		currentFastVertex.position[0] = x;
		currentFastVertex.position[1] = y;
		currentFastVertex.position[2] = z;
		currentFastVertex.position[3] = w;

		EmitVertex();
	}

	template <typename IndexType, GLenum GLIndexType>
	void SimpleRenderer<IndexType, GLIndexType>::Normal4s(GLshort x, GLshort y, GLshort z, GLshort w) {
		currentFastVertex.normal[0] = x;
		currentFastVertex.normal[1] = y;
		currentFastVertex.normal[2] = z;
		currentFastVertex.normal[3] = w;
	}

	template <typename IndexType, GLenum GLIndexType>
	void SimpleRenderer<IndexType, GLIndexType>::TexCoord2s(GLshort s, GLshort t) {
		currentFastVertex.texCoord[0] = s;
		currentFastVertex.texCoord[1] = t;
	}

	template <typename IndexType, GLenum GLIndexType>
	void SimpleRenderer<IndexType, GLIndexType>::Color4ub(GLubyte r, GLubyte g, GLubyte b, GLubyte a) {
		currentFastVertex.color[0] = r;
		currentFastVertex.color[1] = g;
		currentFastVertex.color[2] = b;
		currentFastVertex.color[3] = a;
	}

	template <typename IndexType, GLenum GLIndexType>
	void SimpleRenderer<IndexType, GLIndexType>::Attrib4s(GLshort x, GLshort y, GLshort z, GLshort w) {
		currentFastVertex.attrib[0] = x;
		currentFastVertex.attrib[1] = y;
		currentFastVertex.attrib[2] = z;
		currentFastVertex.attrib[3] = w;
	}

	template <typename IndexType, GLenum GLIndexType>
	void SimpleRenderer<IndexType, GLIndexType>::BuildMemoryBuffers() {
		// determine size of memory buffers
		bufferInfo.zVertexOffset = 0;
		bufferInfo.zVertexSize = (int)zVertices.size() * sizeof(SimpleZVertex);
		bufferInfo.fastVertexOffset = bufferInfo.zVertexOffset + bufferInfo.zVertexSize;
		bufferInfo.fastVertexSize = (int)fastVertices.size() * sizeof(SimpleFastVertex);
		bufferInfo.slowVertexOffset = bufferInfo.fastVertexOffset + bufferInfo.fastVertexSize;
		bufferInfo.slowVertexSize = (int)slowVertices.size() * sizeof(SimpleSlowVertex);

		bufferInfo.zIndexOffset = 0;
		bufferInfo.zIndexSize = (int)zIndices.size() * sizeof(IndexType);
		bufferInfo.IndexOffset = (int)bufferInfo.zIndexOffset + bufferInfo.zIndexSize;
		bufferInfo.IndexSize = (int)Indices.size() * sizeof(IndexType);

		bufferInfo.vertexBufferSizeInBytes = bufferInfo.zVertexSize + bufferInfo.fastVertexSize + bufferInfo.slowVertexSize;
		bufferInfo.indexBufferSizeInBytes = bufferInfo.zIndexSize + bufferInfo.IndexSize;

		vertexMemoryBuffer.resize(bufferInfo.vertexBufferSizeInBytes + 1);
		indexMemoryBuffer.resize(bufferInfo.indexBufferSizeInBytes + 1);

		if (!zVertices.empty())
			memcpy(&vertexMemoryBuffer[bufferInfo.zVertexOffset], &zVertices[0], bufferInfo.zVertexSize);
		if (!fastVertices.empty())
			memcpy(&vertexMemoryBuffer[bufferInfo.fastVertexOffset], &fastVertices[0], bufferInfo.fastVertexSize);
		if (!slowVertices.empty())
			memcpy(&vertexMemoryBuffer[bufferInfo.slowVertexOffset], &slowVertices[0], bufferInfo.slowVertexSize);
		if (!zIndices.empty())
			memcpy(&indexMemoryBuffer[bufferInfo.zIndexOffset], &zIndices[0], bufferInfo.zIndexSize);
		if (!Indices.empty())
			memcpy(&indexMemoryBuffer[bufferInfo.IndexOffset], &Indices[0], bufferInfo.IndexSize);
	}

	template <typename IndexType, GLenum GLIndexType>
	void SimpleRenderer<IndexType, GLIndexType>::BuildBuffers() {
		// Have we already built the buffers?
		if (arrayBuffer && elementArrayBuffer)
			return;

		if (!arrayBuffer) {
			glGenBuffers(1, &arrayBuffer);
		}

		if (!elementArrayBuffer) {
			glGenBuffers(1, &elementArrayBuffer);
		}

		BuildMemoryBuffers();

		glBindBuffer(GL_ARRAY_BUFFER, arrayBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementArrayBuffer);

		if (!vertexMemoryBuffer.empty())
			glBufferData(GL_ARRAY_BUFFER, vertexMemoryBuffer.size(), &vertexMemoryBuffer[0], GL_STATIC_DRAW);
		if (!indexMemoryBuffer.empty())
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexMemoryBuffer.size(), &indexMemoryBuffer[0], GL_STATIC_DRAW);

		if (zVAO)
			glDeleteVertexArrays(1, &zVAO);
		glGenVertexArrays(1, &zVAO);
		glBindVertexArray(zVAO);
		glBindBuffer(GL_ARRAY_BUFFER, arrayBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementArrayBuffer);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glBindVertexArray(0);

		if (fastVAO)
			glDeleteVertexArrays(1, &fastVAO);
		glGenVertexArrays(1, &fastVAO);
		glBindVertexArray(fastVAO);
		glBindBuffer(GL_ARRAY_BUFFER, arrayBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementArrayBuffer);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(0, 4, GL_SHORT, GL_FALSE, sizeof(SimpleFastVertex), (GLvoid*)(long long)(bufferInfo.fastVertexOffset));
		glVertexAttribPointer(1, 4, GL_SHORT, GL_FALSE, sizeof(SimpleFastVertex), (GLvoid*)(long long)((bufferInfo.fastVertexOffset + 8)));
		glVertexAttribPointer(2, 2, GL_SHORT, GL_FALSE, sizeof(SimpleFastVertex), (GLvoid*)(long long)((bufferInfo.fastVertexOffset + 16)));
		glVertexAttribPointer(3, 4, GL_UNSIGNED_BYTE, GL_FALSE, sizeof(SimpleFastVertex), (GLvoid*)(long long)((bufferInfo.fastVertexOffset + 20)));
		glVertexAttribPointer(4, 4, GL_SHORT, GL_FALSE, sizeof(SimpleFastVertex), (GLvoid*)(long long)((bufferInfo.fastVertexOffset + 24)));

		if (slowVAO) {
			glDeleteVertexArrays(1, &slowVAO);
			slowVAO = 0;
		}

		glGenVertexArrays(1, &slowVAO);
		glBindVertexArray(slowVAO);
		glBindBuffer(GL_ARRAY_BUFFER, arrayBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementArrayBuffer);
		for (int i = 0; i < 8; i++) {
			glEnableVertexAttribArray(i);
			glVertexAttribPointer(i, 4, GL_FLOAT, GL_FALSE, sizeof(SimpleSlowVertex), (GLvoid*)(long long)(bufferInfo.slowVertexOffset + i * 16));
		}

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		for (auto surface = surfaces.begin(); surface != surfaces.end(); surface++) {
			if (surface->vertexType == VertexType::UNDECIDED)
				continue;
			if (surface->isIndexed) {
				surface->baseIndexBufferOffset = bufferInfo.IndexOffset + surface->firstIndex * sizeof(IndexType);
				surface->baseZIndexBufferOffset = surface->firstZIndex * sizeof(IndexType);
				//surface->baseIndexBufferOffset = bufferInfo.IndexOffset;
				//surface->baseZIndexBufferOffset = bufferInfo.zIndexOffset;
			}
		}
	}

	template <typename IndexType, GLenum GLIndexType>
	void SimpleRenderer<IndexType, GLIndexType>::Reset() {
		vertexCount = 0;
		triangleCount = 0;
		if (arrayBuffer) {
			glDeleteBuffers(1, &arrayBuffer);
			arrayBuffer = 0;
		}
		if (elementArrayBuffer) {
			glDeleteBuffers(1, &elementArrayBuffer);
			elementArrayBuffer = 0;
		}
		if (zVAO) {
			glDeleteVertexArrays(1, &zVAO);
			zVAO = 0;
		}
		if (fastVAO) {
			glDeleteVertexArrays(1, &fastVAO);
			fastVAO = 0;
		}
		if (slowVAO) {
			glDeleteVertexArrays(1, &slowVAO);
			slowVAO = 0;
		}
		slowVertices.clear();
		fastVertices.clear();
		zVertices.clear();
		Indices.clear();
		zIndices.clear();

		vertexMemoryBuffer.clear();
		indexMemoryBuffer.clear();
		surfaces.clear();
		currentSurface = 0;

		memset(&currentSlowVertex, 0, sizeof(SimpleSlowVertex));
		memset(&currentFastVertex, 0, sizeof(SimpleFastVertex));

		baseFastIndex = 0;
		baseSlowIndex = 0;
		baseZIndex = 0;

		isMakingSurface = false;

		currentGroupId = 0;
		currentObjectId = 0;
		currentProgramId = 0;

		for (auto& p : programs) {
			if (p.second.use_count() > 1)
			{
				HFLOGWARN("Program count not 1: %i", p.second->GetProgram());
			}
			p.second.reset();
		}
		programs.clear();
	}

	template <typename IndexType, GLenum GLIndexType>
	void SimpleRenderer<IndexType, GLIndexType>::BindBuffers() {
		if (!arrayBuffer || !elementArrayBuffer) {
			BuildBuffers();
		}

		glBindBuffer(GL_ARRAY_BUFFER, arrayBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementArrayBuffer);
	}

	template <typename IndexType, GLenum GLIndexType>
	void SimpleRenderer<IndexType, GLIndexType>::Render() {
		// Render all surfaces as slow/fast vertex types with appropriate programs.

		if (!arrayBuffer || !elementArrayBuffer) {
			BuildBuffers();
		}

		glBindVertexArray(fastVAO);

		for (auto surface = surfaces.begin(); surface != surfaces.end(); surface++) {
			if (surface->vertexType != VertexType::FAST_VERTEX)
				continue;

			if (surface->isIndexed) {
				glDrawElements(surface->mode, surface->count, GLIndexType, (GLvoid*)(long long)(surface->baseIndexBufferOffset));
			}
			else {
				glDrawArrays(surface->mode, surface->first, surface->count);
			}
		}

		glBindVertexArray(slowVAO);

		for (auto surface = surfaces.begin(); surface != surfaces.end(); surface++) {
			if (surface->vertexType != VertexType::SLOW_VERTEX)
				continue;

			if (surface->isIndexed) {
				glDrawElements(surface->mode, surface->count, GLIndexType, (GLvoid*)(long long)(surface->baseIndexBufferOffset));
			}
			else {
				glDrawArrays(surface->mode, surface->first, surface->count);
			}
		}

		glBindVertexArray(0);
	}

	//template <typename IndexType, GLenum GLIndexType>
	//void SimpleRenderer<IndexType, GLIndexType>::RenderIf(const std::string &objname, const std::string &mtllib, const std::string &mtl, bool onlyRenderZ)
	//{
	//	// Render all surfaces as slow/fast vertex types with appropriate programs.
	//	if (debugging) std::cout << "SimpleRenderer::RenderIf() -- obj: " << objname << " mtllib: " << mtllib << " mtl: " << mtl << std::endl;
	//	if (!arrayBuffer || !elementArrayBuffer)
	//	{
	//		BuildBuffers();
	//	}

	//	glBindVertexArray(fastVAO);

	//	for (auto surface = surfaces.begin(); surface != surfaces.end(); surface++)
	//	{
	//		if (surface->vertexType != VertexType::FAST_VERTEX)
	//			continue;
	//		if (!objname.empty() && surface->objname != objname)
	//			continue;
	//		if (!mtllib.empty() && surface->mtllib != mtllib)
	//			continue;
	//		if (!mtl.empty() && surface->mtl != mtl)
	//			continue;

	//		if (debugging) std::cout << "SimpleRenderer::RenderIf() -- FAST: obj: " << objname << " mtllib: " << mtllib << " mtl: " << mtl << " -- rendering" << std::endl;
	//		if (debugging) std::cout << "SimpleRenderer::RenderIf() -- FAST: first: " << surface->first << " , count: " << surface->count << ", baseIndexOffset: " << surface->baseIndexBufferOffset << std::endl;

	//		if (surface->isIndexed)
	//		{
	//			glDrawElements(surface->mode, surface->count, GLIndexType, (GLvoid*)surface->baseIndexBufferOffset);
	//		}
	//		else
	//		{
	//			glDrawArrays(surface->mode, surface->first, surface->count);
	//		}
	//	}

	//	glBindVertexArray(slowVAO);

	//	for (auto surface = surfaces.begin(); surface != surfaces.end(); surface++)
	//	{
	//		if (surface->vertexType != VertexType::SLOW_VERTEX)
	//			continue;
	//		if (!objname.empty() && surface->objname != objname)
	//			continue;
	//		if (!mtllib.empty() && surface->mtllib != mtllib)
	//			continue;
	//		if (!mtl.empty() && surface->mtl != mtl)
	//			continue;
	//
	//		if (debugging) std::cout << "SimpleRenderer::RenderIf() -- SLOW: obj: " << objname << " mtllib: " << mtllib << " mtl: " << mtl << " -- rendering" << std::endl;
	//		if (debugging) std::cout << "SimpleRenderer::RenderIf() -- SLOW: first: " << surface->first << " , count: " << surface->count << ", baseIndexOffset: " << surface->baseIndexBufferOffset << std::endl;

	//		if (surface->isIndexed)
	//		{
	//			glDrawElements(surface->mode, surface->count, GLIndexType, (GLvoid*)surface->baseIndexBufferOffset);
	//		}
	//		else
	//		{
	//			glDrawArrays(surface->mode, surface->first, surface->count);
	//		}
	//	}

	//	glBindVertexArray(0);

	//}

	template <typename IndexType, GLenum GLIndexType>
	void SimpleRenderer<IndexType, GLIndexType>::RenderZOnly() {

		// Render all surfaces as Z only with appropriate transformation matrices.
		if (!zVAO || !arrayBuffer || !elementArrayBuffer) {
			BuildBuffers();
		}

		glBindVertexArray(zVAO);

		for (auto surface = surfaces.begin(); surface != surfaces.end(); surface++) {
			if (surface->vertexType == VertexType::UNDECIDED)
				continue;
			if (surface->isIndexed) {
				glDrawElements(surface->mode, surface->count, GLIndexType, (GLvoid*)(long long)(surface->baseZIndexBufferOffset));
			}
			else {
				glDrawArrays(surface->mode, surface->first, surface->count);
			}
		}

		glBindVertexArray(0);
	}

	template <typename IndexType, GLenum GLIndexType>
	void SimpleRenderer<IndexType, GLIndexType>::RenderIf(const std::string& objectName, const std::string& groupName, const std::string& mtllibName, const std::string& mtlName, bool onlyRenderZ) {
		BuildBuffers();

		GLuint lastUsedVAO = 0;

		if (onlyRenderZ) {
			glBindVertexArray(zVAO);
		}

		for (auto surface = surfaces.begin(); surface != surfaces.end(); surface++) {
			if (surface->vertexType == VertexType::UNDECIDED)
				continue;
			if (objectName.empty() != true && objectName != surface->objectName)
				continue;
			if (groupName.empty() != true && groupName != surface->groupName)
				continue;
			if (mtllibName.empty() != true && mtllibName != surface->mtllibName)
				continue;
			if (mtlName.empty() != true && mtlName != surface->mtlName)
				continue;

			GLintptr offset = 0;

			// TODO: Check if it is faster to move these out of the loop and render one VAO at a time...
			if (onlyRenderZ) {
				offset = surface->baseZIndexBufferOffset;
				if (lastUsedVAO != zVAO) {
					lastUsedVAO = zVAO;
					glBindVertexArray(zVAO);
				}
			}
			else if (surface->vertexType == VertexType::FAST_VERTEX) {
				offset = surface->baseIndexBufferOffset;
				if (lastUsedVAO != fastVAO) {
					lastUsedVAO = fastVAO;
					glBindVertexArray(fastVAO);
				}
			}
			else if (surface->vertexType == VertexType::SLOW_VERTEX) {
				offset = surface->baseIndexBufferOffset;
				if (lastUsedVAO != slowVAO) {
					lastUsedVAO = slowVAO;
					glBindVertexArray(slowVAO);
				}
			}

			if (surface->isIndexed) {
				glDrawElements(surface->mode, surface->count, GLIndexType, (GLvoid*)(long long)(surface->baseZIndexBufferOffset));
			}
			else {
				glDrawArrays(surface->mode, surface->first, surface->count);
			}
		}

		glBindVertexArray(0);
	}

	template <typename IndexType, GLenum GLIndexType>
	void SimpleRenderer<IndexType, GLIndexType>::RenderIf(GLuint objectId, GLuint groupId, GLuint mtllibId, GLuint mtlId, bool onlyRenderZ) {
		BuildBuffers();

		GLuint lastUsedVAO = 0;

		if (onlyRenderZ) {
			glBindVertexArray(zVAO);
		}

		for (auto surface = surfaces.begin(); surface != surfaces.end(); surface++) {
			if (surface->vertexType == VertexType::UNDECIDED)
				continue;
			if (objectId != 0 && objectId != surface->objectId)
				continue;
			if (groupId != 0 && groupId != surface->groupId)
				continue;
			if (mtllibId != 0 && mtllibId != surface->mtllibId)
				continue;
			if (mtlId != 0 && mtlId != surface->mtlId)
				continue;

			GLintptr offset = 0;

			// TODO: Check if it is faster to move these out of the loop and render one VAO at a time...
			if (onlyRenderZ) {
				offset = surface->baseZIndexBufferOffset;
				if (lastUsedVAO != zVAO) {
					lastUsedVAO = zVAO;
					glBindVertexArray(zVAO);
				}
			}
			else if (surface->vertexType == VertexType::SLOW_VERTEX) {
				offset = surface->baseIndexBufferOffset;
				if (lastUsedVAO != slowVAO) {
					lastUsedVAO = slowVAO;
					glBindVertexArray(slowVAO);
				}
			}
			else if (surface->vertexType == VertexType::FAST_VERTEX) {
				offset = surface->baseIndexBufferOffset;
				if (lastUsedVAO != fastVAO) {
					lastUsedVAO = fastVAO;
					glBindVertexArray(fastVAO);
				}
			}

			if (surface->isIndexed) {
				glDrawElements(surface->mode, surface->count, GLIndexType, (GLvoid*)offset);
			}
			else {
				glDrawArrays(surface->mode, surface->first, surface->count);
			}
		}

		glBindVertexArray(0);
	}

	// explicit template instantiation is after the implementation
	template class SimpleRenderer<GLbyte, GL_BYTE>;
	template class SimpleRenderer<GLubyte, GL_UNSIGNED_BYTE>;
	template class SimpleRenderer<GLshort, GL_SHORT>;
	template class SimpleRenderer<GLushort, GL_UNSIGNED_SHORT>;
	template class SimpleRenderer<GLint, GL_INT>;
	template class SimpleRenderer<GLuint, GL_UNSIGNED_INT>;
} // namespace Fluxions
