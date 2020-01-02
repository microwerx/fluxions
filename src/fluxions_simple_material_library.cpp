#include "pch.hpp"
#include <fluxions_file_system.hpp>
#include <fluxions_simple_material_library.hpp>

namespace Fluxions
{
	void SimpleMaterialLibrary::clear() {
		maps.clear();
		mtllibs.clear();
		mtls.clear();
	}

	bool SimpleMaterialLibrary::loadMtls(const std::map<std::string, std::string>& mtlsToLoad) {
		bool result = true;
		for (auto& [name, path] : mtlsToLoad) {
			if (!load(path)) result = false;
		}
		return result;
	}

	bool SimpleMaterialLibrary::load(const std::string& filename) {
		FilePathInfo fpi(filename);
		if (mtllibs.count(fpi.fname)) return true;

		std::string name = fpi.fname;
		std::string pathToMTL = fpi.path;

		std::vector<std::string> lines;
		ReadLines(pathToMTL, lines, false);

		SimpleMaterial* curmtl = nullptr;
		for (auto& line : lines) {
			std::string str;
			std::stringstream istr(line);

			istr >> str;
			if (str == "newmtl") {
				curmtl = set_mtl(ReadString(istr));
				HFLOGINFO("newmtl '%s'", curmtl->name.c_str());
			}
			else if (curmtl == nullptr) {
			} // do nothing from now on because we're null
			else if (str.empty()) {
			} // do nothing if there is an empty string
			else if (str.size() >= 3 && str[0] == '#' && str[1] != 'P' && str[2] != 'B') {
			} // do nothing because it is a comment, not a #PB...
			else if (str.size() < 3 && str[0] == '#') {
			} // do nothing because it's a 2 character short comment
			else if (str == "Kd") {
				curmtl->Kd = ReadColor3f(istr);
			}
			else if (str == "Ks") {
				curmtl->Ks = ReadColor3f(istr);
			}
			else if (str == "Ke") {
				curmtl->Ke = ReadColor3f(istr);
			}
			else if (str == "ior") {
				curmtl->Kior.reset(ReadFloat(istr));
			}
			else if (str == "Ns") {
				curmtl->Ksroughness.r = shininessToAlpha(ReadFloat(istr));
			}
			else if (str == "#PBKdm" || str == "Kdm") {
				curmtl->Kdroughness.r = ReadFloat(istr);
			}
			else if (str == "#PBKdfuzzydusty" || str == "Kdfuzzydusty") {
				curmtl->Kdroughness.g = ReadFloat(istr);
			}
			else if (str == "#PBKdsubsurface" || str == "Kdsubsurface") {
				curmtl->Kdroughness.b = ReadFloat(istr);
			}
			else if (str == "#PBKsm" || str == "Ksm") {
				curmtl->Ksroughness.r = ReadFloat(istr);
			}
			else if (str == "#PBKsGGXgamma" || str == "KsGGXgamma") {
				curmtl->Ksroughness.g = ReadFloat(istr);
			}
			else if (str == "#PBKsaniso" || str == "Ksaniso" || str == "aniso") {
				curmtl->Ksroughness.b = ReadFloat(istr);
			}
			else if (str == "#PBKsior" || str == "Ksior") {
				curmtl->Kior = ReadColor3f(istr);
			}
			else if (str == "#PBKdior" || str == "Kdior") {
				curmtl->Kior.a = ReadFloat(istr);
			}
			else if (str == "#PBKsmetallic" || str == "Ksmetallic" || str == "metallic" || str == "Pm") {
				curmtl->KmetallicSpecular.r = ReadFloat(istr);
			}
			else if (str == "#PBKsspecular" || str == "Ksspecular" || str == "specular") {
				curmtl->KmetallicSpecular.g = ReadFloat(istr);
			}
			else if (str == "#PBKsspeculartint" || str == "Ksspeculartint" || str == "speculartint") {
				curmtl->KmetallicSpecular.b = ReadFloat(istr);
			}
			else if (str == "#PBKsanisor" || str == "Ksanisor" || str == "anisor") {
				curmtl->KmetallicSpecular.a = ReadFloat(istr);
			}
			else if (str == "Pc") {
				curmtl->KclearcoatSheen.r = ReadFloat(istr);
			}
			else if (str == "Pcr") {
				curmtl->KclearcoatSheen.g = ReadFloat(istr);
			}
			else if (str == "sheen") {
				curmtl->KclearcoatSheen.b = ReadFloat(istr);
			}
			else if (str == "sheentint") {
				curmtl->KclearcoatSheen.a = ReadFloat(istr);
			}
			else if (str == "map_Ka") {
				readMap(istr, curmtl->map_Ka, fpi.dir);
			}
			else if (str == "map_Kd") {
				readMap(istr, curmtl->map_Kd, fpi.dir);
			}
			else if (str == "map_Ks") {
				readMap(istr, curmtl->map_Ks, fpi.dir);
			}
			else if (str == "map_Ke") {
				readMap(istr, curmtl->map_Ke, fpi.dir);
			}
			else if (str == "map_Kdpbr") {
				readMap(istr, curmtl->map_Kdpbr, fpi.dir);
			}
			else if (str == "map_Kspbr") {
				readMap(istr, curmtl->map_Kspbr, fpi.dir);
			}
			else if (str == "map_Ps") {
				readMap(istr, curmtl->map_Ps, fpi.dir);
			}
			else if (str == "map_Pc") {
				readMap(istr, curmtl->map_Pc, fpi.dir);
			}
			else if (str == "map_Pm") {
				readMap(istr, curmtl->map_Pm, fpi.dir);
			}
			else if (str == "map_norm" || str == "norm") {
				readMap(istr, curmtl->map_norm, fpi.dir);
			}
			else if (str == "map_bump" || str == "bump") {
				readMap(istr, curmtl->map_bump, fpi.dir);
			}
			else if (!str.empty()) {
				HFLOGWARN("property %s not officially supported", str.c_str());
			}
		}

		return true;
	}

