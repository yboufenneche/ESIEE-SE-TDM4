/* NB : Je n’attends pas que les étudiants produisent une solution
 générique comme celle-ci. Ils sont aussi autorisés à utiliser des
 variables globales (ce que je me suis interdit ici). Par contre, je
 vous demande de demander aux étudiants qui auraient terminé en
 avance de modifier leur programme afin de :
 - supprimer les variables globales (ceci implique le passage de
 plusieurs arguments aux threads)
 - rendre le code paramétrable (nbre d’affichage et nbre de threads
 créés). */
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#define N 3
#define M 10
// Structure permettant de passer plusieurs paramètres aux threads
typedef struct
{
    int id;
    sem_t *attente;
    sem_t *debloque;
} thread_args;
// procédure exécutée par les threads
void *Affiche(void *arg)
{
    int j;
    sem_t *attente = ((thread_args *)arg)-> attente;
    sem_t *debloque = ((thread_args *)arg)-> debloque;
    int i = ((thread_args *)arg)-> id;
    for (j = 0; j < M; j++)
    {
        sem_wait(attente);
        printf("Affichage %d du thread %d\n", j + 1, i);
        sem_post(debloque);
    }
}
int main()
{
    int i;
    pthread_t th[N];
    thread_args arg[N]; // arguments des threads
    sem_t mutex[N];     // sémaphores
                        // Initialisation des sémaphores
    sem_init(&(mutex[0]), 0, 1);
    for (i = 1; i < N; i++)
    {
        sem_init(&(mutex[i]), 0, 0);
    }
    //Initialisation des arguments des threads
    for (i = 0; i < N; i++)
    {
        arg[i].id = i + 1;
        arg[i].attente = &(mutex[i]);
        arg[i].debloque = &(mutex[(i + 1) % N]);
    }
    // Creation des threads
    for (i = 0; i < N; i++)
    {
        pthread_create(&th[i], NULL, Affiche, (void *)(&(arg[i])));
    }
    // Attente de terminaison des threads
    for (i = 0; i < N; i++)
    {
        pthread_join(th[i], NULL);
    }
    // Destruction des sémaphores
    for (i = 0; i < N; i++)
    {
        sem_destroy(&(mutex[i]));
    }
    return 0;
}