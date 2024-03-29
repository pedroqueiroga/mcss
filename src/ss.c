#include <stdio.h>
#include <string.h> // strcpy, strcat, strerror
#include <stdlib.h> // malloc, realloc
#include "ss_helper.h"
#include "ss.h"
#include <sys/stat.h>
#include <errno.h>
#include <time.h>

char* pertence_string[] = {"SOL", "SIM", "NAO"};

// se for solução, retorna 0. se d pertencer a I, retorna 1, cc 2.
int get_all(int id, struct Params* prms, double* Y) {
  int cont;// meu contador
  
  int* V = malloc(prms->y_length * sizeof(int));
  int V_index = prms->y_length;
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
  int p = prms->y_length-V_index; // Length[V]
  int m = 0; // Total[V]
  for (cont = V_index; cont < V_index + p; ++cont) { 
    m += V[cont];
  }
  int o = prms->y_length - prms->k - m; // # de uns que faltam
  double a = 0; // soma de uns
  for (cont = V_index; cont < V_index + p; ++cont) {
    // cont - V_index vai encaixar o primeiro de y com o primeiro de V
    a += V[cont]*prms->y[cont-V_index];
  }
  double d = prms->t-a; // desejado
  int int1[2], int2[2];
  int1[0] = p+1; int1[1] = p+o;
  int2[0] = prms->y_length-o+1; int2[1] = prms->y_length;

  int i = int1[0], j = int1[1];
  double Zij1, Zij2;
  if (((p-m) > prms->k) || (m>(prms->y_length-prms->k))) { //j > prms->y_length) {
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

#ifdef DEBUGGING
  printf("(Y, V, m, o, a, p, ij1, ij2, d, Zij1, Zij2)\n(");
  print_arrd(Y, 0, prms->y_length);
  printf(", ");
  print_arr(V, V_index, V_index + p);
  printf(", %d, %d, %lf, %d, ", m, o, a, p);
  print_arr(int1, 0, 2);
  printf(", ");
  print_arr(int2, 0, 2);
  printf(", %lf, %lf, %lf)\n", d, Zij1, Zij2);
#endif

  free(V);
  if (nearlyEqual(Zij1, d) && nearlyEqual(d, Zij2) && nearlyEqual(d, 0)) {
    // zij1 = d = zij2 = 0
    return 0;
  } else if ((nearlyEqual(Zij1,d) || Zij1 < d) && (nearlyEqual(d, Zij2) || d < Zij2)) {
    // zij1 <= d AND d <= zij2
    return 1;
  } else {
    return 2;
  }
}

// depreciado
void traverse_tree(struct SimpleVec* sols_vec, struct Params* prms, int id, double* Y, int* qtd_folhas) {

#ifdef DEBUGGING
  printf("id: %d\n", id);
  if (id == 0) {
    printf("wtf");
    exit(1);
  }
#endif


  int pertence = get_all(id, prms, Y);

#ifdef DEBUGGING
  printf("***************\n pertence: %s \n***************\n", pertence_string[pertence]);
#endif

  if (pertence == 0) {
    sols_vec->vec[(sols_vec->len)++] = id;
    (*qtd_folhas)++;
#ifdef DEBUGGING
    printf("qtd_folhas: %d\n", *qtd_folhas);
#endif
    // expande o vetor de solucoes
    if (sols_vec->cap <= sols_vec->len) {
      vec_expand(sols_vec);
    }
  } else if (pertence == 1) {
    traverse_tree(sols_vec, prms, id*2, Y, qtd_folhas);
    traverse_tree(sols_vec, prms, id*2 + 1, Y, qtd_folhas);
  } else {
    (*qtd_folhas)++;
#ifdef DEBUGGING
    printf("qtd_folhas: %d\n", *qtd_folhas);
#endif
  }
}

double traverse_tree2(struct SimpleVec* sols_vec, struct Params* prms, double* Y, int* qtd_folhas, int compile) {
  int i, id = prms->id;
  struct SimplePilha pilha;
  pilha.len = 0; pilha.cap = 1; pilha.vec = malloc(sizeof(struct ElementoPilha));

  char filename[100], noext_filename[100];
  int mkdir_status = mkdir("draw", S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH); // rwxr-xr-x
  if (mkdir_status == -1) {
    if (errno != EEXIST)
      printf("Erro ao tentar criar draw/\n  %s\n", strerror(errno));
  }
  sprintf(noext_filename, "tree_id%d_t%d_k%d", id, prms->t, prms->k);
  sprintf(filename, "draw/%s.tex", noext_filename);
  printf("escrevendo em %s\n", filename);
  FILE *pilhatex, *aux;
  pilhatex = fopen(filename, "w");
  if (pilhatex == NULL) {
    printf("nao consegui abrir %s\n  %s", filename, strerror(errno));
    free(pilha.vec);
    exit(1);
  }
  aux = fopen("draw/aux", "w");
  if (aux == NULL) {
    printf("nao consegui abrir %s\n  %s", "draw/aux", strerror(errno));
    free(pilha.vec);
    exit(1);
  }
  // escreve o cabecalho do tex:
  fprintf(pilhatex, \
          "\\documentclass[margin=3mm]{standalone}\n" \
          "\\usepackage[edges]{forest}\n\n"           \
          
          "\\begin{document}\n"                 \
          "  \\begin{forest}\n"                 \
          "    for tree={\n"                    \
          "      grow=south,\n"                 \
          "      draw,\n"                               \
          "      inner sep=1pt,\n"                      \
          "      s sep=7mm\n"                           \
          "    }\n");

  // calculando tempo de execucao do algoritmo
  struct timespec beg = {0,0}, end = {0,0};
  clock_gettime(CLOCK_MONOTONIC, &beg);
  if (id == 0) {
      printf("wtf\n");
      fclose(pilhatex);
      free(pilha.vec);
      exit(1);
  }
  do {
#ifdef DEBUGGING
    printf("id: %d\n", id);
#endif
    int pertence = get_all(id, prms, Y);
#ifdef DEBUGGING
    printf("***************\n pertence: %s \n***************\n", pertence_string[pertence]);
#endif

    fprintf(pilhatex, "[%d", id);
    
    switch (pertence) {
    case 1:
      pilha.vec[pilha.len].acc=0;
      pilha.vec[pilha.len++].id=id;
      if (pilha.cap <= pilha.len) {
        pilha_expand(&pilha);
      }
      id = pilha.vec[pilha.len-1].id * 2;

      break;
    case 0:
      sols_vec->vec[(sols_vec->len)++] = id;
#ifdef DEBUGGING
      printf("qtd_folhas: %d\n", *qtd_folhas);
#endif
      // expande o vetor de solucoes
      if (sols_vec->cap <= sols_vec->len) {
        vec_expand(sols_vec);
      }

      fprintf(pilhatex, ",fill=teal");
    case 2:
      pilha.vec[pilha.len].acc=1;
      pilha.vec[pilha.len++].id=id;
      
      fprintf(pilhatex, ",draw=red]");
      
      (*qtd_folhas)++;
      while (pilha.len > 1) {
        i = pilha.len-1;
        if (pilha.vec[i].acc != 0 && pilha.vec[i-1].acc != 0) {
          pilha.vec[i-2].acc = pilha.vec[i].acc + pilha.vec[i-1].acc;
          pilha.len -= 2;
          fprintf(aux, "{%d,%d}\n{%d,%d}\n",
                  pilha.vec[i].id, pilha.vec[i].acc,
                  pilha.vec[i-1].id, pilha.vec[i-1].acc);
          fprintf(pilhatex, "]");
          
        } else break;
      }
      if (pilha.cap <= pilha.len) {
        pilha_expand(&pilha);
      }
      // TODO contrair pilha
      id = pilha.vec[pilha.len-1].id + 1;

#ifdef SHOWPILHA
      printf("pilha: ");
      print_pilha(&pilha);
      if (pilha.vec[0].acc == 0) printf(" %d", id);
      printf("\n");
#endif

      break;
    }
  } while(pilha.vec[0].acc == 0);
  // fim da execucao do algoritmo
  clock_gettime(CLOCK_MONOTONIC, &end);
  double tempo_decorrido = ((double) end.tv_sec + 1.0e-9*end.tv_nsec) - ((double)beg.tv_sec + 1.0e-9*beg.tv_nsec);

  fprintf(aux, "{%d,%d}\n", pilha.vec[0].id, pilha.vec[0].acc);
  fclose(aux);
  
  fprintf(pilhatex,
          "\n\\end{forest}\n"                   \
          "\\end{document}\n");
  fclose(pilhatex);

  // colocando folhas nos nos
  
  char awk_comm[200];
  sprintf(awk_comm,
          "awk -f leaves.awk "      \
          "%s > tmp && mv tmp %s && rm draw/aux",
          filename, filename);
  int sys_ret = system(awk_comm);
  if (sys_ret == -1) {
    printf("nao tem awk? nao foi possivel colocar"      \
           " a qtd de folhas na raiz da arvore.");
  }

  if (compile) {
    char stitch_together[500];
    sprintf(stitch_together,
            "cd draw; pdflatex "                        \
            "-interaction=batchmode %s.tex && "         \
            "rm %s.aux %s.log && xdg-open %s.pdf "      \
            ">/dev/null 2>&1 && echo OK!",
            noext_filename, noext_filename,
            noext_filename, noext_filename);
    sys_ret = system(stitch_together);
    if (sys_ret == -1) {
      printf("arquivo latex gerado em %s\n", filename);
    }
  }
  
  free(pilha.vec);
  return tempo_decorrido;
}