	bool SimpleMaterialLibrary::saveMTL(const std::string& filename) const {
		FilePathInfo fpi(filename);
		const char* endl = "\n    ";
		std::ofstream fout(filename);
		for (const auto& mtl : mtls) {
			WriteLabel(fout, "newmtl");
			WriteString(fout, mtl.name) << endl;
			WriteLabel(fout, "Kd"); WriteColor3f(fout, mtl.Kd.rgb()) << endl;
			WriteLabel(fout, "Ks"); WriteColor3f(fout, mtl.Ks.rgb()) << endl;
			WriteLabel(fout, "Ke"); WriteColor3f(fout, mtl.Ke.rgb()) << endl;
			WriteLabel(fout, "Ns"); WriteFloat(fout, alphaToShininess(mtl.Ksroughness.r)) << endl;
			WriteLabel(fout, "ior"); WriteFloat(fout, mtl.Kior.r) << endl;
			WriteLabel(fout, "Ksior"); WriteColor3f(fout, mtl.Kior.rgb()) << endl;
			WriteLabel(fout, "Kdior"); WriteColor3f(fout, mtl.Kior.a) << endl;
			WriteLabel(fout, "Kdm"); WriteColor3f(fout, mtl.Kdroughness.r) << endl;
			WriteLabel(fout, "Kdfuzzydusty"); WriteColor3f(fout, mtl.Kdroughness.g) << endl;
			WriteLabel(fout, "Kdsubsurface"); WriteColor3f(fout, mtl.Kdroughness.b) << endl;
			WriteLabel(fout, "Ksm"); WriteColor3f(fout, mtl.Ksroughness.r) << endl;
			WriteLabel(fout, "KsGGXgamma"); WriteColor3f(fout, mtl.Ksroughness.g) << endl;
			WriteLabel(fout, "aniso"); WriteColor3f(fout, mtl.Ksroughness.b) << endl;
			WriteLabel(fout, "anisor"); WriteColor3f(fout, mtl.KmetallicSpecular.a) << endl;
			WriteLabel(fout, "Pm"); WriteFloat(fout, alphaToShininess(mtl.KmetallicSpecular.r)) << endl;
			WriteLabel(fout, "specular"); WriteFloat(fout, alphaToShininess(mtl.KmetallicSpecular.g)) << endl;
			WriteLabel(fout, "speculartint"); WriteFloat(fout, alphaToShininess(mtl.KmetallicSpecular.b)) << endl;
			WriteLabel(fout, "Pc"); WriteFloat(fout, alphaToShininess(mtl.KclearcoatSheen.r)) << endl;
			WriteLabel(fout, "Pcr"); WriteFloat(fout, alphaToShininess(mtl.KclearcoatSheen.g)) << endl;
			WriteLabel(fout, "sheen"); WriteFloat(fout, alphaToShininess(mtl.KclearcoatSheen.b)) << endl;
			WriteLabel(fout, "sheentint"); WriteFloat(fout, alphaToShininess(mtl.KclearcoatSheen.a)) << endl;
			printMap(fout, "map_Ka", mtl.map_Ka) << endl;
			printMap(fout, "map_Kd", mtl.map_Kd) << endl;
			printMap(fout, "map_Ks", mtl.map_Ks) << endl;
			printMap(fout, "map_Ke", mtl.map_Ke) << endl;
			printMap(fout, "map_Kdpbr", mtl.map_Kdpbr) << endl;
			printMap(fout, "map_Kspbr", mtl.map_Kspbr) << endl;
			printMap(fout, "map_Ps", mtl.map_Ps) << endl;
			printMap(fout, "map_Pc", mtl.map_Pc) << endl;
			printMap(fout, "map_Pm", mtl.map_Pm) << endl;
			printMap(fout, "map_norm", mtl.map_norm) << endl;
			printMap(fout, "map_bump", mtl.map_bump) << endl;
		}
		return true;
	}

