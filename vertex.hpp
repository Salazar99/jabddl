#include <vector>
#include <memory>
#include <string>
#include <optional>

using vertex_ptr =std::shared_ptr<vertex> ;

//structure to represent a vertex of the bdd
class vertex {
    public:
        std::string root;
        vertex_ptr lsubtree, rsubtree;
        bool complemented_l,complemented_r;
    
        /// @brief Constructs a new vertex
        /// @param root variable of the vertex
        /// @param l left subtree
        /// @param r right subtree
        vertex(const std::string& root, vertex_ptr l, vertex_ptr r);

        /// @brief Constructs a new vertex using complemented edges
        /// @param root variable of the vertex
        /// @param l left subtree
        /// @param r right subtree
        vertex(const std::string& root, vertex_ptr l, vertex_ptr r, bool complemented_l, bool complemented_r);


        /// @brief constuctor for leaf vertexes
        /// @param name either "v0" or "v1" 
        explicit vertex(const std::string& name);

        /// @brief print the bdd which have vert as root
        /// @param vert root of the bdd 
        static void print(const vertex_ptr vert);

};

#include "vertex.i.hpp"