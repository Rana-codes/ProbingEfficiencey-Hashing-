#include "hash_function.h"

int hash_fct(int key, int size) {
   return key % size;
}

