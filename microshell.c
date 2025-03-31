#include<unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#define bufor 2048

#define reset "\x1b[0m"
#define magenta "\x1b[35m"
#define red "\x1b[31m"
#define green "\x1b[92m"
#define yellow  "\x1b[33m"
#define blue "\x1b[36m"


void help() {
    printf(yellow"Microshell in C standard\n");
    printf("author: Julia Gad\n");
    printf(magenta"Welcome to the help section!\n");
    printf(reset);
    printf(" /\\_/\\ \n");
    printf("(=^.^=)\n");
    printf("(\") (\")_/\n");
	printf(blue"Here are available commands:\n - cd \n - exit \n - mkdir\n - pwd\n - rmdir\n - cp\n - cat\n");\
	printf(reset);
}

void pwd(){
    char s[100];
    printf("%s\n",getcwd(s,100));
}


void cd(char **command) {
    if (chdir(command[1])!=0){
        perror("Error: ");
    }
    else{
        chdir(command[1]);
    }

	}

void make(char **command){

    struct stat st = {0};

    if (stat(command[1], &st) == -1) {
        mkdir(command[1], 0700);}
    else if (stat(command[1], &st) == 0){
        printf("This directory already exsits.\n");
    }
}

void rm(char **command){

    struct stat st = {0};

    if (stat(command[1], &st) == 0) {
        rmdir(command[1]);}
    else if (stat(command[1], &st) == -1){
        printf("No such directory.\n");
    }

}

void cat(char **command)
{
        FILE  *file= fopen(command[1],"rw");
        if(file ==  NULL){
            perror(NULL);
            printf("Can't find this text file.\n");
        }
        else{
            char string_to_print;

            while((string_to_print = fgetc(file)) !=EOF)
                printf("%c", string_to_print);
        }


}

void cp(char **command)
{
        char copied_string;
        FILE* file1= fopen(command[1],"rw");
        FILE* file2= fopen(command[2],"rw");
        if(!file1){
            if (!file2){
                printf("Can't find both text files.\n");}
            else{
                printf("Can't find first text file.\n");
            }
        }

        else if(!file2){
                printf("Can't find second text file.\n");
        }
        else{
            while((copied_string = fgetc(file1)) !=EOF)
            fprintf( file2, "%c", copied_string);


            fclose(file1);
            fclose(file2);
        }

}


void polecenie_podzielone(char *wpisane,char **command){
        int i;
    	char rozdzielenie[]=" \n";
    	char *arg = strtok (wpisane, rozdzielenie);
    	i = 0;

    	while(arg !=NULL) {
    		command[i]=arg;
    		i++;
    		arg=strtok(NULL,rozdzielenie);
    	}
    	command[i]=NULL;
}

void execute_process(char **command){
    if(strncmp(command[0],"help",4)==0)
{
        help();
}

        else if(strncmp(command[0],"pwd", 3)==0)
{
        pwd();
}

    else if(strncmp(command[0],"cd",2)==0)
{
        cd(command);
}

    else if(strncmp(command[0],"exit",4)==0)
{
        exit(0);
}
    else if (strncmp(command[0], "mkdir",5)==0){
        make(command);
}
    else if(strncmp(command[0],"rmdir",5)==0)
{
        rm(command);
}

    else if(strncmp(command[0],"cat",3)==0)
{
        cat(command);
}

else if(strncmp(command[0],"cp",2)==0)
{
        cp(command);
}

    else{
			if(fork()==0){
				exit(execvp(command[0],command));
			}
			else{
				int status=0;
				wait(&status);
				if(status != 0){
                    perror("Error: ");
					printf("Type 'help' to see available commands.\n");
    }}

}
}


void print_path(){
char s[100];
    printf(yellow"%s",getlogin());
    printf(green"[");
	printf("%s",getcwd(s,100));
	printf("]");
	printf(magenta" $ ");
	printf(reset);
	}




int main(int argc, char *argv[]){
    char *com;
    char *command[300];
    char wpisane[bufor];
    int lh;
    while (1) {
        print_path();
        com = fgets(wpisane, bufor, stdin);
        lh=strlen(wpisane);
        wpisane[lh-1]='\0';
        polecenie_podzielone(com,command);
        execute_process(command);
    }
return 0;
}
