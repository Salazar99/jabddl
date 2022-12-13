#include "../include/jabddl.hpp"
#include <iostream>
void bash_help(); 


int main(int argc, char **argv) {
    std::string inputFile;
    
    //Order of evaluation
    std::vector<std::string> order;
    jabddl::context cntx;

    //checks for command line arguments
    switch(argc){
    case 1:
        std::cout << "Too few arguments!" <<std::endl;
            std::cout << "type -h for help!" <<std::endl;
            exit(-1); 
    case 2:
        if((std::string)argv[1] != "-h"){ 
            std::cout << "Too few arguments!" <<std::endl;
            std::cout << "type -h for help!" <<std::endl;
            exit(-1);  
        }
        else bash_help();
        break;
    case 3:
     //Maybe we want to do some checks on the parameters in input in the future
        if((std::string)argv[1] != "-f")
        {
           std::cout << "expected \"-f\" before input file" <<std::endl;
           std::cout << "type -h for help!" <<std::endl;
           exit(-1);
        }
        verbosity = 0;
        inputFile = (std::string)argv[2];  
        break;
    case 4:
        //specifying -v means verbosity lvl 1, future updates could implemente different levels
        if((std::string)argv[1] == "-f"){
            if((std::string)argv[3] == "-v"){
                verbosity = 1;
                inputFile = (std::string)argv[2]; 
                break;
            }
        }
           std::cout << "expected \"-f\" before input file" <<std::endl;
           std::cout << "type -h for help!" <<std::endl;
           exit(-1);
        
    default: 
        std::cout << "Too many arguments! type ./jabddl -h for help" <<std::endl;
        exit(-1);
    }

    jabddl::initialize();
    jabddl::parse_input(inputFile, cntx);

    if(verbosity){
        for (auto const& x : cntx.vars)
        {
            std::cout << "variable: " << x << std::endl;
        }
        std::cout << std::endl;
        for (auto &f : cntx.funcs)
        {
            std::cout << "functions: " << f.first << std::endl;
        }
        std::cout << std::endl;
    }


    for (auto &f : cntx.funcs)
    {
        jabddl::fun function = f.second;
        if(verbosity) std::cout << "Building BDD for function: " << function.func_name << std::endl << std::endl;
        cntx.root_vertexes.insert(std::make_pair(function.func_name, jabddl::robdd_build(ite(function.ite_if,function.ite_then,function.ite_else),0,cntx.vars)));
    }

    for(auto &f : cntx.root_vertexes){
        if(cntx.funcs[f.first].tbp){
            std::cout << "Function: " << f.first << std::endl;
            vertex::print(f.second);
            std::cout << std::endl << std::endl;

            std::cout << "Truth table for function: " << f.first << std::endl;
            jabddl::print_truth_table(f.second, cntx.vars);
            std::cout << std::endl << std::endl;
        }
    
    }

    return 0;
}

void bash_help(){
    std::cout << "Usage:" << std::endl
    << "\"-f\": specify input file" <<std::endl
    << "\"-v\": program will print additional informations " <<std::endl <<std::endl
    << "Use example: $ ./jabdd -f input.txt [-v]" <<std::endl <<std::endl
    << "args in \"[]\" are optional" <<std::endl <<std::endl;
    exit(0);
}