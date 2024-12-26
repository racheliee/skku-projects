import subprocess
import re

# function to count the number of hops shown in the traceroute output
def count_hops(res):
    output = res.split("\n")

    pattern = re.compile(r'^\s*\d+\s')  # optional space + digit + space (so it doesn't confuse it with ip addresses)

    count = 0
    for line in output:
        if pattern.match(line):
            count += 1

    return count


def main():
    host_name = input("Enter a server name: ")

    try:
        result = subprocess.run(["traceroute", host_name], text=True,
                                capture_output=True, timeout=20)  # give a 20 second timeout

        if result.returncode != 0:
            print("Traceroute: failed")
            return

        # print the number of hops
        print(f"{count_hops(result.stdout)} hops to {host_name}")

    except subprocess.TimeoutExpired:  # timeout
        print("Traceroute: failed")

    except Exception:  # other exceptions
        print("Traceroute: failed")


if __name__ == '__main__':
    main()
