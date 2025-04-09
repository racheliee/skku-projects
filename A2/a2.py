import sys

# Node classes ====================================================================================
# node is an abstract class for all nodes in the AST
class Node:
    def show(self, indent=0):
        raise NotImplementedError

    def indent_str(self, indent):
        return "  " * indent

'''
Root of AST
FileAST:
'''
class Program(Node):
    def __init__(self, statements):
        self.statements = statements  # list

    def show(self, indent=0):
        print(self.indent_str(indent) + "FileAST:")
        for statement in self.statements:
            statement.show(indent + 1)

'''
Function Signature
- includes name, return type, and arguments (but no body)

example: void userDefined2();
Decl: userDefined2, [], [], [], []
    FuncDecl: 
      TypeDecl: userDefined2, [], None
        IdentifierType: ['void']
'''
class FuncDecl(Node):
    def __init__(self, name, type_node, args):
        self.name = name
        self.type_node = type_node
        self.args = args

    def show(self, indent=0):
        print(self.indent_str(indent) + "FuncDecl:")
        self.type_node.show(indent + 1)
        for arg in self.args:
            arg.show(indent + 1)

'''
function definition
- entire function signature & body
- wraps FuncDecl and Compound

FuncDef: 
    Decl: userDefined2, [], [], [], []
      FuncDecl: 
        TypeDecl: userDefined2, [], None
          IdentifierType: ['void']
    Compound: 
      Decl: a, [], [], [], []
        TypeDecl: a, [], None
          IdentifierType: ['float']
        Constant: double, 2.2
    // etc...
'''
class FuncDef(Node):
    def __init__(self, decl, body):
        self.decl = decl  # FuncDecl
        self.body = body  # Compound

    def show(self, indent=0):
        print(self.indent_str(indent) + "FuncDef:")
        self.decl.show(indent + 1)
        self.body.show(indent + 1)


'''
declaration
- declaring variables or function parameters

Decl: userDefined2, [], [], [], []
Decl: b, [], [], [], []
- 
'''
class Decl(Node):
    def __init__(self, name, type_node, init_expr=None):
        self.name = name  # string
        self.type_node = type_node  # Node for the type (e.g. int, float)
        # Node for the initializer expression (i.e. what comes after the '=' sign)
        self.init_expr = init_expr

    def show(self, indent=0):
        print(self.indent_str(indent) + f"Decl: {self.name}, [], [], [], []")
        # TypeDecl: a, [], None // IdentifierType: ['float']
        self.type_node.show(indent + 1)
        if self.init_expr:  # if there is an initializer expression
            # Constant: double, 2.2 or ID:a or BinaryOp: + or FuncCall: userDefined
            self.init_expr.show(indent + 1)


'''
Type declaration
- represents type information (e.g. int, float, etc.)

example: float a;
TypeDecl: a, [], None
    IdentifierType: ['float']
'''
class Type(Node):
    def __init__(self, name, typename):
        self.name = name  # string
        self.typename = typename  # string

    def show(self, indent=0):
        print(self.indent_str(indent) + f"TypeDecl: {self.name}, [], None")
        print(self.indent_str(indent + 1) +
              f"IdentifierType: ['{self.typename}']")

'''
function call
- using the function name and the arguments
- if no args, ExprList is empty (not printed at all)

example: userDefined2(3);
FuncCall: 
    ID: userDefined2
    ExprList: 
        Constant: int, 3
'''
class FuncCall(Node):
    def __init__(self, name, arg):
        self.name = name
        self.arg = arg

    def show(self, indent=0):
        print(self.indent_str(indent) + "FuncCall:")
        print(self.indent_str(indent + 1) + f"ID: {self.name.name}")
        if self.arg.exprs:
            self.arg.show(indent + 1)

'''
Expression List
- list of expressions (e.g. arguments in a function call)

example: userDefined2(3, 4.5);
ExprList:
    Constant: int, 3
    Constant: double, 4.5

example: printf("%f", x);
ExprList:
    Constant: string, "%f"
    ID: x
'''
class ExprList(Node):
    def __init__(self, exprs):
        self.exprs = exprs

    def show(self, indent=0):
        print(self.indent_str(indent) + "ExprList:")
        for expr in self.exprs:
            expr.show(indent + 1)


