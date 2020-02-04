#include "damselfish_pch.hpp"
#include <damselfish_base.hpp>
#include <damselfish_variable_list.hpp>
#include <damselfish_lexer.hpp>

namespace Df {
	VariableList::VariableList() {}

	VariableList::~VariableList() {
		variables.clear();
	}

	bool VariableList::is_var(const std::string& name) const {
		auto it = variables.find(name);
		if (it == variables.end())
			return false;
		return true;
	}

	void VariableList::set_var(const std::string& name, double dval) {
		Df::Token token;
		token.type = Df::TokenType::TT2_DOUBLE;
		token.dval = dval;
		token.ival = (int)std::floor(dval);
		variables[name] = token;
	}

	void VariableList::set_var(const std::string& name, int ival) {
		Df::Token token;
		token.type = Df::TokenType::TT2_INTEGER;
		token.dval = ival;
		token.ival = ival;
		variables[name] = token;
	}

	void VariableList::set_var(const std::string& name, const std::string& sval) {
		Df::Token token;
		token.type = Df::TokenType::TT2_STRING;
		token.sval = sval;
		variables[name] = token;
	}

	int VariableList::get_var_integer(const std::string& name) const {
		auto it = get_var(name);
		if (it == get_var_end() || it->second.IsIntegerOrDouble() == false)
			return 0;
		return it->second.ival;
	}

	double VariableList::get_var_double(const std::string& name) const {
		auto it = get_var(name);
		if (it == get_var_end() || it->second.IsIntegerOrDouble() == false)
			return 0.0;
		return it->second.dval;
	}

	const std::string& VariableList::get_var_string(const std::string& name) const {
		auto it = get_var(name);
		if (it == get_var_end() || it->second.IsStringOrIdentifier() == false)
			return blankString;
		return it->second.sval;
	}

	std::map<std::string, Df::Token>::const_iterator VariableList::get_var(const std::string& name) const {
		return variables.find(name);
	}

	std::map<std::string, Df::Token>::const_iterator VariableList::get_var_end() const {
		return variables.cend();
	}
} // namespace Df
