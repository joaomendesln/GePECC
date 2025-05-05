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

vector<int> increment_arrange_repitition_mask(vector<int> arrange_mask, vector<int> base);

vector<vector<int>> get_all_arranges_repetition(int terms_tbl_amt, int parameters_conclustion_amt);

int get_height_initial_nodes(Tableau tbl);

Tableau apply_rule_with_premisse(Tableau tbl, TblRule expansion_rule, int rule_idx, vector<TblRule> er);

bool try_apply_closure_rule(Tableau tbl, vector<int> branch, TblRule expansion_rule, vector<TblRule> er);

vector<int> get_premisses_closure_rule(Tableau tbl, vector<int> branch, TblRule expansion_rule, vector<TblRule> er);

map<pair<int,int>, set<string>> get_ps_potential_symbols(Tableau tbl, vector<TblRule> er);

vector<tuple<int, pair<int, int>>> get_vec_ps_symbs(Fmla prem_fmla, Fmla conc_fmla, Fmla justf_fmla, Fmla exp_fmla, vector<tuple<int, pair<int, int>>> ps_symbs);

vector<Tableau> apply_cut(Tableau tbl, vector<TblRule> er);

bool is_a_match(SignedFmla sf, SignedFmla premisse);

bool is_a_match(Fmla sf_tbl, Fmla premisse);

Subst matching_parameters(SignedFmla sf_tbl, SignedFmla premisse);

Subst matching_parameters(Fmla sf_tbl, Fmla premisse);

vector<int> get_tbl_leaves(Tableau tbl);

vector< vector<int>> get_tbl_branches(Tableau tbl);

vector<int> get_tbl_branch_by_node(Tableau tbl, int node_idx);

int branch_leaf(Tableau tbl, vector<int> branch);

vector<int> get_tbl_levels(Tableau tbl);

bool opposite_polarity_nodes(TblNode node1, TblNode node2);

bool branch_is_closed(vector<int> branch, Tableau tbl, vector<TblRule> er);

vector<TblRule> get_closure_rules(vector<TblRule> er);

vector<vector<int>> get_tbl_open_branches(Tableau tbl, vector<TblRule> er);

bool tbl_is_closed(Tableau tbl, vector<TblRule> er);

vector<Term> get_all_terms_of_branch(Tableau tbl, vector<int> branch);

bool is_a_term_in_branch(Tableau tbl, vector<int> branch, Term term);

vector<Fmla> get_cut_fmlas(vector<TblRule> er);

vector<TblRule> add_cut_rule(vector<TblRule> er);

bool has_single_justification_nodes(Tableau tbl, vector<TblRule> er);

Tableau saturate_single_justification_nodes(Tableau tbl, vector<TblRule> er);

vector<Tableau> get_tbl_successors(Tableau tbl, vector<TblRule> er);

vector<int> branch_closure_nodes(vector<int> branch, Tableau tbl, vector<TblRule> er);

bool is_closure_isomorphic(Tableau tbl, Tableau proof_isomorphic_tbl, vector<TblRule> er);

bool are_closure_nodes(Tableau tbl, vector<int> closure_nodes);

bool is_clean(Tableau tbl);

int get_branch_size(Tableau tbl, vector<int> branch, vector<TblRule> er);

int get_size(Tableau tbl, vector<TblRule> er);

// int get_branch_size2(Tableau tbl, vector<int> branch);

// int get_size2(Tableau tbl);

vector<Tableau> extract_minimal_proofs(vector<SignedFmla> sf, vector<TblRule> er);

Tableau clean_tbl(Tableau tbl, vector<TblRule> er);

Tableau remove_node(Tableau tbl, int idx);

vector<SignedFmla> potential_premisse_nodes_branch(Tableau tbl, vector<int> branch, vector<TblRule> er);

bool is_potential_premisse_nodes_branch(SignedFmla sf_input, Tableau tbl, vector<int> branch, vector<TblRule> er);

vector<SignedFmla> potential_premisse_nodes_rule(SignedFmla sf, TblRule rule);

bool fmla_in_vec_fmla(vector<Fmla> fmlas, Fmla fmla);

bool fmla_in_vec_signed_fmla(vector<SignedFmla> signed_fmlas, Fmla fmla);

bool node_in_tbl(SignedFmla sf, Tableau tbl);

bool node_in_branch(SignedFmla sf, Tableau tbl, vector<int> branch);

int get_fmla_max_size(Tableau tbl, vector<int> branch);

vector<TblRule> remove_unnecessary_rules(vector<SignedFmla> sf, vector<TblRule> er);

vector<vector<SignedFmla>> proof_isomorphic_sf_sets(Tableau tbl, vector<TblRule> er);

vector<SignedFmla> get_conclusions_from_justifications(vector<SignedFmla> justifications, vector<TblRule> er);

int get_cut_sibling_node(Tableau tbl, int cut_node_idx);

bool is_proof_isomorphic_sf_set(Tableau tbl, vector<TblRule> er, vector<SignedFmla> sf);

Tableau proof_isomorphic_sf_set(Tableau tbl, vector<TblRule> er, vector<SignedFmla> sf);

vector<vector<SignedFmla>> get_sf_candidates(vector<SignedFmla> initial_sf, map<pair<int,int>, set<string>> get_ps_potential_symbols);

bool vec_sf_equality(vector<SignedFmla> vec_sf1, vector<SignedFmla> vec_sf2);

bool vec_sf_in_vec_vec_sf(vector<vector<SignedFmla>> vec_vec_sf, vector<SignedFmla> vec_sf);

bool is_valid_filled_ps(Tableau filled_ps, vector<pair<symbType, int>> proof_schema);

bool check_rule_application(vector<SignedFmla> justifications, SignedFmla expansion, vector<TblRule> er);

bool check_cuts(Tableau filled_ps);

vector<SignedFmla> get_initial_sf(Tableau tbl);

vector<SignedFmla> pattern_matching_premisses(vector<TblRule> er, TblRule rule, int prem_idx, int conc_idx);

string get_pattern_matching_premisse_symb(Fmla fmla, int node_idx, Fmla matching_fmla);

#endif // DEDUCTION_H