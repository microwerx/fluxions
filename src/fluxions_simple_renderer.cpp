#include "fluxions_pch.hpp"
#include <fluxions_simple_renderer.hpp>

namespace Fluxions {
	// explicit template instantiation is after the implementation

	template <typename IndexType, GLenum GLIndexType>
	SimpleRenderer<IndexType, GLIndexType>::SimpleRenderer() {
		memset(&currentFastVertex, 0, sizeof(SimpleFastVertex));
		memset(&currentSlowVertex, 0, sizeof(SimpleSlowVertex));
		memset(&bufferInfo, 0, sizeof(BUFFERINFO));
	}

	template <typename IndexType, GLenum GLIndexType>
	SimpleRenderer<IndexType, GLIndexType>::~SimpleRenderer() {
		reset(false);
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
		switch (surfaces[currentSurface].vertexType) {
		case VertexType::FAST_VERTEX:
			surfaces[currentSurface].first = (int)fastVertices.size();
			break;
		case VertexType::SLOW_VERTEX:
			surfaces[currentSurface].first = (int)slowVertices.size();
			break;
		}
		surfaces[currentSurface].firstIndex = (int)indices.size();
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
		object_set.insert(currentObjectId);

		// this creates a break in the vertex buffer
		if (isMakingSurface)
			return;

		baseZIndex = (IndexType)zVertices.size();
		baseFastIndex = (IndexType)fastVertices.size();
		baseSlowIndex = (IndexType)slowVertices.size();
	}

	template <typename IndexType, GLenum GLIndexType>
	void SimpleRenderer<IndexType, GLIndexType>::DrawOBJ(const SimpleGeometryMesh& obj) {
		int curIndex;

		vertexCount += (int)obj.Vertices.size();

		Begin(GL_TRIANGLES, true);
		curIndex = 0;
		for (const auto& vertex : obj.Vertices) {
			VertexAttrib4f(1, vertex.normal.x, vertex.normal.y, vertex.normal.z, 1.0f);
			VertexAttrib4f(2, vertex.texcoord.x, vertex.texcoord.y, 0.0f, 1.0f);
			VertexAttrib4f(0, vertex.position.x, vertex.position.y, vertex.position.z, 1.0f);
			curIndex++;
		}
		End();

		for (const auto& surface : obj.Surfaces) {
			triangleCount += surface.count / 3;
			SetCurrentMtlName(surface.materialName);
			SetCurrentMtlId(surface.materialId);
			Begin(GL_TRIANGLES, true);
			for (unsigned i = surface.first; i < surface.first + surface.count; i++) {
				Index((IndexType)obj.Indices[i]);
			}
			End();
			surfaces.back().drawMtlId = surface.materialId;
		}
		SetCurrentMtlName("");
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

	//template <typename IndexType, GLenum GLIndexType>
	//void SimpleRenderer<IndexType, GLIndexType>::AssignMaterialIds(SimpleMaterialSystem& materials) {
	//	for (auto surface = surfaces.begin(); surface != surfaces.end(); surface++) {
	//		materials.SetLibraryMaterial(surface->mtllibName, surface->mtlName);
	//		surface->mtllibId = materials.GetLibraryId();
	//		surface->mtlId = materials.GetMaterialId();
	//	}
	//}

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
			indices.push_back(baseFastIndex + index);
			surfaces[currentSurface].count++;
		}
		else if (surfaces[currentSurface].vertexType == VertexType::SLOW_VERTEX) {
			indices.push_back(baseSlowIndex + index);
			surfaces[currentSurface].count++;
		}

