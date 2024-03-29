#include <stdio.h> // printf
#include <stdlib.h> // memoria
#include "ss.h"
#include "ss_helper.h"

int main(int argc, char** argv) {
  int compile = 0;
  struct Params prms = parse_argv(argc, argv, &compile);
  /*
    int y[] = {90, 92, 93, 97, 99, 103, 104, 108, 111, 112, 115};
    int n = sizeof(y)/sizeof(y[0]);
    int k = 4, t = 697;
  */
  double* Y = malloc(prms.y_length*sizeof(double));
  int i;
  Y[0] = prms.y[0];
  for (i = 1; i < prms.y_length; ++i) {
    Y[i] = Y[i-1] + prms.y[i];
  }
  struct SimpleVec sols;
  sols.len = 0; sols.cap = 1; sols.vec = malloc(sols.cap * sizeof(int));
  
  int qtd_folhas = 0;

  double tempo_decorrido = traverse_tree2(&sols, &prms, Y, &qtd_folhas, compile);

  printf("\nsolucoes: ");
  print_arr(sols.vec, 0, sols.len);

  printf("\nqtd_folhas: %d\n", qtd_folhas);
#ifdef DEBUGGING
  printf("sols->cap: %d\nsols->len: %d\n--\n", sols.cap, sols.len);
#endif
  printf("Tempo decorrido: %.5lfs\n", tempo_decorrido);
  
  free(sols.vec);
  free(Y);
  free(prms.y);
  return 0;
}
