import sys


class Node:
    def show(self, indent=0):
        raise NotImplementedError

    def indent_str(self, indent):
        return "  " * indent


class Program(Node):
    def __init__(self, statements):
        self.statements = statements

    def show(self, indent=0):
        print(self.indent_str(indent) + "FileAST")
        for stmt in self.statements:
            stmt.show(indent + 1)


class Decl(Node):
    def __init__(self, name, type_node, init_expr=None):
        self.name = name
        self.type_node = type_node
        self.init_expr = init_expr

    def show(self, indent=0):
        print(self.indent_str(indent) + f"Decl: {self.name}, [], [], [], []")
        self.type_node.show(indent + 1)
        if self.init_expr:
            print(self.indent_str(indent + 1) + "Init:")
            self.init_expr.show(indent + 2)


class Type(Node):
    def __init__(self, typename):
        self.typename = typename

    def show(self, indent=0):
        print(self.indent_str(indent) + f"Type: {self.typename}, [], None")


class Constant(Node):
    def __init__(self, value, type):
        self.value = value

    def show(self, indent=0):
        val_type = "int" if isinstance(self.value, int) else "double"
        print(self.indent_str(indent) + f"Constant: {val_type}, {self.value}")


class ID(Node):
    def __init__(self, name):
        self.name = name

    def show(self, indent=0):
        print(self.indent_str(indent) + f"ID: {self.name}")


class BinaryOp(Node):
    def __init__(self, op, left, right):
        self.op = op
        self.left = left
        self.right = right

    def show(self, indent=0):
        print(self.indent_str(indent) + f"BinaryOp: {self.op}")
        self.left.show(indent + 1)
        self.right.show(indent + 1)


class Assignment(Node):
    def __init__(self, target, expr):
        self.target = target
        self.expr = expr

    def show(self, indent=0):
        print(self.indent_str(indent) + f"Assignment: {self.target.name}")
        self.expr.show(indent + 1)


class FuncCall(Node):
    def __init__(self, name, arg):
        self.name = name
        self.arg = arg  # just one arg for printf

    def show(self, indent=0):
        print(self.indent_str(indent) + f"FuncCall: {self.name}")
        print(self.indent_str(indent + 1) + "Arg:")
        self.arg.show(indent + 2)


#
def tokenize(source_code):
    pass

#


def parse(tokens):
    return Program([
        Decl("a", Type("int"), Constant("2")),
        Decl("b", Type("int"), Constant("3")),
        Decl("c", Type("int"), BinaryOp('+', ID("a"), ID("b"))),
        FuncCall("printf", ID("c"))
    ])


# evaluates the AST and prints the result
def evaluate(ast):
    ...


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
    ast = parse(tokens)
    ast.show()
    evaluate(ast)


if __name__ == "__main__":
    main()
