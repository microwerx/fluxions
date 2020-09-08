#include "fluxions_ssg_pch.hpp"
#include <damselfish.hpp>
#include <fluxions_ssg_ssphh_light.hpp>
#include <fluxions_renderer_base.hpp>
#include <fluxions_ssphh_utilities.hpp>
#include <fluxions_sphl_sampler.hpp>
#include <fluxions_fileio_iostream.hpp>

namespace Fluxions {
	SimpleSSPHHLight::SimpleSSPHHLight() {
		//hierLightProbeImage.resize(32, 32, 6);
		//hierLightProbeTexture.init("hierLightProbe");
		ptrcLightProbeTexture.init("ptrcLightProbe");
		msphLightProbeTexture.init("msphLightProbe");
	}

	SimpleSSPHHLight::~SimpleSSPHHLight() {
		ptrcLightProbeTexture.kill();
		msphLightProbeTexture.kill();
	}

	bool SimpleSSPHHLight::read(const std::string& keyword, std::istream& istr) {
		if (keyword == "sphl") {
			setName(ReadString(istr));
			E0 = ReadFloat(istr);
			falloffRadius = ReadFloat(istr);
			position = ReadVector3f(istr);
			transform = Matrix4f::MakeTranslation(position);
			addlTransform = Matrix4f::MakeIdentity();
			bbox.reset();
			bbox += position - falloffRadius;
			bbox += position + falloffRadius;
			changeDegrees(MaxSphlDegree);
			return true;
		}
		else if (keyword == "shlight") {
			setName(ReadString(istr));
			E0 = ReadFloat(istr);
			falloffRadius = ReadFloat(istr);
			position = ReadVector3f(istr);
			transform = Matrix4f::MakeTranslation(position);
			addlTransform = Matrix4f::MakeIdentity();
			bbox.reset();
			bbox += position - falloffRadius;
			bbox += position + falloffRadius;
			changeDegrees(MaxSphlDegree);

			orientation = ReadQuaternionf(istr);
			msph[0] = ReadSphericalHarmonicf(istr);
			msph[1] = msph[0];
			msph[2] = msph[0];
			standardize();
			return true;
		}
		return false;
	}

	bool SimpleSSPHHLight::write(std::ostream& ostr) const {
		ostr << "sphl " << "\"" << name() << "\" ";
		WriteFloat(ostr, E0);
		WriteFloat(ostr, falloffRadius);
		WriteVector3f(ostr, position);
		return true;
	}

	void SimpleSSPHHLight::makeDirty() {
		dirty = true;
	}

	void SimpleSSPHHLight::changeDegrees(int degrees) {
		if (degrees < 0)
			return;
		if (degrees > MaxSphlDegree)
			degrees = 9;
		maxDegree = degrees;
		dirty = true;
		for (int i = 0; i < 4; i++)
			msph[i].resize(maxDegree);

		//int i = 0;
		//for (auto & hier : hierarchies)
		//{
		//	hier.resize(maxDegree);
		//	hier.index = i;
		//	i++;
		//}
	}

	void SimpleSSPHHLight::resizeHierarchies(int size) {
		if (size < 0 || size > MaxSphlLights)
			return;

		vizgenLightProbes.resize(size);
	}

	void SimpleSSPHHLight::randomizeCoefficients(float size) {
		for (int l = 0; l <= maxDegree; l++) {
			for (int m = -l; m <= l; m++) {
				msph[0].setCoefficient(l, m, randomSampler(-size, size));
				msph[1].setCoefficient(l, m, randomSampler(-size, size));
				msph[2].setCoefficient(l, m, randomSampler(-size, size));
				msph[3].setCoefficient(l, m, randomSampler(-size, size));
			}
		}
		makeDirty();
	}

