#include"thread_pool.h"
#include<stdio.h>
#include<stdlib.h>
//#include<stdatomic.h>
#include<atomic>
#include<pthread.h>
//手动实现简单的一个自旋锁，自旋锁就是如果锁资源未被释放，那么一直尝试获取锁资源，也就是忙等待，适合短时的操作
struct __spinlock_s{
    //atomic_flag atmc_flg;
    std::atomic_flag atmc_flg;
};

//线程池首先需要定义任务，然后就是任务队列，最后就是线程池
struct __task_s{
    //任务对象中必须要有任务处理函数，下一个任务节点的地址，参数
    void* next_task_ptr;
    void* task_args;
    handler_function handle_func_ptr;
};
//在这个任务队列结构体中，记住多线程是来操作这个任务队列的，也就是说应该对这个任务队列的成员变量进行线程同步保护
struct __task_queue_s{
    //任务队列中需要有头尾两个指针，由于任务队列的职责是管理线程对任务的提取，所以必须将职责划分明确，任务队列就要实现线程对任务操作的同步\
    所以需要一个锁和一个条件变量
    void* queue_head;
    void** queue_tail;
    int block;
    //可能还需要一把自旋锁
    spinlock_t spinlock;
    pthread_cond_t condition;
    pthread_mutex_t mutex;
};

struct __thread_pool_s{
    //线程池是对线程和任务队列的管理，就是将多个线程与任务队列进行交互,所以线程池需要一个任务队列，一个线程集合，顺便记录传入参数的线程数量
    task_queue_t* task_queue;
    pthread_t* threads;
    unsigned int thread_count;   
    
    //可能还需要一个终止原子变量标识
    std::atomic_int quit;
};



void spinlock_init(spinlock_t* spinlock);
void spinlock_lock(spinlock_t* spinlock);
void spinlock_unlock(spinlock_t* spinlock);
task_queue_t* taskqueue_create();
void taskqueue_destory(task_queue_t* tsk_que);
bool threads_create(thread_pool_t* thread_pool,size_t thrd_count);
void threads_create_terminate(thread_pool_t* thread_pool);
int add_task(task_queue_t* tsk_que,void* tsk);
void* pop_task(task_queue_t* tsk_que);
void* get_task(task_queue_t* tsk_que);
void* func_thread_worker(void* args);
void nonblock(task_queue_t* tsk_que);

void spinlock_init(spinlock_t* spinlock){
    //atomic_flag_clear(&spinlock->atmc_flg);
    spinlock->atmc_flg.clear(std::memory_order_release);
}
void spinlock_lock(spinlock_t* spinlock){
    while(/*atomic_flag_test_and_set(&spinlock->atmc_flg)*/spinlock->atmc_flg.test_and_set(std::memory_order_acquire)){

    }
}
void spinlock_unlock(spinlock_t* spinlock){
    //atomic_flag_clear(&spinlock->atmc_flg);
    spinlock->atmc_flg.clear(std::memory_order_release);
}
//通知所有阻塞的工作线程准备结束任务
void nonblock(task_queue_t* tsk_que){
    spinlock_lock(&tsk_que->spinlock);
    tsk_que->block=0;
    spinlock_unlock(&tsk_que->spinlock);
    pthread_cond_broadcast(&tsk_que->condition);
}
//任务队列的创建
task_queue_t* taskqueue_create(){
    task_queue_t* task_queue=(task_queue_t*)malloc(sizeof(task_queue_t));
    if(task_queue){
        //在像这种创建资源式的逻辑中使用回滚式代码
        //回滚式代码就是考虑如果资源创建成功，那么接下来做完该做的动作后紧接着就是销毁if()中的资源
        int ret=pthread_mutex_init(&task_queue->mutex,NULL);
        if(ret==0){
            //如果成功初始化mutex，那么接下来做完应该做的动作后就是销毁mutex这个资源
            ret=pthread_cond_init(&task_queue->condition,NULL);
            if(ret==0){
                //如果成功初始化condition，那么接下来做完应该做的动作后就是销毁condition这个资源
                spinlock_init(&task_queue->spinlock);
                task_queue->block=1;
                task_queue->queue_head=NULL;
                task_queue->queue_tail=&task_queue->queue_head;
                return task_queue;
                
                
                //由于最内层if要做的事就是返回创建出来的task_queue，所以正常情况下永远也不会执行下一行代码，所以可以直接注释掉
                //pthread_cond_destroy(&task_queue->condition);
            }
            pthread_mutex_destroy(&task_queue->mutex);
        }
        
        free(task_queue);
        task_queue=NULL;
    }
    return NULL;
}
//任务队列的销毁
void taskqueue_destory(task_queue_t* tsk_que){
    if(!tsk_que)return;
    //取出一个一个的task*然后free掉
    task_t* task;
    while(task=(task_t*)pop_task(tsk_que)){
        free(task);
    }
    //destory tsk_que本身
    pthread_mutex_destroy(&tsk_que->mutex);
    pthread_cond_destroy(&tsk_que->condition);
    free(tsk_que);
    tsk_que=NULL;
}


//thread_pool中threads线程数组的创建
bool threads_create(thread_pool_t* thread_pool,size_t thrd_count){
    if(!thread_pool)return false;
    pthread_attr_t pthrd_attr;
    int ret=pthread_attr_init(&pthrd_attr);
    if(ret==0){
        thread_pool->threads=NULL;
        thread_pool->threads=(pthread_t*)malloc(sizeof(pthread_t)*thrd_count);
        if(thread_pool->threads){
            size_t i=0;
            for(;i<thrd_count;i++){
                
                pthread_create(&thread_pool->threads[i],&pthrd_attr,func_thread_worker,thread_pool);
            }
            thread_pool->thread_count=i;
            if(i==thrd_count){
                
                return true;
            }
            threads_create_terminate(thread_pool);
            free(thread_pool->threads);
            thread_pool->threads=NULL;
        }
        pthread_attr_destroy(&pthrd_attr);
    }
    return false;
}
//如果在threads创建过程中有thread创建失败，那么调用threads_create_terminate等待已创建threads执行完毕\
最后在threads_create调用free(threads)将threads这块内存释放

