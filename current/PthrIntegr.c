#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  int from;
  int to;
  double dsum, dx;
  double* x;
} a;

void* Integrate(void* args) {
  a* arg = (a*)args;
  double sum = 0, x;
  int n = arg->to - arg->from;
  double f[n];
  for (int i; i < n; i++) {
    x = arg->x[i + arg->from];
    f[i] = arg->dx * x / sin(x);
    sum += f[i];
  }
  arg->dsum = sum;
  pthread_exit(arg);
}

int main(int argc, char** argv) {
  if (argc != 3) {
  	printf("Invalid number of arguments\n");
  	return 1; 
  }

  int N;
  long int k;
  double sum, dx;
  N = strtol(argv[1], 0, 10);
  k = strtoll(argv[2], 0, 10);
  perror ("strtoll");
  pthread_t thread[N];
  printf("%d\n", N);
  printf("%ld\n", k);
  double* x;
  x = (double*) calloc (k, sizeof (double));
  a args[N];
  printf("%ld\n", k);
  a retval;
  dx = M_PI / (4 * k);
  printf("%ld\n", k);

  for (int i = 0; i < k; i++) {
    x[i] = ((M_PI / 8) * (2 * i + 1));
  }

  for (int i = 0; i < N; i++) {
    args[i].dx = dx;
    args[i].x = x;
    if (i < (N - 1)) {
      args[i].from = i * (k / N);
      args[i].to = (i + 1) * (k / N);
    } else {
      args[i].from = args[i - 1].to + 1;
      args[i].to = k - 1;
    }
    if (pthread_create(&thread[i], NULL, Integrate, &args[i]) != 0) {
      perror("Create thread");
    }
  }
  for (int i; i < N; i++) {
    pthread_join(thread[i], (void**)&retval);
    sum += retval.dsum;
  }
  printf ("sum = %f\n", sum);
  free (x);
}