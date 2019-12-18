#ifndef FLUXIONS_SIMPLE_GEOMETRY_GROUP_HPP
#define FLUXIONS_SIMPLE_GEOMETRY_GROUP_HPP

#include <string>
#include <fluxions_gte_matrix4.hpp>
#include <fluxions_gte_bounding_box.hpp>
#include <fluxions_opengl.hpp>
#include <fluxions_file_path_info.hpp>

namespace Fluxions
{
	struct SimpleGeometryGroup
	{
		Matrix4f transform;
		Matrix4f addlTransform;
		BoundingBoxf bbox;

		FilePathInfo fpi;

		std::string objectName;
		std::string mtllibName;

		GLuint objectId = 0;
		GLuint mtllibId = 0;
	};
} // namespace Fluxions

#endif