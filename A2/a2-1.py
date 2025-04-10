
import sys


# Nodes (from reference code provided) ===================================================================
class Node(object):
    __slots__ = ()
    def children(self):
        pass

    def show(self, buf=sys.stdout, offset=0, attrnames=False, showemptyattrs=True, nodenames=False, showcoord=False, _my_node_name=None):
        lead = ' ' * offset
        if nodenames and _my_node_name is not None:
            buf.write(lead + self.__class__.__name__ +
                      ' <' + _my_node_name + '>: ')
        else:
            buf.write(lead + self.__class__.__name__ + ': ')

        if self.attr_names:
            def is_empty(v): return v is None or (
                hasattr(v, '__len__') and len(v) == 0)
            nvlist = [(n, getattr(self, n)) for n in self.attr_names
                      if showemptyattrs or not is_empty(getattr(self, n))]
            if attrnames:
                attrstr = ', '.join('%s=%s' % nv for nv in nvlist)
            else:
                attrstr = ', '.join('%s' % v for (_, v) in nvlist)
            buf.write(attrstr)

        if showcoord:
            buf.write(' (at %s)' % self.coord)
        buf.write('\n')

        for (child_name, child) in self.children():
            child.show(
                buf,
                offset=offset + 2,
                attrnames=attrnames,
                showemptyattrs=showemptyattrs,
                nodenames=nodenames,
                showcoord=showcoord,
                _my_node_name=child_name)


class NodeVisitor(object):
    _method_cache = None

    def visit(self, node):
        if self._method_cache is None:
            self._method_cache = {}

        visitor = self._method_cache.get(node.__class__.__name__, None)
        if visitor is None:
            method = '_eval_' + node.__class__.__name__
            visitor = getattr(self, method, self.generic_visit)
            self._method_cache[node.__class__.__name__] = visitor

        return visitor(node)

    def generic_visit(self, node):
        for c in node:
            self.visit(c)


class Assignment(Node):
    '''
    fyi: the comments under each Node class are for me to understand each component of the AST
    these are the int a = 3; kind of statements
    - assignment operator (=)
    - has a left val and a right val (either side of the =)
    '''
    __slots__ = ('op', 'lvalue', 'rvalue', 'coord', '__weakref__')

    def __init__(self, op, lvalue, rvalue, coord=None):
        self.op = op
        self.lvalue = lvalue
        self.rvalue = rvalue
        self.coord = coord

    def children(self):
        nodelist = []
        if self.lvalue is not None:
            nodelist.append(("lvalue", self.lvalue))
        if self.rvalue is not None:
            nodelist.append(("rvalue", self.rvalue))
        return tuple(nodelist)

    def __iter__(self):
        if self.lvalue is not None:
            yield self.lvalue
        if self.rvalue is not None:
            yield self.rvalue

    attr_names = ('op', )


class BinaryOp(Node):
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
    __slots__ = ('op', 'left', 'right', 'coord', '__weakref__')

    def __init__(self, op, left, right, coord=None):
        self.op = op
        self.left = left
        self.right = right
        self.coord = coord

    def children(self):
        nodelist = []
        if self.left is not None:
            nodelist.append(("left", self.left))
        if self.right is not None:
            nodelist.append(("right", self.right))
        return tuple(nodelist)

    def __iter__(self):
        if self.left is not None:
            yield self.left
        if self.right is not None:
            yield self.right

    attr_names = ('op', )


class Compound(Node):
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
    __slots__ = ('block_items', 'coord', '__weakref__')

    def __init__(self, block_items, coord=None):
        self.block_items = block_items
        self.coord = coord

    def children(self):
        nodelist = []
        for i, child in enumerate(self.block_items or []):
            nodelist.append(("block_items[%d]" % i, child))
        return tuple(nodelist)

    def __iter__(self):
        for child in (self.block_items or []):
            yield child

    attr_names = ()

class Constant(Node):
    '''
    literal value

    Constant: int, 3
    '''
    __slots__ = ('type', 'value', 'coord', '__weakref__')

    def __init__(self, type, value, coord=None):
        self.type = type
        self.value = value
        self.coord = coord

    def children(self):
        nodelist = []
        return tuple(nodelist)

    def __iter__(self):
        return
        yield

    attr_names = ('type', 'value', )


