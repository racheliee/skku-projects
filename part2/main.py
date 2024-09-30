import os
import argparse
from part2 import *

# Top of C++ file with includes and namespaces
top_source_string = """
#include <iostream>
#include <assert.h>
#include <chrono>
using namespace std;
using namespace std::chrono;

typedef double reduce_type;

"""

# The reference loop simply adds together all elements in the array
def reference_reduction_source():

    # function header
    function = "void reference_reduction(reduce_type *b, int size) {"

    # loop header
    loop = "  for (int i = 1; i < size; i++) {"

    #simple reduction equation
    eq = "    b[0] += b[i];"

    # closing braces
    loop_close = "  }"
    function_close = "}"

    # joining together all parts
    return "\n".join([function, loop, eq, loop_close, function_close])

# String for the main function, including timings and
# reference checks.
main_source_string = """
#define SIZE (1024*1024*512)


int main() {
  reduce_type *a;
  a = (reduce_type *) malloc(SIZE * sizeof(reduce_type));

  reduce_type *b;
  b = (reduce_type *) malloc(SIZE * sizeof(reduce_type));

  for (int i = 0; i < SIZE; i++) {
    a[i] = 1;
    b[i] = 1;
  }

  auto new_start = high_resolution_clock::now();
  homework_reduction(a,SIZE);
  auto new_stop = high_resolution_clock::now();
  auto new_duration = duration_cast<nanoseconds>(new_stop - new_start);
  double new_seconds = new_duration.count()/1000000000.0;

  auto ref_start = high_resolution_clock::now();
  reference_reduction(b,SIZE);
  auto ref_stop = high_resolution_clock::now();
  auto ref_duration = duration_cast<nanoseconds>(ref_stop - ref_start);
  double ref_seconds = ref_duration.count()/1000000000.0;

  double speedup = ref_seconds / new_seconds;
  cout << "new loop time: " << new_seconds << endl;
  cout << "reference loop time: " << ref_seconds << endl;
  cout << "speedup: " << speedup << endl << endl;

  return 0;
}
"""

params_str = "#define UNROLL_FACTOR {}"

# Create the program source code
def pp_program(partitions):

    # Your function is called here
    homework_string = homework_reduction_source(partitions)

    # View your homework source string here for debugging
    return "\n".join([top_source_string, reference_reduction_source(), homework_string, params_str.format(partitions), main_source_string])

# Write a string to a file (helper function)
def write_str_to_file(st, fname):
    f = open(fname, 'w')
    f.write(st)
    f.close()

# Compile the program. Don't change the options here for the official
# assignment report. Feel free to change them for your own curiosity.
# Some notes:
#
# I am using a recent version of C++ to use the chrono library.
#
# I am disabling the compiler's loop unrolling so we can ensure the
# reference loop and the homework loops are not unrolled "behind our backs"
#
# I am using the highest optimization level (-O3) to illustrate that
# the compiler is not even brave enough to perform this optimization!
def compile_program():
    cmd = "g++ -std=c++17 -fno-unroll-loops -O0 -o homework homework.cpp"
    print("running: " + cmd)
    assert(os.system(cmd) == 0)

# Run the program
def run_program():
    cmd = "./homework"
    print("running: " + cmd)
    print("")
    assert(os.system(cmd) == 0)

# This is the top level program function. Generate the C++ program,
# compile it, and run it.
def generate_and_run(partitions):
    print("")
    print("----------")
    print("generating and running for:")
    print("partitions = " + str(partitions))
    print("-----")
    print("")

    # get the C++ source
    program_str = pp_program(partitions)

    # write it to a file (homework.cpp)
    write_str_to_file(program_str, "homework.cpp")

    # compile the program
    compile_program()

    # run the program
    run_program()

# gets one command line arg unroll factor (UF)
def main():
    parser = argparse.ArgumentParser(description='Part 2 of Homework 1: exploiting ILP by unrolling reduction loop iterations.')
    parser.add_argument('unroll_factor', metavar='UF', type=int,
                   help='how many loop iterations to unroll')
    args = parser.parse_args()
    UF = args.unroll_factor
    generate_and_run(UF)

if __name__ == "__main__":
    main()
