#ifndef SS_HELPER_HEADER
#define SS_HELPER_HEADER

struct Params {
  int k;
  int t;
  int id;
  int y_length;
  int* y;
};

struct SimpleVec {
  int cap;
  int len;
  int* vec;
};

void print_arr(int* arr, int start, int arr_length);
struct Params parse_argv(int argc, char** argv);
void quicksort(int* v, int v_length);
int stirling_approx(int n);
int perm_rep(int n, int k);
void vec_expand(struct SimpleVec* sv);

#endif
