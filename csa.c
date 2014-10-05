#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

#define freq 8000

int main(int argc, char **argv){

    pid_t pid;
    int pfd[2];
    int i, status;

    int ganancia = 3;  //TODO cambiar cuando se termine la tarea

    FILE * sd;

//    original_file=fopen(argv[1],"r");  //TODO cambiar cuando se termine la tarea

    /*
     * Create a pipe.
     */
    if (pipe(pfd) < 0) {
        perror("pipe");
        exit(1);
    }

    /*
     * Create a child process.
     */
    if ((pid = fork()) < 0) {
        perror("fork");
        exit(1);
    }

    /*
     * The child process executes "octave" with -i option.
     */
    if (pid == 0) {
        int junk;

        /*
         * Attach standard input of this child process to read from the pipe.
         */
        dup2(pfd[0], 0);
        close(pfd[1]);  /* close the write end of the pipe */
        junk = open("/dev/null", O_WRONLY);
        dup2(junk, 1);  /* throw away any message sent to screen*/
        dup2(junk, 2);  /* throw away any error message */
        execlp("octave", "octave", "-i", (char *)0);
        perror("exec");
        exit(-1);
    }

    /*
     * We won't be reading from the pipe.
     */
    close(pfd[0]);
    sd = fdopen(pfd[1], "w");  /* to use fprintf instead of just write */

    //TODO falta arreglar lo de los 40ms, pero se deja para al final mejor, creo

//    fprintf(sd, "];\n");fflush(sd);

    fprintf(sd, "original=loadaudio('SaludoAudacity','raw',16)\n"); fflush(sd);
    fprintf(sd, "y= 0:(1/%d):((rows(original)-1)/%d)\n", freq, freq);fflush(sd);

    fprintf(sd, "subplot(2,1,1);\n"); fflush(sd);
    fprintf(sd, "plot(y,original);\n"); fflush(sd);
    
    fprintf(sd, "amplificado = int16(original*%d)\n", ganancia); fflush(sd);
    fprintf(sd, "saveaudio('amplificado',amplificado,'raw',16)\n"); fflush(sd);

    fprintf(sd, "subplot(2,1,2);\n"); fflush(sd);
    fprintf(sd, "plot(y,amplificado);\n"); fflush(sd);


    printf("ENTER para continuar");
    getchar(); 
    fprintf(sd, "\n exit\n"); fflush(sd);

    /*
     * Wait for the child to exit and
     * close de pipe.
     */
    waitpid(pid, &status, 0);
    fclose(sd);

    /*
     * Exit with a status of 0, indicating that
     * everything went fine.
     */
    exit(0);
 //   return 0;
}
