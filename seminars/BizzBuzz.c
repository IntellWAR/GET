#include <stdio.h>
#include <stdlib.h>

int serror(char *symb)
{
  char *endOFline;
  char endSymb;
  int Number;
  Number = strtol(symb, &endOFline, 10);
  endSymb = *endOFline;
  if (endSymb != '\0') {
    printf("Incorrect input\n");
    exit(1);
  }
  return Number;
}

int main(int argc, char **argv)
{
  int i, N, Number;
  N = argc;
  for (i = 1; i < N; i++) {
    Number = serror(argv[i]);
    if (Number % 15 == 0 && Number != 0) {
      printf("BizzBuzz ");
    } else {
      if (Number % 5 == 0 && Number != 0) {
        printf("Buzz ");
      } else {
        if (Number % 3 == 0 && Number != 0) {
          printf("Bizz ");
        } else {
          printf("%d ", Number);
        }
      }
    }
  }
  printf("\n");
  return 0;
}