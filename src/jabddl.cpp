/*
JABDDL

Copyright© 2023 Daniele Nicoletti 

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), 
to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, 
and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

#include "../include/jabddl.hpp"

int complemented_mode = 0;

namespace jabddl {

//unique table to represent the vertices of all robdds
std::vector<vertex_ptr> unique_table;

void initialize() {
    unique_table.push_back(v0);
    unique_table.push_back(v1);
}


vertex_ptr old_or_new(const std::string& root, vertex_ptr lst, vertex_ptr rst){
    vertex_ptr v;
    auto result = lookup(unique_table, root, lst, rst);
    if (result.has_value())
        return *result;
    else
    {
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


vertex_ptr old_or_new_comp(vertex_ptr root){
    vertex_ptr v;
    auto result = lookup_comp(unique_table, root->root, root->lsubtree, root->rsubtree, root->complemented_l, root->complemented_r);
    if (result.has_value())
        return *result;
    else
    {
        /*create a new vertex*/
        auto l_tree = lookup_comp(unique_table,root->lsubtree->root,root->lsubtree->lsubtree,root->lsubtree->rsubtree,root->lsubtree->complemented_l,root->lsubtree->complemented_r);
        auto r_tree = lookup_comp(unique_table,root->rsubtree->root,root->rsubtree->lsubtree,root->rsubtree->rsubtree,root->rsubtree->complemented_l,root->rsubtree->complemented_r);
        if(l_tree.has_value())
            root->lsubtree = l_tree.value(); 
        if(r_tree.has_value())
            root->rsubtree = r_tree.value();   
        v = std::make_shared<vertex>(root->root, root->lsubtree, root->rsubtree, root->complemented_l, root->complemented_r);
        //adds it to the unique table
        unique_table.push_back(v);
        return v;
    }
}

bool vertex_compare(vertex_ptr vertex1,vertex_ptr vertex2){
    bool result = false;
    if((vertex1->root == "v0" && vertex2->root == "v0") || (vertex1->root == "v1" && vertex2->root == "v1"))
    {
        return true;
    }
    else if((vertex1->root == "v1" && vertex2->root == "v0") || (vertex1->root == "v0" && vertex2->root == "v1"))
    {
        return false;
    }
    else if(vertex1->root != "v0" && vertex2->root != "v1" && vertex1->root != "v1" && vertex2->root != "v0")
    {
        result = vertex_compare(vertex1->lsubtree,vertex2->lsubtree) && vertex_compare(vertex1->rsubtree,vertex2->rsubtree) && (vertex1->root == vertex2->root);
    }
    return result;
}

bool vertex_compare_comp(complemented_vertex vertex1,complemented_vertex vertex2){
    //Base case
    if(vertex1.root->root == "v1" && vertex2.root->root == "v1" && vertex1.complemented == vertex2.complemented)
    {
        return true;
    }
    //roots have same name but one of the two is complemented and the other no
    else if(((vertex1.root->root == vertex2.root->root) && (vertex1.complemented != vertex2.complemented)) || (vertex1.root->root != vertex2.root->root))
    {
        return false;
    }
    //same name and same complementation, need to check for subtrees
    else
    {
        complemented_vertex left1,left2,right1,right2;
        left1.root= vertex1.root->lsubtree;
        left1.complemented = vertex1.root->complemented_l;
        left2.root= vertex2.root->lsubtree;
        left2.complemented = vertex2.root->complemented_l;
        
        right1.root= vertex1.root->rsubtree;
        right1.complemented = vertex1.root->complemented_r;
        right2.root= vertex2.root->rsubtree;
        right2.complemented = vertex2.root->complemented_r;


        return vertex_compare_comp(left1,left2) && vertex_compare_comp(right1,right2) && (vertex1.root->root == vertex2.root->root);
    }
}


