#ifndef SS_HEADER
#define SS_HEADER

#include "ss_helper.h"

int get_all(int id, int* y, int y_length, int k, int t, int* Y);
void traverse_tree(int* sols, int* sols_length, struct Params prms, int id, int* Y, int* qtd_folhas);

#endif
