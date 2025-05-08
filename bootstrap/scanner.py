import enum
import typing
import unittest

class TokenType(enum.Enum):
    EOF = enum.auto()
    INTEGER_LITERAL = enum.auto()
    KW_AND = enum.auto()
    KW_BREAK = enum.auto()
    KW_ELSE = enum.auto()
    KW_FN = enum.auto()
    KW_IF = enum.auto()
    KW_LET = enum.auto()
    KW_LOOP = enum.auto()
    KW_MATCH = enum.auto()
    KW_MOD = enum.auto()
    KW_NOT = enum.auto()
    KW_OR = enum.auto()
    KW_RETURN = enum.auto()
    KW_WHILE = enum.auto()
    OP_ARROW_DOUBLE = enum.auto()
    OP_ASTERISK = enum.auto()
    OP_EQUALS = enum.auto()
    OP_BRACE_OPEN = enum.auto()
    OP_BRACE_CLOSE = enum.auto()
    OP_BRACKET_OPEN = enum.auto()
    OP_BRACKET_CLOSE = enum.auto()
    OP_DOT = enum.auto()
    OP_GREATER_THAN = enum.auto()
    OP_GREATER_THAN_EQUALS = enum.auto()
    OP_LESS_THAN = enum.auto()
    OP_LESS_THAN_EQUALS = enum.auto()
    OP_MINUS = enum.auto()
    OP_NOT_EQUALS = enum.auto()
    OP_PAREN_OPEN = enum.auto()
    OP_PAREN_CLOSE = enum.auto()
    OP_PLUS = enum.auto()
    OP_SET = enum.auto()
    SYMBOL = enum.auto()

class Token(typing.NamedTuple):
    type: TokenType
    text: str
    line: int

class Scanner(object):
    WHITESPACE_CHARACTERS = set(' \t\r\n')
    KEYWORDS = (
        ('and', TokenType.KW_AND),
        ('break', TokenType.KW_BREAK),
        ('else', TokenType.KW_ELSE),
        ('fn', TokenType.KW_FN),
        ('if', TokenType.KW_IF),
        ('let', TokenType.KW_LET),
        ('loop', TokenType.KW_LOOP),
        ('match', TokenType.KW_MATCH),
        ('mod', TokenType.KW_MOD),
        ('not', TokenType.KW_NOT),
        ('or', TokenType.KW_OR),
        ('return', TokenType.KW_RETURN),
        ('while', TokenType.KW_WHILE),
    )
    ONE_CHAR_OPERATORS = {
        '*': TokenType.OP_ASTERISK,
        '{': TokenType.OP_BRACE_OPEN,
        '}': TokenType.OP_BRACE_CLOSE,
        '[': TokenType.OP_BRACKET_OPEN,
        ']': TokenType.OP_BRACKET_CLOSE,
        '.': TokenType.OP_DOT,
        '>': TokenType.OP_GREATER_THAN,
        '<': TokenType.OP_LESS_THAN,
        '-': TokenType.OP_MINUS,
        '(': TokenType.OP_PAREN_OPEN,
        ')': TokenType.OP_PAREN_CLOSE,
        '+': TokenType.OP_PLUS,
        '=': TokenType.OP_SET,
    }
    TWO_CHAR_OPERATORS = {
        '=>': TokenType.OP_ARROW_DOUBLE,
        '==': TokenType.OP_EQUALS,
        '>=': TokenType.OP_GREATER_THAN_EQUALS,
        '<=': TokenType.OP_LESS_THAN_EQUALS,
        '!=': TokenType.OP_NOT_EQUALS,
    }
    NUMBER_CHARACTERS = '0123456789'
    SYMBOL_BEGIN_CHARACTERS = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz_'
    SYMBOL_CONTINUE_CHARACTERS = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_'

    def __init__(self, source):
        self.source = source
        self.index = 0
        self.line = 1

    def scan(self):
        while self.index < len(self.source) and self.source[self.index] in self.WHITESPACE_CHARACTERS:
            if self.source[self.index] == '\n':
                self.line += 1

            self.index += 1

        if self.index == len(self.source):
            return Token(type=TokenType.EOF, text='', line=self.line)

        for kw,t in self.KEYWORDS:
            if self.source[self.index:].startswith(kw):
                self.index += len(kw)
                return Token(type=t, text=kw, line=self.line)

        if self.index + 1 < len(self.source):
            try_op = self.source[self.index:self.index + 2]

            if try_op in self.TWO_CHAR_OPERATORS:
                self.index += 2
                return Token(type=self.TWO_CHAR_OPERATORS[try_op], text=try_op, line=self.line)

        try_op = self.source[self.index]

        if try_op in self.ONE_CHAR_OPERATORS:
            self.index += 1
            return Token(type=self.ONE_CHAR_OPERATORS[try_op], text=try_op, line=self.line)

        if self.source[self.index] in self.SYMBOL_BEGIN_CHARACTERS:
            start = self.index
            self.index += 1

            while self.index < len(self.source) and self.source[self.index] in self.SYMBOL_CONTINUE_CHARACTERS:
                self.index += 1

            return Token(type=TokenType.SYMBOL, text=self.source[start:self.index], line=self.line)

        if self.source[self.index] in self.NUMBER_CHARACTERS:
            start = self.index
            self.index += 1

            while self.index < len(self.source) and self.source[self.index] in self.NUMBER_CHARACTERS:
                self.index += 1

            return Token(type=TokenType.INTEGER_LITERAL, text=self.source[start:self.index], line=self.line)

        raise Exception('Unexpected character {} at {}'.format(
            self.source[self.index],
            self.index,
        ))

