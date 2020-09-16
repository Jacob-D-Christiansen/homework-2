#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

struct arguments{
    int id;
    int *buffer;
    pthread_mutex_t mutex;
};

//hello

int counter = 0;
int successes = 0;

void *do_work(void *args);

int main()
{
    int buffer[3];
    for(int i = 0; i < 3; i++)
    {
        buffer[i] = 0;
    }
    struct arguments *args0, *args1, *args2;
    pthread_t thread_0, thread_1, thread_2;
    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, NULL);
    
    args0 = (struct arguments *) calloc(1, sizeof(struct arguments));
    args1 = (struct arguments *) calloc(1, sizeof(struct arguments));
    args2 = (struct arguments *) calloc(1, sizeof(struct arguments));
    
    args0->id = 1;
    args1->id = 2;
    args2->id = 3;
    
    args0->buffer = buffer;
    args1->buffer = buffer;
    args2->buffer = buffer;
    
    args0->mutex = mutex;
    args1->mutex = mutex;
    args2->mutex = mutex;
    
    pthread_create(&thread_0, NULL, do_work, (void *)args0);
    pthread_create(&thread_1, NULL, do_work, (void *)args1);
    pthread_create(&thread_2, NULL, do_work, (void *)args2);
    
    pthread_join(thread_0, NULL);
    pthread_join(thread_1, NULL);
    pthread_join(thread_2, NULL);
    
    printf("Total sequences generated: %d\n", counter);
    printf("Number of correct sequences: %d\n", successes);
    
    printf("reeeee");
}

void *do_work(void *args)
{
    struct arguments *arguments = (struct arguments *)args;
    
    int id = arguments->id;
    int *buffer = arguments->buffer;
    pthread_mutex_t mutex = arguments->mutex;
    int index = 0;
    
    while(successes < 10)
    {
        pthread_mutex_lock(&mutex);
        while(buffer[index] != 0)
        {
            if(buffer[index] == id || index > 2)
            {
                index == 0;
                pthread_mutex_unlock(&mutex);
                usleep(50000);
                continue;
            }
            index++;
        }
        buffer[index] = id;
        printf("My id: %d\n", id);
        if(index == 2)
        {
            counter = counter + 1;
            if(buffer[0] == 1 && buffer[1] == 2 && buffer[2] == 3)
            {
                successes = successes + 1;
                printf("123\n");
            }
            buffer[0] = 0;
            buffer[1] = 0;
            buffer[2] = 0;
        }
        index = 0;
        pthread_mutex_unlock(&mutex);
        usleep(50000);
    }
}
