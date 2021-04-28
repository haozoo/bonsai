
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define SCR_WIDTH 100
#define SCR_HEIGHT 50
#define SCR_SIZE (SCR_WIDTH * SCR_HEIGHT)
#define MAX_ITERS 100

int main() {
  /* Generate seed using time */
  srand(time(0));

  /* Array of possible positions for new  pieces */
  int pos[SCR_SIZE];
  for (int i = 0; i < SCR_SIZE; i++)
    pos[i] = 0;
  pos[0] = SCR_WIDTH / 2 + SCR_WIDTH * (SCR_HEIGHT / 2);
  int posLen = 1;

  /* Output */
  char output[SCR_SIZE];
  for (int i = 0; i < SCR_SIZE; i++)
    output[i] = ' ';

  /* Clear screen */
  printf("[2J");

  float iters = 0;
  while (iters < MAX_ITERS) {
    /* 1. Generate random number from 0 to 0len */
    int r = (rand() % (posLen + 1));

    /* 2. Use random number to pick from pos and use as index */
    output[pos[r]] = '|';

    /* 3. Delete selected random pos from array
     *  - Set pos[r]] to the last value in pos
     *  - Set last value in pos to 0
     *  - Deincrement by array length by 1
     */
    int tmp = pos[r];
    pos[r] = pos[posLen - 1];
    pos[posLen - 1] = 0;
    posLen--;

    int L = 1, M = 1, R = 1;
    int LL = 1, RR = 1;
    for (int j = 0; j < posLen; j++) {
      if (pos[j] == tmp - SCR_WIDTH - 1)
        L = 0;
      if (pos[j] == tmp - SCR_WIDTH)
        M = 0;
      if (pos[j] == tmp - SCR_WIDTH + 1)
        R = 0;

      if (pos[j] == tmp - SCR_WIDTH - 2)
        LL = 0;
      if (pos[j] == tmp - SCR_WIDTH + 2)
        RR = 0;
    }

    if (L) {
      pos[posLen++] = tmp - SCR_WIDTH - 1;
      pos[posLen++] = tmp - SCR_WIDTH - 1;
      pos[posLen++] = tmp - SCR_WIDTH - 1;
    }
    if (M)
      pos[posLen++] = tmp;
    if (R) {
      pos[posLen++] = tmp - SCR_WIDTH + 1;
      pos[posLen++] = tmp - SCR_WIDTH + 1;
      pos[posLen++] = tmp - SCR_WIDTH + 1;
    }
    if (LL)
      pos[posLen++] = tmp - SCR_WIDTH - 2;
    if (RR)
      pos[posLen++] = tmp - SCR_WIDTH + 2;

    /* Print output[] to the screen*/
    printf("[H");
    int k;
    for (k = 0; k < SCR_SIZE; k++) {
      if (k % SCR_WIDTH) {
        putchar(output[k]);
      } else {
        putchar(10);
      }
    }
    iters += 1;
    usleep(30000);
  }
  return 0;
}