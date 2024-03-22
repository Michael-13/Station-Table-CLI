#include "utils.h"
#include "AbstractClient.h"

int main(int argc, char *argv[]) {
    std::vector<std::string> sv = convertArgs(argc,argv);
    AbstractClient client;
    // note convertArgs does not include ./stationdb as an argument, therefore totalargs=argc-1
    client.start(argc-1,sv);
    return 0;
}