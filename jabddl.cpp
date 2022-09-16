#include "jabbdl.hpp"

#include <sstream>
#include <iostream>

namespace jabbdl {

//unique table to represent the vertices of all robdds
static std::vector<vertex_ptr> unique_table;

expr::expr() : type{expr_type::Var} { }
expr::~expr() { }

expr_ptr expr::make_add(expr_ptr& a, expr_ptr& b) {
    auto res = std::make_shared<expr>();
    res->type = expr_type::Add;
    res->args.l = a;
    res->args.r = b;
    return res;
}
expr_ptr expr::make_mul(expr_ptr& a, expr_ptr& b) {
    auto res = std::make_shared<expr>();
    res->type = expr_type::Mul;
    res->args.l = a;
    res->args.r = b;
    return res;
}
expr_ptr expr::make_neg(expr_ptr& n) {
    auto res = std::make_shared<expr>();
    res->type = expr_type::Not;
    res->arg.c = n;
    return res;
}
expr_ptr expr::make_var(variable var) {
    auto res = std::make_shared<expr>();
    res->var.name = var;
    return res;
};

expr_ptr ite(expr_ptr& a, expr_ptr& b, expr_ptr& c) {

    // If a then b
    auto choise1 = expr::make_mul(a, b);
    
    // if not a then c
    auto neg_a = expr::make_neg(a);
    auto choise2 = expr::make_mul(neg_a, c);

    // x1 is 1 or 0
    return expr::make_add(choise1, choise2);
}

static void print_rec(std::stringstream& stream, const expr_ptr& expr) {
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

void expr::print(const expr_ptr& expr) {
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
        return v;
    }
}

vertex_ptr robdd_build(struct expr f, int i) {


}

std::optional<vertex_ptr> lookup(const std::vector<vertex_ptr>& unique_table, variable root, vertex_ptr lst, vertex_ptr rst){
    for (auto& vertex : unique_table) {
        if (vertex->root.name == root.name && vertex->lsubtree == lst && vertex->rsubtree == rst)
            return std::make_optional<vertex_ptr>(vertex);
    }
    return std::nullopt;
}


} // namespace jabbdl