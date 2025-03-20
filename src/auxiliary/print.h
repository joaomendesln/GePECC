#ifndef PRINT_H
#define PRINT_H

#include <vector>
#include <string>
#include <iostream>
#include <stack>
#include <queue>

#include "../definitions/definitions.h"
#include "deduction.h"

using namespace std;

void pretty_printing_fmla_aux(Fmla fmla, FmlaNode node, int level);

void pretty_printing_fmla(Fmla fmla);

void print_vec_int(vector<int> vec);

void print_fmla_prefix(Fmla fmla);

void print_fmla_prefix_aux(Fmla fmla, int idx);

void print_term_prefix(Term term);

void print_term_prefix_aux(Term term, int idx);

void print_tableau(Tableau tbl); // TODO

void print_tableau_as_list(Tableau tbl);

void print_tableau_as_list_fmla_prefix(Tableau tbl);

// void print_tableau_with_closure(Tableau tbl);

#endif // PRINT_H