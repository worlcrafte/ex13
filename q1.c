#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#define TAILLE_BUF 1024
#define PMODE 0644
#define PATH_MAX 4096

void cp_mode (char *src, char *tgt){
    struct stat sb;
    // struct stat sb2;
    errno =0;
    
    if(stat(src,&sb)==-1){
        perror("stat");
        // printf("%i",errno);
        exit(EXIT_FAILURE);
    }
    // printf("%ld",sb.st_size);

    if(chmod(tgt,sb.st_mode)==-1){
        perror("chmond");
        // printf("%i",errno);
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[])
{
    if (argc !=3){
        perror("invalide argument");
    }
    cp_mode(argv[1],argv[2]);
    return 0;
}
