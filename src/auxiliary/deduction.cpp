/**
 * @file deduction.cpp
 * @author João Mendes
 * @brief Deductive machinery for the generation of proof exercises of comparable complexity
 * @version 0.0.1
 * @date 2025-06-18
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "deduction.h"

using namespace std;


/**
 * @brief Gets an initial tableau for a vector of signed formulas
 * 
 * @param sf_input Vector of signed formulas
 * @return An initial tableau for `sf_input`
 */
Tableau get_initial_tableau(vector<SignedFmla> sf_input) {
    Tableau initial_tableau;

    int idx = 0;
    for (SignedFmla sf : sf_input) {
        TblNode tbl_node;

        tbl_node.signed_fmla = sf;
        tbl_node.justification_parents = {-1};

        if (idx == 0) {
            tbl_node.tbl_parent = 0;
        }
        else {
            tbl_node.tbl_parent = idx - 1;
            initial_tableau[idx - 1].tbl_children = {idx};
        }
        idx++;
        tbl_node.expansion_rule_idx = -1;
        tbl_node.conclusion_idx = -1;

        initial_tableau.push_back(tbl_node);
    }

    return initial_tableau;
}

/**
 * @brief Get the height initial nodes object
 * 
 * @param tbl Tableau
 * @return The maximum height of a formula in the initial segment of `tbl`  
 */
int get_height_initial_nodes(Tableau tbl) {
    vector<Fmla> fmlas;
    for (TblNode tbl_node : tbl) {
        if (tbl_node.justification_parents[0] == -1) {
            fmlas.push_back(tbl_node.signed_fmla.fmla);
        }
    }

    return get_height(fmlas);
}

/**
 * @brief Tries to apply a rule in a tableau
 * 
 * @param tbl Tableaux
 * @param expansion_rule Expansion rule to be applied in the tableau
 * @param rule_idx Index of expansion_rule in er
 * @param er Vector of expansion rules
 * @return If it is possible to apply `expansion_rule` in tbl, the tableau `tbl` after the application of `expansion_rule`
 * @return Otherwise, the tableau itself 
 */
Tableau apply_rule_with_premise(Tableau tbl, TblRule expansion_rule, int rule_idx, vector<TblRule> er) {
    vector<Tableau> resuling_tableaux;
    vector<SignedFmla> premises = expansion_rule.premises;
    vector<SignedFmla> conclusions = expansion_rule.conclusions;

    vector<vector<int>> branches = get_tbl_open_branches(tbl, er);

    Tableau new_tbl = tbl;

    int height_initial_nodes = get_height_initial_nodes(tbl);

    for (vector<int> branch : branches) {
        int nodes_added_amt = 0;
        int branch_size = branch.size();
        int premises_size = premises.size();
        if (branch_size >= premises_size) {
            vector<vector<int>> all_arranges = get_all_arranges(branch_size, premises_size);
            int arranges_amt = all_arranges.size();
            for (int i = 0; i < arranges_amt; i++) {
                vector<int> arrange = all_arranges[i];
                vector<int> justifications;

                for (int j = 0; j < premises.size(); j++) {
                    justifications.push_back(branch[arrange[j]]);
                }

                bool all_premises_match = true;
                for (int j = 0; j < premises.size(); j++) {
                    if (!is_a_match(tbl[justifications[j]].signed_fmla, premises[j])) all_premises_match = false;
                }

                if (all_premises_match) {
                    // check if all matching of premises agree on substitution of parameters
                    bool all_parameters_match = true;
                    Subst parameters_conclusion_subst;
                    
                    vector<Fmla> premises_fmlas;
                    for (SignedFmla premise : premises) {
                        premises_fmlas.push_back(premise.fmla);
                    }
                    set<string> premises_parameters = get_all_parameters(premises_fmlas);

                    for (int j = 0; j < premises.size(); j++) {
                        Subst matching_parameters_map = matching_parameters(tbl[justifications[j]].signed_fmla, premises[j]);
                        for (FmlaNode fmla_node : premises[j].fmla) {
                            if (is_a_parameter(fmla_node)) {
                                string parameter = fmla_node.data;
                                if (parameters_conclusion_subst.find(parameter) != parameters_conclusion_subst.end()) {
                                    if (matching_parameters_map.find(parameter) != matching_parameters_map.end()) {
                                        if (!term_equality(parameters_conclusion_subst[parameter], matching_parameters_map[parameter])) all_parameters_match = false;
                                    }
                                }
                                else {
                                    parameters_conclusion_subst[parameter] = matching_parameters_map[parameter];
                                }
                            }
                        }
                    }

                    // expand tableau
                    if (all_parameters_match) {
                        int leaf = branch_leaf(new_tbl, branch);
                        for (int j = 0; j < conclusions.size(); j++) {
                            SignedFmla conclusion = conclusions[j];
                            SignedFmla expansion_node(conclusion.sign, subst_extension(conclusion.fmla, parameters_conclusion_subst));
                            int expansion_node_height = get_height(expansion_node.fmla);
                            if (!node_in_branch(expansion_node, tbl, branch) && expansion_node_height <= height_initial_nodes + 4 && new_tbl.size() < 120){
                                // it is not necessary that we add all conclusions
                                if (nodes_added_amt > 0) {
                                    leaf = new_tbl.size() - 1;
                                }

                                TblNode new_tbl_node (expansion_node, justifications, leaf, {}, rule_idx, j);
                                
                                new_tbl[leaf].tbl_children.push_back(new_tbl.size());

                                nodes_added_amt += 1;
                                new_tbl.push_back(new_tbl_node);
                            }
                        }
                    }
                }
            }
        }
    }
    return new_tbl;
}

/**
 * @brief Checks if it is possible to apply a closure rule in a tableau
 * 
 * @param tbl Tableau
 * @param branch Branch of tbl in which the closure is tried to be applied
 * @param closure_rule Expansion closure rule
 * @return True if it is possible to apply the `expansion_rule`
 * @return False otherwise
 */
bool try_apply_closure_rule(Tableau tbl, vector<int> branch, TblRule closure_rule) {
    vector<Tableau> resuling_tableaux;
    vector<SignedFmla> premises = closure_rule.premises;

    vector<vector<int>> branches = get_tbl_branches(tbl);

    Tableau new_tbl = tbl;

    int branch_size = branch.size();
    int premises_size = premises.size();
    if (branch_size >= premises_size) {
        vector<vector<int>> all_arranges = get_all_arranges(branch_size, premises_size);
        int arranges_amt = all_arranges.size();
        for (int i = 0; i < arranges_amt; i++) {
            vector<int> arrange = all_arranges[i];
            vector<int> justifications;

            for (int j = 0; j < premises.size(); j++) {
                justifications.push_back(branch[arrange[j]]);
            }

            bool all_premises_match = true;
            for (int j = 0; j < premises.size(); j++) {
                if (!is_a_match(tbl[justifications[j]].signed_fmla, premises[j])) all_premises_match = false;
            }

            if (all_premises_match) {
                // check if all matching of premises agree on substitution of parameters
                bool all_parameters_match = true;
                Subst parameters_conclusion_subst;
                
                vector<Fmla> premises_fmlas;
                for (SignedFmla premise : premises) {
                    premises_fmlas.push_back(premise.fmla);
                }
                set<string> premises_parameters = get_all_parameters(premises_fmlas);

                for (int j = 0; j < premises.size(); j++) {
                    Subst matching_parameters_map = matching_parameters(tbl[justifications[j]].signed_fmla, premises[j]);
                    for (FmlaNode fmla_node : premises[j].fmla) {
                        if (is_a_parameter(fmla_node)) {
                            string parameter = fmla_node.data;
                            if (parameters_conclusion_subst.find(parameter) != parameters_conclusion_subst.end()) {
                                if (matching_parameters_map.find(parameter) != matching_parameters_map.end()) {
                                    if (!term_equality(parameters_conclusion_subst[parameter], matching_parameters_map[parameter])) all_parameters_match = false;
                                }
                            }
                            else {
                                parameters_conclusion_subst[parameter] = matching_parameters_map[parameter];
                            }
                        }
                    }
                }

                if (all_parameters_match) {
                    return true;
                }
            }
        }
    }
    return false;
}

/**
 * @brief Gets the index of the justifications for the application of a closure rule
 * 
 * @param tbl Tableau 
 * @param branch Branch of tbl
 * @param closure_rule Expansion closure rule
 * @return Vector of integers indicating the index in tbl of the justification for the application of `closure_rule` in `tbl`
 */
