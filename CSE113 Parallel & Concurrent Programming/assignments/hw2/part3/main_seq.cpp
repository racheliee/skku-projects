#include <stdlib.h> 
#include <fstream>

using namespace std;

#if defined(COARSE_LOCK)
#include "coarse_lock_stack.h"

#elif defined(RW_LOCK)
#include "rw_lock_stack.h"

#elif defined(SWAPTOP)
#include "swaptop_stack.h"

#else
#error No stack specified!
#endif
CSE113_Stack S;

int main() {
    bool failed = false;
    if (S.pop() != -1) {
        failed = true;
    }
    S.push(1);
    S.push(2);
    if (S.pop() != 2) {
        failed = true;
    }
    if (S.peek() != 1) {
        failed = true;
    }
#if defined(SWAPTOP)
    S.swaptop(2);
    if (S.peek() != 2) {
        failed = true;
    }
    S.swaptop(1);
#endif
    if (S.pop() != 1) {
        failed = true;
    }
    std::ofstream outfile;
    outfile.open("../output", std::ios_base::app);
    outfile << !failed << ",";
}
