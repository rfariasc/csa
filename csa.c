#include <stdio.h>

#define freq 8000


int main(int argc, char **argv){

    FILE *fp;
    short int buffer[1];
//    fp=fopen(argv[1],"r");
    
    fp=fopen("SaludoAudacity.raw","r");

    if(fp==NULL){
        printf("ERROR ABRIENDO ARCHIVO\n");
        return 1;
    }

    while(!feof(fp)){
        fread(buffer, sizeof(short), 1, fp);
        printf("%d\n", buffer[0]); 



    }

    fclose(fp);

    return 0;

}
