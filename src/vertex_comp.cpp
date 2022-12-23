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

/// @brief Calculate vertex cofactor w.r.t. a variable
/// @param root vertex to be co-factorize
/// @param var variable 
/// @param value boolean value for the co-factorization
/// @return pointer to vertex cofactor
vertexc_ptr vertex_cofactor_comp(vertexc_ptr root, const std::string& var, bool value){
    if(root->root == var)
    {
        if(value)
            return root->lsubtree;
        else 
            return root->rsubtree;
    }
    else if(root->root == "v0" || root->root == "v1")
        return root; 
    else
    {
        root->lsubtree = vertex_cofactor_comp(root->lsubtree, var, value);
        root->rsubtree = vertex_cofactor_comp(root->rsubtree, var, value);
        return root;
    }
}

/// @brief Support function for the truth table print
/// @param root vertex root
/// @param ord order in which we want to evaluate 
/// @param truthVector vector of bool values that is used to evaluate the variables 
/// @param i depth index, used to choose the correct value from truthVector
/// @return evaluation value for the function
bool evaluate_vertex_comp(vertexc_ptr root,const std::vector<std::string>& ord,std::vector<bool>& truthVector,int i){
    //no further evaluation is needed
    if(root->root == "v0")
        return false;
    else if( root->root == "v1") 
        return true;
    //We are matching the current variable to evaluate
    else if(root->root == ord[i])
    {    
        if(truthVector[i] == true)
           return evaluate_vertex_comp(root->lsubtree,ord,truthVector,i+1);
        else return evaluate_vertex_comp(root->rsubtree,ord,truthVector,i+1);
    }
    //root is not v0 or v1 but current order var does not match --> try next variable in order
    else
    { 
       return evaluate_vertex_comp(root, ord, truthVector,i+1);
    }
}