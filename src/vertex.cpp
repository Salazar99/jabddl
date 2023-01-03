
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


vertex::vertex(const std::string& root, vertex_ptr l, vertex_ptr r) 
: root{root}, lsubtree{l}, rsubtree{r}, complemented_l{false}, complemented_r{false} {}

vertex::vertex(const std::string& name)
: root{{name}} {}

vertex::vertex(const std::string& root, vertex_ptr l, vertex_ptr r, bool lcomp, bool rcomp) 
: root{root}, lsubtree{l}, rsubtree{r} ,complemented_l{lcomp}, complemented_r{rcomp} {}

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

///
complemented_vertex vertex_cofactor_comp(complemented_vertex root, const std::string& var, bool value){
    complemented_vertex result;
    //if the variable to be eval is the root one, check if there is complementation
    if(root.root->root == var)
    {
        if(value){
            //if we have an odd number of complementations -> root is complemented, if also left is complemented we can get rid of it 
            if(root.root->complemented_l && result.complemented)
                result.complemented = false;
            //else if the root is not complemented but the left subtree is, we need to add complementation to the result
            else if(!result.complemented && root.root->complemented_l)
                result.complemented = true;
            result.root = root.root->lsubtree;
            return result;
        }else{
            //if we have an odd number of complementations -> root is complemented, if also left is complemented we can get rid of it 
            if(root.root->complemented_r && result.complemented)
                result.complemented = false;
            //else if the root is not complemented but the right subtree is, we need to add complementation to the result
            else if(!result.complemented && root.root->complemented_r)
                result.complemented = true;
            result.root = root.root->rsubtree;
            return result;
        }
    }
    //sono arrivato alla foglia 
    else if(root.root->root == "v1"){        
        return root; 
    }else
    {
        complemented_vertex left,right,l,r;
        //if subtree to explore is complemented, propagate it in l
        if(root.root->complemented_l)
            l.complemented = true;
        else
            l.complemented = false;
        l.root = root.root->lsubtree;
        left = vertex_cofactor_comp(l, var, value);

        //if subtree to explore is complemented, propagate it in r
        if(root.root->complemented_r)
            r.complemented = true;
        else  
            l.complemented = false;

        r.root = root.root->rsubtree;
        right = vertex_cofactor_comp(r, var, value);
        
        //create the new vertex
        result.root = root.root;
        result.root->lsubtree = left.root;
        result.root->rsubtree = right.root;
        //keep the complementation according to previous cases 
        result.root->complemented_l = left.complemented;
        result.root->complemented_r = right.complemented;    
        return result;
    }
}

/// @brief Support function for the truth table print
/// @param root vertex root
/// @param ord order in which we want to evaluate 
/// @param truthVector vector of bool values that is used to evaluate the variables 
/// @param i depth index, used to choose the correct value from truthVector
/// @return evaluation value for the function
bool evaluate_vertex_comp(vertex_ptr root,const std::vector<std::string>& ord,std::vector<bool>& truthVector,int i){
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