class Decl(Node):
    '''
    declaration
    - declaring variables or function parameters

    Decl: userDefined2, [], [], [], []
    Decl: b, [], [], [], []
    - 
    '''
    __slots__ = ('name', 'quals', 'align', 'storage', 'funcspec',
                 'type', 'init', 'bitsize', 'coord', '__weakref__')

    def __init__(self, name, quals, align, storage, funcspec, type, init, bitsize, coord=None):
        self.name = name
        self.quals = quals
        self.align = align
        self.storage = storage
        self.funcspec = funcspec
        self.type = type
        self.init = init
        self.bitsize = bitsize
        self.coord = coord

    def children(self):
        nodelist = []
        if self.type is not None:
            nodelist.append(("type", self.type))
        if self.init is not None:
            nodelist.append(("init", self.init))
        if self.bitsize is not None:
            nodelist.append(("bitsize", self.bitsize))
        return tuple(nodelist)

    def __iter__(self):
        if self.type is not None:
            yield self.type
        if self.init is not None:
            yield self.init
        if self.bitsize is not None:
            yield self.bitsize

    attr_names = ('name', 'quals', 'align', 'storage', 'funcspec', )

class ExprList(Node):
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
    __slots__ = ('exprs', 'coord', '__weakref__')

    def __init__(self, exprs, coord=None):
        self.exprs = exprs
        self.coord = coord

    def children(self):
        nodelist = []
        for i, child in enumerate(self.exprs or []):
            nodelist.append(("exprs[%d]" % i, child))
        return tuple(nodelist)

    def __iter__(self):
        for child in (self.exprs or []):
            yield child

    attr_names = ()


class FileAST(Node):
    '''
    FileAST rootnode
    '''
    __slots__ = ('ext', 'coord', '__weakref__')

    def __init__(self, ext, coord=None):
        self.ext = ext
        self.coord = coord

    def children(self):
        nodelist = []
        for i, child in enumerate(self.ext or []):
            nodelist.append(("ext[%d]" % i, child))
        return tuple(nodelist)

    def __iter__(self):
        for child in (self.ext or []):
            yield child

    attr_names = ()


class FuncCall(Node):
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
    __slots__ = ('name', 'args', 'coord', '__weakref__')

    def __init__(self, name, args, coord=None):
        self.name = name
        self.args = args
        self.coord = coord

    def children(self):
        nodelist = []
        if self.name is not None:
            nodelist.append(("name", self.name))
        if self.args is not None:
            nodelist.append(("args", self.args))
        return tuple(nodelist)

    def __iter__(self):
        if self.name is not None:
            yield self.name
        if self.args is not None:
            yield self.args

    attr_names = ()


class FuncDecl(Node):
    '''
    Function Signature
    - includes name, return type, and arguments (but no body)

    example: void userDefined2();
    Decl: userDefined2, [], [], [], []
        FuncDecl: 
        TypeDecl: userDefined2, [], None
            IdentifierType: ['void']
    '''
    __slots__ = ('args', 'type', 'coord', '__weakref__')

    def __init__(self, args, type, coord=None):
        self.args = args
        self.type = type
        self.coord = coord

    def children(self):
        nodelist = []
        if self.args is not None:
            nodelist.append(("args", self.args))
        if self.type is not None:
            nodelist.append(("type", self.type))
        return tuple(nodelist)

    def __iter__(self):
        if self.args is not None:
            yield self.args
        if self.type is not None:
            yield self.type

    attr_names = ()


class FuncDef(Node):
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
    __slots__ = ('decl', 'param_decls', 'body', 'coord', '__weakref__')

    def __init__(self, decl, param_decls, body, coord=None):
        self.decl = decl
        self.param_decls = param_decls
        self.body = body
        self.coord = coord

    def children(self):
        nodelist = []
        if self.decl is not None:
            nodelist.append(("decl", self.decl))
        if self.body is not None:
            nodelist.append(("body", self.body))
        for i, child in enumerate(self.param_decls or []):
            nodelist.append(("param_decls[%d]" % i, child))
        return tuple(nodelist)

    def __iter__(self):
        if self.decl is not None:
            yield self.decl
        if self.body is not None:
            yield self.body
        for child in (self.param_decls or []):
            yield child

    attr_names = ()


