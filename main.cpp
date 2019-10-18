#include <iostream>
#include <fstream>

#include "Editor.h"

int main(int argc, char *argv[])
{
	setlocale(LC_ALL, "Russian");

	if (argc < 3) {
		std::cout << "Invalid number of arguments" << std::endl;
		return 1;
	}

	std::ifstream instream(argv[1]);
	if (!instream.good()) {
		std::cout << "Error occured during opening source file" << std::endl;
		return 1;
	}

	std::string source((std::istreambuf_iterator<char>(instream)), (std::istreambuf_iterator<char>()));

	Editor editor;
	std::string dest = editor.RemoveComments(source);

	std::ofstream outstream(argv[2]);
	if (!outstream.good()) {
		std::cout << "Error occured during opening destination file" << std::endl 
			<< "Edited source code is shown below: " << std::endl;
		std::cout << dest << std::endl;
		return 1;
	}
	outstream << dest;
	std::cout << "Finished..." << std::endl;
    getchar();
	return 0;
}
