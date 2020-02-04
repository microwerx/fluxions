#include "fluxions_ssg_pch.hpp"
#include <fluxions_base.hpp>
#include <fluxions_fileio_iostream.hpp>
#include <fluxions_ssg_materials.hpp>

namespace Fluxions {
	//SimpleMaterialSystem::SimpleMaterialSystem() {}

	//SimpleMaterialSystem::~SimpleMaterialSystem() {}

	//// ------- P R I V A T E   M E M B E R S -------

	//void SimpleMaterialSystem::SynchronizeIds() {
	//	// Check to see if the current material library name is valid
	//	if (currentMtlLibName.empty()) {
	//		currentMtlLibId = 0;
	//	}
	//	else {
	//		currentMtlLibId = mtllibs.getHandleFromName(currentMtlLibName);
	//		if (currentMtlLibId == 0)
	//			currentMtlLibName.clear();
	//	}

	//	// Check to see if we're using a current material library
	//	if (currentMtlLibId == 0) {
	//		currentMtlLibPtr = nullptr;
	//		currentMtlPtr = nullptr;
	//		currentMtlId = 0;
	//		currentMtlName.clear();
	//	}
	//	else {
	//		currentMtlLibPtr = &(mtllibs[currentMtlLibId]);

	//		// Check to see if we're using a valid material
	//		if (currentMtlName.empty()) {
	//			currentMtlId = 0;
	//			currentMtlPtr = nullptr;
	//		}
	//		else {
	//			currentMtlId = currentMtlLibPtr->mtls.getHandleFromName(currentMtlName);
	//			currentMtlPtr = &(currentMtlLibPtr->mtls[currentMtlId]);
	//		}
	//	}
	//}

	//// ------- P U B L I C   M E M B E R S -------

	//void SimpleMaterialSystem::Clear() {
	//	mtllibs.clear();
	//	shaderMaps.clear();
	//	currentMtlId = 0;
	//	currentMtlLibId = 0;
	//	currentMtlName.clear();
	//	currentMtlLibName.clear();
	//	currentMtlLibPtr = nullptr;
	//	currentMtlPtr = nullptr;
	//}

	//bool SimpleMaterialSystem::Save(const std::string& path) {
	//	for (auto it = mtllibs.begin(); it != mtllibs.end(); it++) {
	//		SimpleMaterialLibrary* mtllib = &(it->second);

	//		std::string outputfile = path + "/" + mtllib->fpi.fname;

	//		Save(mtllib->name, outputfile);
	//	}

	//	return true;
	//}

	//bool SimpleMaterialSystem::Load(const std::string& mtllibName, const std::string& filename) {
	//	FilePathInfo mtllibfpi(filename);
	//	SimpleMaterialLibrary* pmtllib = CreateLibrary(mtllibName);
	//	if (!pmtllib) {
	//		return false;
	//	}
	//	SetLibrary(mtllibName);

	//	pmtllib->fpi.Set(filename);
	//	pmtllib->name = mtllibName;

	//	std::vector<std::string> lines;
	//	ReadLines(filename, lines);

	//	pmtllib->maps.clear();
	//	pmtllib->mtls.clear();

	//	TResourceManager<SimpleMaterial>& mtls = pmtllib->mtls;
	//	//TResourceManager<SimpleMap>& maps = pmtllib->maps;
	//	SimpleMaterial* curmtl = nullptr;

	//	for (auto& line: lines) {
	//		std::string str;
	//		std::stringstream istr(line);

