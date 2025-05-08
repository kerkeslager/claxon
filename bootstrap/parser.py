import typing
import unittest

from scanner import Scanner, TokenType

class IntegerLiteralExpression(typing.NamedTuple):
    value: int

class FunctionDefinition(typing.NamedTuple):
    name: str

def parse_expression(scanner):
    token = scanner.scan()

    match token.type:
        case TokenType.INTEGER_LITERAL:
            return IntegerLiteralExpression(value=int(token.text))

        case _:
            raise Exception('Unexpected token')

class TestParseExpression(unittest.TestCase):
    def test_parses_integer_literal(self):
        source = '42'
        scanner = Scanner(source)
        self.assertEqual(
            parse_expression(scanner),
            IntegerLiteralExpression(value=42),
        )

def parse_definition(scanner):
    token = scanner.scan()

    match token.type:
        case TokenType.KW_FN:
            token = scanner.scan()

            if token.type != TokenType.SYMBOL:
                raise Exception('Expected symbol')

            name = token.text

            return FunctionDefinition(
                name=name,
            )

        case _:
            raise Exception('Unexpected token')


class TestParseDefinition(unittest.TestCase):
    def test_parses_function_no_arguments_no_body(self):
        source = 'fn foo() { }'
        scanner = Scanner(source)
        self.assertEqual(
            parse_definition(scanner),
            FunctionDefinition(name='foo'),
        )

if __name__ == '__main__':
    unittest.main()

