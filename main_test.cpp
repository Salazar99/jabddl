#include "jabddl.hpp"
#include <iostream>

void    bash_help(); 


int main(int argc, char **argv) {
    std::string inputFile;
    std::vector<std::string> order;
    std::vector<jabddl::fun> expr;

    switch(argc){
    case 1:
    case 2:
        if((std::string)argv[1] != "-h"){ 
            std::cout << "Too few arguments!" <<std::endl;
            std::cout << "type -h for help!" <<std::endl;
            exit(-1);  
        }
        else bash_help();

    case 3:
     //Maybe we want to do some checks on the parameters in input in the future
        if((std::string)argv[1] != "-f")
        {
           std::cout << "expected \"-f\" before input file" <<std::endl;
           std::cout << "type -h for help!" <<std::endl;
           exit(-1);
        }

        inputFile = (std::string)argv[2];  
        break;
    default: 
        std::cout << "Something went terribly wrong" <<std::endl;
        exit(-1);
    }

    jabddl::initialize();
    jabddl::parse_input(inputFile, order, expr);

    auto x1 = jabddl::expr::make_var({"x1"});
    auto x2 = jabddl::expr::make_var({"x2"});
    auto x3 = jabddl::expr::make_var({"x3"});
    auto x4 = jabddl::expr::make_var({"x4"});
    auto v0 = jabddl::expr::make_var({"v0"});
    auto v1 = jabddl::expr::make_var({"v1"});
    

    auto expr1 = jabddl::ite(x1, x2, v0);
    auto expr2 = jabddl::ite(x2, x1, v0);
    

    std::cout << "Funzione 1";
    jabddl::expr::print(expr1);

    std::cout << "Funzione 2";
    jabddl::expr::print(expr2);

    std::vector<std::string>  ordine = {{"x1"},{"x2"}};
    std::vector<std::string>  ordine_rev = {{"x2"},{"x1"}};
    
    std::vector<std::string>  ordine2 = {{"x4"},{"x3"},{"x2"},{"x1"}};
    
    auto f1 = jabddl::robdd_build(expr1,0,ordine);
    auto f2 = jabddl::robdd_build(expr2,0,ordine);

    auto f3 = jabddl::robdd_build(expr1,0,ordine_rev);
    auto f4 = jabddl::robdd_build(expr2,0,ordine_rev);

    jabddl::print_truth_table(f1, ordine);


    std::cout << "UNIQUE TABLE" <<std::endl;
    jabddl::print_table(jabddl::unique_table);

    std::cout <<std::endl << "robdd for function 1 order: ";
    for(std::string var : ordine)    
        std::cout <<var; 
    std::cout <<std::endl;
    
    jabddl::vertex::print(f1);

      std::cout <<std::endl << "robdd for function 1 order: ";
    for(std::string var : ordine_rev)    
        std::cout <<var; 
    std::cout <<std::endl;
    
    jabddl::vertex::print(f3);

    std::cout <<std::endl << "robdd for function 2 order: ";
    for(std::string var : ordine)    
        std::cout <<var; 
    std::cout <<std::endl;

    jabddl::vertex::print(f2);

     std::cout <<std::endl << "robdd for function 2 order: ";
    for(std::string var : ordine_rev)    
        std::cout <<var; 
    std::cout <<std::endl;

    jabddl::vertex::print(f4);



/*

    auto h = jabddl::robdd_build(arg2,0,ordine);
    auto g = jabddl::robdd_build(arg1,0,ordine);
    auto f= jabddl::robdd_build(f2,0,ordine);
    //jabddl::expr::print(f1);
    //jabddl::vertex::print(h);
    auto x = jabddl::apply_ite(f,g,h,0,ordine);
    jabddl::vertex::print(x);
    */
    /*
    jabddl::vertex_ptr vrt1 = std::make_shared<jabddl::vertex>("x1");
    jabddl::vertex_ptr vrt2 = std::make_shared<jabddl::vertex>("x1");
    
    vrt1->lsubtree = jabddl::v1;
    vrt1->rsubtree = jabddl::v0;
    vrt2->lsubtree = jabddl::v1;
    vrt2->rsubtree = jabddl::v0;

    if(jabddl::vertex_compare(vrt1,vrt2))
        printf("they are equal");
    else
        printf("They are not equal");
    */

    return 0;
}

void bash_help(){
    std::cout << "Usage:" << std::endl <<std::endl
    << "\"-f\": specify input file" <<std::endl
    << "Example of usage: $ ./jabdd -f input.txt" <<std::endl;
    exit(0);
}