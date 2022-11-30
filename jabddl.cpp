#include "jabddl.hpp"

namespace jabddl {

//unique table to represent the vertices of all robdds
std::vector<vertex_ptr> unique_table;

void initialize() {
    unique_table.push_back(v0);
    unique_table.push_back(v1);
}

//0 and 1 variables used to valuate expressions
auto v0_v = std::string("v0");
auto v1_v = std::string("v1");

vertex::vertex(const std::string& name)
: root{{name}} {}

expr::expr() : type{expr_type::Var} { }
expr::~expr() {
    if (type == expr_type::Var)
        delete var.name;
}

//* == and, + == or 
expr_ptr expr::make_add(expr_ptr a, expr_ptr b) {
    auto res = new expr{};
    res->type = expr_type::Add;
    res->args.l = a;
    res->args.r = b;
    return res;
}
expr_ptr expr::make_mul(expr_ptr a, expr_ptr b) {
    auto res = new expr{};
    res->type = expr_type::Mul;
    res->args.l = a;
    res->args.r = b;
    return res;
}
expr_ptr expr::make_neg(expr_ptr n) {
    auto res = new expr{};
    res->type = expr_type::Not;
    res->arg.c = n;
    return res;
}
expr_ptr expr::make_var(const std::string& var) {
    auto res = new expr{};
    res->var.name = new std::string(var);
    return res;
};

expr_ptr ite(expr_ptr a, expr_ptr b, expr_ptr c) {

    // If a then b (a*b)
    auto choise1 = expr::make_mul(a, b);
    
    // if not a then c  (!a*c)
    auto neg_a = expr::make_neg(a);
    auto choise2 = expr::make_mul(neg_a, c);

    // x1 is 1 or 0 (choice1 + choice2)
    return expr::make_add(choise1, choise2);
}

static void print_rec(std::stringstream& stream, const expr_ptr expr) {
    switch (expr->type) {
        case expr_type::Add: {
            stream << "(";
            print_rec(stream, expr->args.l);
            stream << " + ";
            print_rec(stream, expr->args.r);
            stream << ")";
        } break;
        case expr_type::Mul: {
            stream << "(";
            print_rec(stream, expr->args.l);
            stream << " * ";
            print_rec(stream, expr->args.r);
            stream << ")";
        } break;
        case expr_type::Not: {
            stream << "!";
            print_rec(stream, expr->arg.c);
        } break;
        case expr_type::Var: {
            stream << *expr->var.name;
        } break;
    }
}

void expr::print(const expr_ptr expr) {
    std::stringstream stream;
    print_rec(stream, expr);
    std::cout << stream.str() << std::endl;
}

/// @brief support function to print tree starting from root, for ease of print, the first that appears below a node.
/// @param stream stringstream to print
/// @param vert root
/// @param depth depth of the recursion at the moment (used for formatting)
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
        print_vert_rec(stream, vert->lsubtree,depth );
        print_vert_rec(stream,vert->rsubtree, depth);
       
    }
   
}


void vertex::print(const vertex_ptr vert){
    std::stringstream stream;
    print_vert_rec(stream, vert, 0);
    if(VERBOSE) std::cout<<"The format of the print is:\n[root]\n" <<" +[left-child]\n" <<" L[right-child]\n";
    std::cout << stream.str() <<std::endl;
}

//constructor for normal vertexes
vertex::vertex(const std::string& root, vertex_ptr l, vertex_ptr r) 
: root{root}, lsubtree{l}, rsubtree{r} { }

//constructor for complemented edges vertexes
vertex::vertex(const std::string& root, vertex_ptr l, vertex_ptr r, bool complemented_l, bool complemented_r)
: root{root}, lsubtree{l}, rsubtree{r}, complemented_l{complemented_l}, complemented_r{complemented_r} {}

vertex_ptr old_or_new(const std::string& root, vertex_ptr lst, vertex_ptr rst){
    
    vertex_ptr v;
    auto result = lookup(unique_table, root, lst, rst);
    if (result.has_value())
        return *result;
    else{
        /*create a new vertex*/
        auto l_tree = lookup(unique_table,lst->root,lst->lsubtree,lst->rsubtree);
        auto r_tree = lookup(unique_table,rst->root,rst->lsubtree,rst->rsubtree);
        if(l_tree.has_value())
            lst = l_tree.value(); 
        if(r_tree.has_value())
            rst = r_tree.value();   
        v = std::make_shared<vertex>(root, lst, rst);
        //adds it to the unique table
        unique_table.push_back(v);
        return v;
    }
}

