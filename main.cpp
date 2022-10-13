#include "jabddl.hpp"

int main() {
    jabddl::initialize();

    auto x1 = jabddl::expr::make_var({"x1"});
    auto x2 = jabddl::expr::make_var({"x2"});
    auto x3 = jabddl::expr::make_var({"x3"});
    auto x4 = jabddl::expr::make_var({"x4"});
    auto v0 = jabddl::expr::make_var({"v0"});
    auto v1 = jabddl::expr::make_var({"v1"});
    

    auto f1 = jabddl::ite(x1, x2, v0);
    auto f2 = jabddl::ite(x3, x4, v1);

    auto arg1 = jabddl::expr::make_add(f1, x1);
    auto arg2 = jabddl::expr::make_add(f2, x2);
    

    auto f3 = jabddl::ite(f2, arg1, arg2);
    //jabddl::expr::print(f3);

    std::vector<jabddl::variable>  ordine = {jabddl::variable{"x1"},jabddl::variable{"x2"},jabddl::variable{"x3"},jabddl::variable{"x4"}};
    std::vector<jabddl::variable>  ordine2 = {jabddl::variable{"x1"},jabddl::variable{"x2"}};
    
    auto f4 = jabddl::robdd_build_comp(f1,0,ordine2);
    jabddl::print_table(jabddl::unique_table);
    //jabddl::vertex::print(f4);
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