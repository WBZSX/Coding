#include<pthread.h>
#include<sys/eventfd.h>
#include<stdio.h>
#include<stdlib.h>
#include<poll.h>


//变量声明和定义
pthread_t thread_id;
pthread_mutex_t mutex;
typedef struct pollfd threadEvent;
threadEvent* pevent;
int share_variable = 0;

static threadEvent* thread_event_init()
{
    threadEvent *pevent = (threadEvent*)malloc(sizeof(threadEvent));
    int event_fd = eventfd(0, 0);
    if(-1 == event_fd)
    {
        printf("create event fail.\n");
        return NULL;
    }

    pevent->fd = event_fd;
    pevent->events = POLLIN;

    return pevent;
}

static int thread_event_set(threadEvent* pEvent)
{
    if(NULL == pEvent)
    {
        printf("this fd is invalid!");
        return 0;
    }

    long long data = 1;
    write(pEvent->fd, &data, sizeof(long long));
}

static int thread_event_wait(threadEvent* pEvent, int msec)
{
    if(NULL == pEvent)
    {
        printf("this fd is invalid!");
        return 0;
    }

    int rel = poll(pEvent, 1, msec);
    if(-1 == rel)
    {
        printf("wait event fail.\n");
        return 0;
    }

    long long data = 0;
    read(pEvent->fd, &data, sizeof(long long));
    return 0;
}

static void* thread_func(void *pMsg)
{
    while(1)
    {
        thread_event_wait(pevent, 0);
        pthread_mutex_lock(&mutex);
        printf("the child thread:%d\n", share_variable);
        share_variable++;

        if(share_variable > 100)
        {
            break;
        }
        pthread_mutex_unlock(&mutex);
        sleep(2);
        thread_event_set(pevent);
    }
}

int main()
{ 
    //变量初始化
    pthread_mutex_init(&mutex, NULL);
    pevent = thread_event_init();

    //线程属性初始化
    pthread_attr_t thread_attr;
    pthread_attr_init(&thread_attr);
    pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_DETACHED);

    //线程创建
    int Errcode = 0;
    Errcode = pthread_create(&thread_id, &thread_attr, thread_func, NULL);
    while(1)
    {
        sleep(1);
        pthread_mutex_lock(&mutex);
        printf("the main thread: %d\n", share_variable);
        share_variable++;

        if(share_variable > 100)
        {
            break;
        }
        pthread_mutex_unlock(&mutex);
        thread_event_set(pevent);
    }

    return 0;
}