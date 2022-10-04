#pragma once

#include <vector>
#include <memory>
#include <string>
#include <optional>

namespace jabddl {

void initialize();

struct variable { std::string name; };

enum class expr_type {
    Add,
    Not,
    Mul,
    Var
};

struct expr;
struct vertex;
using expr_ptr = expr*;

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

    static expr_ptr make_add(expr_ptr a, expr_ptr b);
    static expr_ptr make_mul(expr_ptr a, expr_ptr b);
    static expr_ptr make_neg(expr_ptr n);
    static expr_ptr make_var(variable var);

    static void print(const expr_ptr expr);
};

expr_ptr ite(expr_ptr a, expr_ptr b, expr_ptr c);

struct vertex;
using vertex_ptr = std::shared_ptr<vertex>;

//0 and 1 leaf 
static inline vertex_ptr v0 = std::make_shared<vertex>("v0");
static inline vertex_ptr v1 = std::make_shared<vertex>("v1");


//structure to represent a vertex of the bdd
struct vertex {
    variable root;
    vertex_ptr lsubtree, rsubtree;

    /// @brief Constructs a new vertex
    /// @param root variable of the vertex
    /// @param l left subtree
    /// @param r right subtree
    vertex(variable root, vertex_ptr l, vertex_ptr r);

    explicit vertex(const std::string& name);

    static void print(const vertex_ptr vert);
};

///@param root is the pointer to the root of the vertex to analyse, 
///@param lst left subtree.
///@param rst right subtree. 
///@brief The function takes a vertex in input and return:
///          a new vertex if the one in input does not exists at the moment or
///          a pointer to the vertex if it is already present
vertex_ptr old_or_new(variable root, vertex_ptr lst, vertex_ptr rst);


///@param f is the expression on wich the bdd must be built,
///@param i represent the first variable to be used for shannon expression   
///@brief: Takes in input an expression and first variable and return the 
///        pointer to the root of the robdd
vertex_ptr robdd_build(expr_ptr f, int i, const std::vector<variable>& ord);


///@param unique_table is a pointer to the unique table of vertices;
///@param root is the pointer to the root of the vertex to analyse, 
///@param lst left subtree.
///@param rst right subtree.       
///@brief Given a vertex, it checks if it is present inside the unique table and returns it
std::optional<vertex_ptr> lookup(const std::vector<vertex_ptr>& unique_table, variable root, vertex_ptr lst, vertex_ptr rst);

/// @param f expression to be evaluated 
/// @param var variable on wich we want to evaluate the expression
/// @brief Evaluates the expression f with respect to the variable var
expr compute(expr f, std::string var);

bool vertex_compare(vertex_ptr vertex1,vertex_ptr vertex2);


} // namespace jabdd 