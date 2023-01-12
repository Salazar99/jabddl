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
#include<iostream>


int main() {
    complemented_mode = 0;

//    f = x1 x2 + x1 x2' + x1' x2 + x1' x2'    ordine x1 x2
    std::cout << "\033[33m" << "Test #1" <<"\033[0m " <<std::endl;

    expr_ptr x1 = expr::make_var("x1");
    expr_ptr x2 = expr::make_var("x2");
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
    jabddl::print_truth_table(vertex, order);

//    f = abc + b'd + c'd    ordine b c d a
    std::cout << "\033[33m" << "Test #2" <<"\033[0m " <<std::endl;
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
    jabddl::print_truth_table(vertex1, order1);

//    f = ab + cd + ef    ordine a b c d e f
//                        ordine a c e b d f
    std::cout << "\033[33m" << "Test #3" <<"\033[0m " <<std::endl;
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
    
    jabddl::print_truth_table(vertex2, order2);
    
    vertex::print(vertex3);

    jabddl::print_truth_table(vertex3, order3);


//    f1 = b + c      ordine a b c
//    f2 = a + b + c
    std::cout << "\033[33m" << "Test #4" <<"\033[0m " <<std::endl;
    expr_ptr function_1 = expr::make_add(b,c);
    expr_ptr function_2 = expr::make_add(a,function_1);
    std::vector<std::string> order4 = {"a","b","c"};

    jabddl::vertex_ptr vertex4 = jabddl::robdd_build(function_2,0,order4);
    vertex::print(vertex4);
    
    jabddl::print_truth_table(vertex4, order4);

complemented_mode = 1;

//    f = x1x2 + x1'x2 + x1x2'     ordine x1 x2, lati complementati
//    g = x1 + x2   
//    h = x1'x2'
    std::cout << "\033[33m" << "Test #5" <<"\033[0m " <<std::endl;
    expr_ptr f_sub1 = expr::make_add(x1x2,x1_nx2);
    expr_ptr function_f = expr::make_add(f_sub1, x1x2_n);

    expr_ptr function_g = expr::make_add(x1,x2);
    expr_ptr function_h = expr::make_mul(x1_n,x2_n);

    jabddl::vertex_ptr vertex_f_comp = jabddl::robdd_build_comp(function_f,0,order);
    jabddl::vertex_ptr vertex_f = jabddl::robdd_build(function_f,0,order);
    
    jabddl::vertex_ptr vertex_g_comp = jabddl::robdd_build_comp(function_g,0,order);
    jabddl::vertex_ptr vertex_g = jabddl::robdd_build(function_g,0,order);

    jabddl::vertex_ptr vertex_h = jabddl::robdd_build_comp(function_h,0,order);
    
    std::cout << "Function f not complemented" <<"\033[0m " <<std::endl;
    vertex::print(vertex_f);
    
    std::cout << "Function f complemented" <<"\033[0m " <<std::endl;
    vertex::print(vertex_f_comp);

    jabddl::print_truth_table(vertex_f_comp, order);

    std::cout << "Function g not complemented" <<"\033[0m " <<std::endl;
    vertex::print(vertex_g);
    
     std::cout << "Function g complemented" <<"\033[0m " <<std::endl;
    vertex::print(vertex_g_comp);

    jabddl::print_truth_table(vertex_g_comp, order);
    
    std::cout << "Function h complemented" <<"\033[0m " <<std::endl;
    vertex::print(vertex_h);

//    f = ab + bcd'   ordine a b c d, lati complementati
//    g = ab + ac'd
//    f ---> g = ite(f,g,1)
    std::cout << "\033[33m" << "Test #6" <<"\033[0m " <<std::endl;
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
    jabddl::vertex_ptr h = jabddl::apply_ite_comp(f_vert,g_vert,jabddl::v1,0,order5);
    
    jabddl::vertex_ptr f_vert_nc = jabddl::robdd_build(f,0,order5);
    jabddl::vertex_ptr g_vert_nc = jabddl::robdd_build(g,0,order5);
    jabddl::vertex_ptr h_nc = jabddl::apply_ite(f_vert_nc,g_vert_nc,jabddl::v1,0,order5);
    
    
    std::cout << "Function f" << std::endl;
    vertex::print(f_vert);
    jabddl::print_truth_table(f_vert,order5);
    std::cout << "Function g" << std::endl;
    vertex::print(g_vert);
    jabddl::print_truth_table(g_vert,order5);
    std::cout << "Function h" << std::endl;
    vertex::print(h);
    jabddl::print_truth_table(h,order5);

    std::cout << "Function f non comp" << std::endl;
    vertex::print(f_vert_nc);
    std::cout << "Function g non comp" << std::endl;
    vertex::print(g_vert_nc);
    std::cout << "Function h non comp" << std::endl;
    vertex::print(h_nc);


//    f = ab + c   ordine a b c d, lati complementati
//    g = bc' + d
//    h = c + d'
//    ite(f,g,h)
    std::cout << "\033[33m" << "Test #7" <<"\033[0m " <<std::endl;
    f = expr::make_add(ab,c);
    expr_ptr bc_neg = expr::make_mul(b,c_neg);
    g = expr::make_add(bc_neg,d);
    expr_ptr h_expr = expr::make_add(c,d_neg);
    
    std::vector<std::string> order6 = {"b","c","d"};
    std::vector<std::string> order7 = {"c","d"};

    f_vert = jabddl::robdd_build_comp(f,0,order4);
    g_vert = jabddl::robdd_build_comp(g,0,order6);
    h = jabddl::robdd_build_comp(h_expr,0,order7);

    std::cout << "Function f comp" << std::endl;
    vertex::print(f_vert);
    std::cout << "Function g comp" << std::endl;
    vertex::print(g_vert);
    std::cout << "Function h comp" << std::endl;
    vertex::print(h);

    f_vert_nc = jabddl::robdd_build(f,0,order4);
    g_vert_nc = jabddl::robdd_build(g,0,order6);
    h_nc = jabddl::robdd_build(h_expr,0,order7);

    std::cout << "Function f " << std::endl;
    vertex::print(f_vert_nc);
    std::cout << "Function g " << std::endl;
    vertex::print(g_vert_nc);
    std::cout << "Function h " << std::endl;
    vertex::print(h_nc);

    std::cout << "Function ite(f,g,h) comp" << std::endl;
    vertex_ptr k = jabddl::apply_ite_comp(f_vert,g_vert,h,0,order5);
    vertex::print(k);

    std::cout << "Function ite(f,g,h) " << std::endl;
    vertex_ptr k_nc = jabddl::apply_ite(f_vert_nc,g_vert_nc,h_nc,0,order5);
    vertex::print(k_nc);

//    f = abd' + ab'd + a'c + a'c'd    ordine a b c d, lati complementati
   std::cout << "\033[33m" << "Test #8" <<"\033[0m " <<std::endl;
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
   f_vert_nc = jabddl::robdd_build(f,0,order5); 

   std::cout << "Function f compl " << std::endl;
   vertex::print(f_vert);

   std::cout << "Function f  " << std::endl;
   vertex::print(f_vert_nc);
   

//    f = abc + a'c'd'    ordine a b c d, lati complementati
   std::cout << "\033[33m" << "Test #9" <<"\033[0m " <<std::endl;
   expr_ptr a_negc_negd_neg = expr::make_mul(a_negc_neg,d_neg);
   f = expr::make_add(abc,a_negc_negd_neg); 
   f_vert = jabddl::robdd_build_comp(f,0,order5); 
   f_vert_nc = jabddl::robdd_build(f,0,order5); 

   std::cout << "Function f compl " << std::endl;
   vertex::print(f_vert);

   std::cout << "Function f " << std::endl;
   vertex::print(f_vert_nc);


//    f = (a + be)(d + c)    ordine a b e d c, lati complementati
   std::cout << "\033[33m" << "Test #10" <<"\033[0m " <<std::endl;
   expr_ptr be = expr::make_mul(b,e);
   first_add = expr::make_add(a,be);
   second_add = expr::make_add(d,c);
   f = expr::make_mul(first_add,second_add); 
   std::vector<std::string> order8 = {"a","b","e","d","c"};

   f_vert = jabddl::robdd_build_comp(f,0,order8); 
   f_vert_nc = jabddl::robdd_build(f,0,order8); 

   std::cout << "Function f compl " << std::endl;
   vertex::print(f_vert);   
   
   std::cout << "Function f " << std::endl;
   vertex::print(f_vert_nc);   

//    f = a + b'c  ordine a b c d, lati complementati
//    g = b'c + d
//    h = b + c' + d
//    ite(f,g,h)
   std::cout << "\033[33m" << "Test #11" <<"\033[0m " <<std::endl;
   expr_ptr b_negc = expr::make_mul(b_neg,c);
   f = expr::make_add(a,b_negc); 
   g = expr::make_add(b_negc,d);
   expr_ptr b_c_neg = expr::make_add(b,c_neg);
   h_expr = expr::make_add(b_c_neg,d);

   f_vert = jabddl::robdd_build_comp(f,0,order5);
   g_vert = jabddl::robdd_build_comp(g,0,order5);
   vertex_ptr h_vert = jabddl::robdd_build_comp(h_expr,0,order5);

   std::cout << "Function k compl " << std::endl;
   k = jabddl::apply_ite_comp(f_vert,g_vert,h_vert,0,order5);
   vertex::print(k);

   f_vert_nc = jabddl::robdd_build(f,0,order5);
   g_vert_nc = jabddl::robdd_build(g,0,order5);
   vertex_ptr h_vert_nc = jabddl::robdd_build(h_expr,0,order5);

   std::cout << "Function k " << std::endl;
   k_nc = jabddl::apply_ite(f_vert_nc,g_vert_nc,h_vert_nc,0,order5);  
   vertex::print(k_nc);    

//    f = b + cd   ordine a b c d, lati complementati
//    g = b + cd
//    h = a'c + b
//    ite(f,g,h)
   std::cout << "\033[33m" << "Test #12" <<"\033[0m " <<std::endl;
   f = expr::make_add(b,cd);
   g = expr::make_add(b,cd);
   h_expr = expr::make_add(a_negc,b);   

   f_vert = jabddl::robdd_build_comp(f,0,order5);
   g_vert = jabddl::robdd_build_comp(g,0,order5);
   h_vert = jabddl::robdd_build_comp(h_expr,0,order5);

   k = jabddl::apply_ite_comp(f_vert,g_vert,h_vert,0,order5);  

   std::cout << "Function k comp " << std::endl;
   vertex::print(k);   
    
   f_vert_nc = jabddl::robdd_build(f,0,order5);
   g_vert_nc = jabddl::robdd_build(g,0,order5);
   h_vert_nc = jabddl::robdd_build(h_expr,0,order5);

   std::cout << "Function k " << std::endl;
   k_nc = jabddl::apply_ite(f_vert_nc,g_vert_nc,h_vert_nc,0,order5);  
   vertex::print(k_nc); 

   jabddl::print_truth_table(k, order5);

return 0;
}