import sys
from pycparser import c_parser


# read file name and open it
if (len(sys.argv) != 2):
        print("Usage: python3 test.py <source_file>")
        sys.exit(1)

try:
    with open(sys.argv[1], 'r') as f:
        source_code = f.read()
except Exception as e:
    print(f"Error reading file: {e}")
    sys.exit(1)
    
# parse the C code
parser = c_parser.CParser()
ast = parser.parse(source_code)
ast.show()