
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

int main() {
  int k, olen = 1;
  float A = 0;
  char output[1760]; // Output array

  srand(time(0));

  int pos[1760];
  for (int i = 0; i < 1760; i++)
    pos[i] = 0;
  pos[0] = 1000;

  memset(output, 32, 1760); // 32 == space

  /* Clears screen & brings cursor to the top */
  printf("[2J");

  while (A < 100) {
    int r = (rand() % (olen + 1));

    output[pos[r]] = '|';

    // swap selected to end
    int temp = pos[r];
    pos[r] = pos[olen - 1];
    pos[olen - 1] = 0;
    olen--;

    int L = 1, M = 1, R = 1;
    int LL = 1, RR = 1;
    for (int j = 0; j < olen; j++) {
      if (pos[j] == temp - 80 - 1)
        L = 0;
      if (pos[j] == temp - 80)
        M = 0;
      if (pos[j] == temp - 80 + 1)
        R = 0;

      if (pos[j] == temp - 80 - 2)
        LL = 0;
      if (pos[j] == temp - 80 + 2)
        RR = 0;
    }

    if (L)
      pos[olen++] = temp - 80 - 1;
    if (M)
      pos[olen++] = temp;
    if (R)
      pos[olen++] = temp - 80 + 1;

    if (LL)
      pos[olen++] = temp - 80 - 2;
    if (RR)
      pos[olen++] = temp - 80 + 2;

    /* Print's output[] to the screen*/
    printf("[H");
    for (k = 0; k < 1761; k++) {
      if (k % 80) {
        putchar(output[k]);
      } else {
        putchar(10);
      }
    }
    A += 1;

    usleep(30000);
  }
  return 0;
}