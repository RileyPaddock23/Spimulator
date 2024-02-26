// add, addi, addu, sub, sll, srl, sra, lui, and, andi, ori, nor, slt
#include "Rtypes.h"
#include "Itypes.h"
#include "asm_parser.h"
#include <stdlib.h> //needed for rand

bool do_cmd(std::string hex_cmd)
{
    std::string bin = hex2bin(hex_cmd);
    int opcode = bin2dec(bin.substr(0, 6), 26);
    if (opcode == 0)
    {
        // R-type
        int rs = bin2dec(bin.substr(6, 5), 27);
        int rt = bin2dec(bin.substr(11, 5), 27);
        int rd = bin2dec(bin.substr(16, 5), 27);
        int shamt = bin2dec(bin.substr(21, 5), 27);
        int func = bin2dec(bin.substr(26, 6), 26);
        Rtypes(func, rs, rt, rd, shamt);
    }
    else if (opcode == 2 | opcode == 3)
    {
        // J-type
        int rs = 0;
        int rt = 0;
        std::string imm = bin.substr(6, 26);
        IJtype(opcode, rs, rt, imm);
        return true;
    }
    else
    {
        // I-type
        int rs = bin2dec(bin.substr(6, 5), 27);
        int rt = bin2dec(bin.substr(11, 5), 27);
        std::string imm = bin.substr(16, 16);
        IJtype(opcode, rs, rt, imm);
        if (opcode == 4 || opcode == 5)
            return true;
    }

    return false;
}

void do_file(std::string filename)
{
    std::ifstream cmdFile;
    cmdFile.open(filename);
    std::string curr_cmd;
    int curr_pos = -1; // Always represents the current cmd stored in curr_cmd
    while (true)
    {
        if (curr_pos != program_counter)
        {
            getline(cmdFile, curr_cmd);
            curr_pos++;
        }
        else
        {
            if (curr_cmd == "exit")
            {
                cmdFile.close();
                return;
            }
            else
            {
                // If our curr command matches where the pc says we should be
                // then we execute the command and track whether we madea jump
                bool jump = do_cmd(curr_cmd);
                if (jump)
                {
                    // If we did jump we need to locate the correct command
                    // So we search again from the start of the program
                    cmdFile.seekg(0);
                    curr_pos = -1;
                }
                else
                {
                    // If we didn't jump then we just go to the next command
                    program_counter++;
                }
            }
        }
    }
}

// When testing make this function the known correct output
// It is currently filled as an example to test andi
long true_val_func(std::string rs, std::string rt, long imm = 0, int shamt = 0, std::string rd = " ")
{
    return bin2dec(rs) & imm;
}

void test_cmd(int tests, std::string command, int shamt = 0, long imm = 0)
{
    for (int i = 0; i < tests; i++)
    {
        long a = (std::rand() % 20000) - 10000;
        long b = (std::rand() % 20000) - 10000;
        registers[4] = dec2bin(a);

        std::string rgsts[2] = {"$a0", "$t0"};
        std::string cmd = cmd_to_bin("andi", rgsts, 0, b);
        std::cout << "Test " << i << ": ";
        do_cmd(bin2hex(cmd));
        long corr_res = true_val_func(registers[4], registers[5], b);
        if (corr_res != bin2dec(registers[5]))
        {
            std::cout << "Failed\n";
            std::cout << "Input one: " << bin2dec(registers[4]) << ", " << registers[4] << "\n";
            std::cout << "Input two: " << b << ", " << dec2bin(b) << "\n";
            std::cout << "Incorrect Output: \n"
                      << bin2dec(registers[5]) << ":\n\t" << registers[5] << "\n";
            std::cout << "Correct Output: \n"
                      << corr_res << ":\n\t" << dec2bin(corr_res) << "\n";
        }
        else
        {
            std::cout << "Passed\n";
        }
    }
}

int main()
{
    // Our stack uses Big Endian and each line represents
    // A single adress of memory, so storing 8 bits, or a byte
    std::string file = "text.txt";
    registers[0] = dec2bin(0);  // setting $zero
    registers[29] = dec2bin(0); // setting $sp

    write_machine("test_read.txt");

    do_file(file);
    std::cout << bin2dec(registers[10]) << "\n";
}