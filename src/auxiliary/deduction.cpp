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
    vector<vector<TblNode>> resuling_tableaux;

    if (premisses.size() == 0) {
        // TODO
    }
    else {
        // Do it for every open tableau branch

        // For, now we just have single branch tableau
        // get the amount of checking
        int tbl_size = tbl.size();
        int premisses_size = premisses.size();
        if (tbl_size >= premisses_size) {
            vector<vector<int>> all_arranges = get_all_arranges(tbl_size, premisses_size);
            int arranges_amt = all_arranges.size();
            for (int i = 0; i < arranges_amt; i++) {
                vector<int> arrange = all_arranges[i];
                vector<int> tbl_nodes_to_check;
                for (int j = 0; j < arrange.size(); j++) {
                    tbl_nodes_to_check.push_back(arrange[j]);
                }

                // for every premisse
                    // check if the premisse fits with the formula in that node

                // if all premisse fit
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

bool is_a_match(SignedFmla sf, SignedFmla premisse) {
    if (sf.sign != premisse.sign) {
        return false;
    }

    if (sf.fmla.size() < premisse.fmla.size()) {
        return false;
    }

    queue<int> sf_queue;
    queue<int> premisse_queue;

    map<string, int> function_symbs = pre_process_function_symbs();
    // map<string, int> predicate_symbs = get_predicate_symbs();

    sf_queue.push(0);
    premisse_queue.push(0);

    if (premisse.fmla[0].data != sf.fmla[0].data) return false;

    while (!premisse_queue.empty()) {
        // int premisse_current = premisse_queue.front();
        // int sf_current = sf_queue.front();

        FmlaNode premisse_node = premisse.fmla[premisse_queue.front()];
        FmlaNode sf_node = sf.fmla[sf_queue.front()];

        sf_queue.pop();
        premisse_queue.pop();

        if (is_function_symb(premisse_node.data, function_symbs)) {
            if (premisse_node.data != sf_node.data) {
                return false;
            }
            if (premisse_node.children.size() > 0) {
                if (premisse_node.children.size() != sf_node.children.size()) return false;
                for (int i = 0; i < premisse_node.children.size(); i++) {
                    premisse_queue.push(premisse_node.children[i]);
                    sf_queue.push(sf_node.children[i]);
                }
            } 
        }
    }
    
    return true;
}