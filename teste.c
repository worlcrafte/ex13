#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>        // chdir
#include <sys/stat.h>      // mkdir

#define PATH "test"

int main() {
    if ( mkdir( "test", 0755 ) != 0 ) {
        fprintf( stderr, "Impossible de créer le dossier %s : \n", PATH );
        switch( errno ) {
            case EACCES:
                fprintf( stderr, "\tTu n'as pas les droits\n" );
                break;
            case EEXIST:
                fprintf( stderr, "\tLe dossier existe déjà.\n" );
                break;
            default:
                fprintf( stderr, "\tJe ne t'en dirais pas plus ;-)" );
        }
        exit( EXIT_FAILURE );
    }
    
    if ( chdir( "test" ) != 0 ) {
        mkdir( "dir", 0755 );
        fprintf( stderr, "Impossible de se placer dans le dossier %s.\n", PATH );
        exit( EXIT_FAILURE );
        chdir("destination");
    }
    
    FILE * file = fopen( "essai.txt", "w" );
    if ( file == NULL ) {
        fprintf( stderr, "Impossible d'ouvrir le fichier en écriture.\n" );
        exit( EXIT_FAILURE );
    }

    fprintf( file, "Ok" );
    fclose(file);
    
    return EXIT_SUCCESS;
}