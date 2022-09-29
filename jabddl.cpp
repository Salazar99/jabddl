#include "jabddl.hpp"

#include <sstream>
#include <cassert>
#include <iostream>

namespace jabddl {

//unique table to represent the vertices of all robdds
static std::vector<vertex_ptr> unique_table;

//0 and 1 leaf 
vertex_ptr v0 = std::make_shared<vertex>("v0");
vertex_ptr v1 = std::make_shared<vertex>("v1");

//0 and 1 variables
auto v0_v = jabddl::expr::make_var({"v0"});
auto v1_v = jabddl::expr::make_var({"v1"});

vertex::vertex(const std::string& name)
: root{{name}} { unique_table.push_back(v0);
                 unique_table.push_back(v1);
                }

expr::expr() : type{expr_type::Var} { }
expr::~expr() { }

expr_ptr expr::make_add(expr_ptr a, expr_ptr b) {
    auto res = new expr{};
    res->type = expr_type::Add;
    res->args.l = a;
    res->args.r = b;
    return res;
}
expr_ptr expr::make_mul(expr_ptr a, expr_ptr b) {
    auto res = new expr{};
    res->type = expr_type::Mul;
    res->args.l = a;
    res->args.r = b;
    return res;
}
expr_ptr expr::make_neg(expr_ptr n) {
    auto res = new expr{};
    res->type = expr_type::Not;
    res->arg.c = n;
    return res;
}
expr_ptr expr::make_var(variable var) {
    auto res = new expr{};
    res->var.name = var;
    return res;
};

expr_ptr ite(expr_ptr a, expr_ptr b, expr_ptr c) {

    // If a then b
    auto choise1 = expr::make_mul(a, b);
    
    // if not a then c
    auto neg_a = expr::make_neg(a);
    auto choise2 = expr::make_mul(neg_a, c);

    // x1 is 1 or 0
    return expr::make_add(choise1, choise2);
}

static void print_rec(std::stringstream& stream, const expr_ptr expr) {
    switch (expr->type) {
        case expr_type::Add: {
            stream << "(";
            print_rec(stream, expr->args.l);
            stream << " + ";
            print_rec(stream, expr->args.r);
            stream << ")";
        } break;
        case expr_type::Mul: {
            stream << "(";
            print_rec(stream, expr->args.l);
            stream << " * ";
            print_rec(stream, expr->args.r);
            stream << ")";
        } break;
        case expr_type::Not: {
            stream << "!";
            print_rec(stream, expr->arg.c);
        } break;
        case expr_type::Var: {
            stream << expr->var.name.name;
        } break;
    }
}

void expr::print(const expr_ptr expr) {
    std::stringstream stream;
    print_rec(stream, expr);
    std::cout << stream.str() << std::endl;
}

vertex::vertex(variable root, vertex_ptr l, vertex_ptr r) 
: root{root}, lsubtree{l}, rsubtree{r} { }

vertex_ptr old_or_new(variable root, vertex_ptr lst, vertex_ptr rst){
    
    vertex_ptr v;
    auto result = lookup(unique_table, root, lst, rst);
    if (result.has_value())
        return *result;
    else{
        /*create a new vertex*/
        v = std::make_shared<vertex>(root, lst, rst);
        //adds it to the unique table
        unique_table.push_back(v);
        return v;
    }
}

expr_ptr evaluate(expr_ptr root, variable var, bool value) {
    if(root->type == jabddl::expr_type::Var && root->var.name.name == var.name){
              if(value)
                root->var = v1_v->var;
              else
                root->var = v0_v->var;
    }
    
    switch(root->type){
        case jabddl::expr_type::Mul: {
            auto l_res = evaluate(root->args.l, var, value);
            auto r_res = evaluate(root->args.r, var, value);

            //0 * 0 = 0
            if(l_res->var.name.name == v0_v->var.name.name || r_res->var.name.name == v0_v->var.name.name)
                return v0_v;

            // 1 * 1 = 1
            if(l_res->var.name.name == v1_v->var.name.name && r_res->var.name.name == v1_v->var.name.name)
                return v1_v;

            // 1 * x = x
            if (l_res->var.name.name == v1_v->var.name.name)
                return r_res;

            // x * 1 = x
            if (r_res->var.name.name == v1_v->var.name.name)
                return l_res;

            //x * 0 = 0
            if(l_res->var.name.name == v0_v->var.name.name)
                return v0_v;

            if(r_res->var.name.name == v0_v->var.name.name)
                return v0_v;

            return root;
        } break;
        case jabddl::expr_type::Add:{
            auto l_res = evaluate(root->args.l, var, value);
            auto r_res = evaluate(root->args.r, var, value);
            
            //0 + 0 = 0
            if(l_res->var.name.name == v0_v->var.name.name && r_res->var.name.name == v0_v->var.name.name)
                return v0_v;
            //1 + 1 = 1
            if(l_res->var.name.name == v1_v->var.name.name && r_res->var.name.name == v1_v->var.name.name)
                return v1_v;    

            //0 + x = x
            if(l_res->var.name.name == v0_v->var.name.name)
                return r_res;

             if(r_res->var.name.name == v0_v->var.name.name)
                return l_res;

            //1 + x = 1 
            if(l_res->var.name.name == v1_v->var.name.name)
                return v1_v;    

            if(r_res->var.name.name == v1_v->var.name.name)
                return v1_v;               

            return root;
        }    break;

        case jabddl::expr_type::Not:{
            evaluate(root->arg.c, var, value);
            if(root->arg.c->var.name.name == v0_v->var.name.name){
                return v1_v;
            }    
            else if(root->arg.c->var.name.name == v1_v->var.name.name){
                return v0_v;
            }  
            else return root;
        }break;

        case jabddl::expr_type::Var:{
            return root;
        }break;

        default:{
            break;
            return root;
        }
    } 
}

expr_ptr copy_expr_rec(const expr_ptr root) {
    
    // Copied expr
    expr_ptr result = new expr{};
    result->type = root->type;

    // Copy childrens
    switch (result->type) {
        case expr_type::Add:
        case expr_type::Mul: {
            result->args.l = copy_expr_rec(root->args.l);
            result->args.r = copy_expr_rec(root->args.r);
        } break;
        case expr_type::Not: {
            result->arg.c = copy_expr_rec(root->arg.c);
        } break;
        case expr_type::Var: {
            result->var.name = root->var.name;
        } break;
    }

    return result;
}

vertex_ptr robdd_build(expr_ptr f, int i, const std::vector<variable>& ord) {
     vertex_ptr l,r;
     variable root;

    if(f->type == jabddl::expr_type::Var){
        if(f->var.name.name == v0_v->var.name.name)
            return v0;
        if(f->var.name.name == v1_v->var.name.name)
            return v1; 
    }
    else{

        assert(i < ord.size());
        root.name = ord[i].name;
        
        expr_ptr l_copy = copy_expr_rec(f);
        l = robdd_build(evaluate(l_copy, ord[i], true), i+1, ord);
        std::cout<< "f valutata per: " << ord[i].name <<" positivo\n";
        expr::print(l_copy);
        //delete_expr(l_copy);

        expr_ptr r_copy = copy_expr_rec(f);
        r = robdd_build(evaluate(r_copy, ord[i], false), i+1, ord);
        std::cout<< "f valutata per: " << ord[i].name <<" positivo\n";
        expr::print(r_copy);
        //delete_expr(r_copy);
    }
    if(l->root.name == r->root.name && l->lsubtree == r->lsubtree && l->rsubtree == r->rsubtree)
        return l;
    else
        return old_or_new(root,l,r);

}

std::optional<vertex_ptr> lookup(const std::vector<vertex_ptr>& unique_table, variable root, vertex_ptr lst, vertex_ptr rst){
    for (auto& vertex : unique_table) {
        if (vertex->root.name == root.name && vertex->lsubtree == lst && vertex->rsubtree == rst)
            return std::make_optional<vertex_ptr>(vertex);
    }
    return std::nullopt;
}

} // namespace jabddl