	bool SimpleMaterialLibrary::saveXml(const std::string& path, int flags) const {
		FilePathInfo fpi(path);
		const char* endl = "\n    ";
		std::ofstream mtlxml_fout(path);
		XmlBeginTag(mtlxml_fout, "mtlLib") << endl;
		if (flags & WRITE_MATERIALS) {
			bool enableSpecular = (flags & WRITE_SPECULAR);
			for (const auto& mtl : mtls) {
				XmlBeginTag(mtlxml_fout, "materialDefinition", "name", mtl.name, 1) << "\n";
				XmlBeginTag(mtlxml_fout, "material", "Native", 2) << "\n";

				bool isMetal = enableSpecular && mtl.KmetallicSpecular.r > 0.0f;
				if (!mtl.map_Kd.empty()) {
					XmlBeginTag(mtlxml_fout, "diffuse", 3) << "\n";
					XmlBeginTag(mtlxml_fout, "map", "Reference", 4);
					mtlxml_fout << mtl.map_Kd;
					XmlEndTag(mtlxml_fout, "map") << "\n";
					XmlEndTag(mtlxml_fout, "diffuse", 3) << "\n";
				}
				else if (isMetal)
					XmlColor3f(mtlxml_fout, "diffuse", Color3f(), 3) << "\n";
				else
					XmlColor3f(mtlxml_fout, "diffuse", mtl.Kd, 3) << "\n";

				if (enableSpecular) {
					XmlBeginTag(mtlxml_fout, "reflect", 3) << "\n";
					XmlColor3f(mtlxml_fout, "color", mtl.Ks, 4) << "\n";

					if (isMetal) {
						XmlFloat(mtlxml_fout, "ior", 999, 4) << "\n";
					}
					else {
						XmlFloat(mtlxml_fout, "ior", mtl.Kior.r, 4) << "\n";
					}
					// Corona uses glossiness which is the reverse of roughness
					// A surface is smooth if roughness is 0 or glossiness is 1
					// A surface is rough if roughness is 1 or glossiness is 0
					if (mtl.Ksroughness.r < 0.0f)
						XmlFloat(mtlxml_fout, "glossiness", clamp(1.0f + mtl.Ksroughness.r, 0.0f, 1.0f), 4) << "\n";
					else
						XmlFloat(mtlxml_fout, "glossiness", clamp(1.0f - mtl.Ksroughness.r, 0.0f, 1.0f), 4) << "\n";
					XmlEndTag(mtlxml_fout, "reflect", 3) << "\n";
				}

				XmlEndTag(mtlxml_fout, "material", 2) << "\n";
				XmlEndTag(mtlxml_fout, "materialDefinition", 1) << "\n";
			}
		}

		if (flags & WRITE_MAPS) {
			XmlComment(mtlxml_fout, "Texture Maps", 1) << "\n\n";
			for (auto& [mapName, imagePath] : maps) {
				XmlCoronaMapTexture(mtlxml_fout, "mapDefinition", mapName, imagePath, 1, 2.2f) << "\n";
			}
		}

		XmlEndTag(mtlxml_fout, "mtlLib");
		return true;
	}

	SimpleMaterial* SimpleMaterialLibrary::set_mtl(std::string&& name) {
		SimpleMaterial newmtl;
		newmtl.name = std::move(name);

		// look for existing material
		for (auto& mtl : mtls) {
			if (mtl.name == name) {
				HFLOGWARN("Duplicate material found '%s'", newmtl.name.c_str());
				return &mtl;
			}
		}

		// add to end
		newmtl.renderIndex = mtls.size();
		mtls.push_back(newmtl);
	}

	bool SimpleMaterialLibrary::readMap(std::istream& istr, std::string& mapname, std::string& basepath) {
		std::string pathToMap = ReadString(istr);
		FilePathInfo fpi(basepath + pathToMap);

		// Check if we have added this map already
		if (maps.count(fpi.fname)) {
			return true;
		}

		// If it doesn't exist, try the working directory
		if (fpi.DoesNotExist()) {
			fpi.Set("./" + pathToMap);
		}

		if (fpi.DoesNotExist()) {
			HFLOGWARN("Map '%s' cannot be located", mapname.c_str());
			pathToMap = fpi.fullfname;
		}
		else {
			pathToMap = fpi.path;
		}

		// update the information in the map list
		mapname = fpi.fname;
		maps[mapname] = pathToMap;
		return true;
	}

	std::ostream& SimpleMaterialLibrary::printMap(std::ostream& ostr,
												  const std::string& maptype,
												  const std::string& mapname) const {
		if (!maptype.empty()) {
			WriteLabel(ostr, maptype);
			WriteString(ostr, maps.at(mapname));
		}
		return ostr;
	}
}