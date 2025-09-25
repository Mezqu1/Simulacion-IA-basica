#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define TOKLEN 128

const char *positivas[] = {"bueno","excelente","feliz","hola","amor"};
const char *negativas[] = {"malo","triste","odio","horrible","feo"};
#define NPOS (sizeof(positivas)/sizeof(positivas[0]))
#define NNEG (sizeof(negativas)/sizeof(negativas[0]))

int main(int argc, char **argv) {
    if (argc < 2) { fprintf(stderr,"uso: analizar <archivo>\n"); return 1; }

    // salida: cambia .txt por _analisis.txt
    char out[1024];
    strncpy(out, argv[1], sizeof out); out[sizeof out - 1] = 0;
    size_t n = strlen(out);
    if (n >= 4 && strcmp(out+n-4,".txt")==0) n -= 4;
    snprintf(out+n, sizeof out - n, "_analisis.txt");
    
    //Abre el archivo de entrada para leer.
    //Abre el archivo de salida para escribir.

    FILE *in = fopen(argv[1], "r");
    if (!in) { perror("entrada"); return 1; }
    FILE *fo = fopen(out, "w");
    if (!fo) { perror("salida"); fclose(in); return 1; }

    int pos=0, neg=0;
    char tok[TOKLEN]; int ti=0, c;

    //procesa cada caracter 

    while ((c=fgetc(in))!=EOF) {
        if (isalpha((unsigned char)c)) {
            if (ti<TOKLEN-1) tok[ti++]=tolower(c);
        } else if (ti>0) {
            tok[ti]=0; ti=0;
            for (size_t i=0;i<NPOS;i++) if (!strcmp(tok,positivas[i])) pos++;
            for (size_t i=0;i<NNEG;i++) if (!strcmp(tok,negativas[i])) neg++;
        }
    }
    if (ti>0) { // último token
        tok[ti]=0;
        for (size_t i=0;i<NPOS;i++) if (!strcmp(tok,positivas[i])) pos++;
        for (size_t i=0;i<NNEG;i++) if (!strcmp(tok,negativas[i])) neg++;
    }

    //analiza el resultado
    
    fprintf(fo,"Positivas: %d\n", pos);
    fprintf(fo,"Negativas: %d\n", neg);
    if (pos>neg) fprintf(fo,"Resultado: positivo\n");
    else if (neg>pos) fprintf(fo,"Resultado: negativo\n");
    else fprintf(fo,"Resultado: neutro\n");

    fclose(in); fclose(fo);
    return 0;
}

