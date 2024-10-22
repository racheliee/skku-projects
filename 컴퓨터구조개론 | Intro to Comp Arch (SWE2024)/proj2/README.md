# Computer Architecture Project 2
Single-cycle RISC-V CPU Simulator

## Grade
full score

## Supported Instructions
- add, sub, addi 
- xor, or, and, xori, ori, andi
- slli, srli, srai, sll, srl, sra
- slti, slt
- auipc, lui
- jal, jalr
- beq, bne, blt, bge
- lw, sw

## Simulator Program Behaviour
Takes 2 or 3 command-line arguments.

### 2 Arguments:
  -  First argument: Input file name for binary instructions
  -  Second argument: Number of instructions to execute (N)

### 3 Arguments:
  -  First argument: Input file name for binary instructions
  -  Second argument: Input file name for binary data
     -  The contents of this file will be loaded to the data memory, from address 0x10000000
  - Third argument: Number of instructions to execute (N)

- If there is no more instruction to execute after executing N instructions, stop simulation

- All registers are initialized to zero (0x00000000) at the beginning.\
- x0 is fixed to zero.
