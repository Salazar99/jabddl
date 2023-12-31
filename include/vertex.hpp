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

#include<vector>
#include <memory>
struct vertex;
using vertex_ptr = std::shared_ptr<vertex>;

/// @brief structure to represent a vertex of the bdd
struct vertex {
    /// @brief node variable
    std::string root;
    /// @brief left child of the node
    vertex_ptr lsubtree;
    /// @brief right child of the node
    vertex_ptr rsubtree;
    /// @brief boolean variables that define if the left child is complemented
    bool complemented_l;
    /// @brief boolean variables that define if the right child is complemented
    bool complemented_r;

    /// @brief Constructs a new vertex
    /// @param root variable of the vertex
    /// @param l left subtree
    /// @param r right subtree
    vertex(const std::string& root, vertex_ptr l, vertex_ptr r);
    
    /// @brief Constructs a new vertex
    /// @param root variable of the vertex
    /// @param l left subtree
    /// @param r right subtree
    /// @param lcomp left child complemented
    /// @param rcomp right child complemented
    vertex(const std::string& root, vertex_ptr l, vertex_ptr r,bool lcomp, bool rcomp);

    /// @brief constructor for vertex without childs, used for leaves declaration
    /// @param name variable name
    explicit vertex(const std::string& name);

    /// @brief print the bdd with vert as root in a visually friendly format 
    /// @param vert root of the bdd
    static void print(const vertex_ptr vert);
};

/// @brief Struct that is used as return value for complemented edges functions
struct complemented_vertex{
    /// @brief is root complemented?
    bool complemented = false;
    /// @brief vertex to which we want to keep track 
    vertex_ptr root;
};

/// @brief Calculate vertex cofactor w.r.t. a variable
/// @param root vertex to be co-factorize
/// @param var variable 
/// @param value boolean value for the co-factorization
/// @return pointer to vertex cofactor
vertex_ptr vertex_cofactor(vertex_ptr root, const std::string& var, bool value);

/// @brief Support function for the truth table print
/// @param root vertex root
/// @param ord order in which we want to evaluate 
/// @param truthVector vector of bool values that is used to evaluate the variables 
/// @param i depth index, used to choose the correct value from truthVector
/// @return evaluation value for the function
bool evaluate_vertex(vertex_ptr root,const std::vector<std::string>& ord,std::vector<bool>& truthVector,int i);

/// @brief Calculate vertex cofactor w.r.t. a variable, for complemented edges bdds
/// @param root vertex structure to be co-factorize
/// @param var variable 
/// @param value boolean value for the co-factorization
/// @return structure with cofactorized function and if it is complemented or not
complemented_vertex vertex_cofactor_comp(complemented_vertex root, const std::string& var, bool value);

/// @brief Support function for the truth table print
/// @param root vertex root
/// @param ord order in which we want to evaluate 
/// @param truthVector vector of bool values that is used to evaluate the variables 
/// @param i depth index, used to choose the correct value from truthVector
/// @return evaluation value for the function
bool evaluate_vertex_comp(complemented_vertex root,const std::vector<std::string>& ord,std::vector<bool>& truthVector,int i);