# Blif2Graph

### Running the Program

1. **Prerequisites**:
   - Compiler: `g++` (GNU C++ Compiler)
   - Operating System: Linux

2. **Steps to Run**:
   - Ensure that the `.blif` netlist file is located in the **same directory** as the source code files.
   - The name of the netlist **must** be entered **without the `.blif` extension**.
   
3. **Compiling the Program**:
   - Open a terminal in the directory containing the source code.
   - Use the following command to compile the code:
     ```bash
     g++ main.cpp -o netlist_parser
     ```

4. **Running the Program**:
   - Once compiled, run the program using:
     ```bash
     ./netlist_parser
     ```
   - The program will prompt you to **enter the name of the netlist file** (without `.blif`).
   - Example: If your file is `example.blif`, just enter `example` when prompted. (a sample file named `alu4` has been included).

5. **Output**:
   - The program will create a graph from the given `.blif` file and output the number of modules, including I/O components.
