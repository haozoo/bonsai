
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main() {
  int k;
  float j;
  float A = 1;
  char b[1760];        // Output array
  memset(b, 32, 1760); // 32 == space

  /* Clears screen & brings cursor to the top */
  printf("[2J");

  for (;;) {
    for (j = 0; j < 6.28; j += 0.07) {
      int x = 40 + 1 * A * (16 * sin(j) * sin(j) * sin(j));
      int y =
          12 - 0.8 * A *
                   (13 * cos(j) - 5 * cos(2 * j) - 2 * cos(3 * j) - cos(4 * j));

      int o = x + 80 * y;

      if (22 > y && y > 0 && x > 0 && 80 > x) {
        b[o] = '@';
      }
    }

    /* Print's output[] to the screen*/
    printf("[H");
    for (k = 0; k < 1761; k++) {
      if (k % 80) {
        putchar(b[k]);
      } else {
        putchar(10);
      }
    }
    A -= 0.02;

    usleep(30000);
  }
  return 0;
}