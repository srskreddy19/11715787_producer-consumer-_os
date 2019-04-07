#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>
typedef struct
{
    int buf[5];   
    int in;              
    int out;              
    sem_t full;           
    sem_t empty;          
    
    pthread_mutex_t mutex;          
} sbuf_t;

sbuf_t shared;

int* arg;
void *Producer(void *arg)
{
    int i, item, index;
    

    index = *((int *)arg);
    

    

    for (i=0; i < 4; i++)
    {
        
        item = i;
        
        sem_wait(&shared.empty);
        pthread_mutex_lock(&shared.mutex);
        shared.buf[shared.in] = item;
        shared.in = (shared.in+1)%5;
        printf("[P%d] Producing %d ...\n", index, item);
        fflush(stdout);
        pthread_mutex_unlock(&shared.mutex);
        sem_post(&shared.full);
        if (i % 2 == 1) sleep(1);
    }
    return NULL;
}

void *Consumer(void *arg)
{
    int i, item, index;
    

    index = *((int *)arg);
    for (i=4; i > 0; i--) {
        sem_wait(&shared.full);
        pthread_mutex_lock(&shared.mutex);
        item=i;
        item=shared.buf[shared.out];
        shared.out = (shared.out+1)%5;
        printf("[C%d] Consuming  %d ...\n", index, item);
        fflush(stdout);
        pthread_mutex_unlock(&shared.mutex);
        sem_post(&shared.empty);
        if (i % 2 == 1) sleep(1);
    }
    return NULL;
}

int main()
{
    pthread_t idP, idC;
    int index;
    

    sem_init(&shared.full, 0, 0);
    sem_init(&shared.empty, 0, 5);
    pthread_mutex_init(&shared.mutex, NULL);
    for (index = 0; index < 3; index++)
    {
        pthread_create(&idP, NULL, Producer, &index);
    }
    for(index=0; index<3; index++)
    {
        pthread_create(&idC, NULL, Consumer, &index);
    }
    pthread_exit(NULL);
}
