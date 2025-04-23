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

int main() {
  char* source = readSource();

  printf("%s", source);

  free(source);

  fclose(f);

  return 0;
}
