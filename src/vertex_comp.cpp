#include"../include/vertex_comp.hpp"
#include <sstream>
#include <iostream>
#include <queue>
#include <cmath>

int verbosity = 0;

vertex_comp::vertex_comp(const std::string& name)
: root{{name}} {}

void printBT(std::stringstream& stream ,const std::string& prefix, const vertexc_ptr node, bool isLeft, bool cmp)
{
    if (node != nullptr)
    {
        stream << prefix;
        stream << (isLeft ? "+--" : "┗--");
        // print the value of the node
        if(cmp) stream << "!";
        stream << "[" <<node->root <<"]" << std::endl;
        // enter the next tree level - left and right branch
        printBT(stream, prefix + (isLeft ? "|   " : "    "), node->lsubtree, true, node->complemented_l);
        printBT(stream, prefix + (isLeft ? "|   " : "    "), node->rsubtree, false, node->complemented_r);
    }
}

/// @brief Print bdd 
/// @param vert root of the bdd
void vertex_comp::print(const vertexc_ptr vert){
    std::stringstream stream;
    printBT(stream,"", vert, false, false);
    if(verbosity) std::cout<<"The format of the print is:\n[root]\n" <<" +[left-child]\n" <<" L[right-child]\n";
    std::cout << stream.str() <<std::endl;
}
