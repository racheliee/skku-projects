# GPU


## WebGPU
- language: wsgl (WebGPU Shading Language)
- wsgl is compiled (vulkan (linux), metal (mac), hlsl (windows))
  - no printing --> difficult to debug
  - not javascript!! -> js is interpreted (not possible on GPUs)

### variable types
- variable type is optional
- compilers can infer types
- sometimes you need to specify types (ex: double precision instead of float)
- types: `i32`, `u32`, `f32`, `vec2<f32>`, `array<type>`
- structures
- built-ins (global id)
- built-in functions
  - arrayLength, sqrt, pow, distance

```wsgl
var a = 1; // inferred as int
```


> [!NOTE]
> overloading: 
> - same function but different paramters <br>
> - compiler can infer types <br>
> - ex: `f(int i)` & `f(vec2 v)` both `f()` but different parameters<br>
>

> [!TIP]
> example interview question: <br>
> function overloading vs overriding <br>
> - overriding: redefine a function in a subclass; used with inheritance
>   - the function is actually in the superclass, but the subclass has a different implementation
> - overloading: same function name but different parameters


### for loops
```wsgl
for (var i : u32 = 0u; i < arrayLength(a); i = i + 1u) {
  // do something
}
```

## memory consistency
- sequential consistency --> you can only order threads; instructions within a thread cannot be reordered
  - program order is sequential
- plain atomic accesses are documented to be sequentially consistent
  - SC doesn't work compossibly
  - two objects that are SC might not be SC when combined (no compossibility)
  - BUT programs contain only 1 shared memory; no reason to compose diff main mems
    - c++ provides atomic sequential consistency
- ISA
  - not SC

## total store order (TSO) memory model
- has a store buffer where stores are held before being written to memory
- threads check store bugger before going to main mem
  - cheap and close to check
- x86 (most conservative)

### relaxed memory consistency
- store buffer can reorder stores execution that is not allowed by SC
- weak memory behaviour
- x86, c++ (but if you use atomic, it's SC)
- sometimes, reordering is useful for performance and doesn't affect the program
  - provides special instructions that disallow weak memory behaviour (aka. fences)
  - ex: `mfence` instruction
- they try to delay to store as much as possible (flush together)
  - store buffer is kept as a queue (FIFO)
  - order is preserved
- loads are done earlier so the data is prepared before it is even needed


### litmus test
- collection of tests to check memory consistency
- small concurrnet programs that check for relaxed memory consistency

### TSO rules
- all behaviours that can be observed in x86
1. stores followed by a load do not have to follow program order
2. stores cannot be reordered past a fence in program order
3. stores cannot be reordered past laods from the same address

```plain
      mem access 0
       L        S
   +-------+-------+
L  |  NO   |  YES  |
   +-------+-------+
S  |  NO   |  NO   |
   +-------+-------+
```
- if memory access 0 appreas before mem access 1 in program order, can it bypass program order?