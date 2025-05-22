# Programming Languages Course Work

**Instructor:** Prof. Tamer

This repository contains solutions and writeups for programming assignments in the Spring 2025 Programming Languages course at Sungkyunkwan University.


## 📘 A1: Theory Assignment

- **Objective**: Practice formal grammar and parsing fundamentals.
- **Grade**: N/A 
- **Key Topics**:
  * Writing context-free grammars
  * Left-recursion elimination
  * Computing FIRST/FOLLOW sets
  * Derivation trees and ambiguity analysis


## 🔬 A2: Lexical Analyzer

- **Objective**: Build a custom lexer for a restricted C-like language.
- **Grade**: 92/100
- **Key Features**:
  * Tokenizes identifiers, keywords, literals, and operators
  * Handles whitespace and comments
  * Input: C-style code (`test.c`)
  * Output: Token stream with type and position

## 🧠 A3: Memory Allocation Benchmark

- **Objective**: Measure and compare the performance of static, stack, and heap memory allocation in C++.
- **Grade**: 100/100
- **Key Features**:
  * Uses `chrono` for timing
  * Tests array operations across different memory types
  * Includes a detailed PDF analysis and graph of results


## 🐍 A4: Python Practice

- **Objective**: Reinforce basic programming constructs using Python.
- **Grade**: 100/100
- **Key Features**:
  * `q1.py`: Generate & sort 200 random numbers, print them, and plot histogram
  * `q2.py`: Recursive vs. closed-form sum of numbers from 1 to n
  * `q3.py`: Interactive prime number finder using 6k±1 optimization


## 📈 A5: Auto-Trading Simulation

- **Objective**: Build a modular auto-trading simulation in Python.
- **Grade**: 100/100
- **Key Features**:
  * Simulates market data updates from `market.json`
  * Users with different strategies (random, momentum, moving average)
  * Trades executed automatically in each simulation round
  * Strategy logic isolated in `strategies.py` for easy extension
