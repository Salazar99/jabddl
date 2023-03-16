
#include "../include/dot.hpp"
#include <iostream>
#include <sstream>

//to print graph once obtained .dot : dot -Tpng filename.dot -o outfile.png

namespace jabddl{
    const int mask = 0xFFFFFF;   

    void tree_dump_dot(std::ofstream& file, vertex_ptr root, std::vector<std::uintptr_t> &visited){    
        file <<"\"" << (void *)(((std::uintptr_t)root.get())) << "\"" << "[label =" << root->root << "];\n" ; 
        visited.push_back((std::uintptr_t)root.get());

        if(root->root == "v1"){}
        else{
                //write on dot file for left child, if complemented edge is dotted
                file <<"\t" <<"\"" << (void *)(((std::uintptr_t)root.get())) << "\"" " -> " << "\"" << (void *)(((std::uintptr_t)root->lsubtree.get())) <<"\"";
                //file << "[label = \"" <<  (void *)(((std::uintptr_t)root->lsubtree.get()) & jabddl::mask) << "\"]";
              
                if(root->complemented_l)
                    file << "[style = dotted];\n";
                else
                    file <<";" << std::endl;  
               
                if(std::find(visited.begin(), visited.end(), ((std::uintptr_t)root->lsubtree.get())) == visited.end())
                    tree_dump_dot(file, root->lsubtree,visited);

                //write on dot file for right child, if complemented, edge is red and dotted, else is dashed
                file <<"\t" <<"\"" << (void *)(((std::uintptr_t)root.get())) << "\"" "-> " << "\"" << (void *)(((std::uintptr_t)root->rsubtree.get())) << "\"";
                //file << "[label = \"" << (void *)(((std::uintptr_t)root->rsubtree.get()) & jabddl::mask) << "\"]";
                if(root->complemented_r)
                    file << "[style = dotted];\n" <<"\tedge [color = red];\n";
                else{
                    file << "[style = dashed];\n";
                }

                if(std::find(visited.begin(), visited.end(), ((std::uintptr_t)root->rsubtree.get())) == visited.end())
                    tree_dump_dot(file, root->rsubtree,visited);
            }
    }



    void dump_dot(complemented_vertex root, const char* filename) {

        std::ofstream file;
        file.open(filename, std::ios::trunc);
        //Store already encountered node to prevent adding them twice
        std::vector<std::uintptr_t> visited;

        file <<  "digraph G {\n\tinput [shape=box];\n";
        file << "\tinput -> " <<"\"" << (void *)(((std::uintptr_t)root.root.get())) << "\"";
        visited.push_back((((std::uintptr_t)root.root.get())));
        //file << "[label = \"" << (void *)(((std::uintptr_t)root.root.get()) & jabddl::mask) << "\"]";
        if(root.complemented)
            file << "[style = dotted];\n";
        else 
            file <<";" <<std::endl;

        file <<"\"" << (void *)(((std::uintptr_t)root.root.get())) << "\"" << "[label =" << root.root->root << "];\n";

        tree_dump_dot(file,root.root,visited);

        file << "}";
        file.close();
    }
 
}//namespace jabddl
