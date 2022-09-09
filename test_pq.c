#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pq.h"

int ascending_int_cmp(const void * a, const void * b) 
{
  return ( *(int*)a - *(int*)b );
}

int main(int argc, char** argv) {
  struct pq* pq;
  int* first, * removed;
  int i, k, p;
  const int n = 16, m = 16;
  int vals[n + m], sorted[n + m];

  srand(0);

  pq = pq_create();
  printf("== Inserting some values into PQ\n");
  for (int i = 0; i < n; i++) {
    vals[i] = rand() % 64;
    pq_insert(pq, &vals[i], vals[i]);
  }

  memcpy(sorted, vals, n * sizeof(int));
  qsort(sorted, n, sizeof(int), ascending_int_cmp);

  k = 0;
  printf("\n== Removing some from PQ: first / removed / priority (expected)\n");
  while (k < n / 2) {
    p = pq_first_priority(pq);
    first = pq_first(pq);
    removed = pq_remove_first(pq);
    if (first && removed) {
      printf("  - %4d / %4d / %4d (%4d)\n", *first, *removed, p, sorted[k]);
    } else {
      printf("  - (NULL) / (NULL) / %4d (%4d)\n", p, sorted[k]);
    }
    k++;
  }

  printf("\n== Inserting more values into PQ\n");
  for (i = n; i < n + m; i++) {
    vals[i] = rand() % 64;
    pq_insert(pq, &vals[i], vals[i]);
  }

  memcpy(sorted + n, vals + n, m * sizeof(int));
  qsort(sorted + k, n - k + m, sizeof(int), ascending_int_cmp);

  printf("\n== Removing remaining from PQ: first / removed / priority (expected)\n");
  while (k < n + m && !pq_isempty(pq)) {
    p = pq_first_priority(pq);
    first = pq_first(pq);
    removed = pq_remove_first(pq);
    if (first && removed) {
      printf("  - %4d / %4d / %4d (%4d)\n", *first, *removed, p, sorted[k]);
    } else {
      printf("  - (NULL) / (NULL) / %4d (%4d)\n", p, sorted[k]);
    }
    k++;
  }

  printf("\n== Is PQ empty (expect 1)? %d\n", pq_isempty(pq));
  printf("== Did we see all values we expected (expect 1)? %d\n", k == m + n);

  pq_free(pq);
  return 0;
}

