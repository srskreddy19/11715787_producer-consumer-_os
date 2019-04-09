#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>
#include<pthread.h>
typedef struct
{
    int buffer[5];   
    int inner;              
    int outter;              
    sem_t full;           
    sem_t empty;          
    
    pthread_mutex_t mutex;          
} sbuffer_v;

sbuffer_v shared;

int* arg;
void *Prod(void *arg)
{
    int k, item1, index1;
    

    index1 = *((int *)arg);
    

    

    for (k=0; k < 4; k++)
    {
        
        item1 = k;
        
        sem_wait(&shared.empty);
        pthread_mutex_lock(&shared.mutex);
        shared.buffer[shared.inner] = item1;
        shared.inner = (shared.inner+1)%20;
        printf(" Producing %d ...\n", index1, item1);
        fflush(stdout);
        pthread_mutex_unlock(&shared.mutex);
        sem_post(&shared.full);
        if (k % 2 == 1) sleep(1);
    }
    return NULL;
}

void *Consu(void *arg)
{
    int k, item2, index2;
    

    index2 = *((int *)arg);
    for (k=4; k > 0; k--) {
        sem_wait(&shared.full);
        pthread_mutex_lock(&shared.mutex);
        item2=k;
        item2=shared.buffer[shared.outter];
        shared.outter= (shared.outter+1)%20;
        printf("Consuming  %d ...\n", index2, item2);
        fflush(stdout);
        pthread_mutex_unlock(&shared.mutex);
        sem_post(&shared.empty);
        if (k % 2 == 1) sleep(1);
    }
    return NULL;
}

int main()
{
    pthread_t idProd, idConsu;
    int indexZ;
    

    sem_init(&shared.full, 0, 0);
    sem_init(&shared.empty, 0, 5);
    pthread_mutex_init(&shared.mutex, NULL);
    for (indexZ = 0; indexZ < 3; indexZ++)
    {
        pthread_create(&idProd, NULL, Prod, &indexZ);
    }
    for(indexZ=0; indexZ<3; indexZ++)
    {
        pthread_create(&idConsu, NULL,Consu,&indexZ);
    }
    pthread_exit(NULL);
}
