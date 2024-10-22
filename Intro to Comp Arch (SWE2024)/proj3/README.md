# Computer Architecture Project 3
Spike RISC-V Simulator

## Grade
unknown

## Overview
Goal 1:
  - Use the RISC-V instruction simulator’s cache simulation functionality to find the best cache configuration for the given applications.\
  - You can do this without understanding the C++ code

The possible cache configuration is as follows:
```shell
  --dc=1:256:64
  --dc=2:128:64
  --dc=4:64:64
  --dc=8:32:64
  --dc=16:16:64
  --dc=32:8:64
  --dc=64:4:64
  --dc=128:2:64
  --dc=256:1:64
```

Goal2:
  - Study the cache simulation in the Spike RISC-V instruction simulator
  - Modify the simulator to implement the LRU replacement algorithm
  - Need to modify C++ code

## Build steps
  - Clone the Spike git repository to your local directory
```
git clone ~swe3005/2023f/proj3/riscv-isa-sim
```
  - Checkout the v1.1.0 version
```
cd riscv-isa-sim
git checkout v1.1.0
cd ..
```
  - Create a “build” directory
```
mkdir build
```
  - Run configuration in the build directory
```
cd build
../riscv-isa-sim/configure
```
  - Run compilation in the build directory
```make ```
