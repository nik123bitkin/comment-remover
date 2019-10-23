#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

bool GetFileContent(const std::string &file_name, std::vector<std::string> &lines) {
    std::ifstream in(file_name.c_str());
    if (!in) {
        std::cerr << "Cannot open the file : " << file_name << std::endl;
        return false;
    }

    std::string line;
    while (std::getline(in, line)) {
        lines.push_back(line);
    }
    lines.emplace_back("\n");

    in.close();
    return true;
}

void TrimRight(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

std::string DeleteComments(std::vector<std::string> lines) {
    std::string dest;
    int is_multi_line_comment = -1;
    size_t start_pos = 0;
    size_t single_comment_pos;
    size_t multi_comment_pos;
    size_t multi_comment_end_pos;

    for (auto &line : lines) {
        TrimRight(line);
        dest.append(line);

        if (line.back() != '\\') {
            dest.push_back('\n');
            do {
                single_comment_pos = dest.find("//", start_pos);
                multi_comment_pos = dest.find("/*", start_pos);
                multi_comment_end_pos = dest.find("*/", start_pos);

                if (single_comment_pos < multi_comment_pos &&
                    single_comment_pos != std::string::npos &&
                    is_multi_line_comment == -1) {

                    dest = dest.substr(0, single_comment_pos);
                    dest.push_back('\n');
                    start_pos = dest.size() - 1;
                } else if (is_multi_line_comment > -1) {
                    if (multi_comment_end_pos != std::string::npos) {
                        dest.erase(is_multi_line_comment, multi_comment_end_pos - is_multi_line_comment + 2);
                        start_pos = is_multi_line_comment;
                        is_multi_line_comment = -1;
                    }
                } else if (multi_comment_pos != std::string::npos) {
                    is_multi_line_comment = multi_comment_pos;
                    start_pos = multi_comment_pos + 2;
                }

            } while ((multi_comment_pos != std::string::npos && (is_multi_line_comment > -1)) ||
                     (single_comment_pos != std::string::npos && (is_multi_line_comment == -1)));
        } else {
            dest.pop_back();
        }
    }
    return dest;
}

int main(int argc, char *argv[]) {
    setlocale(LC_ALL, "Russian");

    if (argc < 3) {
        std::cout << "Invalid number of arguments" << std::endl;
        return 1;
    }

    std::vector<std::string> lines;
    bool result = GetFileContent(argv[1], lines);
    if (!result) {
        return 1;
    }

    std::string dest = DeleteComments(lines);

    std::ofstream outstream(argv[2]);
    if (!outstream.good()) {
        std::cout << "Error occured during opening destination file" << std::endl
                  << "Edited source code is shown below: " << std::endl
                  << dest << std::endl;
        return 1;
    }

    outstream << dest;
    std::cout << "Finished..." << std::endl;
    return 0;
}
