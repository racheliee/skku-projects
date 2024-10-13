---
title: CSE113 Homework 1 Design Document
date: \today
geometry: margin=1in
papersize: letter
fontfamily: newtxtext, newtxmath
fontsize: 10pt
numbersections: true
autoEqnLabels: true
header-includes: |
  \usepackage{authblk}
  \author{Rachel (Younwoo) Park}
  \affil{Dept. of Systems Management Engineering\\Sungkyunkwan University}
  \usepackage{fancyhdr}
  \pagestyle{fancy}
  \fancyhead[L]{Markdown to PDF report}
  \fancyhead[C]{}
  \fancyhead[R]{\thepage}
  \fancyfoot{}
  \usepackage{float}
  \let\origfigure\figure
  \let\endorigfigure\endfigure
  \renewenvironment{figure}[1][2] {\expandafter\origfigure\expandafter[H]} {
    \endorigfigure
  }
#   \usepackage[hangul, nonfrench, finemath]{kotex}
abstract: |

  This is the test abstract of ducment. This is the test abstract of ducment. 
  This is the test abstract of ducment. This is the test abstract of ducment. 
  This is the test abstract of ducment. This is the test abstract of ducment. 
  This is the test abstract of ducment. This is the test abstract of ducment. 
  This is the test abstract of ducment. This is the test abstract of ducment.
---

\tableofcontents

\newpage



# 1. Loop Unrolling Independent Iterations for ILP

## 1.1 Introduction
The loop structure provided in the assignment is chracterized by a dependency chain of length 64. Since the dependency chain cannot be re-ordered or pre-computed due to the non-associative nature of floating point addition, the loop iterations are dependent on each other. 

While the instructions themselves are not independent, each chain of computation is independent of the other chains. Hence, we can unroll the loop to increase the number of independent instructions that can be executed in parallel.

## 1.2 Approach
Let `SEQ(i)` denote the `i-th` iteration of the sequential loop.

In other words, the sequential loop can be represented as follows:
```c
void loop(float * a, int size) {
    for (int i = 0; i < size; i++) {
        SEQ(i);
    }
}
```

### 1.2.1 Sequential Loop Unrolling
While the instructions within the sequence are dependent of each other, each sequence themselves does not depend on each other. Therefore, we can execute several sequences in parallel in one iteration of the loop.

#### Pseudocode
For instance, if the loop were to be unrolled by a factor of $k$, the loop would look as follows:
```c
void loop(float * a, int size) {
    for (int i = 0; i < size; i += k) {
        SEQ(i);
        SEQ(i+1);
        ...
        SEQ(i+k-1);
    }
}
```

### 1.2.2 Interleaved Loop Unrolling
To take advantage of the ILP even further, the instructions of each sequence can be interleaved with one another. This way, the dependent instructions will be spaced further apart, allowing for more instructions to be executed in parallel.

#### Pseudocode
For instance, if the loop were to be unrolled by a factor of $k$, the loop would look as follows:
```c
void loop(float * a, int size) {
    for (int i = 0; i < size; i += k) {
        float tmp0 = b[i];
        float tmp1 = b[i+1];
        float tmp2 = b[i+2];
        // ...
        float tmpk = b[i+k-1];

        tmp0 += 1.0f; tmp1 += 1.0f; tmp2 += 1.0f; ... tmpk += 1.0f;
        tmp0 += 2.0f; tmp1 += 2.0f; tmp2 += 2.0f; ... tmpk += 2.0f;
        // ...
        tmp0 += N; tmp1 += N; tmp2 += N; ... tmpk += N; // N is the length of the chain
    }
}
```



