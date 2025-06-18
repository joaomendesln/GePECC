#ifndef COMBINATORICS_H
#define COMBINATORICS_H

#include <vector>
#include <set>
#include <cmath>

using namespace std;

bool are_mask_digits_different(vector<int> arrange_mask);

vector<int> increment_arrange_mask(vector<int> arrange_mask, int base);

vector<vector<int>> get_all_arranges(int branch_size, int premises_size);

vector<int> increment_arrange_repitition_mask(vector<int> arrange_mask, int base);

vector<int> increment_arrange_repitition_mask(vector<int> arrange_mask, vector<int> base);

vector<vector<int>> get_all_arranges_repetition(int terms_tbl_amt, int parameters_conclustion_amt);

#endif // COMBINATORICS_H