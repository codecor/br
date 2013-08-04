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

// global storage file for files currently being carried (brought?) 
char *tmp_file="/tmp/bringing";
int DEBUG=1;

main (int argc,char *argv[]){
    //printf("args: %i\narg[1]: %s\n",argc,argv[1]);

    // if one, store
    if (argc==2) {
        char file_name[]="file not available";
        char ex_path[1024];
        char file_addr[1024];
        FILE *fp;

        // get file path
        getExePath(&ex_path);
        if (DEBUG) printf("[debug] ex_path = %s\n",ex_path);

        /* lets handle more than 1 file */
        for (len
        return 0;
        strcpy(file_name,argv[1]);
        if (DEBUG) printf("[debug] file_name = %s\n",file_name);

        if (!access(file_name,F_OK)) // file doesn't exist then give usage and exit
        {
            if (DEBUG) printf("file exists.\n");
        } else {
            if (DEBUG) printf("error: file does not exist.\n");
            usage();
            return 1;
        }

        sprintf(file_addr,strcat(ex_path,file_name));
        printf("[debug] file_addr=%s\n",file_addr);

         // check for file
        if ((fp=fopen(tmp_file, "wb")) == NULL) {
            printf("Cannot open temporary file.\n");
            exit(1);
        }
        
        // write tmp addr holder file
        fprintf(fp,file_addr);
        printf("bringing %s...\n",argv[1]);
        fclose(fp);
    } else {
    // if none, drop
        // check for file waiting to be dropped
        if (bring_it()){
            //sprintf(cmd,"cp %s .",
        } else {
            // error: no file to be dropped
            printf("No file to be brought.\n");
        }
    }
    //do_bring(argc,argv[1],argv[2]);

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

int getExePath(char *str){
    char new_str[100];
    int len;
    char path[1024];
    char* slash;

    // read symbolic link /proc/self/exe
    len = readlink("/proc/self/exe",path,sizeof(path));
    if (len==-1)
        return 0;

    path[len]='\0';
    //printf("[getExePath] path=%s\n",path);

    // get the directory in the path by stripping exe name
    slash = strrchr(path,'/');
    if (!slash || slash==path)
        return 0;

    slash+=1;
    *slash='\0';

    strcpy(str,path);
    //printf("[getExePath] str=%s\n",str);
    return 1;
}

int bring_it(){
    // read tmp file for main() to call do_copy with...
    FILE *fp;
    char line[300];
    char cmd[300];
    int status;

    if ((fp=fopen(tmp_file,"r"))==NULL){
        if (DEBUG) printf("cannot open %s\n",tmp_file);
        return 0; 
    } else {
        while(!feof(fp)){
            if(fgets(line,sizeof(line),fp)){
                printf("copying file %s.\n",line);
                sprintf(cmd,"/usr/bin/cp %s .",line);
                if (DEBUG) printf("[d] [bi] [cmd] %s.\n",cmd);
                status=system(cmd);
                if (status != 0) 
                    printf ("error: copy failed.\n");
                else
                {
                    sprintf(cmd,"/usr/bin/rm %s",tmp_file);
                    status=system(cmd);
                    if (DEBUG) printf ("copy completed successfully\n");
                }
            }
        }
        fclose(fp);
        return 1;
    }
}

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
