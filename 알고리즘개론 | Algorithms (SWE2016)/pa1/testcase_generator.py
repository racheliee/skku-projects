import random

my_list = ["I " + str(i) for i in random.sample(range(-2147483648, 2147483647), 300000)]
my_list2 = [random.choice(["D X", "D M", "D E", "F X", "F M", "F E"]) for _ in range(200000)]

combined_list = my_list + my_list2
random.shuffle(combined_list)

print(500000)
for item in combined_list:
    print(item)

