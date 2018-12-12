#ifndef SS_HEADER
#define SS_HEADER

#include "ss_helper.h"

int get_all(int id, struct Params* prms, double* Y);
// depreciado
void traverse_tree(struct SimpleVec* sols_vec, struct Params* prms, int id, double* Y, int* qtd_folhas);
double traverse_tree2(struct SimpleVec* sols_vec, struct Params* prms, double* Y, int* qtd_folhas);

#endif