	// Reads from a Path Traced Light Probe (a cube map stored images from left to right in a single image).
	bool SimpleSSPHHLight::readPtrcLightProbe(const std::string& path) {
		FilePathInfo fpi(path);
		if (fpi.notFound())
			return false;

		if (fpi.extension() == ".ppm")
			ptrcLightProbeImage.loadPPM(path);
		else if (fpi.extension() == ".exr")
			ptrcLightProbeImage.loadEXR(path);
		else
			HFLOGERROR("Path %s is not a PPM or EXR", path.c_str());
		ptrcLightProbeImage.convertRectToCubeMapEx(Image4f::SwizzleCorona);
		double intensity = ptrcLightProbeImage.getTotalIntensity();
		double numPixels = ptrcLightProbeImage.getNumPixels();
		ptrcLightProbePercent = (float)(intensity / numPixels);
		changeDegrees(MaxSphlDegree);

		lightProbeToSph(ptrcLightProbeImage, msph);
		sphToLightProbe(msph, msphLightProbeImage);

		//ptrcLightProbeImage.reset();
		//msphLightProbeImage.reset();
		//hierLightProbeImage.reset();
		uploadLightProbe(ptrcLightProbeImage, ptrcLightProbeTexture);
		uploadLightProbe(msphLightProbeImage, msphLightProbeTexture);
		// UploadLightProbe(hierLightProbeImage, hierLightProbeTexture);

		dirty = true;

		return true;
	}

	// Saves to a Path Traced Light Probe (a cube map with cube faces stored from left to right in a single image).
	bool SimpleSSPHHLight::savePtrcLightProbe(const std::string& path) {
		FilePathInfo fpi(path);

		sphToLightProbe(msph, msphLightProbeImage);
		msphLightProbeImage.convertCubeMapToRect();
		if (fpi.extension() == ".ppm")
			msphLightProbeImage.savePPMi(path, 255.99f, 0, 255, 0, false);
		else if (fpi.extension() == ".exr")
			msphLightProbeImage.saveEXR(path);

		return false;
	}

	// Saves a JSON form of the multispectral (RGBI) of this SPH. I represents a monochromatic version of the RGB components. { maxDegree: (1-10), coefs : [] }
	bool SimpleSSPHHLight::saveJsonSph(const std::string& path) {
		FilePathInfo fpi(path);
		Df::JSONPtr json = Df::JSON::MakeObject({
			{"numChannels", Df::JSON::MakeNumber(3)},
			{"maxDegree", Df::JSON::MakeNumber((int)msph[0].GetMaxDegree())},
			{"coefs", Df::JSON::MakeArray()},
			{"meta", Df::JSON::MakeObject()} });

		Df::JSONPtr coefs = json->getMember("coefs");
		for (unsigned lm = 0; lm < msph->getMaxCoefficients(); lm++) {
			float r = msph[0].getCoefficient(lm);
			float g = msph[1].getCoefficient(lm);
			float b = msph[2].getCoefficient(lm);
			coefs->PushBack(Df::JSON::MakeArray({ r, g, b }));
		}

		Df::JSONPtr m_meta = Df::JSON::MakeObject({
			{ "name", Df::JSON::MakeString(name()) },
			{ "position", Df::JSON::MakeArray({
				position.x,
				position.y,
				position.z
				})
			} });

		Df::JSONPtr meta = json->getMember("meta");
		meta->set(m_meta);

		std::ofstream fout(fpi.shortestPath());
		fout << json->Serialize();
		fout.close();

		return true;
	}

