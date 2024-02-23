#ifndef GLOBALS
#define GLOBALS

#include <string>
#include <exception>
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <fstream>

class OverflowError : public std::exception {
    public:
        std::string what () {
            return "Overflow Error: Output has opposite sign";
        }
};

extern std::string registers[32];

extern long program_counter;

std::string flip(std::string bin);

std::string add(std::string bin1, std::string bin2);

std::string fill(std::string imm, int len = 16, bool u = false);

const char* hex_char_to_bin(char c);

std::string hex2bin(std::string hex);

long bin2decu(std::string bin);

long bin2dec(std::string bin, int fill = 0);

//Expects a positive inpput
std::string dec2binu(long dec);

//Uses 2's compliment version of signed binary
std::string dec2bin(long dec);

std::string bin2hex(std::string bin);
#endif