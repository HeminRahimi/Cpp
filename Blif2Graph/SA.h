#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <string_view>

using namespace std;

typedef vector <pair<int, vector<string>>*> TwoDim_Matrix_Pair;
typedef vector <vector <int>* > TwoDim_integer_Matrix;
typedef vector <vector <short>* > TwoDim_short_Matrix;
typedef vector <pair<int, pair<vector<string>, vector<string>>>*> Matrix_2D;
typedef vector <pair<pair<int, short>, pair<vector<string>, vector<string>>>*> Matrix_2D_Layer;

	static int Clk_includer;
	static Matrix_2D_Layer Graph, Main_Graph;
	static vector <vector <string>* > Models;
	static TwoDim_Matrix_Pair Original_Form_of_modules;
	static vector <string> Inputs;
	static vector <string> Outputs;
	static string title, clk_signal;
	class C_SA
	{
	public:
		///
		friend class Blif_Writer;
		friend int main(void);

		Matrix_2D undirectrd_Graph;
		int CLB_num;
		///  ----   ----   ----   ----   ----   ///
		void Blif2Graph(string netlist_name);
		void IO_Includer(void);
		int no_of_modules(void);
	};
