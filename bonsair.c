
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define SCR_WIDTH 100
#define SCR_HEIGHT 50
#define SCR_SIZE (SCR_WIDTH * SCR_HEIGHT)

int bonsaimaker(char *, int, int, int, int, float);

int main() {
  srand(time(0));

  /* Output */
  char output[SCR_SIZE];
  for (int i = 0; i < SCR_SIZE; i++)
    output[i] = ' ';

  /* Clear screen */
  printf("[2J");

  bonsaimaker(output, 0, 1, SCR_WIDTH / 2 + SCR_WIDTH * (SCR_HEIGHT / 2), 0,
              20);

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

int bonsaimaker(char *out, int xdir, int ydir, int prev, int branch,
                float growth) {
  if (growth == 0) {
    out[prev] = (branch) ? '@' : '#';
    return 0;
  } else {

    // vary trunk
    if (!branch) {
      xdir = (rand() % (3)) - 1;
      ydir = (rand() % (2));
    }

    int r = (rand() % (10));
    // new branch
    if (!r) {
      xdir *= -1;
      ydir = 1;
      bonsaimaker(out, xdir, ydir, prev, 1, growth - 1);
    }

    // vary branch
    if (branch) {
      ydir = (rand() % (2));
    }

    // precheck spot
    if (out[prev - ydir * SCR_WIDTH + xdir] != ' ') {
      xdir *= -1;
    }

    if (ydir) {
      switch (xdir) {
      case -1:
        out[prev] = '\\';
        break;
      case 0:
        out[prev] = '|';
        break;
      case 1:
        out[prev] = '/';
        break;
      }
    } else {
      switch (xdir) {
      case -1:
        out[prev] = '_';
        break;
      case 0:
        out[prev] = '_';
        break;
      case 1:
        out[prev] = '_';
        break;
      }
    }
    return bonsaimaker(out, xdir, ydir, prev - ydir * SCR_WIDTH + xdir, branch,
                       growth - 1);
  }
}
