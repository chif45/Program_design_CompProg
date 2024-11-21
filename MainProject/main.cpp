#include <iostream>
#include <Windows.h>
#include <list>
#include <fstream>
#include <locale.h>
#include "..\DLL_OpenCloseFiles\OpenCloseF.h"
#include <thread>
#include <mutex>

std::mutex contents_mutex;

struct contentFile {
    std::string content;
    std::string path;
};

std::list<contentFile>contents;

std::list<std::thread> threads;

void OpenGetContentClose(std::string path) {
    try {
        std::ifstream* file = OpenFiles(path);
        if (file == nullptr) {
            std::cerr << "Ошибка при открытии файла: " << path << std::endl;
            CloseFiles(file);
            return;
        }

    }
    catch (std::runtime_error) {
        std::cerr << ("Данного файла не существует") << path << std::endl;
        return;
    }

    std::ifstream* file = OpenFiles(path);
    std::string content((std::istreambuf_iterator<char>(*file)), std::istreambuf_iterator<char>());
    CloseFiles(file);

    contents_mutex.lock();
    contents.push_back({ content, path });
    contents_mutex.unlock();
}


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

    files = findFilesWithExtensionHTML(path,".html");
    printelem(files, "Найденные файлы");

    while(!files.empty()) {
        threads.emplace_back(OpenGetContentClose, files.front());
        files.pop_front();
    }
    
    for (auto& t : threads) {
        if (t.joinable()) {
            t.join();
        }
    }
    
    while(!contents.empty()) {
        
        std::cout <<"В файле: "<< contents.front().path << std::endl;
        std::string content = contents.front().content;

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

        contents.pop_front();
    }
    FreeLibrary(loadSearch);
}