/**
 * @file print.cpp
 * @author João Mendes
 * @brief Printing functions to output the result of the generation of proof exercises
 * @version 0.0.1
 * @date 2025-06-21
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "print.h"

using namespace std;

/**
 * @brief Pretty-prints a node of formula
 * 
 * @param fmla Formula
 * @param node Node of the formula
 * @param level Level in which `node` is in `fmla`
 */
void pretty_printing_fmla_aux(Fmla fmla, FmlaNode node, int level) {
    if (node.children.size() == 0) {
        cout << node.data;
    }
    set<string> skolem_symbs = pre_process_skolem_symbs();

    if (skolem_symbs.find(node.data) == skolem_symbs.end() && (node.children.size() == 1 || node.children.size() == 2) ) {
        if (node.children.size() == 1) {
            cout << "(";
            pretty_printing_fmla_aux(fmla, fmla[node.children[0]], level + 1);
            cout << ")";
        }
        if (node.children.size() == 2) {
            if (level == 0) {
                pretty_printing_fmla_aux(fmla, fmla[node.children[0]], level + 1);
                cout << " " << node.data << " ";
                pretty_printing_fmla_aux(fmla, fmla[node.children[1]], level + 1);
            }
            else {
                cout << "(";
                pretty_printing_fmla_aux(fmla, fmla[node.children[0]], level + 1);
                cout << " " << node.data << " ";
                pretty_printing_fmla_aux(fmla, fmla[node.children[1]], level + 1);
                cout << ")";
            }
        }
    }
    else {
        if (node.children.size() > 0) {
            cout << "" << node.data << "";
            cout << "(";
            for (int i = 0; i < node.children.size(); i++) {
                if (i < node.children.size() - 1) {
                    pretty_printing_fmla_aux(fmla, fmla[node.children[i]], level + 1);
                    cout << ", ";
                }
                else {
                    pretty_printing_fmla_aux(fmla, fmla[node.children[i]], level + 1);
                }
            }
            cout << ")";
        }
    }
    
}

/**
 * @brief Recursively pretty-prints a formula
 * 
 * @param fmla Formula do be printed
 */
void pretty_printing_fmla(Fmla fmla) {
    pretty_printing_fmla_aux(fmla, fmla[0], 0);
}

/**
 * @brief Prints a vector of integers
 * 
 * @param vec Vector of integers
 */
void print_vec_int(vector<int> vec) {
    cout << "[";
    for (int i = 0; i < vec.size(); i++){
        if (i < vec.size() - 1) {
            cout << vec[i] << ", ";
        }
        else {
            cout << vec[i];
        }
    }
    cout << "]";
}

/**
 * @brief Prints a node of a term using prefix notation
 * 
 * @param term Term
 * @param idx Index of the node to be printed
 */
void print_term_prefix_aux(Term term, int idx) {
    TermNode term_node = term[idx];

    cout << term_node.data;

    if (term_node.children.size() > 0) {
        cout << "(";
        for (int i = 0; i < term_node.children.size(); i++) {
            print_term_prefix_aux(term, term_node.children[i]);
            if (i < term_node.children.size() - 1) cout << ",";
        }
        cout << ")";
    }

}

/**
 * @brief Recursively prints a term using prefix notation
 * 
 * @param term Term to be printed
 */
void print_term_prefix(Term term) {
    print_term_prefix_aux(term, 0);
}

/**
 * @brief Prints a tableau in tree-format
 * 
 * @param tbl Tableau
 */
void print_tableau(Tableau tbl) {

    vector< vector<int>> branches = get_tbl_branches(tbl);

    // for (vector<int> branch : branches) {
    //     cout << "branch: ";
    //     print_vec_int(branch);
    //     cout << "\n";
    // }

    // BFS: getting the level of the nodes in tableau tree
    vector<int> tbl_level = get_tbl_levels(tbl);

    // DFS: pritting the tree
    stack<int> s;
    s.push(0);

    while (!s.empty()) {
        int current = s.top();

        for (int i = 0; i < tbl_level[current] + 1; i++) cout << "=";
        cout << " " << current << ": ";
        TblNode node = tbl[current];
        polarity sign = node.signed_fmla.sign;
        Fmla fmla = node.signed_fmla.fmla;
        if (sign == polarity::plus) cout << "+ ";
        if (sign == polarity::minus) cout << "- ";
        if (sign == polarity::plus || sign == polarity::minus) pretty_printing_fmla(fmla);
        else cout << fmla[0].data;
        if (node.justification_parents[0] != -1 && node.justification_parents[0] != -2) {
            cout << ", ";
            print_vec_int(node.justification_parents);
        }
        cout << "\n";

        s.pop();
        for (int i = tbl[current].tbl_children.size() - 1; i >= 0; i--) {
            int child = tbl[current].tbl_children[i]; 
            s.push(child);
        }
    }
}

/**
 * @brief Prints a tableau proof in tree-format
 * 
 * @param tbl Tableau proof
 * @param er Expansion rules
 */
void print_proof(Tableau tbl, vector<TblRule> er) {

    vector< vector<int>> branches = get_tbl_branches(tbl);

    // BFS: getting the level of the nodes in tableau tree
    vector<int> tbl_level = get_tbl_levels(tbl);

    // DFS: pritting the tree
    stack<int> s;
    s.push(0);

    while (!s.empty()) {
        int current = s.top();

        for (int i = 0; i < tbl_level[current] + 1; i++) cout << "=";
        cout << " " << current << ": ";
        TblNode node = tbl[current];
        polarity sign = node.signed_fmla.sign;
        Fmla fmla = node.signed_fmla.fmla;
        if (sign == polarity::plus) cout << "+ ";
        if (sign == polarity::minus) cout << "- ";
        if (sign == polarity::plus || sign == polarity::minus) pretty_printing_fmla(fmla);
        else cout << fmla[0].data;
        if (node.justification_parents[0] != -1 && node.justification_parents[0] != -2) {
            cout << ", ";
            print_vec_int(node.justification_parents);
        }
        cout << "\n";

        if (tbl[current].tbl_children.size() == 0) { // print the closure
            // if the branch is closed
            vector<int> resulting_branch = {};
            for (vector<int> branch : branches) {
                for (int node : branch) {
                    if (node == current) {
                        resulting_branch = branch;
                    }
                }
            }

            for (int i = 0; i < tbl_level[current] + 2; i++) cout << "=";
            cout << " *: ";

            vector<int> closure_nodes = branch_closure_nodes(resulting_branch, tbl, er);
            print_vec_int(closure_nodes);
            cout << "\n";

        }

        s.pop();
        for (int i = tbl[current].tbl_children.size() - 1; i >= 0; i--) {
            int child = tbl[current].tbl_children[i]; 
            s.push(child);
        }
    }
}

/**
 * @brief Prints a tableau as a list of nodes
 * 
 * @param tbl Tableau
 */
void print_tableau_as_list(Tableau tbl) {
    for (int i = 0; i < tbl.size(); i++){
        TblNode node = tbl[i];
        cout << i << ": ";
        SignedFmla sf = node.signed_fmla;
        if (sf.sign == polarity::plus) cout << "+ ";
        if (sf.sign == polarity::minus) cout << "- ";
        if (sf.sign == polarity::plus || sf.sign == polarity::minus) pretty_printing_fmla(sf.fmla);
        else cout << sf.fmla[0].data;
        cout << ", ";
        print_vec_int(node.justification_parents);
        cout << ", " << node.tbl_parent << ", ";
        print_vec_int(node.tbl_children);
        cout << "\n";
    }
}