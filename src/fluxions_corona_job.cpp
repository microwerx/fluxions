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
#include <fluxions_corona_scene_file.hpp>
#include <fluxions_fileio.hpp>
#include <fluxions_xml.hpp>

namespace Fluxions
{

	/////////////////////////////////////////////////////////////////////
	// CoronaJob ////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////

	CoronaJob::CoronaJob(const std::string &name, Type jobtype, int arg1, int arg2)
	{
		scene_name = name;
		scene_path = name + ".scn";
		output_path_exr = name + ".exr";
		output_path_ppm = name + ".ppm";
		output_path_png = name + ".png";
		conf_path = name + ".conf";
		hq_output_path_exr = name + "_hq.exr";
		hq_output_path_ppm = name + "_hq.ppm";
		hq_conf_path = name + "_hq.conf";

		type = jobtype;

		switch (jobtype)
		{
		case Type::GEN:
			sendLight = SphlSunIndex;
			recvLight = arg1;
			conf_path = "sphlgen.conf";
			hq_conf_path = "sphlgen_hq.conf";
			break;
		case Type::VIZ:
			sendLight = arg1;
			recvLight = arg2;
			conf_path = "sphlviz.conf";
			hq_conf_path = "sphlviz_hq.conf";
			break;
		case Type::REF:
			conf_path = "ssphh_ground_truth.conf";
			hq_conf_path = "ssphh_ground_truth_hq.conf";
			break;
		case Type::REF_CubeMap:
			break;
		case Type::Sky:
			break;
		}
	}

	CoronaJob::~CoronaJob()
	{
	}

	void CoronaJob::Start(CoronaSceneFile &coronaScene, SimpleSceneGraph &ssg)
	{
		if (state != State::Ready)
		{
			return;
		}

		if (!ignoreCache)
		{
			FilePathInfo fpi(isHQ ? hq_output_path_exr : output_path_exr);
			if (fpi.Exists())
			{
				state = State::Finished;
				return;
			}
		}

		if (1)
		{
			float tonemap = ssg.environment.toneMapExposure;
			if (type == Type::VIZ)
			{
				tonemap = 0.0f;
			}
			if (type == Type::GEN)
			{
				tonemap = 0.0f;
			}
			std::ofstream fout(scene_name + "_tonemap.conf");
			fout << "Float colorMap.simpleExposure = " << tonemap << std::endl;
			if (type == Type::REF)
			{
				fout << "Int image.width = " << imageWidth << std::endl;
				fout << "Int image.height = " << imageHeight << std::endl;
			}
			else
			{
				fout << "Int image.width = " << 6 * 128 << std::endl;
				fout << "Int image.height = " << 128 << std::endl;
			}
			if (!isHQ)
			{
				fout << "Int shading.maxRayDepth = " << maxRayDepth << std::endl;
				fout << "Int progressive.passLimit = " << passLimit << std::endl;
			}
			fout.close();
		}

		double t0 = hflog.getSecondsElapsed();
		state = State::Running;
		bool result = true;
		switch (type)
		{
		case Type::REF:
			coronaScene.WriteSCN(scene_path, ssg);
			result = Run();
			break;
		case Type::REF_CubeMap:
			coronaScene.WriteCubeMapSCN(scene_path, ssg);
			result = Run();
			break;
		case Type::Sky:
			coronaScene.WriteSkySCN(scene_path, ssg);
			result = Run();
			break;
		case Type::GEN:
			coronaScene.WriteSphlVizSCN(scene_path, ssg, -1, recvLight);
			result = Run();
			break;
		case Type::VIZ:
			coronaScene.WriteSphlVizSCN(scene_path, ssg, sendLight, recvLight);
			result = Run();
			break;
		default:
			break;
		}
		elapsedTime = hflog.getSecondsElapsed() - t0;
		state = result ? State::Finished : State::Error;

		std::string tonemapconf = scene_name + "_tonemap.conf";
#ifdef WIN32
		DeleteFile(tonemapconf.c_str());
		DeleteFile(scene_path.c_str());
#else
		execl("rm", tonemapconf.c_str());
		execl("rm", scene_path.c_str());
#endif
	}

	void CoronaJob::CopySPH(const Sph4f &sph_)
	{
		if (!IsFinished() && !IsGEN() && !IsVIZ())
			return;
		memset(sph, 0, sizeof(float) * 484);
		for (size_t i = 0; i < sph_.size(); i++)
		{
			sph[121 * 0 + i] = sph_.r().getCoefficient(i);
			sph[121 * 1 + i] = sph_.g().getCoefficient(i);
			sph[121 * 2 + i] = sph_.g().getCoefficient(i);
			sph[121 * 3 + i] = sph_.a().getCoefficient(i);
		}
	}

	void CoronaJob::CopySPHToSph4f(Sph4f &sph_)
	{
		sph_.resize(MaxSphlDegree);
		for (size_t i = 0; i < sph_.size(); i++)
		{
			sph_.r().setCoefficient(i, sph[121 * 0 + i]);
			sph_.g().setCoefficient(i, sph[121 * 1 + i]);
			sph_.b().setCoefficient(i, sph[121 * 2 + i]);
			sph_.a().setCoefficient(i, sph[121 * 3 + i]);
		}
	}

	std::string CoronaJob::MakeCoronaCommandLine()
	{
		std::ostringstream cmd;
		cmd << "\"C:\\Program Files\\Corona\\Standalone\\Corona.exe\" " << scene_path << " -silent";

		if (isHDR)
		{
			cmd << " -oR " << (isHQ ? hq_output_path_exr : output_path_exr);
		}
		else
		{
			cmd << " -o " << (isHQ ? hq_output_path_exr : output_path_exr);
		}

		if (isHQ)
		{
			cmd << " -c " << hq_conf_path;
		}
		else
		{
			cmd << " -c " << conf_path;
		}

		cmd << " -c " << scene_name << "_tonemap.conf";

		hflog.infofn(__FUNCTION__, "running %s", cmd.str().c_str());

		return cmd.str();
	}

