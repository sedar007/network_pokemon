#include "iostream"
#include "hpp/main.hpp"
#include "hpp/node.hpp"
#include "sstream"


using namespace pokemon;
int main(int argc, char* argv[]) {

    if(argc < 5){
        std::cerr << "Argument error, requires: <comm> -d <filePath> -l <nodeFile>" << std::endl;
        return -1;
    }

    std::string picturePath;
    std::string nodeFile;
    bool traceLog = false;

    // Search for arguments
    for(int i = 1; i< argc - 1; i = i+2){
        std::string value = *(argv + (i+1)) ;
        std::string cmd = *(argv + i) ;
        if(cmd == "-d")
            picturePath = value ;
        if(cmd == "-l")
            nodeFile =  value;
        if(cmd == "-t")
            traceLog = value == "1";
    }


   Main main(traceLog);

   auto node = std::make_unique<Node>(picturePath, nodeFile); // Create a Node

   main.addNode(node);

   auto mainThread = main.run();
   mainThread.join();

   return 0;
}