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
  TOKEN_END
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

Token Scanner_scan(Scanner* self) {
  Token result;

  switch(*(self->source)) {
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

  printf("%s", source);

  free(source);

  return 0;
}
