/*
JABDDL

Copyright© 2023 Daniele Nicoletti 

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), 
to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, 
and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

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
        //default verbosity is 0 
        verbosity = 0;
        //default complementation is 0 
        complemented_mode = 0;
        inputFile = (std::string)argv[2];  
        break;
    case 4:
        //specifying -v means verbosity lvl 1, future updates could implemente different levels
        if((std::string)argv[1] == "-f"){
            if((std::string)argv[3] == "-v"){
                complemented_mode = 0;
                verbosity = 1;
                inputFile = (std::string)argv[2]; 
                break;
            //specifying -c allows usage of complemented edges
            }else if((std::string)argv[3] == "-c"){
                complemented_mode = 1;
                verbosity = 0;
                inputFile = (std::string)argv[2]; 
                break;
            }
            else{
                std::cout << "unexpected argument after file input!" <<std::endl;
                std::cout << "type -h for help!" <<std::endl;
                exit(-1);

            }
        }else{
           std::cout << "expected \"-f\" before input file" <<std::endl;
           std::cout << "type -h for help!" <<std::endl;
           exit(-1);
        }

    case 5:
        //specifying -v means verbosity lvl 1, -c allows usage of complemented edges
        if((std::string)argv[1] == "-f"){
            if(((std::string)argv[3] == "-v" && (std::string)argv[4] == "-c") || ((std::string)argv[4] == "-v" && (std::string)argv[3] == "-c")){
                verbosity = 1;
                complemented_mode = 1;
                inputFile = (std::string)argv[2];
                break;
            }else{
                std::cout << "unexpected argument after file input!" <<std::endl;
                std::cout << "type -h for help!" <<std::endl;
                exit(-1);
            }
        }else{
           std::cout << "expected \"-f\" before input file" <<std::endl;
           std::cout << "type -h for help!" <<std::endl;
           exit(-1);
        }

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
        if(!complemented_mode){
            cntx.root_vertexes.insert(std::make_pair(function.func_name, jabddl::robdd_build(ite(function.ite_if,function.ite_then,function.ite_else),0,cntx.vars)));
        }else{
            //Create the complemented_vertex structure to insert in context
            complemented_vertex fun;
            fun.complemented = false;
            fun.root = jabddl::robdd_build_comp(ite(function.ite_if,function.ite_then,function.ite_else),0,cntx.vars);
            //Insert complemented_vertex in cntx map
            cntx.root_vertexes_comp.insert(std::make_pair(function.func_name, jabddl::propagate_complemented(fun)));
        }
    }

    if(complemented_mode){
        for(auto &f : cntx.root_vertexes_comp){
            if(cntx.funcs[f.first].tbp){
                std::cout << "Function: " << f.first << std::endl;
                std::cout << (f.second.complemented?"!":"") << std::endl;
                vertex::print(f.second.root);
                std::cout << std::endl << std::endl;

                std::cout << "Truth table for function: " << f.first << std::endl;
                jabddl::print_truth_table(f.second.root, cntx.vars,f.second.complemented);
                std::cout << std::endl << std::endl;
            }
        }
    }else{
        for(auto &f : cntx.root_vertexes){
            if(cntx.funcs[f.first].tbp){
                std::cout << "Function: " << f.first << std::endl;
                vertex::print(f.second);
                std::cout << std::endl << std::endl;

                std::cout << "Truth table for function: " << f.first << std::endl;
                jabddl::print_truth_table(f.second, cntx.vars,false);
                std::cout << std::endl << std::endl;
            }
        }
    }
    return 0;
}

void bash_help(){
    std::cout << "Usage:" << std::endl
    << "\"-f\": specify input file" <<std::endl
    << "\"-v\": program will print additional informations " <<std::endl <<std::endl
    << "\"-c\": program will use complemented edges for constructing bdds " << std::endl << std::endl
    << "Use example: $ ./jabdd -f input.txt [-v] [-c]" <<std::endl <<std::endl
    << "args in \"[]\" are optional" <<std::endl <<std::endl;
    exit(0);
}