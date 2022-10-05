#include "jabddl.hpp"

#include <sstream>
#include <cassert>
#include <iostream>

namespace jabddl {

//unique table to represent the vertices of all robdds
static std::vector<vertex_ptr> unique_table;

void initialize() {
    unique_table.push_back(v0);
    unique_table.push_back(v1);
}

//0 and 1 variables used to valuate expressions
auto v0_v = jabddl::expr::make_var({"v0"});
auto v1_v = jabddl::expr::make_var({"v1"});

vertex::vertex(const std::string& name)
: root{{name}} {}

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

static void print_vert_rec(std::stringstream& stream, vertex_ptr vert, int depth){
     ++depth;
    if(vert->root.name == "v0" || vert->root.name == "v1"){
        stream << "\n";
        for(int i = 1; i < depth-1; i++) stream << " |";
        if(depth >1) stream << " L";
        stream << "[" << vert->root.name << "]";
    }else{
       
        stream << "\n";
        for(int i = 1; i < depth-1; i++) stream << " |";
        if(depth >1) stream << " +";
        stream << "[" << vert->root.name << "]";
        print_vert_rec(stream, vert->lsubtree,depth );
        print_vert_rec(stream,vert->rsubtree, depth);
       
    }
   
}


void vertex::print(const vertex_ptr vert){
    std::stringstream stream;
    print_vert_rec(stream, vert, 0);
    std::cout << stream.str() <<std::endl;
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
        auto l_tree = lookup(unique_table,lst->root,lst->lsubtree,lst->rsubtree);
        auto r_tree = lookup(unique_table,rst->root,rst->lsubtree,rst->rsubtree);
        if(l_tree.has_value())
            lst = l_tree.value(); 
        if(r_tree.has_value())
            rst = r_tree.value();   
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
              return root;
    }
    
    switch(root->type){
        case jabddl::expr_type::Mul: {
            root->args.l = evaluate(root->args.l, var, value);
            root->args.r = evaluate(root->args.r, var, value);

            //0 * 0 = 0
            if(root->args.l->var.name.name == v0_v->var.name.name || root->args.r->var.name.name == v0_v->var.name.name)
                return v0_v;

            // 1 * 1 = 1
            if(root->args.l->var.name.name == v1_v->var.name.name && root->args.r->var.name.name == v1_v->var.name.name)
                return v1_v;

            // 1 * x = x
            if (root->args.l->var.name.name == v1_v->var.name.name)
                return root->args.r;

            // x * 1 = x
            if (root->args.r->var.name.name == v1_v->var.name.name)
                return root->args.l;

            //x * 0 = 0
            if(root->args.l->var.name.name == v0_v->var.name.name)
                return v0_v;

            if(root->args.r->var.name.name == v0_v->var.name.name)
                return v0_v;

            
            return root;
        } break;
        case jabddl::expr_type::Add:{
            root->args.l = evaluate(root->args.l, var, value);
            root->args.r = evaluate(root->args.r, var, value);
            
            //0 + 0 = 0
            if(root->args.l->var.name.name == v0_v->var.name.name && root->args.r->var.name.name == v0_v->var.name.name)
                return v0_v;
            //1 + 1 = 1
            if(root->args.l->var.name.name == v1_v->var.name.name && root->args.r->var.name.name == v1_v->var.name.name)
                return v1_v;    

            //0 + x = x
            if(root->args.l->var.name.name == v0_v->var.name.name)
                return root->args.r;

             if(root->args.r->var.name.name == v0_v->var.name.name)
                return root->args.l;

            //1 + x = 1 
            if(root->args.l->var.name.name == v1_v->var.name.name)
                return v1_v;    

            if(root->args.r->var.name.name == v1_v->var.name.name)
                return v1_v;               

            return root;
        }    break;

        case jabddl::expr_type::Not:{
            auto res = evaluate(root->arg.c, var, value);
            if(res->type == jabddl::expr_type::Var){
                if(res->var.name.name == v0_v->var.name.name){
                    return v1_v;
                }    
                else if(res->var.name.name == v1_v->var.name.name){
                    return v0_v;
                }  
            }
            return root;
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

bool vertex_compare(vertex_ptr vertex1,vertex_ptr vertex2){
    bool result = false;
    if((vertex1->root.name == "v0" && vertex2->root.name == "v0") || (vertex1->root.name == "v1" && vertex2->root.name == "v1")){
        return true;
    }
    else if((vertex1->root.name == "v1" && vertex2->root.name == "v0") || (vertex1->root.name == "v0" && vertex2->root.name == "v1")){
        return false;
    }
    else if(vertex1->root.name != "v0" && vertex2->root.name != "v1" && vertex1->root.name != "v1" && vertex2->root.name != "v0"){
        result = vertex_compare(vertex1->lsubtree,vertex2->lsubtree) && vertex_compare(vertex1->rsubtree,vertex2->rsubtree) && (vertex1->root.name == vertex2->root.name);
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

        //assert(i < ord.size());
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
   
    if(vertex_compare(l,r)/*l->lsubtree == r->lsubtree && l->rsubtree == r->rsubtree*/)
        return l;
    else
        return old_or_new(root,l,r);

}

vertex_ptr apply_ite(vertex_ptr f, vertex_ptr g, vertex_ptr h, int i ,const std::vector<variable>& ord){
     vertex_ptr l,r;
     variable root;

    if(f->root.name  == "v1" )
        return g;
    if(f->root.name == "v0" )
        return h;
    else if (g->root.name == "v1" && h->root.name == "v0")
        return f;
    else{
        root.name = ord[i].name;
        l = apply_ite(/*f,g,h evaluated for "root" positive*/,i+1,ord);
        r = apply_ite(/*f,g,h evaluated for "root" negative*/,i+1,ord);
        
        if(vertex_compare(l,r))
            return l;
        else 
            return old_or_new(root,l,r);
    }
}

std::optional<vertex_ptr> lookup(const std::vector<vertex_ptr>& unique_table, variable root, vertex_ptr lst, vertex_ptr rst){
    for (auto& vertex : unique_table) {
        if (vertex->root.name == root.name && vertex->lsubtree == lst && vertex->rsubtree == rst)
            return std::make_optional<vertex_ptr>(vertex);
    }
    return std::nullopt;
}

} // namespace jabddl