#include "damselfish_pch.hpp"
#include <damselfish_lexer.hpp>

namespace Df {
	//enum class ObjectType {
	//	OT_NULLPTR,
	//	OT_FALSE,
	//	OT_TRUE,
	//	OT_NUMBER,
	//	OT_INTEGER,
	//	OT_STRING,
	//	OT_ARRAY,
	//	OT_OBJECT,
	//	OT_IDENTIFIER
	//};

	//struct Object
	//{
	//	ObjectType type;

	//	// Support strings, numbers, booleans,
	//	StringType id;
	//	StringType sval;
	//	int ival;
	//	double fval;
	//	bool bval;
	//	std::vector<Object> aval;
	//	std::map<StringType, Object> oval;

	//	Object operator=(const Object &other);
	//};

	std::ostream& operator<<(std::ostream& ostr, const TokenType& type) {
		switch (type) {
		case TokenType::TT0_NOTHING:
			break;
		case TokenType::TT0_CHAR:
			ostr << "TT0_CHAR";
			break;
		case TokenType::TT0_SPACE:
			ostr << "TT0_SPACE";
			break;
		case TokenType::TT0_TAB:
			ostr << "TT0_TAB";
			break;
		case TokenType::TT0_NEWLINE:
			ostr << "TT0_NEWLINE";
			break;
		case TokenType::TT0_DIGIT:
			ostr << "TT0_DIGIT";
			break;
		case TokenType::TT0_ALPHA:
			ostr << "TT0_ALPHA";
			break;
		case TokenType::TT0_CNTRL:
			ostr << "TT0_CNTRL";
			break;
		case TokenType::TT0_PUNCT:
			ostr << "TT0_PUNCT";
			break;
		case TokenType::TT1_EXCLAMATION_POINT:
			ostr << "TT1_EXCLAMATION_POINT";
			break;
		case TokenType::TT1_DOUBLE_QUOTE:
			ostr << "TT1_DOUBLE_QUOTE";
			break;
		case TokenType::TT1_NUMBER_SIGN:
			ostr << "TT1_NUMBER_SIGN";
			break;
		case TokenType::TT1_DOLLAR_SIGN:
			ostr << "TT1_DOLLAR_SIGN";
			break;
		case TokenType::TT1_PERCENT_SIGN:
			ostr << "TT1_PERCENT_SIGN";
			break;
		case TokenType::TT1_AMPERSAND:
			ostr << "TT1_AMPERSAND";
			break;
		case TokenType::TT1_SINGLE_QUOTE:
			ostr << "TT1_SINGLE_QUOTE";
			break;
		case TokenType::TT1_LPAREN:
			ostr << "TT1_LPAREN";
			break;
		case TokenType::TT1_RPAREN:
			ostr << "TT1_RPAREN";
			break;
		case TokenType::TT1_ASTERICK:
			ostr << "TT1_ASTERICK";
			break;
		case TokenType::TT1_PLUS_SIGN:
			ostr << "TT1_PLUS_SIGN";
			break;
		case TokenType::TT1_COMMA:
			ostr << "TT1_COMMA";
			break;
		case TokenType::TT1_DASH:
			ostr << "TT1_DASH";
			break;
		case TokenType::TT1_PERIOD:
			ostr << "TT1_PERIOD";
			break;
		case TokenType::TT1_FORWARD_SLASH:
			ostr << "TT1_FORWARD_SLASH";
			break;
		case TokenType::TT1_COLON:
			ostr << "TT1_COLON";
			break;
		case TokenType::TT1_SEMICOLON:
			ostr << "TT1_SEMICOLON";
			break;
		case TokenType::TT1_LESS_THAN_SIGN:
			ostr << "TT1_LESS_THAN_SIGN";
			break;
		case TokenType::TT1_EQUAL_SIGN:
			ostr << "TT1_EQUAL_SIGN";
			break;
		case TokenType::TT1_GREATER_THAN_SIGN:
			ostr << "TT1_GREATER_THAN_SIGN";
			break;
		case TokenType::TT1_QUESTION_MARK:
			ostr << "TT1_QUESTION_MARK";
			break;
		case TokenType::TT1_AT_SYMBOL:
			ostr << "TT1_AT_SYMBOL";
			break;
		case TokenType::TT1_LBRACKET:
			ostr << "TT1_LBRACKET";
			break;
		case TokenType::TT1_BACK_SLASH:
			ostr << "TT1_BACK_SLASH";
			break;
		case TokenType::TT1_RBRACKET:
			ostr << "TT1_RBRACKET";
			break;
		case TokenType::TT1_CARET:
			ostr << "TT1_CARET";
			break;
		case TokenType::TT1_UNDERSCORE:
			ostr << "TT1_UNDERSCORE";
			break;
		case TokenType::TT1_GRAVE_ACCENT:
			ostr << "TT1_GRAVE_ACCENT";
			break;
		case TokenType::TT1_LBRACE:
			ostr << "TT1_LBRACE";
			break;
		case TokenType::TT1_VERTICAL_BAR:
			ostr << "TT1_VERTICAL_BAR";
			break;
		case TokenType::TT1_RBRACE:
			ostr << "TT1_RBRACE";
			break;
		case TokenType::TT1_TILDE:
			ostr << "TT1_TILDE";
			break;
		case TokenType::TT2_ID:
			ostr << "TT2_ID";
			break;
		case TokenType::TT2_STRING:
			ostr << "TT2_STRING";
			break;
		case TokenType::TT2_DOUBLE:
			ostr << "TT2_DOUBLE";
			break;
		case TokenType::TT2_INTEGER:
			ostr << "TT2_INTEGER";
			break;
		case TokenType::TT2_HEXINTEGER:
			ostr << "TT2_HEXINTEGER";
			break;
		}
		return ostr;
	}

