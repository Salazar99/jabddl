#pragma once

#include<iostream>
#include<sstream>

/// @brief expression with value 0 
inline std::string v0_v = std::string("v0");
/// @brief expression with value 1 
inline std::string v1_v = std::string("v1");

struct expr;
using expr_ptr = expr*;

/// @brief enumeration for type of expression
enum class expr_type {
    Add,
    Not,
    Mul,
    Var
};

/// @brief expression with 2 arguments
struct expr_arg_2 { 
    /// @brief left member
    expr_ptr l; 
    /// @brief right member
    expr_ptr r; 
};

/// @brief expression with 1 arument
struct expr_arg_1 { 
    /// @brief member of expression
    expr_ptr c; 
};
/// @brief expression for a variable
struct expr_var { 
    /// @brief variable name
    std::string* name;
};

/// @brief Structure represents an expression 
struct expr {
    /// @brief type of the expression
    expr_type type;
    /// @brief dependinf on the type we can have different fields
    union {
        expr_arg_2 args;
        expr_arg_1 arg;
        expr_var var;
    };

    /// @brief constructor
    explicit expr();
    ~expr();

    /// @brief create an add 
    /// @param a first operand
    /// @param b second operand
    /// @return new add expression
    static expr_ptr make_add(expr_ptr a, expr_ptr b);
    /// @brief create a multiplication
    /// @param a fist operand
    /// @param b second operand
    /// @return new multiplication expression
    static expr_ptr make_mul(expr_ptr a, expr_ptr b);
    /// @brief create a negation of a variable
    /// @param n variable to negate
    /// @return new negation of var n
    static expr_ptr make_neg(expr_ptr n);
    /// @brief create a variable from a string
    /// @param var string with variable name 
    /// @return new variable
    static expr_ptr make_var(const std::string& var);

    /// @brief print the expression given for debugging purposes
    /// @param expr pointer to the root of the expression structure
    static void print(const expr_ptr expr);
};

/// @brief return the expression that represent the ite(a,b,c) operation
/// @param a expr for the if 
/// @param b expr then
/// @param c expr else 
/// @return expr which represent (a*b) + (!a*c)
expr_ptr ite(expr_ptr a, expr_ptr b, expr_ptr c);

/// @brief Evaluate given expression
/// @param root expression to be evaluated
/// @param var  variable for evaluation
/// @param value variable value for evaluation (T|F)
/// @return pointer to evaluated expression
expr_ptr evaluate(expr_ptr root, const std::string& var, bool value);

/// @brief Support function for deep copy of expression 
/// @param root expression to be copied
/// @return pointer to a new instance of root expression
expr_ptr copy_expr_rec(const expr_ptr root);
