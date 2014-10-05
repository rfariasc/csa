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
    FILE * sd;

    FILE *audio_file;
    short int buffer[1];

//    audio_file=fopen(argv[1],"r");
    audio_file=fopen("SaludoAudacity.raw","r");
    if(audio_file==NULL){
        printf("ERROR ABRIENDO ARCHIVO\n");
        return 1;
    }

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
    fprintf(sd, "x= [");
    while(!feof(audio_file)){
        fread(buffer, sizeof(short), 1, audio_file);
        fprintf(sd, "%d ", buffer[0]);
//        printf("%d\n", buffer[0]); 
    }
    fprintf(sd, "];\n");fflush(sd);

    fprintf(sd, "y= 0:(1/%d):((columns(x)-1)/%d)\n", freq, freq);fflush(sd);

    fprintf(sd, "plot(y,x);\n"); fflush(sd);

//    fprintf(sd,"columns(x)\n");fflush(sd);
//    fprintf(sd,"columns(y)\n");fflush(sd);
//    fprintf(sd,"x(1)\n");fflush(sd);
//    fprintf(sd,"y(1)\n");fflush(sd);
//    fprintf(sd,"x(columns(x))\n");fflush(sd);
//    fprintf(sd,"y(columns(y))\n");fflush(sd);

    sleep(20);
    fprintf(sd, "\n exit\n"); fflush(sd);


    /*
     * Wait for the child to exit and
     * close de pipe.
     */
    waitpid(pid, &status, 0);
    fclose(sd);

    fclose(audio_file); //cerrar archivo io
    /*
     * Exit with a status of 0, indicating that
     * everything went fine.
     */
    exit(0);


    return 0;

}