	CharClass _charClassLookup[] = {
		// 0 - 8 control codes
		CC_NUL,
		CC_SOH,
		CC_STX,
		CC_ETX,
		CC_EOT,
		CC_ENQ,
		CC_ACK,
		CC_BEL,
		CC_BS,
		// 9
		CC_TAB,
		// 10 - 13
		CC_LF,
		CC_VT,
		CC_FF,
		CC_CR,
		// 14 - 31
		CC_SO,
		CC_SI,
		CC_DLE,
		CC_DC1,
		CC_DC2,
		CC_DC3,
		CC_DC4,
		CC_NAK,
		CC_SYN,
		CC_ETB,
		CC_CAN,
		CC_EM,
		CC_SUB,
		CC_ESC,
		CC_FS,
		CC_GS,
		CC_RS,
		CC_US,
		// 32
		CC_SPACE,
		// 33 - 47 !"#$%&'()*+,-./
		CC_EXCLAMATION_POINT,
		CC_DOUBLE_QUOTE,
		CC_NUMBER_SIGN,
		CC_DOLLAR_SIGN,
		CC_PERCENT_SIGN,
		CC_AMPERSAND,
		CC_SINGLE_QUOTE,
		CC_LPAREN,
		CC_RPAREN,
		CC_ASTERICK,
		CC_PLUS_SIGN,
		CC_COMMA,
		CC_DASH,
		CC_PERIOD,
		CC_FORWARD_SLASH,
		// 48 - 57
		CC_DIGIT, CC_DIGIT, CC_DIGIT, CC_DIGIT, CC_DIGIT,
		CC_DIGIT, CC_DIGIT, CC_DIGIT, CC_DIGIT, CC_DIGIT,
		// 58 - 64 :;<=>?@
		CC_COLON,
		CC_SEMICOLON,
		CC_LESS_THAN_SIGN,
		CC_EQUAL_SIGN,
		CC_GREATER_THAN_SIGN,
		CC_QUESTION_MARK,
		CC_AT_SYMBOL,
		// 65 - 90
		CC_UPPER,
		CC_UPPER, CC_UPPER, CC_UPPER, CC_UPPER, CC_UPPER,
		CC_UPPER, CC_UPPER, CC_UPPER, CC_UPPER, CC_UPPER,
		CC_UPPER, CC_UPPER, CC_UPPER, CC_UPPER, CC_UPPER,
		CC_UPPER, CC_UPPER, CC_UPPER, CC_UPPER, CC_UPPER,
		CC_UPPER, CC_UPPER, CC_UPPER, CC_UPPER, CC_UPPER,
		// 91 - 96 [\]^_`
		CC_LBRACKET,
		CC_BACK_SLASH,
		CC_RBRACKET,
		CC_CARET,
		CC_UNDERSCORE,
		CC_GRAVE_ACCENT,
		// 97 - 122
		CC_LOWER,
		CC_LOWER, CC_LOWER, CC_LOWER, CC_LOWER, CC_LOWER,
		CC_LOWER, CC_LOWER, CC_LOWER, CC_LOWER, CC_LOWER,
		CC_LOWER, CC_LOWER, CC_LOWER, CC_LOWER, CC_LOWER,
		CC_LOWER, CC_LOWER, CC_LOWER, CC_LOWER, CC_LOWER,
		CC_LOWER, CC_LOWER, CC_LOWER, CC_LOWER, CC_LOWER,
		// 123 - 126 {|}~
		CC_LBRACE,
		CC_VERTICAL_BAR,
		CC_RBRACE,
		CC_TILDE,
		// 127
		CC_DEL };