vertex_ptr robdd_build(expr_ptr f, int i, const std::vector<std::string>& ord) {
     vertex_ptr l,r;
     std::string root;

    if(f->type == expr_type::Var && (*f->var.name == v0_v || *f->var.name == v1_v))
    {
        if(*f->var.name == v0_v)
            return v0;
        if(*f->var.name == v1_v)
            return v1; 
    }
    else
    {

        root = ord[i];
        
        auto l_copy = std::unique_ptr<expr>(copy_expr_rec(f));
        l = robdd_build(evaluate(l_copy.get(), ord[i], true), i+1, ord);
        if(verbosity == 2) std::cout<< "f valutata per: " << ord[i] <<" positivo\n";

        auto r_copy = std::unique_ptr<expr>(copy_expr_rec(f));
        r = robdd_build(evaluate(r_copy.get(), ord[i], false), i+1, ord);
        if(verbosity == 2) std::cout<< "f valutata per: " << ord[i] <<" negativo\n";
    
        if(vertex_compare(l,r)/*l->lsubtree == r->lsubtree && l->rsubtree == r->rsubtree*/)
            return l;
        else return old_or_new(root,l,r);
        
    }

    exit(-1);
}

vertex_ptr robdd_build_comp(expr_ptr f, int i, const std::vector<std::string>& ord) {
     vertex_ptr l,r;
     std::string root;
     bool lcomp = false;
     bool rcomp = false;

    if(f->type == expr_type::Var && (*f->var.name == v0_v || *f->var.name == v1_v))
    {
        if(*f->var.name == v0_v)
            return v0;
        if(*f->var.name == v1_v)
            return v1; 
    }
    else
    {

        //assert(i < ord.size());
        root = ord[i];
        
        auto l_copy = std::unique_ptr<expr>(copy_expr_rec(f));
        l = robdd_build_comp(evaluate(l_copy.get(), ord[i], true), i+1, ord);
      
        //if l points to v0, it is replaced by v1 and marked as complemented
        if(l->root == v0->root)
        {
            lcomp = true;
            l = v1;
        }
        if(verbosity == 2) std::cout<< "f evaluated for: " << ord[i] <<" positive\n";


        auto r_copy = std::unique_ptr<expr>(copy_expr_rec(f));
        r = robdd_build_comp(evaluate(r_copy.get(), ord[i], false), i+1, ord);
       
        //if r points to v0, it is replaced by v1 and marked as complemented
        if(r->root == v0->root)
        {
            rcomp = true;
            r = v1;
        }
        if(verbosity == 2) std::cout<< "f evaluated for: " << ord[i] <<" negative\n";

        complemented_vertex left,right;
        left.root = l;
        left.complemented = lcomp;
        right.root = r;
        right.complemented = rcomp;

        if(vertex_compare_comp(left,right)/*l->lsubtree == r->lsubtree && l->rsubtree == r->rsubtree*/)
            return l;
        else{
            vertex_ptr res = std::make_shared<vertex>(root,l,r,lcomp,rcomp);
            return old_or_new_comp(res);
        }
    }

    exit(-1);
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
    else
    {
        root = ord[i];
        l = apply_ite(vertex_cofactor(f,ord[i],true),vertex_cofactor(g,ord[i],true),vertex_cofactor(h,ord[i],true),i+1,ord);
        r = apply_ite(vertex_cofactor(f,ord[i],false),vertex_cofactor(g,ord[i],false),vertex_cofactor(h,ord[i],false),i+1,ord);
        
        if(vertex_compare(l,r))
            return l;
        else 
            return old_or_new(root,l,r);
    }
}