	// Reads a JSON format of a multispectral (RGBL) of this SPH. L represents a monochromatic version of the RGB components. { maxDegree: (1-10), coefs : [] }
	bool SimpleSSPHHLight::readJsonSph(const std::string& path) {
		FilePathInfo fpi(path);
		if (fpi.notFound())
			return false;
		std::ifstream fin(fpi.shortestPath());
		std::string buffer;
		while (!fin) {
			std::string line;
			getline(fin, line);
			buffer += "\n";
			buffer += line;
		}
		fin.close();

		Df::JSONPtr json = Df::JSON::New();
		if (!json->Deserialize(buffer)) {
			HFLOGERROR("Unable to read JSON SPH");
			return false;
		}

		if (json->IsObject() &&
			json->getMember("numChannels")->IsNumber() &&
			json->getMember("maxDegree")->IsNumber() &&
			json->getMember("coefs")->IsArray()) {
			// int jsonMaxDegree = json->getMember("maxDegree")->AsInt();
			Df::JSONPtr coefs = json->getMember("coefs");
			for (unsigned j = 0; j < 4; j++) {
				Df::JSONPtr e = coefs->getElement((int)j);
				if (e->IsArray()) {
					std::vector<float> coefJ;
					coefJ = e->AsFloatArray();
				}
			}
		}

		Df::JSONPtr meta = json->getMember("meta");
		if (meta->IsObject()) {
			auto nameMember = meta->getMember("name");
			if (nameMember->IsString()) {
				setName(nameMember->AsString());
			}

			auto m_position = meta->getMember("position");
			if (m_position->IsArray()) {
				std::vector<float> v = m_position->AsFloatArray();
				if (v.size() >= 3) {
					position.reset(v[0], v[1], v[2]);
				}
			}
			else if (m_position->IsObject()) {
				Df::JSONPtr x = m_position->getMember("x");
				Df::JSONPtr y = m_position->getMember("y");
				Df::JSONPtr z = m_position->getMember("z");
				if (x->IsNumber() && y->IsNumber() && z->IsNumber()) {
					position.reset(x->AsFloat(), y->AsFloat(), z->AsFloat());
				}
			}
		}

		return false;
	}

