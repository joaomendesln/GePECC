#ifndef DEDUCTION_H
#define DEDUCTION_H

#include <vector>
#include <set>
#include <queue>
#include <map>
#include <string>

#include "../definitions/definitions.h"
#include "syntax.h"
#include "print.h"
#include "pre_process.h"

using namespace std;

vector<TblNode> get_initial_tableau();

bool are_mask_digits_different(vector<int> arrange_mask);

vector<int> increment_arrange_mask(vector<int> arrange_mask, int base);

vector<vector<int>> get_all_arranges(int branch_size, int premisses_size);

vector<vector<TblNode>> trying_apply_expansion_rule(vector<TblNode> tbl, TblRule expansion_rule); // One expansion rule may be applied in a tableau in several ways. That's why the output is a vector of tableaux

bool is_a_match(SignedFmla sf, SignedFmla premisse);

#endif // DEDUCTION_H