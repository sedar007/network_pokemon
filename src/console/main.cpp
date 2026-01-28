#include "networkPokemon.h"

int main(int argc, char* argv[]) {

    bool traceLog = false;
    std::cout<<"Console"<<std::endl;

    //pokemon::Main main(traceLog);
    pokemon::Test test;
    std::cout<<test.getIp()<<std::endl;

    return 0;
}
