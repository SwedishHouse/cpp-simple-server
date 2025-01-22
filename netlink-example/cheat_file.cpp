
#include "cheat_file.h"

FileManager::FileManager(const std::string &filename) : filename(filename) {}

void FileManager::writeNumber(unsigned int number) {
    std::ofstream file(filename);
    if (!file) {
        std::cerr << "Ошибка открытия файла для записи" << std::endl;
        return;
    }
    file << number << std::endl;
    file.close();
}

unsigned int FileManager::readNumber() {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Ошибка открытия файла для чтения" << std::endl;
        return -1; // Возвращаем -1 в случае ошибки
    }
    int number;
    file >> number;
    file.close();
    return number;
}
