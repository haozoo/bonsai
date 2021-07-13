
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define SCR_WIDTH 100
#define SCR_HEIGHT 50
#define SCR_SIZE (SCR_WIDTH * SCR_HEIGHT)

#define MAX_GROWTH 30

int bonsaimaker(char *, int, int, int, int, int);
int foliate(char *, int, int, int);
void printscr(char *);

int main() {
  srand(time(0));

  /* Output */
  char output[SCR_SIZE];
  for (int i = 0; i < SCR_SIZE; i++)
    output[i] = ' ';

  /* Clear screen */
  printf("[2J");
  bonsaimaker(output, 0, 1, SCR_WIDTH / 2 + SCR_WIDTH * (SCR_HEIGHT / 2), 0,
              MAX_GROWTH);

  /* Clear screen & print */
  printscr(output);
  return 0;
}

int bonsaimaker(char *out, int xdir, int ydir, int prev, int branch,
                int growth) {

  if (growth == 0) {
    return foliate(out, xdir, ydir, prev);
  } else {

    // vary trunk
    if (!branch) {
      xdir = (rand() % (3)) - 1;
      ydir = (rand() % (2));
    }

    int r = (rand() % (15));
    // new branch
    if (!r) {
      xdir *= -1;
      ydir = 1;
      bonsaimaker(out, xdir, ydir, prev, 1, growth - 1);
    }

    // vary branch
    if (branch) {
      if (!xdir)
        xdir = -1;
      ydir = (rand() % (2));
    }

    // precheck spot
    if (out[prev - ydir * SCR_WIDTH + xdir] != ' ') {

      xdir *= -1;
      ydir = 0;
    }

    if (out[prev] != '#') {
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
    }
    printscr(out);
    usleep(30000);
    return bonsaimaker(out, xdir, ydir, prev - ydir * SCR_WIDTH + xdir, branch,
                       growth - 1);
  }
}

int foliate(char *out, int xdir, int ydir, int prev) {
  prev -= 2 * xdir;
  if (xdir) {
    int k = 15;
    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < k; j++) {
        out[prev - i * SCR_WIDTH + xdir * j] = '#';
        printscr(out);
        usleep(30000);
      }
      k -= 4;
      prev += xdir;
    }
  } else {
    int k = 15;
    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < k / 2; j++) {
        out[prev - i * SCR_WIDTH - j] = '#';
        out[prev - i * SCR_WIDTH + j] = '#';
        printscr(out);
        usleep(30000);
      }
      k -= 7;
      k--;
      prev += xdir;
    }
  }
  return 0;
}

void printscr(char *out) {
  printf("[H");
  for (int k = 0; k < SCR_SIZE; k++) {
    if (k % SCR_WIDTH) {
      printf("%c", out[k]);
    } else {
      printf("\n");
    }
  }
  printf("\n");
}