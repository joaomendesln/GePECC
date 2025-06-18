#include "combinatorics.h"

using namespace std;

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

vector<vector<int>> get_all_arranges(int branch_size, int premises_size) {
    vector<vector<int>> all_arranges;

    int arranges_amt = 1;
    int size = premises_size;
    int base = branch_size;
    while (size > 0) {
        arranges_amt *= base;
        base--;
        size--;
    }

    vector<int> arrange;

    if (premises_size == 1) {
        for (int i = 0; i < branch_size; i++) {
            arrange = {i};
            all_arranges.push_back(arrange);
        }
    }
    else {
        for (int i = 0; i < premises_size; i++) arrange.push_back(0);
        for (int i = 0; i < arranges_amt; i++){
            arrange = increment_arrange_mask(arrange, branch_size);
            all_arranges.push_back(arrange);
        }
    }

    return all_arranges;
}

vector<int> increment_arrange_repitition_mask(vector<int> arrange_mask, int base){
    for (int i = arrange_mask.size() - 1; i >= 0; i--) {
        if (arrange_mask[i] == base - 1) {
            arrange_mask[i] = 0;
        }
        else {
            arrange_mask[i] = arrange_mask[i] + 1;
            return arrange_mask;
        }
    }
    return arrange_mask;
}

vector<vector<int>> get_all_arranges_repetition(int terms_tbl_amt, int parameters_conclustion_amt) {
    vector<vector<int>> all_arranges;

    int arranges_amt = pow(terms_tbl_amt, parameters_conclustion_amt);

    vector<int> arrange;

    for (int i = 0; i < parameters_conclustion_amt; i++) arrange.push_back(0);
    all_arranges.push_back(arrange); // start all arrange with the arrange containing only 0's
    for (int i = 1; i < arranges_amt; i++){
        arrange = increment_arrange_repitition_mask(arrange, terms_tbl_amt);
        all_arranges.push_back(arrange);
    }

    return all_arranges;
}

vector<int> increment_arrange_repitition_mask(vector<int> arrange_mask, vector<int> base) {
    for (int i = arrange_mask.size() - 1; i >= 0; i--) {
        if (arrange_mask[i] == base[i] - 1) {
            arrange_mask[i] = 0;
        }
        else {
            arrange_mask[i] = arrange_mask[i] + 1;
            return arrange_mask;
        }
    }
    return arrange_mask;
}