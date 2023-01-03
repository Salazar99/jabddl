#pragma once

#include<string>
#include<vector>
#include <memory>
struct vertex_comp;
using vertexc_ptr = std::shared_ptr<vertex_comp>;

//structure to represent a vertex of the bdd
struct vertex_comp {
    std::string root;
    vertexc_ptr lsubtree, rsubtree;
    bool complemented_l,complemented_r;

    /// @brief Constructs a new vertex for complemented edges 
    /// @param root variable of the vertex
    /// @param l left subtree
    /// @param r right subtree
    /// @param complemented_l true if left child is complemented, false otherwise
    /// @param complemented_r true if right child is complemented, false otherwise
    vertex_comp(const std::string& root, vertexc_ptr l, vertexc_ptr r, bool complemented_l, bool complemented_r);

    /// @brief constructor for vertex without childs, used for leaves declaration
    /// @param name variable name
    explicit vertex_comp(const std::string& name);

    /// @brief print the bdd with vert as root in a visually friendly form 
    /// @param vert root of the bdd
    static void print(const vertexc_ptr vert);
};
