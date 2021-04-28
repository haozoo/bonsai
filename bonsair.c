
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define SCR_WIDTH 100
#define SCR_HEIGHT 50
#define SCR_SIZE (SCR_WIDTH * SCR_HEIGHT)

int bonsaimaker(char *, char, int, float);

int main() {
  srand(time(0));

  /* Output */
  char output[SCR_SIZE];
  for (int i = 0; i < SCR_SIZE; i++)
    output[i] = ' ';

  /* Clear screen */
  printf("[2J");

  bonsaimaker(output, 1, SCR_WIDTH / 2 + SCR_WIDTH * (SCR_HEIGHT / 2), 20);

  // for (k = 0; k < SCR_SIZE; k++)
  //   printf("%c", output[k]);

  /* Clear screen & print */
  printf("[H");
  for (int k = 0; k < SCR_SIZE; k++) {
    if (k % SCR_WIDTH) {
      putchar(output[k]);
    } else {
      putchar(10);
    }
  }
  putchar(10);

  return 0;
}

int bonsaimaker(char *out, char dir, int prev, float growth) {
  if (growth == 0) {
    out[prev] = '@';
    return 0;
  } else {

    int dir = (rand() % (5));
    int r = (rand() % (10));

    // new branch
    if (!r) {
      int ndir = (rand() % (5));
      bonsaimaker(out, ndir, prev, growth - 1);
    }

    switch (dir) {
    case 0:
      out[prev] = '_';
      return bonsaimaker(out, dir, prev - 1, growth - 1);
      break;
    case 1:
      out[prev] = '\\';
      return bonsaimaker(out, dir, prev - SCR_WIDTH - 1, growth - 1);
      break;
    case 2:
      out[prev] = '|';
      return bonsaimaker(out, dir, prev - SCR_WIDTH, growth - 1);
      break;
    case 3:
      out[prev] = '/';
      return bonsaimaker(out, dir, prev - SCR_WIDTH + 1, growth - 1);
      break;
    case 4:
      out[prev] = '_';
      return bonsaimaker(out, dir, prev + 1, growth - 1);
      break;
    }
  }
  return 0;
}
