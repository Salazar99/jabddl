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
#define TEST_NUM 12

#include "../include/jabddl.hpp"
#include <iostream>
#include "../include/dot.hpp"

int main() {
    complemented_mode = 0;
    jabddl::initialize();

#if TEST_NUM == 1

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

    complemented_vertex fun; 
    fun.complemented=false;
    fun.root = vertex;

    std::string const filename = "test1.dot";
    jabddl::dump_dot(fun, filename.c_str() );

#endif
#if TEST_NUM == 2

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
    expr_ptr function = expr::make_add(sub_expr1,not_cd);
    std::vector<std::string> order1 = {"b", "c", "d", "a"};

    jabddl::vertex_ptr vertex1 = jabddl::robdd_build(function,0,order1);
    vertex::print(vertex1);
    jabddl::print_truth_table(vertex1, order1);

    complemented_vertex fun; 
    fun.complemented=false;
    fun.root = vertex1;

    std::string const filename = "test2.dot";
    jabddl::dump_dot(fun, filename.c_str() );


#endif
#if TEST_NUM == 3

//    f = ab + cd + ef    ordine a b c d e f
//                        ordine a c e b d f
    std::cout << "\033[33m" << "Test #3" <<"\033[0m " <<std::endl;
    expr_ptr c = expr::make_var("c");
    expr_ptr d = expr::make_var("d");
    expr_ptr a = expr::make_var("a");
    expr_ptr b = expr::make_var("b");
    expr_ptr e = expr::make_var("e");
    expr_ptr f = expr::make_var("f");
    
    expr_ptr ab = expr::make_mul(a,b);
    expr_ptr cd = expr::make_mul(c,d);
    expr_ptr ef = expr::make_mul(e,f);
    expr_ptr ab_cd = expr::make_add(ab,cd);
    expr_ptr function = expr::make_add(ab_cd,ef);

    std::vector<std::string> order2 = {"a", "b", "c", "d","e","f"};
    std::vector<std::string> order3 = {"a", "c", "e", "b","d","f"};

    jabddl::vertex_ptr vertex2 = jabddl::robdd_build(function,0,order2);
    jabddl::vertex_ptr vertex3 = jabddl::robdd_build(function,0,order3);
    vertex::print(vertex2);
    
    complemented_vertex fun; 
    fun.complemented=false;
    fun.root = vertex2;

    std::string const filename = "test3_1.dot";
    jabddl::dump_dot(fun, filename.c_str() );

    jabddl::print_truth_table(vertex2, order2);
    
    vertex::print(vertex3);

    fun.complemented=false;
    fun.root = vertex3;

    std::string const filename1 = "test3_2.dot";
    jabddl::dump_dot(fun, filename1.c_str() );

    jabddl::print_truth_table(vertex3, order3);

#endif
#if TEST_NUM == 4

//    f1 = b + c      ordine a b c
//    f2 = a + b + c
    std::cout << "\033[33m" << "Test #4" <<"\033[0m " <<std::endl;

    expr_ptr a = expr::make_var("a");
    expr_ptr b = expr::make_var("b");
    expr_ptr c = expr::make_var("c");

    expr_ptr function_1 = expr::make_add(b,c);
    expr_ptr function_2 = expr::make_add(a,function_1);
    std::vector<std::string> order4 = {"a","b","c"};

    jabddl::vertex_ptr vertex4 = jabddl::robdd_build(function_2,0,order4);
    vertex::print(vertex4);
    
    jabddl::print_truth_table(vertex4, order4);

#endif
complemented_mode = 1;
#if TEST_NUM == 5


//    f = x1x2 + x1'x2 + x1x2'     ordine x1 x2, lati complementati
//    g = x1 + x2   
//    h = x1'x2'
    std::cout << "\033[33m" << "Test #5" <<"\033[0m " <<std::endl;

    expr_ptr x1 = expr::make_var("x1");
    expr_ptr x2 = expr::make_var("x2");
    expr_ptr x2_n = expr::make_neg(x2);
    expr_ptr x1_n = expr::make_neg(x1);


    expr_ptr x1x2 = expr::make_mul(x1,x2);
    expr_ptr x1_nx2 = expr::make_mul(x1_n,x2);
    expr_ptr x1x2_n = expr::make_mul(x1,x2_n);

    expr_ptr f_sub1 = expr::make_add(x1x2,x1_nx2);
    expr_ptr function_f = expr::make_add(f_sub1, x1x2_n);

    expr_ptr function_g = expr::make_add(x1,x2);
    expr_ptr function_h = expr::make_mul(x1_n,x2_n);

    std::vector<std::string> order = {"x1","x2"};

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

#endif
#if TEST_NUM == 6

//    f = ab + bcd'   ordine a b c d, lati complementati
//    g = ab + ac'd
//    f ---> g = ite(f,g,1)
    std::cout << "\033[33m" << "Test #6" <<"\033[0m " <<std::endl;

    expr_ptr a = expr::make_var("a");
    expr_ptr b = expr::make_var("b");
    expr_ptr c = expr::make_var("c");
    expr_ptr d = expr::make_var("d");


    expr_ptr ab = expr::make_mul(a,b);
    expr_ptr bc  = expr::make_mul(b,c);
    expr_ptr d_neg = expr::make_neg(d);
    expr_ptr bcd_neg = expr::make_mul(bc,d_neg);
    expr_ptr f = expr::make_add(ab,bcd_neg);
    expr_ptr c_neg = expr::make_neg(c);
    expr_ptr ac_neg = expr::make_mul(a,c_neg);
    expr_ptr ac_negd = expr::make_mul(ac_neg,d);
    expr_ptr g = expr::make_add(ab,ac_negd);
    std::vector<std::string> order5 = {"a","b","c","d"};

    jabddl::vertex_ptr f_vert = jabddl::robdd_build_comp(f,0,order5);
    jabddl::vertex_ptr g_vert = jabddl::robdd_build_comp(g,0,order5);
    jabddl::vertex_ptr h = jabddl::apply_ite_comp(f_vert,g_vert,jabddl::v1,0,order5);
    complemented_vertex fun;
    fun.complemented = false;
    fun.root = h;
    complemented_vertex h_prop =  jabddl::propagate_complemented(fun);

    std::cout <<"Function h complemented after propagation" << std::endl;
    if(h_prop.complemented)
        std::cout << "Function is complemented" <<std::endl;
    vertex::print(h_prop.root);

    std::string const filename = "test6.dot";
    jabddl::dump_dot(h_prop, filename.c_str() );

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

#endif
#if TEST_NUM == 7

//    f = ab + c   ordine a b c d, lati complementati
//    g = bc' + d
//    h = c + d'
//    ite(f,g,h)
    std::cout << "\033[33m" << "Test #7" <<"\033[0m " <<std::endl;
    
    expr_ptr a = expr::make_var("a");
    expr_ptr b = expr::make_var("b");
    expr_ptr c = expr::make_var("c");
    expr_ptr d = expr::make_var("d");
    expr_ptr c_neg = expr::make_neg(c);
    expr_ptr d_neg = expr::make_neg(d);


    expr_ptr ab = expr::make_mul(a,b);
    expr_ptr f = expr::make_add(ab,c);
    expr_ptr bc_neg = expr::make_mul(b,c_neg);
    expr_ptr g = expr::make_add(bc_neg,d);
    expr_ptr h_expr = expr::make_add(c,d_neg);

    
    std::vector<std::string> order4 = {"a","b","c"};
    std::vector<std::string> order5 = {"a","b","c","d"};
    std::vector<std::string> order6 = {"b","c","d"};
    std::vector<std::string> order7 = {"c","d"};

    vertex_ptr f_vert = jabddl::robdd_build_comp(f,0,order4);
    vertex_ptr g_vert = jabddl::robdd_build_comp(g,0,order6);
    vertex_ptr h = jabddl::robdd_build_comp(h_expr,0,order7);

    std::cout << "Function f comp" << std::endl;
    vertex::print(f_vert);
    std::cout << "Function g comp" << std::endl;
    vertex::print(g_vert);
    std::cout << "Function h comp" << std::endl;
    vertex::print(h);

    vertex_ptr f_vert_nc = jabddl::robdd_build(f,0,order4);
    vertex_ptr g_vert_nc = jabddl::robdd_build(g,0,order6);
    vertex_ptr h_nc = jabddl::robdd_build(h_expr,0,order7);

    std::cout << "Function f " << std::endl;
    vertex::print(f_vert_nc);
    std::cout << "Function g " << std::endl;
    vertex::print(g_vert_nc);
    std::cout << "Function h " << std::endl;
    vertex::print(h_nc);


    std::cout << "Function ite(f,g,h) comp" << std::endl;
    vertex_ptr k = jabddl::apply_ite_comp(f_vert,g_vert,h,0,order5);
    vertex::print(k);

    complemented_vertex fun;

    fun.complemented = false;
    fun.root = k;

    //Propagating complementation 
    complemented_vertex k_prop = jabddl::propagate_complemented(fun);

    if(k_prop.complemented)
        std::cout << "Function is complemented!\n";
    vertex::print(k_prop.root);

    //Printing of .dot file
    std::string const filename1 = "test7.dot";
    jabddl::dump_dot(k_prop, filename1.c_str());

    
    std::cout << "Function ite(f,g,h) " << std::endl;
    vertex_ptr k_nc = jabddl::apply_ite(f_vert_nc,g_vert_nc,h_nc,0,order5);
    vertex::print(k_nc);
    
#endif
#if TEST_NUM == 8
//    f = abd' + ab'd + a'c + a'c'd    ordine a b c d, lati complementati
   std::cout << "\033[33m" << "Test #8" <<"\033[0m " <<std::endl;
   
    expr_ptr a = expr::make_var("a");
    expr_ptr b = expr::make_var("b");
    expr_ptr c = expr::make_var("c");
    expr_ptr d = expr::make_var("d");
    expr_ptr c_neg = expr::make_neg(c);
    expr_ptr d_neg = expr::make_neg(d);
   
    expr_ptr ab = expr::make_mul(a,b);
    expr_ptr abd_neg = expr::make_mul(ab,d_neg);
    expr_ptr b_neg = expr::make_neg(b);
    expr_ptr ab_neg = expr::make_mul(a,b_neg);
    expr_ptr ab_negd = expr::make_mul(ab_neg,d);
    expr_ptr a_neg = expr::make_neg(a);
    expr_ptr a_negc = expr::make_mul(a_neg,c); 
    expr_ptr a_negc_neg = expr::make_mul(a_neg,c_neg);
    expr_ptr a_negc_negd = expr::make_mul(a_negc_neg,d);

    expr_ptr first_add = expr::make_add(abd_neg,ab_negd);
    expr_ptr second_add = expr::make_add(a_negc,a_negc_negd);
    expr_ptr f = expr::make_add(first_add,second_add);

    std::vector<std::string> order5 = {"a","b","c","d"};

    vertex_ptr f_vert = jabddl::robdd_build_comp(f,0,order5); 
    vertex_ptr f_vert_nc = jabddl::robdd_build(f,0,order5);    
    std::cout << "Function f compl " << std::endl;
    vertex::print(f_vert);  

    complemented_vertex fun;

    fun.complemented = false;
    fun.root = f_vert;  

    //Propagating complementation 
    complemented_vertex f_prop = jabddl::propagate_complemented(fun);

    //Printing of .dot file
    std::string const filename2 = "test8.dot";
    jabddl::dump_dot(f_prop, filename2.c_str());    

    std::cout << "Function f  " << std::endl;
    vertex::print(f_vert_nc);
   
#endif
#if TEST_NUM == 9

//    f = abc + a'c'd'    ordine a b c d, lati complementati
    std::cout << "\033[33m" << "Test #9" <<"\033[0m " <<std::endl;
    
    expr_ptr a = expr::make_var("a");
    expr_ptr b = expr::make_var("b");
    expr_ptr c = expr::make_var("c");
    expr_ptr d = expr::make_var("d");
    expr_ptr c_neg = expr::make_neg(c);
    expr_ptr d_neg = expr::make_neg(d);
    expr_ptr a_neg = expr::make_neg(a);
    

    expr_ptr ab = expr::make_mul(a,b);
    expr_ptr abc = expr::make_mul(ab,c);
    expr_ptr a_negc_neg = expr::make_mul(a_neg,c_neg);
    expr_ptr a_negc_negd_neg = expr::make_mul(a_negc_neg,d_neg);
    expr_ptr f = expr::make_add(abc,a_negc_negd_neg); 
    
    std::vector<std::string> order5 = {"a","b","c","d"};


    vertex_ptr f_vert = jabddl::robdd_build_comp(f,0,order5); 
//  vertex_ptr f_vert_nc = jabddl::robdd_build(f,0,order5); 

    std::cout << "Function f compl " << std::endl;
    vertex::print(f_vert);

    complemented_vertex fun;

    fun.complemented = false;
    fun.root = f_vert;

    //Propagating complementation 
    complemented_vertex f_prop = jabddl::propagate_complemented(fun);
    vertex::print(f_prop.root);

    //Printing of .dot file
    std::string const filename3 = "test9.dot";
    jabddl::dump_dot(f_prop, filename3.c_str());

    //std::cout << "Function f " << std::endl;
    //vertex::print(f_vert_nc);

#endif
#if TEST_NUM == 10

//    f = (a + be)(d + c)    ordine a b e d c, lati complementati
    std::cout << "\033[33m" << "Test #10" <<"\033[0m " <<std::endl;

    expr_ptr a = expr::make_var("a");
    expr_ptr b = expr::make_var("b");
    expr_ptr c = expr::make_var("c");
    expr_ptr d = expr::make_var("d");
    expr_ptr e = expr::make_var("e");

    expr_ptr be = expr::make_mul(b,e);
    expr_ptr first_add = expr::make_add(a,be);
    expr_ptr second_add = expr::make_add(d,c);
    expr_ptr f = expr::make_mul(first_add,second_add); 

    std::vector<std::string> order8 = {"a","b","e","d","c"};

    vertex_ptr f_vert = jabddl::robdd_build_comp(f,0,order8); 
    vertex_ptr f_vert_nc = jabddl::robdd_build(f,0,order8); 

    std::cout << "Function f compl " << std::endl;
    vertex::print(f_vert);   

    complemented_vertex fun; 

    fun.complemented = false;
    fun.root = f_vert;

    //Propagating complementation 
    complemented_vertex f_prop = jabddl::propagate_complemented(fun);

    //Printing of .dot file
    std::string const filename4 = "test10.dot";
    jabddl::dump_dot(f_prop, filename4.c_str());
 
    std::cout << "Function f " << std::endl;
    vertex::print(f_vert_nc);   

#endif
#if TEST_NUM == 11

//    f = a + b'c  ordine a b c d, lati complementati
//    g = b'c + d
//    h = b + c' + d
//    ite(f,g,h)
   std::cout << "\033[33m" << "Test #11" <<"\033[0m " <<std::endl;

    expr_ptr a = expr::make_var("a");
    expr_ptr b = expr::make_var("b");
    expr_ptr c = expr::make_var("c");
    expr_ptr d = expr::make_var("d");
    expr_ptr b_neg = expr::make_neg(b);
    expr_ptr c_neg = expr::make_neg(c);


    expr_ptr b_negc = expr::make_mul(b_neg,c);
    expr_ptr f = expr::make_add(a,b_negc); 
    expr_ptr g = expr::make_add(b_negc,d);
    expr_ptr b_c_neg = expr::make_add(b,c_neg);
    expr_ptr h_expr = expr::make_add(b_c_neg,d);

    std::vector<std::string> order5 = {"a","b","c","d"};

    vertex_ptr f_vert = jabddl::robdd_build_comp(f,0,order5);
    vertex_ptr g_vert = jabddl::robdd_build_comp(g,0,order5);
    vertex_ptr h_vert = jabddl::robdd_build_comp(h_expr,0,order5);

    std::cout << "Function k compl " << std::endl;
    vertex_ptr k = jabddl::apply_ite_comp(f_vert,g_vert,h_vert,0,order5);
    vertex::print(k);

    complemented_vertex fun;

    fun.complemented = false;
    fun.root = k;

    //Propagating complementation 
    complemented_vertex k_prop = jabddl::propagate_complemented(fun);

    //Printing of .dot file
    std::string const filename5 = "test11.dot";
    jabddl::dump_dot(k_prop, filename5.c_str());

    vertex_ptr f_vert_nc = jabddl::robdd_build(f,0,order5);
    vertex_ptr g_vert_nc = jabddl::robdd_build(g,0,order5);
    vertex_ptr h_vert_nc = jabddl::robdd_build(h_expr,0,order5);

    std::cout << "Function k " << std::endl;
    vertex_ptr k_nc = jabddl::apply_ite(f_vert_nc,g_vert_nc,h_vert_nc,0,order5);  
    vertex::print(k_nc);    

#endif
#if TEST_NUM == 12

//    f = b + cd   ordine a b c d, lati complementati
//    g = b + cd
//    h = a'c + b
//    ite(f,g,h)
    std::cout << "\033[33m" << "Test #12" <<"\033[0m " <<std::endl;

    expr_ptr a = expr::make_var("a");
    expr_ptr b = expr::make_var("b");
    expr_ptr c = expr::make_var("c");
    expr_ptr d = expr::make_var("d");
    expr_ptr a_neg = expr::make_neg(a);
    expr_ptr c_neg = expr::make_neg(c);

    expr_ptr cd = expr::make_mul(c,d);
    expr_ptr a_negc = expr::make_mul(a_neg,c);

    expr_ptr f = expr::make_add(b,cd);
    expr_ptr g = expr::make_add(b,cd);
    expr_ptr h_expr = expr::make_add(a_negc,b);   

    std::vector<std::string> order5 = {"a","b","c","d"};

    vertex_ptr f_vert = jabddl::robdd_build_comp(f,0,order5);
    vertex_ptr g_vert = jabddl::robdd_build_comp(g,0,order5);
    vertex_ptr h_vert = jabddl::robdd_build_comp(h_expr,0,order5);

    vertex_ptr k = jabddl::apply_ite_comp(f_vert,g_vert,h_vert,0,order5);  

    std::cout << "Function k comp " << std::endl;
    vertex::print(k);   
    
    complemented_vertex fun;    
    fun.complemented = false;
    fun.root = k;

    //Propagating complementation 
    complemented_vertex k_prop = jabddl::propagate_complemented(fun);

    //Printing of .dot file
    std::string const filename6 = "test12.dot";
    jabddl::dump_dot(k_prop, filename6.c_str());

    vertex_ptr f_vert_nc = jabddl::robdd_build(f,0,order5);
    vertex_ptr g_vert_nc = jabddl::robdd_build(g,0,order5);
    vertex_ptr h_vert_nc = jabddl::robdd_build(h_expr,0,order5);

    std::cout << "Function k " << std::endl;
    vertex_ptr k_nc = jabddl::apply_ite(f_vert_nc,g_vert_nc,h_vert_nc,0,order5);  
    vertex::print(k_nc); 

    jabddl::print_truth_table(k, order5,fun.complemented);

#endif
return 0;
}