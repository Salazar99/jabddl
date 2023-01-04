
#include <sstream>
#include <iostream>
#include <queue>
#include <cmath> 
#include "../include/vertex.hpp"

int verbosity = 0;

/// @brief support function for pretty printing the bdd 
/// @param stream stream that contain the string to be printed
/// @param prefix string created until now
/// @param node pointer to node being manipulated at the moment
/// @param isLeft is the node a left one
/// @param cmp is the node complemented 
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

void vertex::print(const vertex_ptr vert){
    std::stringstream stream;
    printBT(stream,"", vert, false, false);
    if(verbosity) std::cout<<"The format of the print is:\n[root]\n" <<" +[left-child]\n" <<" L[right-child]\n";
    std::cout <<"\033[32m" << stream.str() << "\033[0m" <<std::endl;
}


vertex::vertex(const std::string& root, vertex_ptr l, vertex_ptr r) 
: root{root}, lsubtree{l}, rsubtree{r}, complemented_l{false}, complemented_r{false} {}

vertex::vertex(const std::string& name)
: root{{name}} {}

vertex::vertex(const std::string& root, vertex_ptr l, vertex_ptr r, bool lcomp, bool rcomp) 
: root{root}, lsubtree{l}, rsubtree{r} ,complemented_l{lcomp}, complemented_r{rcomp} {}

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


complemented_vertex vertex_cofactor_comp(complemented_vertex root, const std::string& var, bool value){
    complemented_vertex result;
    //if the variable to be eval is the root one, check if there is complementation
    if(root.root->root == var)
    {
        if(value){
            //if I'm evaluating the variable in root for positive value and left child is complemented, the result will be complemented
            result.complemented = root.root->complemented_l;
            result.root = root.root->lsubtree;
            return result;
        }else{
            //if I'm evaluating the variable in root for positive value and right child is complemented, the result will be complemented
            result.complemented = root.root->complemented_r;
            result.root = root.root->rsubtree;
            return result;
        }
    }
    //I arrived at a leaf for root, so return root with its complementation
    else if(root.root->root == "v1"){        
        return root; 
    }else
    {
        complemented_vertex left,right,l,r;
        //if subtree to explore is complemented, propagate it in l
        l.complemented = root.root->complemented_l;
        l.root = root.root->lsubtree;
        left = vertex_cofactor_comp(l, var, value);

        //if subtree to explore is complemented, propagate it in r
        r.complemented = root.root->complemented_r;
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