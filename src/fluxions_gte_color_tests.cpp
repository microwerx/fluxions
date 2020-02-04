#include "fluxions_gte_pch.hpp"
#include <iostream>
#include <iomanip>
#include <fluxions_gte_color_math.hpp>

namespace Fluxions {
#define TEST(test)                                                                   \
	if (!(test))                                                                     \
	{                                                                                \
		result = false;                                                              \
		std::cerr << "Test Failed(Line " << __LINE__ << "): " << #test << std::endl; \
	}

	template <typename ColorType>
	bool TestTColor3() {
		bool result = true;
		using T = typename ColorType::value_type;

		ColorType maxcolor(ColorType::max_value,
						   ColorType::max_value,
						   ColorType::max_value);
		ColorType mincolor(ColorType::min_value,
						   ColorType::min_value,
						   ColorType::min_value);

		// Test to make sure min and max values are unique
		T minvalue = maxcolor.minrgb();
		T maxvalue = maxcolor.maxrgb();
		TEST(minvalue == maxvalue);
		minvalue = mincolor.minrgb();
		maxvalue = mincolor.maxrgb();
		TEST(minvalue == maxvalue);
		minvalue = mincolor.minrgb();
		maxvalue = maxcolor.maxrgb();
		TEST(minvalue != maxvalue);
		// Test to make sure r, g, b values are unique
		TEST(maxcolor.r == maxcolor.g && maxcolor.r == maxcolor.b);
		TEST(mincolor.r == mincolor.g && mincolor.b == mincolor.b);
		maxcolor.reset(minvalue, (minvalue + maxvalue) / 2, maxvalue);
		TEST(maxcolor.r != maxcolor.g && maxcolor.g != maxcolor.b && maxcolor.r != maxcolor.b);
		return result;
	}

	template <typename ColorType>
	bool TestTColor4() {
		return TestTColor3<ColorType>();
	}

	void TestColorType() {
#define FLUXIONS_TEST(test) fprintf(stderr, "%s(): Test " #test " was %s\n", __FUNCTION__, ((test) ? "successful" : "unsuccessful"));
		FLUXIONS_TEST(TestTColor3<Color3i>());
		FLUXIONS_TEST(TestTColor4<Color4i>());
	}
} // namespace Fluxions
