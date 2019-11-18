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
#ifndef DAMSELFISH_HPP
#define DAMSELFISH_HPP

#include <map>
#include <string>
#include <vector>

//#ifndef DAMSELFISH_STATIC
//#ifdef DAMSELFISHDLLEXPORTS
//#define DAMSELFISH_API __declspec(dllexport)
//#else
//#define DAMSELFISH_API __declspec(dllimport)
//#endif
//#else
//#define DAMSELFISH_API
//#endif

namespace Df
{
using StringType = std::string;

enum class TokenType
{
	TT0_NOTHING,
	TT0_CHAR,
	TT0_SPACE,
	TT0_TAB,
	TT0_NEWLINE,
	TT0_DIGIT,
	TT0_ALPHA,
	TT0_CNTRL,
	TT0_PUNCT,
	TT1_EXCLAMATION_POINT,
	TT1_DOUBLE_QUOTE,
	TT1_NUMBER_SIGN,
	TT1_DOLLAR_SIGN,
	TT1_PERCENT_SIGN,
	TT1_AMPERSAND,
	TT1_SINGLE_QUOTE,
	TT1_LPAREN,
	TT1_RPAREN,
	TT1_ASTERICK,
	TT1_PLUS_SIGN,
	TT1_COMMA,
	TT1_DASH,
	TT1_PERIOD,
	TT1_FORWARD_SLASH,
	TT1_COLON,
	TT1_SEMICOLON,
	TT1_LESS_THAN_SIGN,
	TT1_EQUAL_SIGN,
	TT1_GREATER_THAN_SIGN,
	TT1_QUESTION_MARK,
	TT1_AT_SYMBOL,
	TT1_LBRACKET,
	TT1_BACK_SLASH,
	TT1_RBRACKET,
	TT1_CARET,
	TT1_UNDERSCORE,
	TT1_GRAVE_ACCENT,
	TT1_LBRACE,
	TT1_VERTICAL_BAR,
	TT1_RBRACE,
	TT1_TILDE,
	TT2_ID,
	TT2_STRING,
	TT2_DOUBLE,
	TT2_INTEGER,
	TT2_HEXINTEGER
};

struct LexerToken
{
	TokenType type;

	size_t start;
	size_t length;
	char *cptr;
};

struct LexerState
{
	StringType buffer;
	std::vector<LexerToken> tokens;

	LexerToken tmptoken;
	LexerToken *lasttoken;
	bool eatChar;
};

struct Token
{
	TokenType type;
	std::string sval;
	int ival;
	double dval;

	bool IsInteger() const { return type == TokenType::TT2_INTEGER; }
	bool IsDouble() const { return type == TokenType::TT2_DOUBLE; }
	bool IsString() const { return type == TokenType::TT2_STRING; }
	bool IsIdentifier() const { return type == TokenType::TT2_ID; }
	bool IsStringOrIdentifier() const { return type == TokenType::TT2_STRING || type == TokenType::TT2_ID; }
	bool IsIntegerOrDouble() const { return type == TokenType::TT2_INTEGER || type == TokenType::TT2_DOUBLE; }
	bool IsType(TokenType which) const { return type == which; }
};

using LexReplacementType = std::tuple<TokenType, TokenType, StringType::value_type>;
using TokenVector = std::vector<Token>;

std::ostream &operator<<(std::ostream &ostr, const TokenType &type);
size_t lex_init(LexerState &ls);
size_t lex(LexerState &ls, const StringType &inputStr, int level = 0);
void lex_print(LexerState &ls);
void lex_replace(LexerState &ls, LexerState &output, const std::vector<LexReplacementType> &replacements);
size_t lex_scan_number(StringType::value_type *cptr, LexerToken &token);
size_t lex_scan_string(StringType::value_type *cptr, LexerToken &token);
size_t lex_tokens(LexerState &ls, std::vector<Token> &symbols);
void lex_tokens_print(std::vector<Token> &tokens);
size_t lex_quick_parse(const StringType &inputStr, int level, const std::vector<LexReplacementType> &replacements, TokenVector &tokens);
size_t lex_quick_parse(const StringType &inputStr, TokenVector &tokens);
size_t lex_quick_l2_parse(const StringType &inputStr, TokenVector &tokens);
size_t lex_quick_l3_parse(const StringType &inputStr, TokenVector &tokens);
std::string TokenVectorJoin(const TokenVector tokens, const std::string &separator);

class VariableList
{
  private:
	std::string blankString;

  public:
	VariableList();
	~VariableList();

	std::map<std::string, Df::Token> variables;
	double get_var_double(const std::string &name) const;
	int get_var_integer(const std::string &name) const;
	const std::string &get_var_string(const std::string &name) const;
	void set_var(const std::string &name, double dval);
	void set_var(const std::string &name, int ival);
	void set_var(const std::string &name, const std::string &sval);
	bool is_var(const std::string &name) const;
	std::map<std::string, Df::Token>::const_iterator get_var(const std::string &name) const;
	std::map<std::string, Df::Token>::const_iterator get_var_end() const;
};
} // namespace Df

#endif
