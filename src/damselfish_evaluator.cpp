// Damselfish
// Copyright (C) 2017-2019 Jonathan Metzgar
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
#include <damselfish_evaluator.hpp>
#include <damselfish.hpp>
#include <cmath>

namespace Df
{
VariableList::VariableList()
{
}

VariableList::~VariableList()
{
    variables.clear();
}

bool VariableList::is_var(const std::string &name) const
{
    auto it = variables.find(name);
    if (it == variables.end())
        return false;
    return true;
}

void VariableList::set_var(const std::string &name, double dval)
{
    Df::Token token;
    token.type = Df::TokenType::TT2_DOUBLE;
    token.dval = dval;
    token.ival = (int)std::floor(dval);
    variables[name] = token;
}

void VariableList::set_var(const std::string &name, int ival)
{
    Df::Token token;
    token.type = Df::TokenType::TT2_INTEGER;
    token.dval = ival;
    token.ival = ival;
    variables[name] = token;
}

void VariableList::set_var(const std::string &name, const std::string &sval)
{
    Df::Token token;
    token.type = Df::TokenType::TT2_STRING;
    token.sval = sval;
    variables[name] = token;
}

int VariableList::get_var_integer(const std::string &name) const
{
    auto it = get_var(name);
    if (it == get_var_end() || it->second.IsIntegerOrDouble() == false)
        return 0;
    return it->second.ival;
}

double VariableList::get_var_double(const std::string &name) const
{
    auto it = get_var(name);
    if (it == get_var_end() || it->second.IsIntegerOrDouble() == false)
        return 0.0;
    return it->second.dval;
}

const std::string &VariableList::get_var_string(const std::string &name) const
{
    auto it = get_var(name);
    if (it == get_var_end() || it->second.IsStringOrIdentifier() == false)
        return blankString;
    return it->second.sval;
}

std::map<std::string, Df::Token>::const_iterator VariableList::get_var(const std::string &name) const
{
    return variables.find(name);
}

std::map<std::string, Df::Token>::const_iterator VariableList::get_var_end() const
{
    return variables.cend();
}
} // namespace Df
