#include "print_util.h"
#include <stdio.h>
#include <stdlib.h>

void printUCharArray(unsigned char bytes[], int size) {

    printf("0x");
    char str[size * 2 + 1];

    for (size_t j = 0; j < size; j++) {
        sprintf(&str[j * 2], "%02x", bytes[j]);
    }

    printf("%s\n", str);
}
