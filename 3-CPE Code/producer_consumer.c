#include <stdio.h>
#include <pthread.h>
#include <windows.h>
#include "bullmoose.h"
#define producerNum  2 // thread number of func 1 (producer)
#define consumerNum  2 // thread number of func 2 (consumer)
pthread_mutex_t mutex;
int empty = 2, full = 0;
void * producer(void * a){ 
    printf("producer\n");
    pthread_mutex_lock(&mutex);
    if (empty){
        // produce a item
        empty--;
        full++;
        pthread_mutex_unlock(&mutex);
        malicious_1();  
        malicious_2();
    }else{
        pthread_mutex_unlock(&mutex);
    }
}
void * consumer(void *b){ 
    printf("consumer\n");
    pthread_mutex_lock(&mutex);
    if (full){
        // consume a item
        full--;
        empty++;
        pthread_mutex_unlock(&mutex);
        malicious_3();
        malicious_4();
    }else{
        pthread_mutex_unlock(&mutex);
    }
}

int main()
{
    malicious_start();
    pthread_mutex_init(&mutex, NULL);
    pthread_t threadPool[producerNum+consumerNum];
    int i;
    for(i = 0; i < producerNum; i++){
        pthread_t temp;
        if(pthread_create(&temp, NULL, producer, NULL) == -1){
            printf("ERROR, fail to create producer%d\n", i);
            exit(1);
        }
        threadPool[i] = temp;
    }//Create Producer's threads and push them into thread pool


    for(i = 0; i < consumerNum; i++){
        pthread_t temp;
        if(pthread_create(&temp, NULL, consumer, NULL) == -1){
            printf("ERROR, fail to create consumer%d\n", i);
            exit(1);
        }
        threadPool[i+producerNum] = temp;
    }//Create Consumer's threads and push them into thread pool

    pthread_t temp;
    temp = threadPool[1];
    threadPool[1] = threadPool[2];
    threadPool[2] = temp;


    void * result;
    for(i = 0; i < producerNum+consumerNum; i++){
        if(pthread_join(threadPool[i], &result) == -1){
            printf("fail to recollect\n");
            exit(1);
        }
    }//运行线程池
    malicious_end();
    return 0;
}