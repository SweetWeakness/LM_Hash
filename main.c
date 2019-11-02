#include <iostream>
#include <algorithm>
#include <bitset>
#include <sstream>
#include <stdint.h>



// https://en.wikipedia.org/wiki/DES_supplementary_material
// arrays of permutation
short permutation[64] =
        {58, 50, 42, 34, 26, 18, 10, 2, 60, 52, 44, 36, 28, 20, 12, 4,
         62, 54, 46, 38, 30, 22, 14, 6, 64, 56, 48, 40, 32, 24, 16, 8,
         57, 49, 41, 33, 25, 17, 9, 1, 59, 51, 43, 35, 27, 19, 11, 3,
         61, 53, 45, 37, 29, 21, 13, 5, 63, 55, 47, 39, 31, 23, 15, 7};

short back_permutation[64] =
        {40, 8, 48, 16, 56, 24, 64, 32, 39, 7, 47, 15, 55, 23, 63, 31,
         38, 6, 46, 14, 54, 22, 62, 30, 37, 5, 45, 13, 53, 21, 61, 29,
         36, 4, 44, 12, 52, 20, 60, 28, 35, 3, 43, 11, 51, 19, 59, 27,
         34, 2, 42, 10, 50, 18, 58, 26, 33, 1, 41, 9, 49, 17, 57, 25};

short permutation_c[28] =
        {57, 49, 41, 33, 25, 17, 9, 1, 58, 50, 42, 34, 26, 18,
         10, 2, 59, 51, 43, 35, 27, 19, 11, 3, 60, 52, 44, 36};

short permutation_d[28] =
        {63, 55, 47, 39, 31, 23, 15, 7, 62, 54, 46, 38, 30, 22,
         14, 6, 61, 53, 45, 37, 29, 21, 13, 5, 28, 20, 12, 4};

short shifting_iteration[16] =
        {1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1};

short key_permutation[48] =
        {14, 17, 11, 24, 1, 5, 3, 28, 15, 6, 21, 10, 23, 19, 12, 4,
         26, 8, 16, 7, 27, 20, 13, 2, 41, 52, 31, 37, 47, 55, 30, 40,
         51, 45, 33, 48, 44, 49, 39, 56, 34, 53, 46, 42, 50, 36, 29, 32};

short expanding_permutation[48] =
        {32, 1, 2, 3, 4, 5,
         4, 5, 6, 7, 8, 9,
         8, 9, 10, 11, 12, 13,
         12, 13, 14, 15, 16, 17,
         16, 17, 18, 19, 20, 21,
         20, 21, 22, 23, 24, 25,
         24, 25, 26, 27, 28, 29,
         28, 29, 30, 31, 32, 1};

short permutation_b[32] =
        {16, 7, 20, 21, 29, 12, 28, 17,
         1, 15, 23, 26, 5, 18, 31, 10,
         2, 8, 24, 14, 32, 27, 3, 9,
         19, 13, 30, 6, 22, 11, 4, 25};

std::string lazy_conversion[16] =
        {"0000", "0001", "0010", "0011",
         "0100", "0101", "0110", "0111",
         "1000", "1001", "1010", "1011",
         "1100", "1101", "1110", "1111"};

