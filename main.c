#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_COMMANDS 2+1 // 1 is the noCmd for coherence


    char time1[256], time2[256];
    int timeGet1, timeGet2;

    // command from input and conversion
typedef enum{
noCmd,hr,cad
}command;

char *cmdName[NUM_COMMANDS]={"noCmd","hr","cad"};

command convertCommand(char *inputCmd);

//functions declarations

int takeField(FILE *fp, command field);


int main(int argc, char *argv[])
{
    FILE *fin1, *fin2, *fout;
    char f1Line[2048]="";
    char f2Line[2048]="";
    char inputCmd[256];
    command cmd;

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

    //field to merge
    printf("what field do you want to merge: ");
    scanf("%s", inputCmd);
    cmd = convertCommand(inputCmd);


    for(;fscanf(fin1,"%s",f1Line)!=EOF;){

        if(strncmp(f1Line,"<time>",6)==0 && timeGet1!=0){ //taking time value (just a check)

            sscanf(f1Line,"<time>%s</time>", time1);
            timeGet1=1;

            }

        if(strncmp(f1Line,"<gpxtpx:atemp>",14)==0){

        sprintf(f2Line," <gpxtpx:hr>%d</gpxtpx:hr>", takeField(fin2,cmd));

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


int takeField(FILE *fp, command field){

    int value;
    char buffer[2048];
    int flag=0;

        for(;fscanf(fp,"%s",buffer)!=EOF;){
            if(strncmp(buffer,"<time>",6)==0 && timeGet2!=0){ //taking time value (just a check)

            sscanf(buffer,"<time>%s</time>", time2);
            timeGet2=1;

            if(strcmp(time2,time1)>5){
                printf("too much displacement for the activities (more than 5 seconds)!\n");
                return 1;
            }

            }
            switch (field){

                case 1:

                   if(strncmp(buffer,"<gpxtpx:hr>",11)==0){ // field to merge
                    sscanf(buffer,"<gpxtpx:hr>%d</gpxtpx:hr>", &value);
                    flag=1;
                   }

                break;

                default:
                break;

            }

        if(flag==1) // the field has been found, exit
            break;

            }

    return value;
}

command convertCommand(char *inputCmd){
    command cmd=0; //setup to no input command
    int i;

    for(i=0; i<NUM_COMMANDS; i++)
        if(strcmp(inputCmd,cmdName[i])==0)
            cmd=i;

    return cmd;
}
