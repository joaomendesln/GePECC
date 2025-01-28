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

using namespace std;

map<string, int> process_function_symbs();

map<string, int> process_predicate_symbs();

#endif // PRE_PROCESS_H