#include "omp.h"
#include <stdio.h>

int main () {
  #pragma omp parallel
  {
    int ID = omp_get_thread_num();
    printf("Hello (%i)", ID);
    printf("OpenMP (%i)\n", ID);
  }

  return 0;
}