vector<int> get_justifications_closure_rule(Tableau tbl, vector<int> branch, TblRule closure_rule) {
    vector<Tableau> resuling_tableaux;
    vector<SignedFmla> premises = closure_rule.premises;

    vector<vector<int>> branches = get_tbl_branches(tbl);

    Tableau new_tbl = tbl;

    for (vector<int> branch : branches) {
        int branch_size = branch.size();
        int premises_size = premises.size();
        if (branch_size >= premises_size) {
            vector<vector<int>> all_arranges = get_all_arranges(branch_size, premises_size);
            int arranges_amt = all_arranges.size();
            for (int i = 0; i < arranges_amt; i++) {
                vector<int> arrange = all_arranges[i];
                vector<int> justifications;

                for (int j = 0; j < premises.size(); j++) {
                    justifications.push_back(branch[arrange[j]]);
                }

                bool all_premises_match = true;
                for (int j = 0; j < premises.size(); j++) {
                    if (!is_a_match(tbl[justifications[j]].signed_fmla, premises[j])) all_premises_match = false;
                }

                if (all_premises_match) {
                    // check if all matching of premises agree on substitution of parameters
                    bool all_parameters_match = true;
                    Subst parameters_conclusion_subst;
                    
                    vector<Fmla> premises_fmlas;
                    for (SignedFmla premise : premises) {
                        premises_fmlas.push_back(premise.fmla);
                    }
                    set<string> premises_parameters = get_all_parameters(premises_fmlas);

                    for (int j = 0; j < premises.size(); j++) {
                        Subst matching_parameters_map = matching_parameters(tbl[justifications[j]].signed_fmla, premises[j]);
                        for (FmlaNode fmla_node : premises[j].fmla) {
                            if (is_a_parameter(fmla_node)) {
                                string parameter = fmla_node.data;
                                if (parameters_conclusion_subst.find(parameter) != parameters_conclusion_subst.end()) {
                                    if (matching_parameters_map.find(parameter) != matching_parameters_map.end()) {
                                        if (!term_equality(parameters_conclusion_subst[parameter], matching_parameters_map[parameter])) all_parameters_match = false;
                                    }
                                }
                                else {
                                    parameters_conclusion_subst[parameter] = matching_parameters_map[parameter];
                                }
                            }
                        }
                    }

                    if (all_parameters_match) {
                        return justifications;
                    }
                }
            }
        }
    }
    return {};
}

/**
 * @brief Applies a cut rule in a tableau
 * 
 * @param tbl Tableau
 * @param er Vector of expansion rules
 * @return Vector of tableaux containing all possible application of cuts in `tbl` 
 */
vector<Tableau> apply_cut(Tableau tbl, vector<TblRule> er) {
    vector<Tableau> resuling_tableaux;

    vector<vector<int>> branches = get_tbl_open_branches(tbl, er);

    for (vector<int> branch : branches) {

        vector<SignedFmla> copremises = minor_copremises_branch(tbl, branch, er);

        int cuts_amt = 0;

        for (int branch_node_idx : branch) {
            TblNode branch_node = tbl[branch_node_idx];
            if (branch_node.justification_parents[0] == -2) {
                cuts_amt += 1;
            }
        }

        if (cuts_amt < 1) {
            for (SignedFmla signed_fmla : copremises) {
                SignedFmla expansion_node1(polarity::plus, signed_fmla.fmla);
                SignedFmla expansion_node2(polarity::minus, signed_fmla.fmla);

                if (!node_in_branch(expansion_node1, tbl, branch) && !node_in_branch(expansion_node2, tbl, branch)){

                    Tableau new_tbl = tbl;

                    int leaf = branch_leaf(tbl, branch);
                    TblNode new_tbl_node1(expansion_node1, {-2}, leaf, {}, -1, -1);
                    TblNode new_tbl_node2(expansion_node2, {-2}, leaf, {}, -1, -1);
                    
                    new_tbl[leaf].tbl_children.push_back(tbl.size());
                    new_tbl[leaf].tbl_children.push_back(tbl.size() + 1);

                    new_tbl.push_back(new_tbl_node1);
                    new_tbl.push_back(new_tbl_node2);
                    resuling_tableaux.push_back(new_tbl);
                }
            }
        }
    }

    return resuling_tableaux;
}

/**
 * @brief Checks if a signed formula and a premise of a rule are equal under substitution
 * 
 * @param sf_tbl Signed formula of a tableau
 * @param premise Signed formula of a premise of a rule
 * @return True if `sf_tbl` and `premise` are equal under substitution
 * @return False otherwise 
 */
bool is_a_match(SignedFmla sf_tbl, SignedFmla premise) {
    if (sf_tbl.sign != premise.sign) {
        return false;
    }

    if (sf_tbl.fmla.size() < premise.fmla.size()) {
        return false;
    }

    map<string, int> function_symbs = pre_process_function_symbs();
    map<string, int> predicate_symbs = pre_process_predicate_symbs();

    Subst subst;

    queue<int> sf_tbl_queue;
    queue<int> premise_queue;

    sf_tbl_queue.push(0);
    premise_queue.push(0);

    if (premise.fmla[0].data != sf_tbl.fmla[0].data) return false; // if the predicate symbols are different

    while (!premise_queue.empty()) {
        int premise_node_idx = premise_queue.front();
        int sf_tbl_node_idx = sf_tbl_queue.front();

        FmlaNode premise_node = premise.fmla[premise_node_idx];
        FmlaNode sf_tbl_node = sf_tbl.fmla[sf_tbl_node_idx];

        premise_queue.pop();
        sf_tbl_queue.pop();

        if (!is_a_parameter(premise_node)) {
            if (premise_node.children.size() != sf_tbl_node.children.size()) return false;
        }
        else { // is a parameter
            string parameter = premise_node.data;
            if (subst.find(parameter) == subst.end()) {
                subst[parameter] = get_term_of_fmla(sf_tbl.fmla, sf_tbl_node_idx);
            }
            else {
                if (!term_equality(subst[parameter], get_term_of_fmla(sf_tbl.fmla, sf_tbl_node_idx))) return false;
            }
        }

        for (int i = 0; i < premise_node.children.size(); i++) {
            premise_queue.push(premise_node.children[i]);
            sf_tbl_queue.push(sf_tbl_node.children[i]);
        }

        if (is_function_symb(premise_node.data, function_symbs) || is_predicate_symb(premise_node.data, predicate_symbs)) {
            if (premise_node.data != sf_tbl_node.data) return false;
        }
    }
    return true;
}

/**
 * @brief Checks if a formula and a formula of a premise of a rule are equal under substitution
 * 
 * @param fmla_tbl Formula of a signed formula of a tableau
 * @param premise_fmla Formula of signed formula of a premise of a rule
 * @return True if `fmla_tbl` and `premise_fmla` are equal under substitution
 * @return False otherwise 
 */
bool is_a_match(Fmla fmla_tbl, Fmla premise_fmla) {

    if (fmla_tbl.size() < premise_fmla.size()) {
        return false;
    }

    map<string, int> function_symbs = pre_process_function_symbs();
    map<string, int> predicate_symbs = pre_process_predicate_symbs();

    Subst subst;

    queue<int> fmla_tbl_queue;
    queue<int> premise_fmla_queue;

    fmla_tbl_queue.push(0);
    premise_fmla_queue.push(0);

    if (premise_fmla[0].data != fmla_tbl[0].data) return false; // if the predicate symbols are different

    while (!premise_fmla_queue.empty()) {
        int premise_node_idx = premise_fmla_queue.front();
        int fmla_tbl_node_idx = fmla_tbl_queue.front();

        FmlaNode premise_fmla_node = premise_fmla[premise_node_idx];
        FmlaNode fmla_tbl_node = fmla_tbl[fmla_tbl_node_idx];

        premise_fmla_queue.pop();
        fmla_tbl_queue.pop();

        if (!is_a_parameter(premise_fmla_node)) {
            if (premise_fmla_node.children.size() != fmla_tbl_node.children.size()) return false;
        }
        else { // is a parameter
            string parameter = premise_fmla_node.data;
            if (subst.find(parameter) == subst.end()) {
                subst[parameter] = get_term_of_fmla(fmla_tbl, fmla_tbl_node_idx);
            }
            else {
                if (!term_equality(subst[parameter], get_term_of_fmla(fmla_tbl, fmla_tbl_node_idx))) return false;
            }
        }

        for (int i = 0; i < premise_fmla_node.children.size(); i++) {
            premise_fmla_queue.push(premise_fmla_node.children[i]);
            fmla_tbl_queue.push(fmla_tbl_node.children[i]);
        }

        if (is_function_symb(premise_fmla_node.data, function_symbs) || is_predicate_symb(premise_fmla_node.data, predicate_symbs)) {
            if (premise_fmla_node.data != fmla_tbl_node.data) return false;
        }
    }
    return true;
}

/**
 * @brief Constructs the substitution that allows two signed formulas to match, ie, to be equal under substitution
 * 
 * @param sf_tbl Signed formula of a tableau
 * @param premise Signed formula of a premise of a rule
 * @return The substitution that make `sf_tbl` and `premise` to be equal signed formulas 
 */
Subst matching_parameters(SignedFmla sf_tbl, SignedFmla premise) {
    Subst matching_parameters_map;

    if (!is_a_match(sf_tbl, premise)) return matching_parameters_map;

    queue<int> sf_tbl_queue;
    queue<int> premise_queue;

    map<string, int> predicate_symbs = pre_process_predicate_symbs();
    map<string, int> function_symbs = pre_process_function_symbs();

    sf_tbl_queue.push(0);
    premise_queue.push(0);

    while (!premise_queue.empty()) {
        int premise_node_idx = premise_queue.front();
        int sf_tbl_node_idx = sf_tbl_queue.front();

        FmlaNode premise_node = premise.fmla[premise_node_idx];
        FmlaNode sf_tbl_node = sf_tbl.fmla[sf_tbl_node_idx];

        sf_tbl_queue.pop();
        premise_queue.pop();

        for (int i = 0; i < premise_node.children.size(); i++) {
            premise_queue.push(premise_node.children[i]);
            sf_tbl_queue.push(sf_tbl_node.children[i]);
        }

        if (is_a_parameter(premise_node)) {
            matching_parameters_map[premise_node.data] = get_term_of_fmla(sf_tbl.fmla, sf_tbl_node_idx);
        }
    }
    return matching_parameters_map;
}

