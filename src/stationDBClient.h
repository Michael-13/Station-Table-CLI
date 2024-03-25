#ifndef STATIONDBCLIENT_H
#define STATIONDBCLIENT_H

#include "utils.h"
#include "ShellUtils.h"


class stationDBClient : public ShellUtils {
    public:
        stationDBClient() {}

        void printHelp() {
            std::cout << "\nUsage: stationdb [-A file] [-C code] [-h] [-S seismo]\n\n";
            std::cout << "Options:" << std::endl;
            std::cout << "    -A, --add <FILE>                        : Performs bulk-insertion of given csv file.\n";
            std::cout << "                                              Requires valid file name or full path" << std::endl;
            std::cout << "    -C, --checkStation <IRSTATIONCODE> [-d] : Performs a check request on the given station code.\n";
            std::cout << "                                              If -d is used the user will download the station's information" << std::endl;
            std::cout << "    -h, --help                              : Prints the current help" << std::endl;
            std::cout << "    -S, --seismo  <SEISMOGRAM>              : Performs a check request on the given seismogram" << std::endl;
        }

        void downloadStation(std::string code, std::string path) {
            std::vector<std::string> s = getStationInfo(code);
            std::vector<std::vector<std::string>> i = getInstrumentInfo(code);

            std::ofstream info;
            info.open(path + "station.csv", std::ios_base::app);
            // Create Headers and add station information
            info << "Site,IRStationCode,FDSNCode,Latitude,Longitude,Elevation,Depth,CreationDate,TerminationDate\n";
            for (int i = 0; i < s.size(); ++i) {
                info << s[i];
                if (i != s.size() - 1) info << ",";
                else info << "\n";
            }
            // Create headers and add instrument information
            info << "IRStationCode,InstrumentEraStart,InstrumentEraEnd,Instrument,GalvoFreePeriod (seconds),GalvoDampingConst,Azimuth,Dip\n";
            for (auto &v : i) {
                for (int j = 0; j < v.size(); ++j) {
                info << v[j];
                if (j != v.size() - 1) info << ",";
                else info << "\n";
            }
            }
            info.close();
            std::cout << "\n " << code << " \033[1;32msuccessfully\033[0m downloaded to: \033[1;33m"
                      << path << "station.csv\033[0m" << std::endl; 
        }

        void checkStation(std::string code) {
            std::cout << std::endl;
            std::cout << "\033[0;90m >  Search Station: \033[0m"; 
            std::cout << code.c_str() << '\n' << std::endl;

            auto t1 = high_resolution_clock::now();
            std::vector<std::string> sInfo = getStationInfo(code);
            std::vector<std::vector<std::string>> iInfo = getInstrumentInfo(code);
            auto t2 = high_resolution_clock::now();
            
            if (sInfo.size() != 8) return;

            std::cout << " Site:               ";
            handleNA(sInfo[0]);
            std::cout << " FDSNCode:           ";
            handleNA(sInfo[2]);
            std::cout << " Latitude:           ";
            handleNA(sInfo[3]);
            std::cout << " Longitude:          ";
            handleNA(sInfo[4]);
            std::cout << " Elevation:          ";
            handleNA(sInfo[5]);
            std::cout << " Depth:              ";
            handleNA(sInfo[6]);
            std::cout << " CreationDate:       ";
            handleNA(sInfo[7]);
            std::cout << " TerminationDate:    ";
            handleNA(sInfo[8]);
            std::cout << " Instrument History:" << std::endl;
            int c = 0;
            for (auto &v : iInfo) {
                if (c != 0 && c % 10 == 0) {
                    char cmd;
                    std::cout << " (\033[1;33m" << c << "\033[0m/\033[1;33m" << iInfo.size() 
                    << "\033[0m) have been printed. Would you like to print more? (Y/N)";
                    std::cin >> cmd;
                    if (cmd == 'N')  {
                        break;
                    }
                }
                std::cout << "  -> InstrumentEraStart: ";
                handleInst(v[1]);
                std::cout << " InstrumentEraEnd: "; 
                handleInst(v[2]);
                std::cout << " Instrument: ";
                handleInst(v[3]);
                std::cout << std::endl;
                ++c;
            }
            
            duration<double, std::milli> ms_double = t2 - t1;
            printEndBanner(ms_double);
        }

        void uploadCSV(std::string path) {

            std::cout << std::endl;
            std::cout << "\033[0;90m >  Upload CSV: \033[0m"; 
            std::cout << path.c_str() << '\n' << std::endl;

            std::ofstream tmp;
            tmp.open("tmp.csv",std::ios_base::app);
            std::ifstream file;
            file.open("../tables/InstrumentTable.csv");

            auto t1 = high_resolution_clock::now();
            std::vector<std::vector<std::string>> info = sortIncomingCSV(path);

            bool title = true;
            int i = 0;
            std::string IRStationCode,instEraStart,instEraEnd,inst, 
                        freePeriod, dampConst, azi, dip;
            while (std::getline(file,IRStationCode,',')) {
                
                // Had to add this so we were not comparing the new information with the title
                if (!title) {
                    // compares the info by station code and inserts alphabetically
                    while (i < info.size() && info[i][0] < IRStationCode) {
                        for (int j = 0; j < info[i].size(); ++j) {
                            tmp << info[i][j];
                            if (j == info[i].size()-1) tmp << "\n";
                            else tmp << ",";
                        }
                        ++i;
                    }
                }

                std::getline(file,instEraStart,',');
                std::getline(file,instEraEnd,',');
                std::getline(file,inst,',');
                std::getline(file,freePeriod,',');
                std::getline(file,dampConst,',');
                std::getline(file,azi,',');
                std::getline(file,dip,'\n');

                tmp << IRStationCode << "," << instEraStart << "," << instEraEnd << "," << inst
                   << "," << freePeriod << "," << dampConst << "," << azi << "," << dip << "\n";
                title = false;
            }

            file.close();
            std::remove("../tables/InstrumentTable.csv");
            tmp.close();
            std::rename("tmp.csv", "../tables/InstrumentTable.csv");
            auto t2 = high_resolution_clock::now();

            std::cout << " CSV: (\033[1;32m" << path.c_str() 
                      << "\033[0m) succesfully uploaded" << std::endl; 

            duration<double, std::milli> ms_double = t2 - t1;
            printEndBanner(ms_double);

        }
};

#endif