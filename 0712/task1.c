#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void filePrint(FILE *file) {
    char *str = (char *) malloc(1 * sizeof(char));
    while (!feof(file)) {
        fscanf(file, "%s", str);
        printf("%s ", str);
    }
    free(str);
}

void fileAppend(FILE *file, char *str) {
    fseek(file, 0, SEEK_END);
    fprintf(file, "%s", str);
}

void fileSearch(FILE *file, char *str) {
    // find str in line and print this line with UPPER CASE of str
    char *line = (char *) malloc(1 * sizeof(char));
    while (!feof(file)) {
        fscanf(file, "%s", line);
        if (strstr(line, str) != NULL) {
            printf("%s ", line);
        }
    }
}