#ifndef DAMSELFISH_LEXER_HPP
#define DAMSELFISH_LEXER_HPP

#include <damselfish_base.hpp>

namespace Df {
	struct LexerToken {
		TokenType type{ TokenType::TT0_NOTHING };

		size_t start{ 0 };
		size_t length{ 0 };
		char* cptr{ nullptr };
	};

	struct LexerState {
		StringType buffer;
		std::vector<LexerToken> tokens;

		LexerToken tmptoken;
		LexerToken* lasttoken{ nullptr };
		bool eatChar{ false };
	};

	struct Token {
		TokenType type;
		std::string sval;
		int ival{ 0 };
		double dval{ 0.0 };

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

	std::ostream& operator<<(std::ostream& ostr, const TokenType& type);
	size_t lex_init(LexerState& ls);
	size_t lex(LexerState& ls, const StringType& inputStr, int level = 0);
	void lex_print(LexerState& ls);
	void lex_replace(LexerState& ls, LexerState& output, const std::vector<LexReplacementType>& replacements);
	size_t lex_scan_number(StringType::value_type* cptr, LexerToken& token);
	size_t lex_scan_string(StringType::value_type* cptr, LexerToken& token);
	size_t lex_tokens(LexerState& ls, std::vector<Token>& symbols);
	void lex_tokens_print(std::vector<Token>& tokens);
	size_t lex_quick_parse(const StringType& inputStr, int level, const std::vector<LexReplacementType>& replacements, TokenVector& tokens);
	size_t lex_quick_parse(const StringType& inputStr, TokenVector& tokens);
	size_t lex_quick_l2_parse(const StringType& inputStr, TokenVector& tokens);
	size_t lex_quick_l3_parse(const StringType& inputStr, TokenVector& tokens);
	std::string TokenVectorJoin(const TokenVector tokens, const std::string& separator);
}

#endif
