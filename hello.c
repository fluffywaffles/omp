#include <omp.h>
#include <stdio.h>

int main () {
  // Uses processor default (aka compiler default) max num threads
  // On a Ryzen 5 1600 (my machine), that's 12 (0...11)
  #pragma omp parallel
  {
    int ID = omp_get_thread_num();
    printf("Hello (%i)", ID);
    printf("OpenMP (%i)\n", ID);
  }

  return 0;
}