vertex_ptr old_or_new_comp(const std::string& root, vertex_ptr lst, vertex_ptr rst,bool l_comp, bool r_comp){
    
    vertex_ptr v;
    auto result = lookup_comp(unique_table, root, lst, rst, l_comp,r_comp);
    if (result.has_value())
        return *result;
    else{
        /*create a new vertex*/
        auto l_tree = lookup_comp(unique_table,lst->root,lst->lsubtree,lst->rsubtree,l_comp,r_comp);
        auto r_tree = lookup_comp(unique_table,rst->root,rst->lsubtree,rst->rsubtree,l_comp,r_comp);
        if(l_tree.has_value())
            lst = l_tree.value(); 
        if(r_tree.has_value())
            rst = r_tree.value();   
        v = std::make_shared<vertex>(root, lst, rst, l_comp, r_comp);
        //adds it to the unique table
        unique_table.push_back(v);
        return v;
    }
}

expr_ptr evaluate(expr_ptr root, const std::string& var, bool value) {
    if(root->type == jabddl::expr_type::Var && *root->var.name == var){
              if(value)
                root->var.name = new std::string(v1_v);
              else
                root->var.name = new std::string(v0_v);
              return root;
    }
    
    switch(root->type){
        case jabddl::expr_type::Mul: {
            root->args.l = evaluate(root->args.l, var, value);
            root->args.r = evaluate(root->args.r, var, value);

            //0 * 0 = 0
            if(*root->args.l->var.name == v0_v || *root->args.r->var.name == v0_v)
                return expr::make_var(v0_v);

            // 1 * 1 = 1
            if(*root->args.l->var.name == v1_v && *root->args.r->var.name == v1_v)
                return  expr::make_var(v1_v);

            // 1 * x = x
            if (*root->args.l->var.name == v1_v)
                return root->args.r;

            // x * 1 = x
            if (*root->args.r->var.name == v1_v)
                return root->args.l;

            //x * 0 = 0
            if(*root->args.l->var.name == v0_v)
                return expr::make_var(v0_v);

            if(*root->args.r->var.name == v0_v)
                return expr::make_var(v0_v);

            
            return root;
        } break;
        case jabddl::expr_type::Add:{
            root->args.l = evaluate(root->args.l, var, value);
            root->args.r = evaluate(root->args.r, var, value);
            
            //0 + 0 = 0
            if(*root->args.l->var.name == v0_v && *root->args.r->var.name == v0_v)
                return expr::make_var(v0_v);
            //1 + 1 = 1
            if(*root->args.l->var.name == v1_v && *root->args.r->var.name == v1_v)
                return expr::make_var(v1_v);    

            //0 + x = x
            if(*root->args.l->var.name == v0_v)
                return root->args.r;

             if(*root->args.r->var.name == v0_v)
                return root->args.l;

            //1 + x = 1 
            if(*root->args.l->var.name == v1_v)
                return expr::make_var(v1_v);    

            if(*root->args.r->var.name == v1_v)
                return expr::make_var(v1_v);              

            return root;
        }    break;

        case jabddl::expr_type::Not:{
            auto res = evaluate(root->arg.c, var, value);
            if(res->type == jabddl::expr_type::Var){
                if(*res->var.name == v0_v){
                    return expr::make_var(v1_v);
                }    
                else if(*res->var.name == v1_v){
                    return expr::make_var(v0_v);
                }  
            }
            return root;
        }break;

        case jabddl::expr_type::Var:{
            return root;
        }break;

        default:{
            break;
            return root;
        }
    } 
    exit(-1);
}

expr_ptr copy_expr_rec(const expr_ptr root) {
    
    // Copied expr
    expr_ptr result = new expr{};
    result->type = root->type;

    // Copy childrens
    switch (result->type) {
        case expr_type::Add:
        case expr_type::Mul: {

            result->args.l = copy_expr_rec(root->args.l);
            result->args.r = copy_expr_rec(root->args.r);
        } break;
        case expr_type::Not: {
            result->arg.c = copy_expr_rec(root->arg.c);
        } break;
        case expr_type::Var: {
            result->var.name = new std::string(*root->var.name);
        } break;
    }

    return result;
}

