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
#ifndef FLUXIONS_GTE_GEOMETRIC_ALGEBRA_HPP
#define FLUXIONS_GTE_GEOMETRIC_ALGEBRA_HPP

namespace Fluxions
{
using ScalarType = float;

struct G40vec
{
    G40vec(ScalarType xVal = 0)
        : x(xVal)
    {
    }

    ScalarType x;
};

struct G41vec
{
    G41vec(ScalarType e1Val = 0, ScalarType e2Val = 0, ScalarType e3Val = 0, ScalarType e4Val = 0)
        : e1(e1Val), e2(e2Val), e3(e3Val), e4(e4Val)
    {
    }

    ScalarType e1;
    ScalarType e2;
    ScalarType e3;
    ScalarType e4;
};

struct G42vec
{
    G42vec(ScalarType e1e2Val = 0, ScalarType e1e3Val = 0, ScalarType e1e4Val = 0, ScalarType e2e3Val = 0, ScalarType e2e4Val = 0, ScalarType e3e4Val = 0)
        : e1e2(e1e2Val), e1e3(e1e3Val), e1e4(e1e4Val), e2e3(e2e3Val), e2e4(e2e4Val), e3e4(e3e4Val)
    {
    }

    ScalarType e1e2;
    ScalarType e1e3;
    ScalarType e1e4;
    ScalarType e2e3;
    ScalarType e2e4;
    ScalarType e3e4;
};

struct G43vec
{
    G43vec(ScalarType e1e2e3Val = 0, ScalarType e1e2e4Val = 0, ScalarType e1e3e4Val = 0, ScalarType e2e3e4Val = 0)
        : e1e2e3(e1e2e3Val), e1e2e4(e1e2e4Val), e1e3e4(e1e3e4Val), e2e3e4(e2e3e4Val)
    {
    }

    ScalarType e1e2e3;
    ScalarType e1e2e4;
    ScalarType e1e3e4;
    ScalarType e2e3e4;
};

struct G44vec
{
    G44vec(ScalarType val = 0)
        : e1e2e3e4(val)
    {
    }

    ScalarType e1e2e3e4;
};

struct G4multivector
{
    G40vec k0;
    G41vec k1;
    G42vec k2;
    G43vec k3;
    G44vec k4;
};

// operators:
// unary minus
//  TYPE operator-(const TYPE &rhs) { ... }
// binary addition and subtraction
//  TYPE operator+(const TYPE &lhs, const TYPE &rhs) { ... }
//  TYPE operator-(const TYPE &lhs, const TYPE &rhs) { ... }
// scalar multiplication and division
//  TYPE operator*(ScalarType lhs, const TYPE &rhs) { ... }
//  TYPE operator*(const TYPE &lhs, ScalarType rhs) { ... }
//  TYPE operator/(const TYPE &lhs, ScalarType rhs) { ... }
//

inline G40vec operator+(const G40vec &lhs, const G40vec &rhs)
{
    return G40vec(
        lhs.x + rhs.x);
}

inline G41vec operator+(const G41vec &lhs, const G41vec &rhs)
{
    return G41vec(
        lhs.e1 + rhs.e1,
        lhs.e2 + rhs.e2,
        lhs.e3 + rhs.e3,
        lhs.e4 + rhs.e4);
}

inline G42vec operator+(const G42vec &lhs, const G42vec &rhs)
{
    return G42vec(
        lhs.e1e2 + rhs.e1e2,
        lhs.e1e3 + rhs.e1e3,
        lhs.e1e4 + rhs.e1e4,
        lhs.e2e3 + rhs.e2e3,
        lhs.e2e4 + rhs.e2e4,
        lhs.e3e4 + rhs.e3e4);
}

inline G43vec operator+(const G43vec &lhs, const G43vec &rhs)
{
    return G43vec(
        lhs.e1e2e3 + rhs.e1e2e3,
        lhs.e1e2e4 + rhs.e1e2e4,
        lhs.e1e3e4 + rhs.e1e3e4,
        lhs.e2e3e4 + rhs.e2e3e4);
}

inline G44vec operator+(const G44vec &lhs, const G44vec &rhs)
{
    return G44vec(
        lhs.e1e2e3e4 + rhs.e1e2e3e4);
}

inline G40vec operator-(const G40vec &lhs, const G40vec &rhs)
{
    return G40vec(
        lhs.x - rhs.x);
}

inline G41vec operator-(const G41vec &lhs, const G41vec &rhs)
{
    return G41vec(
        lhs.e1 - rhs.e1,
        lhs.e2 - rhs.e2,
        lhs.e3 - rhs.e3,
        lhs.e4 - rhs.e4);
}

inline G42vec operator-(const G42vec &lhs, const G42vec &rhs)
{
    return G42vec(
        lhs.e1e2 - rhs.e1e2,
        lhs.e1e3 - rhs.e1e3,
        lhs.e1e4 - rhs.e1e4,
        lhs.e2e3 - rhs.e2e3,
        lhs.e2e4 - rhs.e2e4,
        lhs.e3e4 - rhs.e3e4);
}

inline G43vec operator-(const G43vec &lhs, const G43vec &rhs)
{
    return G43vec(
        lhs.e1e2e3 - rhs.e1e2e3,
        lhs.e1e2e4 - rhs.e1e2e4,
        lhs.e1e3e4 - rhs.e1e3e4,
        lhs.e2e3e4 - rhs.e2e3e4);
}

inline G44vec operator-(const G44vec &lhs, const G44vec &rhs)
{
    return G44vec(
        lhs.e1e2e3e4 - rhs.e1e2e3e4);
}

G40vec add_G40_G40(const G40vec &u, const G40vec &v);
G41vec add_G41_G41(const G41vec &u, const G41vec &v);
G42vec add_G42_G42(const G42vec &u, const G42vec &v);
G43vec add_G43_G43(const G43vec &u, const G43vec &v);
G44vec add_G44_G44(const G44vec &u, const G44vec &v);
//G33vec add_G3_G3(const G33vec &u, const G33vec &ptr);
} // namespace Fluxions

#endif
