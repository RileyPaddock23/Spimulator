#include "Itypes.h"

void IJtype(int opcode, int rs, int rt, std::string imm){
    switch (opcode){
        case 8:{//addi
            std::string raw_sum = add(registers[rs],fill(imm, 16, false));
            std::string sum = raw_sum.substr(1,32);
            int carry = raw_sum[0]-'0';
            try{
                if(registers[rs][0] == registers[rt][0] && registers[rs][0] != sum[0]) throw OverflowError();
            }catch(OverflowError O){
                std::cout << O.what() << '\n';
            }
            registers[rt] = sum;
            return;
        }
        case 9:{//addiu
            std::string raw_sum = add(registers[rs],fill(imm, 16, true));
            std::string sum = raw_sum.substr(1,32);

            registers[rt] = sum;
            return;
        }
        case 15:{//lui
            std::string res = imm;
            for(int i = 0;i<16;i++){
                res += "0";
            }
            registers[rt]=res;
            return;
        }
        case 12:{//and
            std::string res = "";
            std::string full_imm = fill(imm, 16, false);
            for(int i = 0;i<32;i++){
                if(full_imm[i] == '1' &&  registers[rs][i] == '1'){
                    res += "1";
                }else{
                    res += "0";
                }
            }
            registers[rt] = res;
            return;
        }
        case 13:{//ori
            std::string res = "";
            std::string full_imm = fill(imm, 16, false);
            for(int i = 0;i<32;i++){
                if(registers[rs][i] == '0' &&  full_imm[i] == '0'){
                    res += "0";
                }else{
                    res += "1";
                }
            }
            registers[rt] = res;
            return;
        }
        case 10:{//slti
            if(bin2dec(registers[rs]) < bin2dec(fill(imm,16,false))){
                registers[rt] =  hex2bin("0x00000001");
            }else{
                registers[rt] =  hex2bin("0x00000000");
            }
            return;
        }
        case 11:{//sltiu
            if(bin2decu(registers[rs]) < bin2decu(fill(imm,16,false))){
                registers[rt] =  hex2bin("0x00000001");
            }else{
                registers[rt] =  hex2bin("0x00000000");
            }
            return;
        }
        case 4:{//beq
            if(registers[rs] == registers[rt]){
                program_counter = bin2dec(imm,16);
            }else{
                program_counter ++;
            }
            return;
        }
        case 5:{//bne
            if(registers[rs] != registers[rt]){
                program_counter = bin2dec(imm,16);
            }else{
                program_counter ++;
            }
            return;
        }
        case 2:{//j
            program_counter = bin2dec(imm,6);
            return;
        }
        case 3:{//jal
            registers[31] = program_counter;
            program_counter = bin2dec(imm,6);
            return;
        }
        case 35:{//lw
            long start_add = bin2decu(registers[rs]);
            long desired_pos = start_add + bin2decu(fill(imm,16,false));
            std::ifstream stack;
            stack.open("memory.txt");
            long curr_add = -1;
            std::string curr_mem;
            std::string result;
            while(curr_add < desired_pos+4){
                if(curr_add < desired_pos){
                    getline(stack,curr_mem);
                    curr_add ++;
                }else{
                    result += curr_mem;//Add byte to result
                    getline(stack,curr_mem);
                    curr_add++;
                }
            }
            stack.close();
            registers[rt] = result;
            return;
        }
        case 36:{//lbu
            long start_add = bin2decu(registers[rs]);
            long desired_pos = start_add + bin2decu(fill(imm,16,false));
            std::ifstream stack;
            stack.open("memory.txt");
            long curr_add = -1;
            std::string curr_mem;
            std::string result = "000000000000000000000000";
            while(curr_add < desired_pos+1){
                if(curr_add < desired_pos){
                    getline(stack,curr_mem);
                    curr_add ++;
                }else{
                    result += curr_mem;//Add byte to result
                    getline(stack,curr_mem);
                    curr_add++;
                }
            }
            stack.close();
            registers[rt] = result;
            return;
        }
        case 37:{//lhu
            long start_add = bin2decu(registers[rs]);
            long desired_pos = start_add + bin2decu(fill(imm,16,false));
            std::ifstream stack;
            stack.open("memory.txt");
            long curr_add = -1;
            std::string curr_mem;
            std::string result = "0000000000000000";
            while(curr_add < desired_pos+2){
                if(curr_add < desired_pos){
                    getline(stack,curr_mem);
                    curr_add ++;
                }else{
                    result += curr_mem;//Add byte to result
                    getline(stack,curr_mem);
                    curr_add++;
                }
            }
            stack.close();
            registers[rt] = result;
            return;
        }
        case 43:{//sw
            //M[rs+imm] = rt
            long start_add = bin2decu(registers[rs]);
            long desired_pos = start_add + bin2decu(fill(imm,16,false));
            std::cout << desired_pos <<"\n";
            std::ofstream stack;
            stack.open("memory.txt",std::ios::in | std::ios::out | std::ios::ate);
            std::string byte_i;
            for(int i = 0; i<4;i++){
                stack.seekp(9*desired_pos + (9*i));
                byte_i = registers[rt].substr(8*i,8)+"\n";
                stack.write(byte_i.c_str(),9);
            }
            stack.close();
            return;
        }
        case 40:{//sb
            //M[rs+imm] = rt
            long start_add = bin2decu(registers[rs]);
            long desired_pos = start_add + bin2decu(fill(imm,16,false));
            std::cout << desired_pos <<"\n";
            std::ofstream stack;
            stack.open("memory.txt",std::ios::in | std::ios::out | std::ios::ate);
            std::string byte_i;

            stack.seekp(9*desired_pos);
            byte_i = registers[rt].substr(24,8)+"\n";
            stack.write(byte_i.c_str(),9);

            stack.close();
            return;
        }
        case 41:{//sh
            long start_add = bin2decu(registers[rs]);
            long desired_pos = start_add + bin2decu(fill(imm,16,false));
            std::cout << desired_pos <<"\n";
            std::ofstream stack;
            stack.open("memory.txt",std::ios::in | std::ios::out | std::ios::ate);
            std::string byte_i;
            for(int i = 0; i<2;i++){
                stack.seekp(9*desired_pos + (9*i));
                byte_i = registers[rt].substr(16+(8*i),8)+"\n";
                stack.write(byte_i.c_str(),9);
            }
            stack.close();
            return;
        }
    }
    
}
