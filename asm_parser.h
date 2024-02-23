#ifndef ASM_PARSE
#define ASM_PARSE
#include "globals.h"
std::string rg2bin(std::string* rgsts, int num_rg);
std::string prep_Rtype(std::string* rgsts, int shamt);
std::string prep_Itype(std::string* rgsts, long imm);
std::string cmd_to_bin(std::string inst, std::string* rgsts, int shamt = 0,long imm = 0);
void write_machine(std::string filename);

#endif