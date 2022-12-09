#pragma once

#include <vector>
#include <memory>
#include <string>
#include <optional>
#include <fstream>
#include <regex>
#include <sstream>
#include <cassert>
#include <iostream>
#include <cmath>

extern int verbosity;
//IMPORTANT: In this implementetation the left child of the vertexes is considered to be 
// the high branch of the tree with root in that vertex.
namespace jabddl {

/// @brief Create and add to unique_table the 0 and 1 leaves
void initialize();

enum class expr_type {
    Add,
    Not,
    Mul,
    Var
};

struct expr;
struct vertex;
using expr_ptr = expr*;
using vertex_ptr = std::shared_ptr<vertex>;

struct fun{
    std::string func_name;
    bool tbp = false; //to be printed?
    jabddl::expr_ptr ite_if;
    jabddl::expr_ptr ite_then;
    jabddl::expr_ptr ite_else;
};


struct context{

    std::vector<std::string> vars;
    std::unordered_map<std::string, jabddl::fun> funcs;

    std::unordered_map<std::string, jabddl::vertex_ptr> root_vertexes;

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


//0 and 1 leaf 
static inline vertex_ptr v0 = std::make_shared<vertex>("v0");
static inline vertex_ptr v1 = std::make_shared<vertex>("v1");

//unique table to represent the vertices of all robdds
extern std::vector<vertex_ptr> unique_table;

//structure to represent a vertex of the bdd
struct vertex {
    std::string root;
    vertex_ptr lsubtree, rsubtree;
    bool complemented_l,complemented_r;
   
    /// @brief Constructs a new vertex
    /// @param root variable of the vertex
    /// @param l left subtree
    /// @param r right subtree
    vertex(const std::string& root, vertex_ptr l, vertex_ptr r);
    
    /// @brief Constructs a new vertex for complemented edges 
    /// @param root variable of the vertex
    /// @param l left subtree
    /// @param r right subtree
    /// @param complemented_l true if left child is complemented, false otherwise
    /// @param complemented_r true if right child is complemented, false otherwise
    vertex(const std::string& root, vertex_ptr l, vertex_ptr r, bool complemented_l, bool complemented_r);

    /// @brief constructor for vertex without childs, used for leaves declaration
    /// @param name variable name
    explicit vertex(const std::string& name);

    /// @brief print the bdd with vert as root in a visually friendly form 
    /// @param vert root of the bdd
    static void print(const vertex_ptr vert);
};

///@brief The function takes a vertex in input and return:
///          a new vertex if the one in input does not exists at the moment or
///          a pointer to the vertex if it is already present
///@param root is the pointer to the root of the vertex to analyse, 
///@param lst left subtree.
///@param rst right subtree. 
vertex_ptr old_or_new(const std::string& root, vertex_ptr lst, vertex_ptr rst);

///@brief The function takes a vertex in input and return:
///          a new vertex if the one in input does not exists at the moment or
///          a pointer to the vertex if it is already present.
///       It works with complemented edges.
///@param root is the pointer to the root of the vertex to analyse, 
///@param lst left subtree.
///@param rst right subtree. 
vertex_ptr old_or_new_comp(const std::string& root, vertex_ptr lst, vertex_ptr rst);


///@brief: Takes in input an expression and first variable and return the 
///        pointer to the root of the robdd
///@param f is the expression on wich the bdd must be built,
///@param i vector of variables giving the order in which to calculate shannon expansion 
vertex_ptr robdd_build(expr_ptr f, int i, const std::vector<std::string>& ord);

///@brief: Takes in input an expression and first variable and return the 
///        pointer to the root of the robdd 
///        Works with complemented edges
///@param f is the expression on wich the bdd must be built,
///@param i vector of variables giving the order in which to calculate shannon expansion 
vertex_ptr robdd_build_comp(expr_ptr f, int i, const std::vector<std::string>& ord);

///@brief Given a vertex, it checks if it is present inside the unique table and returns it
///@param unique_table is a pointer to the unique table of vertices;
///@param root is the pointer to the root of the vertex to analyse, 
///@param lst left subtree.
///@param rst right subtree.       
std::optional<vertex_ptr> lookup(const std::vector<vertex_ptr>& unique_table, const std::string& root, vertex_ptr lst, vertex_ptr rst);

///@brief Given a vertex, it checks if it is present inside the unique table and returns it.
///       It workds with complemented edges.
///@param unique_table is a pointer to the unique table of vertices;
///@param root is the pointer to the root of the vertex to analyse, 
///@param lst left subtree.
///@param rst right subtree.       
std::optional<vertex_ptr> lookup_comp(const std::vector<vertex_ptr>& unique_table, const std::string& root, vertex_ptr lst, vertex_ptr rst, bool lcomp, bool rcomp);

///@brief Given two vertex pointers, it compares them to enstablish if they're equal.
///@param vertex1 pointer to a vertex structure;
///@param vertex2 pointer to a vertex structure;       
bool vertex_compare(vertex_ptr vertex1,vertex_ptr vertex2);

///@brief Given two vertex pointers, it compares them to enstablish if they're equal.
///@param vertex1 pointer to a vertex structure;
///@param vertex2 pointer to a vertex structure;       
bool vertex_compare_comp(vertex_ptr vertex1,vertex_ptr vertex2);

///@brief Given three functions it apply ite procedue to obtain a new function = f*g + !f*h.
///@param f pointer to a vertex structure that represent f;
///@param g pointer to a vertex structure that represent g;       
///@param h pointer to a vertex structure that represent h;
vertex_ptr apply_ite(vertex_ptr f, vertex_ptr g, vertex_ptr h, int i ,const std::vector<std::string>& ord);

///@brief Given three functions it apply ite procedue to obtain a new function = f*g + !f*h.
///       Works with complemented edges.
///@param f pointer to a vertex structure that represent f;
///@param g pointer to a vertex structure that represent g;       
///@param h pointer to a vertex structure that represent h;
vertex_ptr apply_ite_comp(vertex_ptr f, vertex_ptr g, vertex_ptr h, int i ,const std::vector<std::string>& ord);

/// @brief Prints the unique table 
/// @param unique_table 
void print_table( std::vector<vertex_ptr> unique_table);

/// @brief print the truth table for function f, given order of construction ord
///        (be careful to use same order used for bdd construction otherwise if will fail)
/// @param f function to evaluate 
/// @param ord order of variables for f 
void print_truth_table(vertex_ptr f, const std::vector<std::string>& ord);

/// @brief parse input file 
/// @param file file to be parsed
/// @param order order that will be returned to main file 
/// @param cntx context obj that stores functions a//in the future may become type jabddl::exprnd variables 
void parse_input(std::string file, std::vector<std::string> &order, jabddl::context & cntx);


} // namespace jabdd 