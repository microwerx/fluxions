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
#include <fluxions_fileio.hpp>
#include <fluxions_sphl_image_texture.hpp>

namespace Fluxions
{
	bool SphlImageTexture::LoadLightProbe(const std::string& path) {
		FilePathInfo fpi(path);
		if (fpi.DoesNotExist())
			return false;

		if (fpi.ext == ".ppm")
			lightProbe.loadPPM(path);
		else if (fpi.ext == ".exr")
			lightProbe.loadEXR(path);
		else
			Hf::Log.errorfn(__FUNCTION__, "Path %s is not a PPM or EXR", path.c_str());
		lightProbe.convertRectToCubeMap();
		return true;
	}

	bool SphlImageTexture::SphToLightProbe(const MultispectralSph4f& sph) {
		float v_coefs[4][121];

		unsigned maxDegree = sph[0].GetMaxDegree();
		unsigned numCoefs = sph[0].getMaxCoefficients();
		for (unsigned j = 0; j < 4; j++) {
			for (unsigned lm = 0; lm < numCoefs; lm++) {
				v_coefs[j][lm] = sph[j][lm];
			}
			for (unsigned lm = numCoefs; lm < 121; lm++) {
				v_coefs[j][lm] = 0.0f;
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

					color.r = calc_spherical_harmonic<float>((int)maxDegree, v_coefs[0], theta, phi);
					color.g = calc_spherical_harmonic<float>((int)maxDegree, v_coefs[1], theta, phi);
					color.b = calc_spherical_harmonic<float>((int)maxDegree, v_coefs[2], theta, phi);
					color.a = calc_spherical_harmonic<float>((int)maxDegree, v_coefs[3], theta, phi);

					lightProbe.setPixelUnsafe(s, t, face, color);
				}
			}
		}
		return true;
	}

	bool SphlImageTexture::UploadLightProbe() {
		texture.Bind(0);
		for (int i = 0; i < 6; i++) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, (GLsizei)lightProbe.width(), (GLsizei)lightProbe.height(), 0, GL_RGBA, GL_FLOAT, lightProbe.getImageData(i));
		}
		glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
		texture.Unbind();
		return true;
	}
}
