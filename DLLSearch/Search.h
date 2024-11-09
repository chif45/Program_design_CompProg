#pragma once
#include <filesystem>
#include <iostream>
#include <string>
#include <list>

void printElements(const std::list<std::string>&, const std::string&);
std::list<std::string> ExtractTitles(const std::string&);
std::list<std::string> ExtractParagraphs(const std::string&);
std::list<std::string> ExtractBoldTexts(const std::string&);
std::list<std::string> findFilesWithExtension(const std::string& directory, const std::string& extension);


