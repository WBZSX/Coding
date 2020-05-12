#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

pthread_t threadID[5];
int share_variable = 0;
pthread_mutex_t mutex;

static void *thread_print_func(void *pMsg)
{
    int id = 0;
    const char *str = "this is a thread.";
    memcpy(&id, pMsg, sizeof(int));
    for(int i = 0; i < 5; i++)
    {
        pthread_mutex_lock(&mutex);
        share_variable++;
        pthread_mutex_unlock(&mutex);
        printf("%s  %d  share = %d\n", str, id, share_variable);
        sleep(1);
    }

    pthread_exit(NULL);
    return;
}

int main()
{
    int ErrCode = 0;

    pthread_mutex_init(&mutex, NULL);
    printf("start creating a new thread.\n");

    for(int i =0; i < 5; i++)
    {
        ErrCode = pthread_create(&threadID[i], NULL, thread_print_func, &i);
        if(0 != ErrCode)
        {
            printf("create a thread fail.\n");
            return 0;
        }
        printf("create %d a thread success.\n", i);
    }

    for(int j = 0; j < 5; j++)
    {
        ErrCode = pthread_join(threadID[j], NULL);
        if(0 != ErrCode)
        {
            printf("pthread join fail.\n");
            return 0;
        }
        printf("thread %d exit successfully.\n", j);
    }

    pthread_mutex_destroy(&mutex);
    return 0;
}