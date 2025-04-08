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
        print(self.indent_str(indent) + "FileAST:")
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
            self.init_expr.show(indent + 1)


class Type(Node):
    def __init__(self, name, typename):
        self.name = name
        self.typename = typename

    def show(self, indent=0):
        print(self.indent_str(indent) + f"TypeDecl: {self.name}, [], None")
        print(self.indent_str(indent + 1) + f"IdentifierType: ['{self.typename}']")

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

class FuncDef(Node):
    def __init__(self, decl, body):
        self.decl = decl  # Decl that wraps a FuncDecl
        self.body = body  # Compound

    def show(self, indent=0):
        print(self.indent_str(indent) + "FuncDef:")
        self.decl.show(indent + 1)
        self.body.show(indent + 1)


class FuncCall(Node):
    def __init__(self, name, arg):
        self.name = name
        self.arg = arg  # just one arg for printf

    def show(self, indent=0):
        print(self.indent_str(indent) + "FuncCall:")
        print(self.indent_str(indent + 1) + f"ID: {self.name.name}")
        if self.arg.exprs:
            self.arg.show(indent + 1)


class ExprList(Node):
    def __init__(self, exprs):
        self.exprs = exprs

    def show(self, indent=0):
        print(self.indent_str(indent) + "ExprList:")
        for expr in self.exprs:
            expr.show(indent + 1)


class Compound(Node):
    def __init__(self, decls, stmts):
        self.decls = decls
        self.stmts = stmts

    def show(self, indent=0):
        print(self.indent_str(indent) + "Compound:")
        for decl in self.decls:
            decl.show(indent + 1)
        for stmt in self.stmts:
            stmt.show(indent + 1)


class Return(Node):
    def __init__(self, expr):
        self.expr = expr

    def show(self, indent=0):
        print(self.indent_str(indent) + "Return:")
        self.expr.show(indent + 1)


class Constant(Node):
    def __init__(self, value, type):
        self.value = value

    def show(self, indent=0):
        val_type = "int" if isinstance(self.value, int) else "double" if isinstance(self.value, float) else "string"
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


#
def tokenize(source_code):
    pass

#


def parse(tokens):
    return Program([
        FuncDef(
            Decl("userDefined2", FuncDecl("userDefined2", Type("userDefined2", "void"), [])),
            Compound(
                decls=[
                    Decl("a", Type("a", "float"), Constant(2.2, "double")),
                    Decl("b", Type("b", "float"), Constant(4.64, "double")),
                    Decl("c", Type("c", "float"), BinaryOp("+", ID("a"), ID("b")))
                ],
                stmts=[
                    FuncCall(
                        ID("printf"),
                        ExprList([
                            Constant("\"%f\"", "string"),
                            ID("c")
                        ])
                    )
                ]
            )
        ),
        FuncDef(
            Decl("main", FuncDecl("main", Type("main", "int"), [])),
            Compound(
                decls=[],
                stmts=[
                    FuncCall(ID("userDefined2"), ExprList([])),
                    Return(Constant(0, "int"))
                ]
            )

        )
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