/**
 * @brief Constructs the substitution that allows two formulas to match, ie, to be equal under substitution
 * 
 * @param fmla_tbl Formula of a tableau
 * @param premise_fmla Formula of a premise of a rule
 * @return The substitution that make `fmla_tbl` and `premise_fmla` to be equal formulas 
 */
Subst matching_parameters(Fmla fmla_tbl, Fmla premise_fmla) {
    Subst matching_parameters_map;

    if (!are_syntactically_isomorphic(fmla_tbl, premise_fmla)) return matching_parameters_map;

    queue<int> fmla_tbl_queue;
    queue<int> premise_fmla_queue;

    map<string, int> predicate_symbs = pre_process_predicate_symbs();
    map<string, int> function_symbs = pre_process_function_symbs();

    fmla_tbl_queue.push(0);
    premise_fmla_queue.push(0);

    while (!premise_fmla_queue.empty()) {
        int premise_fmla_node_idx = premise_fmla_queue.front();
        int fmla_tbl_node_idx = fmla_tbl_queue.front();

        FmlaNode premise_fmla_node = premise_fmla[premise_fmla_node_idx];
        FmlaNode fmla_tbl_node = fmla_tbl[fmla_tbl_node_idx];

        fmla_tbl_queue.pop();
        premise_fmla_queue.pop();

        for (int i = 0; i < premise_fmla_node.children.size(); i++) {
            premise_fmla_queue.push(premise_fmla_node.children[i]);
            fmla_tbl_queue.push(fmla_tbl_node.children[i]);
        }

        if (is_a_parameter(premise_fmla_node)) {
            matching_parameters_map[premise_fmla_node.data] = get_term_of_fmla(fmla_tbl, fmla_tbl_node_idx);
        }
    }
    return matching_parameters_map;
}

/**
 * @brief Gets the leaves of a tableau
 * 
 * @param tbl Tableau
 * @return A vector of integers indicating the leaves of `tbl` 
 */
vector<int> get_tbl_leaves(Tableau tbl) {
    vector<int> leaves_idx;
    for (int i = 0; i < tbl.size(); i++) {
        if (tbl[i].tbl_children.size() == 0) {
            leaves_idx.push_back(i);
        }
    }
    return leaves_idx;
}

/**
 * @brief Gets the branches of a tableau
 * 
 * @param tbl Tableau
 * @return A vector of vector of integers representing the branches of `tbl` in which each vector of intergers represents a single branch 
 */
vector<vector<int>> get_tbl_branches(Tableau tbl) {
    vector<int> leaves = get_tbl_leaves(tbl);

    vector< vector<int>> branches;

    for (int i = 0; i < leaves.size(); i++){
        vector<int> branch;
        if (leaves[i] != 0) {
            branch.push_back(leaves[i]);
            int parent = tbl[leaves[i]].tbl_parent;
            while (parent != 0) {
                branch.push_back(parent);
                parent = tbl[parent].tbl_parent;
            }
        }
        branch.push_back(0);
        branches.push_back(branch);
    }

    return branches;
}

/**
 * @brief Gets the index of leaf node of a tableau branch
 * 
 * @param tbl Tableau
 * @param branch Branch of tbl
 * @return The index of leaf of `branch` 
 */
int branch_leaf(Tableau tbl, vector<int> branch) {
    for (int branch_node_idx : branch) {
        if (tbl[branch_node_idx].tbl_children.size() == 0){
            return branch_node_idx;
        }
    }
    return -1;
}

/**
 * @brief Gets the level of each node of a tableau tree
 * 
 * @param tbl Tableau
 * @return A vector indicating the level of each node of tbl in which the n-th element indicates the level of the n-th node of `tbl` 
 */
vector<int> get_tbl_levels(Tableau tbl) {

    vector<int> tbl_level(tbl.size(), 0);

    queue<int> q;
    q.push(0);

    while (!q.empty()) {
        int current = q.front();

        if (current != 0) tbl_level[current] = tbl_level[tbl[current].tbl_parent] + 1;

        q.pop();
        for (int child : tbl[current].tbl_children) {
            q.push(child);
        }
    }

    return tbl_level;

}

/**
 * @brief Check if two nodes of a tableau have opposite polarity
 * 
 * @param node1 Node of a tableau
 * @param node2 Node of a tableau
 * @return True if `node1` and `node2` have opposite polarity
 * @return False otherwise
 */
bool opposite_polarity_nodes(TblNode node1, TblNode node2) {
    if (node1.signed_fmla.sign == polarity::plus && node2.signed_fmla.sign == polarity::minus) return true;

    if (node1.signed_fmla.sign == polarity::minus && node2.signed_fmla.sign == polarity::plus) return true;

    return false;
}

/**
 * @brief Checks if a branch is closed
 * 
 * @param branch Branch of tbl
 * @param tbl Tableau
 * @param er Expansion rules
 * @return True if `branch` is closed
 * @return False otherwise 
 */
bool branch_is_closed(vector<int> branch, Tableau tbl, vector<TblRule> er) {
    // Closure by conjugate signed formulas
    for (int i = 0; i < branch.size(); i++) {
        for (int j = 0; j < branch.size(); j++) {
            int node1 = branch[i];
            int node2 = branch[j];
            if (fmla_equality(tbl[node1].signed_fmla.fmla, tbl[node2].signed_fmla.fmla) == true && opposite_polarity_nodes(tbl[node1], tbl[node2]) == true) {
                return true;
            }
        }
    }

    // Closure by application of a closure rule
    vector<TblRule> closure_rules = get_closure_rules(er);
    for (TblRule closure_rule : closure_rules) {
        if (try_apply_closure_rule(tbl, branch, closure_rule)) {
            return true;
        }
    }

    return false;
}

/**
 * @brief Gets the closure rules of a vector of expansion rules
 * 
 * @param er Expansion rules
 * @return Closure rules of `er`
 */
vector<TblRule> get_closure_rules(vector<TblRule> er) {
    vector<TblRule> closure_rules;

    for (TblRule rule : er) {
        if (rule.conclusions.size() == 0) {
            closure_rules.push_back(rule);
        }
    }

    return closure_rules;
}

/**
 * @brief Gets the open branches of a tableau
 * 
 * @param tbl Tableau
 * @param er Expansion rules
 * @return Vector of vector of integers representing the open branches of `tbl`  
 */
vector< vector<int>> get_tbl_open_branches(Tableau tbl, vector<TblRule> er) {

    vector<vector<int>> open_branches;
    vector<vector<int>> all_branches = get_tbl_branches(tbl);

    for (vector<int> branch : all_branches) {
        if (!branch_is_closed(branch, tbl, er)) open_branches.push_back(branch);
    }

    return open_branches;
}

/**
 * @brief Checks if a tableau is closed
 * 
 * @param tbl Tableau
 * @param er Expansion rules
 * @return True if `tbl` is closed
 * @return False otherwise
 */
bool tbl_is_closed(Tableau tbl, vector<TblRule> er) {
    vector< vector<int>> branches = get_tbl_branches(tbl);

    bool is_closed = true;
    for (int i = 0; i < branches.size(); i++){
        is_closed = is_closed && branch_is_closed(branches[i], tbl, er); 
    }

    return is_closed;
}

/**
 * @brief Gets all terms of a tableau branch
 * 
 * @param tbl Tableau
 * @param branch Branch of tbl
 * @return Vector of terms indicating the terms present in `branch` 
 */
vector<Term> get_all_terms_of_branch(Tableau tbl, vector<int> branch) {
    vector<Term> all_terms;

    for (int i : branch) {
        TblNode tbl_node = tbl[i];
        Fmla fmla = tbl_node.signed_fmla.fmla;
        vector<Term> terms_of_fmla = get_all_terms_of_fmla(fmla);

        for (Term term_fmla : terms_of_fmla) {
            bool already_in_all_terms = false;
            for (Term term : all_terms) {
                if (term_equality(term, term_fmla)){
                    already_in_all_terms = true;
                }
            }
            if (!already_in_all_terms) {
                all_terms.push_back(term_fmla);
            }
        }
    }

    return all_terms;
}

/**
 * @brief Checks if a tableau has a node that is a justification for a 1-premise rule
 * 
 * @param tbl Tableau
 * @param er Expasnion rules
 * @return True if there is a node in tbl that is a a justification for a 1-premise rule of er
 * @return False otherwise 
 */
bool has_single_justification_nodes(Tableau tbl, vector<TblRule> er) {
    for (int i = 0; i < er.size(); i++) {
        TblRule rule = er[i];
        if (rule.premises.size() == 1) {
            Tableau tbl_successor_rule = apply_rule_with_premise(tbl, rule, i, er);
            if (tbl_successor_rule.size() > tbl.size()) return true;
        }
    }
    return false;
}

