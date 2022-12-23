
#include <sstream>
#include <iostream>
#include <queue>
#include <cmath>
#include "../include/vertex.hpp"

int verbosity = 0;

void printBT(std::stringstream& stream ,const std::string& prefix, const vertex_ptr node, bool isLeft, bool cmp)
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
void vertex::print(const vertex_ptr vert){
    std::stringstream stream;
    printBT(stream,"", vert, false, false);
    if(verbosity) std::cout<<"The format of the print is:\n[root]\n" <<" +[left-child]\n" <<" L[right-child]\n";
    std::cout << stream.str() <<std::endl;
}

/// @brief constructor for non complemented bdds
/// @param root bdd root
/// @param l left child
/// @param r right child
vertex::vertex(const std::string& root, vertex_ptr l, vertex_ptr r) 
: root{root}, lsubtree{l}, rsubtree{r} { }

vertex::vertex(const std::string& name)
: root{{name}} {}

/// @brief Calculate vertex cofactor w.r.t. a variable
/// @param root vertex to be co-factorize
/// @param var variable 
/// @param value boolean value for the co-factorization
/// @return pointer to vertex cofactor
vertex_ptr vertex_cofactor(vertex_ptr root, const std::string& var, bool value){
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
        root->lsubtree = vertex_cofactor(root->lsubtree, var, value);
        root->rsubtree = vertex_cofactor(root->rsubtree, var, value);
        return root;
    }
}

/// @brief Support function for the truth table print
/// @param root vertex root
/// @param ord order in which we want to evaluate 
/// @param truthVector vector of bool values that is used to evaluate the variables 
/// @param i depth index, used to choose the correct value from truthVector
/// @return evaluation value for the function
bool evaluate_vertex(vertex_ptr root,const std::vector<std::string>& ord,std::vector<bool>& truthVector,int i){
    //no further evaluation is needed
    if(root->root == "v0")
        return false;
    else if( root->root == "v1") 
        return true;
    //We are matching the current variable to evaluate
    else if(root->root == ord[i])
    {    
        if(truthVector[i] == true)
           return evaluate_vertex(root->lsubtree,ord,truthVector,i+1);
        else return evaluate_vertex(root->rsubtree,ord,truthVector,i+1);
    }
    //root is not v0 or v1 but current order var does not match --> try next variable in order
    else
    { 
       return evaluate_vertex(root, ord, truthVector,i+1);
    }
}