#include "jabbdl.hpp"

struct vertex *old_or_new(char root, struct vertex *lst, struct vertex *rst){
    vertex * v;
    if((v = lookup(unique_table, root, lst, rst)) != nullptr){
        return v;
    }
    else{
        /*create a new vertex*/
        return v;
    }
}

struct vertex *robdd_build(struct expr f, int i){


}