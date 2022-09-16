#include "jabbdl.hpp"

int main() {

    auto x1 = jabbdl::expr::make_var({"x1"});
    auto x2 = jabbdl::expr::make_var({"x2"});
    auto x3 = jabbdl::expr::make_var({"x3"});
    auto x4 = jabbdl::expr::make_var({"x4"});
    auto v0 = jabbdl::expr::make_var({"v0"});
    auto v1 = jabbdl::expr::make_var({"v1"});

    auto f1 = jabbdl::ite(x1, x2, v0);
    auto f2 = jabbdl::ite(x3, x4, v1);

    auto arg1 = jabbdl::expr::make_add(f1, x1);
    auto arg2 = jabbdl::expr::make_add(f2, x2);
    auto f3 = jabbdl::ite(f2, arg1, arg2);

    jabbdl::expr::print(f3);
    return 0;
}