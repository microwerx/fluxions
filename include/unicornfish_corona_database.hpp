#ifndef FLUXIONS_CORONA_DATABASE_HPP
#define FLUXIONS_CORONA_DATABASE_HPP

#include <fluxions_stdcxx.hpp>
#include <fluxions_gte.hpp>

namespace Uf
{
	class CoronaDatabase
	{
	public:
		//Fluxions::SimpleRenderer_GLuint renderer;
		std::map<std::string, Fluxions::OBJStaticModel> models;

		CoronaDatabase();
		~CoronaDatabase();

		void LoadOBJ(const std::string& objname, const std::string& filename);
		//void BuildBuffers();
		//void Render();
		//void RenderZOnly();
	};
} // namespace Fluxions

#endif
