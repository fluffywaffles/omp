#include "omp.h"
#include <stdio.h>

int main () {
  // Uses processor default (aka compiler default) max num threads
  #pragma omp parallel
  {
    int ID = omp_get_thread_num();
    printf("Hello (%i)", ID);
    printf("OpenMP (%i)\n", ID);
  }

  return 0;
}
