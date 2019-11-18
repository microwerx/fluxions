#include <fluxions_gte.hpp>
#include <fluxions_gte_bounding_box.hpp>
#include <fluxions_gte_color3.hpp>
#include <fluxions_gte_color4.hpp>
#include <fluxions_gte_geometric_algebra.hpp>
#include <fluxions_gte_image.hpp>
#include <fluxions_gte_iostream.hpp>
#include <fluxions_gte_scalar_math.hpp>
#include <fluxions_gte_matrix2.hpp>
#include <fluxions_gte_matrix3.hpp>
#include <fluxions_gte_matrix4.hpp>
#include <fluxions_gte_matrix_extra.hpp>
#include <fluxions_gte_matrix_stack.hpp>
#include <fluxions_gte_plane.hpp>
#include <fluxions_gte_quaternion.hpp>
#include <fluxions_gte_ray.hpp>
#include <fluxions_gte_spherical_harmonic.hpp>
#include <fluxions_gte_vector2.hpp>
#include <fluxions_gte_vector3.hpp>
#include <fluxions_gte_vector4.hpp>
#include <fluxions_gte_image.hpp>

namespace Fluxions
{
	void TestVector3();
	void TestColorType();
	void TestImage();

	void TestFluxionsGTE() {
		TestVector3();
		TestColorType();
		TestImage();
	}
}