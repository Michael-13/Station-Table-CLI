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
                if (args.size() > 2) {
                    if (args[2] == "-d") {
                        std::string path = (args.size() == 4) ? args[3] : "";
                        sdb.downloadStation(args[1], path);
                    } else {
                        std::cout << "stationdb: invalid option -- " << "'" << args[2] << "'";
                        sdb.printHelp();
                        return;
                    }
                }
                sdb.checkStation(args[1]);
            } else if (func == "addInstrument") {
                sdb.uploadCSV(args[1]);
            }
        }
    public:
        AbstractClient() {}

        int parseArguments(int tot, std::vector<std::string> args) {
            for (auto &v : basicCommands) {
                if (args[0] == v[0] || args[1] == v[1]) {
                    sdb.printHelp();
                    return 0;
                }
            }
            if (tot < 2) return -1;
            for (auto &v : mainCommands) {
                if (args[0] == v[0] || args[1] == v[1]) {
                    callFunction(args,v[2]);
                    return 0;
                }
            }
            return 1;
        }

        void start(int tot, std::vector<std::string> args) {
            if (tot == 0) {
                sdb.printHelp();
                return;
            }
            int exitCode = parseArguments(tot, args);
            if (exitCode == 1) {
                std::cout << "stationdb: invalid option -- " << "'" << args[0] << "'";
                sdb.printHelp();
            } else if (exitCode == -1) {
                std::cout << "stationdb: option requires an argument -- " << "'" << args[0] << "'";
                sdb.printHelp();
            }
        }

};