	TokenType _tokenLookup[128] = {
		// 0 - 8 control codes
		TokenType::TT0_NOTHING,
		TokenType::TT0_NOTHING,
		TokenType::TT0_NOTHING,
		TokenType::TT0_NOTHING,
		TokenType::TT0_NOTHING,
		TokenType::TT0_NOTHING,
		TokenType::TT0_NOTHING,
		TokenType::TT0_NOTHING,
		TokenType::TT0_NOTHING,
		// 9
		TokenType::TT0_TAB,
		// 10 - 13
		TokenType::TT0_NOTHING,
		TokenType::TT0_NOTHING,
		TokenType::TT0_NOTHING,
		TokenType::TT0_NEWLINE,
		// 14 - 31
		TokenType::TT0_NOTHING,
		TokenType::TT0_NOTHING,
		TokenType::TT0_NOTHING,
		TokenType::TT0_NOTHING,
		TokenType::TT0_NOTHING,
		TokenType::TT0_NOTHING,
		TokenType::TT0_NOTHING,
		TokenType::TT0_NOTHING,
		TokenType::TT0_NOTHING,
		TokenType::TT0_NOTHING,
		TokenType::TT0_NOTHING,
		TokenType::TT0_NOTHING,
		TokenType::TT0_NOTHING,
		TokenType::TT0_NOTHING,
		TokenType::TT0_NOTHING,
		TokenType::TT0_NOTHING,
		TokenType::TT0_NOTHING,
		TokenType::TT0_NOTHING,
		// 32
		TokenType::TT0_SPACE,
		// 33 - 47 !"#$%&'()*+,-./
		TokenType::TT1_EXCLAMATION_POINT,
		TokenType::TT1_DOUBLE_QUOTE,
		TokenType::TT1_NUMBER_SIGN,
		TokenType::TT1_DOLLAR_SIGN,
		TokenType::TT1_PERCENT_SIGN,
		TokenType::TT1_AMPERSAND,
		TokenType::TT1_SINGLE_QUOTE,
		TokenType::TT1_LPAREN,
		TokenType::TT1_RPAREN,
		TokenType::TT1_ASTERICK,
		TokenType::TT1_PLUS_SIGN,
		TokenType::TT1_COMMA,
		TokenType::TT1_DASH,
		TokenType::TT1_PERIOD,
		TokenType::TT1_FORWARD_SLASH,
		// 48 - 57
		TokenType::TT0_DIGIT, TokenType::TT0_DIGIT, TokenType::TT0_DIGIT, TokenType::TT0_DIGIT, TokenType::TT0_DIGIT,
		TokenType::TT0_DIGIT, TokenType::TT0_DIGIT, TokenType::TT0_DIGIT, TokenType::TT0_DIGIT, TokenType::TT0_DIGIT,
		// 58 - 64 :;<=>?@
		TokenType::TT1_COLON,
		TokenType::TT1_SEMICOLON,
		TokenType::TT1_LESS_THAN_SIGN,
		TokenType::TT1_EQUAL_SIGN,
		TokenType::TT1_GREATER_THAN_SIGN,
		TokenType::TT1_QUESTION_MARK,
		TokenType::TT1_AT_SYMBOL,
		// 65 - 90
		TokenType::TT0_ALPHA,
		TokenType::TT0_ALPHA, TokenType::TT0_ALPHA, TokenType::TT0_ALPHA, TokenType::TT0_ALPHA, TokenType::TT0_ALPHA,
		TokenType::TT0_ALPHA, TokenType::TT0_ALPHA, TokenType::TT0_ALPHA, TokenType::TT0_ALPHA, TokenType::TT0_ALPHA,
		TokenType::TT0_ALPHA, TokenType::TT0_ALPHA, TokenType::TT0_ALPHA, TokenType::TT0_ALPHA, TokenType::TT0_ALPHA,
		TokenType::TT0_ALPHA, TokenType::TT0_ALPHA, TokenType::TT0_ALPHA, TokenType::TT0_ALPHA, TokenType::TT0_ALPHA,
		TokenType::TT0_ALPHA, TokenType::TT0_ALPHA, TokenType::TT0_ALPHA, TokenType::TT0_ALPHA, TokenType::TT0_ALPHA,
		// 91 - 96 [\]^_`
		TokenType::TT1_LBRACKET,
		TokenType::TT1_BACK_SLASH,
		TokenType::TT1_RBRACKET,
		TokenType::TT1_CARET,
		TokenType::TT0_ALPHA, // TokenType::TT1__UNDERSCORE,
		TokenType::TT1_GRAVE_ACCENT,
		// 97 - 122
		TokenType::TT0_ALPHA,
		TokenType::TT0_ALPHA, TokenType::TT0_ALPHA, TokenType::TT0_ALPHA, TokenType::TT0_ALPHA, TokenType::TT0_ALPHA,
		TokenType::TT0_ALPHA, TokenType::TT0_ALPHA, TokenType::TT0_ALPHA, TokenType::TT0_ALPHA, TokenType::TT0_ALPHA,
		TokenType::TT0_ALPHA, TokenType::TT0_ALPHA, TokenType::TT0_ALPHA, TokenType::TT0_ALPHA, TokenType::TT0_ALPHA,
		TokenType::TT0_ALPHA, TokenType::TT0_ALPHA, TokenType::TT0_ALPHA, TokenType::TT0_ALPHA, TokenType::TT0_ALPHA,
		TokenType::TT0_ALPHA, TokenType::TT0_ALPHA, TokenType::TT0_ALPHA, TokenType::TT0_ALPHA, TokenType::TT0_ALPHA,
		// 123 - 126 {|}~
		TokenType::TT1_LBRACE,
		TokenType::TT1_VERTICAL_BAR,
		TokenType::TT1_RBRACE,
		TokenType::TT1_TILDE,
		// 127
		TokenType::TT0_NOTHING };

