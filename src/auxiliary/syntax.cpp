/**
 * @file syntax.cpp
 * @author João Mendes
 * @brief Syntactic machinery for the generation of proof exercises
 * @version 0.0.1
 * @date 2025-06-21
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "syntax.h"

using namespace std;

/**
 * @brief Checks if a string is a function symbol 
 * 
 * @param symb String
 * @return True if `symb` is a function symbol
 * @return False otherwise
 */
bool is_function_symb(string symb) {
    map<string, int> function_symbs = pre_process_function_symbs();
    return function_symbs.find(symb) != function_symbs.end();
}

/**
 * @brief Checks if a string is a predicate symbol
 * 
 * @param symb String
 * @return True if `symb` is a predicate symbol
 * @return False otherwise
 */
bool is_predicate_symb(string symb) {
    map<string, int> predicate_symbs = pre_process_predicate_symbs();
    return predicate_symbs.find(symb) != predicate_symbs.end();
}

/**
 * @brief Checks if a string is either a predicate of a function symbol
 * 
 * @param symb String
 * @return True if `symb` is either a predicate or function symbol
 * @return False otherwise
 */
bool is_language_symb(string symb) {
    map<string, int> language_symbs = pre_process_language_symbs();

    return language_symbs.find(symb) != language_symbs.end();
}

/**
 * @brief Checks if a string is not a skolemization symbol
 * 
 * @param symb String
 * @return True if `symb` is not a skolemization symbol 
 * @return False otherwise 
 */
bool is_not_skolem_symb(string symb) {
    map<string, int> no_skolem_symb = pre_process_no_skolem_symbs();

    return no_skolem_symb.find(symb) != no_skolem_symb.end();
}

/**
 * @brief Checks if a node of a formula is a parameter
 * 
 * @param fmla_node Node of a formula
 * @return True if `fmla_node` is a parameter 
 * @return False otherwise 
 */
bool is_a_parameter(FmlaNode fmla_node) {
    return fmla_node.children.size() == 0 && !is_language_symb(fmla_node.data);
}

/**
 * @brief Checks if a node of a term is a parameter
 * 
 * @param term_node Node of a term
 * @return True if `term_node` is a parameter 
 * @return False otherwise 
 */
bool is_a_parameter(TermNode term_node) {
    return term_node.children.size() == 0 && !is_language_symb(term_node.data);
}

/**
 * @brief Checks if a parameter is in a formula
 * 
 * @param fmla Formula
 * @param parameter String representing a parameter
 * @return True if `parameter` is in `fmla` 
 * @return False 
 */
bool parameter_in_fmla(Fmla fmla, string parameter) {
    for (FmlaNode fmla_node : fmla) {
        if (is_a_parameter(fmla_node) && fmla_node.data == parameter) return true;
    }
    return false;
}

/**
 * @brief Gets the term of a formula by the index of the root of the term
 * 
 * @param fmla Formula
 * @param term_root Index of the root of the term
 * @return The term that has as root the index `term_root` 
 */
Term get_term_of_fmla(Fmla fmla, int term_root) {
    set<int> subfmla_nodes_idx;
    Term term;

    // BFS in the term root
    queue<int> q;
    q.push(term_root);

    // Map to make the correct attribution for the new ids of the term
    map<int, int> new_idx;

    while (!q.empty()) {
        int current = q.front();
        q.pop();

        new_idx[current] = term.size(); 
        FmlaNode fmla_node = fmla[current];

        TermNode term_node;
        term_node.data = fmla_node.data;
        if (term.size() == 0) {
            term_node.parent = 0;
        }
        else {
            term_node.parent = new_idx[fmla_node.parent];
            term[term_node.parent].children.push_back(term.size());
        }
        term_node.children = {};

        term.push_back(term_node);

        for (int child : fmla[current].children) {
            q.push(child);
        }
    }

    return term;
}

/**
 * @brief Get the all terms of a formula
 * 
 * @param fmla Formula
 * @return Vector of terms representing the terms of `fmla` 
 */
vector<Term> get_all_terms_of_fmla(Fmla fmla) {
    vector<Term> all_terms;

    for (int i = 0; i < fmla.size(); i++) {
        if (!is_predicate_symb(fmla[i].data)) {
            all_terms.push_back(get_term_of_fmla(fmla, i));
        }
    }

    return all_terms;
}

/**
 * @brief Substitute a parameter by a term in a formula
 * 
 * @param fmla Formula
 * @param parameter_idx Index of the parameter to be substituted 
 * @param term Term
 * @return Result of substituting the parameter indexed by `parameter_idx` by `term` in `fmla` 
 */
Fmla subst_parameter_by_term(Fmla fmla, int parameter_idx, Term term) {
    map<int, int> new_idx;

    new_idx[0] = parameter_idx;

    for (int i = 1; i < term.size(); i++) {
        new_idx[i] = fmla.size() + i - 1;
    }

    for (int i = 0; i < term.size(); i++) {
        FmlaNode fmla_node;
        fmla_node.data = term[i].data;
        fmla_node.children = {};
        if (i == 0) {
            fmla_node.parent = fmla[parameter_idx].parent;
            fmla[parameter_idx] = fmla_node;
        }
        else {
            fmla_node.parent = new_idx[term[i].parent];
            fmla[fmla_node.parent].children.push_back(fmla.size());
            fmla.push_back(fmla_node);
        }
    }
    return fmla;
}

