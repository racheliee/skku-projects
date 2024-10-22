# Create a loop that is semantically equivalent to the reference loop.
# That is, it computes  a[0] = a[0] + a[1] + a[2] + a[3] + a[4] ... + a[size]
# where size is passed in by the main source string.
#
# You should unroll by a factor of "partitions". This is done logically
# by splitting a into N partitions (where N == partitions). You will compute
# N reductions, one computation for each partition per loop iteration.
#
# You will need a cleanup loop after the main loop to combine
# the values in each partition.
#
# You can assume the size and the number of partitions will be
# a power of 2, which should make the partitioning logic simpler.
#
# You can gain confidence in your solution by running the code with
# several power-of-2 unroll factors, e.g. 2,4,8,16. You can assume
# partition is less than size.
def homework_reduction_source(partitions):
    # header
    function = "void homework_reduction(reduce_type *a, int size) {"

    # implement me!
    function_body = []

    # declare variables: size and N accumulators (N = number of partitions)
    function_body.append(f'  int s = size / {partitions};')
    for i in range(partitions):
        function_body.append(f'  reduce_type acc{i} = a[{i}*s];')
        

    # reduction loop
    function_body.append(f'  for (int i = 1; i < s; ++i) {{')
    for i in range(partitions):
        function_body.append(f'    acc{i} += a[i + ({i} * s)];') # accumulate to each of the accumulators
    function_body.append('  }')

    # accumulate the N accumulators
    for i in range(1, partitions):
        function_body.append(f'  acc0 += acc{i};') 
    function_body.append(f'  a[0] = acc0;') # store the result in a[0]

    # closing brace
    function_close = "}"

    function_body = "\n".join(function_body)
    return "\n".join([function, function_body, function_close])