'''
Compound statement
- a block of code (e.g. function body)

example:
{
    userDefined2(3);
    return 0;
}

Compound: 
    FuncCall: 
        ID: userDefined2
        ExprList: 
            Constant: int, 3
    Return: 
        Constant: int, 0
'''
class Compound(Node):
    def __init__(self, decls, statements):
        self.decls = decls
        self.statements = statements

    def show(self, indent=0):
        print(self.indent_str(indent) + "Compound:")
        for decl in self.decls:
            decl.show(indent + 1)
        for statement in self.statements:
            statement.show(indent + 1)

'''
return statement

Return: 
    Constant: int, 0
'''
class Return(Node):
    def __init__(self, expr):
        self.expr = expr

    def show(self, indent=0):
        print(self.indent_str(indent) + "Return:")
        self.expr.show(indent + 1)

'''
literal value 

Constant: int, 3
'''
class Constant(Node):
    def __init__(self, value):
        self.value = value

    def show(self, indent=0):
        val_type = "int" if isinstance(self.value, int) else "double" if isinstance(
            self.value, float) else "string"
        print(self.indent_str(indent) + f"Constant: {val_type}, {self.value}")

'''
Identifier

- reference to a variable or function name

example: a = 3;
ID: a
example: userDefined2(3);
ID: userDefined2
'''
class ID(Node):
    def __init__(self, name):
        self.name = name

    def show(self, indent=0):
        print(self.indent_str(indent) + f"ID: {self.name}")

'''
binary operation
- arithmetic or bitwise operations
- consider only the following operators: 
    - binary operator (+, -, *, /, %), bitwise operator(^, |, &, <<, >>)
    
example: a = 3 + 4;
BinaryOp: +
    Constant: int, 3
    Constant: int, 4
    
example: d = 2 << 2;
BinaryOp: <<
    Constant: int, 2
    Constant: int, 2
'''
class BinaryOp(Node):
    def __init__(self, op, left, right):
        self.op = op
        self.left = left
        self.right = right

    def show(self, indent=0):
        print(self.indent_str(indent) + f"BinaryOp: {self.op}")
        self.left.show(indent + 1)
        self.right.show(indent + 1)


# Parser functions ====================================================================================
class Parser:
    def __init__(self, tokens):
        self.tokens = tokens
        self.pos = 0
        
    def peek(self):
        if self.pos < len(self.tokens):
            return self.tokens[self.pos]
        return None
    
    def consume(self):
        ...

    def parse_program(self, tokens):
        ...

    # handles parsing variable declarations

    def parse_declaration(tokens):
        ...

    # handles assignments, calls, returns
    def parse_statement(tokens):
        ...

    # handles arithmetic and bitwise operations
    def parse_expression(tokens):
        ...

    # handles return statements
    def parse_return(tokens):
        ...

    # handle terminal nodes
    def parse_terminal(tokens):
        ...


# lexical analysis functions ====================================================================================

# tokenizes the source code into a list of tokens
def tokenize(source_code):
    # add spaces around operators and delimiters to split them
    symbols = [';', '{', '}', '(', ')', ',', '=', '+',
               '-', '*', '/', '%', '&', '|', '^', '<<', '>>']
    for symbol in symbols:
        source_code = source_code.replace(symbol, f" {symbol} ")
    tokens = source_code.split()
    
    keywords = ['int', 'float', 'double', 'void', 'return']
    token_list = []
    
    

    return token_list


# evaluation logic ====================================================================================
# evaluates the AST and prints the result
def evaluate(ast):
    ...


# main function ============================================================================================
def main():
    # check if c file is provided
    if (len(sys.argv) != 2):
        print("Usage: python3 test.py <source_file>")
        sys.exit(1)

    try:
        # check if file exists
        with open(sys.argv[1], 'r') as f:
            source_code = f.read()
    except Exception as e:
        print(f"Error reading file: {e}")
        sys.exit(1)

    tokens = tokenize(source_code)
    ast = Parser(tokens).parse_program(tokens)
    ast.show()
    evaluate(ast)


if __name__ == "__main__":
    main()
