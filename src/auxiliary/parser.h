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

vector<FmlaNode> parse_fmla(string fmla_str);

#endif // PARSER_H