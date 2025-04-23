#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

char* readSource() {
  FILE* f = fopen("main.clx", "r");

  if(f == NULL) {
    fprintf(stderr, "Unable to open main.clx\n");
    exit(1);
  }

  if(fseek(f, 0, SEEK_END) != 0) {
    fprintf(stderr, "Unable to read length of source file\n");
    exit(1);
  }

  size_t sourceLength = ftell(f);

  if(fseek(f, 0, SEEK_SET) != 0) {
    fprintf(stderr, "Error returning to beginning of file\n");
    exit(1);
  }

  char* source = malloc(sizeof(char) * sourceLength + 1);

  if(source == NULL) {
    fprintf(stderr, "Error allocating memory for source\n");
    exit(1);
  }

  if(fread(source, sizeof(char), sourceLength, f) != sourceLength) {
    fprintf(stderr, "Error: source length changed during read\n");
    exit(1);
  }

  if(ferror(f) != 0) {
    fprintf(stderr, "Error: unknown error reading file\n");
    exit(1);
  }

  fclose(f);

  source[sourceLength] = '\0';

  return source;
}

enum TokenType;
typedef enum TokenType TokenType;
enum TokenType {
  TOKEN_ARROW_SINGLE,
  TOKEN_ARROW_DOUBLE,
  TOKEN_AMPERSAND,
  TOKEN_BOOLEAN_LITERAL_FALSE,
  TOKEN_BOOLEAN_LITERAL_TRUE,
  TOKEN_BRACE_OPEN,
  TOKEN_BRACE_CLOSE,
  TOKEN_BREAK,
  TOKEN_COMMA,
  TOKEN_ELSE,
  TOKEN_ENUM,
  TOKEN_END,
  TOKEN_EQUALS,
  TOKEN_FN,
  TOKEN_FOR,
  TOKEN_GREATER_THAN,
  TOKEN_IF,
  TOKEN_INTEGER_LITERAL,
  TOKEN_LET,
  TOKEN_LOOP,
  TOKEN_MATCH,
  TOKEN_MINUS,
  TOKEN_NIL_LITERAL,
  TOKEN_PAREN_OPEN,
  TOKEN_PAREN_CLOSE,
  TOKEN_PERIOD,
  TOKEN_SEMICOLON,
  TOKEN_STAR,
  TOKEN_STRING_LITERAL_SINGLE,
  TOKEN_STRUCT,
  TOKEN_SYMBOL
};

struct Token;
typedef struct Token Token;
struct Token {
  TokenType type;
  char* text;
  size_t length;
};

void Token_init(Token* self, TokenType type, char* text, size_t length) {
  self->type = type;
  self->text = text;
  self->length = length;
}

void Token_println(Token token) {
  switch(token.type) {
    case TOKEN_AMPERSAND:
      printf("AMPERSAND\n");
      break;

    case TOKEN_ARROW_SINGLE:
      printf("ARROW_SINGLE\n");
      break;

    case TOKEN_ARROW_DOUBLE:
      printf("ARROW_DOUBLE\n");
      break;

    case TOKEN_BOOLEAN_LITERAL_FALSE:
      printf("BOOLEAN_LITERAL_FALSE\n");
      break;

    case TOKEN_BOOLEAN_LITERAL_TRUE:
      printf("BOOLEAN_LITERAL_TRUE\n");
      break;

    case TOKEN_BRACE_OPEN:
      printf("BRACE_OPEN\n");
      break;

    case TOKEN_BRACE_CLOSE:
      printf("BRACE_CLOSE\n");
      break;

    case TOKEN_BREAK:
      printf("BREAK\n");
      break;

    case TOKEN_COMMA:
      printf("COMMA\n");
      break;

    case TOKEN_END:
      printf("END\n");
      break;

    case TOKEN_ELSE:
      printf("ELSE\n");
      break;

    case TOKEN_ENUM:
      printf("ENUM\n");
      break;

    case TOKEN_EQUALS:
      printf("EQUALS\n");
      break;

    case TOKEN_FN:
      printf("FN\n");
      break;

    case TOKEN_FOR:
      printf("FOR\n");
      break;

    case TOKEN_GREATER_THAN:
      printf("GREATER_THAN\n");
      break;

    case TOKEN_IF:
      printf("IF\n");
      break;

    case TOKEN_INTEGER_LITERAL:
      printf("INTEGER_LITERAL\n");
      break;

    case TOKEN_LET:
      printf("LET\n");
      break;

    case TOKEN_LOOP:
      printf("LOOP\n");
      break;

    case TOKEN_MATCH:
      printf("MATCH\n");
      break;

    case TOKEN_MINUS:
      printf("MINUS\n");
      break;

    case TOKEN_NIL_LITERAL:
      printf("NIL_LITERAL\n");
      break;

    case TOKEN_PAREN_OPEN:
      printf("PAREN_OPEN\n");
      break;

    case TOKEN_PAREN_CLOSE:
      printf("PAREN_CLOSE\n");
      break;

    case TOKEN_PERIOD:
      printf("PERIOD\n");
      break;

    case TOKEN_SEMICOLON:
      printf("SEMICOLON\n");
      break;

    case TOKEN_STAR:
      printf("SEMICOLON\n");
      break;

    case TOKEN_STRING_LITERAL_SINGLE:
      printf("STRING_LITERAL_SINGLE %.*s\n", (int)token.length, token.text);
      break;

    case TOKEN_STRUCT:
      printf("STRUCT\n");
      break;

    case TOKEN_SYMBOL:
      printf("SYMBOL %.*s\n", (int)token.length, token.text);
      break;
  }
}

