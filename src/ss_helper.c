#include <math.h> //pi, euler, round, pow, sqrt
#include <stdio.h> // printf
#include <stdlib.h> // atoi, malloc, qsort
#include <float.h> // DBL_MAX
#include "ss_helper.h"
#include <errno.h>
#include <string.h> // strerror

#define COMO_USAR "ss arquivo [-id id] [-c] [-h]\n"          \
  "-id id: começar a busca a partir de id. "                 \
  "default: 1\n"                                             \
  "-c: compila e mostra o arquivo tex da arvore\n"           \
  "-h: help\n"
  
#define ARQUIVO_FORMATO "Formato:\n"            \
  "n\n"                                         \
  "y1 y2 y3 ... yn\n"                           \
  "t\n"                                         \
  "k\n"                                         \
  "EOF\n"                                       \
  "\nn: inteiro; "                              \
  "yi: racional; "                              \
  "t: inteiro; "                                \
  "k: inteiro\n"

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
void print_arrd(double* arr, int start, int arr_length) {
  printf("(");
  int i;
  for (i = start; i < arr_length; ++i) {
    printf("%lf", arr[i]);
    if (i != arr_length-1) {
      printf(", ");
    }
  }
  printf(")");
}
void print_pilha(struct SimplePilha* pilha) {
  printf("(");
  int i;
  for (i = 0; i < pilha->len; ++i) {
    printf("(%d, %d)", pilha->vec[i].id, pilha->vec[i].acc);
    if (i != pilha->len-1) {
      printf(", ");
    }
  }
  printf(")");  
  
}
struct Params parse_argv(int argc, char** argv, int* compile) {
  struct Params prms;
  if (argc == 1) {
    printf(COMO_USAR);
    printf(ARQUIVO_FORMATO);
    exit(1);
  }

  int i, oki = 0;
  for (i = 2; i < argc; i++) {
    if (strcmp(argv[i], "-id") == 0) {
      if (argc <= i+1) {
        printf(COMO_USAR);
        exit(1);
      }
      prms.id = atoi(argv[++i]);
      oki = 1;
    } else if (strcmp(argv[i], "-c") == 0) {
      *compile = 1;
    } else if (strcmp(argv[i], "-h") == 0) {
      printf(COMO_USAR);
      printf(ARQUIVO_FORMATO);
      exit(1);
    } else {
      printf(COMO_USAR);
      exit(1);
    }
  }
  
  if (!oki) {
    prms.id = 1;
  } else if (prms.id == 0) {
    printf(COMO_USAR);
    exit(1);
  }
  
  FILE* input_file = fopen(argv[1], "r");
  if (input_file == NULL) {
    printf("%s\n", strerror(errno));
    exit(1);
  }
  int scan_ok;
  scan_ok = fscanf(input_file, "%d", &prms.y_length);
  if (scan_ok != 1) {
    printf("Problema ao ler n. Cheque o formato do arquivo providenciado." \
           "\n");
    printf(ARQUIVO_FORMATO);
    exit(1);
  }
  prms.y = malloc(prms.y_length*sizeof(double));
  if (prms.y == NULL) {
    printf("Erro ao alocar memória para o vetor y.\n");
    exit(1);
  }
  for (i = 0; i < prms.y_length; i++) {
    scan_ok = fscanf(input_file, "%lf", &prms.y[i]);
    if (scan_ok != 1) {
      printf("Problema ao ler y. Cheque o formato do arquivo providenciado.\n");
      printf(ARQUIVO_FORMATO);
      exit(1);
    }
  }
  quicksort(prms.y, prms.y_length);
  printf("\n");
  scan_ok = fscanf(input_file, "%d", &prms.t);
  if (scan_ok != 1) {
    printf("Problema ao ler t. Cheque o formato do arquivo providenciado.\n");
    printf(ARQUIVO_FORMATO);
    exit(1);
  }

  scan_ok = fscanf(input_file, "%d", &prms.k);
  if (scan_ok != 1) {
    printf("Problema ao ler k. Cheque o formato do arquivo providenciado.\n");
    printf(ARQUIVO_FORMATO);
    exit(1);
  }

  fclose(input_file);
  printf("*** parametros lidos ***\nid: %d\nk: %d\nt: %d\ny: ", prms.id, prms.k, prms.t);
  print_arrd(prms.y, 0, prms.y_length);
  printf("\n---\n");
  return prms;
}

// depreciado
struct Params parse_argv2(int argc, char** argv, int* rec) {
  int i, j, y_length, okk=0,okt=0,oky=0,oki=0;
  struct Params prms;
  for (i = 0; i < argc; ++i) {
    if (argv[i][0] == '-') {
      switch (argv[i][1]) {
      case 'r':
        *rec = 1;
        break;
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
        prms.y = malloc(y_length*sizeof(int));
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
  print_arrd(prms.y, 0, prms.y_length);
  printf("\n---\n");
  return prms;
}

int mycompare(const void* a, const void* b) {
  double subtraction = ((* (double*) a) - (* (double*) b));
  return subtraction < 0 ? -1 : subtraction > 0 ? 1 : 0;
}

void quicksort(double* v, int v_length) {
  qsort(v, v_length, sizeof(double), mycompare);
}

int stirling_approx(int n) {
  return (int) round(sqrt(2*M_PI*n)*pow(((float)n)/M_E,n));
}

int perm_rep(int n, int k) {
  return stirling_approx(n)/(stirling_approx(n-k)*stirling_approx(k));
}

void vec_expand(struct SimpleVec* sv) {
#ifdef DEBUGGING
  printf("sv->cap: %d\nsv->len: %d\n", sv->cap, sv->len);
#endif
  sv->cap = sv->cap * 2;
  int* temp = realloc(sv->vec, sv->cap*sizeof(int));
  if (temp) {
    sv->vec = temp;
#ifdef DEBUGGING
    printf("sv->cap: %d\nsv->len: %d\n--\n", sv->cap, sv->len);
#endif
  } else {
    free(sv->vec);
    printf("ERRO REALOCANDO sv->vec!!!");
    exit(2);
  }
}

void pilha_expand(struct SimplePilha* sp) {
#ifdef DEBUGGING
  printf("sp->cap: %d\nsp->len: %d\n", sp->cap, sp->len);
#endif
  sp->cap = sp->cap * 2;
  struct ElementoPilha* temp = realloc(sp->vec, sp->cap*sizeof(struct ElementoPilha));
  if (temp) {
    sp->vec = temp;
#ifdef DEBUGGING
    printf("sp->cap: %d\nsp->len: %d\n--\n", sp->cap, sp->len);
#endif
  } else {
    free(sp->vec);
    printf("ERRO REALOCANDO sp->vec!!!");
    exit(2);
  }
}

int nearlyEqual(double a, double b) {
  double e = 1e-10;
  double A = fabs(a);
  double B = fabs(b);
  double diff = fabs(a - b);
#ifdef DEBUGGING
  printf("%e, %e, %e, %e\nhehe\n",A,B,diff, DBL_MIN);
#endif
  if (a == b) { // shortcut, handles infinities
    return 1;
  } else if (a == 0 || b == 0 || diff < DBL_MIN) {
    // a or b is zero or both are extremely close to it
    // relative error is less meaningful here
    return diff < (e * DBL_MIN);
  } else { // use relative error
    return (diff / min((A + B), DBL_MAX)) < e;
  }
}

double min(double a, double b) {
  if (a < b)
    return a;
  return b;
}
