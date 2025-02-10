#include "Minterm2ANF.cpp"

int main() 
{
    int variable_num;
    std::vector <short> minterms;
    std::vector <short> output_list;
    short minterm;

    std::cout << "Please enter the number variables: " << std::endl;
    std::cin >> variable_num;
    Minterm2ANF calc(variable_num);
    calc.num_of_vars = variable_num;
    calc.total_size = pow(2, variable_num);

    minterms.resize(calc.total_size, 0);
    
    std::cout << "Enter minterms (order is matter and enter -1 to stop): " << std::endl;
    while (std::cin >> minterm && minterm != -1) 
    {
        minterms.at(minterm) = 1;
    }
    output_list = calc.GenerateANF(minterms) ; 
    calc.PrintANF (output_list);

    return 0;
}



