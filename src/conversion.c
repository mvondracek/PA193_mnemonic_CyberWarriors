#include "conversion.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

unsigned char *hexstr_to_char(const char *hexstr) {

    size_t len = strlen(hexstr);
    size_t final_len = len / 2;

    unsigned char *chrs = (unsigned char *) malloc((final_len + 1) * sizeof(*chrs));

    size_t i, j;

    for (i = 0, j = 0; j < final_len; i += 2, j++)
        chrs[j] = (hexstr[i] % 32 + 9) % 25 * 16 + (hexstr[i + 1] % 32 + 9) % 25;
    chrs[final_len] = '\0';

    return chrs;
}
