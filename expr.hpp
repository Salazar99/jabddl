#pragma once

#include<iostream>
#include<sstream>

inline std::string v0_v = std::string("v0");
inline std::string v1_v = std::string("v1");

struct expr;
using expr_ptr = expr*;

enum class expr_type {
    Add,
    Not,
    Mul,
    Var
};


struct expr_arg_2 { expr_ptr l; expr_ptr r; };
struct expr_arg_1 { expr_ptr c; };
struct expr_var   { std::string* name; };

/// Represents an expression 
struct expr {
    expr_type type;
    union {
        expr_arg_2 args;
        expr_arg_1 arg;
        expr_var var;
    };

    explicit expr();
    ~expr();

    static expr_ptr make_add(expr_ptr a, expr_ptr b);
    static expr_ptr make_mul(expr_ptr a, expr_ptr b);
    static expr_ptr make_neg(expr_ptr n);
    static expr_ptr make_var(const std::string& var);

    static void print(const expr_ptr expr);
};

/// @brief return the expression that represent the ite(a,b,c) operation
/// @param a expr for the if 
/// @param b expr then
/// @param c expr else 
/// @return expr which represent (a*b) + (!a*c)
expr_ptr ite(expr_ptr a, expr_ptr b, expr_ptr c);

/// @brief Evaluate given function
/// @param root expression 
/// @param var  variable to which evaluate
/// @param value variable value for evaluation (T|F)
/// @return pointer to evaluated expression
expr_ptr evaluate(expr_ptr root, const std::string& var, bool value);

/// @brief Support function for deep copy of expression 
/// @param root expression to be copied
/// @return pointer to a new instance of root expression
expr_ptr copy_expr_rec(const expr_ptr root);
