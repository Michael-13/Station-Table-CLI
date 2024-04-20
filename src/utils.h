#ifndef UTILS_H
#define UTILS_H
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <fstream>
#include <chrono>
#include <thread>
#include <future>
#include <fcntl.h>
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>

// Namespaces
using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;
using namespace cv;

// Files needed
const std::string stationPath = "../tables/StationTable.csv";
const std::string instrumentPath = "../tables/InstrumentTable.csv";
const std::string seismoPath = "../tables/SeismoTable.csv";

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

inline std::vector<std::string> getSeismogramInfo(const std::string &id) {
    std::vector<std::string> info;

    std::ifstream seismoTable;
    seismoTable.open(seismoPath);

    std::string DOI, IRStationCode, Instrument,
                StartTime, EndTime, VectorizedTrace;
    while(std::getline(seismoTable,DOI,',')) {

        std::getline(seismoTable,IRStationCode,',');
        std::getline(seismoTable,Instrument,',');
        std::getline(seismoTable,StartTime,',');
        std::getline(seismoTable,EndTime,',');
        std::getline(seismoTable,VectorizedTrace,'\n');
        if (DOI == id) {
            info = {DOI,IRStationCode,Instrument,StartTime,EndTime,VectorizedTrace};
            break;
        }
    }

    seismoTable.close();
    return info;
}

inline std::vector<std::string> findInstrument(const std::vector<std::vector<std::string>> &instInfo, 
                                               const std::pair<std::string,std::string> &dep) {
    // remember dep contains {Instrument, StartTime}
    // remember instInfo {{IRStationCode,InstEraStart,InstEraEnd,inst,
    //                    freePeroid,dampConst,azi,dip},..}
    std::vector<std::string> res;

    std::unordered_map<std::string, std::vector<std::vector<std::string>>> iMap;
    for (auto &v : instInfo) {
        iMap[v.at(3)].emplace_back(v);
    }
    
    int depYear = std::stoi(dep.second.substr(0,4));
    for (auto &v : iMap[dep.first]) {
        int sYear = std::stoi(v.at(1).substr(0,4)), eYear = std::stoi(v.at(2).substr(0,4));

        if (depYear >= sYear && depYear <= eYear) {
            res = v;
        } 
    }

    return res;
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

inline void copyImage(const std::string &inPath, const std::string &outPath) {
    Mat im = imread(inPath);
    imwrite(outPath, im);
}

template<typename T>
inline bool futureIsReady(std::future<T> &t) {
    return t.wait_for(std::chrono::seconds(0)) == std::future_status::ready;
}

inline void handleDownload(const std::string &inPath, const std::string &outPath) {
    std::future<void> foo = std::async(std::launch::async, copyImage, inPath, outPath);
    
    int i = 0;
    while(true) {
        if (futureIsReady(foo)) break;
        std::cout << "\r Downloading ";
        int mod = i % 3;
        if (mod == 0) {
            std::cout << "\033[1;32m.  \033[0m" << std::flush;
        } else if (mod == 1) {
            std::cout << "\033[1;33m.. \033[0m" << std::flush;
        } else {
            std::cout << "\033[1;31m...\033[0m" << std::flush;
        }
        ++i;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    std::cout << "\r                \r";
}

#endif
