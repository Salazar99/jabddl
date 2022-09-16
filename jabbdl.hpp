#pragma once

#include <vector>
#include <memory>
#include <string>
#include <optional>

namespace jabbdl {

struct variable { std::string name; };

enum class expr_type {
    Add,
    Not,
    Mul,
    Var
};

struct expr;
using expr_ptr = std::shared_ptr<expr>;

struct expr_arg_2 { expr_ptr l; expr_ptr r; };
struct expr_arg_1 { expr_ptr c; };
struct expr_var   { variable name; };

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

    static expr_ptr make_add(expr_ptr& a, expr_ptr& b);
    static expr_ptr make_mul(expr_ptr& a, expr_ptr& b);
    static expr_ptr make_neg(expr_ptr& n);
    static expr_ptr make_var(variable var);

    static void print(const expr_ptr& expr);
};

expr_ptr ite(expr_ptr& a, expr_ptr& b, expr_ptr& c);

expr create_expr(const std::string& f);

struct vertex;
using vertex_ptr = std::shared_ptr<vertex>;

//structure to represent a vertex of the bdd
struct vertex {
    variable root;
    vertex_ptr lsubtree, rsubtree;

    /// @brief Constructs a new vertex
    /// @param root variable of the vertex
    /// @param l left subtree
    /// @param r right subtree
    vertex(variable root, vertex_ptr l, vertex_ptr r);
};

//@input: root is the pointer to the root of the vertex to analyse, 
//        lst and rst are respectively the left and right subtree.
//@output: pointer to a vertex representing the one given in input
//@brief: The function takes a vertex in input and return:
//          a new vertex if the one in input does not exists at the moment or
//          a pointer to the vertex if it is already present
vertex_ptr old_or_new(variable root, vertex_ptr lst, vertex_ptr rst);


//@input: "f" is the expression on wich the bdd must be built,
//        "i" represent the first variable to be used for shannon expression   
//@output: pointer to the vertex of the robbd
//@brief: Takes in input an expression and first variable and return the 
//        pointer to the root of the robdd
vertex_ptr robdd_build(expr f, int i);


//@input: unique_table is a pointer to the unique table of vertices;
//        root is the pointer to the root of the vertex to analyse, 
//        lst and rst are respectively the left and right subtree.         
//@output: pointer to the vertex if found, otherwise nullptr;
//@brief: Given a vertex, it checks if it is present inside the unique table and returns it
std::optional<vertex_ptr> lookup(const std::vector<vertex_ptr>& unique_table, variable root, vertex_ptr lst, vertex_ptr rst);


} // namespace jabbd 