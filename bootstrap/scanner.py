import enum
import typing
import unittest

class TokenType(enum.Enum):
    EOF = 1
    KW_FN = 2
    KW_LET = 3

class Token(typing.NamedTuple):
    type: TokenType
    text: str
    line: int

class Scanner(object):
    WHITESPACE_CHARACTERS = set(' \t\r\n')
    KEYWORDS = (
        ('fn', TokenType.KW_FN),
        ('let', TokenType.KW_LET),
    )

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

        raise Exception()

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
            ('fn', TokenType.KW_FN),
            ('let', TokenType.KW_LET),
        )

        source = ' '.join(kw for kw,t in KEYWORDS)
        scanner = Scanner(source)

        for kw, t in KEYWORDS:
            self.assertEqual(
                scanner.scan(),
                Token(type=t, text=kw, line=1),
            )

if __name__ == '__main__':
    unittest.main()