	size_t lex_init(LexerState& ls) {
		ls.buffer.clear();
		ls.tokens.clear();

		ls.eatChar = false;
		ls.lasttoken = nullptr;
		return 1;
	}

	size_t lex_quick_parse(const StringType& inputStr, int level, const std::vector<LexReplacementType>& replacements, TokenVector& tokens) {
		LexerState ls, ls2;
		lex_init(ls);
		lex(ls, inputStr, level);
		lex_replace(ls, ls2, replacements);
		lex_tokens(ls2, tokens);
		lex_init(ls);
		lex_init(ls2);
		return tokens.size();
	}

	size_t lex_quick_parse(const StringType& inputStr, TokenVector& tokens) {
		std::vector<LexReplacementType> replacements;
		return lex_quick_parse(inputStr, 2, replacements, tokens);
	}

	size_t lex_quick_l3_parse(const StringType& inputStr, TokenVector& tokens) {
		// L3, ignore # ... and // ... and group contiguous regions unless
		// they are integers, doubles, or strings
		std::vector<LexReplacementType> replacements;
		TokenVector tk;
		size_t count = lex_quick_parse(inputStr, 2, replacements, tk);

		tokens.clear();
		if (count == 0)
			return 0;
		size_t i = 0;
		while (i < count) {
			if (tk[i].IsType(TokenType::TT0_SPACE))
				;
			else if (tk[i].IsType(TokenType::TT1_NUMBER_SIGN))
				return 0;
			else if (
				tk[i].IsType(TokenType::TT1_FORWARD_SLASH) && i + 1 < count && tk[i + 1].IsType(TokenType::TT1_FORWARD_SLASH))
				return 0;
			i++;
		}

		Token tmp;
		size_t length = 0;
		bool tokenPushed = false;
		for (auto it : tk) {
			//TokenType type = it.type;
			if (length == 0 && (it.IsDouble() || it.IsInteger() || it.IsString())) {
				tokens.push_back(it);
				tokenPushed = true;
			}
			else if (!it.IsType(TokenType::TT0_SPACE) && !it.IsType(TokenType::TT0_TAB) && !it.IsType(TokenType::TT0_NEWLINE)) {
				tmp.sval += it.sval;
				length += it.sval.length();
			}
			else if (length > 0) {
				// add a new token
				tmp.type = TokenType::TT2_STRING;
				tokens.push_back(tmp);
				tokenPushed = true;
			}

			if (tokenPushed) {
				tmp.ival = 0;
				tmp.dval = 0.0;
				tmp.sval.clear();
				length = 0;
				tokenPushed = false;
			}
		}

		if (length > 0) {
			// add a new token
			tmp.type = TokenType::TT2_STRING;
			tokens.push_back(tmp);
			tokenPushed = true;
		}

		return tokens.size();
	}

