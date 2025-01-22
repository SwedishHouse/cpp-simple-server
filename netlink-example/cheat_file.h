#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <string>

const std::string path_to_info = "server_info";

class FileManager {
public:
    FileManager(const std::string &filename);

    void writeNumber(unsigned int number);
    unsigned int readNumber();

private:
    std::string filename;
};
