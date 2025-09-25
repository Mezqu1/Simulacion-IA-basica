#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdlib.h>
#include <errno.h>
//handler
static void reap(int sig){
    int st; (void)sig;
    while (waitpid(-1, &st, WNOHANG) > 0) { /* limpia zombies */ }
}

int main(void) {
   
    if (signal(SIGCHLD, reap) == SIG_ERR) {
        perror("signal");
        return 1;
    }
    //bucle prompt
    for (;;) {
        printf("ia> "); fflush(stdout);
        char line[1024];
        if (!fgets(line, sizeof line, stdin)) break;
        if (line[0]=='\n') continue;
        line[strcspn(line, "\n")] = 0;
    
        //parseo simple
        char *cmd = strtok(line, " \t");
        if (!cmd) continue;
        if (!strcmp(cmd,"exit")) break;

        char *fname = strtok(NULL, " \t");
        if (!fname){ fprintf(stderr,"falta <nombre_archivo>\n"); continue; }

        const char *prog = NULL;
        if      (!strcmp(cmd,"tokenizar"))  prog = "./labo2_ia_tokenizador";
        else if (!strcmp(cmd,"vectorizar")) prog = "./labo2_ia_vectorizador";
        else if (!strcmp(cmd,"analizar"))   prog = "./labo2_ia_analizador";
        else { fprintf(stderr,"comando desconocido: %s\n", cmd); continue; }
       
        //validacion
        char data_path[1024];
        snprintf(data_path, sizeof data_path, "data/%s", fname);
        if (access(data_path, R_OK) != 0){ perror("data/<archivo>"); continue; }


        //fork
        pid_t pid = fork();
        if (pid == 0){
            char *argv[] = { (char*)prog, data_path, NULL };
            execvp(prog, argv);
            perror("execvp"); _exit(127);
        } else if (pid > 0){
            printf("[lanzado] %s %s (pid=%d)\n", prog, data_path, pid);
        } else {
            perror("fork");
        }
    }
    return 0;
}