class ID(Node):
    '''
    Identifier

    - reference to a variable or function name

    example: a = 3;
    ID: a
    example: userDefined2(3);
    ID: userDefined2
    '''
    __slots__ = ('name', 'coord', '__weakref__')

    def __init__(self, name, coord=None):
        self.name = name
        self.coord = coord

    def children(self):
        nodelist = []
        return tuple(nodelist)

    def __iter__(self):
        return
        yield

    attr_names = ('name', )


class IdentifierType(Node):
    '''
    identifier is the type of the variable
    example: int a;
    '''
    __slots__ = ('names', 'coord', '__weakref__')

    def __init__(self, names, coord=None):
        self.names = names
        self.coord = coord

    def children(self):
        nodelist = []
        return tuple(nodelist)

    def __iter__(self):
        return
        yield

    attr_names = ('names', )

class ParamList(Node):
    __slots__ = ('params', 'coord', '__weakref__')

    def __init__(self, params, coord=None):
        self.params = params
        self.coord = coord

    def children(self):
        nodelist = []
        for i, child in enumerate(self.params or []):
            nodelist.append(("params[%d]" % i, child))
        return tuple(nodelist)

    def __iter__(self):
        for child in (self.params or []):
            yield child

    attr_names = ()


class Return(Node):
    '''
    return statement

    Return: 
        Constant: int, 0
    '''
    __slots__ = ('expr', 'coord', '__weakref__')

    def __init__(self, expr, coord=None):
        self.expr = expr
        self.coord = coord

    def children(self):
        nodelist = []
        if self.expr is not None:
            nodelist.append(("expr", self.expr))
        return tuple(nodelist)

    def __iter__(self):
        if self.expr is not None:
            yield self.expr

    attr_names = ()


class TypeDecl(Node):
    '''
    Type declaration
    - represents type information (e.g. int, float, etc.)

    example: float a;
    TypeDecl: a, [], None
        IdentifierType: ['float']
    '''
    __slots__ = ('declname', 'quals', 'align', 'type', 'coord', '__weakref__')

    def __init__(self, declname, quals, align, type, coord=None):
        self.declname = declname
        self.quals = quals
        self.align = align
        self.type = type
        self.coord = coord

    def children(self):
        nodelist = []
        if self.type is not None:
            nodelist.append(("type", self.type))
        return tuple(nodelist)

    def __iter__(self):
        if self.type is not None:
            yield self.type

    attr_names = ('declname', 'quals', 'align', )


class UnaryOp(Node):
    '''
    this is here for the - (minus) operator
    '''
    __slots__ = ('op', 'expr', 'coord', '__weakref__')

    def __init__(self, op, expr, coord=None):
        self.op = op
        self.expr = expr
        self.coord = coord

    def children(self):
        nodelist = []
        if self.expr is not None:
            nodelist.append(("expr", self.expr))
        return tuple(nodelist)

    def __iter__(self):
        if self.expr is not None:
            yield self.expr

    attr_names = ('op', )

# lexer ======================================================================


class Lexer:
    _keywords = {'int', 'float', 'double', 'return', 'void'}
    _symbols = [';', '{', '}', '(', ')', ',', '=', '+',
                '-', '*', '/', '%', '&', '|', '^', '<<', '>>']

    def __init__(self, source_code):
        self.source_code = source_code
        self._tokens = []

    def tokenize(self):
        for symbol in self._symbols:
            # add space around all symbols except mod (bc we need to take care of "%d" and stuff in printf)
            if symbol != "%":
                self.source_code = self.source_code.replace(
                    symbol, f" {symbol} ")

        # add spaces around mod
        in_string = False
        for i in range(len(self.source_code)):
            c = self.source_code[i]
            if c == '"':
                in_string = not in_string
            elif c == '%' and not in_string:
                self.source_code = self.source_code[:i] + \
                    " % " + self.source_code[i+1:]

        raw_tokens = self.source_code.split()
        # print("Raw tokens:", raw_tokens)

        # add to tokens in form of (TYPE, VALUE)
        # types can be KEYWORD, ID, STRING, NUM, FLOAT, SYMBOL
        for tok in raw_tokens:
            if tok in self._keywords:
                self._tokens.append(('KEYWORD', tok))
            elif tok.isidentifier():
                self._tokens.append(('ID', tok))
            elif tok.startswith('"') and tok.endswith('"'):
                self._tokens.append(('STRING', tok))
            elif '.' in tok:
                try:
                    self._tokens.append(('FLOAT', float(tok)))
                except ValueError:
                    raise ValueError(f"Invalid float: {tok}")
            elif tok.isdigit():
                self._tokens.append(('NUM', int(tok)))
            elif tok in self._symbols:
                self._tokens.append(('SYMBOL', tok))
            else:
                raise ValueError(f"Unknown token: {tok}")

        return self._tokens