std::string lazy_conversion_2[16] =
        {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F"};

short conversion_s[512] =
        // b is horizontal, a is vertical (positions in this array)
        {14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7,
         0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8, // B'_1
         4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0,
         15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13, //last is 63

         15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10,
         3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5, // B'_2
         0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15,
         13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9,

         10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8,
         13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1, // B'_3
         13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7,
         1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12,

         7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15,
         13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9, // B'_4
         10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4,
         3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14,

         2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9,
         14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6, // B'_5
         4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14,
         11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3,

         12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11,
         10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8, // B'_6
         9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6,
         4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13,

         4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1,
         13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6, // B'_7
         1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2,
         6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12,

         13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7,
         1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2, // B'_8
         7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8,
         2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11};


//******************************** END OF GLOBAL VARS **************************************


int GetElementInS(int i, int a, int b){ // works
    return conversion_s[(i - 1) * 64 + a * 16 + b];
}

void ShiftLeft(std::string& str, int k) { // works
    // k == 1, 2 only
    if (k == 2) {
        int tmp1 = str[0];
        int tmp2 = str[1];
        for (int i = 0; i < str.size() - 2; ++i) {
            str[i] = str[i + 2];
        }
        str[str.size() - 2] = tmp1;
        str[str.size() - 1] = tmp2;
    } else {
        int tmp = str[0];
        for (int i = 0; i < str.size() - 1; ++i) {
            str[i] = str[i + 1];
        }
        str[str.size() - 1] = tmp;
    }
}

const std::string GetNewKey(int i, std::string& C, std::string& D) { // works
    std::string out, tmp;

    // shifts left C and D arrays
    ShiftLeft(C, shifting_iteration[i - 1]); // it should be less by 1
    ShiftLeft(D, shifting_iteration[i - 1]); // it should be less by 1

    tmp += C + D;

    // gets new k_i (48 bites) using C and D
    for (int j = 0; j < 48; ++j) {
        out += tmp[key_permutation[j] - 1]; // it should be less by 1
    }

    return out;
}

std::string MakeBinaryString(const std::string& str) { // works
    std::string binary_string;

    // converts string to bites (also string)
    for (int i = 0; i < str.size(); ++i) {
        binary_string += std::bitset<8>(str[i]).to_string();
    }

    return binary_string;
}

std::string StringXOR(const std::string& a, const std::string& b) { // works
    std::string out;

    for (int i = 0; i < a.size(); ++i) {
        out += (a[i] != b[i] ? "1" : "0");
    }

    return out;
}

std::string ConversionOfB(int i, const std::string& B_i) { // works
    // B uses 6 bites
    // gets position (a, b) in conversion_s
    int a = (B_i[0] - 48) * 2 + (B_i[5] - 48);
    int b = (B_i[1] - 48) + (B_i[2] - 48) * 2 + (B_i[3] - 48) * 4 + (B_i[4] - 48) * 8;

    // gets B' (but in int form)
    int S_i = GetElementInS(i, a, b);

    // gets B' using int form (to string)
    return lazy_conversion[S_i];
}

std::string FeistelFunction(std::string& str, const std::string& key_string) { // works
    std::string expand_str;

    // 1. expanding input string (36 -> 48 bites)
    for (int i = 0; i < 48; ++i) {
        expand_str += str[expanding_permutation[i] - 1]; // it should be less by 1
    }

    // 2. key_string xor expand_str == B
    std::string B = StringXOR(expand_str, key_string);

    // 3. conversion of B (48 bites) using S (gets B')
    std::string out, out_tmp; // == Feistel result and B'
    for (int i = 0; i < 8; ++i) {
        out_tmp += ConversionOfB(i + 1, B.substr(i * 6, 6));
    }

    // 4. permutation of B' (32 bites)
    for (int i = 0; i < 32; ++i) {
        out += out_tmp[permutation_b[i] - 1];  // it should be less by 1
    }

    return out;
}

std::string DES(const std::string& str_in, std::string& key) { // works
    std::string str, C, D;
    std::string tmp_str = MakeBinaryString(str_in);

    // 1.1 permutation for data block
    for (int i = 0; i < tmp_str.size(); ++i) {
        str += tmp_str[permutation[i] - 1]; // it should be less by 1
    }

    // 1.2 permutation for C and D arrays (for future keys)
    // their size is 28 (28 bites * 2 = 56 for each k_i)
    for (int i = 0; i < 28; ++i){
        C += key[permutation_c[i] - 1];  // it should be less by 1
        D += key[permutation_d[i] - 1];  // it should be less by 1
    }

    // 2. "16 cycles of Feistel function"
    std::string L_old = str.substr(0, 32);
    std::string R_old = str.substr(32, 32);
    for (int i = 1; i <= 16; ++i) {
        // 0. tmp_str = L_(i-1)
        tmp_str = L_old;

        // 1. L_i = R_(i-1)
        L_old = R_old;

        // 2. R_i = L_(i-1) xor F(R_(i-1), k_i)
        R_old = StringXOR(tmp_str, FeistelFunction(R_old, GetNewKey(i, C, D)));
    }

    // 3. last permutation
    str = L_old + R_old;
    std::string str_out;
    for (int i = 0; i < str.size(); ++i) {
        str_out += str[back_permutation[i] - 1]; // it should be less by 1
    }

    return str_out;
}

uint64_t StringToUINT64(std::string& str) { // works
    uint64_t block = 0;

    for (int i = 0; i < str.size(); ++i) {
        block = block << 1u;
        if (str[i] == '1') block += 1;
    }

    return block;
}

std::string InsertZero(const std::string& s) { // works
    std::stringstream ss;

    // inserts '0' after every 7th bite
    ss << s[0];
    for (int i = 1; i < s.size(); ++i) {

        if (i % 7 == 0) ss << '0';
        ss << s[i];
    }
    ss << '0';

    return ss.str();
}

std::string MakeKeyForDES(const std::string& str) { // works
    std::string binary_string = MakeBinaryString(str);

    // inserts zeros
    binary_string = InsertZero(binary_string);

    return binary_string;
}

const std::string HashOutputForm(std::string& a, std::string& b) { // works
    std::string out, tmp;

    for (int i = 0; i < a.size() / 4; ++i) {
        tmp = a.substr(i * 4, 4);
        int numb = (tmp[3] - 48) + (tmp[2] - 48) * 2 + (tmp[1] - 48) * 4 + (tmp[0] - 48) * 8;
        out += lazy_conversion_2[numb];
    }

    for (int i = 0; i < b.size() / 4; ++i) {
        tmp = b.substr(i * 4, 4);
        int numb = (tmp[3] - 48) + (tmp[2] - 48) * 2 + (tmp[1] - 48) * 4 + (tmp[0] - 48) * 8;
        out += lazy_conversion_2[numb];
    }

    return out;
}

const std::string LM_Hash(std::string& input_string) { // works
    std::string str, key1, key2;

    // 1. makes str size equal to 14
    if (input_string.size() > 14) {
        str = input_string.substr(0, 14);
    } else {
        str = input_string;
    }

    // 2. converts str to uppercase
    std::transform(str.begin(), str.end(), str.begin(), ::toupper);

    // 3. null-padding
    for (int i = str.size(); i < 14; ++i){
        str += (char) 0;
    }

    // 4. divides str into 2 parts and makes keys
    key1 = MakeKeyForDES(str.substr(0, 7));
    key2 = MakeKeyForDES(str.substr(7, 7));

    // 5. using keys to encrypt "KGS!@#$%"
    key1 = DES("KGS!@#$%", key1);
    key2 = DES("KGS!@#$%", key2);

    // returning their combination
    return HashOutputForm(key1, key2);
}

int main() {
    std::string input_string; // input
    std::cin >> input_string;

    std::cout << LM_Hash(input_string); // output
    return 0;
}
