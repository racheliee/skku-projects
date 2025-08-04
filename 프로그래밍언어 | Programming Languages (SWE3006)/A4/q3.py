def is_prime(n) -> bool:
    if n <= 1:
        return False
    if n <= 3:
        return True
    if n % 2 == 0 or n % 3 == 0:
        return False

    end = int(n**0.5) + 1
    for i in range(5, end, 6):  # check for 6k +/- 1
        if n % i == 0 or n % (i+2) == 0:
            return False

    return True


def find_prime_at_nth_rank(n) -> int:
    rank = 0
    num = 2
    while rank != n:
        if is_prime(num):
            rank += 1
        num += 1

    return num - 1


def main():
    while (True):
        val = input("What is the prime number at rank: ")

        if val.lower() == "exit":
            break

        try:
            num = int(val)
            if num < 1:  # rank starts from 1
                print("Please enter a number greater than or equal to 1")
                continue
            else:
                ret = find_prime_at_nth_rank(num)
            print(f"The prime number is {ret}")
        except ValueError:
            print("Invalid Input. Please enter a valid integer or \'Exit\' to exit")
            continue
        except KeyboardInterrupt:
            print("\nExiting the program.")
            break


if __name__ == "__main__":
    print("Welcome to the prime number rank program. Type \'Exit\' to exit")
    main()
