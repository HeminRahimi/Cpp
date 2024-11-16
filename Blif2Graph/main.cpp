/******************************************************************
 * File: main.cpp
 * Author: Hemin Rahimi
 * Description: 
 * This program reads a BLIF file, parses it, and creates a 
 * graph representation of the BLIF file. 
 * 
 * Project: Blif2Graph
 * Date Created: January, 2019
 *******************************************************************/

#include "SA.h"
C_SA obj1;
void logo(void);

int main(void)
{
	int flag = 0;
	int C1, C2, C;
	string input;
	logo();
	cout << "Plz Enter the netlist name: " << endl;
	getline(cin, input);
	input = input + ".blif";
	obj1.Blif2Graph(input);
	cout << "Graph Created ... " << endl;
	//obj1.IO_Includer();
	cout << "#of Modules : " << obj1.no_of_modules() << "(I/Os Excluded)" << endl;
	return 0;
}

/////////////////////////////////////////////////////////

void C_SA::Blif2Graph(string netlist_name)
{
	fstream input_file1(netlist_name, ios::in);
	fstream input_file2(netlist_name, ios::in);

	static std::string inp;
	string str;
	short flag = 0;
	short names_num = 0;
	int latch_num = 0;
	int subckt_num = 0;
	int counter = 0;
	size_t Size;
	int i = 0;
	int line_num;
	vector <string> Temp;
	vector <string> T1, T2;
	vector <string> subckt_in;
	vector <string> subckt_out;
	//----------- Keywords for Searching -------------//
	const char* target0 = ".inputs";
	const char* target1 = ".outputs";
	const char* target2 = ".names";
	const char* target3 = ".latch";
	const char* target4 = ".subckt";
	string new_line = "\\";                

	//-------------------------------------------
	/// .model extraction
	inp.clear();
	line_num = 0;
	int Counter = 0;
	int detected = 0;
	if (input_file2.is_open())
	{
		cout << "\n   Running..." << endl;
		while (getline(input_file2, inp))
		{
			++line_num;
			std::string::iterator itr2 = inp.begin();
			if (!inp.empty())
			{
				if (*itr2 == '.')
				{
					std::string::size_type m = inp.find(".model");
					if (m != std::string::npos)
					{
						++Counter;
						if (Counter == 1)
							title = inp;
						else
						{
							++detected;
							Models.push_back(new vector <string>);
							while (1)
							{
								Models.at(detected - 1)->push_back(inp);
								getline(input_file2, inp);
								++line_num;
								std::string::size_type m = inp.find(".end");
								if (m != std::string::npos)
								{
									Models.at(detected - 1)->push_back(inp);
									break;
								}
							}
						}
					}
				}
			}
		}
	}
	else
	{
		cout << "Can not open the netlist file" << endl;
		exit(1);
	}
	input_file2.close();
	///-----------------------------------------------
	if (input_file1.is_open())
	{
		line_num = 0;
		while (getline(input_file1, inp))
		{
			++line_num;
			std::string::iterator itr0 = inp.begin();
			if (!inp.empty())
			{
				/// -------- storing The LUT values of the each ".names" in it's row
				std::string::iterator itr1 = inp.begin();
				/// comment ignoring
				if (*itr1 == '#')
				{
					continue;
				}
				if (*itr1 == '0' || *itr1 == '1' || *itr1 == '-' || *itr1 == ' ')
				{
					Original_Form_of_modules.at(i - 1)->second.push_back(inp);
				}
				//	 -----------------------------------------------   //
				std::string::size_type n = inp.find(new_line);
				if (n != std::string::npos)
					flag = 1;
				else
					flag = 0;

				if (*itr0 == '.')    //// ------- "dot" finding for keywords extraction --------////
				{
					std::string::size_type pos = inp.find(".end");
					if (pos != std::string::npos)
						break;
					else
					{
						////-------- ".input" finding  and storing to a vector  -------- ////
						std::string::size_type position0 = inp.find(target0); //  target0 --> input
						if (position0 != std::string::npos)
						{
							istringstream iss(inp);        /// spliting string to seperated words
							do
							{
								string subs;
								iss >> subs;
								if ((subs != "") && (subs != ".inputs") && (subs != "\\"))
									Inputs.push_back(subs);
							} while (iss);
							if (flag == 1)            /// //
							{
							lable_0:
								getline(input_file1, inp);
								++line_num;
								while (flag == 1)
								{
									istringstream iss(inp);
									do
									{
										string subs;
										iss >> subs;
										if ((subs != "") && (subs != "\\"))
											Inputs.push_back(subs);
									} while (iss);
									flag = 0;
									std::string::size_type n = inp.find(new_line);
									if (n != std::string::npos)
									{
										flag = 1;
										goto lable_0;
									}
									else
										break;
								}
							}
						}
						////****************************************************************////

						//// ---------  ".output" finding  and storing to a vector --------////
						std::string::size_type position1 = inp.find(target1);  //  target1 --> output
						if (position1 != std::string::npos)
						{
							istringstream iss(inp);        /// spliting string to seperated words
							do
							{
								string subs;
								iss >> subs;
								if ((subs != "") && (subs != ".outputs") && (subs != "\\"))
									Outputs.push_back(subs);
							} while (iss);
							if (flag == 1)            /// //
							{
							lable_1:
								getline(input_file1, inp);
								++line_num;
								while (flag == 1)
								{
									istringstream iss(inp);
									do
									{
										string subs;
										iss >> subs;
										if ((subs != "") && (subs != "\\"))
											Outputs.push_back(subs);
									} while (iss);
									flag = 0;
									std::string::size_type n = inp.find(new_line);
									if (n != std::string::npos)
									{
										flag = 1;
										goto lable_1;
									}
									else
										break;
								}
							}
						}
						////****************************************************************////

						////----------- ".names" finding  and storing to a vector  ---------////
						////------  Structure--->>>  LineNumber   ...<- Inputs ->...  --------------////

						std::string::size_type position2 = inp.find(target2);  ///  target2 --> names
						if (position2 != std::string::npos)
						{
							undirectrd_Graph.push_back(new pair <int, pair<vector<string>, vector<string>>>);      
							Original_Form_of_modules.push_back(new pair <int, vector <string>>);   

							++i;
							++names_num;
							Original_Form_of_modules.at(i - 1)->first = i - 1;
							Original_Form_of_modules.at(i - 1)->second.push_back(inp);  /// The Original line storing
							undirectrd_Graph.at(i - 1)->first = i - 1;
							T1.clear();
							T1.shrink_to_fit();
							Temp.clear();
							Temp.shrink_to_fit();
							istringstream iss(inp);
							do
							{
								string subs;
								iss >> subs;
								if ((subs != "") && (subs != ".names") && (subs != "\\"))
								{
									T1.push_back(subs);
								}
							} while (iss);
							if (flag == 1)                /// //
							{

							lable_2:

								getline(input_file1, inp);
								++line_num;
								Original_Form_of_modules.at(i - 1)->second.push_back(inp);

								while (flag == 1)
								{
									istringstream iss(inp);
									do
									{
										string subs;
										iss >> subs;
										if ((subs != "") && (subs != "\\"))
											T1.push_back(subs);
									} while (iss);
									flag = 0;
									std::string::size_type n = inp.find(new_line);
									if (n != std::string::npos)
									{
										flag = 1;
										goto lable_2;
									}
									else
										break;
								}
							}
							if (!T1.empty())
							{
								Size = (T1.size() - 1);
								for (size_t x = 0; x < Size; ++x)
								{
									str = T1.at(x);
									undirectrd_Graph.at(i - 1)->second.first.push_back(str);
								}
								str = T1.at(Size);
								undirectrd_Graph.at(i - 1)->second.second.push_back(str);
							}
						}
						////****************************************************************////

						////-------------- ".latch" finding  and storing to a vector -----------////
						//// ----  LineNumber  Input  Output  Type  Control  intialValue ------////
						std::string::size_type position3 = inp.find(target3);  ///  target3 --> latch
						if (position3 != std::string::npos)
						{
							undirectrd_Graph.push_back(new pair <int, pair<vector<string>, vector<string>>>);
							Original_Form_of_modules.push_back(new pair <int, vector <string>>);
							//std::string latch_line_num = std::to_string(line_num);
							Clk_includer = 1;     // for finding Clk SingNAL name;
							++i;
							++latch_num;
							++counter;
							T1.clear();
							T1.shrink_to_fit();
							Temp.clear();
							Temp.shrink_to_fit();
							short cnt = 0;
							Original_Form_of_modules.at(i - 1)->first = i - 1;
							Original_Form_of_modules.at(i - 1)->second.push_back(inp);
							undirectrd_Graph.at(i - 1)->first = i - 1;
							istringstream iss(inp);
							do
							{
								string subs;
								iss >> subs;
								if ((subs != "") && (subs != ".latch") && (subs != "\\"))
								{
									++cnt;
									if (cnt == 1)    /// input storing
										undirectrd_Graph.at(i - 1)->second.first.push_back(subs);
									if (cnt == 2)   /// output stornig
										undirectrd_Graph.at(i - 1)->second.second.push_back(subs);
									if (cnt == 4)   /// clock signal storing
									{
										if (counter == 1)      // just once!
											clk_signal = subs;
									}
								}
							} while (iss);

							///-------------------------------------------------
						}
						////-------------------  subckt storing --------------------////
						////---------   ... <- Inputs -> ... <- Outputs -> ...
						std::string::size_type position4 = inp.find(target4);  ///  target4 --> subckt
						if (position4 != std::string::npos)
						{
							undirectrd_Graph.push_back(new pair <int, pair<vector<string>, vector<string>>>);
							Original_Form_of_modules.push_back(new pair <int, vector <string>>);

							subckt_in.clear(); subckt_in.shrink_to_fit();
							subckt_out.clear(); subckt_out.shrink_to_fit();
							T1.clear(); T1.shrink_to_fit();
							++i;
							++subckt_num;
							Original_Form_of_modules.at(i - 1)->first = i - 1;
							Original_Form_of_modules.at(i - 1)->second.push_back(inp);
							undirectrd_Graph.at(i - 1)->first = i - 1;
							string temprorry;
							if (Clk_includer == 1)
								temprorry = clk_signal;
							else
								temprorry = "unconn";
							//
							istringstream iss(inp);
							do
							{
								string word;
								string subs;
								iss >> word;
								size_t x = word.size();
								size_t a = word.find('=');
								if (a != std::string::npos)
								{
									string Tmp;
									Tmp = word.substr(0, a);
									size_t F = Tmp.find("out");
									if (F != std::string::npos)
									{
										string Out;
										Out = word.substr(a + 1, x);
										if ((subs != "unconn"))
										{
											subckt_out.push_back(Out);
										}
									}
									else
									{
										subs = word.substr(a + 1, x);
										if ((subs != "unconn") && subs != temprorry)
										{
											T1.push_back(subs);
										}
									}
								}
							} while (iss);
							if (flag == 1)                /// //
							{
							lable_4:

								getline(input_file1, inp);
								++line_num;
								Original_Form_of_modules.at(i - 1)->second.push_back(inp);
								while (flag == 1)
								{
									istringstream iss(inp);
									do
									{
										string word;
										string subs;
										iss >> word;
										size_t x = word.size();
										size_t a = word.find('=');
										if (a != std::string::npos)
										{
											string Tmp;
											Tmp = word.substr(0, a);
											size_t F = Tmp.find("out");
											if (F != std::string::npos)
											{
												string Out;
												Out = word.substr(a + 1, x);
												if ((subs != "unconn"))
												{
													subckt_out.push_back(Out);
												}
											}
											else
											{
												subs = word.substr(a + 1, x);
												if ((subs != "unconn") && subs != temprorry)
												{
													T1.push_back(subs);
												}
											}
										}
									} while (iss);
									flag = 0;
									std::string::size_type n = inp.find(new_line);
									if (n != std::string::npos)
									{
										flag = 1;
										goto lable_4;
									}
									else
										break;
								}
							}
							Size = subckt_out.size();
							for (size_t x = 0; x < Size; ++x)
							{
								str = subckt_out.at(x);
								undirectrd_Graph.at(i - 1)->second.second.push_back(str);
							}
							Size = T1.size();
							for (size_t x = 0; x < Size; ++x)
							{
								str = T1.at(x);
								undirectrd_Graph.at(i - 1)->second.first.push_back(str);
							}
						}
					}
				}
			}
		}
	}
	else
	{
		cout << "Can not open the netlist file" << endl;
		exit(1);
	}
	CLB_num = names_num;
	input_file1.close();
}
/// ********************************** ///
void C_SA::IO_Includer(void)
{
	string str;
	size_t SIZE = Original_Form_of_modules.size();
	for (size_t i = 0; i < Inputs.size(); ++i)
	{
		str = Inputs.at(i);
		int temp = str.compare(clk_signal);
		if (temp != 0)
		{
			undirectrd_Graph.push_back(new pair <int, pair<vector<string>, vector<string>>>);
			undirectrd_Graph.at(SIZE)->first = SIZE;
			undirectrd_Graph.at(SIZE)->second.second.push_back(str);
			++SIZE;
		}
	}
	
	for (size_t i = 0; i < Outputs.size(); ++i)
	{
		str = Outputs.at(i);
		undirectrd_Graph.push_back(new pair <int, pair<vector<string>, vector<string>>>);
		undirectrd_Graph.at(SIZE)->first = SIZE;
		undirectrd_Graph.at(SIZE)->second.first.push_back(str);
		++SIZE;
	}
	
}
/// ********************************** ///
int C_SA::no_of_modules(void)
{
	int total_modules;
	total_modules = undirectrd_Graph.size();
	return total_modules;
}
/// ********************************** ///
void logo(void)
{

	cout << "**             " << endl;
	cout << "**             " << endl;
	cout << "**          **                            " << endl;
	cout << "*********        *********   **********   *********   **     **" << endl;
	cout << "*********   **   *********           **   ***         **     **" << endl;
	cout << "**     **   **   **     **   **********   **          *********" << endl;
	cout << "**     **   **   **     **   **      **   **                 **" << endl;
	cout << "*********   **   **     **   **********   **          *********" << endl;
	cout << "================================================================" << endl;
}
