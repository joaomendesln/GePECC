#ifndef DEDUCTION_H
#define DEDUCTION_H

#include <vector>
#include <set>
#include <queue>
#include <map>
#include <string>
#include <cmath>

#include "../definitions/definitions.h"
#include "syntax.h"
#include "print.h"
#include "pre_process.h"

using namespace std;

Tableau get_initial_tableau(vector<SignedFmla> sf_input);

bool are_mask_digits_different(vector<int> arrange_mask);

vector<int> increment_arrange_mask(vector<int> arrange_mask, int base);

vector<vector<int>> get_all_arranges(int branch_size, int premisses_size);

vector<int> increment_arrange_repitition_mask(vector<int> arrange_mask, int base);

vector<vector<int>> get_all_arranges_repetition(int terms_tbl_amt, int parameters_conclustion_amt);

vector<Tableau> trying_apply_expansion_rule(Tableau tbl, TblRule expansion_rule); // One expansion rule may be applied in a tableau in several ways. That's why the output is a vector of tableaux

vector<Tableau> apply_cut(Tableau tbl, Fmla cut_fmla);

bool is_a_match(SignedFmla sf, SignedFmla premisse);

Subst matching_parameters(SignedFmla sf_tbl, SignedFmla premisse);

vector<int> get_tbl_leaves(Tableau tbl);

vector< vector<int>> get_tbl_branches(Tableau tbl);

int branch_leaf(Tableau tbl, vector<int> branch);

bool opposite_polarity_nodes(TblNode node1, TblNode node2);

bool branch_is_closed(vector<int> branch, Tableau tbl);

vector< vector<int>> get_tbl_open_branches(Tableau tbl);

bool tbl_is_closed(Tableau tbl);

vector<Term> get_all_terms_of_branch(Tableau tbl, vector<int> branch);

vector<Fmla> get_cut_fmlas(vector<TblRule> er);

vector<Tableau> get_tbl_successors(Tableau tbl, vector<TblRule> er);

bool is_closed(Tableau tbl);

pair<int, int> branch_closure_nodes(vector<int> branch, Tableau tbl);

bool is_clean(Tableau tbl);

int get_branch_size(Tableau tbl, vector<int> branch);

int get_size(Tableau tbl);

vector<Tableau> extract_minimal_proofs(vector<SignedFmla> sf, vector<TblRule> er);

bool node_in_tbl(SignedFmla sf, Tableau tbl);

int get_fmla_max_size(Tableau tbl, vector<int> branch);

#endif // DEDUCTION_H