	//		istr >> str;
	//		if (str == "newmap") {
	//			std::string name = ReadString(istr);
	//			std::string shader = ReadString(istr);
	//			AddMapShader(name, shader);
	//			HFLOGINFO("newmap %s", shader.c_str());
	//		}
	//		else if (str == "newmtl") {
	//			std::string name = ReadString(istr);
	//			CreateMaterial(name);
	//			SetMaterialDefaults(mtls[name]);
	//			curmtl = currentMtlPtr;
	//			HFLOGINFO("newmtl %s", name.c_str());
	//		}
	//		else if (curmtl == nullptr) {
	//		} // do nothing from now on because we're null
	//		else if (str.empty()) {
	//		} // do nothing if there is an empty string
	//		else if (str.size() >= 3 && str[0] == '#' && str[1] != 'P' && str[2] != 'B') {
	//		} // do nothing because it is a comment, not a #PB...
	//		else if (str.size() < 3 && str[0] == '#') {
	//		} // do nothing because it's a 2 character short comment
	//		else if (str == "Kd") {
	//			auto Kd = curmtl->Kd = ReadColor3f(istr);
	//			curmtl->Properties["Kd"] = MakeSharedCol3Property(Kd);
	//		}
	//		else if (str == "Ks") {
	//			auto Ks = curmtl->Ks = ReadColor3f(istr);
	//			curmtl->Properties["Ks"] = MakeSharedCol3Property(Ks);
	//		}
	//		else if (str == "Ke") {
	//			auto Ke = curmtl->Ke = ReadColor3f(istr);
	//			curmtl->Properties["Ke"] = MakeSharedCol3Property(Ke);
	//		}
	//		else if (str == "Ka") {
	//			auto Ka = curmtl->Ka = ReadColor3f(istr);
	//			curmtl->Properties["Ka"] = MakeSharedCol3Property(Ka);
	//		}
	//		else if (str == "Tf") {
	//			auto Tf = curmtl->Tf = ReadColor3f(istr);
	//			curmtl->Properties["Tf"] = MakeSharedCol3Property(Tf);
	//		}
	//		else if (str == "Tr") {
	//			auto Tr = curmtl->Tr = (float)ReadDouble(istr);
	//			curmtl->Properties["Tr"] = MakeSharedFloatProperty(Tr);
	//		}
	//		else if (str == "#PBm") {
	//			auto PBm = curmtl->PBm = (float)ReadDouble(istr);
	//			curmtl->Properties["PBm"] = MakeSharedFloatProperty(PBm);
	//		}
	//		else if (str == "#PBk") {
	//			auto PBk = curmtl->PBk = (float)ReadDouble(istr);
	//			curmtl->Properties["PBk"] = MakeSharedFloatProperty(PBk);
	//		}
	//		else if (str == "#PBKdm") {
	//			auto PBKdm = curmtl->PBKdm = (float)ReadDouble(istr);
	//			curmtl->Properties["PBKdm"] = MakeSharedFloatProperty(PBKdm);
	//		}
	//		else if (str == "#PBKsm") {
	//			auto PBKsm = curmtl->PBKsm = (float)ReadDouble(istr);
	//			curmtl->Properties["PBKsm"] = MakeSharedFloatProperty(PBKsm);
	//		}
	//		else if (str == "#PBior") {
	//			auto PBior = curmtl->PBior = (float)ReadDouble(istr);
	//			curmtl->Properties["PBior"] = MakeSharedFloatProperty(PBior);
	//		}
	//		else if (str == "#PBmap") {
	//			auto PBmap = curmtl->PBmap = ReadString(istr);
	//			curmtl->Properties["PBmap"] = MakeSharedStringProperty(PBmap);
	//			AddMap(mtllibfpi.dir, curmtl->PBmap);
	//		}
	//		else if (str == "#PBGGXgamma") {
	//			auto PBGGXgamma = curmtl->PBGGXgamma = ReadFloat(istr);
	//			curmtl->Properties["PBGGXgamma"] = MakeSharedFloatProperty(PBGGXgamma);
	//		}
	//		else if (str == "Refract") {
	//			auto Refract = curmtl->Refract = ReadVector3f(istr);
	//			curmtl->Properties["Refract"] = MakeSharedCol3Property(Refract);
	//		}
	//		else if (str == "Opacity") {
	//			auto Opacity = curmtl->Opacity = ReadVector3f(istr);
	//			curmtl->Properties["Opacity"] = MakeSharedCol3Property(Opacity);
	//		}
	//		else if (str == "Ns") {
	//			// overrides the PBKsm factor
	//			auto Ns = curmtl->Ns = (float)ReadDouble(istr);
	//			curmtl->Properties["Ns"] = MakeSharedFloatProperty(Ns);
	//			auto PBKsm = curmtl->PBKsm = -sqrt(2.0f / (Ns + 2.0f));
	//			curmtl->Properties["PBKsm"] = MakeSharedFloatProperty(PBKsm);
	//		}
	//		else if (str == "Ni") {
	//			auto Ni = curmtl->Ni = (float)ReadDouble(istr);
	//			curmtl->Properties["Ni"] = MakeSharedFloatProperty(Ni);
	//		}
	//		else if (str == "EmissionGlossiness") {
	//			auto EmissionGlossiness = curmtl->EmissionGlossiness = (float)ReadDouble(istr);
	//			curmtl->Properties["EmissionGlossiness"] = MakeSharedFloatProperty(EmissionGlossiness);
	//		}
	//		else if (str == "ReflectFresnel") {
	//			auto ReflectFresnel = curmtl->ReflectFresnel = (float)ReadDouble(istr);
	//			curmtl->Properties["ReflectFresnel"] = MakeSharedFloatProperty(ReflectFresnel);
	//		}
	//		else if (str == "RefractGlossiness") {
	//			auto RefractGlossiness = curmtl->RefractGlossiness = (float)ReadDouble(istr);
	//			curmtl->Properties["RefractGlossiness"] = MakeSharedFloatProperty(RefractGlossiness);
	//		}
	//		else if (str == "Anisotropy") {
	//			auto AnisotropyAmount = curmtl->AnisotropyAmount = (float)ReadDouble(istr);
	//			curmtl->Properties["AnisotropyAmount"] = MakeSharedFloatProperty(AnisotropyAmount);
	//			auto AnisotropyRotation = curmtl->AnisotropyRotation = (float)ReadDouble(istr);
	//			curmtl->Properties["AnisotropyRotation"] = MakeSharedFloatProperty(AnisotropyRotation);
	//		}
	//		else if (str == "MeanCosine") {
	//			auto MeanCosine = curmtl->MeanCosine = (float)ReadDouble(istr);
	//			curmtl->Properties["MeanCosine"] = MakeSharedFloatProperty(MeanCosine);
	//		}
	//		//else if (str == "RoundedCorners") {
	//		//	auto HasRoundedCorners = curmtl->HasRoundedCorners = true;
	//		//	auto RoundedCorners = curmtl->RoundedCorners = (float)ReadDouble(istr);
	//		//	curmtl->Properties["RoundedCorners"] = MakeSharedFloatProperty(RoundedCorners);
	//		//}
	//		//else if (str == "Portal") {
	//		//	auto IsPortal = curmtl->IsPortal = true;
	//		//}
	//		//else if (str == "IesProfile") {
	//		//	auto HasIesProfile = curmtl->HasIesProfile = true;
	//		//	auto IesProfileFilename = curmtl->IesProfileFilename = ReadString(istr);
	//		//	curmtl->Properties["IesProfileFilename"] = MakeSharedStringProperty(IesProfileFilename);
	//		//	auto IesProfileMatrix = curmtl->IesProfileMatrix = ReadMatrix4f(istr);
	//		//	curmtl->Properties["IesProfileMatrix"] = MakeSharedMat4Property(IesProfileMatrix);
	//		//	auto IesProfileKeepSharp = curmtl->IesProfileKeepSharp = ReadBool(istr);
	//		//	curmtl->Properties["IesProfileKeepSharp"] = MakeSharedBoolProperty(IesProfileKeepSharp);
	//		//}
	//		else if (str == "RefractMode") {
	//			curmtl->RefractMode.clear();
	//			while (istr) {
	//				str = "";
	//				istr >> str;
	//				if (istr)
	//					curmtl->RefractMode.push_back(str);
	//			}
	//		}
	//		else if (str == "Invisible") {
	//			curmtl->Invisible.clear();
	//			while (istr) {
	//				str = "";
	//				istr >> str;
	//				if (istr)
	//					curmtl->Invisible.push_back(str);
	//			}
	//		}
	//		else if (str == "map_Ka") {
	//			auto map_Ka = curmtl->map_Ka = ReadString(istr);
	//			curmtl->Properties["map_Ka"] = MakeSharedStringProperty(map_Ka);
	//			AddMap(mtllibfpi.dir, curmtl->map_Ka);
	//		}
	//		else if (str == "map_Kd") {
	//			auto map_Kd = curmtl->map_Kd = ReadString(istr);
	//			curmtl->Properties["map_Kd"] = MakeSharedStringProperty(map_Kd);
	//			AddMap(mtllibfpi.dir, curmtl->map_Kd);
	//		}
	//		else if (str == "map_Ks") {
	//			auto map_Ks = curmtl->map_Ks = ReadString(istr);
	//			curmtl->Properties["map_Ks"] = MakeSharedStringProperty(map_Ks);
	//			AddMap(mtllibfpi.dir, curmtl->map_Ks);
	//		}
	//		else if (str == "map_Ke") {
	//			auto map_Ke = curmtl->map_Ke = ReadString(istr);
	//			curmtl->Properties["map_Ke"] = MakeSharedStringProperty(map_Ke);
	//			AddMap(mtllibfpi.dir, curmtl->map_Ke);
	//		}
	//		else if (str == "map_d") {
	//			auto map_d = curmtl->map_d = ReadString(istr);
	//			curmtl->Properties["map_d"] = MakeSharedStringProperty(map_d);
	//			AddMap(mtllibfpi.dir, curmtl->map_d);
	//		}
	//		else if (str == "map_opacity") {
	//			auto map_opacity = curmtl->map_opacity = ReadString(istr);
	//			curmtl->Properties["map_opacity"] = MakeSharedStringProperty(map_opacity);
	//			AddMap(mtllibfpi.dir, curmtl->map_opacity);
	//		}
	//		else if (str == "map_Ns") {
	//			auto map_Ns = curmtl->map_Ns = ReadString(istr);
	//			curmtl->Properties["map_Ns"] = MakeSharedStringProperty(map_Ns);
	//			AddMap(mtllibfpi.dir, curmtl->map_Ns);
	//		}
	//		else if (str == "map_Tr") {
	//			auto map_Tr = curmtl->map_Tr = ReadString(istr);
	//			curmtl->Properties["map_Tr"] = MakeSharedStringProperty(map_Tr);
	//			AddMap(mtllibfpi.dir, curmtl->map_Tr);
	//		}
	//		else if (str == "map_Tf") {
	//			auto map_Tf = curmtl->map_Tf = ReadString(istr);
	//			curmtl->Properties["map_Tf"] = MakeSharedStringProperty(map_Tf);
	//			AddMap(mtllibfpi.dir, curmtl->map_Tf);
	//		}
	//		else if (str == "map_refract") {
	//			auto map_refract = curmtl->map_refract = ReadString(istr);
	//			curmtl->Properties["map_refract"] = MakeSharedStringProperty(map_refract);
	//			AddMap(mtllibfpi.dir, curmtl->map_refract);
	//		}
	//		else if (str == "map_anisotropy") {
	//			auto map_anisotropy = curmtl->map_anisotropy = ReadString(istr);
	//			curmtl->Properties["map_anisotropy"] = MakeSharedStringProperty(map_anisotropy);
	//			AddMap(mtllibfpi.dir, curmtl->map_anisotropy);
	//		}
	//		else if (str == "map_anisorotation") {
	//			auto map_anisorotation = curmtl->map_anisorotation = ReadString(istr);
	//			curmtl->Properties["map_anisorotation"] = MakeSharedStringProperty(map_anisorotation);
	//			AddMap(mtllibfpi.dir, curmtl->map_anisorotation);
	//		}
	//		else if (str == "map_scatteringAlbedo") {
	//			auto map_scatteringAlbedo = curmtl->map_scatteringAlbedo = ReadString(istr);
	//			curmtl->Properties["map_scatteringAlbedo"] = MakeSharedStringProperty(map_scatteringAlbedo);
	//			AddMap(mtllibfpi.dir, curmtl->map_scatteringAlbedo);
	//		}
	//		else if (str == "map_attenuation") {
	//			auto map_attenuation = curmtl->map_attenuation = ReadString(istr);
	//			curmtl->Properties["map_attenuation"] = MakeSharedStringProperty(map_attenuation);
	//			AddMap(mtllibfpi.dir, curmtl->map_attenuation);
	//		}
	//		else if (str == "map_normal") {
	//			auto map_normal = curmtl->map_normal = ReadString(istr);
	//			curmtl->Properties["map_normal"] = MakeSharedStringProperty(map_normal);
	//			AddMap(mtllibfpi.dir, curmtl->map_normal);
	//		}
	//		else if (str == "map_norm") {
	//			auto map_normal = curmtl->map_normal = ReadString(istr);
	//			curmtl->Properties["map_normal"] = MakeSharedStringProperty(map_normal);
	//			AddMap(mtllibfpi.dir, curmtl->map_normal);
	//		}
	//		else if (str == "norm") {
	//			auto map_normal = curmtl->map_normal = ReadString(istr);
	//			curmtl->Properties["map_normal"] = MakeSharedStringProperty(map_normal);
	//			AddMap(mtllibfpi.dir, curmtl->map_normal);
	//		}
	//		else if (str == "map_bump") {
	//			auto map_bump = curmtl->map_bump = ReadString(istr);
	//			curmtl->Properties["map_bump"] = MakeSharedStringProperty(map_bump);
	//			AddMap(mtllibfpi.dir, curmtl->map_bump);
	//		}
	//		else if (!str.empty()) {
	//			HFLOGWARN("property %s not officially supported", str.c_str());
	//		}
	//	}

