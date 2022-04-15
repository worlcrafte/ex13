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

void cp_file (char *src, char *tgt){
    int fd_source,fd_cible;
        int nb_car_lu;
        char buf[TAILLE_BUF];
        /* test si parametres correctes */
        /* test ouverture fichier source */
        if (( fd_source = open(src, O_RDONLY))== -1)
                perror("Ne peux ouvrir ");
        /* test ouverture en création/écrasement fichier cible 
            usage de open équivalent a creat */
        if (( fd_cible = open(tgt, O_WRONLY|O_CREAT|O_TRUNC, PMODE)) == -1)
                perror("Ne peux creer ");
        /* On copie donc */
        while (( nb_car_lu = read(fd_source, buf, TAILLE_BUF)) >0) 
              if ( write(fd_cible, buf, nb_car_lu) != nb_car_lu)
                   perror("Erreur Ecriture en cour de copie");

        cp_mode(src,tgt);
}

int main(int argc, char *argv[])
{
    if (argc !=3){
        perror("invalide argument");
    }
    cp_file(argv[1],argv[2]);
    return 0;
}
