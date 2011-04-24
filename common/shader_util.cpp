#include <cstring>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

#include "shader_util.h"

const char* loadShader(const char* file_name) {
    std::ifstream file;
    std::ostringstream file_content_stream;
    std::string file_content;

    file.open(file_name);

    if (file.is_open()) {
        file_content_stream << file.rdbuf();
        file_content = file_content_stream.str();
        file.close();
    }

    char* return_data = new char[file_content.length()+1];
    strcpy(return_data, file_content.c_str());

    return return_data;
}
