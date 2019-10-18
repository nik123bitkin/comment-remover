#include "Editor.h"

std::string Editor::RemoveComments(std::string source)
{
	State state = stWaitFirstSlash;
	auto prev_it = source.begin();

	for (auto it = source.begin(); it != source.end(); ++it) {

		TokenType tokenType = getTokenType(*it);
		state = static_cast<State>(states[state][tokenType]);

		if (state == stError) {
			//cout << "Source code contains syntax error" << endl;
			break;
		}

		if (state == stWaitSlashAst) {
			prev_it = it;
		}

		if (state == stRemoveComment) {
			source.erase(prev_it, *(it) == '/' ? it + 1 : it);
			it = prev_it;
			state = stWaitFirstSlash;
		}
	}

	if (state == stWaitNewLine) {
		source.erase(prev_it, source.end());
	}

	return source;
}