	//	return true;
	//}

	//bool SimpleMaterialSystem::Save(const std::string& mtllibName, const std::string& filename) {
	//	FilePathInfo fpi(filename);

	//	if (!mtllibs.isAHandle(mtllibName))
	//		return false;

	//	for (auto it = mtllibs.begin(); it != mtllibs.end(); it++) {
	//		SetLibrary(it->second.name);
	//		if (currentMtlLibPtr == nullptr)
	//			continue;
	//		SimpleMaterialLibrary* pmtllib = currentMtlLibPtr;

	//		std::ofstream fout;
	//		fout.open(filename.c_str(), std::ios::out);
	//		if (!fout)
	//			continue;
	//		for (auto map = pmtllib->maps.begin(); map != pmtllib->maps.end(); map++) {
	//			if (map->second.pathname.empty()) {
	//				fout << "newmap " << map->first << " ";
	//				WriteString(fout, map->second.mapName);
	//				fout << std::endl;
	//			}
	//		}
	//		for (auto mtl = pmtllib->mtls.begin(); mtl != pmtllib->mtls.end(); mtl++) {
	//			fout << "newmtl " << mtl->first << std::endl;
	//			WriteMaterial(fout, mtl->second);
	//			fout << std::endl;
	//		}
	//		fout.close();
	//	}