/// @brief recursive step for ite_comp procedure
/// @param f structure for f function keeping track of complementation
/// @param g structure for g function keeping track of complementation
/// @param h structure for h function keeping track of complementation
/// @param i counter for level of recursion
/// @param ord vector for variable names
/// @return structure that contains vertex_ptr to ite function and a var that says if it is complemented or not
complemented_vertex apply_ite_comp_rec(complemented_vertex f, complemented_vertex g, complemented_vertex h,int i ,const std::vector<std::string>& ord){
     vertex_ptr l,r;
     std::string root;
    //if finished to evaluate f 
    if(f.root->root == "v1"){
        //referring to lsubtree and not complemented
        if(!f.complemented){
            return g;
        }       
        //referring to lsubtree and complemented
        else{
            return h;
        }
    }
    //if both functions g and h are true, then return simply f
    else if (g.root->root == "v1" && h.root->root == "v1" && !g.complemented && h.complemented){
        return f;
    }
    else
    {
        root = ord[i];
        vertex_ptr new_vertex = std::make_shared<vertex>(root,l,r,false,false);
        complemented_vertex left, right, res;

        left = apply_ite_comp_rec(vertex_cofactor_comp(f,ord[i],true),vertex_cofactor_comp(g,ord[i],true),vertex_cofactor_comp(h,ord[i],true),i+1,ord);
        right = apply_ite_comp_rec(vertex_cofactor_comp(f,ord[i],false),vertex_cofactor_comp(g,ord[i],false),vertex_cofactor_comp(h,ord[i],false),i+1,ord);
        
        new_vertex->complemented_l = left.complemented;
        new_vertex->complemented_r = right.complemented;
        new_vertex->lsubtree = left.root;
        new_vertex->rsubtree = right.root;
        res.complemented = false;

        if(vertex_compare_comp(left,right))
            return left;
        else{
            res.root = old_or_new_comp(new_vertex);
            return res;
        }
    }

}


vertex_ptr apply_ite_comp(vertex_ptr f, vertex_ptr g, vertex_ptr h,int i ,const std::vector<std::string>& ord){
    complemented_vertex f_c,g_c,h_c,result;
    f_c.root = f;
    f_c.complemented = false;
    g_c.root = g;
    g_c.complemented = false;
    h_c.root = h;
    h_c.complemented = false;

    result = apply_ite_comp_rec(f_c,g_c,h_c,i,ord);

    if(result.complemented)
        std::cout << "The resulting function is to be considered complemented" <<std::endl;
   
    return result.root;
    
}

complemented_vertex propagate_complemented(complemented_vertex root){
    
    //First we need to arrive at the leaves
    if(root.root->root == "v1")
        return root;

    else{
        complemented_vertex l,r,left,right, ph;
        vertex_ptr v = std::make_shared<vertex>(root.root->root);
        ph.root = v;

        l.root = root.root->lsubtree;
        l.complemented = root.root->complemented_l;
        r.root = root.root->rsubtree;
        r.complemented = root.root->complemented_r;

        right = propagate_complemented(r);
        left =  propagate_complemented(l);
        //At this point we need to evaluate how to propagate the complementation, modifying this level and the bottom one as well.
        if(!root.complemented && left.complemented && right.complemented){
            root.complemented = true;
            ph.root->lsubtree = left.root;
            ph.root->rsubtree = right.root;
            ph.root->complemented_l = false;
            ph.root->complemented_r = false;
            
            vertex_ptr vrt = jabddl::old_or_new_comp(ph.root);
            root.root = vrt;
            return root;
        }
        else if(root.complemented && left.complemented && right.complemented){
            root.complemented = false;
            ph.root->lsubtree = left.root;
            ph.root->rsubtree = right.root;
            ph.root->complemented_l = false;
            ph.root->complemented_r = false;
            
            vertex_ptr vrt = jabddl::old_or_new_comp(ph.root);
            root.root = vrt;

            return root;
        }
        else if(root.complemented && left.complemented){
            root.complemented = false;
            ph.root->lsubtree = left.root;
            ph.root->rsubtree = right.root;
            ph.root->complemented_l = false;
            ph.root->complemented_r = true;
            
            vertex_ptr vrt = jabddl::old_or_new_comp(ph.root);
            root.root = vrt;

            return root;
        }
        else if(left.complemented && !root.complemented){
            root.complemented = true;
            ph.root->lsubtree = left.root;
            ph.root->rsubtree = right.root;
            ph.root->complemented_l = false;
            ph.root->complemented_r = true;
            
            vertex_ptr vrt = jabddl::old_or_new_comp(ph.root);
            root.root = vrt;

            return root;
        }
        else{
            ph.root->lsubtree = left.root;
            ph.root->rsubtree = right.root;
            ph.root->complemented_l = left.complemented;
            ph.root->complemented_r = right.complemented;
            
            vertex_ptr vrt = jabddl::old_or_new_comp(ph.root);
            root.root = vrt;

            return root;
        }
    }
}



