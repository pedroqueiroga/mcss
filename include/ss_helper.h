#ifndef SS_HELPER_HEADER
#define SS_HELPER_HEADER

struct Params {
  int k;
  int t;
  int id;
  int y_length;
  double* y;
};

struct SimpleVec {
  int cap;
  int len;
  int* vec;
};

struct ElementoPilha {
  int id;
  int acc;
};

struct SimplePilha {
  int cap;
  int len;
  struct ElementoPilha* vec;
};

void print_arr(int* arr, int start, int arr_length);
void print_arrd(double* arr, int start, int arr_length);
void print_pilha(struct SimplePilha* pilha);
struct Params parse_argv(int argc, char** argv);
struct Params parse_argv2(int argc, char** argv, int* rec);
void quicksort(double* v, int v_length);
int stirling_approx(int n);
int perm_rep(int n, int k);
void vec_expand(struct SimpleVec* sv);
void pilha_expand(struct SimplePilha* sp);
int nearlyEqual(double a, double b);
double min(double a, double b);

#endif