struct Scanner;
typedef struct Scanner Scanner;
struct Scanner {
  char* source;
  size_t line;
  bool panic;
};

void Scanner_init(Scanner* self, char* source) {
  self->source = source;
  self->line = 1;
  self->panic = false;
}

bool isSymbolChar(char ch) {
  return ch == '_' || ('A' <= ch && ch <= 'Z') || ('a' <= ch && ch <= 'z');
}

Token Scanner_symbol(Scanner* self, char* start) {
  while(isSymbolChar(*(self->source))) self->source++;
  Token result;
  Token_init(&result, TOKEN_SYMBOL, start, self->source - start);
  return result;
}

Token Scanner_keyword(Scanner* self, TokenType type, char* start, const char* suffix) {
  for(;;) {
    if(*suffix == '\0') {
      if(isSymbolChar(*(self->source))) {
        return Scanner_symbol(self, start);
      }

      Token result;
      Token_init(&result, type, start, self->source - start);
      return result;
    }

    if(*suffix == *(self->source)) {
      suffix++;
      self->source++;
    } else {
      return Scanner_symbol(self, start);
    }
  }
}

Token Scanner_singleCharToken(Scanner* self, TokenType type) {
  Token result;
  Token_init(&result, type, self->source++, 1);
  return result;
}

Token Scanner_doubleCharToken(Scanner* self, TokenType type) {
  Token result;
  Token_init(&result, type, self->source, 2);
  self->source += 2;
  return result;
}

void Scanner_consumeWhitespace(Scanner* self) {
  for(;;) {
    switch(*(self->source)) {
      case ' ':
        self->source++;
        break;

      case '\n':
        self->source++;
        break;

      default:
        return;
    }
  }
}

