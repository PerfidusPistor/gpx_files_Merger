#include <stdio.h>
#include <stdlib.h>
#include <string.h>

    char time1[256], time2[256];
    int timeGet1, timeGet2;

int takeField(FILE *fp, char *field);

int main(int argc, char *argv[])
{
    FILE *fin1, *fin2, *fout;
    char f1Line[2048]="";
    char f2Line[2048]="";

    printf("\t--------------------------------------\n");
    printf("\t----------GPX Merger by Ivan----------\n");
    printf("\t--------------------------------------\n");

    if(argc!=3)
    {
        printf("usage: %s file1.gpx file2.gpx", argv[0]);
        return -1;
    }

    fin1 = fopen(argv[1],"r");
    fin2 = fopen(argv[2],"r");
    fout = fopen("merged.gpx","w");

    for(;fscanf(fin1,"%s",f1Line)!=EOF;){

        if(strncmp(f1Line,"<time>",6)==0 && timeGet1!=0){ //taking time value (just a check)

            sscanf(f1Line,"<time>%s</time>", time1);
            timeGet1=1;

            }

        if(strncmp(f1Line,"<gpxtpx:atemp>",14)==0){

        sprintf(f2Line," <gpxtpx:hr>%d</gpxtpx:hr>", takeField(fin2,NULL));

        //copy to new file (fout)
        fprintf(fout,"%s\n%s\n", f1Line, f2Line);


        }
        else{

        fprintf(fout,"%s\n", f1Line);
        }

    }


    printf("all done!\n");

    fclose(fin1);
    fclose(fin2);
    fclose(fout);
    return 0;
}


int takeField(FILE *fp, char *field){

    int value;
    char buffer[2048];

        for(;fscanf(fp,"%s",buffer)!=EOF;){
            if(strncmp(buffer,"<time>",6)==0 && timeGet2!=0){ //taking time value (just a check)

            sscanf(buffer,"<time>%s</time>", time2);
            timeGet2=1;

            if(strcmp(time2,time1)>5){
                printf("too much displacement for the activities (more than 5 seconds)!\n");
                return 1;
            }

            }

            if(strncmp(buffer,"<gpxtpx:hr>",11)==0){ // field to merge (health rate)

            sscanf(buffer,"<gpxtpx:hr>%d</gpxtpx:hr>", &value);
            break;

            }


        }

    return value;
}
