#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

struct args
{
    int id;
    int *count;
    int *suc;
    int *buffer;
    int *flag;
    pthread_mutex_t mutex;
};

void *do_work1(void *arg);
void *do_work2(void *arg);

int main()
{
    pthread_t threads[5];
    struct args *arguments[5];
    pthread_mutex_t mutex1, mutex2;
    int buffer1[3], buffer2[3];
    int count1[1] = {0};
    int count2[1] = {0};
    int suc1[1] = {0};
    int suc2[1] = {0};
    int flag[1] = {0};
    
    pthread_mutex_init( &mutex1, NULL );
    pthread_mutex_init( &mutex2, NULL );
    for(int i = 0; i < 3; i++)
    {
        buffer1[i] = 0;
        buffer2[i] = 0;
    }
    
    for(int i = 0; i < 6; i++)
    {
        arguments[i] = (struct args *)calloc(1, sizeof(struct args));
        arguments[i]->id = i + 1;
        arguments[i]->flag = flag;
        if(i < 3)
        {
            arguments[i]->count = count1;
            arguments[i]->suc = suc1;
            arguments[i]->buffer = buffer1;
            arguments[i]->mutex = mutex1;
            pthread_create(&threads[i], NULL, do_work1, (void *)arguments[i]);
        }
        else
        {
            arguments[i]->count = count2;
            arguments[i]->suc = suc2;
            arguments[i]->buffer = buffer2;
            arguments[i]->mutex = mutex2;
            pthread_create(&threads[i], NULL, do_work2, (void *)arguments[i]);
        }
    }
    
    for(int i = 0; i < 5; i++)
    {
        pthread_join(threads[i], NULL);
    }
    
    printf("Total sequences generated team1: %d\n", count1[0]);
    printf("Number of correct sequences team1: %d\n", suc1[0]);
    
    printf("Total sequences generated team2: %d\n", count2[0]);
    printf("Number of correct sequences team2: %d\n", suc2[0]);
}

void *do_work1(void *arg)
{
    struct args *argu = (struct args *)arg;
    int id = argu->id;
    int *count = argu->count;
    int *suc = argu->suc;
    int *buffer = argu->buffer;
    int *flag = argu->flag;
    pthread_mutex_t mutex = argu->mutex;
    int index = 0;
    
    while(suc[0] < 10 && flag[0] == 0)
    {
        usleep(50000);
        pthread_mutex_lock(&mutex);
        
        if(suc[0] >= 10 || flag[0] > 0)
        {
            pthread_mutex_unlock(&mutex);
            break;
        }
        
        for(index = 0; index < 3; index++)
        {
            if(buffer[index] == id)
            {
                index = -1;
                break;
            }
            if(buffer[index] == 0)
            {
                printf("My id: %d\n", id);
                buffer[index] = id;
                break;
            }
        }
        
        if(index == 2)
        {
            count[0] = count[0] + 1;
            if(buffer[0] == 1 && buffer[1] == 2 && buffer[2] == 3)
            {
                printf("123\n");
                suc[0] = suc[0] + 1;
            }
            buffer[0] = 0;
            buffer[1] = 0;
            buffer[2] = 0;
        }
        
        pthread_mutex_unlock(&mutex);
        usleep(50000);
    }
    if(flag[0] == 0)
    {
        printf("Team 1 won!\n");
        flag[0] = 1;
    }
}

void *do_work2(void * arg)
{
    struct args *argu = (struct args *)arg;
    int id = argu->id;
    int *count = argu->count;
    int *suc = argu->suc;
    int *buffer = argu->buffer;
    int *flag = argu->flag;
    pthread_mutex_t mutex = argu->mutex;
    int index = 0;
    
    while(suc[0] < 10 && flag[0] == 0)
    {
        usleep(50000);
        pthread_mutex_lock(&mutex);
        
        if(suc[0] >= 10 || flag[0] > 0)
        {
            pthread_mutex_unlock(&mutex);
            break;
        }
        
        for(index = 0; index < 3; index++)
        {
            if(buffer[index] == id)
            {
                index = -1;
                break;
            }
            if(buffer[index] == 0)
            {
                printf("My id: %d\n", id);
                buffer[index] = id;
                break;
            }
        }
        
        if(index == 2)
        {
            count[0] = count[0] + 1;
            if(buffer[0] == 4 && buffer[1] == 5 && buffer[2] == 6)
            {
                printf("456\n");
                suc[0] = suc[0] + 1;
            }
            buffer[0] = 0;
            buffer[1] = 0;
            buffer[2] = 0;
        }
        
        pthread_mutex_unlock(&mutex);
        usleep(50000);
    }
    
    if(flag[0] == 0)
    {
        printf("Team 2 won!\n");
        flag[0] = 1;
    }
}