	bool SimpleSSPHHLight::saveOBJ(const std::string& path, const std::string& gname) {
		FilePathInfo fpi(path);
		static const char* icos_path = "ssphh-data/resources/models/icos4.txt";
		static unsigned numVertices = 0;
		static unsigned numTriangles = 0;
		static std::vector<Vector3f> vertices;
		static std::vector<Vector3ui> triangles;
		static std::vector<Vector3f> rgbiy[5];

		if (!fpi.isDirectory()) {
			HFLOGERROR("Path %s is not a directory!", path.c_str());
			return false;
		}

		if (numTriangles == 0) {
			FilePathInfo geosphere(icos_path);

			if (!geosphere.exists()) {
				HFLOGERROR("Icosahedron %s does not exist", icos_path);
				return false;
			}
			std::ifstream fin(icos_path);
			fin >> numVertices;
			vertices.resize(numVertices);
			for (Vector3f& v : vertices) {
				fin >> v.x;
				fin >> v.y;
				fin >> v.z;
				v.normalize();
			}
			fin >> numTriangles;
			triangles.resize(numTriangles);
			for (Vector3ui& t : triangles) {
				fin >> t.x >> t.y >> t.z;
			}
			fin.close();
		}

		// Make sure new RGBIY vertices are the right size
		for (auto& x : rgbiy) {
			x.resize(vertices.size());
		}

		for (unsigned i = 0; i < 3; i++) {
			if (msph[i].GetMaxDegree() < 5) {
				msph[i].resize(5);
				for (unsigned lm = 0; lm < msph[i].getMaxCoefficients(); lm++) {
					float x = 2.0f * (float)rand() / (float)RAND_MAX - 1.0f;
					msph[i].setCoefficient(lm, x);
				}
			}
		}

		// Calculate the 3D positions for the red, green,
		// blue, intensity, and luma Y' channels
		for (unsigned i = 0; i < vertices.size(); i++) {
			Vector3f& v = vertices[i];
			float theta = v.theta();
			float phi = v.phi();
			float r = this->msph[0].calc(theta, phi);
			float g = this->msph[1].calc(theta, phi);
			float b = this->msph[2].calc(theta, phi);
			float I = 0.333333f * (r + g + b);
			float Y = 0.299f * r + 0.587f * g + 0.114f * b;
			rgbiy[0][i] = (0.5f + 0.25f * r) * v;
			rgbiy[1][i] = (0.5f + 0.25f * g) * v;
			rgbiy[2][i] = (0.5f + 0.25f * b) * v;
			rgbiy[3][i] = (0.5f + 0.25f * I) * v;
			rgbiy[4][i] = (0.5f + 0.25f * Y) * v;
			if (i < 5) std::cout << r << std::endl;
		}
		const Color3f R(1.0f, 0.0f, 0.0f);
		const Color3f G(0.0f, 1.0f, 0.0f);
		const Color3f B(0.0f, 0.0f, 1.0f);
		const Color3f W(1.0f, 1.0f, 1.0f);

		// Try to save all the files; stop if one fails
		bool result = true;
		if (result) result = SaveSphlOBJ(path, gname + "_sph_ch0_R", R, rgbiy[0], triangles);
		if (result) result = SaveSphlOBJ(path, gname + "_sph_ch1_G", G, rgbiy[1], triangles);
		if (result) result = SaveSphlOBJ(path, gname + "_sph_ch2_B", B, rgbiy[2], triangles);
		if (result) result = SaveSphlOBJ(path, gname + "_sph_ch3_I", W, rgbiy[3], triangles);
		if (result) result = SaveSphlOBJ(path, gname + "_sph_ch4_Y", W, rgbiy[4], triangles);

		return result;
		//if (!__g_sphl_icos) {
		//	__g_sphl_icos = new SimpleGeometryMesh();
		//	for (unsigned i = 0; i < numVertices; i++) {
		//		__g_sphl_icos->Attrib3f(0, vertices[i]);
		//	}
		//	__g_sphl_icos->BeginSurface(SimpleGeometryMesh::SurfaceType::Triangles);
		//	for (unsigned i = 0; i < numTriangles; i++) {
		//		__g_sphl_icos->AddIndex(triangles[i * 3 + 0]);
		//		__g_sphl_icos->AddIndex(triangles[i * 3 + 1]);
		//		__g_sphl_icos->AddIndex(triangles[i * 3 + 2]);
		//	}
		//	__g_sphl_icos->EnableAttrib(0);
		//}

		/* TODO: Move this into the ASCM section
		const float a = 0.0f;
		const float b = 1.0f;
		const float c = ((1.0f + sqrtf(5.0f)) / 2.0f);
		const Vector3f A1(+a, +c, -b);
		const Vector3f A2(+a, -c, -b);
		const Vector3f A3(+a, -c, +b);
		const Vector3f A4(+a, +c, +b);
		const Vector3f B1(-b, +a, -c);
		const Vector3f B2(+b, +a, -c);
		const Vector3f B3(+b, +a, +c);
		const Vector3f B4(-b, +a, +c);
		const Vector3f C1(-c, +b, +a);
		const Vector3f C2(-c, -b, +a);
		const Vector3f C3(+c, -b, +a);
		const Vector3f C4(+c, +b, +a);

		std::vector<Vector3f> verts = {
			Vector3f(+0.0000,+1.0000,+0.0000),
			Vector3f(+0.8944,+0.4472,+0.0000),
			Vector3f(+0.2764,+0.4472,-0.8507),
			Vector3f(-0.7236,+0.4472,-0.5257),
			Vector3f(-0.7236,+0.4472,+0.5257),
			Vector3f(+0.2764,+0.4472,+0.8507),
			Vector3f(+0.7236,-0.4472,+0.5257),
			Vector3f(+0.7236,-0.4472,-0.5257),
			Vector3f(-0.2764,-0.4472,-0.8507),
			Vector3f(-0.8944,-0.4472,+0.0000),
			Vector3f(-0.2764,-0.4472,+0.8507),
			Vector3f(+0.0000f,-1.0000,0.0000)
		};

		std::vector<int> indcs = {
			0,1,2,
			0,2,3,
			0,3,4,
			0,4,5,
			0,5,1,
			1,6,7,
			2,7,8,
			3,8,9,
			4,9,10,
			5,10,6,
			7,2,1,
			8,3,2,
			9,4,3,
			10,5,4,
			6,1,5,
			11,8,7,
			11,9,8,
			11,10,9,
			11,6,10,
			11,7,6
		};

		// subdivide
		std::vector<Vector3f> inew;
		for (unsigned i = 0; i < indcs.size(); i += 3) {
			unsigned vi = indcs[i];
			unsigned vj = indcs[i + 1];
			unsigned vk = indcs[i + 2];
			Vector3f sidea(0.5 * (verts[vi] + verts[vj]));
			Vector3f sideb(0.5 * (verts[vj] + verts[vk]));
			Vector3f sidec(0.5 * (verts[vk] + verts[vi]));
			unsigned first = inew.size();
			verts.push_back(sidea);
			verts.push_back(sideb);
			verts.push_back(sidec);
		}
		*/
	}