bool vertex_compare(vertex_ptr vertex1,vertex_ptr vertex2){
    bool result = false;
    if((vertex1->root == "v0" && vertex2->root == "v0") || (vertex1->root == "v1" && vertex2->root == "v1")){
        return true;
    }
    else if((vertex1->root == "v1" && vertex2->root == "v0") || (vertex1->root == "v0" && vertex2->root == "v1")){
        return false;
    }
    else if(vertex1->root != "v0" && vertex2->root != "v1" && vertex1->root != "v1" && vertex2->root != "v0"){
        result = vertex_compare(vertex1->lsubtree,vertex2->lsubtree) && vertex_compare(vertex1->rsubtree,vertex2->rsubtree) && (vertex1->root == vertex2->root);
    }
    return result;
}

vertex_ptr robdd_build(expr_ptr f, int i, const std::vector<std::string>& ord) {
     vertex_ptr l,r;
     std::string root;

    if(f->type == jabddl::expr_type::Var && (*f->var.name == v0_v || *f->var.name == v1_v)){
        if(*f->var.name == v0_v)
            return v0;
        if(*f->var.name == v1_v)
            return v1; 
    }
    else{

        //assert(i < ord.size());
        root = ord[i];
        
        auto l_copy = std::unique_ptr<expr>(copy_expr_rec(f));
        l = robdd_build(evaluate(l_copy.get(), ord[i], true), i+1, ord);
        //std::cout<< "f valutata per: " << ord[i].name <<" positivo\n";
        //expr::print(l_copy);
        //delete_expr(l_copy);

        auto r_copy = std::unique_ptr<expr>(copy_expr_rec(f));
        r = robdd_build(evaluate(r_copy.get(), ord[i], false), i+1, ord);
        //std::cout<< "f valutata per: " << ord[i].name <<" positivo\n";
        //expr::print(r_copy);
        //delete_expr(r_copy);
    
        if(vertex_compare(l,r)/*l->lsubtree == r->lsubtree && l->rsubtree == r->rsubtree*/)
            return l;
        else return old_or_new(root,l,r);
        
    }

    exit(-1);
}

bool vertex_compare_comp(vertex_ptr vertex1,vertex_ptr vertex2){
    bool result = false;
    //Base case
    if(vertex1->root == "v1" && vertex2->root == "v1"){
        return true;
    }
    //roots have same name but one of the two is complemented and the other no
    else if((vertex1->root == vertex2->root && (!(vertex1->complemented_l && vertex2->complemented_l) || !(vertex1->complemented_r && vertex2->complemented_r))) || (vertex1->root != vertex2->root)){
        return false;
    }
    //same name and same complementation, need to check for subtrees
    else{
        result = vertex_compare(vertex1->lsubtree,vertex2->lsubtree) && vertex_compare(vertex1->rsubtree,vertex2->rsubtree) && (vertex1->root == vertex2->root);
    }
    return result;
}


vertex_ptr robdd_build_comp(expr_ptr f, int i, const std::vector<std::string>& ord) {
     vertex_ptr l,r;
     std::string root;
     bool lcomp = false;
     bool rcomp = false;

    if(f->type == jabddl::expr_type::Var && (*f->var.name == v0_v || *f->var.name == v1_v)){
        if(*f->var.name == v0_v)
            return v0;
        if(*f->var.name == v1_v)
            return v1; 
    }
    else{

        //assert(i < ord.size());
        root = ord[i];
        
        auto l_copy = std::unique_ptr<expr>(copy_expr_rec(f));
        l = robdd_build_comp(evaluate(l_copy.get(), ord[i], true), i+1, ord);
      
        //if l points to v0, it is replaced by v1 and marked as complemented
        if(l->root == v0->root){
            lcomp = true;
            l = v1;
        }
        //std::cout<< "f valutata per: " << ord[i].name <<" positivo\n";
        //expr::print(l_copy);
        //delete_expr(l_copy);

        auto r_copy = std::unique_ptr<expr>(copy_expr_rec(f));
        r = robdd_build_comp(evaluate(r_copy.get(), ord[i], false), i+1, ord);
       
        //if r points to v0, it is replaced by v1 and marked as complemented
        if(r->root == v0->root){
            rcomp = true;
            r = v1;
        }
        //std::cout<< "f valutata per: " << ord[i].name <<" positivo\n";
        //expr::print(r_copy);
        //delete_expr(r_copy);

        if(vertex_compare_comp(l,r) && (lcomp == rcomp)/*l->lsubtree == r->lsubtree && l->rsubtree == r->rsubtree*/)
            return l;
        return old_or_new_comp(root,l,r,lcomp,rcomp);
    }

    exit(-1);
}

