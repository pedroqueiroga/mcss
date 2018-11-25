#include <stdio.h> // printf
#include <stdlib.h> // memoria, quicksort
#include <math.h> // PI, euler, sqrt, pow
#include "ss.h"
#include "ss_helper.h"

#include <time.h>

//$ ./ss -t 597 -y 90 92 93 108 99 103 104 97 111 115 112 -k 5

int main(int argc, char** argv) {
  int rec = 0;
  struct Params prms = parse_argv(argc, argv, &rec);
  /*
    int y[] = {90, 92, 93, 97, 99, 103, 104, 108, 111, 112, 115};
    int n = sizeof(y)/sizeof(y[0]);
    int k = 4, t = 697;
  */
  int* Y = (int*) malloc(prms.y_length*sizeof(int));
  int i;
  Y[0] = prms.y[0];
  for (i = 1; i < prms.y_length; ++i) {
    Y[i] = Y[i-1] + prms.y[i];
  }
  struct SimpleVec sols;
  sols.len = 0; sols.cap = 1; sols.vec = malloc(sols.cap * sizeof(int));
  
  int qtd_folhas = 0;
  time_t beg, end;

  if (rec) {
    printf("começando busca recursiva...\n");

    time(&beg);

    traverse_tree(&sols, &prms, prms.id, Y, &qtd_folhas);

    time(&end);
    printf("FIM rec\nsolucoes: ");
  } else {
    printf("começando busca iterativa...\n");
    time(&beg);
    traverse_tree2(&sols, &prms, Y, &qtd_folhas);
    time(&end);
    printf("FIM iter\nsolucoes: ");
  }    
  double timer = difftime(beg, end);
  print_arr(sols.vec, 0, sols.len);
  printf("\nqtd_folhas: %d\n", qtd_folhas);
#ifdef DEBUGGING
  printf("sols->cap: %d\nsols->len: %d\n--\n", sols.cap, sols.len);
#endif
  printf("Tempo decorrido: %lfs\n", timer);

  free(sols.vec);
  free(Y);
  free(prms.y);
  return 0;
}
