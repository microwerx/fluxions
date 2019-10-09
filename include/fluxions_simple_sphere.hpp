#ifndef FLUXIONS_SIMPLE_SPHERE_HPP
#define FLUXIONS_SIMPLE_SPHERE_HPP

#include <fluxions_gte_matrix4.hpp>
#include <fluxions_opengl.hpp>

namespace Fluxions
{
	struct SimpleSphere
	{
		Matrix4f transform;
		std::string mtllibName;
		std::string mtlName;
		GLuint mtllibId = 0;
		GLuint mtlId = 0;
		GLuint objectId = 0;
	};
} // namespace Fluxions

#endif