# parser ======================================================================
class Parser:
    _types = ['int', 'float', 'double', 'void']
    _precedence = {  # from the provided reference
        '||': 0,  # weakest binding
        '&&': 1,
        '|': 2,
        '^': 3,
        '&': 4,
        '==': 5, '!=': 5,
        '>': 6, '>=': 6, '<': 6, '<=': 6,
        '>>': 7, '<<': 7,
        '+': 8, '-': 8,
        '*': 9, '/': 9, '%': 9  # strongest binding
    }
    _tokens: list[tuple[str, str]]  # list of tokens (type, value)
    _pos: int # current position in the token list

    def __init__(self, tokens):
        self._tokens = tokens
        self._pos = 0

    def _peek(self, offset=0):  # no change in pos btw!!! only checks the next token
        if self._pos + offset < len(self._tokens):
            return self._tokens[self._pos + offset]
        return None

    def _consume(self, expected_type=None, expected_val=None):  # advances pos
        curr = self._peek()
        if curr is None:
            return None
        tok_type, tok_val = curr

        if expected_type and tok_type != expected_type:
            return None
        if expected_val and tok_val != expected_val:
            return None

        self._pos += 1
        return curr

    def _is_prototype(self):
        # return True if the current token is a function prototype (ends with ;)
        # used to check if it's a function prototype or a function definition
        pos = self._pos

        if pos + 2 >= len(self._tokens):  # keyword + id
            return False

        pos += 3
        depth = 1
        while pos < len(self._tokens) and depth > 0:
            curr_val = self._tokens[pos][1]
            if curr_val == '(':
                depth += 1
            elif curr_val == ')':
                depth -= 1
            pos += 1

        if pos >= len(self._tokens) or self._tokens[pos][1] != ';':
            return False

        return True

    def parse(self):
        elements = []

        # no need to consider global variables
        while self._peek() is not None:
            token_type = self._is_prototype()
        #    print("token_type:", token_type)
            if token_type:
                elements.append(self._parse_prototype())
            else:
                elements.append(self._parse_funcdef())
        return FileAST(elements)

    def _parse_prototype(self):
        return_type = self._consume('KEYWORD')[1]
        function_name = self._consume('ID')[1]
        self._consume('SYMBOL', '(')

        params = []
        while self._peek() is not None and self._peek()[1] != ')':
            # print("params:", self._peek())
            param_type = self._parse_type()
            param_name = self._parse_identifier()
            params.append(Decl(
                name=param_name,
                quals=[], align=[], storage=[], funcspec=[],
                type=TypeDecl(param_name, [], None,
                              IdentifierType([param_type])),
                init=None, bitsize=None
            ))
            if self._peek()[1] == ')':
                break
            self._consume('SYMBOL', ',')
        self._consume('SYMBOL', ')')
        self._consume('SYMBOL', ';')

        # print('tokens:', self._tokens)
        # print("pos:", self._pos)

        return Decl(
            name=function_name,
            quals=[], align=[], storage=[], funcspec=[],
            type=FuncDecl(
                args=ParamList(params) if len(params) > 0 else None,
                type=TypeDecl(function_name, [], None,
                              IdentifierType([return_type]))
            ),
            init=None, bitsize=None
        )

    def _parse_funcdef(self):
        return_type = self._consume('KEYWORD')[1]
        function_name = self._consume('ID')[1]
        self._consume('SYMBOL', '(')

        params = []
        while self._peek() is not None and self._peek()[1] != ')':
            # print("params:", self._peek())
            param_type = self._parse_type()
            param_name = self._parse_identifier()
            params.append(Decl(
                name=param_name,
                quals=[], align=[], storage=[], funcspec=[],
                type=TypeDecl(param_name, [], None,
                              IdentifierType([param_type])),
                init=None, bitsize=None
            ))
            if self._peek()[1] == ')':
                break
            self._consume('SYMBOL', ',')
        self._consume('SYMBOL', ')')
        self._consume('SYMBOL', '{')

        # print("tokens", self._tokens)
        # print("pos: ", self._pos)

        # check for declarations/statements in function --> basically it's a compound statement
        body = []
        typ, tok = self._peek()
        while typ is not None and tok != '}':
            # print(self._peek()[1])
            if typ == 'KEYWORD' and tok in self._types:
                body.append(self._parse_declaration())
            else:
                # print("parse_statement")
                body.append(self._parse_statement())

            typ, tok = self._peek()

        self._consume('SYMBOL', '}')

        # print("decls:", decls)

        return FuncDef(
            decl=Decl(
                name=function_name,
                quals=[], align=[], storage=[], funcspec=[],
                type=FuncDecl(
                    args=ParamList(params) if len(params) > 0 else None,
                    type=TypeDecl(function_name, [], None,
                                  IdentifierType([return_type]))
                ),
                init=None, bitsize=None
            ),
            param_decls=[],  # leave empty; otherwise, Decl is printed twice for param args
            body=Compound(body)
        )

    def _parse_statement(self):
        # statement: single unit of execution
        # declaration, assignment, function call, return, compound, expressions 임!! 그냥 한줄이라고 생각하면 댐

        typ, tok = self._peek()
        if typ is None or tok is None:
            return None

        if typ == 'KEYWORD':
            if tok == 'return':
                return self._parse_return()
            elif tok in self._types:
                return self._parse_declaration()
        elif typ == 'ID':
            next_typ, next_tok = self._peek(1)

            if next_typ == 'SYMBOL' and next_tok == '=':  # assignment
                return self._parse_assignment()
            elif next_typ == 'SYMBOL' and next_tok == '(':  # function call
                # print("parse_func_call")
                return self._parse_func_call()
            elif next_typ == 'SYMBOL' and next_tok == ';':  # just an identifier
                self._consume('ID')
                self._consume('SYMBOL', ';')
                return ID(tok)
            else:
                # print("parse_expression")
                return self._parse_expression()

    def _parse_expression(self, min_prec=0):
        '''
        something that may evaluate to a value
        ex: a + b, 3, foo(1)
        '''
        left = self._parse_term()
        # print("parse_expression left:", left)

        while True:
            operator = self._peek()

            # don't think precendence needs to be checked but just in cases lol
            if operator is None or operator[0] != 'SYMBOL' or operator[1] not in self._precedence:
                break

            op_prec = self._precedence[operator[1]]
            if op_prec < min_prec:
                break

            self._consume('SYMBOL', operator[1])
            right = self._parse_expression(op_prec + 1)
            left = BinaryOp(operator[1], left, right)

        self._consume('SYMBOL', ';')

        return left

    def _parse_func_call(self):
        typ, tok = self._peek()
        if typ is None or tok is None:
            return None
        # print("parse_func_call:", typ, tok)
        if typ == 'ID':
            function_name = self._consume('ID')[1]
            self._consume('SYMBOL', '(')

            args = []
            while self._peek() is not None and self._peek()[1] != ')':
                args.append(self._parse_term())
                if self._peek()[1] == ')':
                    break
                self._consume('SYMBOL', ',')
            self._consume('SYMBOL', ')')
            self._consume('SYMBOL', ';')

            # print("args:", args)

            return FuncCall(
                name=ID(function_name),
                args=ExprList(args) if len(args) > 0 else None,
            )
        return None

    def _parse_assignment(self):
        typ, tok = self._peek()
        if typ is None or tok is None:
            return None

        # print("parse_assignment:", typ, tok)
        if typ == 'ID':
            var_name = self._consume('ID')[1]
            self._consume('SYMBOL', '=')
            expr = self._parse_expression()
            # print("expr:", expr)
            self._consume('SYMBOL', ';')
            return Assignment(
                op='=',
                lvalue=ID(var_name),
                rvalue=expr
            )
        elif typ == 'KEYWORD':  # there shouldn't be any?
            ...
        elif typ == 'SYMBOL':
            if tok == '-':
                self._consume('SYMBOL', '-')
                expr = self._parse_term()
                return UnaryOp('-', expr)
            elif tok == '(':
                self._consume('SYMBOL', '(')
                expr = self._parse_expression()
                self._consume('SYMBOL', ')')
                return expr
        elif typ == 'NUM':
            self._consume('NUM')
            return Constant('int', tok)
        elif typ == 'FLOAT':  # double check with float & double calcs
            self._consume('FLOAT')
            return Constant('double', tok)

        # strings are only used for printf (double check this)
        elif typ == 'STRING':
            self._consume('STRING')
            return Constant('string', tok)

    def parse_compound(self):
        # wired in within funcdef... no conditionals or loops so i guess none needed?
        ...

    def _parse_term(self):
        typ, tok = self._peek()
        if typ is None or tok is None:
            return None

        # print("parse_term:", typ, tok)

        if typ == 'NUM':
            self._consume('NUM')
            return Constant('int', tok)
        elif typ == 'FLOAT':
            self._consume('FLOAT')
            return Constant('double', tok)
        elif typ == 'STRING':  # there shouldn't be any cases though i think?
            self._consume('STRING')
            return Constant('string', tok)
        elif typ == 'ID':
            # check if it's a function call
            next_typ, next_tok = self._peek(1)
            if next_typ == 'SYMBOL' and next_tok == '(':
                return self._parse_func_call()
            elif next_typ == 'SYMBOL' and next_tok == '=':
                return self._parse_assignment()
            self._consume('ID')
            self._consume('SYMBOL', ';')
            return ID(tok)
        elif typ == 'SYMBOL':
            if tok == '(':
                self._consume('SYMBOL', '(')
                expr = self._parse_expression()
                self._consume('SYMBOL', ')')
                return expr
            elif tok == '-':
                self._consume('SYMBOL', '-')
                expr = self._parse_term()
                return UnaryOp('-', expr)

    def _parse_declaration(self):
        typ, tok = self._peek()
        if typ is None or tok is None:
            return None

        init = None
        # print("parse_declaration:", typ, tok)
        if typ == 'KEYWORD' and tok in self._types:
            var_type = self._consume('KEYWORD')[1]
            var_name = self._consume('ID')[1]

            if self._peek() is not None and self._peek()[0] == 'SYMBOL' and self._peek()[1] == '=':
                self._consume('SYMBOL', '=')
                init = self._parse_expression()
            else:
                self._consume('SYMBOL', ';')

            return Decl(
                name=var_name,
                quals=[], align=[], storage=[], funcspec=[],
                type=TypeDecl(
                    var_name, [], None, IdentifierType([var_type])
                ),
                init=init,
                bitsize=None
            )

    def _parse_type(self):
        typ, tok = self._peek()
        if typ is None or tok is None:
            return None
        if typ == 'KEYWORD' and tok in self._types:
            return self._consume('KEYWORD')[1]
        return None

    def _parse_identifier(self):
        if self._peek() is None:
            return None
        if self._peek()[0] == 'ID':
            return self._consume('ID')[1]
        return None

    def _parse_return(self):
        if self._peek() is None:
            return None
        self._consume('KEYWORD', 'return')
        # print("consume return")
        return_statement = self._parse_term()
        # print("term parsed:", return_statement)
        self._consume('SYMBOL', ';')
        return Return(expr=return_statement, coord=None)


