#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

#define freq 8000

pid_t octave_aplay(int *pfd, char *path);

int main(int argc, char **argv){

    pid_t pid, pid2;
    int pfd[2];
    int i, status;
    FILE *sd;
    

//////SIMULACIÓN ARGUMENTOS/////////
    int p=1;
    char path[100] = "SaludoAudacity.raw";    //argv[1] se supone que es la ruta del archivo
    int ganancia = 3;
    int offset = 200; 
////////////////////////////////////

    char full_name[(int) sizeof(path)];
    char filename[(int)sizeof(path)];
    char *temp;

    temp = strrchr(path,'/');   
    if( temp == NULL){
        strcpy(full_name,path);
    }else{
        strcpy(full_name,(strrchr(path,'/')+1));
    }

    temp = malloc(sizeof(path));
    strcpy(temp,full_name);
    strcpy(filename ,strtok(temp, "."));
    printf("%s\n",filename );
    free(temp);

    pid = octave_aplay(pfd, NULL);

    sd = fdopen(pfd[1], "w");   /* to use fprintf instead of just write */

    //TODO falta arreglar lo de los 40ms, pero se deja para al final mejor, creo
    //TODO en Octave se pone solo el nombre del archivo, sin extensión... hay que arreglar eso...
    int win_left = offset*8;
    int win_right = win_left+320;

    fprintf(sd, "test(1)\n");fflush(sd);
    fprintf(sd, "original=loadaudio('%s','raw',16)\n", filename); fflush(sd);
    fprintf(sd, "y= 0:(1/%d):((rows(original)-1)/%d)\n", freq, freq);fflush(sd);

    fprintf(sd, "subplot(3,2,1);\n"); //fflush(sd);
    fprintf(sd, "plot(y(%d:%d),original(%d:%d));\n",win_left,win_right,win_left,win_right); fflush(sd);
    
    fprintf(sd, "amplificado = int16(original*%d)\n", ganancia); fflush(sd);
    fprintf(sd, "saveaudio('amplificado',amplificado,'raw',16)\n"); fflush(sd);
    fprintf(sd, "amplificado = loadaudio('amplificado','raw',16)\n");fflush(sd);
    fprintf(sd, "subplot(3,2,3);\n"); //fflush(sd);
    fprintf(sd, "plot(y(%d:%d),amplificado(%d:%d));\n",win_left,win_right,win_left,win_right); fflush(sd);

    fprintf(sd, "filtrado = elo330(amplificado);\n"); fflush(sd);
    fprintf(sd, "saveaudio('filtrado',filtrado,'raw',16)\n"); fflush(sd);

    fprintf(sd, "subplot(3,2,5);\n"); //fflush(sd);
    fprintf(sd, "plot(y(%d:%d),filtrado(%d:%d))\n",win_left,win_right,win_left,win_right);fflush(sd);
    fprintf(sd, "error(original,filtrado,%d)\n;",ganancia);fflush(sd);


    if(p==1){       //si es que se pone la bandera P
        pid2 = octave_aplay(pfd, path);
        waitpid(pid2, &status, 0); //Wait for the child to exit and close the pipe
        pid2 = octave_aplay(pfd, "amplificado.raw");
        waitpid(pid2, &status, 0); //Wait for the child to exit and close the pipe
        pid2 = octave_aplay(pfd, "filtrado.raw");

        printf("ENTER para terminar");
        getchar(); 

        kill(pid2, SIGKILL);   
        fprintf(sd, "\n exit\n"); fflush(sd);
    }else{
        printf("ENTER para terminar");
        getchar(); 

        fprintf(sd, "\n exit\n"); fflush(sd);
    }

    waitpid(pid, &status, 0); //Wait for the child to exit and close the pipe
    fclose(sd);

    char str[999];
    FILE * tmp;
    tmp = fopen( "temp.tmp" , "r");
    if (tmp) {
        while (fscanf(tmp, "%s", str)!=EOF)
            printf(" el error es:%s",str);
    fclose(tmp);
}

    return 0;
}

pid_t octave_aplay(int *pfd, char *filename){
    pid_t pid;

    //Create a pipe.
    if (pipe(pfd) < 0) {
        perror("pipe");
        exit(1);
    }

    //Create a child process.
    if ((pid = fork()) < 0) {
        perror("fork");
        exit(1);
    }

    if (pid == 0) {
        int junk;

        //Attach standard input of this child process to read from the pipe.
        dup2(pfd[0], 0);
        close(pfd[1]);  /* close the write end of the pipe */
        junk = open("/dev/null", O_WRONLY);
        dup2(junk, 1);  /* throw away any message sent to screen*/
        dup2(junk, 2);  /* throw away any error message */
        
        if(filename == NULL){   
            execlp("octave", "octave", "-i", (char *)0);
        }else{
            execlp("aplay", "aplay", "--format=S16_LE", "-t", "raw", filename, (char *)0);
        }
        perror("exec");
        exit(-1);
    }
    close(pfd[0]);  /* We won't be reading from the pipe*/
    
    return pid;
}
