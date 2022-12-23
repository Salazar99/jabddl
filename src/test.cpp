#include "../include/jabddl.hpp"
#include<iostream>

int main() {
//    f = x1 x2 + x1 x2' + x1' x2 + x1' x2'    ordine x1 x2
    expr_ptr x1 = expr::make_var("x1");
    expr_ptr x2 = expr::make_var("x1");
    expr_ptr x1_n = expr::make_neg(x1);
    expr_ptr x2_n = expr::make_neg(x2);
    expr_ptr x1x2 = expr::make_mul(x1,x2);
    expr_ptr x1x2_n = expr::make_mul(x1,x2_n);
    expr_ptr x1_nx2 = expr::make_mul(x1_n,x2);
    expr_ptr x1_nx2_n = expr::make_mul(x1_n,x2_n);
    expr_ptr first_add = expr::make_add(x1x2,x1x2_n);
    expr_ptr second_add = expr::make_add(x1_nx2,x1_nx2_n);
    expr_ptr function = expr::make_add(first_add,second_add);
    std::vector<std::string> order = {"x1", "x2"};

    jabddl::vertex_ptr vertex = jabddl::robdd_build(function,0,order);
    vertex::print(vertex);

//    f = abc + b'd + c'd    ordine b c d a
    expr_ptr a = expr::make_var("a");
    expr_ptr b = expr::make_var("b");
    expr_ptr c = expr::make_var("c");
    expr_ptr d = expr::make_var("d");
    expr_ptr not_b = expr::make_neg(b);
    expr_ptr not_c = expr::make_neg(c);
    expr_ptr ab = expr::make_mul(a,b);
    expr_ptr abc = expr::make_mul(ab,c);
    expr_ptr not_bd = expr::make_mul(not_b,d);
    expr_ptr not_cd = expr::make_mul(not_c,d);
    expr_ptr sub_expr1  = expr::make_add(abc, not_bd);
    function = expr::make_add(sub_expr1,not_cd);
    std::vector<std::string> order1 = {"b", "c", "d", "a"};

    jabddl::vertex_ptr vertex1 = jabddl::robdd_build(function,0,order1);
    vertex::print(vertex1);

//    f = ab + cd + ef    ordine a b c d e f
//                        ordine a c e b d f
    expr_ptr e = expr::make_var("e");
    expr_ptr f = expr::make_var("f");
    expr_ptr cd = expr::make_mul(c,d);
    expr_ptr ef = expr::make_mul(e,f);
    expr_ptr ab_cd = expr::make_add(ab,cd);
    function = expr::make_add(ab_cd,ef);

    std::vector<std::string> order2 = {"a", "b", "c", "d","e","f"};
    std::vector<std::string> order3 = {"a", "c", "e", "b","d","f"};

    jabddl::vertex_ptr vertex2 = jabddl::robdd_build(function,0,order2);
    jabddl::vertex_ptr vertex3 = jabddl::robdd_build(function,0,order3);
    vertex::print(vertex2);
    vertex::print(vertex3);

//    f1 = b + c      ordine a b c
//    f2 = a + b + c
    expr_ptr function_1 = expr::make_add(b,c);
    expr_ptr function_2 = expr::make_add(a,function_1);
    std::vector<std::string> order4 = {"a","b","c"};

    jabddl::vertex_ptr vertex4 = jabddl::robdd_build(function_2,0,order4);
    vertex::print(vertex4);

//    f = x1x2 + x1'x2 + x1x2'     ordine x1 x2, lati complementati
//    g = x1 + x2   
//    h = x1'x2'

    expr_ptr fun = expr::make_add(x1x2,x1_nx2);
    expr_ptr function_3 = expr::make_add(fun, x1x2_n);

    jabddl::vertex_ptr vertex5 = jabddl::robdd_build_comp(function_3,0,order);
    vertex::print(vertex5);


//    f = ab + bcd'   ordine a b c d, lati complementati
//    g = ab + ac'd
//    f ---> g = ite(f,g,1)
    expr_ptr bc  = expr::make_mul(b,c);
    expr_ptr d_neg = expr::make_neg(d);
    expr_ptr bcd_neg = expr::make_mul(bc,d_neg);
    f = expr::make_add(ab,bcd_neg);
    expr_ptr c_neg = expr::make_neg(c);
    expr_ptr ac_neg = expr::make_mul(a,c_neg);
    expr_ptr ac_negd = expr::make_mul(ac_neg,d);
    expr_ptr g = expr::make_add(ab,ac_negd);
    std::vector<std::string> order5 = {"a","b","c","d"};



    jabddl::vertex_ptr f_vert = jabddl::robdd_build_comp(f,0,order5);
    jabddl::vertex_ptr g_vert = jabddl::robdd_build_comp(g,0,order5);
    jabddl::vertex_ptr h = jabddl::apply_ite(f_vert,g_vert,jabddl::v1,0,order5);
    std::cout << "Function f" << std::endl;
    vertex::print(f_vert);
    std::cout << "Function g" << std::endl;
    vertex::print(g_vert);
    std::cout << "Function h" << std::endl;
    vertex::print(h);

//    f = ab + c   ordine a b c d, lati complementati
//    g = bc' + d
//    h = c + d'
//    ite(f,g,h)
    f = expr::make_add(ab,c);
    expr_ptr bc_neg = expr::make_mul(b,c_neg);
    g = expr::make_add(bc_neg,d);
    expr_ptr h_expr = expr::make_add(c,d_neg);
    std::vector<std::string> order6 = {"b","c","d"};
    std::vector<std::string> order7 = {"c","d"};

    f_vert = jabddl::robdd_build_comp(f,0,order4);
    g_vert = jabddl::robdd_build_comp(g,0,order6);
    h = jabddl::robdd_build_comp(h_expr,0,order7);

    vertex_ptr k = jabddl::apply_ite_comp(f_vert,g_vert,h,0,order7);

//    f = abd' + ab'd + a'c + a'c'd    ordine a b c d, lati complementati

   expr_ptr abd_neg = expr::make_mul(ab,d_neg);
   expr_ptr b_neg = expr::make_neg(b);
   expr_ptr ab_neg = expr::make_mul(a,b_neg);
   expr_ptr ab_negd = expr::make_mul(ab_neg,d);
   expr_ptr a_neg = expr::make_neg(a);
   expr_ptr a_negc = expr::make_mul(a_neg,c); 
   expr_ptr a_negc_neg = expr::make_mul(a_neg,c_neg);
   expr_ptr a_negc_negd = expr::make_mul(a_negc_neg,d);
   first_add = expr::make_add(abd_neg,ab_negd);
   second_add = expr::make_add(a_negc,a_negc_negd);
   f = expr::make_add(first_add,second_add);

   f_vert = jabddl::robdd_build_comp(f,0,order5); 


//    f = abc + a'c'd'    ordine a b c d, lati complementati
   expr_ptr a_negc_negd_neg = expr::make_mul(a_negc_neg,d_neg);
   f = expr::make_add(abc,a_negc_negd_neg); 
   f_vert = jabddl::robdd_build_comp(f,0,order5); 


//    f = (a + be)(d + c)    ordine a b e d c, lati complementati
   expr_ptr be = expr::make_mul(b,e);
   first_add = expr::make_add(a,be);
   second_add = expr::make_add(d,c);
   f = expr::make_mul(first_add,second_add); 
   std::vector<std::string> order8 = {"a","b","e","d","c"};

   f_vert = jabddl::robdd_build_comp(f,0,order8); 


//    f = a + b'c  ordine a b c d, lati complementati
//    g = b'c + d
//    h = b + c' + d
//    ite(f,g,h)

   expr_ptr b_negc = expr::make_mul(b_neg,c);
   f = expr::make_add(a,b_negc); 
   g = expr::make_add(b_negc,d);
   expr_ptr b_c_neg = expr::make_add(b,c_neg);
   h_expr = expr::make_add(b_c_neg,d);

   f_vert = jabddl::robdd_build_comp(f,0,order5);
   g_vert = jabddl::robdd_build_comp(g,0,order5);
   vertex_ptr h_vert = jabddl::robdd_build_comp(h_expr,0,order5);

   k = jabddl::apply_ite_comp(f_vert,g_vert,h_vert,0,order5);  

//    f = b + cd   ordine a b c d, lati complementati
//    g = b + cd
//    h = a'c + b
//    ite(f,g,h)
   f = expr::make_add(b,cd);
   g = expr::make_add(b,cd);
   h_expr = expr::make_add(a_negc,b);   

   f_vert = jabddl::robdd_build_comp(f,0,order5);
   g_vert = jabddl::robdd_build_comp(g,0,order5);
   h_vert = jabddl::robdd_build_comp(h_expr,0,order5);

   k = jabddl::apply_ite_comp(f_vert,g_vert,h_vert,0,order5);  


return 0;
}