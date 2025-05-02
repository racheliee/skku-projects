
def recursive_sum(n) -> int:
    if n == 1:
        return 1
    
    return n + recursive_sum(n-1)

def large_num_sum(n) -> int:
    return (n * (n + 1)) // 2

def main():
    while (True):
        val = input("Enter a number: ")
        
        if val.lower() == "exit":
            break
        
        try: 
            num = int(val)
            if num > 998: # to prevent maximum recursion depth exceeded error
                ret = large_num_sum(num)
            elif num < 0: # skip negative numbers
                print("Please enter a number greater than or equal to 0")
                continue
            else:
                ret = recursive_sum(num-1)
            print(f"Output: {ret}")
        except ValueError:
            print("Invalid Input. Please enter a valid integer or \'Exit\' to exit")
            continue
    
    
if __name__ == "__main__":
    print("Welcome to the sum program. Type \'Exit\' to exit")
    main()