std::optional<vertex_ptr> lookup(const std::vector<vertex_ptr>& unique_table, const std::string& root, vertex_ptr lst, vertex_ptr rst){
    for (auto& vertex : unique_table)
    {
        if (vertex->root == root && vertex->lsubtree == lst && vertex->rsubtree == rst)
            return std::make_optional<vertex_ptr>(vertex);
    }
    return std::nullopt;
}

std::optional<vertex_ptr> lookup_comp(const std::vector<vertex_ptr>& unique_table, const std::string& root, vertex_ptr lst, vertex_ptr rst, bool lcomp, bool rcomp){
    for (auto& vertex : unique_table)
    {
        if (vertex->root == root && vertex->lsubtree == lst && vertex->rsubtree == rst && vertex->complemented_l == lcomp && vertex->complemented_r == rcomp)
            return std::make_optional<vertex_ptr>(vertex);
    }
    return std::nullopt;
}

void print_table(std::vector<vertex_ptr> unique_table){
    
    int i = 0;
    for(const auto& v : unique_table)
    {
        
        char lbuf[16], rbuf[16];
        if (v->root != "v1" && v->root != "v0") 
        {
            snprintf(lbuf, 16, "%c%p", char((v->complemented_l) ? '!' : ' '), (void*)v->lsubtree.get());
            snprintf(rbuf, 16, "%c%p", char((v->complemented_r) ? '!' : ' '), (void*)v->rsubtree.get());
        }
        else
        {
            snprintf(lbuf, 16, "---");
            snprintf(rbuf, 16, "---");
        }

        printf("%2d] %2s %p L-> %s R-> %s\n", 
            i, v->root.c_str(), (void*)v.get(), lbuf, rbuf);
        
        i += 1;
    }
}

void print_truth_table(vertex_ptr f, const std::vector<std::string>& ord, bool complemented){
    printf("\nTruth table for the function: \n\n");

    std::vector<bool> truthTable(pow(2, ord.size())*(ord.size()+1),false);

    /*
    for (auto& var : ord)
        printf("%05s", var.c_str());
    printf("\n");
    */
    unsigned int bits = 0b0;
    for(unsigned int i = 0; i < pow(2, ord.size()); i++)
    {
        for(unsigned int j = 0; j < ord.size(); j++)
        {

            bool value = (bits >> j) & 0b1;
            truthTable[i * (ord.size()+1) + j] = (complemented?value:!value);
            //printf("%05s", value ? "T" : "F");

        }
        //printf("\n");
        bits += 1;
    }

    std::vector<bool> truthVect;

    for(int i = 0; i < pow(2, ord.size())*(ord.size()+1); i = i + (ord.size()+1))
    {
        std::vector<bool> subtable = std::vector<bool>(truthTable.begin() + i, truthTable.begin() + i + ord.size() + 1);
        if(!complemented_mode){
            truthTable[i+ord.size()] = evaluate_vertex(f,ord, subtable,0);
        }else{
            complemented_vertex f_c;
            f_c.complemented = false;
            f_c.root = f;    
            truthTable[i+ord.size()] = evaluate_vertex_comp(f_c,ord, subtable, 0);
        }
    }

    
    for (auto& var : ord)
        printf("%5s", var.c_str());
    printf("%5s", "out");    
    printf("\n");
    

    for(unsigned int i = 0; i < pow(2, ord.size()); i++)
    {
        for(unsigned int j = 0; j < ord.size()+1; j++)
        {
           printf("%5s",truthTable[i * (ord.size()+1) + j] ? "T" : "F");
        }
        printf("\n");
    }

    printf("\n");
}

