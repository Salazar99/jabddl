//represent an entry in the unique table of vertices
struct entry_table{
    char *variable;
    int high,low;
};

//unique table to represent the vertices of all robdds
struct entry_table *unique_table;

//represent an expression 
struct expr{

};

//structure to represent a vertex of the bdd
struct vertex{
    char* root;
    struct vertex* lsubtree,*rsubtree;
};

//@input: root is the pointer to the root of the vertex to analyse, 
//        lst and rst are respectively the left and right subtree.
//@output: pointer to a vertex representing the one given in input
//@brief: The function takes a vertex in input and return:
//          a new vertex if the one in input does not exists at the moment or
//          a pointer to the vertex if it is already present
struct vertex *old_or_new(char root, struct vertex *lst, struct vertex *rst);


//@input: "f" is the expression on wich the bdd must be built,
//        "i" represent the first variable to be used for shannon expression   
//@output: pointer to the vertex of the robbd
//@brief: Takes in input an expression and first variable and return the 
//        pointer to the root of the robdd
struct vertex *robdd_build(struct expr f, int i);