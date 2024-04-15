# CodeMetrics Analyser

The provided code represents a C program designed to analyze source code files and calculate various metrics such as lines of code, number of functions, and cyclomatic complexity. It utilizes data structures like linked lists to organize the parsed information about files and functions. The main functionality includes parsing source code files, calculating cyclomatic complexity for each function, and generating metrics reports.Brief description of the project.

## Features

- Source Code Analysis: Parses C source code files to extract information about functions.

- Metrics Calculation: Calculates metrics such as lines of code, number of functions, and cyclomatic complexity.

- Dynamic Memory Allocation: Uses dynamic memory allocation to create and manage data structures for storing file and function information.

- Modular Design: The code is structured into modular functions for readability and maintainability.

- Output Generation: Generates a metrics report file containing information about the analyzed source code.

- Scalability: Capable of analyzing large codebases with multiple files and functions.

- Customization: Easily extendable to include additional metrics or analysis functionalities.

## Code Explanation

- `FunctionNode`: Represents a function in the source code with its name and cyclomatic complexity.
- `FileNode`: Represents a file containing functions with its name and a pointer to the list of functions.
- `CodeMetrics`: Stores metrics including lines of code, number of functions, and cyclomatic complexity.


