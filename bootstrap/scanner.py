import enum
import typing
import unittest

class TokenType(enum.Enum):
    EOF = 1

class Token(typing.NamedTuple):
    type: TokenType
    text: str
    line: int

class Scanner(object):
    WHITESPACE_CHARACTERS = set(' \t\r\n')

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
            return Token(TokenType.EOF, text='', line=self.line)

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

if __name__ == '__main__':
    unittest.main()
