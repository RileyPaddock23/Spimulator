#include "globals.h"

std::string registers[32];

long program_counter = 0;

std::string flip(std::string bin){
    std::string flipped = "";
    for(int i =0;i<32;i++){
        if(bin[i]=='1'){
            flipped += "0";
        }else{
            flipped += "1";
        }
    }
    return flipped;
}

std::string add(std::string bin1, std::string bin2){
    std::string res = "";
    int carry = 0;
    for(int i = 31;i>=0;i--){
        int b1 = bin1[i]-'0';
        int b2 = bin2[i]-'0';
        res.insert(0,std::to_string((carry+b1+b2)%2));
        carry = (carry+b1+b2)/2;
    }
    std::string with_carry = std::to_string(carry)+res;
    return with_carry;
}

std::string fill(std::string imm, int len, bool u){
    std::string fill_imm = "";
    for(int i = 0;i < len;i++){
        if(imm[0]=='1' && !u){
            fill_imm += "1";
        }else{
            fill_imm += "0";
        }
        
    }
    return fill_imm+imm;
}

const char* hex_char_to_bin(char c)
{
    // TODO handle default / error
    switch(toupper(c))
    {
        case '0': return "0000";
        case '1': return "0001";
        case '2': return "0010";
        case '3': return "0011";
        case '4': return "0100";
        case '5': return "0101";
        case '6': return "0110";
        case '7': return "0111";
        case '8': return "1000";
        case '9': return "1001";
        case 'A': return "1010";
        case 'B': return "1011";
        case 'C': return "1100";
        case 'D': return "1101";
        case 'E': return "1110";
        case 'F': return "1111";
        default: throw std::invalid_argument("Unrecognized Hex Character");
    }
}

std::string hex2bin(std::string hex){
    std::string bin = "";
    for(int i = 2; i < 10; i++){
        const char* conv_byte = hex_char_to_bin(hex[i]);
        bin += conv_byte;
    }
    return bin;
}

long bin2decu(std::string bin){
    long double dec = 0;
    for(int i =0;i<32;i++){
        if (bin[31-i] == '1'){
            dec += pow(2,i);
        }
    }
    return dec;
}

long bin2dec(std::string bin, int fill){
    std::string buff = "";
    for(int i = 0; i<fill;i++){
        buff+="0";
    }
    bin = buff+bin;

    if (bin[0] == '0'){
        return bin2decu(bin);
    }

    std::string minusOne = hex2bin("0xFFFFFFFF");
    std::string pos = add(bin,minusOne).substr(1,32);
    return -1*bin2decu(flip(pos));
}



//Expects a positive inpput
std::string dec2binu(long dec){
    std::string bin = "";
    long remaining = dec;
    for(int i = 31;i>=0;i--){
        if (pow(2,i) <= remaining){
            bin+="1";
            remaining -= pow(2,i);
        }else{
            bin +="0";
        }
    }
    return bin;
}

//Uses 2's compliment version of signed binary
std::string dec2bin(long dec){
    if(dec > 0){
        return dec2binu(dec);
    }

    std::string flipped = flip(dec2binu(-1*dec));
    std::string one = hex2bin("0x00000001");
    return add(flipped,one).substr(1,32);
}

std::string bin2hex(std::string bin){
    //Assume it is always 32
    char hexChars[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
    std::string hex = "0x";
    for(int i = 0;i<8;i++){
        hex+= hexChars[bin2dec(bin.substr(4*i,4),28)];
    }
    return hex;
}