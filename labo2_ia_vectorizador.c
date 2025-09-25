#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAXTOK  8192
#define TOKLEN  128

typedef struct { char word[TOKLEN]; int freq; } Entry;

static int find_or_add(Entry *tab, int *n, const char *w) {
    for (int i = 0; i < *n; i++) if (strcmp(tab[i].word, w) == 0) return i;
    if (*n >= MAXTOK) return -1;
    strncpy(tab[*n].word, w, TOKLEN-1);
    tab[*n].word[TOKLEN-1] = 0;
    tab[*n].freq = 0;
    return (*n)++;
}

int main(int argc, char **argv) {
    if (argc < 2) { fprintf(stderr, "uso: vectorizar <archivo>\n"); return 1; }

    // nombre salida: data/<nombre>_vector.txt
    char out[1024];
    strncpy(out, argv[1], sizeof out); out[sizeof out - 1] = 0;
    size_t n = strlen(out);
    if (n >= 4 && strcmp(out + n - 4, ".txt") == 0) n -= 4;
    snprintf(out + n, sizeof out - n, "_vector.txt");

    //Abre entrada en modo lectura y salida en escritura
    FILE *in = fopen(argv[1], "r");
    if (!in) { perror("fopen in"); return 1; }
    FILE *fo = fopen(out, "w");
    if (!fo) { perror("fopen out"); fclose(in); return 1; }

    Entry vocab[MAXTOK]; int nv = 0; //conteo de entradas usadas
    char tok[TOKLEN]; int ti = 0; int c; //buffer para el token en construcción.

    // tokenización simple: solo letras, a minúsculas
    while ((c = fgetc(in)) != EOF) {
        if (isalpha((unsigned char)c)) {
            if (ti < TOKLEN-1) tok[ti++] = (char)tolower(c);
        } else if (ti > 0) {
            tok[ti] = 0;
            int idx = find_or_add(vocab, &nv, tok);
            if (idx >= 0) vocab[idx].freq++;
            ti = 0;
        }
    }
    if (ti > 0) { // último token si termina en letra
        tok[ti] = 0;
        int idx = find_or_add(vocab, &nv, tok);
        if (idx >= 0) vocab[idx].freq++;
    }

    // salida
    fprintf(fo, "Vocabulario: [");
    for (int i = 0; i < nv; i++) {
        if (i) fprintf(fo, ", ");
        fprintf(fo, "%s", vocab[i].word);
    }
    fprintf(fo, "]\n");

    fprintf(fo, "Vector: [");
    for (int i = 0; i < nv; i++) {
        if (i) fprintf(fo, ", ");
        fprintf(fo, "%d", vocab[i].freq);
    }
    fprintf(fo, "]\n");

    fclose(in); fclose(fo);
    return 0;
}
