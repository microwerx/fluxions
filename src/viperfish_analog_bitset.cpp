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
#include "pch.h"
#include <viperfish_widget.hpp>

namespace Viperfish {
AnalogBitset::AnalogBitset()
{
    std::fill_n(fbits, 32, 0.0f);
    ibits = 0;
}

bool AnalogBitset::BitTest(unsigned bitsToTest) const
{
    return (ibits & bitsToTest) != 0;
}

void AnalogBitset::BitOn(unsigned which)
{
    unsigned value = 1 << which;

    if (within(which, 0, 31)) {
        ibits |= value;
        fbits[which] = 1.0f;
    }
}

void AnalogBitset::BitOff(unsigned which)
{
    unsigned value = 1 << which;
    if (within(which, 0, 31)) {
        ibits &= ~value;
        fbits[which] = 0.0f;
    }
}

void AnalogBitset::BitSetf(unsigned which, float value)
{
    unsigned ivalue = 1 << which;
    if (within(which, 0, 31)) {
        if (value == 0.0) {
            ibits |= ivalue;
        } else {
            ibits &= ~ivalue;
        }
        if (isfinite(value))
            fbits[which] = clamp(value, -1.0f, 1.0f);
        else
            fbits[which] = 0.0f;
    }
}

void AnalogBitset::BitSet(unsigned bits)
{
    for (int i = 0; i < 32; i++) {
        unsigned value = bits & 1;
        BitSet(i, value ? 1.0f : 0.0f);
        bits >>= 1;
    }
}
}