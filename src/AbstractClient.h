#include "ShellUtils.h"
#include "stationDBClient.h"
#include <string>
#include <vector>

class stationDBClient;

class AbstractClient : public ShellUtils {
    private:
        stationDBClient sdb;

        const std::vector<std::vector<std::string>> basicCommands = {
            {"-h","--help","printHelp"}
        };
        const std::vector<std::vector<std::string>> mainCommands = {
            {"-C", "--checkSta", "checkStation"},
            {"-S", "--seismo", "checkSeismo"},
            {"-A", "--add", "addInstrument"}

        };

        void callFunction(std::vector<std::string> args, std::string func) {
            if (func == "checkStation") {
                sdb.checkStation(args[1]);
            }
        }
    public:
        AbstractClient() {}

        bool parseArguments(int tot, std::vector<std::string> args) {
            for (auto &v : basicCommands) {
                if (args[0] == v[0] || args[1] == v[1]) {
                    sdb.printHelp();
                    return true;
                }
            }
            if (tot < 3) return false;
            for (auto &v : mainCommands) {
                if (args[0] == v[0] || args[1] == v[1]) {
                    callFunction(args,v[2]);
                    return true;
                }
            }
            return false;
        }

        void start(int tot, std::vector<std::string> args) {
            if (tot == 1) {
                std::cout << std::endl;
                std::cout << "Usage: stationdb [-A file] [-C code] [-h] [-S seismo]\n";
                std::cout << std::endl;
                sdb.printHelp();
                return;
            }
            if (!parseArguments(tot, args)) {
                std::cout << "bad" << std::endl;
            }
        }

};