vertex_ptr vertex_cofactor(vertex_ptr root, const std::string& var, bool value){
    if(root->root == var){
        if(value)
            return root->lsubtree;
        else 
            return root->rsubtree;
    }else if(root->root == "v0" || root->root == "v1")
        return root; 
    else{
        root->lsubtree = vertex_cofactor(root->lsubtree, var, value);
        root->rsubtree = vertex_cofactor(root->rsubtree, var, value);
        return root;
    }
}

vertex_ptr apply_ite(vertex_ptr f, vertex_ptr g, vertex_ptr h, int i ,const std::vector<std::string>& ord){
     vertex_ptr l,r;
     std::string root;

    if(f->root  == "v1" )
        return g;
    if(f->root == "v0" )
        return h;
    else if (g->root == "v1" && h->root == "v0")
        return f;
    else{
        root = ord[i];
        l = apply_ite(vertex_cofactor(f,ord[i],true),vertex_cofactor(g,ord[i],true),vertex_cofactor(h,ord[i],true),i+1,ord);
        r = apply_ite(vertex_cofactor(f,ord[i],false),vertex_cofactor(g,ord[i],false),vertex_cofactor(h,ord[i],false),i+1,ord);
        
        if(vertex_compare(l,r))
            return l;
        else 
            return old_or_new(root,l,r);
    }
}

std::optional<vertex_ptr> lookup(const std::vector<vertex_ptr>& unique_table, const std::string& root, vertex_ptr lst, vertex_ptr rst){
    for (auto& vertex : unique_table) {
        if (vertex->root == root && vertex->lsubtree == lst && vertex->rsubtree == rst)
            return std::make_optional<vertex_ptr>(vertex);
    }
    return std::nullopt;
}
 
std::optional<vertex_ptr> lookup_comp(const std::vector<vertex_ptr>& unique_table, const std::string& root, vertex_ptr lst, vertex_ptr rst, bool lcomp, bool rcomp){
    for (auto& vertex : unique_table) {
        if (vertex->root == root && vertex->lsubtree == lst && vertex->rsubtree == rst && vertex->complemented_l == lcomp && vertex->complemented_r == rcomp)
            return std::make_optional<vertex_ptr>(vertex);
    }
    return std::nullopt;
}

void print_table(std::vector<vertex_ptr> unique_table){
    
    int i = 0;
    for(const auto& v : unique_table){
        
        char lbuf[16], rbuf[16];
        if (v->root != "v1" && v->root != "v0") {
            snprintf(lbuf, 16, "%c%p", char((v->complemented_l) ? '!' : ' '), v->lsubtree.get());
            snprintf(rbuf, 16, "%c%p", char((v->complemented_r) ? '!' : ' '), v->rsubtree.get());
        }
        else{
            snprintf(lbuf, 16, "---");
            snprintf(rbuf, 16, "---");
        }

        printf("%2d] %2s %p L-> %s R-> %s\n", 
            i, v->root.c_str(), v.get(), lbuf, rbuf);
        
        i += 1;
    }
}

/// @brief Support function for the truth table print
/// @param root vertex root
/// @param ord order in which we want to evaluate 
/// @param truthVector vector of bool values that is used to evaluate the variables 
/// @param i depth index, used to choose the correct value from truthVector
/// @return evaluation value for the function
bool evaluate_vertex(vertex_ptr root,const std::vector<std::string>& ord,std::vector<bool>& truthVector,int i){
    if(root->root == "v0")
        return false;
    else if( root->root == "v1") 
        return true;
    else if(root->root == ord[i])
    {    
        if(truthVector[i] == true)
           return evaluate_vertex(root->lsubtree,ord,truthVector,i+1);
        else return evaluate_vertex(root->rsubtree,ord,truthVector,i+1);
    }
    else
    { 
        printf("Mistakes were made during evaluation of the Vertex");
        exit(-1);
        }
}



