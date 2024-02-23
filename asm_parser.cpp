#include "asm_parser.h"

std::string rg2bin(std::string* rgsts, int num_rg){
    std::string bin = "";
    for(int i = 0; i< num_rg;i++){
        if (rgsts[i] == "$zero")
            bin += "00000";
        else if (rgsts[i] == "$at")
            bin += "00001";
        else if (rgsts[i] == "$v0")
            bin += "00010";
        else if (rgsts[i] == "$v1")
            bin += "00011";
        else if (rgsts[i] == "$a0")
            bin += "00100";
        else if (rgsts[i] == "$a1")
            bin += "00101";
        else if (rgsts[i] == "$a2")
            bin += "00110";
        else if (rgsts[i] == "$a3")
            bin += "00111";
        else if (rgsts[i] == "$t0")
            bin += "01000";
        else if (rgsts[i] == "$t1")
            bin += "01001";
        else if (rgsts[i] == "$t2")
            bin += "01010";
        else if (rgsts[i] == "$t3")
            bin += "01011";
        else if (rgsts[i] == "$t4")
            bin += "01100";
        else if (rgsts[i] == "$t5")
            bin += "01101";
        else if (rgsts[i] == "$t6")
            bin += "01110";
        else if (rgsts[i] == "$t7")
            bin += "01111";
        else if (rgsts[i] == "$s0")
            bin += "10000";
        else if (rgsts[i] == "$s1")
            bin += "10001";
        else if (rgsts[i] == "$s2")
            bin += "10010";
        else if (rgsts[i] == "$s3")
            bin += "10011";
        else if (rgsts[i] == "$s4")
            bin += "10100";
        else if (rgsts[i] == "$s5")
            bin += "10101";
        else if (rgsts[i] == "$s6")
            bin += "10110";
        else if (rgsts[i] == "$s7")
            bin += "10111";
        else if (rgsts[i] == "$t8")
            bin += "11000";
        else if (rgsts[i] == "$t9")
            bin += "11001";
        else if (rgsts[i] == "$k0")
            bin += "11010";
        else if (rgsts[i] == "$k1")
            bin += "11011";
        else if (rgsts[i] == "$gp")
            bin += "11100";
        else if (rgsts[i] == "$sp")
            bin += "11101";
        else if (rgsts[i] == "$fp")
            bin += "11110";
        else if (rgsts[i] == "$ra")
            bin += "11111";
    }
    return bin;
}

std::string prep_Rtype(std::string* rgsts, int shamt){
    std::string bin = "";
    bin += "000000"; //opcode
    bin+= rg2bin(rgsts,3);//registers
    bin += dec2bin(shamt).substr(27,5);//shamt
    return bin;
}
std::string prep_Itype(std::string* rgsts, long imm){
    std::string bin = "";
    bin+= rg2bin(rgsts,2);
    bin += dec2bin(imm).substr(16,16);
    return bin;
}

std::string cmd_to_bin(std::string inst, std::string* rgsts, int shamt,long imm)
{
    std::string bin = "";
    if (inst == "add"){
        bin += prep_Rtype(rgsts, shamt);
        bin += "100000";}
    else if (inst == "addu")
        {bin += prep_Rtype(rgsts, shamt);
        bin += "100001";}
    else if (inst == "and")
        {bin += prep_Rtype(rgsts, shamt);
        bin += "100100";}
    else if (inst == "nor")
        {bin += prep_Rtype(rgsts, shamt);
        bin += "100111";}
    else if (inst == "or")
        {bin += prep_Rtype(rgsts, shamt);
        bin += "100101";}
    else if (inst == "slt")
        {bin += prep_Rtype(rgsts, shamt);
        bin += "101010";}
    else if (inst == "sltu")
        {bin += prep_Rtype(rgsts, shamt);
        bin += "101011";}
    else if (inst == "sll")
        {bin += prep_Rtype(rgsts, shamt);
        bin += "000000";}
    else if (inst == "sra")
        {bin += prep_Rtype(rgsts, shamt);
        bin += "000011";}
    else if (inst == "srl")
        {bin += prep_Rtype(rgsts, shamt);
        bin += "000010";}
    else if (inst == "sub")
        {bin += prep_Rtype(rgsts, shamt);
        bin += "100010";}
    else if (inst == "subu")
        {bin += prep_Rtype(rgsts, shamt);
        bin += "010011";}
    else if(inst == "addi")
        {bin += "001000";
        bin += prep_Itype(rgsts,imm);}
    else if(inst == "addiu")
        {bin += "001001";
        bin += prep_Itype(rgsts,imm);}
    else if(inst == "lui")
        {bin += "001111";
        bin += prep_Itype(rgsts,imm);}
    else if(inst == "andi")
        {bin += "001100";
        bin += prep_Itype(rgsts,imm);}
    else if(inst == "ori")
        {bin += "001101";
        bin += prep_Itype(rgsts,imm);}
    else if(inst == "slti")
        {bin += "001010";
        bin += prep_Itype(rgsts,imm);}
    else if(inst == "sltiu")
        {bin += "001011";
        bin += prep_Itype(rgsts,imm);}
    else if(inst == "beq")
        {bin += "000100";
        bin += prep_Itype(rgsts,imm);}
    else if(inst == "bne")
        {bin += "000101";
        bin += prep_Itype(rgsts,imm);}
    else if(inst == "lw")
        {bin += "100011";
        bin += prep_Itype(rgsts,imm);}
    else if(inst == "sw")
        {bin += "101011";
        bin += prep_Itype(rgsts,imm);}
    else if(inst == "j")
        {bin += "000010";
            bin += dec2bin(imm).substr(6,26);}
    else if(inst == "jal")
        {bin += "000011";
        bin += dec2bin(imm).substr(6,26);}
    
    return bin;
}

