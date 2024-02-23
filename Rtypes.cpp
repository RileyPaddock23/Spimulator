#include "Rtypes.h"

void Rtypes(int func, int rs ,int rt, int rd, int shamt){
    switch(func){
        case 8:{//jr
            program_counter = bin2dec(registers[rs]);
        }
        case 32://add
        {
            std::string raw_sum = add(registers[rs],registers[rt]);
            std::string sum = raw_sum.substr(1,32);
            int carry = raw_sum[0]-'0';
            try{
                if(registers[rs][0] == registers[rt][0] && registers[rs][0] != sum[0]) throw OverflowError();
            }catch(OverflowError O){
                std::cout << O.what() << '\n';
            }
            registers[rd] = sum;
            return;
        }
        case 33://addu
        {
            std::string raw_sum = add(registers[rs],registers[rt]);
            std::string sum = raw_sum.substr(1,32);

            registers[rd] = sum;
            return;
        }
        case 34:{//sub
            
            std::string minus_rt = add(flip(registers[rt]),dec2bin(1)).substr(1,32);
            std::string raw_diff = add(registers[rs],minus_rt);
            std::string diff = raw_diff.substr(1,32);
            try{
                if(diff[0] == registers[rt][0] && registers[rs][0] != diff[0]) throw OverflowError();
            }catch(OverflowError O){
                std::cout << O.what() << '\n';
            }
            registers[rd] = diff;
            return;
        }
        case 35:{//subu
            std::string minus_rt = add(flip(registers[rt]),dec2bin(1)).substr(1,32);
            std::string raw_diff = add(registers[rs],minus_rt);
            std::string diff = raw_diff.substr(1,32);
            
            registers[rd] = diff;
            return;
        }
        case 0:{//sll
            std::string res = "";
            for(int i = shamt;i<32;i++){
                res += registers[rt][i];
            }
            for(int i = 0;i<shamt;i++){
                res += "0";
            }
            registers[rd] = res;
            return;
        }
        case 2:{//srl
            std::string res = "";
            for(int i = 0;i<shamt;i++){
                res += "0";
            }
            for(int i=0;i<32-shamt;i++){
                res += registers[rt][i];
            }
            registers[rd] = res;
            return;
        }
        case 3:{//sra
            std::string res = "";
            for(int i = 0;i<shamt;i++){
                res += registers[rt][0];
            }
            for(int i=0;i<32-shamt;i++){
                res += registers[rt][i];
            }
            registers[rd] = res;
            return;
        }
        case 36:{//and
            std::string res = "";
            for(int i = 0;i<32;i++){
                if(registers[rt][i] == '1' &&  registers[rs][i] == '1'){
                    res += "1";
                }else{
                    res += "0";
                }
            }
            registers[rd] = res;
            return;
        }
        case 39:{//nor
            std::string res = "";
            for(int i = 0;i<32;i++){
                res+= std::to_string(((registers[rt][i]+registers[rs][i])-'0')%2);
            }
            registers[rd] = res;
            return;
        }
        case 37:{//or
            std::string res = "";
            for(int i = 0;i<32;i++){
                if(registers[rt][i] == '1' || registers[rs][i]=='1'){
                    res += "1";
                }else{
                    res += "0";
                }
            }
            registers[rd] = res;
            return;
        }
        case 42:{//slt
            if(bin2dec(registers[rs]) < bin2dec(registers[rt])){
                registers[rd] =  hex2bin("0x00000001");
            }else{
                registers[rd] =  hex2bin("0x00000000");
            }
            return;
            
        }
        case 43:{//sltu
            if(bin2decu(registers[rs]) < bin2decu(registers[rt])){
                registers[rd] =  hex2bin("0x00000001");
            }else{
                registers[rd] =  hex2bin("0x00000000");
            }
            return;
        }
    }
}