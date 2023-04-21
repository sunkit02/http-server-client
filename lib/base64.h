#ifndef BASE_64
#define BASE_64

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

char *base64_encode(void *data, size_t inputLength, size_t *outputLength);

void *base64_decode(const char *data, size_t inputLength, size_t *outputLength);
 

#endif // !BASE_64
