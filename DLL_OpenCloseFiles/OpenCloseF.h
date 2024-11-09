#pragma once
#include <fstream>
#include <stdexcept>

#ifdef OpenCloseF_EXPORTS
#define OpenCloseF_API __declspec(dllexport)
#else
#define OpenCloseF_API __declspec(dllimport)
#endif

extern "C" OpenCloseF_API std::ifstream * OpenFiles(const std::string & fileName);
extern "C" OpenCloseF_API void CloseFiles(std::ifstream * fileStream);