	//	return true;
	//}

	//bool SimpleMaterialSystem::AddMap(const std::string& path, const std::string& filename) {
	//	FilePathInfo fpi(filename);

	//	if (!currentMtlLibPtr) {
	//		return false;
	//	}

	//	if (currentMtlLibPtr->maps.isAHandle(fpi.fname))
	//		return true;

	//	GLuint id = currentMtlLibPtr->maps.create(filename);
	//	currentMtlLibPtr->maps[id].mapId = id;
	//	currentMtlLibPtr->maps[id].mapName = filename;
	//	if (path.back() != '/' && TestIfFileExists(path + "/" + filename))
	//		currentMtlLibPtr->maps[id].pathname = path + "/" + filename;
	//	else if (TestIfFileExists(path + filename))
	//		currentMtlLibPtr->maps[id].pathname = path + filename;
	//	else if (TestIfFileExists(filename))
	//		currentMtlLibPtr->maps[id].pathname = path + filename;
	//	currentMtlLibPtr->maps[id].cached.textureId = 0;
	//	currentMtlLibPtr->maps[id].cached.samplerId = 0;

	//	return true;
	//}

	//bool SimpleMaterialSystem::AddMapShader(const std::string& name, const std::string& shader) {
	//	if (!currentMtlLibPtr) {
	//		return false;
	//	}

	//	if (currentMtlLibPtr->maps.isAHandle(name))
	//		return true;

	//	GLuint id = currentMtlLibPtr->maps.create(name);
	//	currentMtlLibPtr->maps[id].mapId = id;
	//	currentMtlLibPtr->maps[id].mapName = name;
	//	currentMtlLibPtr->maps[id].pathname.clear();
	//	currentMtlLibPtr->maps[id].cached.textureId = 0;
	//	currentMtlLibPtr->maps[id].cached.samplerId = 0;
	//	currentMtlLibPtr->maps[id].shader = shader;

