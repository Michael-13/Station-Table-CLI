#ifndef SHELLUTILS_H
#define SHELLUTILS_H
#include "utils.h"
#include <iostream>
#include <windows.h>


class ShellUtils {
    protected:
        HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);    
    public:
        ShellUtils() {}

        void printRequestTime(duration<double, std::milli> t) {
            SetConsoleTextAttribute(h,8);
            std::wcout << L"\n Request Time: ";
            SetConsoleTextAttribute(h,7); 
            std::wcout << t.count() << "ms" << std::endl;
        }

        void printEndBanner() {
            SetConsoleTextAttribute(h,8);
            std::wcout << L" ----------------------------------------------------" << std::endl;
            std::wcout << L" Michael-13\\StationDB v0.0.1" << std::endl;
            SetConsoleTextAttribute(h,7); 
        }

        void handleNA(std::string s) {
            (s == "N/A") ? SetConsoleTextAttribute(h,4) : SetConsoleTextAttribute(h,10);
            std::wcout << s.c_str() << std::endl;
            SetConsoleTextAttribute(h,7);
        }

        void handleInst(std::string s) {
            SetConsoleTextAttribute(h,14);
            std::wcout << s.c_str();
            SetConsoleTextAttribute(h,7);
        }
};
#endif