# evaluator ======================================================================


class Evaluator(NodeVisitor):
    _precedence = {  # same as parser's precedence
        '||': 0,  # weakest binding
        '&&': 1,
        '|': 2,
        '^': 3,
        '&': 4,
        '==': 5, '!=': 5,
        '>': 6, '>=': 6, '<': 6, '<=': 6,
        '>>': 7, '<<': 7,
        '+': 8, '-': 8,
        '*': 9, '/': 9, '%': 9  # strongest binding
    }
    _variables: dict[str, any]  #{name: value}
    _functions: dict[str, FuncDef]  #{name: FuncDef}
    _return_val: any #return val of the function

    def __init__(self):
        self._variables = {}
        self._functions = {}
        self._return_val = None

    def _eval_FileAST(self, node):
        # add all of the functions in the _function dictonary
        for ext in node.ext:
            print("adding ext: ", ext.decl.name)
            self._functions[ext.decl.name] = ext
        
        # start by visiting the main function
        if 'main' in self._functions:
            self.visit(self._functions['main'])

    def _eval_FuncDef(self, node: Node):
        # print("visiting funcdef: ", node.decl.name)
        self._variables = {}  # reset for each function
        self._return_val = None
        
        is_not_main = node.decl.name != 'main'
        self._eval_Compound(node.body, in_function = is_not_main)

    def _eval_Compound(self, node, in_function = False):
        # print("visiting compound: ", node)
        if node.block_items is None:
            return

        for statements in node.block_items:
            self.visit(statements)
            # check if a statement was a return statement (for early checkout)
            if in_function and self._return_val is not None:
                return

    def _eval_Decl(self, node):
        # print("visiting decl: ", node.name)
        if node.init is not None:
            # print("init:", node.init)
            self._variables[node.name] = self.visit(node.init)
        else:
            self._variables[node.name] = None
            

    def _eval_Assignment(self, node):
        # print("lvalue:", node.lvalue.name)
        # print("rvalue:", node.rvalue.name)
        self._variables[node.lvalue.name] = self.visit(node.rvalue)

    def _eval_BinaryOp(self, node):
        # print("visiting binaryop: ", node.op)
        left = self.visit(node.left)
        right = self.visit(node.right)
    
        
        # print("left:", left)
        # print("right:", right)
        match node.op: 
            # 토론방: binary operator (+, -, *, /, %), bitwise operator(^, |, &, <<, >>) 만 고려하셔도 무방합니다.
            case '+':
                return left + right
            case '-':
                return left - right
            case '*':
                return left * right
            case '/':
                if right != 0:
                    return left / right
            case '%':
                if right != 0:
                    return left % right
            case '^':
                return left ^ right
            case '|':
                return left | right
            case '&':
                return left & right
            case '<<':
                # to match c's overflow behaviour
                res =  (left << right) & 0xFFFFFFFF
                if res & (1 << 31):
                    res -= (1 << 32)
                return res
            case '>>':
                # to match c's overflow behaviour
                left = left & 0xFFFFFFFF
                
                if left & (1 << 31):
                    left -= (1 << 32)
                return left >> right
                

    def _eval_Constant(self, node):
        # print("visiting constant: ", node.value)
        if node.type == 'int':
            return int(node.value)
        elif node.type == 'float':
            return float(node.value)

        return node.value

    def _eval_UnaryOp(self, node):
        # print("visiting unaryop: ", node)
        if node.op == '-':
            return -self.visit(node.expr)
        return self.visit(node.expr)

    def _eval_ID(self, node):
        # print("visiting id: ", node.name)
        value = self._variables.get(node.name)
        if isinstance(value, Node):
            return self.visit(value)
        return value

    def _eval_FuncCall(self, node):
        # print("visiting funcCall: ", node.name.name)
        name = node.name.name
        if name == 'printf':
            format_type = node.args.exprs[0].value
            arg = self.visit(node.args.exprs[1])  # there's only one arg

            if format_type == '"%d"':
                print(f"Computation Result: {int(arg)}")
            elif format_type == '"%f"':
                print(f"Computation Result: {float(arg):.6f}")
            elif format_type == '"%lf"':
                print(f"Computation Result: {float(arg):.6f}")
            return

        # the user defined functions
        if name in self._functions:
            func = self._functions[name]
            args = node.args.exprs if node.args is not None else []
            evaluated_args = [self.visit(arg) for arg in args]  
            
            # save the current variables 
            old_vars = self._variables.copy()
            
            param_names = [param.name for param in func.decl.type.args.params]
            self._variables = {name: val for name, val in zip(param_names, evaluated_args)}
            
            self._return_val = None
            self._eval_Compound(func.body, in_function = True)
            
            # restore the old variables
            self._variables = old_vars
            
            # return the value of the function
            return self._return_val
            
             

    def _eval_Return(self, node):
        # print("visiting return: ", node)
        self._return_val = self.visit(node.expr)
        # return self.visit(node.expr)


# main function ============================================================================================
def main():
    # check if c file is provided
    if (len(sys.argv) != 2):
        print("Usage: python3 test.py <source_file>")
        sys.exit(1)

    try:
        with open(sys.argv[1], 'r') as f:
            source_code = f.read()
    except Exception as e:
        print(f"Error reading file: {e}")
        sys.exit(1)

    tokens = Lexer(source_code).tokenize()
    # print("Tokens:", tokens)
    ast = Parser(tokens).parse()
    ast.show()
    # print("Computation Result: ", end="") # no this is going to be printed for each line of printf
    evaluator = Evaluator()
    evaluator.visit(ast)


if __name__ == "__main__":
    main()