	//	return true;
	//}

	//SimpleMaterialLibrary* SimpleMaterialSystem::CreateLibrary(const std::string& name) {
	//	mtllibs.create(name);
	//	return SetLibrary(name);
	//}

	//SimpleMaterial* SimpleMaterialSystem::CreateMaterial(const std::string& name) {
	//	if (currentMtlLibPtr) {
	//		currentMtlLibPtr->mtls.create(name);
	//	}
	//	return SetMaterial(name);
	//}

	//void SimpleMaterialSystem::DeleteLibrary(const std::string& name) {
	//	mtllibs.erase(name);
	//	SynchronizeIds();
	//}

	//void SimpleMaterialSystem::DeleteMaterial(const std::string& name) {
	//	if (currentMtlLibPtr) {
	//		currentMtlLibPtr->mtls.erase(name);
	//	}
	//	SynchronizeIds();
	//}

	//void SimpleMaterialSystem::DeleteLibraryMaterial(const std::string& mtllibName, const std::string& mtlName) {
	//	std::string oldLibrary = currentMtlLibName;
	//	std::string oldMaterial = currentMtlName;

	//	SimpleMaterialLibrary* pmtllib = SetLibrary(mtllibName);
	//	if (pmtllib) {
	//		pmtllib->mtls.erase(mtlName);
	//	}

	//	SetLibraryMaterial(oldLibrary, oldMaterial);
	//}

	//SimpleMaterialLibrary* SimpleMaterialSystem::SetLibrary(const std::string& name) {
	//	currentMtlLibName = name;
	//	currentMtlName = "";
	//	SynchronizeIds();
	//	return currentMtlLibPtr;
	//}

	//SimpleMaterial* SimpleMaterialSystem::SetLibraryMaterial(const std::string& mtllibName, const std::string& mtlName) {
	//	currentMtlLibName = mtllibName;
	//	currentMtlName = mtlName;
	//	SynchronizeIds();
	//	return currentMtlPtr;
	//}

	//const SimpleMaterial* SimpleMaterialSystem::GetLibraryMaterial(const std::string& mtllibName, const std::string& mtlName) const {
	//	const SimpleMaterial* mtl = nullptr;

	//	auto mtlLibId = mtllibs.getHandleFromName(mtllibName);
	//	auto mtlLibPtr = &(mtllibs[mtlLibId]);

	//	// Check to see if we're using a valid material
	//	auto mtlId = mtlLibPtr->mtls.getHandleFromName(mtlName);
	//	mtl = &(mtlLibPtr->mtls[mtlId]);

	//	return mtl;
	//}

	//SimpleMaterial* SimpleMaterialSystem::SetMaterial(const std::string& name) {
	//	currentMtlName = name;
	//	SynchronizeIds();
	//	return currentMtlPtr;
	//}

	//const std::string& SimpleMaterialSystem::GetLibraryName() const {
	//	return currentMtlLibName;
	//}

	//const std::string& SimpleMaterialSystem::GetMaterialName() const {
	//	return currentMtlName;
	//}

	//const std::string& SimpleMaterialSystem::GetMaterialName(GLuint handle) const {
	//	if (currentMtlLibPtr != nullptr)
	//		return currentMtlLibPtr->mtls.getNameFromHandle(handle);
	//	else
	//		return defaultName;
	//}

	//GLuint SimpleMaterialSystem::GetLibraryId() const {
	//	return currentMtlLibId;
	//}

	//GLuint SimpleMaterialSystem::GetMaterialId() const {
	//	return currentMtlId;
	//}

	//GLuint SimpleMaterialSystem::GetLibraryId(const std::string& name) const {
	//	return mtllibs.getHandleFromName(name);
	//}

	//GLuint SimpleMaterialSystem::GetMaterialId(const std::string& name) const {
	//	if (currentMtlLibPtr) {
	//		return currentMtlLibPtr->mtls.getHandleFromName(name);
	//	}
	//	return 0;
	//}

	//GLuint SimpleMaterialSystem::GetLibraryMaterialId(const std::string& mtllibName, const std::string& mtlName) const {
	//	const SimpleMaterialLibrary* pmtllib = nullptr;
	//	GLuint handle = 0;
	//	if (mtllibs.isAHandle(mtllibName))
	//		pmtllib = &(mtllibs[mtllibName]);
	//	if (pmtllib != nullptr) {
	//		handle = pmtllib->mtls.getHandleFromName(mtlName);
	//	}
	//	return handle;
	//}

	//void SimpleMaterialSystem::FindMapPaths(const std::vector<std::string>& pathsToTry) {
	//	maps_paths.clear();

	//	//for (auto libIt = mtllibs.begin(); libIt != mtllibs.end(); libIt++) {
	//	for (auto& [mtllibk, mtllib]: mtllibs) {
	//		//for (auto mapIt = libIt->second.maps.begin(); mapIt != libIt->second.maps.end(); mapIt++) {
	//		for (auto& [mapk, map]: mtllib.maps) {
	//			if (map.pathname.empty()) continue;
	//			std::string path = FindPathIfExists(map.pathname, pathsToTry);
	//			if (!path.empty()) {
	//				// TODO: Renable texture mapping
	//				//map.cached.textureObject.loadTexture2D(map.pathname);
	//				//map.cached.textureId = map.cached.textureObject.getTextureId();
	//				maps_paths[map.mapName] = path;
	//			}
	//		}
	//	}
	//}

