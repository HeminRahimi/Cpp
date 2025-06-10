# ANF Program

This program computes the Algebraic Normal Form (ANF) of a Boolean function based on a provided truth table.

## Prerequisites

- **C++17**: Ensure you have a C++ compiler that supports the C++17 standard (e.g., `g++`).
- **Input File**: Prepare an input file (`inp_file.txt`) containing the truth table of the Boolean function.

## Compilation

To compile the program, use the following command:

```bash
g++ -std=c++17 -o ANF ANF.cpp
```

This command compiles the `ANF.cpp` source file into an executable named `ANF`.

## Running the Program

To run the program, execute the following command:

```bash
./ANF <inp_file.txt>
```

### Input File Format

- The input file (`inp_file.txt`) should contain the truth table of the Boolean function.
- Ensure the file is correctly formatted as expected by the program.

## Example

1. Compile the program:
   ```bash
   g++ -std=c++17 -o ANF ANF.cpp
   ```

2. Run the program with an input file:
   ```bash
   ./ANF input.txt
   ```

## Notes

- Replace `inp_file.txt` with the actual path to your input file containing the truth table.
- Ensure the input file is in the correct format to avoid runtime errors.
