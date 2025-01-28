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

using namespace std;

map<string, int> get_function_symbs();

map<string, int> get_predicate_symbs();

bool is_function_symb(string symb);

bool is_predicate_symb(string symb);

// vector<FmlaNode> get_subfmla(vector<FmlaNode> fmla, int subfmla_root);

// vector<FmlaNode> left_gen_subfmla(vector<FmlaNode> fmla);

// vector<FmlaNode> right_gen_subfmla(vector<FmlaNode> fmla);

// vector<FmlaNode> join_subfmla(vector<FmlaNode> fmla, int parent_node, int child_position, vector<FmlaNode> subfmla);

// bool fmla_equality(vector<FmlaNode> fmla1, vector<FmlaNode> fmla2);

// set<string> get_fmla_vars(vector<FmlaNode> fmla);

// string get_new_var(set<string> vars);

// bool is_predicate_symb(vector<FmlaNode> fmla);

// bool is_function_symb(vector<FmlaNode> fmla);

// bool is_unary_function_symb(vector<FmlaNode> fmla);

// bool is_atomic(vector<FmlaNode> fmla);

#endif // SYNTAX_H