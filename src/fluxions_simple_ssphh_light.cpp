// SSPHH/Fluxions/Unicornfish/Viperfish/Hatchetfish/Sunfish/KASL/GLUT Extensions
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
#include "stdafx.h"
#include <fluxions_sphl_sampler.hpp>

namespace Fluxions
{
bool SphlImageTexture::LoadLightProbe(const string &path)
{
    FilePathInfo fpi(path);
    if (fpi.DoesNotExist())
        return false;

    lightProbe.loadPPM(path);
    lightProbe.convertRectToCubeMap();
    return true;
}

bool SphlImageTexture::SphToLightProbe(const MultispectralSph4f &sph)
{
    float v_coefs[4][121];

    int maxDegree = sph[0].GetMaxDegree();
    int numCoefs = sph[0].getMaxCoefficients();
    for (int j = 0; j < 4; j++)
    {
        for (int lm = 0; lm < numCoefs; lm++)
        {
            v_coefs[j][lm] = sph[j][lm];
        }
        for (int lm = numCoefs; lm < 121; lm++)
        {
            v_coefs[j][lm] = 0.0f;
        }
    }

    lightProbe.resize(32, 32, 6);
    for (int face = 0; face < lightProbe.depth(); face++)
    {
        for (int s = 0; s < lightProbe.width(); s++)
        {
            for (int t = 0; t < lightProbe.height(); t++)
            {
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

bool SphlImageTexture::UploadLightProbe()
{
    texture.Bind(0);
    for (int i = 0; i < 6; i++)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, lightProbe.width(), lightProbe.height(), 0, GL_RGBA, GL_FLOAT, lightProbe.getImageData(i));
    }
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    texture.Unbind();
    return true;
}

SimpleSSPHHLight::SimpleSSPHHLight()
{
    //lightProbe_hier.resize(32, 32, 6);
}

SimpleSSPHHLight::~SimpleSSPHHLight()
{
}

void SimpleSSPHHLight::MakeDirty()
{
    dirty = true;
}

void SimpleSSPHHLight::ChangeDegrees(int degrees)
{
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

void SimpleSSPHHLight::ResizeHierarchies(int size)
{
    if (size < 0 || size > MaxSphlLights)
        return;

    vizgenLightProbes.resize(size);
}

void SimpleSSPHHLight::Randomize(float size)
{
    for (int l = 0; l <= maxDegree; l++)
    {
        for (int m = -l; m <= l; m++)
        {
            msph[0].setCoefficient(l, m, randomSampler(-size, size));
            msph[1].setCoefficient(l, m, randomSampler(-size, size));
            msph[2].setCoefficient(l, m, randomSampler(-size, size));
            msph[3].setCoefficient(l, m, randomSampler(-size, size));
        }
    }
    MakeDirty();
}

// Reads from a Corona Light Probe (a cube map stored images from left to right in a single image).
bool SimpleSSPHHLight::ReadCoronaLightProbe(const string &path)
{
    FilePathInfo fpi(path);
    if (fpi.DoesNotExist())
        return false;

    lightProbe_corona.loadPPM(path);
    lightProbe_corona.convertRectToCubeMap();
    double intensity = lightProbe_corona.getIntensity();
    double numPixels = lightProbe_corona.getNumPixels();
    coronaLightProbePercent = (float)(intensity / numPixels);
    ChangeDegrees(MaxSphlDegree);

    LightProbeToSph(lightProbe_corona, msph);
    SphToLightProbe(msph, lightProbe_sph);

    UploadLightProbe(lightProbe_corona, coronaLightProbeTexture);
    UploadLightProbe(lightProbe_sph, sphLightProbeTexture);
    // UploadLightProbe(lightProbe_hier, hierLightProbeTexture);

    dirty = true;

    return true;
}

// Saves to a Corona Light Probe (a cube map with cube faces stored from left to right in a single image).
bool SimpleSSPHHLight::SaveCoronaLightProbe(const string &path)
{
    FilePathInfo fpi(path);

    SphToLightProbe(msph, lightProbe_sph);
    lightProbe_sph.convertCubeMapToRect();
    lightProbe_sph.savePPMi(path, 255.99f, 0, 255, 0, false);

    return false;
}

// Saves a JSON form of the multispectral (RGBL) of this SPH. L represents a monochromatic version of the RGB components. { maxDegree: (1-10), coefs : [] }
bool SimpleSSPHHLight::SaveJsonSph(const string &path)
{
    FilePathInfo fpi(path);
    KASL::JSONPtr json = KASL::JSON::MakeObject({{"maxDegree", KASL::JSON::MakeNumber((int)msph[0].GetMaxDegree())},
                                                 {"coefs", KASL::JSON::MakeArray()}});

    auto coefs = json->getMember("coefs");
    for (int j = 0; j < 4; j++)
    {
        vector<float> coef_f = msph[j].getCoefficients();
        KASL::JSONPtr jsonArray = KASL::JSON::MakeArray(coef_f);
        coefs->PushBack(jsonArray);
    }

    ofstream fout(fpi.path);
    fout << json->Serialize();
    fout.close();

    return true;
}

// Reads a JSON format of a multispectral (RGBL) of this SPH. L represents a monochromatic version of the RGB components. { maxDegree: (1-10), coefs : [] }
bool SimpleSSPHHLight::ReadJsonSph(const string &path)
{
    FilePathInfo fpi(path);
    if (fpi.DoesNotExist())
        return false;
    ifstream fin(fpi.path);
    string buffer;
    while (!fin)
    {
        string line;
        getline(fin, line);
        buffer += "\n";
        buffer += line;
    }
    fin.close();

    KASL::JSONPtr json = KASL::JSON::New();
    if (!json->Deserialize(buffer))
    {
        hflog.error("Unable to read JSON SPH");
        return false;
    }

    if (json->IsObject() && json->getMember("maxDegree")->IsNumber() && json->getMember("coefs")->IsArray() && json->getMember("coefs")->Size() == 4 && json->getMember("coefs")->getElement(0)->IsArray() && json->getMember("coefs")->getElement(1)->IsArray() && json->getMember("coefs")->getElement(2)->IsArray() && json->getMember("coefs")->getElement(3)->IsArray())
    {
        // int jsonMaxDegree = json->getMember("maxDegree")->AsInt();
        KASL::JSONPtr coefs = json->getMember("coefs");
        for (size_t j = 0; j < 4; j++)
        {
            KASL::JSONPtr e = coefs->getElement(j);
            if (e->IsArray())
            {
                vector<float> coefJ;
                coefJ = e->AsFloatArray();
            }
        }
    }
    return false;
}

bool SimpleSSPHHLight::LightProbeToSph(const Image4f &lightProbe, MultispectralSph4f &sph)
{
    SphlSampler sphlSampler;
    sphlSampler.resize(64, 32);
    sphlSampler.sampleCubeMap(lightProbe, sph);
    MakeIntensityChannel4f(sph);
    return false;
}

bool SimpleSSPHHLight::SphToLightProbe(const MultispectralSph4f &sph, Image4f &lightProbe)
{
    float v_coefs[4][121];

    for (int j = 0; j < 4; j++)
    {
        for (int l = 0; l <= maxDegree; l++)
        {
            for (int m = -l; m <= l; m++)
            {
                auto lm = sph[j].getCoefficientIndex(l, m);
                v_coefs[j][lm] = E0 * sph[j][lm];
            }
        }
    }

    lightProbe.resize(32, 32, 6);
    for (int face = 0; face < lightProbe.depth(); face++)
    {
        for (int s = 0; s < lightProbe.width(); s++)
        {
            for (int t = 0; t < lightProbe.height(); t++)
            {
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

bool SimpleSSPHHLight::SphToLightProbe(const MultispectralSph4f &sph, Image4f &lightProbe, int maxDegree_)
{
    float v_coefs[4][121];

    maxDegree_ = min(maxDegree_, this->maxDegree);

    for (int j = 0; j < 4; j++)
    {
        for (int l = 0; l <= maxDegree_; l++)
        {
            for (int m = -l; m <= l; m++)
            {
                auto lm = sph[j].getCoefficientIndex(l, m);
                v_coefs[j][lm] = E0 * sph[j][lm];
            }
        }
    }

    lightProbe.resize(32, 32, 6);
    for (int face = 0; face < lightProbe.depth(); face++)
    {
        for (int s = 0; s < lightProbe.width(); s++)
        {
            for (int t = 0; t < lightProbe.height(); t++)
            {
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

bool SimpleSSPHHLight::UploadLightProbe(Image4f &lightProbe, SimpleGpuTexture &texture)
{
    if (lightProbe.empty())
        lightProbe.resize(32, 32, 6);

    texture = SimpleGpuTexture(GL_TEXTURE_CUBE_MAP);
    texture.Bind(0);
    for (int i = 0; i < 6; i++)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, lightProbe.width(), lightProbe.height(), 0, GL_RGBA, GL_FLOAT, lightProbe.getImageData(i));
    }
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    texture.Unbind();
    return true;
}

bool SimpleSSPHHLight::UploadLightProbe(Image4f &lightProbe, GLuint &texture)
{
    if (lightProbe.empty())
        lightProbe.resize(32, 32, 6);

    if (texture == 0)
    {
        glGenTextures(1, &texture);
    }
    //texture = SimpleGpuTexture(GL_TEXTURE_CUBE_MAP);
    //texture.Bind(0);
    glutDebugBindTexture(GL_TEXTURE_CUBE_MAP, texture);
    for (int i = 0; i < 6; i++)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, lightProbe.width(), lightProbe.height(), 0, GL_RGBA, GL_FLOAT, lightProbe.getImageData(i));
    }
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    //texture.Unbind();
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    return true;
}

Color3f SimpleSSPHHLight::GetCoefficientColor(int l, int m) const
{
    return Color3f(
        GetCoefficient(0, l, m),
        GetCoefficient(1, l, m),
        GetCoefficient(2, l, m));
}

float SimpleSSPHHLight::GetCoefficient(int j, int l, int m) const
{
    if (j < 0 || j > 4)
        return 0.0f;
    return msph[j].getCoefficient(l, m);
}

float SimpleSSPHHLight::GetCoefficient(int j, int lm) const
{
    if (!within(j, 0, 3))
        return 0.0f;
    return msph[j].getCoefficient(lm);
}

void SimpleSSPHHLight::ZeroCoefficients()
{
    SetCoefficient(-1000, -1000, -1000, 0.0f);
}

void SimpleSSPHHLight::IsolateCoefficient(int j, int l, int m)
{
    float value = GetCoefficient(j, l, m);
    SetCoefficient(-1000, -1000, -1000, 0.0f);
    SetCoefficient(j, l, m, value);
}

void SimpleSSPHHLight::SetCoefficient(int j, int l, int m, float value)
{
    if (j > 4)
        return;

    int minJ = 0;
    int maxJ = 0;
    int minL = 0;
    int maxL = 0;
    int minM = 0;
    int maxM = 0;

    if (j < 0)
    {
        minJ = 0;
        maxJ = 4;
    }
    else
    {
        minJ = j;
        maxJ = j + 1;
    }

    if (l < 0)
    {
        minL = 0;
        maxL = maxDegree;
        m = -maxL - 1;
    }
    else
    {
        minL = l;
        maxL = l + 1;
    }
    for (int _j = minJ; _j < maxJ; _j++)
    {
        for (int _l = minL; _l <= maxL; _l++)
        {
            if (m < -_l)
            {
                minM = -_l;
                maxM = _l;
            }
            else
            {
                minM = m;
                maxM = m;
            }
            for (int _m = minM; _m <= maxM; _m++)
            {
                msph[_j].setCoefficient(_l, _m, value);
            }
        }
    }
    dirty = true;
}

void SimpleSSPHHLight::RandomizeCoefficient(int j, int l, int m, float minValue, float maxValue)
{
    if (j > 4)
        return;

    int minJ = 0;
    int maxJ = 0;
    int minL = 0;
    int maxL = 0;
    int minM = 0;
    int maxM = 0;

    if (j < 0)
    {
        minJ = 0;
        maxJ = 4;
    }
    else
    {
        minJ = j;
        maxJ = j + 1;
    }

    if (l < 0)
    {
        minL = 0;
        maxL = maxDegree;
    }
    else
    {
        minL = l;
        maxL = l + 1;
    }
    for (int _j = minJ; _j < maxJ; _j++)
    {
        for (int _l = minL; _l <= maxL; _l++)
        {
            if (m < 0)
            {
                minM = -_l;
                maxM = _l;
            }
            else
            {
                minM = m;
                maxM = m;
            }
            for (int _m = minM; _m <= maxM; _m++)
            {
                msph[_j].setCoefficient(_l, _m, randomSampler(minValue, maxValue));
            }
        }
    }

    dirty = true;
}

void SimpleSSPHHLight::SortHierarchiesByPercent()
{
    return;

    if (index < 0)
        throw "SSPHHLight not properly initialized";
    //int selfIndex = index;
    //sort(hierarchies.begin(), hierarchies.end(),
    //	[selfIndex](const SSPHHLightHierarchy & a, const SSPHHLightHierarchy & b)
    //{
    //	// cause self to float to node 0
    //	if (a.index == selfIndex)
    //		return true;
    //	if (b.index == selfIndex)
    //		return false;
    //	return b.percentVisible < a.percentVisible;
    //});
}

void SimpleSSPHHLight::SortHierarchiesByIndex()
{
    return;

    if (index < 0)
        throw "SSPHHLight not properly initialized";
    //int selfIndex = index;
    //sort(hierarchies.begin(), hierarchies.end(),
    //	[selfIndex](const SSPHHLightHierarchy & a, const SSPHHLightHierarchy & b)
    //{
    //	//// cause self to float to node 0
    //	//if (a.index == selfIndex)
    //	//	return true;
    //	//if (b.index == selfIndex)
    //	//	return false;
    //	return a.index < b.index;
    //});
}

void SimpleSSPHHLight::CopySphlToHierarchies()
{
    //if (index < 0)
    //	throw "SSPHHLight not properly initialized";
    //hierarchies[index].CopySph(msph, 1.0);
}

void SimpleSSPHHLight::SetHierarchyDescriptionToIndex()
{
    //ostringstream ostr;
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

void SimpleSSPHHLight::SetHierarchyDescriptionToPercent()
{
    //ostringstream ostr;
    //int i = 0;
    //ostr << "(" << hierarchies.size() << ") => [ ";
    //for (auto & hier : hierarchies)
    //{
    //	if (i != 0)
    //		ostr << ", ";
    //	ostr << fixed << setprecision(2) << (hier.percentVisible * 100.0) << "%";
    //	i++;
    //}
    //ostr << " ]";
    //hier_description = ostr.str();
}

void SimpleSSPHHLight::SetHierarchyDescription()
{
    //ostringstream ostr;
    //int i = 0;
    //ostr << "(" << index << "," << hierarchies.size() << ") => [ ";
    //for (auto & hier : hierarchies)
    //{

    //	float weight = 0.0;
    //	weight += hier.sph.sum();

    //	if (i != 0)
    //		ostr << ", ";
    //	ostr << "(" << hier.index << ", " << fixed << setprecision(2) << (hier.percentVisible * 100.0) << "%, " << weight << ")";
    //	i++;
    //}
    //ostr << " ]";
    //hier_description = ostr.str();
}

bool MakeStandardizedSph(SphericalHarmonicf &sph, MultispectralSph4f &msph)
{
    for (size_t i = 0; i < 4; i++)
    {
        if (sph.GetMaxDegree() != msph[i].GetMaxDegree())
            return false;
    }
    auto lmmax = sph.getMaxCoefficients();

    for (size_t lm = 0; lm < lmmax; lm++)
    {
        float r = msph[0].getCoefficient(lm);
        float g = msph[1].getCoefficient(lm);
        float b = msph[2].getCoefficient(lm);
        sph.setCoefficient(lm, (r + g + b) / 3.0f);
        msph[3].setCoefficient(lm, 0.2126f * r + 0.7152f * g + 0.0722f * b);
    }
    return true;
}

bool MakeIntensityChannel4f(MultispectralSph4f &msph)
{
    size_t lmmax = msph[0].getMaxCoefficients();

    for (size_t lm = 0; lm < lmmax; lm++)
    {
        float r = msph[0].getCoefficient(lm);
        float g = msph[1].getCoefficient(lm);
        float b = msph[2].getCoefficient(lm);
        msph[3].setCoefficient(lm, (r + g + b) / 3.0f);
    }
    return true;
}

bool MakeLuminanceChannel4f(MultispectralSph4f &msph)
{
    size_t lmmax = msph[0].getMaxCoefficients();

    for (size_t lm = 0; lm < lmmax; lm++)
    {
        float r = msph[0].getCoefficient(lm);
        float g = msph[1].getCoefficient(lm);
        float b = msph[2].getCoefficient(lm);
        msph[3].setCoefficient(lm, 0.2126f * r + 0.7152f * g + 0.0722f * b);
    }
    return true;
}

////////////////////////////////////////////////////////////////////////////
// SSPHH Algorithm /////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

SimpleSSPHH::SimpleSSPHH()
{
}

SimpleSSPHH::~SimpleSSPHH()
{
}

void SimpleSSPHH::INIT(SimpleSceneGraph &ssg)
{
    sceneName = ssg.name;
    sphls_ = &ssg.ssphhLights;
    size_ = sphls_->size();
    S.resize(size_);
    for (auto &S_i : S)
    {
        S_i.resize(MaxSphlDegree, 0.0f);
    }
    H.resize(size_);
    for (auto &h_i : H)
    {
        h_i.resize(size_);
        for (auto &h_ij : h_i)
        {
            h_ij.resize(MaxSphlDegree, 0.0f);
        }
    }
    P.resize(size_);
    for (auto &p : P)
    {
        p.resize(size_, 0.0f);
    }
    Q.resize(size_);
    for (size_t i = 0; i < size_; i++)
    {
        Q[i].index = i;
    }
    Qsorted.resize(size_);
    Sprime.resize(size_);
    for (auto &S_i : Sprime)
    {
        S_i.resize(MaxSphlDegree);
    }
    self.resize(size_);
    for (auto &s : self)
    {
        s.resize(MaxSphlDegree);
    }
    neighbor.resize(size_);
    for (auto &n : neighbor)
    {
        n.resize(MaxSphlDegree);
    }
}

void SimpleSSPHH::GEN()
{
    if (!sphls_)
        return;
    auto &sphls = *sphls_;

    int i = 0;
    for (auto &sphl : *sphls_)
    {
        S[i].resize(sphl.maxDegree);
        sphl.LightProbeToSph(sphl.vizgenLightProbes[i], S[i].msph);
        H[i][i] = S[i];
        P[i][i] = 1.0f;

        float lm0, lm1, lm2, lm3;
        if (sphl.maxDegree >= 1)
        {
            lm0 = S[i].a().getCoefficient(0);
            lm1 = S[i].a().getCoefficient(1);
            lm2 = S[i].a().getCoefficient(2);
            lm3 = S[i].a().getCoefficient(3);
        }
        else
        {
            lm0 = -1.0f;
            lm1 = -1.0f;
            lm2 = -1.0f;
            lm3 = -1.0f;
        }
        hflog.info("%s(): (%d, %d) -> [ %.2f, %.2f, %.2f, %.2f ]", __FUNCTION__, i, i, lm0, lm1, lm2, lm3);
        i++;
    }
}

void SimpleSSPHH::VIZ()
{
    if (!sphls_)
        return;
    auto &sphls = *sphls_;

    for (size_t i = 0; i < size_; i++)
    {
        auto &sphl = sphls[i];
        for (size_t j = 0; j < size_; j++)
        {
            if (i == j)
                continue;
            if (sphl.vizgenLightProbes.empty())
            {
                hflog.error("%s(): VIZ() called with no light probes!", __FUNCTION__);
                continue;
            }

            H[i][j].resize(sphl.maxDegree);
            sphl.LightProbeToSph(sphl.vizgenLightProbes[j], H[i][j].msph);

            string basename = CoronaJob::MakeVIZName(sceneName, i, j);
            if (savePPMs)
            {
                Image4f lightProbe(32, 32, 6);
                sphl.SphToLightProbe(H[i][j].msph, lightProbe);
                lightProbe.convertCubeMapToRect();
                lightProbe.savePPMi(basename + "_sph.ppm", 255.99f, 0, 255);
            }
            if (saveJSONs)
                H[i][j].SaveJSON(basename + ".json");

            // TODO: calculate visibility percentage
            P[i][j] = 0.25f;

            float lm0, lm1, lm2, lm3;
            if (sphl.maxDegree >= 1)
            {
                lm0 = H[i][j].a().getCoefficient(0);
                lm1 = H[i][j].a().getCoefficient(1);
                lm2 = H[i][j].a().getCoefficient(2);
                lm3 = H[i][j].a().getCoefficient(3);
            }
            else
            {
                lm0 = -1.0f;
                lm1 = -1.0f;
                lm2 = -1.0f;
                lm3 = -1.0f;
            }
            hflog.info("%s(): (%d, %d) -> [ %.2f, %.2f, %.2f, %.2f ]", __FUNCTION__, i, j, lm0, lm1, lm2, lm3);
        }
    }
    return;
}

void SimpleSSPHH::HIER(bool includeSelf, bool includeNeighbor, int MaxDegrees)
{
    if (!sphls_)
        return;
    auto &sphls = *sphls_;
    for (size_t i = 0; i < size_; i++)
    {
        for (size_t j = 0; j < size_; j++)
        {
            Q[j].index = j;
            Q[j].p = P[i][j];
        }
        Qsorted = Q;
        sort(Qsorted.begin(), Qsorted.end(), [](Qpair &a, Qpair &b) {
            if (a.p > b.p)
                return true;
            return false;
        });
        Sprime[i].reset();
        self[i].reset();
        neighbor[i].reset();
        for (size_t j = 0; j < size_; j++)
        {
            assert(Q[j].index >= 0);
            size_t Qj_index = (size_t)Q[j].index;
            if (includeNeighbor && Qj_index != i)
                Sprime[i].Accumulate(H[i][j], S[Qj_index], MaxDegrees);
            if (includeSelf && Qj_index == i)
                Sprime[i].Accumulate(S[Qj_index], Q[j].p, MaxDegrees);

            if (Qj_index == i)
            {
                self[i].Accumulate(S[Qj_index], Q[j].p, MaxDegrees);
            }
            else
            {
                neighbor[i].Accumulate(H[i][j], S[Qj_index], MaxDegrees);
            }
            float lm0, lm1, lm2, lm3;
            if (H[i][j].a().GetMaxDegree() >= 1)
            {
                lm0 = Sprime[i].a().getCoefficient(0);
                lm1 = Sprime[i].a().getCoefficient(1);
                lm2 = Sprime[i].a().getCoefficient(2);
                lm3 = Sprime[i].a().getCoefficient(3);
            }
            else
            {
                lm0 = -1.0f;
                lm1 = -1.0f;
                lm2 = -1.0f;
                lm3 = -1.0f;
            }
            // hflog.info("%s(): (%d, %d) S' += H * %.2f -> [ %.2f, %.2f, %.2f, %.2f ]", __FUNCTION__, i, j, Q[j].p, lm0, lm1, lm2, lm3);
        }
    }

    for (size_t i = 0; i < size_; i++)
    {
        sphls[i].self = self[i];
        sphls[i].neighbor = neighbor[i];
        sphls[i].SphToLightProbe(Sprime[i].msph, sphls[i].lightProbe_hier);
        sphls[i].UploadLightProbe(sphls[i].lightProbe_hier, sphls[i].hierLightProbeTexture);

        string base = CoronaJob::MakeHIERName(sceneName, i, MaxDegrees);

        if (savePPMs)
        {
            Image4f lightProbeSprime;
            sphls[i].SphToLightProbe(Sprime[i].msph, lightProbeSprime, MaxDegrees);
            lightProbeSprime.convertCubeMapToRect();
            lightProbeSprime.savePPMi(base + "_01_Sprime.ppm", 255.99f, 0, 0);
        }
        if (saveJSONs)
            Sprime[i].SaveJSON(base + "_01_Sprime.json");

        if (savePPMs)
        {
            Image4f lightProbeSelf;
            sphls[i].SphToLightProbe(self[i].msph, lightProbeSelf, MaxDegrees);
            lightProbeSelf.convertCubeMapToRect();
            lightProbeSelf.savePPMi(base + "_02_self.ppm", 255.99f, 0, 0);
        }
        if (saveJSONs)
            self[i].SaveJSON(base + "_02_self.json");

        if (savePPMs)
        {
            Image4f lightProbeNeighbor;
            sphls[i].SphToLightProbe(neighbor[i].msph, lightProbeNeighbor, MaxDegrees);
            lightProbeNeighbor.convertCubeMapToRect();
            lightProbeNeighbor.savePPMi(base + "_03_neighbor.ppm", 255.99f, 0, 0);
        }
        if (saveJSONs)
            neighbor[i].SaveJSON(base + "_03_neighbor.json");

        float lm0, lm1, lm2, lm3;
        if (Sprime[i].a().GetMaxDegree() >= 1)
        {
            lm0 = Sprime[i].a().getCoefficient(0);
            lm1 = Sprime[i].a().getCoefficient(1);
            lm2 = Sprime[i].a().getCoefficient(2);
            lm3 = Sprime[i].a().getCoefficient(3);
        }
        else
        {
            lm0 = -1.0f;
            lm1 = -1.0f;
            lm2 = -1.0f;
            lm3 = -1.0f;
        }
        hflog.info("%s(): S'_%d -> [ %.2f, %.2f, %.2f, %.2f ]", __FUNCTION__, i, lm0, lm1, lm2, lm3);
    }
    return;
}
} // namespace Fluxions