/**
 * @brief Applies all possible single premise rules in a tableau
 * 
 * @param tbl Tableau
 * @param er Expansion rules
 * @return The resulting tableau of applying in `tbl` all possible single premise rules of `er`
 */
Tableau apply_single_premise_rules(Tableau tbl, vector<TblRule> er) {
    for (int i = 0; i < er.size(); i++) {
        TblRule rule = er[i];
        if (rule.premises.size() == 1) {
            tbl = apply_rule_with_premise(tbl, rule, i, er);
        }
    }
    return tbl;
}

/**
 * @brief Gets the successor tableaux of a tableau
 * 
 * @param tbl Tableaux
 * @param er Expansion rules
 * @return Vector of tableaux representing the successor tableaux of tbl 
 */
vector<Tableau> get_tbl_successors(Tableau tbl, vector<TblRule> er) {
    vector<Tableau> tableau_successors;

    // get tableau successors from linear rules
    int successors_amt = 0;
    for (int i = 0; i < er.size(); i++) {
        TblRule rule = er[i];
        if (rule.premises.size() > 1) {
            Tableau tbl_successor_rule = apply_rule_with_premise(tbl, rule, i, er);
            if (tbl_successor_rule.size() > tbl.size()) {
                successors_amt += 1;
                tableau_successors.push_back(tbl_successor_rule);
            }
        }
    }

    if (successors_amt == 0) { // tableau is already saturated and we need to resort to 0-premise rules
        vector<Tableau> tbl_successors_cut = apply_cut(tbl, er);
        
        for (Tableau tbl_successor : tbl_successors_cut) {
            tableau_successors.push_back(tbl_successor);
        }
    }

    return tableau_successors;

}

/**
 * @brief Gets the indexes of the nodes that make a branch to close
 * 
 * @param branch Branch of tbl
 * @param tbl Tableau
 * @param er Expansion rules
 * @return Vector of integers indicating the indexes that make `branch` to close 
 */
vector<int> branch_closure_nodes(vector<int> branch, Tableau tbl, vector<TblRule> er) {
    for (int i = 0; i < branch.size(); i++) {
        for (int j = 0; j < branch.size(); j++) {
            int node1 = branch[i];
            int node2 = branch[j];
            if (fmla_equality(tbl[node1].signed_fmla.fmla, tbl[node2].signed_fmla.fmla) == true && opposite_polarity_nodes(tbl[node1], tbl[node2]) == true) {
                return {node1, node2};
            }
        }
    }

    vector<TblRule> closure_rules = get_closure_rules(er);
    for (TblRule closure_rule : closure_rules) {
        if (try_apply_closure_rule(tbl, branch, closure_rule)) {
            return get_justifications_closure_rule(tbl, branch, closure_rule);
        }
    }

    return {};
}

/**
 * @brief Gets the size of a branch of a tableau
 * 
 * @param tbl Tableau
 * @param branch Branch of tbl
 * @param er Expansion rules
 * @return The size of `branch` 
 */
int get_branch_size(Tableau tbl, vector<int> branch, vector<TblRule> er) {
    int branch_size = 0;
    vector<int> closure_nodes = branch_closure_nodes(branch, tbl, er);

    for (int node : closure_nodes) {
        queue<int> q;
        q.push(node);

        while (!q.empty()) {
            int current = q.front();
            branch_size += 1;
            q.pop();

            for (int justification : tbl[current].justification_parents) {
                if (justification == 0) {
                    if (current > 0) q.push(justification);
                }
                if (justification > 0) q.push(justification);
            }
        }
    }    

    return branch_size + 1; // '+ 1' so we count the closure
}

/**
 * @brief Gets the size of a tableau
 * 
 * @param tbl Tableau
 * @param er Expansion rules
 * @return The size of `tbl` 
 */
int get_size(Tableau tbl, vector<TblRule> er) {
    int size = 0;
    vector<vector<int>> branches = get_tbl_branches(tbl);

    for (vector<int> branch : branches) {
        size += get_branch_size(tbl, branch, er);
    }

    return size;
}

/**
 * @brief Searches for the minimal proofs for a set of signed formulas
 * 
 * @param sf Signed forfmulas
 * @param er Expansion rules
 * @return The minimal `er`-proofs for `sf`
 */
vector<Tableau> search_minimal_proofs(vector<SignedFmla> sf, vector<TblRule> er) {
    queue<tuple<Tableau, int>> aux_queue;

    Tableau initial_tbl = get_initial_tableau(sf);
    aux_queue.push(make_tuple(initial_tbl, 1));
    vector<Tableau> minimal_proofs;
    int minimal_proof_size = -1;

    while (!aux_queue.empty()) {

        tuple<Tableau, int> current = aux_queue.front();
        Tableau tbl = get<0>(current);
        int level = get<1>(current);
        aux_queue.pop();

        if (tbl_is_closed(tbl, er)) {
            Tableau tbl_clean = clean_tbl(tbl, er);
            int size = get_size(tbl_clean, er);
            if (minimal_proofs.size() == 0) {
                minimal_proof_size = size;
                minimal_proofs.push_back(tbl_clean);
            }
            else {
                if (size == minimal_proof_size) {
                    minimal_proofs.push_back(tbl_clean);
                }
                if (size < minimal_proof_size) {
                    minimal_proof_size = size;
                    minimal_proofs.clear();
                    minimal_proofs.push_back(tbl_clean);
                }
            }
        }
        else {
            if (minimal_proofs.size() == 0 || level < minimal_proof_size + 1) {
                if (has_single_justification_nodes(tbl, er)) {
                    Tableau tbl_child = apply_single_premise_rules(tbl, er);
                    aux_queue.push(make_tuple(tbl_child, level + 1));
                }
                else {
                    vector<Tableau> tbl_children = get_tbl_successors(tbl, er);
                    for (Tableau tbl_child : tbl_children) {
                        if (tbl_child.size() < 120) {
                            aux_queue.push(make_tuple(tbl_child, level + 1));
                        }
                    }
                }
            }
        }
    }

    return minimal_proofs;
}

/**
 * @brief Gets the clean proof for a proof
 * 
 * @param tbl Tableau proof
 * @param er Expansion rules
 * @return Clean proof for `tbl` 
 */
Tableau clean_tbl(Tableau tbl, vector<TblRule> er) {
    map<int, bool> is_dispensable;

    for (int i = 0; i < tbl.size(); i++){
        is_dispensable[i] = true;
    }

    vector<vector<int>> branches = get_tbl_branches(tbl);

    for (vector<int> branch : branches) {
        vector<int> closure_nodes = branch_closure_nodes(branch, tbl, er);

        for (int node : closure_nodes) {
            queue<int> q;
            q.push(node);

            while (!q.empty()) {
                int current = q.front();
                q.pop();

                is_dispensable[current] = false;

                for (int justification : tbl[current].justification_parents) {
                    if (justification > 0) q.push(justification);
                }
            }
        }
    }

    Tableau resulting_tbl = tbl;
    for (int i = resulting_tbl.size() - 1; i > 0; i--){
        if (resulting_tbl[i].justification_parents.size() > 0) {
            if (resulting_tbl[i].justification_parents[0] != -1 && is_dispensable[i]) {
                resulting_tbl = remove_node(resulting_tbl, i);
            }
        }
    }

    return resulting_tbl;
}

/**
 * @brief Removes a node of a tableau
 * 
 * @param tbl Tableau
 * @param idx Index of the node to be removed
 * @return Tableau with the node of index `idx` removed
 */
Tableau remove_node(Tableau tbl, int idx) {
    vector<int> tbl_children_idx = tbl[idx].tbl_children;
    int tbl_parent_idx = tbl[idx].tbl_parent;

    tbl[tbl_parent_idx].tbl_children = tbl_children_idx;
    for (int child : tbl_children_idx) {
        tbl[child].tbl_parent = tbl_parent_idx;
    }

    tbl.erase(tbl.begin() + idx);

    for (int i = 0; i < tbl.size(); i++) {
        TblNode tbl_node = tbl[i];

        vector<int> justification_parents = tbl_node.justification_parents;
        for (int j = 0; j < justification_parents.size(); j++) {
            if (justification_parents[j] >= idx) {
                justification_parents[j] = justification_parents[j] - 1;
            }

        }
        tbl_node.justification_parents = justification_parents;

        if (tbl_node.tbl_parent >= idx) {
            tbl_node.tbl_parent = tbl_node.tbl_parent - 1;
        }

        vector<int> tbl_children = tbl_node.tbl_children;
        for (int j = 0; j < tbl_children.size(); j++) {
            if (tbl_children[j] >= idx) {
                tbl_children[j] = tbl_children[j] - 1;
            }

        }
        tbl_node.tbl_children = tbl_children;

        tbl[i] = tbl_node;
    }

    return tbl;
}

/**
 * @brief Gets the minor co-premises of a branch
 * 
 * @param tbl Tableau
 * @param branch Branch of tbl
 * @param er Expansion rules
 * @return Vector of signed formulas of the minor co-premises of the `branch` 
 */
