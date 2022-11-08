#include "jabddl.hpp"
#include <iostream>

int main(int argc, char **argv) {
    jabddl::initialize();
    std::vector<jabddl::expr_ptr> expr;

    auto x1 = jabddl::expr::make_var({"x1"});
    auto x2 = jabddl::expr::make_var({"x2"});
    auto x3 = jabddl::expr::make_var({"x3"});
    auto x4 = jabddl::expr::make_var({"x4"});
    auto v0 = jabddl::expr::make_var({"v0"});
    auto v1 = jabddl::expr::make_var({"v1"});
    

    auto f1 = jabddl::ite(x1, x2, v0);
    auto f2 = jabddl::ite(x2, x1, v0);
    

    std::cout << "Funzione 1";
    jabddl::expr::print(f1);

    std::cout << "Funzione 2";
    jabddl::expr::print(f2);

    std::vector<std::string>  ordine = {{"x1"},{"x2"}};
    std::vector<std::string>  ordine2 = {{"x4"},{"x3"},{"x2"},{"x1"}};
    
    auto f4 = jabddl::robdd_build(f1,0,ordine);
    auto f5 = jabddl::robdd_build(f2,0,ordine);

    jabddl::print_truth_table(f4, ordine);


    std::cout << "UNIQUE TABLE" <<std::endl;
    jabddl::print_table(jabddl::unique_table);

    std::cout <<std::endl << "robdd for function order 1:" <<std::endl;
    jabddl::vertex::print(f4);
    std::cout <<std::endl << "robdd for function order 2:" <<std::endl;
    jabddl::vertex::print(f5);



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