void threads_create_terminate(thread_pool_t* thread_pool){
    if(!thread_pool)return;
    //如果进入threads_create_terminate这个函数，表明整个线程创建过程出错，需要立即停止整个线程池的工作，所以设置这个结束标志
    atomic_store(&thread_pool->quit,1);
    nonblock(thread_pool->task_queue);
    for(int i=0;i<thread_pool->thread_count;i++){
        pthread_join(thread_pool->threads[i],NULL);
    }
}
//线程池的创建
thread_pool_t* threadpool_create(unsigned int cnt){
    thread_pool_t* thread_pool=(thread_pool_t*)malloc(sizeof(thread_pool_t));
    if(thread_pool){
        task_queue_t* task_queue=taskqueue_create();
        if(task_queue){
            thread_pool->task_queue=task_queue;
            
            if(threads_create(thread_pool,cnt)){
                atomic_init(&thread_pool->quit,0);
                return thread_pool;
            }
            taskqueue_destory(task_queue);
        }
        free(thread_pool);
        thread_pool=NULL;
    }
    return NULL;
}
//线程池的销毁
void threadpool_destory(thread_pool_t* thread_pool){
    if(!thread_pool)return;
    
    threads_create_terminate(thread_pool);
    free(thread_pool->threads);
    thread_pool->threads=NULL;
}
//工作线程的任务函数
void* func_thread_worker(void* args){
    thread_pool_t* thread_pool=(thread_pool_t*)args;
    task_t* task;
    
    while(atomic_load(&thread_pool->quit)==0){
        //这些工作子线程就需要一直循环取任务然后执行
        
        task=(task_t*)get_task(thread_pool->task_queue);
        spinlock_lock(&thread_pool->task_queue->spinlock);
        pthread_t threadId=pthread_self();
        printf("-----------------\n");
        printf("thread:%ld get a task\n",(long unsigned int)threadId);
        if(!task){
            spinlock_unlock(&thread_pool->task_queue->spinlock);
            break;
        }
        task->handle_func_ptr(task->task_args);
        printf("----------------\n");
        spinlock_unlock(&thread_pool->task_queue->spinlock);
        free(task);
        task=NULL;
    }
    return NULL;
}
//将任务从线程池踢出去
void* pop_task(task_queue_t* tsk_que){
    if(!tsk_que)return NULL;
    spinlock_lock(&tsk_que->spinlock);
    if(tsk_que->queue_head==NULL){
        spinlock_unlock(&tsk_que->spinlock);
        return NULL;
    }
    void* task=tsk_que->queue_head;
    void** link=(void**)task;
    tsk_que->queue_head=*link;
    if(tsk_que->queue_head==NULL){
        tsk_que->queue_tail=&tsk_que->queue_head;
    }
    spinlock_unlock(&tsk_que->spinlock);
    return task;
}
//从任务队列中得到一个任务实例
void* get_task(task_queue_t* tsk_que){
    if(!tsk_que)return NULL;
    spinlock_lock(&tsk_que->spinlock);
    int i=0;
    if(tsk_que->queue_head){
        //printf("get_task successful\n");
    }
    else{
        //printf("get_task failed\n");
    }
    /*while(tsk_que->queue_head){
        i++;
    }*/
    spinlock_unlock(&tsk_que->spinlock);
    //printf("task cnt:%d\n",i);
    void* task;
    while((task=pop_task(tsk_que))==NULL){
        pthread_mutex_lock(&tsk_que->mutex);
        if(tsk_que->block==0){
            pthread_mutex_unlock(&tsk_que->mutex);
            return NULL;
        }
        pthread_cond_wait(&tsk_que->condition,&tsk_que->mutex);
        pthread_mutex_unlock(&tsk_que->mutex);
    }
    return task;
}
//为线程池的任务队列添加任务
int task_push(thread_pool_t* thread_pool,handler_function func,void* args){
    if(!thread_pool)return -1;
    if(atomic_load(&thread_pool->quit)==1)return -1;
    
    task_t* task=(task_t*)malloc(sizeof(task_t));
    if(!task)return -1;
    task->handle_func_ptr=func;
    task->task_args=args;
    add_task(thread_pool->task_queue,task);
    return 0;
}
//具体的添加任务函数
int add_task(task_queue_t* tsk_que,void* tsk){
    if(!tsk_que||!tsk)return -1;
    printf("enter add_task\n");
    void** link=(void**)tsk;
    *link=NULL;
    spinlock_lock(&tsk_que->spinlock);
    
    *tsk_que->queue_tail=link;
    tsk_que->queue_tail=link;
    int i=0;
    task_t* ptr=(task_t*)(tsk_que->queue_head);
    while(ptr){
        ptr=(task_t*)(ptr->next_task_ptr);
        i++;
    }
    printf("add_task i:%d\n",i);
    spinlock_unlock(&tsk_que->spinlock);
    //将任务添加进任务队列后记得通知工作线程取出任务执行
    pthread_cond_signal(&tsk_que->condition);
    return 0;
}
void threadpool_waitdown(thread_pool_t* thread_pool){
    if(!thread_pool)return;
    int cnt=thread_pool->thread_count;
    nonblock(thread_pool->task_queue);
    for(int i=0;i<cnt;i++){
        pthread_join(thread_pool->threads[i],NULL);
    }
}