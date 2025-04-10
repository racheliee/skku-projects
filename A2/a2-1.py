
import sys


def _repr(obj):
    """
    Get the representation of an object, with dedicated pprint-like format for lists.
    """
    if isinstance(obj, list):
        return '[' + (',\n '.join((_repr(e).replace('\n', '\n ') for e in obj))) + '\n]'
    else:
        return repr(obj)


class Node(object):
    __slots__ = ()
    """ Abstract base class for AST nodes.
    """

    def __repr__(self):
        """ Generates a python representation of the current node
        """
        result = self.__class__.__name__ + '('

        indent = ''
        separator = ''
        for name in self.__slots__[:-2]:
            result += separator
            result += indent
            result += name + '=' + (_repr(getattr(self, name)).replace(
                '\n', '\n  ' + (' ' * (len(name) + len(self.__class__.__name__)))))

            separator = ','
            indent = '\n ' + (' ' * len(self.__class__.__name__))

        result += indent + ')'

        return result

    def children(self):
        """ A sequence of all children that are Nodes
        """
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
        """ Visit a node.
        """

        if self._method_cache is None:
            self._method_cache = {}

        visitor = self._method_cache.get(node.__class__.__name__, None)
        if visitor is None:
            method = 'visit_' + node.__class__.__name__
            visitor = getattr(self, method, self.generic_visit)
            self._method_cache[node.__class__.__name__] = visitor

        return visitor(node)

    def generic_visit(self, node):
        """ Called if no explicit visitor function exists for a
            node. Implements preorder visiting of the node.
        """
        for c in node:
            self.visit(c)


class Assignment(Node):
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


class CompoundLiteral(Node):
    __slots__ = ('type', 'init', 'coord', '__weakref__')

    def __init__(self, type, init, coord=None):
        self.type = type
        self.init = init
        self.coord = coord

    def children(self):
        nodelist = []
        if self.type is not None:
            nodelist.append(("type", self.type))
        if self.init is not None:
            nodelist.append(("init", self.init))
        return tuple(nodelist)

    def __iter__(self):
        if self.type is not None:
            yield self.type
        if self.init is not None:
            yield self.init

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


class DeclList(Node):
    __slots__ = ('decls', 'coord', '__weakref__')

    def __init__(self, decls, coord=None):
        self.decls = decls
        self.coord = coord

    def children(self):
        nodelist = []
        for i, child in enumerate(self.decls or []):
            nodelist.append(("decls[%d]" % i, child))
        return tuple(nodelist)

    def __iter__(self):
        for child in (self.decls or []):
            yield child

    attr_names = ()


class Default(Node):
    __slots__ = ('stmts', 'coord', '__weakref__')

    def __init__(self, stmts, coord=None):
        self.stmts = stmts
        self.coord = coord

    def children(self):
        nodelist = []
        for i, child in enumerate(self.stmts or []):
            nodelist.append(("stmts[%d]" % i, child))
        return tuple(nodelist)

    def __iter__(self):
        for child in (self.stmts or []):
            yield child

    attr_names = ()


class EmptyStatement(Node):
    __slots__ = ('coord', '__weakref__')

    def __init__(self, coord=None):
        self.coord = coord

    def children(self):
        return ()

    def __iter__(self):
        return
        yield

    attr_names = ()


class ExprList(Node):
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


class InitList(Node):
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


class Label(Node):
    __slots__ = ('name', 'stmt', 'coord', '__weakref__')

    def __init__(self, name, stmt, coord=None):
        self.name = name
        self.stmt = stmt
        self.coord = coord

    def children(self):
        nodelist = []
        if self.stmt is not None:
            nodelist.append(("stmt", self.stmt))
        return tuple(nodelist)

    def __iter__(self):
        if self.stmt is not None:
            yield self.stmt

    attr_names = ('name', )