	size_t lex_quick_l2_parse(const StringType& inputStr, TokenVector& tokens) {
		std::vector<LexReplacementType> replacements;
		replacements.push_back(Df::LexReplacementType(Df::TokenType::TT0_SPACE, Df::TokenType::TT0_NOTHING, '\0'));
		return lex_quick_parse(inputStr, 2, replacements, tokens);
	}

	size_t lex(LexerState& ls, const StringType& inputStr, int level) {
		// level >= 0: keep character classes contiguous
		// level >= 1: punctuation character classes are not continguous separate
		// level >= 2: ...

		// Append input string to buffer, recording the start position
		// so we can continue lexing through the input.
		size_t start = ls.buffer.size();
		ls.buffer += inputStr;
		size_t length = ls.buffer.length();

		// reserve enough memory if every character is a token.
		ls.tokens.reserve(ls.tokens.size() + inputStr.size());

		//bool gotCR = false;
		//bool gotDQ = false;
		//bool gotSQ = false;
		//bool gotbackslash = false;
		bool eatChar = false;
		bool keepContinguous = false;
		ls.lasttoken = nullptr;
		//bool wasnonprint = true;

		for (auto i = start; i < length; i++) {
			if (ls.eatChar == false) {
				ls.tmptoken.type = TokenType::TT0_NOTHING;
				ls.tmptoken.start = i;
				ls.tmptoken.length = 0;
				ls.tmptoken.cptr = &ls.buffer[i];
			}
			else {
				ls.tmptoken.length++;
			}

			// determine type of character
			ls.eatChar = false;
			keepContinguous = true;
			bool gobbledL2 = false;
			bool gobbledL1 = false;
			int c = ls.buffer[i];
			//int peekc = (i + 1 < length) ? ls.buffer[i + 1] : 0;

			if (c < 0 || c >= 128) {
				ls.tmptoken.type = TokenType::TT0_NOTHING;
				continue;
			}

			if (level >= 2) {
				size_t len = lex_scan_string(&ls.buffer[i], ls.tmptoken);
				if (len == 0) {
					len = lex_scan_number(&ls.buffer[i], ls.tmptoken);
				}
				if (len > 0) {
					gobbledL2 = true;
					i += len - 1;
				}
				else {
					if (isalpha(c) || c == '_') {
						ls.tmptoken.type = TokenType::TT2_ID;
						gobbledL2 = true;
						ls.tmptoken.length++;
					}
				}
			}
			if (!gobbledL2 && level >= 1) {
				if (isalpha(c) || (level >= 1 && c == '_')) {
					gobbledL1 = true;
					ls.tmptoken.type = TokenType::TT0_ALPHA;
					ls.tmptoken.length++;
				}
				else {
					gobbledL1 = true;
					ls.tmptoken.type = _tokenLookup[c];
					ls.tmptoken.length++;
				}
				if (ispunct(c) && c != '_')
					keepContinguous = false;
			}
			if (!gobbledL2 && !gobbledL1 && level >= 0) {
				if (isalpha(c)) {
					ls.tmptoken.type = TokenType::TT0_ALPHA;
				}
				else if (isdigit(c)) {
					ls.tmptoken.type = TokenType::TT0_DIGIT;
				}
				else if (ispunct(c)) {
					ls.tmptoken.type = TokenType::TT0_PUNCT;
					if (level >= 1)
						keepContinguous = false;
				}
				else if (iscntrl(c)) {
					ls.tmptoken.type = TokenType::TT0_CNTRL;
					if (c == '\r') {
						// peek at next character
						if (i + 1 < length && ls.tmptoken.cptr[1] == '\n')
							eatChar = true;
					}
					else if (c == '\n') {
						ls.tmptoken.type = TokenType::TT0_NEWLINE;
					}
					else if (c == '\t') {
						ls.tmptoken.type = TokenType::TT0_TAB;
					}
				}
				else if (isblank(c)) {
					ls.tmptoken.type = TokenType::TT0_SPACE;
				}
				else {
					ls.tmptoken.type = TokenType::TT0_CHAR;
					if (level >= 1)
						keepContinguous = false;
				}
				ls.tmptoken.length++;
			}

			// special case: if we've already identified we need to eat another character,
			// go ahead and just do it.
			if (eatChar)
				continue;

			// skip "nothing" characters
			if (ls.tmptoken.type == TokenType::TT0_NOTHING)
				continue;

#ifdef DAMSELFISH_EXTRA_DEBUGGING
			std::cout << i << " " << c << " " << ls.tmptoken.type << std::endl;
#endif

			// finally, add token to list

			// case 1: we have no previous token, so add it to the end of the list
			if (ls.lasttoken == nullptr) {
				ls.tokens.push_back(ls.tmptoken);
				ls.lasttoken = &ls.tokens.back();
			}
			// case 2: we have a previous token and it is the same type
			// increase the length of the last token.
			else if (keepContinguous && ls.lasttoken->type == ls.tmptoken.type) {
				ls.lasttoken->length++;
			}
			// case 3: we have a previous token, but it is not the same type
			// so add another token to the end of the list
			else {
				ls.tokens.push_back(ls.tmptoken);
				ls.lasttoken = &ls.tokens.back();
			}
		}

		ls.tokens.shrink_to_fit();

		// std::vector<T>::back() doesn't check if element exists.
		if (ls.tokens.empty()) {
			ls.lasttoken = nullptr;
		}
		else {
			ls.lasttoken = &ls.tokens.back();
		}

		return ls.tokens.size();
	}

