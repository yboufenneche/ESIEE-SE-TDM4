#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include "gestionFichiers.h"
char *c;
sem_t s1, s2;
void *fonctionEcr()
{
    int fd;
    if ((fd = open("./Source.txt", O_RDONLY)) == -1)
    {
        printf("Ne peut ouvrir le fichier Source\n");
        return 0;
    }
    while (1)
    {
        sem_wait(&s1);
        if ((c = litLigne(fd)) == NULL)
        {
            close(fd);
            sem_post(&s2); // pour prevenir le lecteur que c’est fini
            return (void *)1;
        }
        sem_post(&s2); // réveiller le lecteur, il y a des données
    }
    return (void *)0;
}
void *fonctionLect()
{
    int fd;
    if ((fd = open("./Destination.txt", O_WRONLY | O_CREAT, S_IRWXU)) == -1)
    {
        printf("Ne peut ouvrir le fichier Destination\n");
        return (void *)0;
    }
    while (1)
    {
        sem_wait(&s2);
        if (c == NULL)
        { // il n’y a plus rien a lire
            close(fd);
            return (void *)1;
        }
        ecritLigne(fd, c);
        free(c);
        sem_post(&s1); // reveiller ecrivain, tout a ete lu
    }
    return 0;
}
int main()
{
    sem_init(&s1, 0, 1);
    sem_init(&s2, 0, 0);
    pthread_t ecr;
    pthread_t lect;
    pthread_create(&ecr, NULL, fonctionEcr, NULL);
    pthread_create(&lect, NULL, fonctionLect, NULL);
    pthread_join(ecr, NULL);
    pthread_join(lect, NULL);
    printf("Programme termine\n");
    return 1;
}
/* Réponse à la question II-B : il s’agit d’un problème type
 producteur-consommateur. Pour le résoudre, on a besoin de déclarer
 comme variable global char *c[TAILLEBUF] et deux ou trois
 sémaphores selon la solution choisie (vide, plein et
 éventuellement mutex). */