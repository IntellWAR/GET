#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

double dx, k;
int from, to;

void* Integrate(double* x) {
  double sum = 0;
  int n = to - from;
  double f[n];
  for (int i=0; i < n; i++) {
    f[i] = dx * x[i] / sin(x[i]);
    sum += f[i];
  }
  pthread_exit(&sum);
}

int main(int argc, char** argv) {
  if (argc != 3) {
  	printf("Invalid number of arguments\n");
  	return 1; 
  }

  int N, from;
  long int k;
  double sum = 0;
  N = strtol(argv[1], 0, 10);
  k = strtoll(argv[2], 0, 10);
  perror ("strtoll");
  pthread_t* thread;
  printf("N = %d\n", N);
  printf("k = %ld\n", k);
  double* x;
  thread = (pthread_t*) calloc (N, sizeof (pthread_t));
  x = calloc (k, sizeof (double));
  dx = M_PI / (4 * k);

  for (int i = 0; i < k; i++) {
    x[i] = ((M_PI / 8) * (2 * i + 1));
  }

  for (int i = 0; i < N; i++) {
    if (i < (N - 1)) {
      from = i * (k / N);
      to = (i + 1) * (k / N);
    } else {
      from = to + 1;
      to = k - 1;
    }
    if (pthread_create(&thread[i], NULL, &Integrate, x) != 0) {
      perror("Create thread");
    }
  }
  for (int i; i < N; i++) {
    pthread_join(thread[i], (void**)retval);
    sum += retval.dsum;
  }
  printf ("sum = %f\n", sum);
  free (x);
}