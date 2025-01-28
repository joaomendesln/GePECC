#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <set>
#include <queue>
#include <map>
#include <string>
#include <utility>
#include <tuple>
#include <iostream>

#include "../definitions/definitions.h"
#include "syntax.h"
#include "print.h"
#include "pre_process.h"

using namespace std;

vector<FmlaNode> parse_fmla(string fmla);

vector<string> split(string fmla, char delimiter);

vector<string> get_tokens(string fmla);

bool has_non_unicode(string str);

pair<int, string> get_pred_symb(vector<string> tokens_fmla);

pair<int, string> parse_term(vector<string> tokens_term);

vector< vector<string>> split_by_symb_idx(vector<string> tokens_fmla, int symb_idx);

vector<string> remove_external_parentheses(vector<string> str);

#endif // PARSER_H