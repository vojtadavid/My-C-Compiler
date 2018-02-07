#include <iostream>
#include <experimental/filesystem>
#include "Parser.h"

int main(int argc, char* argv[]) {
    Parser parser;

    if(argc>=2){
        if(argc==3){
            parser.binaryOutputname=argv[2];
        }
        std::ifstream infile(argv[1]);
        if(infile.good()){
            parser.run(argv[1]);
        } else{
            printf("couldn't open input file\n");
        }
    } else{
        printf("no input file\n");
    }



}