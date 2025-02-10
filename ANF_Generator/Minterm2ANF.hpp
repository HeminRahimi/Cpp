#ifndef ANF_CALC_HPP
#define ANF_CALC_HPP

#include <vector>
#include <iostream>
#include <cmath> 

class Minterm2ANF {

public:

    int num_of_vars;
    int total_size;
    int num_of_ones;

    std::vector<std::string> terms_str;

    Minterm2ANF(int num_of_vars);    

    std::vector<short> dec2bin(int decimal, int bit_size);
    std::vector<std::string> generate_anf_terms(void);
    std::vector<short> GenerateANF(const std::vector<short>& minterms);
    void PrintANF (const std::vector<short> & list) const;

};

#endif 