	bool SimpleSSPHHLight::lightProbeToSph(const Image4f& lightProbe, MultispectralSph4f& sph) {
		SphlSampler sphlSampler;
		sphlSampler.resize(64, 32);
		sphlSampler.sampleCubeMap(lightProbe, sph);
		MakeIntensityChannel4f(sph);
		return false;
	}

	bool SimpleSSPHHLight::sphToLightProbe(const MultispectralSph4f& sph, Image4f& lightProbe) {
		float v_coefs[4][121];

		for (int j = 0; j < 4; j++) {
			for (int l = 0; l <= maxDegree; l++) {
				for (int m = -l; m <= l; m++) {
					auto lm = sph[j].getCoefficientIndex(l, m);
					v_coefs[j][lm] = E0 * sph[j][lm];
				}
			}
		}

		lightProbe.resize(32, 32, 6);
		for (unsigned face = 0; face < lightProbe.depth(); face++) {
			for (unsigned s = 0; s < lightProbe.width(); s++) {
				for (unsigned t = 0; t < lightProbe.height(); t++) {
					float _s = (float)s / (float)lightProbe.width();
					float _t = (float)t / (float)lightProbe.height();
					Vector3f v;
					MakeCubeVectorFromFaceST(face, _s, 1.0f - _t, &v.x, &v.y, &v.z);
					v.normalize();
					Color4f color;
					float theta = v.theta();
					float phi = v.phi();

					color.r = calc_spherical_harmonic<float>(maxDegree, v_coefs[0], theta, phi);
					color.g = calc_spherical_harmonic<float>(maxDegree, v_coefs[1], theta, phi);
					color.b = calc_spherical_harmonic<float>(maxDegree, v_coefs[2], theta, phi);
					color.a = calc_spherical_harmonic<float>(maxDegree, v_coefs[3], theta, phi);

					lightProbe.setPixelUnsafe(s, t, face, color);
				}
			}
		}
		return true;
	}

	bool SimpleSSPHHLight::sphToLightProbe(const MultispectralSph4f& sph, Image4f& lightProbe, int maxDegree_) {
		float v_coefs[4][121];

		maxDegree_ = std::min(maxDegree_, this->maxDegree);

		for (int j = 0; j < 4; j++) {
			for (int l = 0; l <= maxDegree_; l++) {
				for (int m = -l; m <= l; m++) {
					auto lm = sph[j].getCoefficientIndex(l, m);
					v_coefs[j][lm] = E0 * sph[j][lm];
				}
			}
		}

		lightProbe.resize(32, 32, 6);
		for (unsigned face = 0; face < lightProbe.depth(); face++) {
			for (unsigned s = 0; s < lightProbe.width(); s++) {
				for (unsigned t = 0; t < lightProbe.height(); t++) {
					float _s = (float)s / (float)lightProbe.width();
					float _t = (float)t / (float)lightProbe.height();
					Vector3f v;
					MakeCubeVectorFromFaceST(face, _s, 1.0f - _t, &v.x, &v.y, &v.z);
					v.normalize();
					Color4f color;
					float theta = v.theta();
					float phi = v.phi();

					color.r = calc_spherical_harmonic<float>(maxDegree_, v_coefs[0], theta, phi);
					color.g = calc_spherical_harmonic<float>(maxDegree_, v_coefs[1], theta, phi);
					color.b = calc_spherical_harmonic<float>(maxDegree_, v_coefs[2], theta, phi);
					color.a = calc_spherical_harmonic<float>(maxDegree_, v_coefs[3], theta, phi);

					lightProbe.setPixelUnsafe(s, t, face, color);
				}
			}
		}
		return true;
	}

