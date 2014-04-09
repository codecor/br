// author: cc 
// dec 2 2012
// bring file from dir-a to dir-b
//
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <errno.h>
#include <sys/wait.h>
#include <termios.h>
#include <stdint.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>

// global storage file for files currently bein brought 
char *tmp_file="/tmp/bringing";
int DEBUG=0;
int usage(){
    printf("*** br - bring ***\n## Overview\n Similar to cp, br copies files.");
    printf("br will bring the files from point a to point b.\n## Usage\n ");
    printf("When selecting files:\n  br <somefile>\n ... do what you gotta");
    printf("do ... go where you gotta go; then to drop: \n  br\n");
    printf("## Example\n [/root/]$ br main.c utils.c\n [/root/]$ cd ");
    printf("development/\n [/root/development/]$ br");
    printf("\n");
    return 1;
}

int main (int argc,char *argv[]){

    // if one, store
    if (argc==2) {
        // ^^ not going to work because there could be more than 1 file 
        // to bring.
        char file_name[]="file not available";
        char file_addr[4096];
        char wrk_dir[4096];
        FILE *fp;

        // get file path
        // we want the file's full path..
        if (getWorkingDir(&wrk_dir)==0) {
            if (DEBUG) printf ("[debug] wrk_dir = %s\n",wrk_dir);
        }
        //
        // lets handle more than 1 file 
        //for (len
        //return 0;
        strcpy(file_name,argv[1]);
        if (DEBUG) printf("[debug] file_name = %s\n",file_name);

        if (!access(file_name,F_OK)) // file doesn't exist then give usage and exit
        {
            if (DEBUG) printf("[debug] file exists.\n");
        } else {
            if (DEBUG) printf("[debug] error: file does not exist.\n");
            usage();
            return 1;
        }

        strcat(wrk_dir,"/");
        sprintf(file_addr,strcat(wrk_dir,file_name));
        if (DEBUG) printf("[debug] file_addr=%s\n",file_addr);

        // check for file
        if ((fp=fopen(tmp_file, "wb")) == NULL) {
            printf("Cannot open temporary file.\n");
            exit(1);
        }
        
        // write tmp addr holder file
        fprintf(fp,file_addr);
        if (DEBUG) printf("[debug] bringing %s...\n",file_addr);
        fclose(fp);
        if (DEBUG) printf("[debug] fclosed\n");
    } else {
        // if none, drop
        // check for file waiting to be dropped
        if (bring_it()!=0){
            printf("No file to be brought.\n");
        }
    }

    if (DEBUG) printf("[debug] returning\n");
    return 0;
}

int do_bring(int cnt,char *src,char *dest)
{
    FILE *in, *out;
    char ch;

    if (cnt!=3) {
        printf("You forgot to enter a filename.\n");
        exit(1);
    }
    if ((in=fopen(src, "rb")) == NULL) {
        printf("Cannot open input file.\n");
        exit(1);
    }
    if ((out=fopen(dest, "wb")) == NULL) {
        printf("Cannot open output file.\n");
        exit(1);
    }

    while (!feof(in)) {
        ch=getc(in);
        if (ferror(in))
        {
            printf("Read error\n");
            clearerr(in);
            break;
        }
        else
        {
            if (!feof(in)) putc(ch,out);
            if (ferror(out)) {
                printf("Write error\n");
                clearerr(out);
                break;
            }
        }
    }

    fclose(in);
    fclose(out);
}

int bring_it(){
    // read tmp file for main() to call do_copy with...
    FILE *fp;
    char line[4096];
    char cmd[4096];
    int status;

    if ((fp=fopen(tmp_file,"r"))==NULL){
        if (DEBUG) printf("cannot open %s\n",tmp_file);
        return 0; 
    } else {
        while(!feof(fp)){
            if(fgets(line,sizeof(line),fp)){
                printf("copying file %s.\n",line);
                sprintf(cmd,"/usr/bin/cp %s .",line);
                if (DEBUG) printf("[debug] [bi] [cmd] %s.\n",cmd);
                status=system(cmd);
                if (status != 0) 
                    printf ("error: copy failed.\n");
                else
                {
                    sprintf(cmd,"/usr/bin/rm %s",tmp_file);
                    status=system(cmd);
                    if (DEBUG) printf ("[debug] copy completed successfully\n");
                }
            }
        }
        fclose(fp);
        return 0;
    }
    return 1;
}

int getWorkingDir(char *val) {
    char cwd[4096];
    if (getcwd(cwd,sizeof(cwd)) != NULL) {
        strcpy(val,cwd);
        return 0;
    }
    return 1;
}
