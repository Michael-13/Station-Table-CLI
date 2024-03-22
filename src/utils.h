#ifndef UTILS_H
#define UTILS_H
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <chrono>
#include <fcntl.h>
//#include <io.h>
#include <stdio.h>

// Namespaces
using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;

// Files needed
const std::string stationPath = "../tables/StationTable.csv";
const std::string instrumentPath = "../tables/InstrumentTable.csv";

// Helper Functions
inline std::vector<std::string> getStationInfo(const std::string code) {
    std::vector<std::string> info;

    std::ifstream stationTable;
    stationTable.open(stationPath);

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

    stationTable.close();
    return info;
}

inline std::vector<std::vector<std::string>> getInstrumentInfo(const std::string code) {
    std::vector<std::vector<std::string>> info;

    std::ifstream instrumentTable;
    instrumentTable.open(instrumentPath);

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

    instrumentTable.close();
    return info;
}

inline std::vector<std::string> convertArgs(int tot, char* arg[]) {
    std::vector<std::string> tmp;
    for (int i = 1; i < tot; ++i) {
        std::string s(arg[i]);
        tmp.emplace_back(s);
    }
    return tmp;
}

inline int cols(std::string path) {
    std::ifstream file;
    file.open(path);
    std::string row;
    std::getline(file,row,'\n');
    file.close();

    int cnt = 0;
    for (auto &c : row) if (c == ',') ++cnt;

    return cnt + 1;
}

inline std::vector<std::vector<std::string>> csvToVec(std::string path) {
    std::vector<std::vector<std::string>> info;
    std::ifstream file;
    file.open(path);

    int c = cols(path);
    std::string str;
    while (std::getline(file,str,',')) {
        std::vector<std::string> tmp;
        tmp.emplace_back(str);

        for (int i = 1; i < c; ++i) {
            if (i != c-1) std::getline(file,str,',');
            else std::getline(file,str,'\n');
            tmp.emplace_back(str);
        }

        info.emplace_back(tmp);
    }

    file.close();
    return info;
}

inline std::vector<std::vector<std::string>> sortIncomingCSV(std::string path) {
    std::vector<std::vector<std::string>> file = csvToVec(path);
    // sort by station code alphabetically
    std::sort(file.begin(), file.end(),
            [](const std::vector<std::string> a, const std::vector<std::string> b) {return a[0] < b[0];});

    return file;
}

#endif
