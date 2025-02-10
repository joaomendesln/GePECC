#ifndef PRE_PROCESS_H
#define PRE_PROCESS_H

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <filesystem>
#include <fstream>
#include <sstream>

#include "../definitions/definitions.h"
#include "parser.h"

using namespace std;

vector<string> open_file(string file_path);

map<string, int> pre_process_symb_line(string symb_line);

map<string, int> pre_process_function_symbs();

map<string, int> pre_process_predicate_symbs();

map<string, int> pre_process_language_symbs();

SignedFmla pre_process_signed_fmla(string signed_fmla_str);

vector<SignedFmla> pre_process_signed_fmla_list(string list);

TblRule pre_process_single_expansion_rule(string line);

vector<TblRule> pre_process_expansion_rules_input();

vector<SignedFmla> pre_process_signed_fmla_input();

set<string> pre_process_skolem_symbs();

#endif // PRE_PROCESS_H