vector<SignedFmla> minor_copremises_branch(Tableau tbl, vector<int> branch, vector<TblRule> er) {
    vector<SignedFmla> resulting_nodes; 

    vector<Term> branch_terms = get_all_terms_of_branch(tbl, branch);
    set<string> branch_pred_symbols;

    for (int branch_node : branch) {
        TblNode tbl_node = tbl[branch_node];
        SignedFmla signed_fmla = tbl_node.signed_fmla;
        branch_pred_symbols.insert(signed_fmla.fmla[0].data);    
    }


    for (int branch_node : branch) {
        TblNode tbl_node = tbl[branch_node];
        SignedFmla signed_fmla = tbl_node.signed_fmla;
        for (TblRule rule : er) {
            int premise_size = rule.premises.size();
            if (premise_size > 1) {
                vector<SignedFmla> copremises = minor_copremises_rule(signed_fmla, rule);
                
                for (SignedFmla copremise : copremises) {
                    vector<Term> premise_terms = get_all_terms_of_fmla(copremise.fmla);

                    bool all_premises_terms_in_branch = true;
                    for (Term premise_term : premise_terms) {
                        bool all_single_premise_terms_in_branch = false;
                        for (Term branch_term : branch_terms) {
                            if (term_equality(premise_term, branch_term)) {
                                all_single_premise_terms_in_branch = true;
                            }
                        }
                        all_premises_terms_in_branch &= all_single_premise_terms_in_branch;
                    }

                    if (all_premises_terms_in_branch == true) {
                        if (branch_pred_symbols.find(copremise.fmla[0].data) != branch_pred_symbols.end()) {
                            if (!fmla_in_vec_signed_fmla(resulting_nodes, copremise.fmla)) {
                                resulting_nodes.push_back(copremise);
                            }
                        }
                    }
                }
            }
        }
    }
    return resulting_nodes;
}

/**
 * @brief Gets the minor co-premises of a rule
 * 
 * @param sf Signed formula
 * @param rule Expansion rule
 * @return Vector of signed formulas of the minor co-premises of `rule` 
 */
vector<SignedFmla> minor_copremises_rule(SignedFmla sf, TblRule rule) {
    vector<SignedFmla> resulting_copremises;
    
    for (int i = 0; i < rule.premises.size(); i++){
        SignedFmla premise = rule.premises[i];
        if (is_a_match(sf, premise) && sf.sign == premise.sign) {
            Subst matching_parameters_map = matching_parameters(sf, premise);
            for (int j = 0; j < rule.premises.size(); j++) {
                if (i != j) {
                    set<string> prem_parameters = get_all_parameters_of_fmla(rule.premises[j].fmla);

                    bool parameters_are_in_subst = true;
                    for (string prem_param : prem_parameters) {
                        if (matching_parameters_map.find(prem_param) == matching_parameters_map.end()) parameters_are_in_subst = false;
                    }
                    
                    if (parameters_are_in_subst) {
                        Fmla fmla = subst_extension_copremise(rule.premises[j].fmla, matching_parameters_map);
                        SignedFmla sf(rule.premises[j].sign, fmla);
                        resulting_copremises.push_back(sf);
                    }
                }
            }
        }
    }

    return resulting_copremises;
}

/**
 * @brief Checks if a formula is in a vector of formulas
 * 
 * @param fmlas Vector of formulas
 * @param fmla Formula
 * @return True if `fmla` is in `fmlas` 
 * @return False otherwise
 */
bool fmla_in_vec_fmla(vector<Fmla> fmlas, Fmla fmla) {
    for (Fmla vec_fmla : fmlas) {
        if (fmla_equality(vec_fmla, fmla)) return true;
    }
    return false;
}

/**
 * @brief Checks if a formula is in a formula of a vector of signed formulas
 * 
 * @param signed_fmlas Vector of signed formulas
 * @param fmla Formula
 * @return True if `fmla` is in a formula of `signed_fmlas`
 * @return False otherwise
 */
bool fmla_in_vec_signed_fmla(vector<SignedFmla> signed_fmlas, Fmla fmla) {
    vector<Fmla> vec_fmla;
    
    for (SignedFmla signed_fmla : signed_fmlas) {
        vec_fmla.push_back(signed_fmla.fmla);
    }
    return fmla_in_vec_fmla(vec_fmla, fmla);
}

/**
 * @brief Checks if a signed formula is in a vector of signed formula
 * 
 * @param signed_fmlas Vector of signed formulas
 * @param sf Signed formula
 * @return True if `sf` is in `signed_fmlas` 
 * @return False otherwise
 */
bool signed_fmla_in_vec_signed_fmla(vector<SignedFmla> signed_fmlas, SignedFmla sf) {

    for (SignedFmla vec_sf : signed_fmlas) {
        if (fmla_equality(vec_sf.fmla, sf.fmla) && vec_sf.sign == sf.sign) return true;
    }
    return false;
}

/**
 * @brief Checks if a signed formula is in a branch of a tableau
 * 
 * @param sf Signed formula
 * @param tbl Tableau
 * @param branch Branch
 * @return True if `sf` is in `branch`
 * @return False otherwise 
 */
bool node_in_branch(SignedFmla sf, Tableau tbl, vector<int> branch) {
    Fmla fmla1 = sf.fmla;
    for (int node_idx : branch) {
        SignedFmla sf_tbl = tbl[node_idx].signed_fmla;
        Fmla fmla2 = sf_tbl.fmla;
        if (fmla_equality(fmla1, fmla2) && sf.sign == sf_tbl.sign) return true;
    }
    return false;
}

/**
 * @brief Searches for proof-isomorphic sets of signed formulas
 * 
 * @param tbl Tableau
 * @param er Expansion rules
 * @return Vector of vector of signed formulas representing the set of proof-isomorphic sets of signed formulas to the nodes of the initial segment of `tbl` 
 */
vector<vector<SignedFmla>> search_proof_isomorphic_sf_sets(Tableau tbl, vector<TblRule> er) {
    vector<vector <SignedFmla>> resulting_sf_sets;
    vector<SignedFmla> initial_sf = get_initial_sf(tbl);

    map<pair<int,int>, set<string>> ps_candidate_symbols = get_ps_candidate_symbols(tbl, er);

    vector<vector<SignedFmla>> sf_candidates = get_sf_candidates(initial_sf, ps_candidate_symbols);

    for (vector<SignedFmla> sf_candidate : sf_candidates) {

        if (is_proof_isomorphic_sf_set(tbl, er, sf_candidate)) {
            if (!vec_sf_in_vec_vec_sf(resulting_sf_sets, sf_candidate)) {
                resulting_sf_sets.push_back(sf_candidate);
            }
        }
    }
    return resulting_sf_sets;
}

/**
 * @brief Gets the proof-isomorphic candidate symbols of the nodes in the initial segment of a tableau
 * 
 * @param tbl Tableau
 * @param er Expansion rules
 * @return A map indicating the proof-isomorphic candidate symbols of the symbols of the formulas in the initial segment of `tbl`
 */
map<pair<int,int>, set<string>> get_ps_candidate_symbols(Tableau tbl, vector<TblRule> er) {
    map<pair<int,int>, set<string>> candidate_symbs;

    queue<pair<int, vector<tuple<int, pair<int, int>>>>> q;
    // n := q.first
    // vec := q.second
    // p := vec[i].first
    // (x,y) := vec[i].second
    // the p-th symbol of the n-th node in tbl is a root of a subterm of the initial tableau of tbl. (x,y) is the reference for the symbol in the initial tableau

    for(int i = 0; i < tbl.size(); i++) {
        TblNode tbl_node = tbl[i];
        if (tbl_node.justification_parents[0] == -1) {
            Fmla fmla = tbl_node.signed_fmla.fmla;
            vector<tuple<int, pair<int, int>>> vec;
            for (int j = 0; j < fmla.size(); j++) {
                FmlaNode fmla_node = fmla[j];
                if (is_predicate_symb(fmla_node.data) || is_function_symb(fmla_node.data)) {
                    vec.push_back(make_tuple(j, make_pair(i, j)));
                }
            }
            q.push(make_pair(i, vec));
        }
    }

    while(!q.empty()) {
        pair<int, vector<tuple<int, pair<int, int>>>> current = q.front();
        int node_idx = current.first;
        vector<tuple<int, pair<int, int>>> current_vec = current.second;

        q.pop();

        vector<int> expansion_nodes;

        for(int i = 0; i < tbl.size(); i++) {
            TblNode tbl_node = tbl[i];
            bool expansion_node_found = false;
            for (int j : tbl_node.justification_parents) {
                if (j == node_idx) {
                    expansion_node_found = true;
                }
            }

            if(expansion_node_found) {
                expansion_nodes.push_back(i);
            }
        }

        for (int exp_idx : expansion_nodes) {
            // getting the premise idx of the expansion node in the rule
            int premise_idx = -1;
            vector<int> justifications = tbl[exp_idx].justification_parents;
            for (int just_idx = 0; just_idx < justifications.size(); just_idx++) {
                if (justifications[just_idx] == node_idx) {
                    premise_idx = just_idx;
                }
            }
            TblRule rule = er[tbl[exp_idx].expansion_rule_idx];

            // processing the expansion node
            int conc_idx = tbl[exp_idx].conclusion_idx;
            vector<SignedFmla> vec_matching_premise = pattern_matching_premises(er, rule, premise_idx, conc_idx);
            Fmla current_fmla = tbl[node_idx].signed_fmla.fmla;
            for (int i = 0; i < current_vec.size(); i++) {
                int symb_idx = get<0>(current_vec[i]);
                string symb = current_fmla[symb_idx].data;
                if (is_not_skolem_symb(symb)) {
                    for (SignedFmla signed_fmla : vec_matching_premise) {
                        string matching_symb = get_syntactic_matching_symb(current_fmla, symb_idx, signed_fmla.fmla);
                        if (!matching_symb.empty()) {
                            int x = get<1>(current_vec[i]).first;
                            int y = get<1>(current_vec[i]).second;

                            candidate_symbs[{x,y}].insert(matching_symb);
                        }
                    }
                }
            }

            // adding nodes in q
            Fmla prem_fmla = rule.premises[premise_idx].fmla;
            Fmla conc_fmla = rule.conclusions[tbl[exp_idx].conclusion_idx].fmla;
            Fmla justf_fmla = tbl[node_idx].signed_fmla.fmla;
            Fmla exp_fmla = tbl[exp_idx].signed_fmla.fmla;
            vector<tuple<int, pair<int, int>>> vec_symb_descendant_occurrences = get_vec_symb_descendant_occurrences(prem_fmla, conc_fmla, justf_fmla, exp_fmla, current_vec);

            if (vec_symb_descendant_occurrences.size() > 0) {
                q.push({exp_idx, vec_symb_descendant_occurrences});
            }
        }
    }

    return candidate_symbs;
    
}

