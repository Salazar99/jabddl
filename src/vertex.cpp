
#include <sstream>
#include <iostream>
#include "../include/vertex.hpp"

int verbosity = 0;

/// @brief support function to print tree starting from root
/// @param stream stringstream to print
/// @param vert root
/// @param depth depth of the recursion at the moment (used for formatting)
static void print_vert_rec(std::stringstream& stream, vertex_ptr vert, int depth){
     ++depth;

    if(vert->root == "v0" || vert->root == "v1")
    {
        stream << "\n";
        for(int i = 1; i < depth-1; i++) stream << " |";
        if(depth >1) stream << " L";
        stream << "[" << vert->root << "]";
    }
    else
    {
       
        stream << "\n";
        for(int i = 1; i < depth-1; i++) stream << " |";
        if(depth >1) stream << " +";
        stream << "[" << vert->root << "]";
        print_vert_rec(stream, vert->lsubtree,depth );
        print_vert_rec(stream,vert->rsubtree, depth);
       
    }
   
}

/// @brief Print bdd 
/// @param vert root of the bdd
void vertex::print(const vertex_ptr vert){
    std::stringstream stream;
    print_vert_rec(stream, vert, 0);
    if(verbosity) std::cout<<"The format of the print is:\n[root]\n" <<" +[left-child]\n" <<" L[right-child]\n";
    std::cout << stream.str() <<std::endl;
}

/// @brief constructor for non complemented bdds
/// @param root bdd root
/// @param l left child
/// @param r right child
vertex::vertex(const std::string& root, vertex_ptr l, vertex_ptr r) 
: root{root}, lsubtree{l}, rsubtree{r} { }

/// @brief constructor for complemented edges bdds
/// @param root bdd root
/// @param l left child
/// @param r right child
vertex::vertex(const std::string& root, vertex_ptr l, vertex_ptr r, bool complemented_l, bool complemented_r)
: root{root}, lsubtree{l}, rsubtree{r}, complemented_l{complemented_l}, complemented_r{complemented_r} {}

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