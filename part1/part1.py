# First homework function here! Implement the reference loop unrolled
# *sequentially*, That is, create dependency chains of length
# *chain_length*. Unroll the loop by a factor of *unroll_factor*. Do
# the unrolled loop iterations sequentially: i.e. do not start the
# chain of one iteration before the previous one is finished.

# to view the reference loop for a dependency chain of N, just
# run: python3 main.py N 1.
# Your code will initially fail the assertion check, but you
# should be able to view the reference loop in homework.cpp.

# You can assume that the unroll factor evenly divides the
# array length. That is, you should be able to do this all
# in one loop without any extra clean-up loops.

# Don't forget! Floating point constants must have 'f' after them!
# that is, you would write 2 in floating point as '2.0f'
#
# You can gain confidence that implemented this correctly by executing
# skeleton.py with several power-of-two options for the unroll factor
# for example, try 1,2,4,8, etc.
def homework_loop_sequential_source(chain_length, unroll_factor):
    function = "void homework_loop_sequential(float *b, int size) {"
    #implement me!
    function_body = ""
    function_close = "}"
    return "\n".join([function, function_body, function_close])


# Second homework function here! The specification for this
# function is the same as the first homework function, except
# this time you will interleave the instructions of the unrolled
# dependency chains.

# You can assume the unroll factor is a power of 2 and that the
# the dependency chain also a power of two.
def homework_loop_interleaved_source(chain_length, unroll_factor):
    function = "void homework_loop_interleaved(float *b, int size) {"
    #implement me!
    function_body = ""
    function_close = "}"
    return "\n".join([function, function_body, function_close])