	//SimpleMap* SimpleMaterialSystem::GetTextureMap(const std::string& name) {
	//	SimpleMap* map = nullptr;

	//	if (currentMtlLibPtr != nullptr) {
	//		if (currentMtlLibPtr->maps.isAHandle(name)) {
	//			map = &(currentMtlLibPtr->maps[name]);
	//		}
	//	}

	//	return map;
	//}

	//const SimpleMap* SimpleMaterialSystem::GetTextureMap(const std::string& name) const {
	//	SimpleMap* map = nullptr;

	//	if (currentMtlLibPtr != nullptr) {
	//		if (currentMtlLibPtr->maps.isAHandle(name)) {
	//			map = &(currentMtlLibPtr->maps[name]);
	//		}
	//	}

	//	return map;
	//}

	//const std::string& SimpleMaterialSystem::GetMapPath(const std::string& mapName) const {
	//	auto it = maps_paths.find(mapName);
	//	if (it == maps_paths.end())
	//		return defaultMapPath;
	//	return it->second;
	//}

	//// ------- F U N C T I O N S -------

	//void SetMaterialDefaults(SimpleMaterial& mtl) {
	//	mtl.Kd = Vector3f(1, 1, 1);
	//	mtl.Ks = Vector3f(0, 0, 0);
	//	mtl.Ke = Vector3f(0, 0, 0);
	//	mtl.Ka = Vector3f(0.2f, 0.2f, 0.2f);
	//	mtl.Tf = Vector3f(0, 0, 0);
	//	mtl.Tr = 0;
	//	mtl.d = 1;
	//	mtl.Refract = Vector3f(0, 0, 0);
	//	mtl.Opacity = Vector3f(0, 0, 0);
	//	mtl.Ns = 0;
	//	mtl.Ni = 0;
	//	mtl.PBm = 0;
	//	mtl.PBk = 0;
	//	mtl.PBKdm = 0;
	//	mtl.PBKsm = 0;
	//	mtl.PBior = 0;
	//	mtl.PBGGXgamma = 1.0;
	//	mtl.EmissionGlossiness = 0;
	//	mtl.ReflectFresnel = 0;
	//	mtl.RefractGlossiness = 0;
	//	mtl.ReflectGlossiness = 0;
	//	mtl.AnisotropyAmount = 0;
	//	mtl.AnisotropyRotation = 0;
	//	mtl.AttenuationColor = Vector3f(0, 0, 0);
	//	mtl.AttenuationDistance = 0;
	//	mtl.ScatteringAlbedo = Vector3f(0, 0, 0);
	//	mtl.MeanCosine = 0;
	//	mtl.RefractMode.clear();
	//	//mtl.HasRoundedCorners = false;
	//	//mtl.RoundedCorners = 0;
	//	//mtl.IsPortal = false;
	//	//mtl.HasIesProfile = false;
	//	//mtl.IesProfileFilename = "";
	//	//mtl.IesProfileMatrix.LoadIdentity();
	//	//mtl.IesProfileKeepSharp = false;
	//	mtl.Invisible.clear();
	//	mtl.map_Ka = "";
	//	mtl.map_Kd = "";
	//	mtl.map_Ks = "";
	//	mtl.map_d = "";
	//	mtl.map_opacity = "";
	//	mtl.map_Ns = "";
	//	mtl.map_Ke = "";
	//	mtl.map_Tf = "";
	//	mtl.map_Tr = "";
	//	mtl.map_refract = "";
	//	mtl.map_anisotropy = "";
	//	mtl.map_anisorotation = "";
	//	mtl.map_scatteringAlbedo = "";
	//	mtl.map_attenuation = "";
	//	mtl.map_normal = "";
	//	mtl.map_bump = "";

