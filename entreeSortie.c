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

//fct qui permet de copié de manière récursive des directories 
void cp_dir(char *src, char* tgt){
    DIR *d=opendir(src);
    struct stat sb;
    // struct stat sg;
    struct dirent *dir;
    char pathTgt[PATH_MAX];
    char pathSrc[PATH_MAX];
    if(strncpy(pathSrc,src,strlen(src)+1)==NULL){
        perror("strncpy");
        exit(EXIT_FAILURE);
    }
    if(strcat(pathSrc,"/")==NULL){//le strcat et strcpy me permet de me place dans le fichier "racine" càd src/
        perror("strcat");
        exit(EXIT_FAILURE);
    }
    if(strncpy(pathTgt,tgt,strlen(tgt)+1)==NULL){//ici on créer une chaine de caractère qu'on manipulera à l'avenir. Ce sera notre chemin
        perror("strncpy");
        exit(EXIT_FAILURE);
    }
    // printf("path:%s\n",pathSrc);
    // char *resolved_path=malloc(4096*sizeof(char*));
    errno=0;
    // char s[PATH_MAX];
    if (d)
    {
        while ((dir = readdir(d)))
        {
            if (dir->d_name[0] !='.'){//on ne veut pas les fichier parent et des fiechier commençant par un .
                // printf("d_name:%s\n",dir->d_name);
                if (strcat(pathSrc,dir->d_name)== NULL){//ici on aura un chemin de la sorte src/dir->d_name
                    
                    perror("strcat");
                    exit(EXIT_FAILURE);
                }
                // printf("pathSrc->%s\n",pathSrc);
                if (stat(pathSrc,&sb)==-1){
                    perror("stat");
                    exit(EXIT_FAILURE);
                }
                if( 16895 == sb.st_mode){//je vérifie si c'est une directory
                    // printf("ok\n");
                    if(strcat(pathTgt,"/")==NULL){
                        perror("strcat");
                        exit(EXIT_FAILURE);
                    }
                    if(strcat(pathTgt,dir->d_name)==NULL){//si oui pathTgt sera de la forme tgt/dir->d_name
                        // printf("e");
                        perror("strcat");
                        exit(EXIT_FAILURE);
                    }
                    
                    // printf("pathTgt%s\n",pathTgt);
                    // printf("pathSrc->%s\n",pathSrc);
                    
                    if(mkdir(pathTgt,0777)){//on créer notre fichier dir->d_name dans l'emplacement tgt
                        perror("mkdir");
                        exit(EXIT_FAILURE);
                    }
                    cp_mode(pathSrc,pathTgt);//on copie les permissions
                    cp_dir(pathSrc,pathTgt); // on rappelle la fonction tant qu'on tombe sur un fichier donc notre nveau tgt aura la forme de tgt/dir->d_name (de même pour src)
                    if(strncpy(pathTgt,tgt,strlen(tgt)+1)==NULL){//on remet pathTgt à l'initiale càd tgt
                        perror("strncpy");
                        exit(EXIT_FAILURE);
                    }
                }
                // }else if( S_ISLNK (sb.st_mode)){
                    
                // 
                // }
                else{//on est dans le cas d'un document 
                    if(strcat(pathTgt,"/")==NULL){
                        perror("strcat");
                        exit(EXIT_FAILURE);
                    }
                    if (strcat(pathTgt,dir->d_name)==NULL){//de même que précédament 
                        perror("strcat");
                        exit(EXIT_FAILURE);
                    }
                    // printf("Pathtgt-> %s\n",pathTgt);
                    if(creat( pathTgt, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH )==-1){//on créer notre document
                        perror("creat");
                        exit(EXIT_FAILURE);
                    }
                    cp_file(pathSrc,pathTgt); // on copie les info et les permissions 
                    // printf("dname->%s\n",dir->d_name);
                    if(strncpy(pathSrc,src,strlen(src)+1)==NULL){//on rement pathSrc à src
                        perror("strncpy");
                        exit(EXIT_FAILURE); 
                    }
                    // if(strcat(pathSrc,"/")==NULL){
                    //     perror("strcat");
                    //     exit(EXIT_FAILURE);
                    // }
                    // printf("<<%s\n",pathSrc);
                }
                // printf("src %s\n",src);
                // printf("<<%s\n",pathSrc);
                // on les rements à leurs valeurs initiale
                if(strncpy(pathSrc,src,strlen(src)+1)==NULL){
                    perror("strncpy");
                    exit(EXIT_FAILURE);
                }
               if( strcat(pathSrc,"/")==NULL){
                   perror("strcat");
                   exit(EXIT_FAILURE);
               }
                if(strncpy(pathTgt,tgt,strlen(tgt)+1)==NULL){
                    perror("strncpy");
                    exit(EXIT_FAILURE);
                }
                // printf("<<%s\n",pathSrc);
            }
            
        }
    }   

    closedir(d);
}


int main(int argc, char *argv[])
{
    if (argc !=3){
        perror("invalide argument");
    }
    cp_dir(argv[1],argv[2]);
    return 0;
}
