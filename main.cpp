#include <iostream>
#include <fstream>
#include <sstream>
#include "ExpressionManager.h"

using namespace std;

#ifdef _MSC_VER // Memory leak check
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#define VS_MEM_CHECK _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#else
#define VS_MEM_CHECK
#endif

int main(int argc, char* argv[]) {
    VS_MEM_CHECK               // Enable memory leak check
    if(argc < 3){
        cerr << "Please provide name of input and output files.";
        return 1;
    }
    cout << "Input file: " << argv[1] << endl;
    ifstream in(argv[1]);   //reading from input file
    if(!in.is_open()){
        cerr << "Unable to open " << argv[1] << " for input.";
        return 2;
    }
    cout << "Output file: " << argv[2] << endl;
    ofstream out(argv[2]);   //writing to output file
    if(!out){
        in.close();
        cerr << "Unable to open " << argv[2] << " for output.";
        return 3;
    }

    string line;
    string command;
    string expression;
    ExpressionManager em;
    while(getline(in, line)){
        try{
            if(line.size() == 0){
                continue;
            }
            istringstream iss(line);
            iss >> command;
            if(command == "Expression:") {
                out << endl;
            }
            out << command << " ";
            if(command == "Expression:") {
                expression = line.substr(line.find(": ") + 2, line.size() - 1);
                em = ExpressionManager(expression);
                out << em.toString() << endl;
                continue;
            }
            else if(command == "Infix:"){
                out << em.infix() << endl;
                continue;
            }
            else if(command == "Postfix:"){
                out << em.postfix() << endl;
                continue;
            }
            else if(command == "Prefix:"){
                out << em.prefix() << endl;
                continue;
            }
            else if(command == "Value:"){
                out << em.value() << endl;
                continue;
            }
        }
        catch(exception& e){
            out << e.what() << endl;
        }
    }
    return 0;
}
