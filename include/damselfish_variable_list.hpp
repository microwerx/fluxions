#ifndef DAMSELFISH_VARIABLE_LIST_HPP
#define DAMSELFISH_VARIABLE_LIST_HPP

#include <damselfish_base.hpp>
#include <damselfish_lexer.hpp>

namespace Df {
	class VariableList {
	private:
		std::string blankString;

	public:
		VariableList();
		~VariableList();

		std::map<std::string, Df::Token> variables;
		double get_var_double(const std::string& name) const;
		int get_var_integer(const std::string& name) const;
		const std::string& get_var_string(const std::string& name) const;
		void set_var(const std::string& name, double dval);
		void set_var(const std::string& name, int ival);
		void set_var(const std::string& name, const std::string& sval);
		bool is_var(const std::string& name) const;
		std::map<std::string, Df::Token>::const_iterator get_var(const std::string& name) const;
		std::map<std::string, Df::Token>::const_iterator get_var_end() const;
	};
}

#endif