class NamedInitializer(Node):
    __slots__ = ('name', 'expr', 'coord', '__weakref__')

    def __init__(self, name, expr, coord=None):
        self.name = name
        self.expr = expr
        self.coord = coord

    def children(self):
        nodelist = []
        if self.expr is not None:
            nodelist.append(("expr", self.expr))
        for i, child in enumerate(self.name or []):
            nodelist.append(("name[%d]" % i, child))
        return tuple(nodelist)

    def __iter__(self):
        if self.expr is not None:
            yield self.expr
        for child in (self.name or []):
            yield child

    attr_names = ()


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


class Typedef(Node):
    __slots__ = ('name', 'quals', 'storage', 'type', 'coord', '__weakref__')

    def __init__(self, name, quals, storage, type, coord=None):
        self.name = name
        self.quals = quals
        self.storage = storage
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

    attr_names = ('name', 'quals', 'storage', )


class Typename(Node):
    __slots__ = ('name', 'quals', 'align', 'type', 'coord', '__weakref__')

    def __init__(self, name, quals, align, type, coord=None):
        self.name = name
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

    attr_names = ('name', 'quals', 'align', )


class UnaryOp(Node):
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
    _keywords = {'int', 'float', 'double', 'return', 'void', 'printf'}
    _symbols = [';', '{', '}', '(', ')', ',', '=', '+',
                '-', '*', '/', '%', '&', '|', '^', '<<', '>>']

    def __init__(self, source_code):
        self.source_code = source_code
        self.tokens = []

    def tokenize(self):
        for symbol in self._symbols:
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

        for tok in raw_tokens:
            if tok in self._keywords:
                self.tokens.append(('KEYWORD', tok))
            elif tok.isidentifier():
                self.tokens.append(('ID', tok))
            elif tok.startswith('"') and tok.endswith('"'):
                self.tokens.append(('STRING', tok))
            elif '.' in tok:
                try:
                    self.tokens.append(('FLOAT', float(tok)))
                except ValueError:
                    raise ValueError(f"Invalid float: {tok}")
            elif tok.isdigit():
                self.tokens.append(('NUM', int(tok)))
            elif tok in self._symbols:
                self.tokens.append(('SYMBOL', tok))
            else:
                raise ValueError(f"Unknown token: {tok}")

        return self.tokens


