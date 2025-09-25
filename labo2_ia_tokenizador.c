#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main(int argc, char **argv) {
    if (argc < 2) { fprintf(stderr, "uso: tokenizar <archivo>\n"); return 1; }

    // Construir nombre de salida: data/<nombre>_tokens.txt
    char out[1024];
    strncpy(out, argv[1], sizeof out);
    out[sizeof out - 1] = 0;
    size_t n = strlen(out);
    if (n >= 4 && strcmp(out + n - 4, ".txt") == 0) n -= 4;
    snprintf(out + n, sizeof out - n, "_tokens.txt");

    FILE *in = fopen(argv[1], "r");
    if (!in) { perror("fopen in"); return 1; }
    FILE *fo = fopen(out, "w");
    if (!fo) { perror("fopen out"); fclose(in); return 1; }

    char tok[1024];
    int ti = 0, first = 1, c;

    fprintf(fo, "[");
    while ((c = fgetc(in)) != EOF) {
        if (isalpha((unsigned char)c)) {
            if (ti < (int)sizeof tok - 1) tok[ti++] = (char)tolower(c);
        } else if (ti > 0) {
            tok[ti] = 0;
            if (!first) fprintf(fo, ", ");
            fprintf(fo, "%s", tok);
            first = 0; ti = 0;
        }
    }
    if (ti > 0) { // último token si termina en letra
        tok[ti] = 0;
        if (!first) fprintf(fo, ", ");
        fprintf(fo, "%s", tok);
    }
    fprintf(fo, "]\n");

    fclose(in);
    fclose(fo);
    return 0;
}

