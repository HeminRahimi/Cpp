
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>
#include <bitset>
#include <filesystem>

//////////////

using namespace std;
namespace fs = std::filesystem;

/////

const int Max_num_of_vars = 26;

string output_gen(const string &inputFile)
{
    string baseName = fs::path(inputFile).filename().string();

    size_t dotPos = baseName.find_last_of('.');
    string nameWithoutExt;
    if (dotPos != string::npos)
    {
        nameWithoutExt = baseName.substr(0, dotPos);
    }
    else
    {
        nameWithoutExt = baseName;
    }

    /// --- Create output dir
    fs::path outputDir = "output";
    if (!fs::exists(outputDir))
    {
        fs::create_directory(outputDir);
    }

    return (outputDir / (nameWithoutExt + "_out.txt")).string();
}

/////  --------------- //////

vector<bool> read_Truth_Table(const string &filename, int &n)
{
    ifstream file(filename);
    if (!file.is_open())
    {
        throw runtime_error("Cannot open file: " + filename);
    }

    vector<bool> truthTable;
    string line;
    int lineNumber = 0;

    while (getline(file, line))
    {
        ++lineNumber;
        line.erase(remove_if(line.begin(), line.end(), ::isspace), line.end());
        if (line.empty())
            continue;

        if (line == "0")
        {
            truthTable.push_back(false);
        }
        else if (line == "1")
        {
            truthTable.push_back(true);
        }
        else
        {
            cerr << "Warning: Ignoring invalid line " << lineNumber << ": '" << line << "'" << endl;
        }
    }

    file.close();

    if (truthTable.empty())
    {
        throw runtime_error("Truth table is empty");
    }

    n = log2(truthTable.size());
    if (pow(2, n) != truthTable.size())
    {
        throw runtime_error("Truth table size (" + to_string(truthTable.size()) +
                            ") is not a power of 2");
    }

    if (n > Max_num_of_vars)
    {
        throw runtime_error("Number of variables (" + to_string(n) +
                            ") exceeds maximum supported (" + to_string(Max_num_of_vars) + ")");
    }

    cout << "Read truth table (" << truthTable.size() << " entries, " << n << " variables):" << endl;
    for (size_t i = 0; i < truthTable.size(); ++i)
    {
        cout << "f(" << bitset<32>(i).to_string().substr(32 - n) << ") = " << truthTable[i] << endl;
    }

    return truthTable;
}

/////  --------------- //////

string intToTerm(size_t num, int n)
{
    string result;
    const char vars[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
                         'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};
    for (int i = 0; i < n; ++i)
    {
        if (num & (size_t(1) << (n - 1 - i)))
        {
            result += vars[i];
        }
    }
    return result.empty() ? "1" : result;
}

/////  --------------- //////

vector<bool> Gen_ANF(const vector<bool> &truthTable, int n)
{
    vector<bool> anf = truthTable;
    size_t size = size_t(1) << n;

    for (int i = 0; i < n; ++i)
    {
        for (size_t j = 0; j < size; ++j)
        {
            if (j & (size_t(1) << i))
            {
                anf[j] = anf[j] ^ anf[j ^ (size_t(1) << i)];
            }
        }
    }

    return anf;
}

/////  --------------- //////

void printANF(const vector<bool> &anf, int n, const string &inputFile)
{

    string outputFile = output_gen(inputFile);

    ofstream outFile(outputFile);
    if (!outFile.is_open())
    {
        throw runtime_error("Cannot open output file: " + outputFile);
    }

    string anfString;
    bool first = true;
    for (size_t i = 0; i < anf.size(); ++i)
    {
        if (anf[i])
        {
            if (!first)
                anfString += " + ";
            anfString += intToTerm(i, n);
            first = false;
        }
    }
    if (first)
        anfString = "0";

    cout << "ANF: " << anfString << endl;

    outFile << anfString << endl;
    outFile.close();
}

/////  --------------- //////

bool validation_phase(const vector<bool> &anf, const vector<bool> &truthTable, int n)
{
    if (n > 10)
    {
        cout << "Skipping validation for n=" << n << " (too large)" << endl;
        return true;
    }

    vector<bool> recomputed(size_t(1) << n, false);
    for (size_t i = 0; i < anf.size(); ++i)
    {
        if (!anf[i])
            continue;
        for (size_t j = 0; j < recomputed.size(); ++j)
        {
            if ((i & j) == i)
            {
                recomputed[j] = recomputed[j] ^ true;
            }
        }
    }

    bool valid = true;
    for (size_t i = 0; i < truthTable.size(); ++i)
    {
        if (recomputed[i] != truthTable[i])
        {
            valid = false;
            cerr << "Validation failed at index " << i << ": expected " << truthTable[i]
                 << ", got " << recomputed[i] << endl;
        }
    }
    return valid;
}

///////////////////////////////////////

int main(int argc, char *argv[])
{

    if (argc != 2)
    {
        cerr << "Usage: " << argv[0] << " <truth_table_file.txt>" << endl;
        return 1;
    }

    try
    {
        int n;

        vector<bool> truthTable = read_Truth_Table(argv[1], n);

        vector<bool> anf = Gen_ANF(truthTable, n);

        printANF(anf, n, argv[1]);

        if (validation_phase(anf, truthTable, n))
        {
            cout << "ANF validation: Passed" << endl;
        }
        else
        {
            cout << "ANF validation: Failed" << endl;
        }
    }

    catch (const exception &e)
    {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }

    return 0;
}
