import random

# create list of 200 random numbers
random_number_list = [random.randint(1, 100) for _ in range(200)]

random_number_list.sort()

count = [0, 0, 0, 0, 0]

# count the number of occurrences of each number
for n in random_number_list:
    if n <= 20:
        count[0] += 1
    elif n <= 40:
        count[1] += 1
    elif n <= 60:
        count[2] += 1
    elif n <= 80:
        count[3] += 1
    else:
        count[4] += 1


# print output
for idx, n in enumerate(random_number_list):
    num_digits = len(str(n))
    print(n, end=' '*(5 - num_digits), flush=True)
    if ((idx + 1) % 20) == 0 and idx != 0: # new line every 20 numbers
        print()

print("--"*20) # divider

ranges = [" 1 -  20", "21 -  40", "41 -  60", "61 -  80", "81 - 100"]

for i in range(len(count)):
    print(f"{ranges[i]}: ", end='')
    for j in range(count[i]):
        print("*", end=' ')
    print(f"   {count[i]}")
