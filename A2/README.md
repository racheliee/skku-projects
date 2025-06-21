# C-AST Generation with Python

This project implements a lightweight interpreter for a subset of the C programming language. It lexes, parses, and evaluates C-style code that includes function definitions, arithmetic and bitwise expressions, variable declarations, and basic control structures like `return`.

Grade: 92/100


## Features

- Supports C-style function declarations and definitions
- Handles integer, float, and double types
- Correctly evaluates arithmetic and bitwise operations
- Evaluates user-defined and built-in function calls (including `printf`)
- Mimics C-style behavior for division and modulus (i.e., rounding toward zero)
- Parses and executes function prototypes
- Builds an Abstract Syntax Tree (AST) for debugging and visualization

## Limitations

- No support for loops (`while`, `for`) or conditionals (`if`, `else`)
- No global variables
- No pointers or memory manipulation
- No nested function declarations
- No I/O except for `printf`

## How It Works

### 1. Lexer

The `Lexer` class tokenizes the source code into meaningful chunks:
- Keywords (`int`, `float`, `return`, etc.)
- Identifiers (e.g., variable names)
- Symbols (`+`, `*`, `(`, `)`, etc.)
- Literals (numbers, strings)

### 2. Parser

The `Parser` builds an Abstract Syntax Tree (AST) from the tokens. It identifies:
- Function definitions and prototypes
- Declarations
- Binary and unary operations
- Function calls
- Return statements

### 3. Evaluator

The `Evaluator` visits AST nodes and computes the result:
- Supports arithmetic operations with C-style overflow handling
- Tracks variables and scopes correctly
- Evaluates `printf` and user-defined functions
- Mimics C’s division and modulo behavior (rounding toward zero)

## How to Run

1. Make sure you're using **Python 3.10+** (for `match-case` syntax).
2. Save your C-style code in a file (e.g., `example.c`).
3. Run the interpreter from the command line:

```bash
python a2.py example.c
```

## Miscellaneous
- `./testing/test` holds the test cases that were used for grading the assignment. 6 out of the 10 test cases are provided.
- the rest of the files under `./testing` folder hold test cases that were used to test the code myself and the script to generate the answers for the test cases.