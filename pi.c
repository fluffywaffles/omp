#include <omp.h>
#include <stdio.h>

static long num_steps = 1000000000;

/**
 * A motivating example for OpenMP:
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
  double sum  = 0.0;
  double step = 1.0 / (double) num_steps;
  int max_threads = omp_get_max_threads();
  int actual_slice_count = -1;

  printf("info: maximum usable threads: %d\n\n", max_threads);
  double slices[max_threads];
  for (int i = 0; i < max_threads; i++) slices[i] = 0.0;

  #pragma omp parallel
  {
    int id                = omp_get_thread_num();
    int thread_count      = omp_get_num_threads();
    int extra_slices      = num_steps % thread_count;
    int thread_slice_size = num_steps / thread_count;

    if (actual_slice_count == -1) actual_slice_count = thread_count;

    int slice_start = id * thread_slice_size;
    int slice_end   = slice_start + thread_slice_size;
    if (id == (thread_count - 1)) {
      printf(
        "thread(%2d): add %d iterations to reach %ld\n",
        id, extra_slices, num_steps
      );
      slice_end += extra_slices;
    }

    printf(
      "thread(%2d): summing slice: %10d ‒ %10d\n",
      id, slice_start, slice_end
    );

    int i; double x;
    for (i = slice_start; i < slice_end; i++) {
      x = (i + 0.5) * step;
      slices[id] += 4.0 / (1.0 + x*x);
    }
  }

  for (int i = 0; i < actual_slice_count; i++) sum += slices[i];

  double pi = step * sum;

  printf("\ncompleted summation.\n\n");
  printf("pi after %ld steps ≈\n%1.50f\n\n", num_steps, pi);
}
