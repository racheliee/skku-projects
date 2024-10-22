# Computer Architecture Project 1
RISC-V Binary Code

## Grade
full score

## Goal
  - Reads a binary file containing RISC-V machine code and prints the assembly representation of the code

## Input
- The input is a binary file containing RISC-V machine codes.\
- The input file name is provided as the first command-line argument.\
- The file name length will not exceed 255 characters.

## Output
- Print the disassembled instructions.\
- Each line should display one instruction in the following format:

```
inst <instruction number>: <32-bit binary code in hex format> <disassembled instruction>
```

## Instructions to support
```
lui, auipc, jal, jalr, beq, bne, blt, bge, bltu, bgeu,
lb, lh, lw, lbu, lhu, sb, sh, sw, addi, slti, sltiu,
xori, ori, andi, slli, srli, srai, add, sub, sll, slt,
sltu, xor, srl, sra, or, and
```

If an instruction cannot be interpreted, print “unknown instruction”