	void lex_print(LexerState& ls) {
		for (auto it = ls.tokens.begin(); it != ls.tokens.end(); it++) {
			std::cout << "type: " << it->type << ", start: " << it->start << ", length: " << it->length << std::endl;
		}
	}

	void lex_tokens_print(std::vector<Token>& tokens) {
		for (auto it = tokens.begin(); it != tokens.end(); it++) {
			std::cout << "type: " << it->type << ", " << it->sval << std::endl;
		}
	}

	void lex_replace(LexerState& ls, LexerState& output,
					 const std::vector<LexReplacementType>& replacements) {
		lex_init(output);
		output.buffer = ls.buffer;
		output.tokens.clear();
		output.eatChar = false;
		output.lasttoken = nullptr;
		for (auto it = ls.tokens.begin(); it != ls.tokens.end(); it++) {
			bool skipped = false;
			bool pushed = false;
			for (auto r = replacements.begin(); r != replacements.end(); r++) {
				TokenType fmType = std::get<0>(*r);
				TokenType toType = std::get<1>(*r);
				StringType::value_type c = std::get<2>(*r);

				if (it->type == fmType) {
					if (toType == TokenType::TT0_NOTHING) {
						skipped = true;
						break;
					}
					if (it->cptr && *(it->cptr) == c) {
						if (!pushed) {
							output.tokens.push_back(*it);
							LexerToken& back = output.tokens.back();
							back.type = toType;
							pushed = true;
						}
						break;
					}
				}
			}

			// leave as-is
			if (!pushed && !skipped) {
				output.tokens.push_back(*it);
			}
		}
	}

