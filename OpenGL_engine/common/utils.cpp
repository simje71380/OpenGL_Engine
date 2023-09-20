#include "utils.h"

std::string Get_Extension(std::string file_name)
{
    //store the position of last '.' in the file name
    int position = file_name.find_last_of(".");

    //store the characters after the '.' from the file_name string
    std::string extension = file_name.substr(position);

    return extension;
}