/**
 * @brief Gets the direct descendant occurrences of the occurrence of a symbol
 * 
 * @param prem_fmla Formula
 * @param conc_fmla Formula
 * @param justf_fmla Formula
 * @param exp_fmla Formula
 * @param ps_symbs 
 * @return TODO
 */
vector<tuple<int, pair<int, int>>> get_vec_symb_descendant_occurrences(Fmla prem_fmla, Fmla conc_fmla, Fmla justf_fmla, Fmla exp_fmla, vector<tuple<int, pair<int, int>>> ps_symbs) {
    vector<tuple<int, pair<int, int>>> result;

    set<int> prem_param_idxs = get_parameters_idxs(prem_fmla);

    for(int prem_param_idx : prem_param_idxs) {
        int term_justf_idx = get_term_idx_img_subst(prem_fmla, prem_param_idx, justf_fmla);
        if (!is_a_parameter(justf_fmla[prem_param_idx])) {
            set<int> conc_param_idxs;
            string curr_param = prem_fmla[prem_param_idx].data;
            for (int i = 0; i < conc_fmla.size(); i++) {
                if (conc_fmla[i].data == curr_param) {
                    conc_param_idxs.insert(i);
                }
            }
            for (int conc_param_idx : conc_param_idxs) {
                int term_conc_idx = get_term_idx_img_subst(conc_fmla, conc_param_idx, exp_fmla);
                queue<int> q_justf, q_exp;
                q_justf.push(term_justf_idx);
                q_exp.push(term_conc_idx);

                while(!q_justf.empty()) {
                    int curr_justf = q_justf.front();
                    int curr_exp = q_exp.front();

                    q_justf.pop();
                    q_exp.pop();

                    if (is_not_skolem_symb(justf_fmla[curr_justf].data)){
                        pair<int, int> p;
                        for (int i = 0; i < ps_symbs.size(); i++) {
                            if (get<0>(ps_symbs[i]) == curr_justf) {
                                p = get<1>(ps_symbs[i]);
                            }
                        }
                        result.push_back({curr_exp, p});
                    }
                    for (int i = 0; i < justf_fmla[curr_justf].children.size(); i++) {
                        q_justf.push(justf_fmla[curr_justf].children[i]);
                        q_exp.push(exp_fmla[curr_exp].children[i]);
                    }
                }
            }
        }
    }

    return result;
}

/**
 * @brief Gets the proof-isomorphic candidates set of signed formulas
 * 
 * @param initial_sf Signed formulas
 * @param ps_candidate_symbols Candidate symbolss
 * @return Vector indicating the proof-isomorphic candidate sets of signed formulas 
 */
vector<vector<SignedFmla>> get_sf_candidates(vector<SignedFmla> initial_sf, map<pair<int,int>, set<string>> ps_candidate_symbols) {

    vector<vector<SignedFmla>> sf_candidates;

    for(int i = 0; i < initial_sf.size(); i++) {
        SignedFmla signed_fmla = initial_sf[i];
        Fmla fmla = signed_fmla.fmla;
        for (int j = 0; j < fmla.size(); j++) {
            FmlaNode fmla_node = fmla[j];
            int arity = fmla_node.children.size();
            if (ps_candidate_symbols.find({i, j}) == ps_candidate_symbols.end()) {
                set<string> symbols;
                map<string, int> language_symbs;
                bool is_a_param = true;
                if (j == 0) { // is a predicate symbol
                    language_symbs = pre_process_predicate_symbs();
                    is_a_param = false;
                }
                else { // is a function symbol
                    if (!is_a_parameter(fmla_node)) {
                        is_a_param = false;
                        map<string, int> no_skolem_symbs = pre_process_no_skolem_symbs();

                        for (const auto& [key, value] : no_skolem_symbs) {
                            if (is_function_symb(key)) {
                                language_symbs[key] = value;
                            }
                        }
                    }
                }
                for (const auto& [key, value] : language_symbs) {
                    if (value == arity) {
                        symbols.insert(key);
                    }
                }
                if (!is_a_param) {
                    ps_candidate_symbols[{i,j}] = symbols;
                }
            }
        }
    }

    // count amount of combinations
    int signed_fmlas_amt = initial_sf.size();

    vector<int> signs_mask(signed_fmlas_amt, 1); // start with a vector of 1's so in 1st iteration, they are all set to 0

    // processing symbols combinations
    vector<pair<int, int>> map_no_skolem_symb;
    vector<int> base_no_skolem_symbs;
    map<pair<int,int>, vector<string>> ps_candidate_symbols_vec;
    for (const auto& [key, value] : ps_candidate_symbols) {
        map_no_skolem_symb.push_back(key);
        base_no_skolem_symbs.push_back(value.size());
        ps_candidate_symbols_vec[key] = {};
        for (string symb : ps_candidate_symbols[key]) {
            ps_candidate_symbols_vec[key].push_back(symb);
        }
    }
    int no_skolem_symbs_amt = ps_candidate_symbols.size();

    vector<int> theory_symbs_mask;
    long int combinations_symbs = 1;

    for (int j = 0; j < base_no_skolem_symbs.size(); j++) {
        combinations_symbs *= base_no_skolem_symbs[j];
        theory_symbs_mask.push_back(base_no_skolem_symbs[j] - 1);
    }

    for (int j = 0; j < combinations_symbs; j++) {
        vector<SignedFmla> filled_cs = initial_sf;
        theory_symbs_mask = increment_arrange_repitition_mask(theory_symbs_mask, base_no_skolem_symbs);
        for (int k = 0; k < no_skolem_symbs_amt; k++) {
            int sf_node_idx = map_no_skolem_symb[k].first;
            int fmla_node_idx = map_no_skolem_symb[k].second;
            // getting the correspondent fmla_node we want to manipulate
            SignedFmla sf = filled_cs[sf_node_idx];
            FmlaNode fmla_node = sf.fmla[fmla_node_idx];

            fmla_node.data = ps_candidate_symbols_vec[{sf_node_idx, fmla_node_idx}][theory_symbs_mask[k]];

            sf.fmla[fmla_node_idx] = fmla_node;
            filled_cs[sf_node_idx] = sf;
        }
        sf_candidates.push_back(filled_cs);
    }
    return sf_candidates;
}

/**
 * @brief Checks if two vector of signed formulas are
 * 
 * @param vec_sf1 Vector of signed formulas
 * @param vec_sf2 Vector of signed formulas
 * @return True if vec_sf1 is equal to vec_sf2
 * @return False otherwise
 */
bool vec_sf_equality(vector<SignedFmla> vec_sf1, vector<SignedFmla> vec_sf2) {
    if (vec_sf1.size() != vec_sf2.size()) return false;

    for (int i = 0; i < vec_sf1.size(); i++) {
        SignedFmla sf1 = vec_sf1[i];
        SignedFmla sf2 = vec_sf2[i];
        if (sf1.sign != sf2.sign || !fmla_equality(sf1.fmla, sf2.fmla)) return false;
    }
    return true;
}

/**
 * @brief Checks if a vector of signed formulas is in a vector of vector of signed formulas
 * 
 * @param vec_vec_sf Vector of vector of signed formulas
 * @param vec_sf Vector of signed formulas
 * @return True if vec_sf is in vec_vec_sf 
 * @return False otherwise
 */
bool vec_sf_in_vec_vec_sf(vector<vector<SignedFmla>> vec_vec_sf, vector<SignedFmla> vec_sf) {
    for (vector<SignedFmla> vec_sf2 : vec_vec_sf) {
        if (vec_sf_equality(vec_sf, vec_sf2)) return true;
    }

    return false;
}

/**
 * @brief Gets the index of the sibling node of a cut node
 * 
 * @param tbl Tableau
 * @param cut_node_idx Index of cut node
 * @return Index of the sibling node of the node indexed by cut_node_idx 
 */
