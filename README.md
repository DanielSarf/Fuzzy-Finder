# Fast Fuzzy Searching using BK Trees!

## Description

**Easily search your `.txt` files with this Console program that finds near matches to your search term. This tool helps you find words and phrases even if they contain typos, making it perfect for quickly locating information in your files.
The distance metric used to find near matches is levenshtein distance.
This project was assigned to me by my DSA teacher in the 4th Semester. The goal was to use an efficient method (Data structure and/or algorithm) to find near matches to the search string from a specified text file.**

## Features

- Fast searching within documents after tree is built
- Results are sorted by how closely they match your search term
- Get exact location of where the chosen result occurs within `.txt` document
- Error handling
- Supports Windows and Linux

## Compilation

To compile on Linux, use the following command:

```sh
g++ -g BKMap.cpp BKTree.cpp BKTreeNode.cpp Source.cpp -o ./FuzzySearch.o
```

This command compiles multiple source files into an executable named `FuzzySearch.o`.

## Running the Project on Windows

To run this project on Windows using Visual Studio, follow these steps:

1. **Create a New Project in Visual Studio**:

2. **Add Source Files**:
   - Right-click on the "Source Files" folder in Solution Explorer.
   - Select "Add" > "Existing Item..." and add all the `.cpp` files.

3. **Add Header Files**:
   - Right-click on the "Header Files" folder in Solution Explorer.
   - Select "Add" > "Existing Item..." and add all the `.h` files.

4. **Build and Run**:
   - Build the project by clicking "Build" > "Build Solution".
   - Run the project in Release mode.

## How to use


## Screenshots


## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for more details.
