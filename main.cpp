#include "jabddl.hpp"

int main() {

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
    jabddl::expr::print(f3);

    std::vector<jabddl::variable>  ordine = {jabddl::variable{"x1"},jabddl::variable{"x2"},jabddl::variable{"x3"},jabddl::variable{"x4"}};
    auto f = jabddl::robdd_build(f3,0,ordine);

    return 0;
}