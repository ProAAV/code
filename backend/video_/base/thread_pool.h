#ifndef __THREAD_POOL_H__
#define __THREAD_POOL_H__
#include<stddef.h>
#include<stdbool.h>


//class EventLoop;

typedef void (*handler_function)(void*);
//typedef void (*handler_function)(int,EventLoop&);


typedef struct __thread_pool_s thread_pool_t;
typedef struct __task_queue_s task_queue_t;
typedef struct __task_s task_t;
typedef struct __spinlock_s spinlock_t;




thread_pool_t* threadpool_create(unsigned int cnt);
void threadpool_destory(thread_pool_t* thread_pool);
int task_push(thread_pool_t* thread_pool,handler_function func,void* args);
void threadpool_waitdown(thread_pool_t* thread_pool);


#endif