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


main (int argc,char *argv[]){
    //printf("args: %i\narg[1]: %s\n",argc,argv[1]);

    // if one, store
    if (argc==2) {
        char *tmp_file="tmp";
        char file_name[]="2bring-file-addr";
        char ex_path[1024];
        char file_addr[1024];
        FILE *fp;

        // check for file
        if ((fp=fopen(tmp_file, "wb")) == NULL) {
            printf("Cannot open temporary file.\n");
            exit(1);
        }

        // get file path
        getExePath(&ex_path);
        //printf("[debug] ex_path = %s\n",ex_path);

        strcpy(file_name,argv[1]);
        //printf("[debug] file_name = %s\n",file_name);

        //strcat(ex_path,"test");
        strcat(ex_path,file_name);
        strcpy(file_addr,ex_path);
        printf("[debug] file_addr=%s\n",file_addr);

        // write tmp addr holder file
        fprintf(fp,file_addr);
        printf("bringing %s...\n",argv[1]);
        fclose(fp);
    } else {
    // if none, drop
        // check for file waiting to be dropped
        // error: no file to be dropped
        printf("No file to be brought.\n");
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
