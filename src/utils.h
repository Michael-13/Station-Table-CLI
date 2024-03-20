#ifndef UTILS_H
#define UTILS_H
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <chrono>
#include <fcntl.h>
#include <io.h>
#include <stdio.h>

// Namespaces
using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;

// Files needed
std::ifstream stationTable{"../tables/StationTable.csv"};
std::ifstream instrumentTable{"../tables/InstrumentTable.csv"};

// Helper Functions
std::vector<std::string> getStationInfo(const std::string code) {
    std::vector<std::string> info;

    std::string site, IRStationCode, FDSNCode, latitude, longitude, elevation,
                depth, creationDate, terminationDate;
    while(std::getline(stationTable,site,',')) {
        std::getline(stationTable,IRStationCode,',');
        std::getline(stationTable,FDSNCode,',');
        std::getline(stationTable,latitude,',');
        std::getline(stationTable,longitude,',');
        std::getline(stationTable,elevation,',');
        std::getline(stationTable,depth,',');
        std::getline(stationTable,creationDate,',');
        std::getline(stationTable,terminationDate,'\n');
        if (IRStationCode == code) {
            info.emplace_back(site);
            info.emplace_back(IRStationCode);
            info.emplace_back(FDSNCode);
            info.emplace_back(latitude);
            info.emplace_back(longitude);
            info.emplace_back(elevation);
            info.emplace_back(depth);
            info.emplace_back(creationDate);
            info.emplace_back(terminationDate);
            break;
        }
    }
    return info;
}

std::vector<std::vector<std::string>> getInstrumentInfo(const std::string code) {
    std::vector<std::vector<std::string>> info;

    bool found = false;
    std::string IRStationCode,instEraStart,instEraEnd,inst, 
                freePeriod, dampConst, azi, dip;
    while(std::getline(instrumentTable,IRStationCode,',')) {
        if (found && IRStationCode != code) break;

        std::vector<std::string> tmp;
        std::getline(instrumentTable,instEraStart,',');
        std::getline(instrumentTable,instEraEnd,',');
        std::getline(instrumentTable,inst,',');
        std::getline(instrumentTable,freePeriod,',');
        std::getline(instrumentTable,dampConst,',');
        std::getline(instrumentTable,azi,',');
        std::getline(instrumentTable,dip,'\n');
        if (IRStationCode == code) {
            std::vector<std::string> tmp {
                IRStationCode,instEraStart,instEraEnd,
                inst, freePeriod, dampConst, azi, dip
            };
            info.emplace_back(tmp);
            found = true;
        }
    }

    return info;
}

std::vector<std::string> convertArgs(int tot, char* arg[]) {
    std::vector<std::string> tmp;
    for (int i = 1; i < tot; ++i) {
        std::string s(arg[i]);
        tmp.emplace_back(s);
    }
    return tmp;
}

#endif