Token Scanner_scan(Scanner* self) {
  Scanner_consumeWhitespace(self);

  Token result;

  switch(*(self->source)) {
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
      {
        char* start = self->source;
        self->source++;

        while('0' <= *(self->source) && *(self->source) <= '9') self->source++;

        Token_init(&result, TOKEN_INTEGER_LITERAL, start, self->source - start);
        return result;
      }

    case 'b':
      {
        char* start = self->source;
        self->source++;
        return Scanner_keyword(self, TOKEN_BREAK, start, "reak");
      }

    case 'e':
      {
        char* start = self->source;
        self->source++;


        switch(*(self->source)) {
          case 'l':
            self->source++;
            return Scanner_keyword(self, TOKEN_ELSE, start, "se");

          case 'n':
            self->source++;
            return Scanner_keyword(self, TOKEN_ENUM, start, "um");

          default:
            return Scanner_symbol(self, start);
        }
      }

    case 'f':
      {
        char* start = self->source;
        self->source++;

        switch(*(self->source)) {
          case 'a':
            self->source++;
            return Scanner_keyword(self, TOKEN_BOOLEAN_LITERAL_FALSE, start, "lse");

          case 'o':
            self->source++;
            return Scanner_keyword(self, TOKEN_FOR, start, "r");

          case 'n':
            self->source++;
            return Scanner_keyword(self, TOKEN_FN, start, "");

          default:
            return Scanner_symbol(self, start);
        }
      }

    case 'i':
      {
        char* start = self->source;
        self->source++;
        return Scanner_keyword(self, TOKEN_IF, start, "f");
      }

    case 'l':
      {
        char* start = self->source;
        self->source++;

        switch(*(self->source)) {
          case 'e':
            self->source++;
            return Scanner_keyword(self, TOKEN_LET, start, "t");

          case 'o':
            self->source++;
            return Scanner_keyword(self, TOKEN_LOOP, start, "op");

          default:
            return Scanner_symbol(self, start);
        }
      }

    case 'm':
      {
        char* start = self->source;
        self->source++;
        return Scanner_keyword(self, TOKEN_MATCH, start, "atch");
      }

    case 'n':
      {
        char* start = self->source;
        self->source++;
        return Scanner_keyword(self, TOKEN_NIL_LITERAL, start, "il");
      }

    case 's':
      {
        char* start = self->source;
        self->source++;
        return Scanner_keyword(self, TOKEN_STRUCT, start, "truct");
      }

    case 't':
      {
        char* start = self->source;
        self->source++;
        return Scanner_keyword(self, TOKEN_BOOLEAN_LITERAL_TRUE, start, "rue");
      }

    case '_':
    case 'A':
    case 'B':
    case 'C':
    case 'D':
    case 'E':
    case 'F':
    case 'G':
    case 'H':
    case 'I':
    case 'J':
    case 'K':
    case 'L':
    case 'M':
    case 'N':
    case 'O':
    case 'P':
    case 'Q':
    case 'R':
    case 'S':
    case 'T':
    case 'U':
    case 'V':
    case 'W':
    case 'X':
    case 'Y':
    case 'Z':
    case 'a':
    case 'c':
    case 'd':
    case 'o':
    case 'p':
    case 'r':
      return Scanner_symbol(self, self->source++);

    case '\'':
      {
        char* start = self->source;
        for(;;) {
          self->source++;
          switch(*(self->source)) {
            case '\'':
              self->source++;
              Token_init(&result, TOKEN_STRING_LITERAL_SINGLE, start, self->source - start);
              return result;

            case '\\':
              self->source++;
              switch(*(self->source)) {
                case '0':
                  break;

                default:
                  assert(false);
              }
              break;

            case '\0':
              assert(false);

            default:
              break;
          }
        }
      } break;

    case '&':
      return Scanner_singleCharToken(self, TOKEN_AMPERSAND);

    case '{':
      return Scanner_singleCharToken(self, TOKEN_BRACE_OPEN);

    case '}':
      return Scanner_singleCharToken(self, TOKEN_BRACE_CLOSE);

    case ',':
      return Scanner_singleCharToken(self, TOKEN_COMMA);

    case '(':
      return Scanner_singleCharToken(self, TOKEN_PAREN_OPEN);

    case ')':
      return Scanner_singleCharToken(self, TOKEN_PAREN_CLOSE);

    case '.':
      return Scanner_singleCharToken(self, TOKEN_PERIOD);

    case ';':
      return Scanner_singleCharToken(self, TOKEN_SEMICOLON);

    case '*':
      return Scanner_singleCharToken(self, TOKEN_STAR);

    case '-':
      switch(self->source[1]) {
        case '>':
          return Scanner_doubleCharToken(self, TOKEN_ARROW_SINGLE);

        default:
          return Scanner_singleCharToken(self, TOKEN_MINUS);
      }

    case '=':
      switch(self->source[1]) {
        case '>':
          return Scanner_doubleCharToken(self, TOKEN_ARROW_DOUBLE);

        default:
          return Scanner_singleCharToken(self, TOKEN_EQUALS);
      }

    case '\0':
      Token_init(&result, TOKEN_END, self->source, 0);
      return result;

    default:
      assert(false);
  }
}

int main() {
  char* source = readSource();

  Scanner scanner;
  Scanner_init(&scanner, source);

  for(;;) {
    Token token = Scanner_scan(&scanner);
    Token_println(token);
    if(token.type == TOKEN_END) break;
  }

  free(source);

  return 0;
}
