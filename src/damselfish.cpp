#include "damselfish_pch.hpp"
#include <damselfish.hpp>

//#define DAMSELFISH_INTERPRETER

#ifdef DAMSELFISH_INTERPRETER
int main(int argc, char** argv) {
	std::ifstream fin;
	bool openedfile = false;

	if (argc >= 2) {
		fin.open(argv[1], ios::in);
		if (!fin) {
			cerr << "Unable to open input file.\n";
			return -1;
		}
		openedfile = true;
	}
	istream& is = !fin ? cin : fin;

	Df::LexerState ls, newls;
	std::vector<Df::LexReplacementType> replacements;
	replacements.push_back(Df::LexReplacementType(Df::TokenType::TT0_SPACE, Df::TokenType::TT0_NOTHING, '\0'));

	std::string newline = "\r\n";
	int level = 0;
	lex_init(ls);
	while (cin) {
		std::string inputstr;
		getline(cin, inputstr);
		if (inputstr == "quit")
			break;
		if (inputstr == "clear") {
			lex_init(ls);
			continue;
		}
		if (inputstr == "level0") {
			level = 0;
			continue;
		}
		if (inputstr == "level1") {
			level = 1;
			continue;
		}
		if (inputstr == "level2") {
			level = 2;
			continue;
		}
		if (inputstr == "print") {
			Df::lex_print(ls);
			continue;
		}
		if (inputstr == "replace") {
			Df::lex_replace(ls, newls, replacements);
			Df::lex_print(newls);
			continue;
		}
		inputstr += "\n";
		Df::lex(ls, inputstr, level);
	}

	if (openedfile) {
		fin.close();
	}
	return 0;
}
#endif

