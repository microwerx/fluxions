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
#include "kasl_evaluator.hpp"
#include "kasl.hpp"
#include "stdafx.h"

namespace KASL {
VariableList::VariableList()
{
}

VariableList::~VariableList()
{
    variables.clear();
}

bool VariableList::is_var(const string& name) const
{
    auto it = variables.find(name);
    if (it == variables.end())
        return false;
    return true;
}

void VariableList::set_var(const string& name, double dval)
{
    KASL::Token token;
    token.type = KASL::TokenType::TT2_DOUBLE;
    token.dval = dval;
    token.ival = (int)floor(dval);
    variables[name] = token;
}

void VariableList::set_var(const string& name, int ival)
{
    KASL::Token token;
    token.type = KASL::TokenType::TT2_INTEGER;
    token.dval = ival;
    token.ival = ival;
    variables[name] = token;
}

void VariableList::set_var(const string& name, const string& sval)
{
    KASL::Token token;
    token.type = KASL::TokenType::TT2_STRING;
    token.sval = sval;
    variables[name] = token;
}

int VariableList::get_var_integer(const string& name) const
{
    auto it = get_var(name);
    if (it == get_var_end() || it->second.IsIntegerOrDouble() == false)
        return 0;
    return it->second.ival;
}

double VariableList::get_var_double(const string& name) const
{
    auto it = get_var(name);
    if (it == get_var_end() || it->second.IsIntegerOrDouble() == false)
        return 0.0;
    return it->second.dval;
}

const string& VariableList::get_var_string(const string& name) const
{
    auto it = get_var(name);
    if (it == get_var_end() || it->second.IsStringOrIdentifier() == false)
        return blankString;
    return it->second.sval;
}

map<string, KASL::Token>::const_iterator VariableList::get_var(const string& name) const
{
    return variables.find(name);
}

map<string, KASL::Token>::const_iterator VariableList::get_var_end() const
{
    return variables.cend();
}
}