int get_cut_sibling_node(Tableau tbl, int cut_node_idx) {
    if (tbl[cut_node_idx].justification_parents[0] == -3) {
        return -1;
    }

    int tbl_parent_node_idx = tbl[cut_node_idx].tbl_parent;
    vector<int> tbl_children = tbl[tbl_parent_node_idx].tbl_children;
    if (tbl_children[0] == cut_node_idx) return tbl_children[1];
    else return tbl_children[0];
}

/**
 * @brief Check if there is a proof for a vector of signed formulas that is proof-isomorphic to a tableau given as input
 * 
 * @param tbl Tableau
 * @param er Expansion rules
 * @param sf Signed formulas
 * @return True if there is a proof for `sf` that is proof-isomorphic to `tbl` 
 * @return False otherwise
 */
bool is_proof_isomorphic_sf_set(Tableau tbl, vector<TblRule> er, vector<SignedFmla> sf) {
    Tableau proof_isomorphic_tbl = get_initial_tableau(sf);
    set<int> visited_cut_nodes;

    queue<Tableau> q;

    q.push(proof_isomorphic_tbl);

    while (!q.empty()) {
        Tableau current = q.front();
        int size = current.size();

        q.pop();

        if (current.size() == tbl.size()) {
            if (is_closure_isomorphic(tbl, current, er)) {
                return true;
            }
        }
        else if (current.size() < tbl.size()) {
            TblNode tbl_node = tbl[size];

            if (visited_cut_nodes.find(size) == visited_cut_nodes.end()) {
                if (tbl_node.justification_parents[0] == -2) { // is a cut
                    vector<int> proof_isomorphic_branch;
                    int tbl_parent_idx = tbl_node.tbl_parent;
                    while(tbl_parent_idx != 0) {
                        proof_isomorphic_branch.push_back(tbl_parent_idx);
                        tbl_parent_idx = tbl[tbl_parent_idx].tbl_parent;
                    }
                    proof_isomorphic_branch.push_back(0);
                    vector<SignedFmla> copremises = minor_copremises_branch(current, proof_isomorphic_branch, er);

                    int sibling_node_idx = get_cut_sibling_node(tbl, size);
                    visited_cut_nodes.insert(sibling_node_idx);
                    TblNode sibling_node = tbl[sibling_node_idx];

                    TblNode expansion_node1, expansion_node2;
                    for (SignedFmla copremise : copremises) {
                        if (are_isomorphic_with_equal_parameters(copremise.fmla, tbl_node.signed_fmla.fmla)){
                            expansion_node1.justification_parents = tbl_node.justification_parents;
                            expansion_node1.tbl_parent = tbl_node.tbl_parent;
                            expansion_node1.tbl_children = tbl_node.tbl_children;
                            SignedFmla sf1(tbl_node.signed_fmla.sign, copremise.fmla);
                            expansion_node1.signed_fmla = sf1;

                            expansion_node2.justification_parents = sibling_node.justification_parents;
                            expansion_node2.tbl_parent = sibling_node.tbl_parent;
                            expansion_node2.tbl_children = sibling_node.tbl_children;
                            
                            SignedFmla sf2(sibling_node.signed_fmla.sign, copremise.fmla);
                            expansion_node2.signed_fmla = sf2;

                            current.push_back(expansion_node1);
                            current.push_back(expansion_node2);
                            q.push(current);
                        }
                    }

                } else {
                    vector<SignedFmla> justifications;
                    for (int j : tbl_node.justification_parents) {
                        justifications.push_back(current[j].signed_fmla);
                    }
                    
                    vector<SignedFmla> conclusions = get_conclusions_from_justifications(justifications, er);
                    if (conclusions.size() > 0) {
                        TblNode expansion_node;
                        for (SignedFmla conclusion : conclusions) {
                            Tableau conc = current;
                            if (are_isomorphic_with_equal_parameters(conclusion.fmla, tbl_node.signed_fmla.fmla)){
                                expansion_node.justification_parents = tbl_node.justification_parents;
                                expansion_node.tbl_parent = tbl_node.tbl_parent;
                                expansion_node.tbl_children = tbl_node.tbl_children;
                                expansion_node.signed_fmla = conclusion;

                                conc.push_back(expansion_node);
                                q.push(conc);
                            }
                        }
                    }
                }
            }
        }

    }

    return false;
}

/**
 * @brief 
 * 
 * @param tbl 
 * @param er 
 * @param sf 
 * @return Tableau 
 */
Tableau search_proof_isomorphic_sf_set(Tableau tbl, vector<TblRule> er, vector<SignedFmla> sf) {

    Tableau proof_isomorphic_tbl = get_initial_tableau(sf);
    for (int i = 0; i < tbl.size(); i++) {
        TblNode tbl_node = tbl[i];
        if (tbl_node.justification_parents[0] != -1) {

            vector<SignedFmla> justifications;
            for (int j : tbl_node.justification_parents) {
                justifications.push_back(proof_isomorphic_tbl[j].signed_fmla);
            }
            
            vector<SignedFmla> conclusions = get_conclusions_from_justifications(justifications, er);
            if (conclusions.size() > 0) {
                TblNode expansion_node;
                bool has_expansion_node = false;
                for (SignedFmla conclusion : conclusions) {
                    if (are_isomorphic_with_equal_parameters(conclusion.fmla, tbl_node.signed_fmla.fmla)){
                        expansion_node.justification_parents = tbl_node.justification_parents;
                        expansion_node.tbl_parent = tbl_node.tbl_parent;
                        expansion_node.tbl_children = tbl_node.tbl_children;
                        expansion_node.signed_fmla = conclusion;

                        has_expansion_node = true;
                    }
                }

                if (has_expansion_node) {
                    proof_isomorphic_tbl.push_back(expansion_node);
                }
                else {
                    return {};
                }
            }
            else {
                return {};
            }
        }
    }
    if (is_closure_isomorphic(tbl, proof_isomorphic_tbl, er)) {
        return proof_isomorphic_tbl;
    }
    else {
        return {};
    }
}

/**
 * @brief Checks if two tableaux proofs are closure-isomorphic
 * 
 * @param tbl Tableau
 * @param proof_isomorphic_tbl Tableau
 * @param er Expansion rules
 * @return True if `tbl` and `proof_isomorphic_tbl` are closure-isomorphic
 * @return False 
 */
bool is_closure_isomorphic(Tableau tbl, Tableau proof_isomorphic_tbl, vector<TblRule> er) {
    // tbl is the tableau that is closed
    vector< vector<int>> branches = get_tbl_branches(tbl);

    bool all_branches_closure_isomorphic = true;
    for (vector<int> branch : branches){
        vector<int> closure_nodes = branch_closure_nodes(branch, tbl, er);

        bool branch_closure_isomorphic = false; 
        if (closure_nodes.size() == 2) { // closure IS given by conjugate nodes
            int node1 = closure_nodes[0];
            int node2 = closure_nodes[1];
            if (fmla_equality(proof_isomorphic_tbl[node1].signed_fmla.fmla, proof_isomorphic_tbl[node2].signed_fmla.fmla) == true && opposite_polarity_nodes(proof_isomorphic_tbl[node1], proof_isomorphic_tbl[node2]) == true) {
                branch_closure_isomorphic = true;
            }
        }
        
        // closure IS NOT given by conjugate nodes
        bool all_closure_nodes_isomorphic = true;
        vector<int> closure_isomorphic_nodes;
        vector<TblRule> closure_rules = get_closure_rules(er);
        for (TblRule closure_rule : closure_rules) {
            if (try_apply_closure_rule(proof_isomorphic_tbl, branch, closure_rule)) {
                closure_isomorphic_nodes = get_justifications_closure_rule(proof_isomorphic_tbl, branch, closure_rule);
            }
        }

        if (closure_isomorphic_nodes.size() != closure_nodes.size()) {
            all_closure_nodes_isomorphic = false;
        }
        else {
            for (int i = 0; i < closure_nodes.size(); i++) {
                TblNode tbl_node = tbl[closure_nodes[i]];
                TblNode isomorphic_node = proof_isomorphic_tbl[closure_isomorphic_nodes[i]];
                if (!are_isomorphic_with_equal_parameters(tbl_node.signed_fmla.fmla, isomorphic_node.signed_fmla.fmla)) {
                    all_closure_nodes_isomorphic = false;
                }
            }
        }
        if (all_closure_nodes_isomorphic) {
            branch_closure_isomorphic = true;
        }

        all_branches_closure_isomorphic &= branch_closure_isomorphic;
        
    }
    return all_branches_closure_isomorphic; 
}

/**
 * @brief Gets the result of applying a rule having as justifications a vector of signed formula given as input
 * 
 * @param justifications Signed formulas
 * @param er Expansion rules
 * @return The result of applying a rule of `er` having as justifications `justifications`
 */