	std::string CoronaJob::MakeConvertCommandLine()
	{
		std::ostringstream cmd;
		if (isHQ)
		{
			cmd << "magick " << hq_output_path_exr << " -compress none " << hq_output_path_ppm;
		}
		else
		{
			cmd << "magick " << output_path_exr << " -compress none " << output_path_ppm;
		}
		return cmd.str();
	}

	bool CoronaJob::Run()
	{
		int retval = 0;
		retval = lastCoronaRetval = system(MakeCoronaCommandLine().c_str());
		if (retval != 0)
		{
			hflog.errorfn(__FUNCTION__, "unable to run corona");
			return false;
		}
		//retval = lastConvertRetval = system(MakeConvertCommandLine().c_str());
		//if (retval != 0) return false;

		{
			std::ostringstream cmd;
			cmd << "magick " << output_path_exr << " " << output_path_png;
			retval = lastConvertRetval = system(cmd.str().c_str());
			if (retval != 0)
			{
				hflog.errorfn(__FUNCTION__, "unable to convert EXR to PNG");
				return false;
			}
		}
		{
			std::ostringstream cmd;
			cmd << "magick " << output_path_png << " -compress none " << output_path_ppm;
			retval = lastConvertRetval = system(cmd.str().c_str());
			if (retval != 0)
			{
				hflog.errorfn(__FUNCTION__, "unable to convert PNG to PPM");
				return false;
			}
		}
		return true;
	}

	std::string CoronaJob::ToString() const
	{
		std::ostringstream ostr;
		//ostr << imageWidth << std::endl;
		//ostr << imageHeight << std::endl;
		//ostr << ignoreCache << std::endl;
		//ostr << maxRayDepth << std::endl;
		//ostr << passLimit << std::endl;
		//ostr << elapsedTime << std::endl;
		//ostr << finished << std::endl;
		//ostr << working << std::endl;
		//ostr << isHQ << std::endl;
		//ostr << isHDR << std::endl;
		//ostr << sendLight << std::endl;
		//ostr << recvLight << std::endl;
		//ostr << (type == Type::VIZ ? "VIZ" : (type == Type::GEN ? "GEN" : "REF")) << std::endl;
		//ostr << scene_name << std::endl;
		//ostr << output_path_ppm << std::endl;

		return ostr.str();
	}

	void CoronaJob::FromString(const std::string &str)
	{
	}

	//////////////////////////////////////////////////////////////////
	// Static CoronaJob methods //////////////////////////////////////
	//////////////////////////////////////////////////////////////////

	std::string CoronaJob::MakeREFName(const std::string &prefix, bool isCubeMap, bool isHDR, bool isHQ, bool ks, int MaxRayDepth, int PassLimit)
	{
		std::ostringstream ostr;

		if (!prefix.empty())
			ostr << prefix << "_";
		ostr << "REF";
		if (isCubeMap)
			ostr << "_cubemap";
		if (isHDR)
			ostr << "_hdr";
		if (isHQ)
			ostr << "_hq";
		else
		{
			ostr << "_" << std::setw(2) << std::setfill('0') << MaxRayDepth;
			ostr << "_" << std::setw(2) << std::setfill('0') << PassLimit;
			if (ks)
				ostr << "_Ks";
		}
		return ostr.str();
	}

	std::string CoronaJob::MakeVIZName(const std::string &prefix, int srcLightIndex, int recvLightIndex, bool isHDR, bool isHQ, bool ks, int MaxRayDepth, int PassLimit)
	{
		std::ostringstream ostr;
		if (!prefix.empty())
			ostr << prefix << "_";
		ostr << "VIZ" << std::setfill('0') << std::setw(2) << srcLightIndex + 1 << std::setw(2) << recvLightIndex + 1;
		if (isHDR)
			ostr << "_hdr";
		if (isHQ)
			ostr << "_hq";
		else
		{
			ostr << "_" << std::setw(2) << std::setfill('0') << MaxRayDepth;
			ostr << "_" << std::setw(2) << std::setfill('0') << PassLimit;
			if (ks)
				ostr << "_Ks";
		}
		return ostr.str();
	}

	std::string CoronaJob::MakeGENName(const std::string &prefix, int recvLightIndex, bool isHDR, bool isHQ, bool ks, int MaxRayDepth, int PassLimit)
	{
		std::ostringstream ostr;
		if (!prefix.empty())
			ostr << prefix << "_";
		ostr << "GEN" << std::setfill('0') << std::setw(2) << recvLightIndex + 1;
		if (isHDR)
			ostr << "_hdr";
		if (isHQ)
			ostr << "_hq";
		else
		{
			if (MaxRayDepth >= 0)
				ostr << "_" << std::setw(2) << std::setfill('0') << MaxRayDepth;
			if (PassLimit >= 0)
				ostr << "_" << std::setw(2) << std::setfill('0') << PassLimit;
			if (ks)
				ostr << "_Ks";
		}
		return ostr.str();
	}

	std::string CoronaJob::MakeHIERName(const std::string &prefix, int sendLightIndex, int MaxDegrees)
	{
		std::ostringstream ostr;
		if (!prefix.empty())
			ostr << prefix << "_";
		ostr << "HIER" << std::setfill('0') << std::setw(2) << sendLightIndex + 1;
		if (MaxDegrees >= 0)
			ostr << "_" << MaxDegrees;
		return ostr.str();
	}

} // namespace Fluxions