#include <hatchetfish.hpp>
#include <fluxions_sphl_sampler.hpp>

namespace Fluxions {
	SphlSampler::SphlSampler() {
		resize(ix, iy);
	}

	void SphlSampler::resize(unsigned samplesX, unsigned samplesY) {
		ix = samplesX;
		iy = samplesY;
		if (numSamples != ix * iy) {
			numSamples = ix * iy;
			randomVectors.resize(numSamples);
			theta.resize(numSamples);
			phi.resize(numSamples);
			sph.resize(numSamples * 121);
			debug_sides.resize(numSamples);

			if (numSamples == 0)
				return;

			Vector3f avgVector;

			// Create stratified set of samples on the sphere.
			for (unsigned b = 0; b < iy; b++) {
				for (unsigned a = 0; a < ix; a++) {
					float x = 0.0f;

					float y = 0.0f;
					float sampleTheta = 0.0f;
					float samplePhi = 0.0f;
					int method = 1;
					if (method == 0) {
						x = randomSampler(0.0f, 1.0f);
						y = randomSampler(0.0f, 1.0f);
					}
					else if (method == 1) {
						x = randomSampler((a + 0.25f) / ix, (a + 0.75f) / ix);
						y = randomSampler((b + 0.25f) / iy, (b + 0.75f) / iy);
					}
					//double sampleTheta = 2 * asin(sqrt(y));
					//double samplePhi = FX_PI * X;
					samplePhi = FX_F32_TWOPI * x - FX_F32_PI;
					sampleTheta = 2 * asin(sqrt(y)); // FX_PI * X;
					Vector3f v;
					randomVectors[b * ix + a].from_theta_phi(sampleTheta, samplePhi);
				}
			}

			sampleMap.resize(ix * pxscale, iy * pxscale);
			for (unsigned i = 0; i < numSamples; i++) {
				//float rtheta = randomSampler(0.0f, FX_PI);
				//float rphi = randomSampler(0, 8.0 * FX_PI);
				//randomVectors[i].from_theta_phi(rtheta, rphi);
				//randomVectors[i].reset(randomSampler(-1.0f, 1.0f), randomSampler(-1.0f, 1.0f), randomSampler(-1.0f, 1.0f));
				//while (randomVectors[i].lengthSquared() == 0)
				//	randomVectors[i].reset(randomSampler(-1.0f, 1.0f), randomSampler(-1.0f, 1.0f), randomSampler(-1.0f, 1.0f));
				//randomVectors[i].normalize();
				theta[i] = randomVectors[i].theta();
				phi[i] = randomVectors[i].phi();
				avgVector += randomVectors[i];
				float s;
				float t;
				unsigned side;
				MakeFaceSTFromCubeVector(randomVectors[i].x, randomVectors[i].y, randomVectors[i].z, &s, &t, &side);
				debug_sides[i] = side;
			}

			//for (unsigned i = 0; i < 6; i++)
			//{
			//	std::cout << "side " << i << " = " << count(debug_sides.begin(), debug_sides.end(), i) << std::endl;
			//}
			avgVector *= 1.0 / numSamples;
			// std::cout << "Average vector: (" << avgVector.x << ", " << avgVector.y << ", " << avgVector.z << ")" << std::endl;
			for (int l = 0; l <= 10; l++) {
				for (int m = -l; m <= l; m++) {
					for (unsigned i = 0; i < numSamples; i++) {
						unsigned lm = l * (l + 1) + m;
						double sph_value = spherical_harmonic(l, m, theta[i], phi[i]);
						sph[i * 121 + lm] = sph_value;
					}
				}
			}
		}
	}

	void SphlSampler::saveSampleMap(const std::string& path, int pixelScale) {
		Color3i White(255);

		sampleMap.reset();
		sampleMap.resize(ix * pixelScale, iy * pixelScale);
		for (unsigned i = 0; i < numSamples; i++) {
			int px = (int)(ix * pixelScale * (phi[i] / FX_PI));
			int py = (int)(iy * pixelScale * (theta[i] + FX_PI) / FX_TWOPI);

			if (px >= 0 && py >= 0)
				sampleMap.setPixel(px, py, White);
		}
		// NOTE: we changed from savePPMi
		// sampleMap.savePPMi(path, 1.0f, 0, 255);
		HFLOGCHECKWARN(sampleMap.savePPM(path));
	}

	void SphlSampler::sampleCubeMap(const Image4f& cubeMap, MultispectralSph4f& msph) {
		if (cubeMap.empty() || !cubeMap.IsCubeMap())
			return;

		unsigned numCoefs = msph[0].getMaxCoefficients();
		for (unsigned lm = 0; lm < numCoefs; lm++) {
			Color4f coef;
			float pixelArea = (4.0f * FX_F32_PI) / numSamples;
			for (unsigned i = 0; i < numSamples; i++) {
				Color4f texel = cubeMap.getPixelCubeMap(randomVectors[i]);
				coef += texel * (float)sph[i * 121 + lm];
			}
			coef *= pixelArea;

			float r = coef.r;
			float g = coef.g;
			float b = coef.b;
			msph[0].setCoefficient(lm, r);
			msph[1].setCoefficient(lm, g);
			msph[2].setCoefficient(lm, b);
			msph[3].setCoefficient(lm, 0.2126f * r + 0.7152f * g + 0.0722f * b);
		}
	}
} // namespace Fluxions
