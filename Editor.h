#pragma once
#include <vector>
#include <string>

class Editor
{
	public:
		std::string RemoveComments(std::string source);
	private:
		enum TokenType { tkOther, tkSlash, tkAsterisk, tkNewLine };
		enum State { stError, stWaitFirstSlash, stWaitSlashAst, stWaitNewLine, stRemoveComment = 6 };

		const std::vector<std::vector<int>> states = {
			{0, 0, 0, 0},		// error 0
			{1, 2, 1, 1},		// wait for '/' 1
			{1, 3, 4, 1},		// wait for '/' or '*' 2
			{3, 3, 3, 6},		// wait for '\n' if '//' opened 3
			{4, 4, 5, 4},		// wait for '*' if '/*' opened 4
			{4, 6, 4, 4},		// wait for '/' if got '*' 5
			{1, 1, 1, 1},		// state for deleting comment 6
		};

		static inline TokenType getTokenType(char token) {
			return token == '/' ? tkSlash :
				token == '*' ? tkAsterisk :
				token == '\n' ? tkNewLine : tkOther;
		}
};

