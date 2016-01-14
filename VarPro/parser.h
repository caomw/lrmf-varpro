#ifndef _PARSER_H_
#define _PARSER_H_

#include "options.h"

void outputCommands();
void parse_options_from_args(int argc, char* argv[], options_t &general_opts, optimizer_opts_t &optimier_opts);
void parse_options_manually(options_t &general_opts, bool START_FROM_SCRATCH = 0);

#endif