vector<SignedFmla> get_conclusions_from_justifications(vector<SignedFmla> justifications, vector<TblRule> er) {
    vector<SignedFmla> resulting_conclusions;
    for (TblRule rule : er) {
        vector<SignedFmla> premises = rule.premises;
        vector<SignedFmla> conclusions = rule.conclusions;
        if (premises.size() == justifications.size()) {
            vector<vector<int>> all_arranges = get_all_arranges(justifications.size(), premises.size());
            int arranges_amt = all_arranges.size();
            for (int i = 0; i < arranges_amt; i++) {
                vector<int> justifications_idx = all_arranges[i];

                bool all_premises_match = true;
                for (int j = 0; j < premises.size(); j++) {
                    if (!is_a_match(justifications[justifications_idx[j]], premises[j])) all_premises_match = false;
                }

                if (all_premises_match) {
                    // check if all matching of premises agree on substitution of parameters
                    bool all_parameters_match = true;
                    Subst parameters_conclusion_subst;
                    
                    vector<Fmla> premises_fmlas;
                    for (SignedFmla premise : premises) {
                        premises_fmlas.push_back(premise.fmla);
                    }
                    set<string> premises_parameters = get_all_parameters(premises_fmlas);

                    for (int j = 0; j < premises.size(); j++) {
                        set<string> premise_parameter = get_parameters(premises[j].fmla);
                        Subst matching_parameters_map = matching_parameters(justifications[justifications_idx[j]], premises[j]);
                        for (string parameter : premise_parameter) {
                            if (parameters_conclusion_subst.find(parameter) != parameters_conclusion_subst.end()) {
                                if (matching_parameters_map.find(parameter) != matching_parameters_map.end()) {
                                    if (!term_equality(parameters_conclusion_subst[parameter], matching_parameters_map[parameter])) all_parameters_match = false;
                                }
                            } else {
                                parameters_conclusion_subst[parameter] = matching_parameters_map[parameter];
                            }
                        }
                    }

                    // expand tableau
                    if (all_parameters_match) {
                        for (int j = 0; j < conclusions.size(); j++) {
                            SignedFmla conclusion = conclusions[j];
                            conclusion.fmla = subst_extension(conclusion.fmla, parameters_conclusion_subst);
                            resulting_conclusions.push_back(conclusion);
                        }
                    }
                }
            }
        }
    }
    return resulting_conclusions;
}

/**
 * @brief Checks if a vector of signed formulas can be the justification of a rule and if a signed formula can be the result of applying such rule
 * 
 * @param justifications Vector of signed formula
 * @param expansion Signed formula
 * @param er Expansion rules
 * @return True if `justification` is a justification for some rule in `er` and `expansion` is the result of applying this rule
 * @return False otherwise 
 */
bool check_rule_application(vector<SignedFmla> justifications, SignedFmla expansion, vector<TblRule> er) {
    for (TblRule rule : er) {
        vector<SignedFmla> premises = rule.premises;
        vector<SignedFmla> conclusions = rule.conclusions;
        if (premises.size() == justifications.size()) {
            vector<vector<int>> all_arranges = get_all_arranges(justifications.size(), premises.size());
            int arranges_amt = all_arranges.size();
            for (int i = 0; i < arranges_amt; i++) {
                vector<int> justifications_idx = all_arranges[i];

                bool all_premises_match = true;
                for (int j = 0; j < premises.size(); j++) {
                    if (!is_a_match(justifications[justifications_idx[j]], premises[j])) all_premises_match = false;
                }

                if (all_premises_match) {
                    // check if all matching of premises agree on substitution of parameters
                    bool all_parameters_match = true;
                    Subst parameters_conclusion_subst;
                    
                    vector<Fmla> premises_fmlas;
                    for (SignedFmla premise : premises) {
                        premises_fmlas.push_back(premise.fmla);
                    }
                    set<string> premises_parameters = get_all_parameters(premises_fmlas);

                    for (int j = 0; j < premises.size(); j++) {
                        set<string> premise_parameter = get_parameters(premises[j].fmla);
                        Subst matching_parameters_map = matching_parameters(justifications[justifications_idx[j]], premises[j]);
                        for (string parameter : premise_parameter) {
                            if (parameters_conclusion_subst.find(parameter) != parameters_conclusion_subst.end()) {
                                if (matching_parameters_map.find(parameter) != matching_parameters_map.end()) {
                                    if (!term_equality(parameters_conclusion_subst[parameter], matching_parameters_map[parameter])) all_parameters_match = false;
                                }
                            } else {
                                parameters_conclusion_subst[parameter] = matching_parameters_map[parameter];
                            }
                        }
                    }

                    // expand tableau
                    if (all_parameters_match) {
                        for (int j = 0; j < conclusions.size(); j++) {
                            SignedFmla conclusion = conclusions[j];
                            conclusion.fmla = subst_extension(conclusion.fmla, parameters_conclusion_subst);
                            if (conclusion.sign == expansion.sign && fmla_equality(conclusion.fmla, expansion.fmla)) return true;
                        }
                    }
                }
            }
        }
    }
    return false;
}

/**
 * @brief Gets the signed formulas of the initial segment of a tableau
 * 
 * @param tbl Tableau
 * @return Vector of signed of formulas that are in the initial segment of `tbl` 
 */
vector<SignedFmla> get_initial_sf(Tableau tbl) {
    vector<SignedFmla> initial_sf;

    for (TblNode tbl_node : tbl) {
        if (tbl_node.justification_parents[0] == -1) {
            initial_sf.push_back(tbl_node.signed_fmla);
        }
    }

    return initial_sf;
}

/**
 * @brief Gets the premises of the rules that are syntatically isomorphic to the premises of a rule given as input
 * 
 * @param er Expansion rules
 * @param rule Rule
 * @param prem_idx Index of a premise of rule
 * @param conc_idx Index of a conclusion of rule
 * @return Vector of the signed formulas that are premises of a rule in `er` and that are syntactically isomorphic to the premise `prem_idx` of `rule` 
 */
vector<SignedFmla> pattern_matching_premises(vector<TblRule> er, TblRule rule, int prem_idx, int conc_idx) {
    vector<SignedFmla> result;

    vector<SignedFmla> rule_premises = rule.premises;

    if (prem_idx >= rule_premises.size()) { // avoiding index out of range
        return result;
    }

    for (TblRule exp_rule : er) {
        vector<SignedFmla> premises = exp_rule.premises;
        vector<SignedFmla> conclusions = exp_rule.conclusions;
        if (premises.size() == rule_premises.size()) {
            vector<vector<int>> all_arranges = get_all_arranges(rule_premises.size(), premises.size());
            int arranges_amt = all_arranges.size();
            for (int i = 0; i < arranges_amt; i++) {
                vector<int> rule_premises_idx = all_arranges[i];

                bool all_premises_match = true;
                for (int j = 0; j < premises.size(); j++) {
                    if (!are_syntactically_isomorphic(rule_premises[rule_premises_idx[j]].fmla, premises[j].fmla)) all_premises_match = false;
                }

                if (all_premises_match) {
                    // check if all matching of premises agree on substitution of parameters
                    bool all_parameters_match = true;
                    Subst parameters_conclusion_subst;
                    
                    vector<Fmla> premises_fmlas;
                    for (SignedFmla premise : premises) {
                        premises_fmlas.push_back(premise.fmla);
                    }
                    set<string> premises_parameters = get_all_parameters(premises_fmlas);

                    for (int j = 0; j < premises.size(); j++) {
                        Subst matching_parameters_map = matching_parameters(rule_premises[rule_premises_idx[j]].fmla, premises[j].fmla);
                        for (FmlaNode fmla_node : premises[j].fmla) {
                            if (is_a_parameter(fmla_node)) {
                                string parameter = fmla_node.data;
                                if (parameters_conclusion_subst.find(parameter) != parameters_conclusion_subst.end()) {
                                    if (matching_parameters_map.find(parameter) != matching_parameters_map.end()) {
                                        if (!term_equality(parameters_conclusion_subst[parameter], matching_parameters_map[parameter])) all_parameters_match = false;
                                    }
                                }
                                else {
                                    parameters_conclusion_subst[parameter] = matching_parameters_map[parameter];
                                }
                            }
                        }
                    }

                    if (all_parameters_match) {
                        bool some_conclusion_matches = false;
                        SignedFmla rule_conclusion = rule.conclusions[conc_idx];

                        for (SignedFmla conclusion : conclusions) {
                            if (are_syntactically_isomorphic(conclusion.fmla, rule_conclusion.fmla)) some_conclusion_matches = true;
                        }
                        if (some_conclusion_matches) {
                            if (!signed_fmla_in_vec_signed_fmla(result, premises[prem_idx])) {
                                result.push_back(premises[prem_idx]);
                            }
                        }
                    }
                }
            }
        }
    }
    return result;
}

/**
 * @brief Gets the syntactic matching symbol of the occurrence of a symbol given as input
 * 
 * @param fmla Formula
 * @param node_idx Occurence of a symbol
 * @param matching_fmla Formula
 * @return Syntactic matching symbol in `matching_fmla` to the occurence of `node_idx` in `fmla`
 */
string get_syntactic_matching_symb(Fmla fmla, int node_idx, Fmla matching_fmla) {

    queue<int> q1, q2;
    q1.push(0);
    q2.push(0);

    while (!q2.empty()) {
        int current1 = q1.front();
        int current2 = q2.front();
        q1.pop(); q2.pop();

        if (node_idx == current1) {
            if (!is_a_parameter(matching_fmla[current2])) {
                return matching_fmla[current2].data;
            }
        }
       
        for (int i = 0; i < matching_fmla[current2].children.size(); i++) {
            q1.push(fmla[current1].children[i]);
            q2.push(matching_fmla[current2].children[i]);
        }
        
    }

    return "";
}