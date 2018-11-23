#include <stdio.h>
#include <stdlib.h>

//$ ./ss -t 597 -y 90 92 93 108 99 103 104 97 111 115 112 -k 5

char* pertence_string[] = {"SOL", "SIM", "NAO"};

void print_arr(int* arr, int start, int arr_length);
int get_all(int id, int* y, int y_length, int k, int t, int* Y);
struct Params parse_argv(int argc, char** argv);
void traverse_tree(int* sols, int* sols_length, struct Params prms, int id, int* Y, int* qtd_folhas);
void quicksort(int* v, int v_length);

struct Params {
  int k;
  int t;
  int y_length;
  int* y;
};

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
  // as permutacoes de n k a k
  int qtd_max_sols = prms.y_length;
  for (i = 1; i <= prms.k + 1; i++) {
    qtd_max_sols *= (prms.y_length-i);
  }
  int* sols = (int*) malloc(qtd_max_sols * sizeof(int));
  int qtd_folhas = 0;
  int sols_length = 0;
  traverse_tree(sols, &sols_length, prms, 1, Y, &qtd_folhas);
  printf("solucoes: ");
  print_arr(sols, 0, sols_length);
  printf("\nqtd_folhas: %d\n", qtd_folhas);
  free(sols);
  free(Y);
  free(prms.y);
  return 0;
}

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
  int i, j, y_length, okk=0,okt=0,oky=0;
  struct Params prms;
  for (i = 0; i < argc; ++i) {
    if (argv[i][0] == '-') {
      switch (argv[i][1]) {
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
    printf("argumentos:\n-k qtd de zeros\n-t objetivo\n-y vetor separado por espaços\n");
    exit(1);
  }
  printf("k: %d\n", prms.k);
  printf("t: %d\n", prms.t);
  printf("y: ");
  print_arr(prms.y, 0, prms.y_length);
  printf("\n");
  return prms;
}

int mycompare(const void* a, const void* b) {
  return ((* (int*) a) - (* (int*) b));
}

void quicksort(int* v, int v_length) {
  qsort(v, v_length, sizeof(int), mycompare);
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
