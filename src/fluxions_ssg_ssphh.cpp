#include "fluxions_ssg_pch.hpp"
#include <fluxions_ssg_ssphh.hpp>
#include <fluxions_ssg_ssphh_renderer_plugin.hpp>
#include <fluxions_ssphh_utilities.hpp>
#include <cassert>

namespace Fluxions {
	////////////////////////////////////////////////////////////////////////////
	// SSPHH Algorithm /////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////

	SimpleSSPHH::SimpleSSPHH() {}

	SimpleSSPHH::~SimpleSSPHH() {}

	void SimpleSSPHH::INIT(SimpleSceneGraph& ssg) {
		SSG_SSPHHRendererPlugin* ssphh = dynamic_cast<SSG_SSPHHRendererPlugin*>(ssg.userdata);
		if (!ssphh) {
			HFLOGERROR("ssphh pointer is nullptr");
		}
		HFLOGINFO("SSPHH INIT");
		sceneName = ssg.name_str();
		sphls_ = &ssphh->ssphhLights;
		size_ = sphls_->size();
		S.resize(size_);
		for (auto& S_i : S) {
			S_i.resize(MaxSphlDegree, 0.0f);
		}
		H.resize(size_);
		for (auto& h_i : H) {
			h_i.resize(size_);
			for (auto& h_ij : h_i) {
				h_ij.resize(MaxSphlDegree, 0.0f);
			}
		}
		P.resize(size_);
		for (auto& p : P) {
			p.resize(size_, 0.0f);
		}
		Q.resize(size_);
		for (size_t i = 0; i < size_; i++) {
			Q[i].index = (int)i;
		}
		Qsorted.resize(size_);
		Sprime.resize(size_);
		for (auto& S_i : Sprime) {
			S_i.resize(MaxSphlDegree);
		}
		self.resize(size_);
		for (auto& s : self) {
			s.resize(MaxSphlDegree);
		}
		neighbor.resize(size_);
		for (auto& n : neighbor) {
			n.resize(MaxSphlDegree);
		}
	}

	void SimpleSSPHH::GEN() {
		if (!sphls_)
			return;
		HFLOGINFO("SSPHH GEN");
		//auto &sphls = *sphls_;

		int i = 0;
		for (auto& sphl : *sphls_) {
			S[i].resize(sphl.maxDegree);
			sphl.lightProbeToSph(sphl.vizgenLightProbes[i], S[i].msph);
			H[i][i] = S[i];
			P[i][i] = 1.0f;

			float lm0, lm1, lm2, lm3;
			if (sphl.maxDegree >= 1) {
				lm0 = S[i].a().getCoefficient(0);
				lm1 = S[i].a().getCoefficient(1);
				lm2 = S[i].a().getCoefficient(2);
				lm3 = S[i].a().getCoefficient(3);
			}
			else {
				lm0 = -1.0f;
				lm1 = -1.0f;
				lm2 = -1.0f;
				lm3 = -1.0f;
			}

			std::string basename = "output/" + MakeGENName(sceneName, (int)i);
			if (savePPMs) {
				Image4f lightProbe(32, 32, 6);
				sphl.sphToLightProbe(H[i][i].msph, lightProbe);
				lightProbe.convertCubeMapToRect();
				lightProbe.savePPMi(basename + "_sph.ppm", 255.99f, 0, 255);
				lightProbe.saveEXR(basename + "_sph.exr");
			}
			if (saveJSONs) {
				H[i][i].SaveJSON(basename + ".json", basename, sphl.position);
			}
			HFLOGINFO("(%d, %d) -> [ %.2f, %.2f, %.2f, %.2f ]", i, i, lm0, lm1, lm2, lm3);
			i++;
		}
	}

	void SimpleSSPHH::VIZ() {
		if (!sphls_)
			return;
		HFLOGINFO("SSPHH VIZ");

		auto& sphls = *sphls_;

		for (size_t i = 0; i < size_; i++) {
			auto& sphl = sphls[i];
			for (size_t j = 0; j < size_; j++) {
				if (i == j)
					continue;
				if (sphl.vizgenLightProbes.empty()) {
					HFLOGERROR("VIZ() called with no light probes!");
					continue;
				}

				H[i][j].resize(sphl.maxDegree);
				sphl.lightProbeToSph(sphl.vizgenLightProbes[j], H[i][j].msph);

				std::string basename = "output/" + MakeVIZName(sceneName, (int)i, (int)j);
				if (savePPMs) {
					Image4f lightProbe(32, 32, 6);
					sphl.sphToLightProbe(H[i][j].msph, lightProbe);
					lightProbe.convertCubeMapToRect();
					lightProbe.savePPMi(basename + "_sph.ppm", 255.99f, 0, 255);
					lightProbe.saveEXR(basename + "_sph.exr");
				}
				if (saveJSONs)
					H[i][j].SaveJSON(basename + ".json", basename, sphl.position);

				// TODO: calculate visibility percentage
				P[i][j] = 0.25f;

				float lm0, lm1, lm2, lm3;
				if (sphl.maxDegree >= 1) {
					lm0 = H[i][j].a().getCoefficient(0);
					lm1 = H[i][j].a().getCoefficient(1);
					lm2 = H[i][j].a().getCoefficient(2);
					lm3 = H[i][j].a().getCoefficient(3);
				}
				else {
					lm0 = -1.0f;
					lm1 = -1.0f;
					lm2 = -1.0f;
					lm3 = -1.0f;
				}
				HFLOGINFO("(%d, %d) -> [ %.2f, %.2f, %.2f, %.2f ]", i, j, lm0, lm1, lm2, lm3);
			}
		}
		return;
	}

