# Overview
This repository contains project assignments for the Computer Architecture course focused on RISC-V architecture. The assignments provide practical experience in different aspects of computer architecture.

# Projects Included
## Computer Architecture Proj1: RISC-V Binary Code
- Goal: Disassemble a binary file containing RISC-V machine code into assembly instructions.
- Input: Binary file (specified as command-line argument).
- Output: Disassembled instructions in a specified format.
- Computer Architecture PA2: Single-cycle RISC-V CPU Simulator

## Computer Architecture Proj2: Single-cycle RISC-V CPU Simulator
- Supported Instructions: Includes `add`, `sub`, `addi`, `lw`, `sw`, `beq`, `jal`, and more.
- Behavior:
  - Takes 2 or 3 command-line arguments for instruction and data files, and number of instructions to execute.
  - Initializes all registers to zero.

## Computer Architecture Proj3: Spike RISC-V Simulator
- Goals:
  - Find the best cache configuration using the simulator's cache simulation.
  - Implement the LRU replacement algorithm in the simulator.
- Build Steps:
  - Clone the repository and checkout version v1.1.0.
  - Modify the cachesim.cc file to implement the LRU replacement algorithm.
  - Configure and compile the project.
