#ifndef PARSING_H
#define PARSING_H
#include <iostream>
#include "rig_utils.h"

using results_t = std::vector<outcome_t>;

results_t parse_tree(std::istream*);

results_t parse_list(std::istream*);

#endif