	void SimpleSSPHH::HIER(bool includeSelf, bool includeNeighbor, int MaxDegrees) {
		if (!sphls_)
			return;
		HFLOGINFO("SSPHH HIER");

		auto& sphls = *sphls_;
		for (size_t i = 0; i < size_; i++) {
			for (size_t j = 0; j < size_; j++) {
				Q[j].index = (int)j;
				Q[j].p = P[i][j];
			}
			Qsorted = Q;
			sort(Qsorted.begin(), Qsorted.end(), [](Qpair& a, Qpair& b) {
				if (a.p > b.p)
					return true;
				return false;
				 });
			Sprime[i].reset();
			self[i].reset();
			neighbor[i].reset();
			for (size_t j = 0; j < size_; j++) {
				assert(Q[j].index >= 0);
				size_t Qj_index = (size_t)Q[j].index;
				if (includeNeighbor && Qj_index != i)
					Sprime[i].Accumulate(H[i][j], S[Qj_index], VIZmix, MaxDegrees);
				if (includeSelf && Qj_index == i)
					Sprime[i].Accumulate(S[Qj_index], Q[j].p, MaxDegrees);

				if (Qj_index == i) {
					self[i].Accumulate(S[Qj_index], Q[j].p, MaxDegrees);
				}
				else {
					neighbor[i].Accumulate(H[i][j], S[Qj_index], MaxDegrees);
				}
				float lm0, lm1, lm2, lm3;
				if (H[i][j].a().GetMaxDegree() >= 1) {
					lm0 = Sprime[i].a().getCoefficient(0);
					lm1 = Sprime[i].a().getCoefficient(1);
					lm2 = Sprime[i].a().getCoefficient(2);
					lm3 = Sprime[i].a().getCoefficient(3);
				}
				else {
					lm0 = -1.0f;
					lm1 = -1.0f;
					lm2 = -1.0f;
					lm3 = -1.0f;
				}
				// HFLOGERROR("(%d, %d) S' += H * %.2f -> [ %.2f, %.2f, %.2f, %.2f ]", i, j, Q[j].p, lm0, lm1, lm2, lm3);
			}
		}

		for (size_t i = 0; i < size_; i++) {
			sphls[i].self = self[i];
			sphls[i].neighbor = neighbor[i];
			sphls[i].sphToLightProbe(Sprime[i].msph, sphls[i].hierLightProbeImage);
			sphls[i].uploadLightProbe(sphls[i].hierLightProbeImage, sphls[i].hierLightProbeTexture);

			std::string base = "output/" + MakeHIERName(sceneName, (int)i, (int)MaxDegrees);

			if (savePPMs) {
				Image4f lightProbeSprime;
				sphls[i].sphToLightProbe(Sprime[i].msph, lightProbeSprime, MaxDegrees);
				lightProbeSprime.convertCubeMapToRect();
				lightProbeSprime.savePPMi(base + "_01_Sprime.ppm", 255.99f, 0, 0);
				lightProbeSprime.saveEXR(base + "_01_Sprime.exr");
			}
			if (saveJSONs) {
				std::string basename = base + "_01_Sprime";
				Sprime[i].SaveJSON(base + "_01_Sprime.json", basename, sphls[i].position);
			}

			if (savePPMs) {
				Image4f lightProbeSelf;
				sphls[i].sphToLightProbe(self[i].msph, lightProbeSelf, MaxDegrees);
				lightProbeSelf.convertCubeMapToRect();
				lightProbeSelf.savePPMi(base + "_02_self.ppm", 255.99f, 0, 0);
				lightProbeSelf.saveEXR(base + "_02_self.exr");
			}
			if (saveJSONs) {
				std::string basename = base + "_02_self";
				self[i].SaveJSON(base + "_02_self.json", basename, sphls[i].position);
			}

			if (savePPMs) {
				Image4f lightProbeNeighbor;
				sphls[i].sphToLightProbe(neighbor[i].msph, lightProbeNeighbor, MaxDegrees);
				lightProbeNeighbor.convertCubeMapToRect();
				lightProbeNeighbor.savePPMi(base + "_03_neighbor.ppm", 255.99f, 0, 0);
				lightProbeNeighbor.saveEXR(base + "_03_neighbor.exr");
			}
			if (saveJSONs) {
				std::string basename = base + "_03_neighbor";
				neighbor[i].SaveJSON(base + "_03_neighbor.json", basename, sphls[i].position);
			}

			float lm0, lm1, lm2, lm3;
			if (Sprime[i].a().GetMaxDegree() >= 1) {
				lm0 = Sprime[i].a().getCoefficient(0);
				lm1 = Sprime[i].a().getCoefficient(1);
				lm2 = Sprime[i].a().getCoefficient(2);
				lm3 = Sprime[i].a().getCoefficient(3);
			}
			else {
				lm0 = -1.0f;
				lm1 = -1.0f;
				lm2 = -1.0f;
				lm3 = -1.0f;
			}
			HFLOGINFO("S'_%d -> [ %.2f, %.2f, %.2f, %.2f ]", i, lm0, lm1, lm2, lm3);
		}
		return;
	}
} // namespace Fluxions