/// @brief Support function to parse ite body parts (if,then,else)
/// @param ite_part string representing the ite_part
/// @param cntx context object to be populated
/// @return expression representing the ite_part 
jabddl::expr_ptr parse_ite_parts(std::string ite_part, jabddl::context &cntx){
    std::regex p_const("\\s*([0-1])"), p_var("\\s*([a-z][0-9]?)"), p_fun("F[0-9]"), p_f_op("(F[0-9]\\+F[0-9])+|(F[0-9]\\-F[0-9])+|(F[0-9]\\+[a-z][0-9]?)+|(F[0-9]\\-[a-z][0-9]?)+|(F[0-9]\\+[0-1])+|(F[0-9]\\-[0-1])+"),
               p_op("(F[0-9])|(\\-|\\+)|([a-z][0-9]?)"), p_negvar("\\s*(\\![a-z][0-9]?)");
    std::smatch match_obj;

    if(verbosity) std::cout << "Beginning parsing of: " << ite_part << std::endl;

    //if regex matches a restricted function name 
    if(std::regex_search(ite_part, match_obj ,p_f_op)){
        //parse Function , operation and varible 
        if(std::regex_search(ite_part,match_obj,p_op)){
            if(cntx.funcs.find(match_obj.str(0)) != cntx.funcs.end()){
                auto f = cntx.funcs[match_obj.str(0)];
                std::string f_name = match_obj.str();
                std::string cof_op,var_str;
                ite_part = match_obj.suffix().str();

                if(!(std::regex_search(ite_part,match_obj,p_op))){
                    std::cout << "Parsing of restricted function failed to retrieve type of cofactor" << std::endl;
                    exit(-1);
                }
                cof_op = match_obj.str();
                ite_part = match_obj.suffix().str();

                 if(!(std::regex_search(ite_part,match_obj,p_op))){
                    std::cout << "Parsing of restricted function failed to retreive cofactor variable" << std::endl;
                    exit(-1);
                }
                var_str = match_obj.str();

                if(verbosity){
                    std::cout << "function: " << f_name << std::endl;
                    std::cout << "operation: " << cof_op  << std::endl;
                    std::cout << "variable: " << var_str << std::endl;
                }

                if(cof_op == "+"){
                    if(verbosity){
                        std::cout << "Function: " << f_name << " Evaluated for variable: " <<var_str << " positive cofactor" <<std::endl;
                    }
                    return evaluate(ite(f.ite_if,f.ite_then,f.ite_else),var_str,true);
                }
                else{
                    if(verbosity){
                        std::cout << "Function: " << f_name << " Evaluated for variable: " <<var_str << " negative cofactor" <<std::endl;
                     }
                    return evaluate(ite(f.ite_if,f.ite_then,f.ite_else),var_str,false);
                }
            }
            else{
                std::cout << "Trying to refence a function that has never been declared!!" << ite_part <<std::endl;
                exit(-1);
            }
        }    
    }
    //if regex matches a function name
    else if ( std::regex_search(ite_part, match_obj, p_fun)){
        //If the function exists in context
        if(cntx.funcs.find(match_obj.str()) != cntx.funcs.end()){
            auto f =  cntx.funcs[match_obj.str()];
            //return the ite of the function parts
            if(verbosity){
                std::cout << "Function: " << match_obj.str()  <<std::endl;
            }
            return ite(f.ite_if,f.ite_then,f.ite_else);
        }
    //if regex matches a negation of variable 
    }else if(std::regex_search(ite_part, match_obj, p_negvar)){
        std::string notvar = match_obj.str();
        if(std::regex_search(notvar, match_obj, p_var)){
            if(std::find(cntx.vars.begin(), cntx.vars.end(), match_obj.str()) != cntx.vars.end()){
                //return a expr_ptr variable 
                if(verbosity){
                    std::cout << "Variable: " <<match_obj.str()  <<std::endl;
                }
                //add a negative variable to ite_part
                return expr::make_neg(expr::make_var(match_obj.str()));
            }
            else{
                std::cout << "Trying to reference a variable that has never been declared!! " <<std::endl;
                exit(-1);
            }
        }
        else{
            std::cout << "Error parsing variable for a !var " <<std::endl;
            exit(-1);
        }
    //if regex matches a variable     
    }else if(std::regex_search(ite_part, match_obj, p_var)){
        if(std::find(cntx.vars.begin(), cntx.vars.end(), match_obj.str()) != cntx.vars.end()){
            //return a expr_ptr variable 
            if(verbosity){
                std::cout << "Variable: " <<match_obj.str()  <<std::endl;
            }
            return expr::make_var(match_obj.str());
        }
        else{
            std::cout << "Trying to reference a variable that has never been declared!! " <<std::endl;
            exit(-1);
        }
    }
    //if regex matches a constant (0 or 1)
    else if(std::regex_search(ite_part, match_obj, p_const)){
        //return a variable named: v0 or v1 
        if(verbosity){
            std::cout << "constant: " <<match_obj.str()  <<std::endl;
        }
        return expr::make_var("v"+match_obj.str());
    }
    else{
        std::cout << "Impossible to have a match on ite_part: " << ite_part <<std::endl;
        exit(-1);
    }
    std::cout << "Error: execution escaped if-else statement during parsing of ite body parts!! " <<std::endl;
        exit(-1);
}

