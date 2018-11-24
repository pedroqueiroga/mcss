#include <stdio.h> // printf
#include <stdlib.h> // malloc
#include "ss_helper.h"

char* pertence_string[] = {"SOL", "SIM", "NAO"};

// se for solução, retorna 0. se d pertencer a I, retorna 1, cc 2.
int get_all(int id, int* y, int y_length, int k, int t, int* Y) {
  int cont;// meu contador
  
  int* V = (int*) malloc(y_length * sizeof(int));
  int V_index = y_length;
  while (id > 1) {
    --V_index;
    //printf("V_index: %d\n", V_index);
    if (id & 1) { // impar
      V[V_index] = 1;
      id = (id-1)/2;
    } else { // par
      V[V_index] = 0;
      id = id/2;
    }
  }
  if (V_index < 0) {
    // id invalido
    free(V);
    return 2;
  }
  int p = y_length-V_index; // Length[V]
  int m = 0; // Total[V]
  for (cont = V_index; cont < V_index + p; ++cont) { 
    m += V[cont];
  }
  int o = y_length - k - m; // # de uns que faltam
  int a = 0; // soma de uns
  for (cont = V_index; cont < V_index + p; ++cont) {
    // cont - V_index vai encaixar o primeiro de y com o primeiro de V
    a += V[cont]*y[cont-V_index];
  }
  int d = t-a; // desejado
  int int1[2], int2[2];
  int1[0] = p+1; int1[1] = p+o;
  int2[0] = y_length-o+1; int2[1] = y_length;

  int i = int1[0], j = int1[1];
  int Zij1, Zij2;
  if (((p-m) > k) || (m>(y_length-k))) { //j > y_length) {
    // p+o eh mais do que tenho
    free(V);
    return 2;
  }
  if (i > 1) {
    Zij1 = Y[j-1] - Y[i-2];
  } else {
    Zij1 = Y[j-1];
  }
  i = int2[0]; j = int2[1];
  if (i > 1) {
    Zij2 = Y[j-1] - Y[i-2];
  } else {
    Zij2 = Y[j-1];
  }

  printf("Y: ");
  print_arr(Y, 0, y_length);

  printf("\nV: ");
  print_arr(V, V_index, V_index + p);

  printf("\nm: %d\no: %d\na: %d\np: %d\nint1: ", m, o, a, p);
  print_arr(int1, 0, 2);
  printf("\nint2: ");
  print_arr(int2, 0, 2);
  printf("\nd: %d\nZij1: %d\nZij2: %d\n", d, Zij1, Zij2);

  free(V);
  if (Zij1 == d && d == Zij2 && d == 0) {
    return 0;
  } else if (Zij1 <= d && d <= Zij2) {
    return 1;
  } else {
    return 2;
  }
}

void traverse_tree(int* sols, int* sols_length, struct Params prms, int id, int* Y, int* qtd_folhas) {
  printf("id: %d\n", id);
  if (id == 0) {
    printf("wtf");
    exit(1);
  }
  int pertence = get_all(id, prms.y, prms.y_length, prms.k, prms.t, Y);
  printf("**************\npertence: %s\n**************\n", pertence_string[pertence]);
  if (pertence == 0) {
    sols[(*sols_length)++] = id;
    (*qtd_folhas)++;
  } else if (pertence == 1) {
    traverse_tree(sols, sols_length, prms, id*2, Y, qtd_folhas);
    traverse_tree(sols, sols_length, prms, id*2 + 1, Y, qtd_folhas);
  } else {
    (*qtd_folhas)++;
  }
}