	bool SimpleSSPHHLight::uploadLightProbe(Image4f& lightProbe, RendererTextureCube& texture) {
		if (lightProbe.empty())
			lightProbe.resize(32, 32, 6);

		//texture.init("lightProbe");
		texture.bind(0);
		for (int i = 0; i < 6; i++) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, (GLsizei)lightProbe.width(), (GLsizei)lightProbe.height(), 0, GL_RGBA, GL_FLOAT, lightProbe.getImageData(i));
		}
		FxGenerateMipmap(GL_TEXTURE_CUBE_MAP);
		texture.unbind();
		return true;
	}

	bool SimpleSSPHHLight::uploadLightProbe(Image4f& lightProbe, GLuint& texture) {
		if (lightProbe.empty())
			lightProbe.resize(32, 32, 6);

		FxCreateTexture(GL_TEXTURE_CUBE_MAP, &texture);
		for (int i = 0; i < 6; i++) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, (GLsizei)lightProbe.width(), (GLsizei)lightProbe.height(), 0, GL_RGBA, GL_FLOAT, lightProbe.getImageData(i));
		}
		FxGenerateMipmap(GL_TEXTURE_CUBE_MAP);
		//texture.Unbind();
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		return true;
	}

	Color3f SimpleSSPHHLight::getCoefficientColor(int l, int m) const {
		return Color3f(
			getCoefficient(0, l, m),
			getCoefficient(1, l, m),
			getCoefficient(2, l, m));
	}

	float SimpleSSPHHLight::getCoefficient(int j, int l, int m) const {
		if (j < 0 || j > 4)
			return 0.0f;
		return msph[j].getCoefficient(l, m);
	}

	float SimpleSSPHHLight::getCoefficient(int j, int lm) const {
		if (!within(j, 0, 3))
			return 0.0f;
		return msph[j].getCoefficient(lm);
	}

	void SimpleSSPHHLight::zeroCoefficients() {
		setCoefficient(-1000, -1000, -1000, 0.0f);
	}

	void SimpleSSPHHLight::isolateCoefficient(int j, int l, int m) {
		float value = getCoefficient(j, l, m);
		setCoefficient(-1000, -1000, -1000, 0.0f);
		setCoefficient(j, l, m, value);
	}

	void SimpleSSPHHLight::setCoefficient(int j, int l, int m, float value) {
		if (j > 4)
			return;

		int minJ = 0;
		int maxJ = 0;
		int minL = 0;
		int maxL = 0;
		int minM = 0;
		int maxM = 0;

		if (j < 0) {
			minJ = 0;
			maxJ = 4;
		}
		else {
			minJ = j;
			maxJ = j + 1;
		}

		if (l < 0) {
			minL = 0;
			maxL = maxDegree;
			m = -maxL - 1;
		}
		else {
			minL = l;
			maxL = l + 1;
		}
		for (int _j = minJ; _j < maxJ; _j++) {
			for (int _l = minL; _l <= maxL; _l++) {
				if (m < -_l) {
					minM = -_l;
					maxM = _l;
				}
				else {
					minM = m;
					maxM = m;
				}
				for (int _m = minM; _m <= maxM; _m++) {
					msph[_j].setCoefficient(_l, _m, value);
				}
			}
		}
		dirty = true;
	}

	void SimpleSSPHHLight::randomizeCoefficient(int j, int l, int m, float minValue, float maxValue) {
		if (j > 4)
			return;

		int minJ = 0;
		int maxJ = 0;
		int minL = 0;
		int maxL = 0;
		int minM = 0;
		int maxM = 0;

		if (j < 0) {
			minJ = 0;
			maxJ = 4;
		}
		else {
			minJ = j;
			maxJ = j + 1;
		}

		if (l < 0) {
			minL = 0;
			maxL = maxDegree;
		}
		else {
			minL = l;
			maxL = l + 1;
		}
		for (int _j = minJ; _j < maxJ; _j++) {
			for (int _l = minL; _l <= maxL; _l++) {
				if (m < 0) {
					minM = -_l;
					maxM = _l;
				}
				else {
					minM = m;
					maxM = m;
				}
				for (int _m = minM; _m <= maxM; _m++) {
					msph[_j].setCoefficient(_l, _m, randomSampler(minValue, maxValue));
				}
			}
		}

		dirty = true;
	}

	void SimpleSSPHHLight::sortHierarchiesByPercent() {

	}

	void SimpleSSPHHLight::sortHierarchiesByIndex() {

	}

	void SimpleSSPHHLight::copySphlToHierarchies() {
		//if (index < 0)
		//	throw "SSPHHLight not properly initialized";
		//hierarchies[index].CopySph(msph, 1.0);
	}

	void SimpleSSPHHLight::setHierarchyDescriptionToIndex() {
		//std::ostringstream ostr;
		//int i = 0;
		//ostr << "(" << hierarchies.size() << ") => [ ";
		//for (auto & hier : hierarchies)
		//{
		//	if (i != 0)
		//		ostr << ", ";
		//	ostr << hier.index;
		//	i++;
		//}
		//ostr << " ]";
		//hier_description = ostr.str();
	}

	void SimpleSSPHHLight::setHierarchyDescriptionToPercent() {
		//std::ostringstream ostr;
		//int i = 0;
		//ostr << "(" << hierarchies.size() << ") => [ ";
		//for (auto & hier : hierarchies)
		//{
		//	if (i != 0)
		//		ostr << ", ";
		//	ostr << std::fixed << std::setprecision(2) << (hier.percentVisible * 100.0) << "%";
		//	i++;
		//}
		//ostr << " ]";
		//hier_description = ostr.str();
	}

	void SimpleSSPHHLight::setHierarchyDescription() {
		//std::ostringstream ostr;
		//int i = 0;
		//ostr << "(" << index << "," << hierarchies.size() << ") => [ ";
		//for (auto & hier : hierarchies)
		//{

		//	float weight = 0.0;
		//	weight += hier.sph.sum();

		//	if (i != 0)
		//		ostr << ", ";
		//	ostr << "(" << hier.index << ", " << std::fixed << std::setprecision(2) << (hier.percentVisible * 100.0) << "%, " << weight << ")";
		//	i++;
		//}
		//ostr << " ]";
		//hier_description = ostr.str();
	}

	bool MakeStandardizedSph(SphericalHarmonicf& sph, MultispectralSph4f& msph) {
		for (unsigned i = 0; i < 4; i++) {
			if (sph.GetMaxDegree() != msph[i].GetMaxDegree())
				return false;
		}
		auto lmmax = sph.getMaxCoefficients();

		for (unsigned lm = 0; lm < lmmax; lm++) {
			float r = msph[0].getCoefficient(lm);
			float g = msph[1].getCoefficient(lm);
			float b = msph[2].getCoefficient(lm);
			sph.setCoefficient(lm, (r + g + b) / 3.0f);
			msph[3].setCoefficient(lm, 0.2126f * r + 0.7152f * g + 0.0722f * b);
		}
		return true;
	}

	bool MakeIntensityChannel4f(MultispectralSph4f& msph) {
		unsigned lmmax = msph[0].getMaxCoefficients();

		for (unsigned lm = 0; lm < lmmax; lm++) {
			float r = msph[0].getCoefficient(lm);
			float g = msph[1].getCoefficient(lm);
			float b = msph[2].getCoefficient(lm);
			msph[3].setCoefficient(lm, (r + g + b) / 3.0f);
		}
		return true;
	}

	bool MakeLuminanceChannel4f(MultispectralSph4f& msph) {
		unsigned lmmax = msph[0].getMaxCoefficients();

		for (unsigned lm = 0; lm < lmmax; lm++) {
			float r = msph[0].getCoefficient(lm);
			float g = msph[1].getCoefficient(lm);
			float b = msph[2].getCoefficient(lm);
			msph[3].setCoefficient(lm, 0.2126f * r + 0.7152f * g + 0.0722f * b);
		}
		return true;
	}
} // namespace Fluxions
