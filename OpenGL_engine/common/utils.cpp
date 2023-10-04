#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

std::string Get_Extension(std::string file_name)
{
    //store the position of last '.' in the file name
    int position = file_name.find_last_of(".");

    //store the characters after the '.' from the file_name string
    std::string extension = file_name.substr(position);

    return extension;
}

std::string Change_File(const char* orignal_path, const char* new_file_name)
{
    //store the position of last '/' in the file name
    int position = ((std::string)orignal_path).find_last_of("/");
    //store the characters before the '/' from the orignal_path string
    std::string path = ((std::string)orignal_path).substr(0, position+1);
    path += (std::string)new_file_name;
    return path;
}