#ifndef SHELLUTILS_H
#define SHELLUTILS_H
#include "utils.h"
#include <iostream>
//#include <windows.h>


class ShellUtils { 
    public:
        ShellUtils() {}

        void printRequestTime(duration<double, std::milli> t) {
            const auto now = std::chrono::system_clock::now();
            const std::time_t t_c = std::chrono::system_clock::to_time_t(now);
            std::cout << "\n\033[0;90m Request Time: ";
            std::cout << t.count() << "ms | UTC Time: " << std::ctime(&t_c) << "\033[0m";
        }

        void printEndBanner() {
            std::cout << "\033[0;90m ------------------------------------------------------------" << std::endl;
            std::cout << " Michael-13\\StationDB v0.0.1\033[0m" << std::endl;
        }

        void handleNA(std::string s) {
            std::cout << ((s == "N/A") ? "\033[1;31m" : "\033[1;32m");
            std::cout << s.c_str() << "\033[0m" << std::endl;
        }

        void handleInst(std::string s) {
            std::cout << "\033[1;33m" << s.c_str() << "\033[0m";
        }
};
#endif

