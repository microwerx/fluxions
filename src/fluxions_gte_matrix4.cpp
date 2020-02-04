#include "fluxions_gte_pch.hpp"
#define FLUXIONS_NO_EXTERN_TEMPLATES
#include <fluxions_gte_matrix4.hpp>

namespace Fluxions {
	template class TMatrix4<float>;
	template class TMatrix4<double>;

	void ___TestMatrix4() {
		Matrix4f m(5); // = 5.0f;
		m = 5.0;
		m = Matrix4f::MakeCubeMatrix(0);
		m = Matrix4f::MakeFrustum(0.0f, 1.0f, 0.0f, 1.0f, 0.001f, 1000.0f);
		m.Rotate(0.0, 1.0, 0.0, 0.0);
		m = Matrix4f::MakeOrtho(0.0f, 1280.0f, 0.0f, 720.0f, -1.0f, 1.0f);
		m = Matrix4f::MakeOrtho2D(0.0f, 1280.0f, 0.0f, 720.0f);
	}
} // namespace Fluxions
