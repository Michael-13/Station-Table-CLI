#ifndef STATIONDBCLIENT_H
#define STATIONDBCLIENT_H

#include "utils.h"
#include "ShellUtils.h"


class stationDBClient : public ShellUtils {
    public:
        stationDBClient() {}

        void printHelp() {
            std::cout << "Options:" << std::endl;
            std::cout << "    -A, --add <FILE>                   : Performs bulk-insertion of given csv file. Requires valid file name or full path" << std::endl;
            std::cout << "    -C, --checkStation <IRSTATIONCODE> : Performs a check request on the given station code" << std::endl;
            std::cout << "    -h, --help                         : Prints the current help" << std::endl;
            std::cout << "    -S, --seismo  <SEISMOGRAM>         : Performs a check request on the given seismogram" << std::endl;
        }

        void checkStation(std::string code) {
            _setmode(_fileno(stdout), 0x00020000);
            std::wcout << std::endl;
            SetConsoleTextAttribute(h,8);
            std::wcout << L" ▶  Search Station: ";
            SetConsoleTextAttribute(h,7); 
            std::wcout << code.c_str() << '\n' << std::endl;

            auto t1 = high_resolution_clock::now();
            std::vector<std::string> s = getStationInfo(code);
            std::vector<std::vector<std::string>> i = getInstrumentInfo(code);
            auto t2 = high_resolution_clock::now();

            std::wcout << L" Site:               ";
            handleNA(s[0]);
            std::wcout << L" FDSNCode:           ";
            handleNA(s[2]);
            std::wcout << L" Latitude:           ";
            handleNA(s[3]);
            std::wcout << L" Longitude:          ";
            handleNA(s[4]);
            std::wcout << L" Elevation:          ";
            handleNA(s[5]);
            std::wcout << L" Depth:              ";
            handleNA(s[6]);
            std::wcout << L" CreationDate:       ";
            handleNA(s[7]);
            std::wcout << L" TerminationDate:    ";
            handleNA(s[8]);
            std::wcout << L" Instrument History:" << std::endl;
            int c = 0;
            for (auto &v : i) {
                if (c != 0 && c % 10 == 0) {
                    char cmd;
                    std::wcout << " (" << c << "/" << i.size() 
                    << ") have been printed. Would you like to print more? (Y/N)";
                    std::cin >> cmd;
                    if (cmd == 'N')  {
                        break;
                    }
                }
                std::wcout << L"  \u2192 InstrumentEraStart: ";
                handleInst(v[1]);
                std::wcout << L" InstrumentEraEnd: "; 
                handleInst(v[2]);
                std::wcout << " Instrument: ";
                handleInst(v[3]);
                std::wcout << std::endl;
                ++c;
            }
            
            duration<double, std::milli> ms_double = t2 - t1;
            printRequestTime(ms_double);
            printEndBanner();
        }
};

#endif