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


#pragma once

#include <memory>
#include <string>
#include <optional>
#include <fstream>
#include <regex>
#include <cassert>
#include <cmath>
#include "vertex.hpp"
#include "expr.hpp"
#include "vertex.hpp"

extern int verbosity;
extern int complemented_mode;

/// @brief In this implementetation the left child of the vertexes is considered to be the high branch of the tree with root in that vertex.
namespace jabddl {

/// @brief pointer to expression
using expr_ptr = expr*;
/// @brief pointer to vertex structure
using vertex_ptr = std::shared_ptr<vertex>;

/// @brief contains parsed informations about functions
struct fun{
    /// @brief name of the function
    std::string func_name;
    /// @brief to be printed or not
    bool tbp = false;
    /// @brief if part for ite operator
    jabddl::expr_ptr ite_if;
    /// @brief then part for ite operator
    jabddl::expr_ptr ite_then;
    /// @brief else part for ite operator
    jabddl::expr_ptr ite_else;
};

/// @brief object that contains variables and functions for manipulation purposes
struct context{
    /// @brief vector of variables
    std::vector<std::string> vars;
    /// @brief map of declared functions
    std::unordered_map<std::string, jabddl::fun> funcs;
    /// @brief map to store bdd corresponding to functions
    std::unordered_map<std::string, jabddl::vertex_ptr> root_vertexes;
};

/// @brief 0 leaf for unique table
static inline vertex_ptr v0 = std::make_shared<vertex>("v0");
/// @brief 1 leaf for unique table
static inline vertex_ptr v1 = std::make_shared<vertex>("v1");

/// @brief unique table to collect the vertices of all robdds
extern std::vector<vertex_ptr> unique_table;

/// @brief Create and add to unique_table the 0 and 1 leaves
void initialize();

/// @brief The function takes a vertex in input and return: a new vertex if the one in input does not exists at the moment or a pointer to the vertex if it is already present
/// @param root is the pointer to the root of the vertex to analyse, 
/// @param lst left subtree.
/// @param rst right subtree. 
/// @return vertex pointer representing root,lst,rst
vertex_ptr old_or_new(const std::string& root, vertex_ptr lst, vertex_ptr rst);

/// @brief The function takes a vertex in input and return: a new vertex if the one in input does not exists at the moment or a pointer to the vertex if it is already present. It works with complemented edges.
/// @param root is the pointer to the root of the vertex to analyse
/// @return vertex pointer representing root,lst,rst
vertex_ptr old_or_new_comp(vertex_ptr root);

/// @brief: Takes in input an expression and first variable and return the pointer to the root of the robdd
/// @param f is the expression on wich the bdd must be built,
/// @param i vector of variables giving the order in which to calculate shannon expansion 
/// @param ord vector of variable names
/// @return pointer to root of the bdd
vertex_ptr robdd_build(expr_ptr f, int i, const std::vector<std::string>& ord);

/// @brief: Takes in input an expression and first variable and return the pointer to the root of the robdd. Works with complemented edges
/// @param f is the expression on wich the bdd must be built,
/// @param i vector of variables giving the order in which to calculate shannon expansion 
/// @param ord vector of variable names
/// @return pointer to root of the bdd
vertex_ptr robdd_build_comp(expr_ptr f, int i, const std::vector<std::string>& ord);

/// @brief Given a vertex, it checks if it is present inside the unique table and returns it. It is a support function for old_or_new
/// @param unique_table is a pointer to the unique table of vertices;
/// @param root is the pointer to the root of the vertex to analyse, 
/// @param lst left subtree.
/// @param rst right subtree.
/// @return pointer to vertex if found, nothing otherwise
std::optional<vertex_ptr> lookup(const std::vector<vertex_ptr>& unique_table, const std::string& root, vertex_ptr lst, vertex_ptr rst);
 
/// @brief Check for the presence of a vertex in the unique table and eventually returns it
/// @param unique_table table of vertexes
/// @param root vertex root
/// @param lst vertex left subtree
/// @param rst vetex right subtree
/// @param lcomp is l subtree complemented?
/// @param rcomp is r subtree complemented?
/// @return pointer to vertex if found, nothing otherwise  
std::optional<vertex_ptr> lookup_comp(const std::vector<vertex_ptr>& unique_table, const std::string& root, vertex_ptr lst, vertex_ptr rst, bool lcomp, bool rcomp);

/// @brief Given two vertex pointers, it compares them to enstablish if they're equal.
/// @param vertex1 pointer to a vertex structure;
/// @param vertex2 pointer to a vertex structure;
/// @return true if equal, false otherwise       
bool vertex_compare(vertex_ptr vertex1,vertex_ptr vertex2);

/// @brief Given two vertex pointers complemented, it compares them to enstablish if they're equal.
/// @param vertex1 pointer to a vertex structure;
/// @param vertex2 pointer to a vertex structure;       
/// @return true if equal, false otherwise
bool vertex_compare_comp(complemented_vertex vertex1,complemented_vertex vertex2);

/// @brief Given three functions it apply ite procedue to obtain a new function = f*g + !f*h.
/// @param f pointer to a vertex structure that represent f;
/// @param g pointer to a vertex structure that represent g;       
/// @param h pointer to a vertex structure that represent h;
/// @param i counter for level of recursion
/// @param ord vector for variable names
/// @return pointer to root vertex for ite procedure
vertex_ptr apply_ite(vertex_ptr f, vertex_ptr g, vertex_ptr h, int i ,const std::vector<std::string>& ord);

/// @brief Given three functions it apply ite procedue to obtain a new function = f*g + !f*h. Works with complemented edges.
/// @param f pointer to a vertex structure that represent f;
/// @param g pointer to a vertex structure that represent g;       
/// @param h pointer to a vertex structure that represent h;
/// @param i counter for level of recursion
/// @param ord vector for variable names
/// @return pointer to root vertex for ite procedure
vertex_ptr apply_ite_comp(vertex_ptr f, vertex_ptr g, vertex_ptr h, int i ,const std::vector<std::string>& ord);

/// @brief Prints the unique table 
/// @param unique_table unique table of bdd nodes
void print_table( std::vector<vertex_ptr> unique_table);

/// @brief print the truth table for function f, given order of construction ord (be careful to use same order used for bdd construction otherwise if will fail)
/// @param f function to evaluate 
/// @param ord order of variables for f 
void print_truth_table(vertex_ptr f, const std::vector<std::string>& ord);

/// @brief Function to parse input file
/// @param file Filename
/// @param cntx context variable
void parse_input(std::string file, jabddl::context & cntx);


} // namespace jabdd 