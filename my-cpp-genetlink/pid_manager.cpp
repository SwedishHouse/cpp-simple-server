#include "pid_manager.h"
#include <iostream>
#include <fstream>

// Метод для записи PID в файл
void PIDManager::writePIDToFile(const std::string& filename, int value) {
    std::ofstream file(filename);
    if (file.is_open()) {
        file << value << std::endl;
        file.close();
        std::cout << "Значение " << value << " записано в файл: " << filename << std::endl;
    } else {
        std::cerr << "Не удалось открыть файл для записи." << std::endl;
    }
}

// Метод для считывания PID из файла
pid_t PIDManager::readPIDFromFile(const std::string& filename) {
    std::ifstream file(filename);
    pid_t pid = 0;
    if (file.is_open()) {
        file >> pid;
        file.close();
        std::cout << "PID считан из файла: " << filename << std::endl;
    } else {
        std::cerr << "Не удалось открыть файл для чтения." << std::endl;
    }
    return pid;
}