	size_t lex_scan_string(StringType::value_type* cptr, LexerToken& token) {
		if (cptr == nullptr)
			return 0;

		size_t length = 0;
		int state = 0;
		StringType::value_type c;

		if (state == 0) {
			c = *cptr;
			if (c == '\"')
				state = 1;
			else
				return 0;
			length++;
			cptr++;
		}

		while (state == 1) {
			if (state == 1) {
				c = *cptr;
				if (c == '\"')
					state = 2;
				else if (c == '\\')
					state = 3;
				length++;
				cptr++;
			}

			// state == 2 will cause the loop to terminate
			if (state == 2)
				break;

			if (state == 3) {
				c = *cptr;
				switch (c) {
				case '\"':
				case '\\':
				case '/':
				case 'b':
				case 'f':
				case 'n':
				case 'r':
				case 't':
					state = 1;
					break;
				case 'u':
					state = 4;
					break;
				default:
					// error, invalid literal...ignore...
					state = 1;
					break;
				}
				length++;
				cptr++;
			}

			while (state >= 4 && state <= 7) {
				c = *cptr;
				if (isxdigit(c))
					state++;
				// error, invalid literal...ignore...
				else
					state = 1;
				length++;
				cptr++;
			}

			if (state == 8) {
				state = 1;
			}
		}

		token.length = length;
		token.type = TokenType::TT2_STRING;

		return length;
	}

	size_t lex_scan_number(StringType::value_type* cptr, LexerToken& token) {
		if (cptr == nullptr)
			return 0;

		size_t length = 0;
		size_t hexdigits = 0;
		int state = 0;
		StringType::value_type c;

		if (state == 0) {
			c = *cptr;
			// start state
			if (c == '-')
				state = 1;
			else if (c == '0')
				state = 3;
			else if (c == '.')
				state = 4;
			else if (c >= '1' && c <= '9')
				state = 2;
			else
				return 0;
			length++;
			cptr++;
		}

		if (state == 1) {
			c = *cptr;
			if (c == '.')
				state = 4;
			else if (c == '0')
				state = 3;
			else if (c >= '1' && c <= '9')
				state = 2;
			else
				return 0;
			length++;
			cptr++;
		}

		while (state == 2) {
			c = *cptr;
			if (c >= '0' && c <= '9')
				state = 2;
			else if (c == '.')
				state = 5;
			else {
				token.length = length;
				token.type = TokenType::TT2_INTEGER;
				return length;
			}
			length++;
			cptr++;
		}

		if (state == 3) {
			c = *cptr;
			if (c == '.')
				state = 5;
			else if (c == 'x')
				state = 9;
			else {
				token.length = length;
				token.type = TokenType::TT2_INTEGER;
				return length;
			}
			length++;
			cptr++;
		}

		if (state == 4) {
			c = *cptr;
			if (c >= '0' && c <= '9')
				state = 5;
			else
				return 0;
			length++;
			cptr++;
		}

		while (state == 5) {
			c = *cptr;
			if (c >= '0' && c <= '9')
				state = 5;
			else if (c == 'e' || c == 'E')
				state = 6;
			else {
				token.length = length;
				token.type = TokenType::TT2_DOUBLE;
				return length;
			}
			length++;
			cptr++;
		}

		if (state == 6) {
			c = *cptr;
			if (c >= '0' && c <= '9')
				state = 8;
			else if (c == '-' || c == '+')
				state = 7;
			else
				return 0;
			length++;
			cptr++;
		}

		if (state == 7) {
			c = *cptr;
			if (c >= '0' && c <= '9')
				state = 8;
			else
				return 0;
			length++;
			cptr++;
		}

		while (state == 8) {
			c = *cptr;
			if (c >= '0' && c <= '9')
				state = 8;
			else {
				token.length = length;
				token.type = TokenType::TT2_DOUBLE;
				return length;
			}
			length++;
			cptr++;
		}

		if (state == 9) {
			c = *cptr;
			if (c >= '0' && c <= '9')
				state = 10;
			else if (c >= 'a' && c <= 'f')
				state = 10;
			else if (c >= 'A' && c <= 'F')
				state = 10;
			else
				return 0;
			length++;
			cptr++;
			hexdigits++;
		}

		while (state == 10) {
			c = *cptr;
			if (c >= '0' && c <= '9')
				state = 10;
			else if (c >= 'a' && c <= 'f')
				state = 10;
			else if (c >= 'A' && c <= 'F')
				state = 10;
			else {
				token.length = length;
				token.type = TokenType::TT2_HEXINTEGER;
				return length;
			}
			length++;
			cptr++;
			hexdigits++;
			if (hexdigits > 8)
				return 0;
		}

		token.length = length;

		return length;
	}

