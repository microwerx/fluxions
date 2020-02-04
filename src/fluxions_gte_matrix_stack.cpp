#include "fluxions_gte_pch.hpp"
#define FLUXIONS_NO_EXTERN_TEMPLATES
#include <fluxions_gte_matrix_stack.hpp>

namespace Fluxions {
	template class Matrix4Stack<float, 32>;
	template class Matrix4Stack<double, 32>;

	void Matrix4StackTest() {
		Matrix4Stackf ms;
		ms.reset();
		ms.LoadIdentity();
		ms.Frustum(0.0f, 1920.0f, 1080.0f, 0.0f, 0.1f, 100.0f);
		ms.Ortho(0.0f, 1920.0f, 1080.0f, 0.0f, -1.0f, 1.0f);
		ms.Ortho2D(0.0f, 1920.0f, 1080.0f, 0.0f);
		ms.Perspective(45.0f, 1.0f, 0.01f, 100.0f);
		ms.PushMatrix();
		ms.PopMatrix();
	}
} // namespace Fluxions
