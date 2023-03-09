
#include "../include/dot.hpp"
#include <iostream>
#include <sstream>

//to print graph once obtained .dot : dot -Tpng filename.dot -o outfile.ps

namespace jabddl{
       
    void tree_dump_dot(std::ofstream& file, vertex_ptr root){    
        if(root->root == "v1"){}
        else{
                //write on dot file for left child, if complemented edge is dotted
                file <<"\t" << root->root << " -> " <<root->lsubtree->root;
                if(root->complemented_l)
                    file << "[style = dotted];\n";
                else
                    file <<";" << std::endl;
                tree_dump_dot(file, root->lsubtree);

                //write on dot file for right child, if complemented, edge is red and dotted, else is dashed
                file <<"\t" << root->root << "-> " <<root->rsubtree->root;
                if(root->complemented_r)
                    file << "[style = dotted];\n" <<"\tedge [color = red];\n";
                else{
                    file << "[style = dashed];\n";
                }
                tree_dump_dot(file, root->rsubtree);
            }
    }



    void dump_dot(complemented_vertex root, const char* filename) {

        std::ofstream file;
        file.open(filename, std::ios::trunc);

        file <<  "digraph G {\n\tinput [shape=box];\n";
        file << "\tinput -> " << root.root->root;
        if(root.complemented)
            file << "[style = dotted];\n";
        else 
            file <<";" <<std::endl;

        tree_dump_dot(file,root.root);

        file << "}";
        file.close();
    }
 
}//namespace jabddl
