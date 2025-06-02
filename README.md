# C++ Sudoku Solver

A modern C++ Sudoku solver designed with compile-time optimizations, leveraging template metaprogramming and traits-based design. 
I created this solver as a private project to learn how to code in modern C++. This solver can be used for educational purposes.

The code supports 9x9 Sudoku boards but can be built for 16x16 by changing the board size in the SudokuTraits header.
Two different strategies are implemented for solving the Sudoku:
- Backtracking, a simple and fast brute force algorithm, but it might fail to solve more complicated boards
- Constraint Propagation, a more sophisticated algorithm that uses constraint propagation to solve the Sudoku, modeled after Peter Norvig's original idea.

The solver accepts a Sudoku board in virtually any format. It basically just reads the first 81 characters that are identified as a cell value,
(0-9 and A-F for 16x16 boards). A dot '.' or a zero '0' represents an empty cell.

### Examples of valid input formats

```
8 . .   . . .   . . .       0 0 3 | 0 2 0 | 6 0 0         400000805 
. . 3   6 . .   . . .       9 0 0 | 3 0 5 | 0 0 1         030000000 
. 7 .   . 9 .   2 . .       0 0 1 | 8 0 6 | 4 0 0         000700000 
                            ------+-------+------         020000060                  
. 5 .   . . 7   . . .       0 0 8 | 1 0 2 | 9 0 0         000080400 
. . .   . 4 5   7 . .       7 0 0 | 0 0 0 | 0 0 8         000010000 
. . .   1 . .   . 3 .       0 0 6 | 7 0 8 | 2 0 0         000603070 
                            ------+-------+------         500200000                    
. . 1   . . .   . 6 8       0 0 2 | 6 0 9 | 5 0 0         104000000 
. . 8   5 . .   . 1 .       8 0 0 | 2 0 3 | 0 0 9          
. 9 .   . . .   4 . .       0 0 5 | 0 1 0 | 3 0 0          
```

### Command line options (man page)

```
Usage: sudoku_solver [OPTIONS]
    Options:
      -h, --help                 Show this help message and exit
      -i, --input <file>         Read puzzle from file. (Default: stdin)
      -I, --input-format <fmt>   Output format: text or json. (Default: text)
      -o, --output <file>        Write solution to file. (Default: stdout
      -O, --output-format <fmt>  Output format: pretty, block, line or json. (Default: block)
      -s, --solvers <solver,...> Solvers to use: backtracking or constraint. (Default: backtracking and constraint)
      -v, --verbose              Verbose output
      -q, --quiet                Quiet output
```

## Features

- **Compile-Time Optimizations**: Utilizes `constexpr` and template metaprogramming to perform computations at compile time.
- **Traits-Based Design**: Flexible traits system to define board size, value ranges, etc.
- **Doxygen Documentation**: Automatically generated documentation.
- **Modern C++ Practices**: Follows best practices and leverages C++17 features.

## Getting Started

The code is delivered as is and must be compiled for the target platform. No binary releases are available.
>The code has been developed using JetBrain CLion IDE on macOS Sequoia (15.0) running on Apple Silicon (arm64). Other target environments have not been tested

### Prerequisites

- A C++17 (or later) compatible compiler (e.g., GCC 10+, Clang 10+, MSVC 2019+)
- CMake (version 3.15 or later)
- [Doxygen](https://www.doxygen.nl/) (optional, for building documentation)
- [vcpkg](https://github.com/microsoft/vcpkg) from Microsoft for installation of external packages

#### Install Doxygen
```
brew install doxygen
```

#### Install vcpkg

Run this in the ~/Developer directory
```
cd ~
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg
./bootstrap-vcpkg.sh
```

Install needed dependencies
```
brew install pkg-config
```

Add this line to your ~/.zshrc file
```
export PATH="$HOME/vcpkg:$PATH"
```

#### Configure CLion
Set the following CMake option in Settings—Build,Execution,Deployment—CMake
 ```
 -DCMAKE_TOOLCHAIN_FILE=<path to vcpkg>/vcpkg/scripts/buildsystems/vcpkg.cmake
 ```

### Build Instructions

Clone the repository and from the root of the project you run the following commands
```bash
mkdir build && cd build
cmake ..
cmake --build .
```

To build the documentation, you run
```bash
cmake --build . --target doc
```
Documentation will be generated in the `docs/` directory.

### Run the Solver
```bash
./bin/sudoku_solver
```

## Project Layout

```
includes/       # Header files
src/            # Source files
docs/           # Doxygen output
CMakeLists.txt  # Build configuration
Doxyfile        # Doxygen configuration
```

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Acknowledgements

Thanks to:
- Peter Norvig for his original Python Sudoku solver inspiration. See [Solving Every Sudoku Puzzle](http://norvig.com/sudoku.html)
- The C++ community for tooling and best practices.

## 📚 Full Documentation

Full reference docs are available [here](docs/html/index.html).  