class ScannerTests(unittest.TestCase):
    def test_scan_eof(self):
        source = ''
        scanner = Scanner(source)
        self.assertEqual(
            scanner.scan(),
            Token(type=TokenType.EOF, text='', line=1),
        )

    def test_consume_whitespace(self):
        source = ' \t\r'
        scanner = Scanner(source)
        self.assertEqual(
            scanner.scan(),
            Token(type=TokenType.EOF, text='', line=1),
        )

    def test_line_increment(self):
        source = '\n'
        scanner = Scanner(source)
        self.assertEqual(
            scanner.scan(),
            Token(type=TokenType.EOF, text='', line=2),
        )

    def test_scan_keywords(self):
        KEYWORDS = (
            ('and', TokenType.KW_AND),
            ('break', TokenType.KW_BREAK),
            ('else', TokenType.KW_ELSE),
            ('fn', TokenType.KW_FN),
            ('if', TokenType.KW_IF),
            ('let', TokenType.KW_LET),
            ('loop', TokenType.KW_LOOP),
            ('or', TokenType.KW_OR),
            ('match', TokenType.KW_MATCH),
            ('mod', TokenType.KW_MOD),
            ('return', TokenType.KW_RETURN),
            ('while', TokenType.KW_WHILE),
        )

        source = ' '.join(kw for kw,t in KEYWORDS)
        scanner = Scanner(source)

        for kw, t in KEYWORDS:
            self.assertEqual(
                scanner.scan(),
                Token(type=t, text=kw, line=1),
            )

    def test_scan_one_char_operators(self):
        ONE_CHAR_OPERATORS = (
            ('*', TokenType.OP_ASTERISK),
            ('{', TokenType.OP_BRACE_OPEN),
            ('}', TokenType.OP_BRACE_CLOSE),
            ('[', TokenType.OP_BRACKET_OPEN),
            (']', TokenType.OP_BRACKET_CLOSE),
            ('.', TokenType.OP_DOT),
            ('>', TokenType.OP_GREATER_THAN),
            ('<', TokenType.OP_LESS_THAN),
            ('-', TokenType.OP_MINUS),
            ('(', TokenType.OP_PAREN_OPEN),
            (')', TokenType.OP_PAREN_CLOSE),
            ('+', TokenType.OP_PLUS),
            ('=', TokenType.OP_SET),
        )

        source = ' '.join(op for op,t in ONE_CHAR_OPERATORS)
        scanner = Scanner(source)

        for op,t in ONE_CHAR_OPERATORS:
            self.assertEqual(
                scanner.scan(),
                Token(type=t, text=op, line=1),
            )


    def test_scan_two_char_operators(self):
        TWO_CHAR_OPERATORS = (
            ('=>', TokenType.OP_ARROW_DOUBLE),
            ('==', TokenType.OP_EQUALS),
            ('>=', TokenType.OP_GREATER_THAN_EQUALS),
            ('<=', TokenType.OP_LESS_THAN_EQUALS),
            ('!=', TokenType.OP_NOT_EQUALS),
        )

        source = ' '.join(op for op,t in TWO_CHAR_OPERATORS)
        scanner = Scanner(source)

        for op,t in TWO_CHAR_OPERATORS:
            self.assertEqual(
                scanner.scan(),
                Token(type=t, text=op, line=1),
            )

    def test_scan_symbols(self):
        source = 'foo'
        scanner = Scanner(source)
        self.assertEqual(
            scanner.scan(),
            Token(type=TokenType.SYMBOL, text='foo', line=1),
        )

    def test_scan_integers(self):
        source = '42'
        scanner = Scanner(source)
        self.assertEqual(
            scanner.scan(),
            Token(type=TokenType.INTEGER_LITERAL, text='42', line=1),
        )

if __name__ == '__main__':
    unittest.main()