/**
 * @brief Gets the indexes of the parameters of a formula
 * 
 * @param fmla Formula
 * @return Set of integers of the indexes of the parameters of `fmla` 
 */
set<int> get_parameters_idxs(Fmla fmla) {
    set<int> parameters_idx;

    for (int i = 0; i < fmla.size(); i++) {
        if (is_a_parameter(fmla[i])) parameters_idx.insert(i);
    }

    return parameters_idx;
}

/**
 * @brief Get the parameters of a formula
 * 
 * @param fmla Formula
 * @return Set string of the parameters of `fmla` 
 */
set<string> get_parameters(Fmla fmla) {
    set<string> parameters;

    for (int i = 0; i < fmla.size(); i++) {
        if (is_a_parameter(fmla[i])) parameters.insert(fmla[i].data);
    }

    return parameters;

}

/**
 * @brief Get the parameters of a vector of formulas
 * 
 * @param fmlas Vector of formulas
 * @return Set string of the parameters of `fmlas` 
 */
set<string> get_parameters(vector<Fmla> fmlas) {
    set<string> parameters;

    for (Fmla fmla : fmlas){
        for (int i = 0; i < fmla.size(); i++) {
            FmlaNode fmla_node = fmla[i];
            if (is_a_parameter(fmla_node)) parameters.insert(fmla_node.data);

        }
    }

    return parameters;
}

/**
 * @brief Substitutes parameters by term in a formula provided a substitution mapping
 * 
 * @param fmla Formula
 * @param subs Substitution mapping
 * @return Resulting of applying `subs` in `fmla` 
 */
Fmla subst_extension(Fmla fmla, Subst subs) {
    int fmla_initial_size = fmla.size();

    set<int> parameters_idxs = get_parameters_idxs(fmla);

    for (int i = 0; i < fmla_initial_size; i++) {
        if (parameters_idxs.find(i) != parameters_idxs.end()) {
            fmla = subst_parameter_by_term(fmla, i, subs[fmla[i].data]);
        }
    }

   return fmla;
}

/**
 * @brief Checks if two formulas are equal
 * 
 * @param fmla1 Formula
 * @param fmla2 Formula
 * @return True if `fmla1` and `fmla2` are equal
 * @return False otherwise 
 */
bool fmla_equality(Fmla fmla1, Fmla fmla2) {
    if (fmla1.size() != fmla2.size()) return false;

    queue<FmlaNode> q1, q2;
    q1.push(fmla1[0]);
    q2.push(fmla2[0]);

    while (!q1.empty()) {
        FmlaNode current1 = q1.front();
        FmlaNode current2 = q2.front();

        if (current1.data != current2.data) return false;

        q1.pop(); q2.pop();


        if (current1.children.size() != current2.children.size()) {
            return false;
        }
        else {
            for (int i = 0; i < current1.children.size(); i++) {
                q1.push(fmla1[current1.children[i]]);
                q2.push(fmla2[current2.children[i]]);
            }
        }
    }

    return true;
}

/**
 * @brief Checks if two formulas are equal under substitution
 * 
 * @param fmla1 Formula
 * @param fmla2 Formula
 * @return True if `fmla1` and `fmla2` are equal under a substitution 
 * @return False otherwise
 */
bool fmla_equality_under_subst(Fmla fmla1, Fmla fmla2) {
    if (fmla1.size() != fmla2.size()) return false;

    map<string, string> subst;
    set<string> subst_img;

    queue<FmlaNode> q1, q2;
    q1.push(fmla1[0]);
    q2.push(fmla2[0]);

    while (!q1.empty()) {
        FmlaNode current1 = q1.front();
        FmlaNode current2 = q2.front();
        q1.pop(); q2.pop();

        if (is_a_parameter(current1) && is_a_parameter(current2)) {
            if (subst.find(current1.data) != subst.end()) {
                if (subst[current1.data] != current2.data) return false;
            }
            else {
                if (subst_img.find(current2.data) != subst_img.end()) return false;
                else {
                    subst[current1.data] = current2.data;
                    subst_img.insert(current2.data);
                }
            }
        }
        else if (current1.data != current2.data) return false;

        if (current1.children.size() != current2.children.size()) {
            return false;
        }
        else {
            for (int i = 0; i < current1.children.size(); i++) {
                q1.push(fmla1[current1.children[i]]);
                q2.push(fmla2[current2.children[i]]);
            }
        }
    }

    return true;

}

/**
 * @brief Checks if two formulas are isomorphic
 * 
 * @param fmla1 Formula
 * @param fmla2 Formula
 * @return True if `fmla1` and `fmla2` are isomorphic 
 * @return False otherwise 
 */
