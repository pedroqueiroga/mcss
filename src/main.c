#include <stdio.h> // printf
#include <stdlib.h> // memoria, quicksort
#include <math.h> // PI, euler, sqrt, pow
#include "ss.h"
#include "ss_helper.h"

#include <time.h>

//$ ./ss -t 597 -y 90 92 93 108 99 103 104 97 111 115 112 -k 5

int main(int argc, char** argv) {
  struct Params prms = parse_argv(argc, argv);
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
  // n-k da a qtd de uns que eu devo ter,
  // logo, a qtd maxima de solucoes sera
  // as permutacoes com rep (n!/(k!*(n-k)!))
  int qtd_max_sols = perm_rep(prms.y_length, prms.k);
  printf("qtd_max_sols: %d\n", qtd_max_sols);
  int* sols = (int*) malloc(qtd_max_sols * sizeof(int));
  int qtd_folhas = 0;
  int sols_length = 0;
  printf("começando busca...\n");

  time_t beg, end;
  time(&beg);

  traverse_tree(sols, &sols_length, prms, prms.id, Y, &qtd_folhas);

  time(&end);
  double time = difftime(beg, end);

  printf("FIM\nsolucoes: ");
  print_arr(sols, 0, sols_length);
  printf("\nqtd_folhas: %d\n", qtd_folhas);

  printf("Tempo decorrido: %lfs\n", time);

  free(sols);
  free(Y);
  free(prms.y);
  return 0;
}
