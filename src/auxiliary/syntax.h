#ifndef SYNTAX_H
#define SYNTAX_H

#include <iostream>
#include <vector>
#include <set>
#include <queue>
#include <map>
#include <string>
#include <filesystem>
#include <fstream>
#include <sstream>

#include "../definitions/definitions.h"
#include "pre_process.h"

using namespace std;

bool is_function_symb(string symb, map<string, int> function_symbs);

bool is_function_symb(string symb);

bool is_predicate_symb(string symb, map<string, int> predicate_symbs);

bool is_predicate_symb(string symb);

bool is_language_symb(string symb, map<string, int> language_symbs);

bool is_not_skolem_symb(string symb);

bool is_a_parameter(FmlaNode fmla_node);

bool is_a_parameter(TermNode term_node);

bool parameter_in_fmla(Fmla fmla, string parameter);

Term get_term_of_fmla(Fmla fmla, int subfmla_root);

vector<Term> get_all_terms_of_fmla(Fmla fmla);

bool term_in_vector_of_terms(Term term, vector<Term> terms);

Fmla subst_parameter_by_term(Fmla fmla, int parameter_idx, Term term);

set<int> get_parameters_idxs(Fmla fmla);

set<string> get_parameters(Fmla fmla);

Fmla subst_extension(Fmla fmla, Subst subs);

Fmla subst_extension_potential(Fmla fmla, Subst subs);

string get_new_parameter(set<string> parameters);

set<string> get_all_parameters_of_fmla(Fmla fmla);

vector<string> get_all_parameters_of_term(Term term);

set<string> get_all_parameters(vector<Fmla> fmlas);

bool fmla_equality(Fmla fmla1, Fmla fmla2);

bool fmla_equality_under_subst(Fmla fmla1, Fmla fmla2);

bool are_syntactically_isomorphic(Fmla fmla1, Fmla fmla2);

bool are_isomorphic_with_equal_parameters(Fmla fmla1, Fmla fmla2);

bool term_equality(Term term1, Term term2);

int get_term_idx_img_subst(Fmla fmla1, int parameter_idx, Fmla fmla2);

int get_height(Fmla fmla);

int get_height(vector<Fmla> fmlas);

#endif // SYNTAX_H