	//	// This is the new
	//	mtl.Properties["Kd"] = MakeSharedVec3Property(Vector3f(1.0f, 1.0f, 1.0f));
	//	mtl.Properties["Ks"] = MakeSharedVec3Property(Vector3f(0.0f, 0.0f, 0.0f));
	//	mtl.Properties["Ke"] = MakeSharedVec3Property(Vector3f(0.0f, 0.0f, 0.0f));
	//	mtl.Properties["Ka"] = MakeSharedVec3Property(Vector3f(0.0f, 0.0f, 0.0f));
	//	mtl.Properties["Tf"] = MakeSharedVec3Property(Vector3f(0.0f, 0.0f, 0.0f));
	//	mtl.Properties["Tr"] = MakeSharedFloatProperty(0.0f);
	//	mtl.Properties["d"] = MakeSharedFloatProperty(1.0f);
	//	mtl.Properties["Refract"] = MakeSharedVec3Property(Vector3f(0.0f, 0.0f, 0.0f));
	//	mtl.Properties["Opacity"] = MakeSharedVec3Property(Vector3f(0.0f, 0.0f, 0.0f));
	//	mtl.Properties["Ns"] = MakeSharedFloatProperty(0.0f);
	//	mtl.Properties["Ni"] = MakeSharedFloatProperty(0.0f);
	//	mtl.Properties["PBm"] = MakeSharedFloatProperty(0.0f);
	//	mtl.Properties["PBk"] = MakeSharedFloatProperty(0.0f);
	//	mtl.Properties["PBKdm"] = MakeSharedFloatProperty(0.0f);
	//	mtl.Properties["PBKsm"] = MakeSharedFloatProperty(0.0f);
	//	mtl.Properties["PBior"] = MakeSharedFloatProperty(0.0f);
	//	mtl.Properties["PBGGXgamma"] = MakeSharedFloatProperty(1.0f);
	//	mtl.Properties["EmissionGlossiness"] = MakeSharedFloatProperty(0.0f);
	//	mtl.Properties["ReflectFresnel"] = MakeSharedFloatProperty(0.0f);
	//	mtl.Properties["RefractGlossiness"] = MakeSharedFloatProperty(0.0f);
	//	mtl.Properties["ReflectGlossiness"] = MakeSharedFloatProperty(0.0f);
	//	mtl.Properties["AnisotropyAmount"] = MakeSharedFloatProperty(0.0f);
	//	mtl.Properties["AnisotropyRotation"] = MakeSharedFloatProperty(0.0f);
	//	mtl.Properties["AttenuationColor"] = MakeSharedVec3Property(Vector3f(0.0f, 0.0f, 0.0f));
	//	mtl.Properties["AttenuationDistance"] = MakeSharedFloatProperty(0.0f);
	//	mtl.Properties["ScatteringAlbedo"] = MakeSharedVec3Property(Vector3f(0.0f, 0.0f, 0.0f));
	//	mtl.Properties["MeanCosine"] = MakeSharedFloatProperty(0.0f);
	//	mtl.Properties["HasRoundedCorners"] = MakeSharedIntProperty(0);
	//	mtl.Properties["RoundedCorners"] = MakeSharedFloatProperty(0.0f);
	//	mtl.Properties["IsPortal"] = MakeSharedIntProperty(0);
	//	mtl.Properties["HasIesProfile"] = MakeSharedIntProperty(0);
	//	mtl.Properties["IesProfileFilename"] = MakeSharedStringProperty("");
	//	Matrix4f identity;
	//	identity.LoadIdentity();
	//	mtl.Properties["IesProfileMatrix"] = MakeSharedMat4Property(identity);
	//	mtl.Properties["IesProfileKeepSharp"] = MakeSharedIntProperty(0);
	//	mtl.Properties["map_Ka"] = MakeSharedStringProperty("");
	//	mtl.Properties["map_Kd"] = MakeSharedStringProperty("");
	//	mtl.Properties["map_Ks"] = MakeSharedStringProperty("");
	//	mtl.Properties["map_d"] = MakeSharedStringProperty("");
	//	mtl.Properties["map_opacity"] = MakeSharedStringProperty("");
	//	mtl.Properties["map_Ns"] = MakeSharedStringProperty("");
	//	mtl.Properties["map_Ke"] = MakeSharedStringProperty("");
	//	mtl.Properties["map_Tf"] = MakeSharedStringProperty("");
	//	mtl.Properties["map_Tr"] = MakeSharedStringProperty("");
	//	mtl.Properties["map_refract"] = MakeSharedStringProperty("");
	//	mtl.Properties["map_anisotropy"] = MakeSharedStringProperty("");
	//	mtl.Properties["map_anisorotation"] = MakeSharedStringProperty("");
	//	mtl.Properties["map_scatteringAlbedo"] = MakeSharedStringProperty("");
	//	mtl.Properties["map_attenuation"] = MakeSharedStringProperty("");
	//	mtl.Properties["map_normal"] = MakeSharedStringProperty("");
	//	mtl.Properties["map_bump"] = MakeSharedStringProperty("");
	//}

