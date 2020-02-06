#include "fluxions_gte_pch.hpp"
#define FLUXIONS_NO_EXTERN_TEMPLATES
#include <fluxions_gte_color3.hpp>

namespace Fluxions {
	template class TColor3<float>;
	template class TColor3<double>;
	template class TColor3<char>;
	template class TColor3<unsigned char>;
	template class TColor3<short>;
	template class TColor3<unsigned short>;
	template class TColor3<int>;
	template class TColor3<unsigned int>;

	Color3f HLSToRGBf(float h, float l, float s) noexcept {
		float q = l < 0.5f ? l * (1.0f + s) : l + s - l * s;
		float p = 2.0f * l - q;
		auto h2rgb = [&q, &p](float t) {
			if (t < 0.0f)
				t += 1.0f;
			if (t > 1.0f)
				t -= 1.0f;
			if (t < 0.16667f)
				return p + (q - p) * 6.0f * t;
			if (t < 0.5f)
				return q;
			if (t < 0.66667f)
				return p + (q - p) * (0.66667f - t) * 6.0f;
			return 0.0f;
		};

		return Color3f(h2rgb(h + 0.33333f), h2rgb(h), h2rgb(h - 0.33333f));
	}

	Color3d HLSToRGBd(double h, double l, double s) noexcept {
		double q = l < 0.5 ? l * (1.0f + s) : l + s - l * s;
		double p = 2.0 * l - q;
		auto h2rgb = [&q, &p](double t) {
			if (t < 0.0)
				t += 1.0;
			if (t > 1.0)
				t -= 1.0;
			if (t < 0.16667)
				return p + (q - p) * 6.0 * t;
			if (t < 0.5)
				return q;
			if (t < 0.66667)
				return p + (q - p) * (0.66667 - t) * 6.0;
			return 0.0;
		};

		return Color3d(h2rgb(h + 0.33333f), h2rgb(h), h2rgb(h - 0.33333f));
	}

} // namespace Fluxions
