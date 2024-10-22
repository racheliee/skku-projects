# System Programming Assignment 03
Program Profiling & Optimization

## Grade
full score

## Overview
This project involves profiling and optimizing a target program using the `gprof` tool in a Linux environment. The process includes multiple stages of optimization based on profiling results.

There are two files in this project:
- `bigram_og.c`: The original target program to be optimized.
- `bigram_opt.c`: The optimized version of the target program.
side note: Wrapper functions were used to check the time each function takes to execute.


## Objective
- Perform multi-step program optimization and analysis through profiling.
- Use a self-developed target program, ideally the "Bigram Analyzer" studied in SP05-3 or any program with at least 15 functions, over 300 lines of code, and an initial runtime of over 3 minutes.

## Project Steps
1. **Implement and Test Target Program:** Develop and verify the functionality of your target program.
2. **Initial Profiling:** Use `gprof` to perform the first profiling and analyze the results.
3. **Select Functions for Optimization:** Identify functions to optimize based on profiling analysis.
4. **Perform Code Optimization:** Optimize the selected functions.
5. **Repeat Profiling and Optimization:** Repeat steps 2-4 until sufficient optimization is achieved.
6. **Conclusion:** Finalize the optimization process and document the results.

## Evaluation Criteria
- Focus on the systematic analysis and optimization process rather than the extent of performance improvement.
- Comprehensive documentation and analysis in the report are crucial.
- Any form of cheating will result in a score of zero and an "F" grade.
