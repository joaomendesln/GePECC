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
#include "combinatorics.h"

using namespace std;

Tableau get_initial_tableau(vector<SignedFmla> sf_input);

int get_height_initial_nodes(Tableau tbl);

Tableau apply_rule_with_premise(Tableau tbl, TblRule expansion_rule, int rule_idx, vector<TblRule> er);

bool try_apply_closure_rule(Tableau tbl, vector<int> branch, TblRule closure_rule);

vector<int> get_justifications_closure_rule(Tableau tbl, vector<int> branch, TblRule closure_rule);

map<pair<int,int>, set<string>> get_ps_candidate_symbols(Tableau tbl, vector<TblRule> er);

vector<tuple<int, pair<int, int>>> get_vec_symb_descendant_occurrences(Fmla prem_fmla, Fmla conc_fmla, Fmla justf_fmla, Fmla exp_fmla, vector<tuple<int, pair<int, int>>> ps_symbs);

vector<Tableau> apply_cut(Tableau tbl, vector<TblRule> er);

bool is_a_match(SignedFmla sf, SignedFmla premise);

bool is_a_match(Fmla fmla_tbl, Fmla premise_fmla);

Subst matching_parameters(SignedFmla sf_tbl, SignedFmla premise);

Subst matching_parameters(Fmla sf_tbl, Fmla premise);

vector<int> get_tbl_leaves(Tableau tbl);

vector< vector<int>> get_tbl_branches(Tableau tbl);

int branch_leaf(Tableau tbl, vector<int> branch);

vector<int> get_tbl_levels(Tableau tbl);

bool opposite_polarity_nodes(TblNode node1, TblNode node2);

bool branch_is_closed(vector<int> branch, Tableau tbl, vector<TblRule> er);

vector<TblRule> get_closure_rules(vector<TblRule> er);

vector<vector<int>> get_tbl_open_branches(Tableau tbl, vector<TblRule> er);

bool tbl_is_closed(Tableau tbl, vector<TblRule> er);

vector<Term> get_all_terms_of_branch(Tableau tbl, vector<int> branch);

bool has_single_justification_nodes(Tableau tbl, vector<TblRule> er);

Tableau apply_single_premise_rules(Tableau tbl, vector<TblRule> er);

vector<Tableau> get_tbl_successors(Tableau tbl, vector<TblRule> er);

vector<int> branch_closure_nodes(vector<int> branch, Tableau tbl, vector<TblRule> er);

bool is_closure_isomorphic(Tableau tbl, Tableau proof_isomorphic_tbl, vector<TblRule> er);

int get_branch_size(Tableau tbl, vector<int> branch, vector<TblRule> er);

int get_size(Tableau tbl, vector<TblRule> er);

vector<Tableau> search_minimal_proofs(vector<SignedFmla> sf, vector<TblRule> er);

Tableau clean_tbl(Tableau tbl, vector<TblRule> er);

Tableau remove_node(Tableau tbl, int idx);

vector<SignedFmla> minor_copremises_branch(Tableau tbl, vector<int> branch, vector<TblRule> er);

vector<SignedFmla> minor_copremises_rule(SignedFmla sf, TblRule rule);

bool fmla_in_vec_fmla(vector<Fmla> fmlas, Fmla fmla);

bool fmla_in_vec_signed_fmla(vector<SignedFmla> signed_fmlas, Fmla fmla);

bool node_in_branch(SignedFmla sf, Tableau tbl, vector<int> branch);

vector<vector<SignedFmla>> search_proof_isomorphic_sf_sets(Tableau tbl, vector<TblRule> er);

vector<SignedFmla> get_conclusions_from_justifications(vector<SignedFmla> justifications, vector<TblRule> er);

int get_cut_sibling_node(Tableau tbl, int cut_node_idx);

bool is_proof_isomorphic_sf_set(Tableau tbl, vector<TblRule> er, vector<SignedFmla> sf);

vector<vector<SignedFmla>> get_sf_candidates(vector<SignedFmla> initial_sf, map<pair<int,int>, set<string>> ps_candidate_symbols);

bool vec_sf_equality(vector<SignedFmla> vec_sf1, vector<SignedFmla> vec_sf2);

bool vec_sf_in_vec_vec_sf(vector<vector<SignedFmla>> vec_vec_sf, vector<SignedFmla> vec_sf);

bool check_rule_application(vector<SignedFmla> justifications, SignedFmla expansion, vector<TblRule> er);

vector<SignedFmla> get_initial_sf(Tableau tbl);

vector<SignedFmla> pattern_matching_premises(vector<TblRule> er, TblRule rule, int prem_idx, int conc_idx);

string get_syntactic_matching_symb(Fmla fmla, int node_idx, Fmla matching_fmla);

#endif // DEDUCTION_H