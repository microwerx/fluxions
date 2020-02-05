#include "viperfish_pch.hpp"
#include <fluxions_stdcxx.hpp>
#include <viperfish_widget.hpp>

namespace Vf {
	AnalogBitset::AnalogBitset() {
		std::fill_n(fbits, 32, 0.0f);
		ibits = 0;
	}

	bool AnalogBitset::bitTest(unsigned bitsToTest) const {
		return (ibits & bitsToTest) != 0;
	}

	void AnalogBitset::bitOn(unsigned which) {
		unsigned value = 1 << which;

		if (within(which, 0, 31)) {
			ibits |= value;
			fbits[which] = 1.0f;
		}
	}

	void AnalogBitset::bitOff(unsigned which) {
		unsigned value = 1 << which;
		if (within(which, 0, 31)) {
			ibits &= ~value;
			fbits[which] = 0.0f;
		}
	}

	void AnalogBitset::bitSetf(unsigned which, float value) {
		unsigned ivalue = 1 << which;
		if (within(which, 0, 31)) {
			if (value == 0.0) {
				ibits |= ivalue;
			}
			else {
				ibits &= ~ivalue;
			}
			if (std::isfinite(value))
				fbits[which] = Fx::clamp(value, -1.0f, 1.0f);
			else
				fbits[which] = 0.0f;
		}
	}

	void AnalogBitset::bitSet(unsigned bits) {
		for (int i = 0; i < 32; i++) {
			unsigned value = bits & 1;
			bitSet(i, value ? 1.0f : 0.0f);
			bits >>= 1;
		}
	}
} // namespace Vf
