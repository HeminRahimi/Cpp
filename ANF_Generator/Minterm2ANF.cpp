#include "Minterm2ANF.hpp"

Minterm2ANF::Minterm2ANF(int num_of_vars) : num_of_vars(num_of_vars), 
    total_size(pow(2, num_of_vars)), num_of_ones(0) 
{
    terms_str = generate_anf_terms();
}

////////////

std::vector<short> Minterm2ANF::dec2bin(int decimal, int bit_size) {
    std::vector<short> binary(bit_size, 0);
    int index = 0;

    while (decimal > 0 && index < bit_size) {
        binary[index] = decimal % 2;
        decimal /= 2;
        ++index;
    }

    return binary;
}

/////////

std::vector<std::string> Minterm2ANF::generate_anf_terms(void) 
{
    for (int i = 0; i < total_size-1; i++) 
    {
        std::string term;
        for (int j = 0; j < num_of_vars; j++) 
        {
            if (i & (1 << j)) 
            { 
                term += (-j + 96 + num_of_vars); 
            }
        }
        if (term.empty()) {
            term = "1"; 
        }
        terms_str.push_back(term);
        
    }
  
    return terms_str;
}

/////

std::vector<short> Minterm2ANF::GenerateANF(const std::vector<short>& minterms) 
{
    std::vector<short> list;
    list.reserve(minterms.size()); 

    std::vector<short> ones_index;
    int table_size = minterms.size();
    for (auto i = 0 ; i < table_size ; ++i)
    {
        if (minterms.at(i) == 1)
        {
            ones_index.push_back(i);
        }
    }
    if (minterms.at(0) == 1)
        list.push_back(1);
    else
        list.push_back(0);

    int num_of_ones = ones_index.size();
    
    int bit_size = num_of_vars;
    std::vector<short> cap_val, cmp_data;
    
    int cnt;
    for (auto i = 1 ; i < table_size - 1 ; ++i)
    {
        int cnt = num_of_ones;
        std::vector<short> cmp_bits;

        cap_val = dec2bin(i, bit_size);
        
        for (auto j = 0 ; j < cap_val.size() ; ++j)
        {
            if (cap_val[j] == 0)
                cmp_bits.push_back(j);
        }

        for (auto j = 0 ; j < num_of_ones ; ++j)
        {
            cmp_data = dec2bin(ones_index[j], bit_size);

            for (auto n = 0 ; n < cmp_bits.size(); ++n)
            {
                if (cmp_data[cmp_bits[n]] == 1)
                {
                    --cnt;
                    break;
                }
            }
        }
        list.push_back(cnt);
    }
    return list;
}

/////

void Minterm2ANF::PrintANF(const std::vector<short>& list) const 
{

    if (list.empty()) {
        std::cout << "No terms to display." << std::endl;
        return;
    }

    std::cout << "*************************" << std::endl;
    std::cout << "Algebraic Normal Form:" << std::endl;
    std::cout << "---------------------" << std::endl;

    bool firstTerm = true;
    for (size_t i = 0; i < list.size(); ++i) {
        if (list[i] % 2 == 1) {
            if (!firstTerm) {
                std::cout << " + ";
            }
            std::cout << "\033[31m" << terms_str.at(i) << "\033[0m"; 
            firstTerm = false;
        }
    }

    std::cout << std::endl;
    std::cout << "----------------------------------" << std::endl;
    std::cout << "Note: variable < a > is MSB" << std::endl;
    std::cout << "**********************************" << std::endl;
}
