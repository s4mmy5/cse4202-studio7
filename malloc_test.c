#include <stdlib.h>

int main(int argc, char* argv[]) {
  if (argc != 2) {
    return EXIT_FAILURE;
  }

  long bytes = atol(argv[1]);
  void * ptr = malloc(bytes);

  return EXIT_SUCCESS;
}