		// Either way, emit a Z vertex
		zIndices.push_back(baseZIndex + index);
		surfaces[currentSurface].zCount++;
	}

	template <typename IndexType, GLenum GLIndexType>
	void SimpleRenderer<IndexType, GLIndexType>::Index(std::vector<IndexType> _indices) {
		// a baseIndex of < 0 means to use the current surface first vertex as 0
		for_each(_indices.begin(), _indices.end(), [&](IndexType i) {
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
		bufferInfo.indexOffset = (int)bufferInfo.zIndexOffset + bufferInfo.zIndexSize;
		bufferInfo.indexSize = (int)indices.size() * sizeof(IndexType);

		bufferInfo.vertexBufferSizeInBytes = bufferInfo.zVertexSize + bufferInfo.fastVertexSize + bufferInfo.slowVertexSize;
		bufferInfo.indexBufferSizeInBytes = bufferInfo.zIndexSize + bufferInfo.indexSize;

		vertexMemoryBuffer.resize((size_t)bufferInfo.vertexBufferSizeInBytes + 1);
		indexMemoryBuffer.resize((size_t)bufferInfo.indexBufferSizeInBytes + 1);

		if (!zVertices.empty())
			memcpy(&vertexMemoryBuffer[bufferInfo.zVertexOffset], &zVertices[0], bufferInfo.zVertexSize);
		if (!fastVertices.empty())
			memcpy(&vertexMemoryBuffer[bufferInfo.fastVertexOffset], &fastVertices[0], bufferInfo.fastVertexSize);
		if (!slowVertices.empty())
			memcpy(&vertexMemoryBuffer[bufferInfo.slowVertexOffset], &slowVertices[0], bufferInfo.slowVertexSize);
		if (!zIndices.empty())
			memcpy(&indexMemoryBuffer[bufferInfo.zIndexOffset], &zIndices[0], bufferInfo.zIndexSize);
		if (!indices.empty())
			memcpy(&indexMemoryBuffer[bufferInfo.indexOffset], &indices[0], bufferInfo.indexSize);
	}

	template <typename IndexType, GLenum GLIndexType>
	bool SimpleRenderer<IndexType, GLIndexType>::BuildBuffers() {
		// Have we already built the buffers?
		if (abo && eabo)
			return true;

		BuildMemoryBuffers();

		FxCreateBuffer(GL_ARRAY_BUFFER, &abo, vertexMemoryBuffer.size(), &vertexMemoryBuffer[0], GL_STATIC_DRAW);
		FxCreateBuffer(GL_ELEMENT_ARRAY_BUFFER, &eabo, indexMemoryBuffer.size(), &indexMemoryBuffer[0], GL_STATIC_DRAW);

		FxCreateVertexArray(&zVAO);
		glBindBuffer(GL_ARRAY_BUFFER, abo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eabo);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glBindVertexArray(0);

		FxCreateVertexArray(&fastVAO);
		glBindBuffer(GL_ARRAY_BUFFER, abo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eabo);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(0, 4, GL_SHORT, GL_FALSE, sizeof(SimpleFastVertex), (GLvoid*)(bufferInfo.fastVertexOffset));
		glVertexAttribPointer(1, 4, GL_SHORT, GL_FALSE, sizeof(SimpleFastVertex), (GLvoid*)(bufferInfo.fastVertexOffset + 8));
		glVertexAttribPointer(2, 2, GL_SHORT, GL_FALSE, sizeof(SimpleFastVertex), (GLvoid*)(bufferInfo.fastVertexOffset + 16));
		glVertexAttribPointer(3, 4, GL_UNSIGNED_BYTE, GL_FALSE, sizeof(SimpleFastVertex), (GLvoid*)(bufferInfo.fastVertexOffset + 20));
		glVertexAttribPointer(4, 4, GL_SHORT, GL_FALSE, sizeof(SimpleFastVertex), (GLvoid*)(bufferInfo.fastVertexOffset + 24));

		FxCreateVertexArray(&slowVAO);
		glBindBuffer(GL_ARRAY_BUFFER, abo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eabo);
		for (int i = 0; i < 8; i++) {
			glEnableVertexAttribArray(i);
			GLsizeiptr offset = bufferInfo.slowVertexOffset + i * 16;
			glVertexAttribPointer(i, 4, GL_FLOAT, GL_FALSE, sizeof(SimpleSlowVertex), (GLvoid*)offset);
		}

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		for (auto surface = surfaces.begin(); surface != surfaces.end(); surface++) {
			if (surface->vertexType == VertexType::UNDECIDED)
				continue;
			if (surface->isIndexed) {
				surface->baseIndexBufferOffset = bufferInfo.indexOffset + surface->firstIndex * sizeof(IndexType);
				surface->baseZIndexBufferOffset = surface->firstZIndex * sizeof(IndexType);
			}
		}

		return (abo && eabo);
	}

	template <typename IndexType, GLenum GLIndexType>
	void SimpleRenderer<IndexType, GLIndexType>::reset(bool softReset) {
		vertexCount = 0;
		triangleCount = 0;
		if (!softReset) {
			FxDeleteBuffer(&abo);
			FxDeleteBuffer(&eabo);
			FxDeleteVertexArray(&zVAO);
			FxDeleteVertexArray(&fastVAO);
			FxDeleteVertexArray(&slowVAO);
		}
		slowVertices.clear();
		fastVertices.clear();
		zVertices.clear();
		indices.clear();
		zIndices.clear();
		if (softReset) {
			slowVertices.reserve(1000000);
			indices.reserve(1000000);
		}

		vertexMemoryBuffer.clear();
		indexMemoryBuffer.clear();
		surfaces.clear();
		object_set.clear();
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
	}

	template <typename IndexType, GLenum GLIndexType>
	void SimpleRenderer<IndexType, GLIndexType>::BindBuffers() {
		if (!BuildBuffers())
			return;

		glBindBuffer(GL_ARRAY_BUFFER, abo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eabo);
	}

	template <typename IndexType, GLenum GLIndexType>
	void SimpleRenderer<IndexType, GLIndexType>::Render() {
		RenderFast();
		RenderSlow();
	}

	template <typename IndexType, GLenum GLIndexType>
	void SimpleRenderer<IndexType, GLIndexType>::RenderFast() {
		if (!BuildBuffers())
			return;

		glBindVertexArray(fastVAO);

		for (auto& surface : surfaces) {
			if (surface.vertexType != VertexType::FAST_VERTEX)
				continue;

			if (surface.isIndexed) {
				glDrawElements(surface.mode, surface.count, GLIndexType, (GLvoid*)(surface.baseIndexBufferOffset));
			}
			else {
				glDrawArrays(surface.mode, surface.first, surface.count);
			}
		}

		glBindVertexArray(0);
	}

	template <typename IndexType, GLenum GLIndexType>
	void SimpleRenderer<IndexType, GLIndexType>::RenderSlow() {
		if (!BuildBuffers())
			return;

		glBindVertexArray(slowVAO);

		for (auto& surface : surfaces) {
			if (surface.vertexType != VertexType::SLOW_VERTEX)
				continue;

			if (surface.isIndexed) {
				glDrawElements(surface.mode, surface.count, GLIndexType, (GLvoid*)(surface.baseIndexBufferOffset));
			}
			else {
				glDrawArrays(surface.mode, surface.first, surface.count);
			}
		}

		glBindVertexArray(0);
	}

	//template <typename IndexType, GLenum GLIndexType>
	//void SimpleRenderer<IndexType, GLIndexType>::RenderIf(const std::string &objname, const std::string &mtllib, const std::string &mtl, bool onlyRenderZ)
	//{
	//	// Render all surfaces as slow/fast vertex types with appropriate programs.
	//	if (debugging) std::cout << "SimpleRenderer::RenderIf() -- obj: " << objname << " mtllib: " << mtllib << " mtl: " << mtl << std::endl;
	//	if (!abo || !eabo)
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
		if (!zVAO || !abo || !eabo) {
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

	template <typename IndexType, GLenum GLIndexType>
	int SimpleRenderer<IndexType, GLIndexType>::RenderIf(GLuint objectId, int mtlId, bool onlyRenderZ) {
		if (objectId == 0)
			return 0;
		if (!BuildBuffers())
			return 0;

		GLuint lastUsedVAO = 0;

		if (onlyRenderZ) {
			glBindVertexArray(zVAO);
		}

		int count = 0;
		for (auto& surface : surfaces) {
			if (surface.vertexType == VertexType::UNDECIDED)
				continue;
			if (objectId != 0 && objectId != surface.objectId)
				continue;
			if (mtlId != surface.drawMtlId)
				continue;

			GLintptr offset = 0;

			// TODO: Check if it is faster to move these out of the loop and render one VAO at a time...
			if (onlyRenderZ) {
				offset = surface.baseZIndexBufferOffset;
				if (lastUsedVAO != zVAO) {
					lastUsedVAO = zVAO;
					glBindVertexArray(zVAO);
				}
			}
			else if (surface.vertexType == VertexType::SLOW_VERTEX) {
				offset = surface.baseIndexBufferOffset;
				if (lastUsedVAO != slowVAO) {
					lastUsedVAO = slowVAO;
					glBindVertexArray(slowVAO);
				}
			}
			else if (surface.vertexType == VertexType::FAST_VERTEX) {
				offset = surface.baseIndexBufferOffset;
				if (lastUsedVAO != fastVAO) {
					lastUsedVAO = fastVAO;
					glBindVertexArray(fastVAO);
				}
			}

			if (surface.isIndexed) {
				glDrawElements(surface.mode, surface.count, GLIndexType, (GLvoid*)offset);
			}
			else {
				glDrawArrays(surface.mode, surface.first, surface.count);
			}
			count++;
		}

		glBindVertexArray(0);
		return count;
	}

	// explicit template instantiation is after the implementation
	template class SimpleRenderer<GLbyte, GL_BYTE>;
	template class SimpleRenderer<GLubyte, GL_UNSIGNED_BYTE>;
	template class SimpleRenderer<GLshort, GL_SHORT>;
	template class SimpleRenderer<GLushort, GL_UNSIGNED_SHORT>;
	template class SimpleRenderer<GLint, GL_INT>;
	template class SimpleRenderer<GLuint, GL_UNSIGNED_INT>;
} // namespace Fluxions