void write_machine(std::string filename){
    std::ifstream cmdFile;
    std::ofstream rawFile;
    cmdFile.open(filename);
    rawFile.open("text.txt");
    std::string curr_cmd;
    std::string curr_read;
    std::string cmd;
    std::string rgsts[3];
    while(getline(cmdFile,curr_cmd)){
        int shamt = 0;
        long imm = 0;
        int startIndex = 0;
        int endIndex = 0;

        //Get command name
        endIndex = curr_cmd.find(' ',startIndex);
        cmd = curr_cmd.substr(startIndex,endIndex-startIndex);
        std::cout << "Curr read: "<<cmd<<"\n";

        //Get rs or jump add
        startIndex = endIndex+1;
        endIndex = curr_cmd.find(' ',startIndex);
        curr_read = curr_cmd.substr(startIndex,endIndex-startIndex);
        std::cout << "Curr read: "<<curr_read<<"\n";
        if(curr_read[0]=='$'){
            rgsts[0] = curr_read;

            //Get rt
            startIndex = endIndex+1;
            endIndex = curr_cmd.find(' ',startIndex);
            rgsts[1] = curr_cmd.substr(startIndex,endIndex-startIndex);
            std::cout << "Curr read: "<<curr_read<<"\n";
            //get either rd or imm or shamt
            startIndex = endIndex+1;
            endIndex = curr_cmd.find(" ",startIndex);
            curr_read = curr_cmd.substr(startIndex,endIndex-startIndex);
            std::cout << "Curr read: "<<curr_read<<"\n";
            if(curr_read[0]=='$'){
                rgsts[2] = curr_read;
            }else{
                if(cmd == "sll" | cmd == "srl" |cmd == "sra"){
                    shamt = stoi(curr_read);
                }else{
                    imm = stol(curr_read);
                }
            }

        }else{
            imm = stol(curr_read);
        }

        std::string bin_cmd = cmd_to_bin(cmd,rgsts,shamt, imm);
        rawFile << bin2hex(bin_cmd)<<"\n";
    }
    cmdFile.close();
    rawFile << "exit";
    rawFile.close();

}



// std::string Rrgsts[3] = {"$t1", "$t2", "$t2"};
// std::string Irgsts[2];
// std::string cmd;
// cmdFile.open(file);

// Irgsts[0] = "$zero";
// Irgsts[1] = "$t0";
// cmd = cmd_to_bin('I',"addi", Irgsts, 0, n);
// cmdFile << bin2hex(cmd) << "\n";

// Irgsts[1] = "$t1";
// cmd = cmd_to_bin('I',"addi", Irgsts, 0, 0);
// cmdFile << bin2hex(cmd) << "\n";

// Irgsts[1] = "$t2";
// cmd = cmd_to_bin('I',"addi", Irgsts, 0, 0);
// cmdFile << bin2hex(cmd) << "\n";

// Irgsts[0] = "$t1";
// Irgsts[1] = "$t1";
// cmd = cmd_to_bin('I',"addi", Irgsts, 0, 1);
// cmdFile << bin2hex(cmd) << "\n";

// cmd = cmd_to_bin('R',"add", Rrgsts);
// cmdFile << bin2hex(cmd) << "\n";

// Irgsts[0] = "$t0";
// cmd = cmd_to_bin('I',"beq", Irgsts, 0, 7);
// cmdFile << bin2hex(cmd) << "\n";

// cmd = cmd_to_bin('J',"j", Irgsts, 0, 3);
// cmdFile << bin2hex(cmd) << "\n";