	//std::ostream& WriteMaterial(std::ostream& ostr, const SimpleMaterial& mtl) {
	//	ostr << "Kd ";
	//	WriteColor3f(ostr, mtl.Kd);
	//	ostr << std::endl;
	//	ostr << "Ks ";
	//	WriteColor3f(ostr, mtl.Ks);
	//	ostr << std::endl;
	//	ostr << "Ke ";
	//	WriteColor3f(ostr, mtl.Ke);
	//	ostr << std::endl;
	//	ostr << "Ka ";
	//	WriteColor3f(ostr, mtl.Ka);
	//	ostr << std::endl;
	//	ostr << "Tf ";
	//	WriteColor3f(ostr, mtl.Tf);
	//	ostr << std::endl;
	//	ostr << "Tr ";
	//	WriteDouble(ostr, mtl.Tr);
	//	ostr << std::endl;
	//	ostr << "Refract ";
	//	WriteColor3f(ostr, mtl.Refract);
	//	ostr << std::endl;
	//	ostr << "Opacity ";
	//	WriteColor3f(ostr, mtl.Opacity);
	//	ostr << std::endl;
	//	ostr << "Ns ";
	//	WriteDouble(ostr, mtl.Ns);
	//	ostr << std::endl;
	//	ostr << "Ni ";
	//	WriteDouble(ostr, mtl.Ni);
	//	ostr << std::endl;
	//	ostr << "ReflectFresnel ";
	//	WriteDouble(ostr, mtl.ReflectFresnel);
	//	ostr << std::endl;
	//	ostr << "RefractGlossiness ";
	//	WriteDouble(ostr, mtl.RefractGlossiness);
	//	ostr << std::endl;
	//	ostr << "ReflectGlossiness ";
	//	WriteDouble(ostr, mtl.ReflectGlossiness);
	//	ostr << std::endl;
	//	ostr << "Anisotropy ";
	//	WriteDouble(ostr, mtl.AnisotropyAmount);
	//	WriteDouble(ostr, mtl.AnisotropyRotation);
	//	ostr << std::endl;
	//	ostr << "Attenuation ";
	//	WriteColor3f(ostr, mtl.AttenuationColor);
	//	WriteDouble(ostr, mtl.AttenuationDistance);
	//	ostr << std::endl;
	//	ostr << "MeanCosine ";
	//	WriteDouble(ostr, mtl.MeanCosine);
	//	ostr << std::endl;
	//	//if (mtl.HasRoundedCorners) {
	//	//	ostr << "RoundedCorners ";
	//	//	WriteDouble(ostr, mtl.RoundedCorners);
	//	//	ostr << std::endl;
	//	//}
	//	//if (mtl.IsPortal)
	//	//	ostr << "Portal\n";
	//	//if (mtl.HasIesProfile) {
	//	//	ostr << "IesProfile ";
	//	//	WriteString(ostr, mtl.IesProfileFilename);
	//	//	WriteMatrix4f(ostr, mtl.IesProfileMatrix);
	//	//	WriteBool(ostr, mtl.IesProfileKeepSharp);
	//	//}
	//	for (auto it = mtl.RefractMode.begin(); it <= mtl.RefractMode.end(); it++) {
	//		if (it == mtl.RefractMode.begin())
	//			ostr << "RefractMode ";
	//		ostr << (*it) << " ";
	//	}
	//	for (auto it = mtl.Invisible.begin(); it <= mtl.Invisible.end(); it++) {
	//		if (it == mtl.Invisible.begin())
	//			ostr << "Invisible ";
	//		ostr << (*it) << " ";
	//	}
	//	if (!mtl.map_Ka.empty()) {
	//		ostr << "map_Ka ";
	//		WriteString(ostr, mtl.map_Ka);
	//		ostr << std::endl;
	//	}
	//	if (!mtl.map_Kd.empty()) {
	//		ostr << "map_Kd ";
	//		WriteString(ostr, mtl.map_Kd);
	//		ostr << std::endl;
	//	}
	//	if (!mtl.map_Ks.empty()) {
	//		ostr << "map_Ks ";
	//		WriteString(ostr, mtl.map_Ks);
	//		ostr << std::endl;
	//	}
	//	if (!mtl.map_Ke.empty()) {
	//		ostr << "map_Ke ";
	//		WriteString(ostr, mtl.map_Ke);
	//		ostr << std::endl;
	//	}
	//	if (!mtl.map_d.empty()) {
	//		ostr << "map_d ";
	//		WriteString(ostr, mtl.map_d);
	//		ostr << std::endl;
	//	}
	//	if (!mtl.map_opacity.empty()) {
	//		ostr << "map_opacity ";
	//		WriteString(ostr, mtl.map_opacity);
	//		ostr << std::endl;
	//	}
	//	if (!mtl.map_Ns.empty()) {
	//		ostr << "map_Ns ";
	//		WriteString(ostr, mtl.map_Ns);
	//		ostr << std::endl;
	//	}
	//	if (!mtl.map_Tr.empty()) {
	//		ostr << "map_Tr ";
	//		WriteString(ostr, mtl.map_Tr);
	//		ostr << std::endl;
	//	}
	//	if (!mtl.map_Tf.empty()) {
	//		ostr << "map_Tf ";
	//		WriteString(ostr, mtl.map_Tf);
	//		ostr << std::endl;
	//	}
	//	if (!mtl.map_refract.empty()) {
	//		ostr << "map_refract ";
	//		WriteString(ostr, mtl.map_refract);
	//		ostr << std::endl;
	//	}
	//	if (!mtl.map_anisotropy.empty()) {
	//		ostr << "map_anisotropy ";
	//		WriteString(ostr, mtl.map_anisotropy);
	//		ostr << std::endl;
	//	}
	//	if (!mtl.map_anisorotation.empty()) {
	//		ostr << "map_anisorotation ";
	//		WriteString(ostr, mtl.map_anisorotation);
	//		ostr << std::endl;
	//	}
	//	if (!mtl.map_scatteringAlbedo.empty()) {
	//		ostr << "map_scatteringAlbedo ";
	//		WriteString(ostr, mtl.map_scatteringAlbedo);
	//		ostr << std::endl;
	//	}
	//	if (!mtl.map_attenuation.empty()) {
	//		ostr << "map_attenuation ";
	//		WriteString(ostr, mtl.map_attenuation);
	//		ostr << std::endl;
	//	}
	//	if (!mtl.map_normal.empty()) {
	//		ostr << "map_normal ";
	//		WriteString(ostr, mtl.map_normal);
	//		ostr << std::endl;
	//	}
	//	if (!mtl.map_bump.empty()) {
	//		ostr << "map_bump ";
	//		WriteString(ostr, mtl.map_bump);
	//		ostr << std::endl;
	//	}
	//	return ostr;
	//}
} // namespace Fluxions
