#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>

pthread_t threadID[5];
int share_variable = 0;
pthread_mutex_t mutex;
int thread_exit_flag[5] = {1};

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

    thread_exit_flag[id] = 1;
    return;
}

int main()
{
    int ErrCode = 0;

    pthread_attr_t thread_attr;

    ErrCode = pthread_attr_init(&thread_attr);
    if(0 != ErrCode)
    {
        printf("thread attr init fail.\n");
        return 0;
    }

    int scope = PTHREAD_SCOPE_SYSTEM;
    ErrCode = pthread_attr_setscope(&thread_attr, scope);
    if(0 != ErrCode)
    {
        printf("thread attr set scope fail.\n");
        return 0;
    }

    int detach = PTHREAD_CREATE_DETACHED;
    ErrCode = pthread_attr_setdetachstate(&thread_attr, detach);
    if(0 != ErrCode)
    {
        printf("thread attr set detachstate fail.\n");
        return 0;
    } 

    pthread_mutex_init(&mutex, NULL);

    printf("start creating a new thread.\n");

    for(int i =0; i < 5; i++)
    {
        ErrCode = pthread_create(&threadID[i], &thread_attr, thread_print_func, &i);
        if(0 != ErrCode)
        {
            printf("create a thread fail.\n");
            return 0;
        }
        pthread_mutex_lock(&mutex);
        thread_exit_flag[i] = 0;
        pthread_mutex_unlock(&mutex);
        printf("create %d a thread success.\n", i);
    }

#if 0
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
#endif

    while(1)
    {
        if((1 == thread_exit_flag[0])
            &&(1 == thread_exit_flag[1])
            &&(1 == thread_exit_flag[2])
            &&(1 == thread_exit_flag[3])
            &&(1 == thread_exit_flag[4])
        )
        {
            printf("all thread exit success.\n");
            break;
        }
    }

    pthread_mutex_destroy(&mutex);
    pthread_attr_destroy(&thread_attr);
    return 0;
}