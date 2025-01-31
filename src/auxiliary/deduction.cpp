#include "deduction.h"

using namespace std;

vector<TblNode> get_initial_tableau() {
    vector<SignedFmla> sf_input = pre_process_signed_fmla_input();
    vector<TblNode> initial_tableau;

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

        initial_tableau.push_back(tbl_node);
    }

    return initial_tableau;
}

bool are_mask_digits_different(vector<int> arrange_mask) {
    set<int> mask_digits_set;
    for (int i = 0; i < arrange_mask.size(); i++) {
        mask_digits_set.insert(arrange_mask[i]);
    }
    if (mask_digits_set.size() == arrange_mask.size()) return true;
    else return false;
}

vector<int> increment_arrange_mask(vector<int> arrange_mask, int base){
    for (int i = arrange_mask.size() - 1; i >= 0; i--) {
        if (arrange_mask[i] == base - 1) {
            arrange_mask[i] = 0;
        }
        else {
            arrange_mask[i] = arrange_mask[i] + 1;
            if (are_mask_digits_different(arrange_mask)) {
                return arrange_mask;
            }
            else {
                i = arrange_mask.size();
            }
        }
    }
    return {};
}

vector<vector<int>> get_all_arranges(int branch_size, int premisses_size) {
    vector<vector<int>> all_arranges;

    int arranges_amt = 1;
    int size = premisses_size;
    int base = branch_size;
    while (size > 0) {
        arranges_amt *= base;
        base--;
        size--;
    }

    vector<int> arrange;

    for (int i = 0; i < premisses_size; i++) arrange.push_back(0);
    for (int i = 0; i < arranges_amt; i++){
        arrange = increment_arrange_mask(arrange, branch_size);
        all_arranges.push_back(arrange);
    }

    return all_arranges;
}

vector<vector<TblNode>> trying_apply_expansion_rule(vector<TblNode> tbl, TblRule expansion_rule) {
    vector<SignedFmla> premisses = expansion_rule.premisses;
    vector<SignedFmla> conclusions = expansion_rule.conclusion;
    vector<vector<TblNode>> resuling_tableaux;

    if (premisses.size() == 0) {
        // TODO
    }
    else {
        vector<vector<int>> branches_idxs = get_tbl_open_branches(tbl);

        for (vector<int> branch_idxs : branches_idxs) {
            int branch_size = branch_idxs.size();
            int premisses_size = premisses.size();
            if (branch_size >= premisses_size) {
                vector<vector<int>> all_arranges = get_all_arranges(branch_size, premisses_size);
                int arranges_amt = all_arranges.size();
                for (int i = 0; i < arranges_amt; i++) {
                    vector<int> arrange = all_arranges[i];

                    bool all_premisses_match = true;
                    for (int j = 0; j < premisses.size(); j++) {
                        int branch_node_idx = branch_idxs[arrange[j]];
                        if (!is_a_match(tbl[branch_node_idx].signed_fmla, premisses[j])) all_premisses_match = false;
                    }

                    // check if all matching of premisses agree on substitution of parameters
                    bool all_parameters_match = true;
                    if (all_premisses_match) {
                        map<string, vector<TermNode>> parameters_conclusion_subst;
                        
                        vector<vector<FmlaNode>> premisses_fmlas;
                        for (SignedFmla premisse : premisses) {
                            premisses_fmlas.push_back(premisse.fmla);
                        }
                        set<string> premisses_parameters = get_all_parameters(premisses_fmlas);

                        for (int j = 0; j < premisses.size(); j++) {
                            int branch_node_idx = branch_idxs[arrange[j]];
                            map<string, vector<TermNode>> matching_parameters_map = matching_parameters(tbl[branch_node_idx].signed_fmla, premisses[j]);
                            for (string parameter : premisses_parameters) {
                                if (parameters_conclusion_subst.find(parameter) != parameters_conclusion_subst.end()) {
                                    if (matching_parameters_map.find(parameter) != matching_parameters_map.end()) {
                                        if (!term_equality(parameters_conclusion_subst[parameter], matching_parameters_map[parameter])) all_parameters_match = false;
                                    }
                                } else {
                                    parameters_conclusion_subst[parameter] = matching_parameters_map[parameter];
                                }
                            }
                        }
                    }

                    

                    // for every premisse
                        // check if the premisse fits with the formula in that node

                    // if all premisse fit
                        // get all parameters in premisses
                        // check if the parameters unification is the same
                    
                    // if the paramters unification is the same
                        // expand the tableau: tableau input, leaf_node, mapping from parameters to term, conclusions
                }

            }
            else {
                // DO NOTHING
            }
        }
    }
    return resuling_tableaux;
}

