#ifndef PRINT_H
#define PRINT_H

#include <vector>
#include <string>
#include <iostream>
#include <stack>
#include <queue>

#include "../definitions/definitions.h"
// #include "tableaux.h"

using namespace std;

void pretty_printing_fmla_aux(vector<FmlaNode> fmla, FmlaNode node, int binary_acestor_amt, side side);

void pretty_printing_fmla(vector<FmlaNode> fmla);

void print_vec_int(vector<int> vec);

void print_fmla_infix(vector<FmlaNode> fmla);

void print_fmla_infix_aux(vector<FmlaNode> fmla, int idx);

void print_tableau(vector<TblNode> tbl); // TODO

void print_tableau_as_list(vector<TblNode> tbl);

// void print_tableau_with_closure(vector<TblNode> tbl);

#endif // PRINT_H