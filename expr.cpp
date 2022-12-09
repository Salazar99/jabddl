#include "expr.hpp"

expr::expr() : type{expr_type::Var} { }
expr::~expr() {
    if (type == expr_type::Var)
        delete var.name;
}

//* == and, + == or
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
expr_ptr expr::make_var(const std::string& var) {
    auto res = new expr{};
    res->var.name = new std::string(var);
    return res;
};

expr_ptr ite(expr_ptr a, expr_ptr b, expr_ptr c) {

    // If a then b (a*b)
    auto choise1 = expr::make_mul(a, b);
    
    // if not a then c  (!a*c)
    auto neg_a = expr::make_neg(a);
    auto choise2 = expr::make_mul(neg_a, c);

    // x1 is 1 or 0 (choice1 + choice2)
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
            stream << *expr->var.name;
        } break;
    }
}

void expr::print(const expr_ptr expr) {
    std::stringstream stream;
    print_rec(stream, expr);
    std::cout << stream.str() << std::endl;
}


/// @brief Evaluate given function
/// @param root expression 
/// @param var  variable to which evaluate
/// @param value variable value for evaluation (T|F)
/// @return pointer to evaluated expression
expr_ptr evaluate(expr_ptr root, const std::string& var, bool value) {
    if(root->type == expr_type::Var && *root->var.name == var){
              if(value)
                root->var.name = new std::string(v1_v);
              else
                root->var.name = new std::string(v0_v);
              return root;
    }
    
    switch(root->type){
        case expr_type::Mul: {
            root->args.l = evaluate(root->args.l, var, value);
            root->args.r = evaluate(root->args.r, var, value);

            //0 * 0 = 0
            if(*root->args.l->var.name == v0_v || *root->args.r->var.name == v0_v)
                return expr::make_var(v0_v);

            // 1 * 1 = 1
            if(*root->args.l->var.name == v1_v && *root->args.r->var.name == v1_v)
                return  expr::make_var(v1_v);

            // 1 * x = x
            if (*root->args.l->var.name == v1_v)
                return root->args.r;

            // x * 1 = x
            if (*root->args.r->var.name == v1_v)
                return root->args.l;

            // 0 * x = 0
            if(*root->args.l->var.name == v0_v)
                return expr::make_var(v0_v);
            // x * 0 = 0
            if(*root->args.r->var.name == v0_v)
                return expr::make_var(v0_v);

            
            return root;
        } break;
        case expr_type::Add:{
            root->args.l = evaluate(root->args.l, var, value);
            root->args.r = evaluate(root->args.r, var, value);
            
            //0 + 0 = 0
            if(*root->args.l->var.name == v0_v && *root->args.r->var.name == v0_v)
                return expr::make_var(v0_v);

            //1 + 1 = 1
            if(*root->args.l->var.name == v1_v && *root->args.r->var.name == v1_v)
                return expr::make_var(v1_v);    

            //0 + x = x
            if(*root->args.l->var.name == v0_v)
                return root->args.r;
            //x + 0 = 0 
             if(*root->args.r->var.name == v0_v)
                return root->args.l;

            //1 + x = 1 
            if(*root->args.l->var.name == v1_v)
                return expr::make_var(v1_v);    
            //x + 1 = 1 
            if(*root->args.r->var.name == v1_v)
                return expr::make_var(v1_v);              

            return root;
        }    break;

        case expr_type::Not:{
            auto res = evaluate(root->arg.c, var, value);
            if(res->type == expr_type::Var){
                if(*res->var.name == v0_v){
                    return expr::make_var(v1_v);
                }    
                else if(*res->var.name == v1_v){
                    return expr::make_var(v0_v);
                }  
            }
            return root;
        }break;

        case expr_type::Var:{
            return root;
        }break;

        default:{
            break;
            return root;
        }
    } 
    exit(-1);
}


/// @brief Support function for deep copy of expression 
/// @param root expression to be copied
/// @return pointer to a new instance of root expression
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
            result->var.name = new std::string(*root->var.name);
        } break;
    }

    return result;
}