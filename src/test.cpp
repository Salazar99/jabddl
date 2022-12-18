#include "./include/jabddl.hpp"
#include<iostream>

void test_func() {
//    f = x1 x2 + x1 x2' + x1' x2 + x1' x2'    ordine x1 x2
    expr x1 = expr::make_var("x1");
    expr x2 = expr::make_var("x1");
    expr x1_n = expr::make_neg(x1);
    expr x2_n = expr::make_neg(x2);
    expr x1x2 = expr::make_mul(x1,x2);
    expr x1x2_n = expr::make_mul(x1,x2_n);
    expr x1_nx2 = expr::make_mul(x1_n,x2);
    expr x1_nx2_n = expr::make_mul(x1_n,x2_n);
    expr first_add = expr::make_add(x1x2,x1x2_n);
    expr second_add = expr::make_add(x1_nx2,x1_nx2_n);
    expr function = expr::make_add(first_add,second_add);
    std::vector<std::string> order = {"x1", "x2"};

    jabddl::expr_ptr vertex = jabddl::robdd_build(function,0,order);

//    f = abc + b'd + c'd    ordine b c d a
    expr a = expr::make_var("a");
    expr b = expr::make_var("b");
    expr c = expr::make_var("c");
    expr not_b = expr::make_neg("b");
    expr not_c = expr::make_neg("c");
    expr ab = expr::make_mul(a,b);
    expr abc = expr::make_mul(ab,c);
    expr not_bd = expr::make_mul(not_b,d);
    expr not_cd = expr::make_mul(not_c,d);
    expr sub_expr1  = expr::make_add(abc, not_bd);
    function = expr::make_add(sub_expr1,not_cd);
    std::vector<std::string> order1 = {"b", "c", "d", "a"};

    jabddl::expr_ptr vertex = jabddl::robdd_build(function,0,order1);

//    f = ab + cd + ef    ordine a b c d e f
//                        ordine a c e b d f


//    f1 = b + c      ordine a b c
//    f2 = a + b + c


//    f = x1x2 + x1'x2 + x1x2'     ordine x1 x2, lati complementati
//    g = x1 + x2   
//    h = x1'x2'


//    f = ab + bcd'   ordine a b c d, lati complementati
//    g = ab + ac'd
//    f ---> g = ite(f,g,1)


//    f = ab + c   ordine a b c d, lati complementati
//    g = bc' + d
//    h = c + d'
//    ite(f,g,h)


//    f = abd' + ab'd + a'c + a'c'd    ordine a b c d, lati complementati


//    f = abc + a'c'd'    ordine a b c d, lati complementati


//    f = (a + be)(d + c)    ordine a b e d c, lati complementati


//    f = a + b'c  ordine a b c d, lati complementati
//    g = b'c + d
//    h = b + c' + d
//    ite(f,g,h)


//    f = b + cd   ordine a b c d, lati complementati
//    g = b + cd
//    h = a'c + b
//    ite(f,g,h)
}