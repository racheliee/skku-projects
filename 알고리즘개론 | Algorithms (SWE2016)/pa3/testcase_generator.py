import random

def generate_test_case(num_nodes, num_operations):
    with open('mst.in', 'w') as f:
        f.write(f"{num_nodes}\n")

        operations = ['insertEdge', 'findMST', 'deleteEdge', 'changeWeight']
        weights = [5, 1, 2, 2]  # Adjust weights as needed

        for _ in range(num_operations):
            operation = random.choices(operations, weights = weights)[0]
            node1 = random.randint(1, num_nodes)
            node2 = random.randint(1, num_nodes)
            weight = random.randint(1, 100)

            if operation == 'insertEdge':
                f.write(f"{operation} {node1} {node2} {weight}\n")
            elif operation == 'findMST':
                f.write(f"{operation}\n")
            elif operation == 'deleteEdge':
                f.write(f"{operation} {node1} {node2}\n")
            elif operation == 'changeWeight':
                f.write(f"{operation} {node1} {node2} {weight}\n")

# Set the desired number of nodes and operations
num_nodes = 500
num_operations = 10000

# Generate the test case
generate_test_case(num_nodes, num_operations)