bool are_syntactically_isomorphic(Fmla fmla1, Fmla fmla2) {
    if (fmla1.size() != fmla2.size()) return false;

    map<string, string> subst;
    set<string> subst_img;

    queue<FmlaNode> q1, q2;
    q1.push(fmla1[0]);
    q2.push(fmla2[0]);

    while (!q1.empty()) {
        FmlaNode current1 = q1.front();
        FmlaNode current2 = q2.front();
        q1.pop(); q2.pop();

        if (is_a_parameter(current1) && is_a_parameter(current2)) {
            if (subst.find(current1.data) != subst.end()) {
                if (subst[current1.data] != current2.data) return false;
            }
            else {
                if (subst_img.find(current2.data) != subst_img.end()) return false;
                else {
                    subst[current1.data] = current2.data;
                    subst_img.insert(current2.data);
                }
            }
        }

        if (current1.children.size() != current2.children.size()) {
            return false;
        }
        else {
            for (int i = 0; i < current1.children.size(); i++) {
                q1.push(fmla1[current1.children[i]]);
                q2.push(fmla2[current2.children[i]]);
            }
        }
    }

    return true;

}

/**
 * @brief Checks if two formulas are isomorphic and their parameters are equal
 * 
 * @param fmla1 Formula
 * @param fmla2 Formula
 * @return True if `fmla1` and `fmla2` are isomorphic and their parameters are equal
 * @return False otherwise
 */
bool are_isomorphic_with_equal_parameters(Fmla fmla1, Fmla fmla2) {
    if (fmla1.size() != fmla2.size()) return false;

    queue<FmlaNode> q1, q2;
    q1.push(fmla1[0]);
    q2.push(fmla2[0]);

    while (!q1.empty()) {
        FmlaNode current1 = q1.front();
        FmlaNode current2 = q2.front();
        q1.pop(); q2.pop();

        if (is_a_parameter(current1) && is_a_parameter(current2)) {
            if (current1.data != current2.data) {
                return false;
            }            
        }

        if (current1.children.size() != current2.children.size()) {
            return false;
        }
        else {
            for (int i = 0; i < current1.children.size(); i++) {
                q1.push(fmla1[current1.children[i]]);
                q2.push(fmla2[current2.children[i]]);
            }
        }
    }

    return true;

}

/**
 * @brief Checks if two terms are equal
 * 
 * @param term1 Term
 * @param term2 Term
 * @return True if `term1` and `term2` are equal
 * @return False otherwise 
 */
bool term_equality(Term term1, Term term2) {
    if (term1.size() != term2.size()) return false;

    queue<TermNode> q1, q2;
    q1.push(term1[0]);
    q2.push(term2[0]);

    while (!q1.empty()) {
        TermNode current1 = q1.front();
        TermNode current2 = q2.front();

        if (current1.data != current2.data) return false;

        q1.pop(); q2.pop();

        if (current1.children.size() != current2.children.size()) {
            return false;
        }
        else {
            for (int i = 0; i < current1.children.size(); i++) {
                q1.push(term1[current1.children[i]]);
                q2.push(term2[current2.children[i]]);
            }
        }
    }

    return true;
}

/**
 * @brief For two isomorphic formulas fmla1 and fmla2, gets the node in fmla2 that is isomorphic to the node in fmla1
 * 
 * @param fmla1 Formula
 * @param parameter_idx Index of a node in `fmla1`
 * @param fmla2 Formula
 * @return Integer representing the isomorphic node in `fmla2` of `parameter_idx` in `fmla1`
 */
int get_term_idx_img_subst(Fmla fmla1, int parameter_idx, Fmla fmla2) {
    // Assume subst(fmla1) = fmla2. We want to find where the parameter in the position of parameter_idx is substituted in fmla2 

    queue<int> q1, q2;
    q1.push(0);
    q2.push(0);

    while(!q1.empty()) {
        int curr1 = q1.front();
        int curr2 = q2.front();

        q1.pop(); q2.pop();

        if (curr1 == parameter_idx) {
            return curr2;
        }
        for (int i = 0; i < fmla1[curr1].children.size(); i++) {
            q1.push(fmla1[curr1].children[i]);
            q2.push(fmla2[curr2].children[i]);
        }
    }

    return -1;
}

/**
 * @brief Gets the height of a fomrula
 * 
 * @param fmla Formula
 * @return Height of `fmla`
 */
int get_height(Fmla fmla) {

    vector<int> fmla_level(fmla.size(), 0);

    queue<int> q;
    q.push(0);

    while (!q.empty()) {
        int current = q.front();

        if (current != 0) fmla_level[current] = fmla_level[fmla[current].parent] + 1;

        q.pop();
        for (int child : fmla[current].children) {
            q.push(child);
        }
    }

    int height = -1;

    for (int i = 0; i < fmla_level.size(); i++) {
        if (fmla_level[i] > height) {
            height = fmla_level[i];
        }
    }
    return height;
    
}

/**
 * @brief Gets the maximum height of a formula in a vector of formulas
 * 
 * @param fmlas Vector of formulas
 * @return Maximum height of a formula in `fmlas`
 */
int get_height(vector<Fmla> fmlas) {

    int height = -1;

    for (Fmla fmla : fmlas) {
        if (get_height(fmla) > height) {
            height = get_height(fmla);
        }
    }
    return height;
    
}