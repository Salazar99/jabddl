
#pragma once

#include <vector>
#include <memory>
#include <string>
#include <optional>
#include<iostream>
#include <sstream>
#include <cassert>

#include "vertex.hpp"

/// @brief Support function for printing in a meaningfull way vertexes
/// @param stream 
/// @param vert 
/// @param depth 
static void print_vert_rec(std::stringstream& stream, vertex_ptr vert, int depth){
     ++depth;
    if(vert->root == "v0" || vert->root == "v1"){
        stream << "\n";
        for(int i = 1; i < depth-1; i++) stream << " |";
        if(depth >1) stream << " L";
        stream << "[" << vert->root << "]";
    }else{
       
        stream << "\n";
        for(int i = 1; i < depth-1; i++) stream << " |";
        if(depth >1) stream << " +";
        stream << "[" << vert->root << "]";
        if(vert->complemented_l)
            stream << "!";
        print_vert_rec(stream, vert->lsubtree,depth );
        if(vert->complemented_r)
            stream << "!";
        print_vert_rec(stream,vert->rsubtree, depth);
       
    }
   
}

void vertex::print(const vertex_ptr vert){
    std::stringstream stream;
    print_vert_rec(stream, vert, 0);
    std::cout << stream.str() <<std::endl;
}

//constructor for normal vertexes
vertex::vertex(const std::string& root, vertex_ptr l, vertex_ptr r) 
: root{root}, lsubtree{l}, rsubtree{r}, complemented_l{false}, complemented_r{false} {}

//constructor for complemented edges vertexes
vertex::vertex(const std::string& root, vertex_ptr l, vertex_ptr r, bool complemented_l, bool complemented_r)
: root{root}, lsubtree{l}, rsubtree{r}, complemented_l{complemented_l}, complemented_r{complemented_r} {}

vertex::vertex(const std::string& name)
: root{{name}} {}
