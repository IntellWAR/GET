#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct thread_argument {
  double max_delta;
  double left_border;
  double right_border;
  double integrate_result;
};

double f(double x) {
  return sqrt(1-1*pow(x, 2))*2;
}

void *Integrate(void *arg) {
  struct thread_argument *info = arg;
  info->integrate_result = 0;
  const double max_delta = info->max_delta;
  double x = info->left_border;
  double right_border = info->right_border - max_delta; //для эффективности, чтобы не пересчитывать в while
  while (x <= right_border) {
    info->integrate_result += f(x) * max_delta;
    x += max_delta;
  }
  info->integrate_result += f(x) * (info->right_border - x); //остаток

  pthread_exit(NULL);
}

int main(int argc, char **argv) {
  if (argc != 3) {
    printf("Invalid number of arguments\n");
    return 1;
  }
  const double LEFT_BORDER = -1;
  const double RIGHT_BORDER = 1;
  long long N = strtoll(argv[1], 0, 10);
  long long k = strtoll(argv[2], 0, 10);
  perror("strtoll");

  printf("N = %lld\n", N);
  printf("k = %lld\n", k);

  pthread_t *thread = (pthread_t *) calloc(N, sizeof(pthread_t));
  struct thread_argument *args = calloc(N, sizeof(struct thread_argument));
  long long thread_segment_count = k / N;
  const double max_delta = (RIGHT_BORDER - LEFT_BORDER) / (double) k;
  for (int i = 0; i < N - 1; ++i) {
    args[i].max_delta = max_delta;
    args[i].left_border = LEFT_BORDER + (double) (i * thread_segment_count) * max_delta;
    args[i].right_border = LEFT_BORDER + (double) ((i + 1) * thread_segment_count) * max_delta;
  }
  args[N - 1].max_delta = max_delta;
  args[N - 1].left_border = LEFT_BORDER + (double) ((N - 1) * thread_segment_count) * max_delta;
  args[N - 1].right_border = RIGHT_BORDER;

  clock_t start = clock();

  for (int i = 0; i < N; ++i) {
    if (pthread_create(&thread[i], NULL, &Integrate, args + i)) {
      perror("Create thread\n");
    }
  }
  double sum = 0;
  for (int i = 0; i < N; ++i) {
    void *thread_status;
    int status = pthread_join(thread[i], (void **) &thread_status);
    if (status) {
      printf("thread error\n");
    }
    sum += args[i].integrate_result;
  }

  clock_t end = clock();
  double seconds = (double) (end - start) / CLOCKS_PER_SEC;

  printf("sum = %.016lf\n", sum);
  printf("time (sec) = %lf\n", seconds);
  free(thread);
  free(args);
}