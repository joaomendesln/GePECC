#ifndef PRE_PROCESS_H
#define PRE_PROCESS_H

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <chrono>
#include <ctime>

#include "../definitions/definitions.h"
#include "parser.h"

using namespace std;

vector<string> open_file(string file_path);

map<string, int> converts_str_to_symb_map(string symb_line);

pair<map<string, int>, map<string, int>> pre_process_symbols();

map<string, int> pre_process_function_symbs();

map<string, int> pre_process_predicate_symbs();

map<string, int> pre_process_language_symbs();

map<string, int> pre_process_no_skolem_symbs();

SignedFmla converts_str_to_signed_fmla(string signed_fmla_str, int line_idx);

vector<SignedFmla> converts_str_to_signed_fmla_list(string list, int line_idx);

TblRule converts_str_to_single_expansion_rule(string line, int line_idx);

vector<TblRule> pre_process_expansion_rules_input(string file_path);

vector<SignedFmla> converts_str_to_signed_fmla_input(string file_path);

set<string> pre_process_skolem_symbs();

#endif // PRE_PROCESS_H