# parser ======================================================================
class Parser:
    types = ['int', 'float', 'double', 'void']
    precedence = {
        '|': 1,
        '^': 2,
        '&': 3,
        '<<': 4, '>>': 4,
        '+': 5, '-': 5,
        '*': 6, '/': 6, '%': 6,
    }

    def __init__(self, tokens):
        self.tokens = tokens
        self.pos = 0

    def peek(self, offset=0):
        ' this does not change the position of the token stream; just checks next or offset amt'
        if self.pos + offset < len(self.tokens):
            return self.tokens[self.pos + offset]
        return None

    def consume(self, expected_type=None, expected_val=None):
        ' this changes the position of the token stream '
        curr = self.peek()
        if curr is None:
            return None
        tok_type, tok_val = curr

        if expected_type and tok_type != expected_type:
            return None
        if expected_val and tok_val != expected_val:
            return None

        self.pos += 1
        return curr

    def is_prototype(self):
        ''' return True if the current token is a function prototype (ends with ;) '''
        pos = self.pos

        if pos + 2 >= len(self.tokens):  # keyword + id
            return False

        pos += 3
        depth = 1
        while pos < len(self.tokens) and depth > 0:
            curr_val = self.tokens[pos][1]
            if curr_val == '(':
                depth += 1
            elif curr_val == ')':
                depth -= 1
            pos += 1

        if pos >= len(self.tokens) or self.tokens[pos][1] != ';':
            return False

        return True

    def parse(self):
        elements = []

        # no need to consider global variables
        while self.peek() is not None:
            token_type = self.is_prototype()
        #    print("token_type:", token_type)
            if token_type:
                elements.append(self.parse_prototype())
            else:
                elements.append(self.parse_funcdef())
        return FileAST(elements)

    def parse_prototype(self):
        return_type = self.consume('KEYWORD')[1]
        function_name = self.consume('ID')[1]
        self.consume('SYMBOL', '(')

        params = []
        while self.peek() is not None and self.peek()[1] != ')':
            # print("params:", self.peek())
            param_type = self.parse_type()
            param_name = self.parse_identifier()
            params.append(Decl(
                name=param_name,
                quals=[], align=[], storage=[], funcspec=[],
                type=TypeDecl(param_name, [], None,
                              IdentifierType([param_type])),
                init=None, bitsize=None
            ))
            if self.peek()[1] == ')':
                break
            self.consume('SYMBOL', ',')
        self.consume('SYMBOL', ')')
        self.consume('SYMBOL', ';')

        # print('tokens:', self.tokens)
        # print("pos:", self.pos)

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

    def parse_funcdef(self):
        return_type = self.consume('KEYWORD')[1]
        function_name = self.consume('ID')[1]
        self.consume('SYMBOL', '(')

        params = []
        while self.peek() is not None and self.peek()[1] != ')':
            # print("params:", self.peek())
            param_type = self.parse_type()
            param_name = self.parse_identifier()
            params.append(Decl(
                name=param_name,
                quals=[], align=[], storage=[], funcspec=[],
                type=TypeDecl(param_name, [], None,
                              IdentifierType([param_type])),
                init=None, bitsize=None
            ))
            if self.peek()[1] == ')':
                break
            self.consume('SYMBOL', ',')
        self.consume('SYMBOL', ')')
        self.consume('SYMBOL', '{')

        # print("tokens", self.tokens)
        # print("pos: ", self.pos)

        # check for declarations/statements in function --> basically it's a compound statement
        body = []
        typ, tok = self.peek()
        while typ is not None and tok != '}':
            # print(self.peek()[1])
            if typ == 'KEYWORD' and tok in self.types:
                body.append(self.parse_declaration())
            else:
                # print("parse_statement")
                body.append(self.parse_statement())

            typ, tok = self.peek()

        self.consume('SYMBOL', '}')

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

    def parse_statement(self):
        '''
        single unit of execution
        - declaration, assignment, function call, return, compound, expressions
        '''
        typ, tok = self.peek()
        if typ is None or tok is None:
            return None

        if typ == 'KEYWORD' and tok == 'return':
            return self.parse_return()
        if typ == 'ID':
            next_typ, next_tok = self.peek(1)

            if next_typ == 'SYMBOL' and next_tok == '=':  # assignment
                return self.parse_assignment()
            elif next_typ == 'SYMBOL' and next_tok == '(':  # function call
                return self.parse_func_call()
            elif next_typ == 'SYMBOL' and next_tok == ';':  # just an identifier
                self.consume('ID')
                self.consume('SYMBOL', ';')
                return ID(tok)
        if typ == 'KEYWORD':
            ...

    def parse_expression(self, min_prec=0):
        '''
        something that may evaluate to a value
        ex: a + b, 3, foo(1)
        '''
        left = self.parse_term()
        
        while True:
            operator = self.peek()
            
            if operator is None or operator[0] != 'SYMBOL' or operator[1] not in self.precedence: # don't think precendence needs to be checked but just in cases lol
                break
            
            op_prec = self.precedence[operator[1]]
            if op_prec < min_prec:
                break
            
            self.consume('SYMBOL', operator[1])
            right = self.parse_expression(op_prec)
            left = BinaryOp(operator[1], left, right)
        
        return left

    def parse_func_call(self):
        typ, tok = self.peek()
        if typ is None or tok is None:
            return None
        if typ == 'ID':
            function_name = self.consume('ID')[1]
            self.consume('SYMBOL', '(')

            args = []
            while self.peek() is not None and self.peek()[1] != ')':
                args.append(self.parse_term())
                if self.peek()[1] == ')':
                    break
                self.consume('SYMBOL', ',')
            self.consume('SYMBOL', ')')
            self.consume('SYMBOL', ';')

            return FuncCall(
                name=ID(function_name),
                args=ExprList(args) if len(args) > 0 else None,
            )
        return None

    def parse_assignment(self):
        typ, tok = self.peek()
        if typ is None or tok is None:
            return None

        print("parse_assignment:", typ, tok)
        if typ == 'ID':
            var_name = self.consume('ID')[1]
            self.consume('SYMBOL', '=')
            expr = self.parse_expression()
            # print("expr:", expr)
            self.consume('SYMBOL', ';')
            return Assignment(
                op='=',
                lvalue=ID(var_name),
                rvalue=expr
            )
        elif typ == 'KEYWORD':
            ...
        elif typ == 'SYMBOL':
            ...
        elif typ == 'NUM':
            ...
        elif typ == 'FLOAT':
            ...
        # strings are only used for printf (double check this)
        elif typ == 'STRING':
            ...

    def parse_compound(self): ...

    def parse_term(self):
        typ, tok = self.peek()
        if typ is None or tok is None:
            return None

        # print("parse_term:", typ, tok)

        if typ == 'NUM':
            self.consume('NUM')
            return Constant('int', tok)
        elif typ == 'FLOAT':
            return Constant('double', tok)
        elif typ == 'STRING':  # there shouldn't be any cases though i think?
            return Constant('string', tok)
        elif typ == 'ID':
            # check if it's a function call
            next_typ, next_tok = self.peek(1)
            if next_typ == 'SYMBOL' and next_tok == '(':
                return self.parse_func_call()
            elif next_typ == 'SYMBOL' and next_tok == '=':
                return self.parse_assignment()
            self.consume('ID')
            self.consume('SYMBOL', ';')
            return ID(tok)
        elif typ == 'SYMBOL':
            if tok == '(':
                self.consume('SYMBOL', '(')
                expr = self.parse_expression()
                self.consume('SYMBOL', ')')
                return expr
            elif tok == '-':
                self.consume('SYMBOL', '-')
                expr = self.parse_term()
                return UnaryOp('-', expr)

    def parse_declaration(self):
        typ, tok = self.peek()
        if typ is None or tok is None:
            return None
        if typ == 'KEYWORD' and tok in self.types:
            var_type = self.consume('KEYWORD')[1]
            var_name = self.consume('ID')[1]
            self.consume('SYMBOL', ';')

            return Decl(
                name=var_name,
                quals=[], align=[], storage=[], funcspec=[],
                type=TypeDecl(
                    var_name, [], None, IdentifierType([var_type])
                ),
                init=None, bitsize=None
            )

    def parse_type(self):
        typ, tok = self.peek()
        if typ is None or tok is None:
            return None
        if typ == 'KEYWORD' and tok in self.types:
            return self.consume('KEYWORD')[1]
        return None

    def parse_identifier(self):
        if self.peek() is None:
            return None
        if self.peek()[0] == 'ID':
            return self.consume('ID')[1]
        return None

    def parse_return(self):
        if self.peek() is None:
            return None
        self.consume('KEYWORD', 'return')
        # print("consume return")
        return_statement = self.parse_term()
        # print("term parsed:", return_statement)
        self.consume('SYMBOL', ';')
        return Return(expr=return_statement, coord=None)


# evaluator ======================================================================


class Evaluator(NodeVisitor):
    precedence = (
        ('left', 'LOR'),
        ('left', 'LAND'),
        ('left', 'OR'),
        ('left', 'XOR'),
        ('left', 'AND'),
        ('left', 'EQ', 'NE'),
        ('left', 'GT', 'GE', 'LT', 'LE'),
        ('left', 'RSHIFT', 'LSHIFT'),
        ('left', 'PLUS', 'MINUS'),
        ('left', 'TIMES', 'DIVIDE', 'MOD')
    )

    def __init__(self):
        self.variables = {}
        self.functions = {}

    def visit_Assignment(self, node):
        # Handle assignment
        pass

    def visit_BinaryOp(self, node):
        # Handle binary operations
        pass

    def visit_Constant(self, node):
        # Handle constants
        pass

    def visit_ID(self, node):
        # Handle identifiers
        pass

    def visit_FuncCall(self, node):
        # Handle function calls
        pass


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

    tokens = Lexer(source_code).tokenize()
    print("Tokens:", tokens)
    ast = Parser(tokens).parse()
    ast.show()
    # evaluate(ast)


if __name__ == "__main__":
    main()
