#ifndef SS_HEADER
#define SS_HEADER

#include "ss_helper.h"

int get_all(int id, struct Params* prms, int* Y);
void traverse_tree(struct SimpleVec* sols_vec, struct Params* prms, int id, int* Y, int* qtd_folhas);

#endif
