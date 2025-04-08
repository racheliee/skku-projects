from pycparser import CParser

# read file name and open it
with open("test.c", 'r') as f:
    source_code = f.read()
    
# parse the C code
parser = CParser()
ast = parser.parse(source_code)
ast.show()