/// @brief Parse line_expr to get if then else tokens 
/// @param func fun obj to be filled
/// @param line_expr string that represent ite expr
/// @param cntx context object to store variable and functions
void parse_func(jabddl::fun &func, std::string line_expr, jabddl::context &cntx){
    std::regex p_v_name("(F[0-9]\\+F[0-9])+|(F[0-9]\\-F[0-9])+|(F[0-9]\\+[a-z][0-9]?)+|(F[0-9]\\-[a-z][0-9]?)+|(F[0-9]\\+[0-1])+|(F[0-9]\\-[0-1])+|(\\!?[a-z][0-9]?)+|([0-1])+|(F[0-9])+");
    std::regex p_const("[0-1]"), p_var("[a-z][0-9]?"), p_fun("F[0-9]"), p_f_op("(F[0-9]\\+F[0-9])+|(F[0-9]\\-F[0-9])+|(F[0-9]\\+[a-z][0-9]?)+|(F[0-9]\\-[a-z][0-9]?)+|(F[0-9]\\+[0-1])+|(F[0-9]\\-[0-1])+");
    std::smatch part;
    
    std::string ite_if,ite_then,ite_else;
    if(verbosity) std::cout << "Beginning parsing of function: " << func.func_name <<std::endl;

    //need to cycle 4 times because ite is an [a-z] so it is matched as a viable token
    for(int c = 0; c < 4 ; c++){
        if(std::regex_search(line_expr,part,p_v_name)){
            switch(c){
                case 0: break;
                case 1: ite_if = part.str(); break;
                case 2: ite_then = part.str(); break;
                case 3: ite_else = part.str(); break;
                default: std::cout << "Error in parsing ITE body!!" <<std::endl;
                         exit(-1);
            };
            if(verbosity) std::cout << "Found ite member: " << part.str() <<std::endl;
            line_expr = part.suffix().str();
        }
        else{
            std::cout << "Format error in ITE body of function: " <<func.func_name <<std::endl;
            exit(-1);
        }
    }

    func.ite_if = parse_ite_parts(ite_if, cntx);
    if(verbosity) std::cout << "ite_if parsed correctly" <<std::endl;
    func.ite_then = parse_ite_parts(ite_then, cntx);
    if(verbosity) std::cout << "ite_then parsed correctly" <<std::endl;
    func.ite_else = parse_ite_parts(ite_else , cntx);
    if(verbosity) std::cout << "ite_else parsed correctly" <<std::endl;

    //if true it means we have more than 3 member for ITE
    if(std::regex_search(line_expr,part,p_v_name)){
        std::cout << "Too many arguments for ITE in function: " <<func.func_name <<std::endl;
        exit(-1);
    }
    
}

