#include "utils.h"
#include "AbstractClient.h"

int main(int argc, char *argv[]) {
    std::vector<std::string> sv = convertArgs(argc,argv);
    AbstractClient client;
    client.start(argc,sv);
    return 0;
}