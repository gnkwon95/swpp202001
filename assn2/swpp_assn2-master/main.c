#include <stdio.h>

int count_even(int *arr, int N);

int main(int argc, char **argv) {
  FILE *f = fopen(argv[1], "r");
  int N;
  int arr[21];
  fscanf(f, "%d", &N);

  for (int i = 0; i < N; i++)
    fscanf(f, "%d", &arr[i]);
  arr[N] = 0;
  printf("%d\n", count_even(arr, N));

  fclose(f);

  return 0;
}
