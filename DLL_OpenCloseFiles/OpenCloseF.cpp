#include <fstream>
#include <iostream>
#include "OpenCloseF.h"


extern "C" OpenCloseF_API std::ifstream * OpenFiles(const std::string & filePath) {
    auto* file = new std::ifstream(filePath);
    if (!file->is_open()) {
        delete file;
        throw std::runtime_error("Не удалось открыть файл: " + filePath);
    }
    return file;
}

extern "C" OpenCloseF_API void CloseFiles(std::ifstream * fileStream) {
    if (fileStream) {
        fileStream->close();
        delete fileStream;
    }
}
