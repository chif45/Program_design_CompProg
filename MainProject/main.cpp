#include <iostream>
#include <Windows.h>
#include <list>
#include <fstream>
#include <locale.h>
#include "..\DLL_OpenCloseFiles\OpenCloseF.h"

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "RUS");

    if (argc < 2) {
        return 1;
    }
    std::string path = argv[1];

    std::list<std::string> titles, paragraphs, boldTexts, files;

    HINSTANCE loadSearch;

    loadSearch = LoadLibrary("DLLSearch.dll");

    if (!loadSearch) {
        std::cerr << "Ошибка при загрузке DLL" << std::endl;
        return 1;
    }

    typedef std::list<std::string> (*extract) (const std::string&);
    typedef void (*printe) (const std::list<std::string>&, const std::string&);
    typedef std::list<std::string>(*findfiles) (const std::string& directory, const std::string& extension);

    extract extractTitles = (extract)GetProcAddress(loadSearch, "ExtractTitles");
    extract extractParagraphs = (extract)GetProcAddress(loadSearch, "ExtractParagraphs");
    extract extractBoldText = (extract)GetProcAddress(loadSearch, "ExtractBoldTexts");
    printe printelem = (printe)GetProcAddress(loadSearch, "printElements");
    findfiles findFilesWithExtensionHTML = (findfiles)GetProcAddress(loadSearch, "findFilesWithExtension");

    if (!findFilesWithExtensionHTML) {
        std::cerr << "Ошибка получения адреса функции findFilesWithExtension" << std::endl;
        FreeLibrary(loadSearch);
        return 1;
    }

    files = findFilesWithExtensionHTML(path,".html");
    printelem(files, "Найденные файлы");

    while(!files.empty()) {
        std::cout << "------------------------------\n";
        std::cout << "Поиск в файле: "<<files.front() << std::endl;
        try {
            std::ifstream* file = OpenFiles(files.front());
            if (file == nullptr) {
                std::cerr << "Ошибка при открытии файла: " << path << std::endl;
                CloseFiles(file);
                return 1;
            }

        }
        catch (std::runtime_error) {
            std::cerr << ("Данного файла не существует") << path << std::endl;
            return 1;
        }

        std::ifstream* file = OpenFiles(files.front());
        files.pop_front();
        std::string content((std::istreambuf_iterator<char>(*file)), std::istreambuf_iterator<char>());
        CloseFiles(file);
       
        std::list<std::string>Paragraphs = extractParagraphs(content);
        std::list<std::string>Titles = extractTitles(content);
        std::list<std::string>BoldTexts = extractBoldText(content);

        if (Paragraphs.empty()) {
            std::cout<<"Абзацев нет \n"<<std::endl;
        }
        else {
            printelem(Paragraphs, "Абзацы");
        }

        if (Titles.empty()) {
            std::cout << "Заголовков нет \n" << std::endl;
        }
        else {
            printelem(Titles, "Заголовки");
        }

        if (BoldTexts.empty()) {
            std::cout << "Жирного текста нет \n" << std::endl;
        }
        else {
            printelem(BoldTexts, "Жирный текст");
        }

        content.clear();
        Titles.clear();
        Paragraphs.clear();
        BoldTexts.clear();
    }
    FreeLibrary(loadSearch);
}