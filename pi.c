#include <omp.h>
#include <stdio.h>

static long num_steps = 100000;

double step;

/* A motivating example for OpenMP:
 *
 * We can calculate the exact value of π as:
 *   integral[ 4 / (1 + x^2)]
 *
 * We can approximate this with rectangular slices - Riemann sums.
 *
 * Available functions:
 *
 * int omp_get_num_threads() : number of threads you have
 * int omp_get_thread_num()  : index of current thread
 * double omp_get_wtime()    : number of seconds since a point in the past
 *
 * And use exactly one: #pragma omp parallel {...}
 */
int main () {
  int printed = 0;
  double sum = 0.0;
  step = 1.0 / (double) num_steps;

  #pragma omp parallel
  {
    int id                = omp_get_thread_num();
    int thread_count      = omp_get_num_threads();
    int extra_slices      = num_steps % thread_count;
    int thread_slice_size = num_steps / thread_count;

    int slice_start = id * thread_slice_size;
    int slice_end   = slice_start + thread_slice_size;
    if (id == (thread_count - 1)) {
      printf("%2d: adding %d extra iterations to reach %ld\n", id, extra_slices, num_steps);
      slice_end += extra_slices;
    }

    printf("%2d: summing slice: %10d ‒ %10d\n", id, slice_start, slice_end);

    int i;
    double x;
    for (i = slice_start; i < slice_end; i++) {
      x = (i + 0.5) * step;
      sum += 4.0 / (1.0 + x*x);
    }
  }

  double pi = step * sum;

  printf("\n\ncompleted summation.\n\n\n");
  printf("pi after %ld steps ≈\n%1.80f\n\n", num_steps, pi);
}