	size_t lex_tokens(LexerState& ls, std::vector<Token>& tokens) {
		tokens.clear();
		tokens.resize(ls.tokens.size());
		size_t i = 0;
		for (auto it = ls.tokens.begin(); it != ls.tokens.end(); it++) {
			tokens[i].type = it->type;
			if (it->type != TokenType::TT2_STRING) {
				tokens[i].sval = StringType(it->cptr, it->length);
			}
			else {
				tokens[i].sval = StringType(it->cptr + 1, it->length - 2);
			}
			if (it->type == TokenType::TT2_DOUBLE) {
				tokens[i].dval = strtod(tokens[i].sval.c_str(), nullptr);
				tokens[i].ival = tokens[i].dval > INT_MAX ? INT_MAX : tokens[i].dval < INT_MIN ? INT_MIN : (int)tokens[i].dval;
			}
			else if (it->type == TokenType::TT2_HEXINTEGER) {
				tokens[i].ival = strtol(tokens[i].sval.c_str(), nullptr, 16);
				tokens[i].dval = tokens[i].ival;
			}
			else if (it->type == TokenType::TT2_INTEGER) {
				tokens[i].ival = strtol(tokens[i].sval.c_str(), nullptr, 10);
				tokens[i].dval = tokens[i].ival;
			}
			i++;
		}
		return tokens.size();
	}

	std::string TokenVectorJoin(const TokenVector tokens, const std::string& separator) {
		std::ostringstream ostr;
		std::string result;
		size_t i = 0;
		for (auto t : tokens) {
			if (i != 0)
				ostr << separator;
			ostr << t.sval;
			i++;
		}
		result = ostr.str();
		return result;
	}

	/*
			int lex_kasl(LexerState &ls, LexerState &output)
			{
			// converts "strings", myIdentifer, 123456, 3.14124e12
			// token -> id | string | integer | double | <ANY>
			// id -> TT0_ALPHA
			// string -> " (\" | \' | \n | \t | <ANY>) "
			// 0 | ALPHA | DIGIT | DASH | DQUOTE | <ANY>
			// 1 | ID    | 2     | 2
			// 2 |
			lex_init(output);
			output.buffer = ls.buffer;

			int row, col;

			bool epsilon = true;
			for (auto it = ls.tokens.begin(); it != ls.tokens.end(); it++)
			{
			if (epsilon)
			{
			ls.tmptoken.type = TokenType::TT0_NOTHING;
			ls.tmptoken.start = it->start;
			ls.tmptoken.length = it->length;
			ls.tmptoken.cptr = it->cptr;
			}

			epsilon = false;
			if (ls.tmptoken.type == TokenType::TT0_ALPHA)
			{
			ls.tmptoken.type = TokenType::TT2_ID;
			epsilon = true;
			}
			}
			}
			*/
} // namespace Df