bool is_a_match(SignedFmla sf_tbl, SignedFmla premisse) {
    if (sf_tbl.sign != premisse.sign) {
        return false;
    }

    if (sf_tbl.fmla.size() < premisse.fmla.size()) {
        return false;
    }

    queue<int> sf_tbl_queue;
    queue<int> premisse_queue;

    map<string, int> function_symbs = pre_process_function_symbs();
    map<string, int> predicate_symbs = pre_process_predicate_symbs();

    sf_tbl_queue.push(0);
    premisse_queue.push(0);

    if (premisse.fmla[0].data != sf_tbl.fmla[0].data) return false; // if the predicate symbols are different

    while (!premisse_queue.empty()) {
        FmlaNode premisse_node = premisse.fmla[premisse_queue.front()];
        FmlaNode sf_tbl_node = sf_tbl.fmla[sf_tbl_queue.front()];

        sf_tbl_queue.pop();
        premisse_queue.pop();

        for (int i = 0; i < premisse_node.children.size(); i++) {
            premisse_queue.push(premisse_node.children[i]);
            sf_tbl_queue.push(sf_tbl_node.children[i]);
        }

        if (is_function_symb(premisse_node.data, function_symbs) || is_predicate_symb(premisse_node.data, predicate_symbs)) {
            if (premisse_node.data != sf_tbl_node.data) return false;
        }
    }
    return true;
}

map<string, vector<TermNode>> matching_parameters(SignedFmla sf_tbl, SignedFmla premisse) {
    map<string, vector<TermNode>> matching_parameters_map;

    if (!is_a_match(sf_tbl, premisse)) return matching_parameters_map;

    queue<int> sf_tbl_queue;
    queue<int> premisse_queue;

    map<string, int> predicate_symbs = pre_process_predicate_symbs();
    map<string, int> function_symbs = pre_process_function_symbs();

    sf_tbl_queue.push(0);
    premisse_queue.push(0);

    while (!premisse_queue.empty()) {
        int premisse_node_idx = premisse_queue.front();
        int sf_tbl_node_idx = sf_tbl_queue.front();

        FmlaNode premisse_node = premisse.fmla[premisse_node_idx];
        FmlaNode sf_tbl_node = sf_tbl.fmla[sf_tbl_node_idx];

        sf_tbl_queue.pop();
        premisse_queue.pop();


        for (int i = 0; i < premisse_node.children.size(); i++) {
            premisse_queue.push(premisse_node.children[i]);
            sf_tbl_queue.push(sf_tbl_node.children[i]);
        }

        if (!is_function_symb(premisse_node.data, function_symbs) && !is_predicate_symb(premisse_node.data, predicate_symbs)) {
            matching_parameters_map[premisse_node.data] = get_term_of_fmla(sf_tbl.fmla, sf_tbl_node_idx);
        }
    }
    return matching_parameters_map;
}

vector<int> get_tbl_leaves(vector<TblNode> tbl) {
    vector<int> leaves_idx;
    for (int i = 0; i < tbl.size(); i++) {
        if (tbl[i].tbl_children.size() == 0) {
            leaves_idx.push_back(i);
        }
    }
    return leaves_idx;
}

vector< vector<int>> get_tbl_branches(vector<TblNode> tbl) {
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

bool opposite_polarity_nodes(TblNode node1, TblNode node2) {
    if (node1.signed_fmla.sign == polarity::plus && node2.signed_fmla.sign == polarity::minus) return true;

    if (node1.signed_fmla.sign == polarity::minus && node2.signed_fmla.sign == polarity::plus) return true;

    return false;
}

bool branch_is_closed(vector<int> branch, vector<TblNode> tbl) {
    for (int i = 0; i < branch.size(); i++) {
        for (int j = 0; j < branch.size(); j++) {
            int node1 = branch[i];
            int node2 = branch[j];
            if (fmla_equality(tbl[node1].signed_fmla.fmla, tbl[node2].signed_fmla.fmla) == true && opposite_polarity_nodes(tbl[node1], tbl[node2]) == true) {
                return true;
            }
        }
    }
    return false;
}

vector< vector<int>> get_tbl_open_branches(vector<TblNode> tbl) {

    vector<vector<int>> open_branches;
    vector<vector<int>> all_branches = get_tbl_branches(tbl);

    for (vector<int> branch : all_branches) {
        if (!branch_is_closed(branch, tbl)) open_branches.push_back(branch);
    }

    return open_branches;
}

bool tbl_is_closed(vector<TblNode> tbl) {
    vector<int> leaves = get_tbl_leaves(tbl);

    vector< vector<int>> branches = get_tbl_branches(tbl);

    bool is_closed = true;
    for (int i = 0; i < branches.size(); i++){
        is_closed = is_closed && branch_is_closed(branches[i], tbl); 
    }

    return is_closed;
}