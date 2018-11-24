#include <math.h> //pi, euler, round, pow, sqrt
#include <stdio.h> // printf
#include <stdlib.h> // atoi, malloc, qsort
#include "ss_helper.h"

void print_arr(int* arr, int start, int arr_length) {
  printf("(");
  int i;
  for (i = start; i < arr_length; ++i) {
    printf("%d", arr[i]);
    if (i != arr_length-1) {
      printf(", ");
    }
  }
  printf(")");  
}

struct Params parse_argv(int argc, char** argv) {
  int i, j, y_length, okk=0,okt=0,oky=0,oki=0;
  struct Params prms;
  for (i = 0; i < argc; ++i) {
    if (argv[i][0] == '-') {
      switch (argv[i][1]) {
      case 'i':
	prms.id = atoi(argv[++i]);
	oki=1;
	break;
      case 'k':
        prms.k = atoi(argv[++i]);
        okk=1;
        break;
      case 't':
        prms.t = atoi(argv[++i]);
        okt=1;
        break;
      case 'y':
        y_length = 0;
        for(j = i+1; j < argc && argv[j][0] != '-'; ++j) {
          y_length++;
        }
        prms.y_length = y_length;
        prms.y = (int*) malloc(y_length*sizeof(int));
        for (j = i+1; j < argc && argv[j][0] != '-'; ++j) {
          prms.y[j-(i+1)] = atoi(argv[j]);
        }
        i = j-1;
        quicksort(prms.y, prms.y_length);
        oky=1;
        break;
      default:
        printf("argumentos:\n-k qtd de zeros\n-t objetivo\n-y vetor separado por espaços\n");
        exit(1);
      }
    }
  }
  if (!(okk && okt && oky)) {
    printf("argumentos:\n-k qtd de zeros\n-t objetivo\n-y vetor separado por espaços\n[-i id]\n");
    exit(1);
  }
  if (!oki) {
    prms.id = 1;
  }
  printf("*** parametros lidos ***\nid: %d\nk: %d\nt: %d\ny: ", prms.id, prms.k, prms.t);
  print_arr(prms.y, 0, prms.y_length);
  printf("\n---\n");
  return prms;
}

int mycompare(const void* a, const void* b) {
  return ((* (int*) a) - (* (int*) b));
}

void quicksort(int* v, int v_length) {
  qsort(v, v_length, sizeof(int), mycompare);
}

int stirling_approx(int n) {
  return (int) round(sqrt(2*M_PI*n)*pow(((float)n)/M_E,n));
}

int perm_rep(int n, int k) {
  return stirling_approx(n)/(stirling_approx(n-k)*stirling_approx(k));
}