void parse_input(std::string file, jabddl::context &cntx){
    std::ifstream infile(file);
    std::string line;
    std::string delimiter_func = "=";
    std::string delimiter_var = " ";
    jabddl::fun func_token;

    //template for variables and functions name
    std::regex p_v_name("([a-z][0-9]?)+"), p_fname("F[0-9]+"), p_print("print F[0-9]+");
    //template for ite 
    std::regex p_expr("ite\\(((,?\\s*\\!?[a-z][0-9]?)+|(,?\\s*[0-1])+|(,?\\s*F[0-9]\\+[a-z][0-9]?)+|(,?\\s*F[0-9]\\-[a-z][0-9]?)+|(,?\\s*F[0-9]\\-[0-1])+|(,?\\s*F[0-9]\\+[0-1])+|(,?\\s*F[0-9])+)+\\)");

    std::smatch var,func, expr_parse,print;

    if(verbosity) std::cout << "Beginning parsing..." <<std::endl;

    //Get number of variables
    std::getline(infile, line);
    int num_var = std::stoi(line);

    //Get number of functions
    std::getline(infile, line);
    int num_func = std::stoi(line);

    if(verbosity) std::cout << std::endl  << "Beginning varibles parsing..." <<std::endl;

    //parse variables
        std::getline(infile, line); 
        //Check if template is correct
        int count = 0;
        while(std::regex_search(line,var, p_v_name))
        {
                cntx.vars.push_back(var.str());
                if(verbosity)
                {
                    //std::cout << "match size:" <<var.size() <<std::endl;
                    std::cout << "Found var: " << var[1].str() <<std::endl;
                }
                line = var.suffix().str();
                count++;
        }

        if(count != num_var)
        {
            std::cout <<  "Error parsing variables";
            exit(-1);
        }

    if(verbosity) std::cout << std::endl  << "Beginning functions parsing..." <<std::endl;

    //parse functions 
    for(int i = 0 ; i < num_func; i++){
        //read newline
        std::getline(infile, line); 
        //Check if function name template is correct
        if(std::regex_search(line,func, p_fname))
            func_token.func_name = func.str();
        else
        {
            std::cout <<  "Error parsing name of function";
            exit(-1);
        }
        if(verbosity)
            std::cout <<"Found function: " << func.str() << std::endl;
        //check if expression template is correct
        if(std::regex_search(line,expr_parse, p_expr)){
            parse_func(func_token,expr_parse.str(), cntx);
        }
        else
        {
            std::cout <<  "Error parsing ite body, incorrect template!!";
            exit(-1);
        }
        if(verbosity)
            std::cout <<"Function body: " << expr_parse.str() << std::endl;
        cntx.funcs.insert(std::make_pair(func_token.func_name,func_token));
    }

    if(verbosity) std::cout << std::endl << "Beginning prints parsing..." <<std::endl;

    //Should remain only prints now
    //Cycle through them 
    while(std::getline(infile, line)){
        //If pattern matches
        if(std::regex_search(line, print, p_fname)){
            //And function is present in context, set it to be printable
            if(cntx.funcs.find(print.str()) != cntx.funcs.end()){
                cntx.funcs[print.str()].tbp = true;
                if(verbosity)
                    std::cout << "To be printed: " << print.str() <<std::endl;
            }
            else{
                std::cout <<"Trying to print a function that has never been declared!!" << std::endl;
                exit(-1);
            }
        }
        else
        {
            std::cout <<"Error parsing print!" << std::endl;
            exit(-1);
        }
    }
    if(verbosity) std::cout << "Parsing complete!" <<std::endl;
}
} // namespace jabddl