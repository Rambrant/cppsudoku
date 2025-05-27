# C++ Sudoku Solver

A modern C++ Sudoku solver designed with compile-time optimizations, leveraging template metaprogramming and traits-based design. 
I created this solver as a private project to learn how to code in modern C++. This solver can be used for educational purposes.

>I have done this on MacOS Seqouia (15.0) running on Apple Silicon (arm64). Other target environments has not been tested

## Features

- **Compile-Time Optimizations**: Utilizes `constexpr` and template metaprogramming to perform computations at compile time.
- **Traits-Based Design**: Flexible traits system to define board size, value ranges, etc.
- **Doxygen Documentation**: Automatically generated documentation.
- **Modern C++ Practices**: Follows best practices and leverages C++17 features.

## Getting Started

### Prerequisites

- A C++17 (or later) compatible compiler (e.g., GCC 10+, Clang 10+, MSVC 2019+)
- CMake (version 3.15 or later)
- [Doxygen](https://www.doxygen.nl/) (optional, for building documentation)
- [vcpkg](https://github.com/microsoft/vcpkg) from Microsoft for installation of external packages

#### Install Doxygen
```bash
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
```bash
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
externals/      # External dependencies
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