void print_truth_table(vertex_ptr f, const std::vector<std::string>& ord){
    printf("\nTruth table for the function: \n\n");

    std::vector<bool> truthTable(pow(2, ord.size())*(ord.size()+1),false);

    /*
    for (auto& var : ord)
        printf("%05s", var.c_str());
    printf("\n");
    */
    auto ord_size = ord.size();
    unsigned int bits = 0b0;
    for(unsigned int i = 0; i < pow(2, ord.size()); i++) {
        for(unsigned int j = 0; j < ord.size(); j++) {

            bool value = (bits >> j) & 0b1;
            truthTable[i * (ord.size()+1) + j] = value;
            //printf("%05s", value ? "T" : "F");

        }
        //printf("\n");
        bits += 1;
    }

    std::vector<bool> truthVect;

    for(int i = 0; i < pow(2, ord.size())*(ord.size()+1); i = i + (ord.size()+1)) {
        std::vector<bool> subtable = std::vector<bool>(truthTable.begin() + i, truthTable.begin() + i + ord.size() + 1);
        truthTable[i+ord.size()] = evaluate_vertex(f,ord, subtable,0);
    }

    
    for (auto& var : ord)
        printf("%5s", var.c_str());
    printf("%5s", "out");    
    printf("\n");
    

    for(unsigned int i = 0; i < pow(2, ord.size()); i++) {
        for(unsigned int j = 0; j < ord.size()+1; j++) {
           printf("%5s",truthTable[i * (ord.size()+1) + j] ? "T" : "F");
        }
        printf("\n");
    }

    printf("\n");
}


void parse_input(std::string file, std::vector<std::string> &order, std::vector<jabddl::fun> &expr){
    std::ifstream infile(file);
    std::string line;
    std::string delimiter_func = "=";
    std::string delimiter_var = " ";
    jabddl::fun func_token;
    size_t pos = 0;
    //template for variables and functions name
    std::regex p_v_name("([a-z][0-9])+"), p_fname("F[0-9]+"), p_print("print F[0-9]+");
    //template for ite 
    std::regex p_expr("ite\(((,?\s*[a-z][0-9])+|(,?\s*[0-1])+|(,?\s*F[0-9]\+[a-z]*[0-9])+|(,?\s*F[0-9]\-[a-z]*[0-9])+|(,?\s*F[0-9])+)+\)");

    std::smatch var,func, expr_parse;

    if(VERBOSE) std::cout << "Beginning parsing..." <<std::endl;

    //Get number of variables
    std::getline(infile, line);
    int num_var = std::stoi(line);

    //Get number of functions
    std::getline(infile, line);
    int num_func = std::stoi(line);

    //parse variables
        std::getline(infile, line); 
        //Check if template is correct
        int count = 0;
        while(std::regex_search(line,var, p_v_name)){
                order.push_back(var[1].str());
                if(VERBOSE){
                    std::cout << "match size:" <<var.size();
                    std::cout << "Found var: " << var[1].str() <<std::endl;
                }
                line = var.suffix().str();
                count++;
        }
        if(count == 0){
            std::cout <<  "Error parsing variables";
            exit(-1);
        }

    //parse functions 
    for(int i = 0 ; i < num_func; i++){
        //read newline
        std::getline(infile, line); 
        pos = line.find(delimiter_func);
        std::string token_string = line.substr(0, pos);
        //Check if function name template is correct
        if(std::regex_search(token_string,func, p_fname))
            func_token.func_name = line.substr(0, pos);
        else{
            std::cout <<  "Error parsing name of function";
            exit(-1);
        }
        if(VERBOSE)
            std::cout <<"Found function: " << line << std::endl;
        line.erase(0, pos + delimiter_func.length());
        token_string = line.substr(0, pos);
        //check if expression template is correct
        if(std::regex_search(token_string,expr_parse, p_expr))
            func_token.expr = line;
        else{
            std::cout <<  "Error parsing ite body";
            exit(-1);
        }
        if(VERBOSE)
            std::cout <<"Function body: " << line << std::endl;
        expr.push_back(func_token);
    }
    if(VERBOSE) std::cout << "Parsing complete!" <<std::endl;
}


} // namespace jabddl