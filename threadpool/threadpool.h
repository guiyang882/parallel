#pragma once

#include <pthread.h>
#include <unistd.h>
#include <deque>
#include <iostream>
#include <fstream>
#include <vector>
#include <errno.h>
#include <string.h>
#include <string>
#include <map>
#include <set>

#include "mutex.h"
#include "task.h"
#include "log/log.h"

/*
 * This thread pool not manage the memory delete and free
 * Who Malloc and New, who should pay attention to the memory management !
*/

const int DEFAULT_POOL_SIZE = 10;
const int STARTED = 0;
const int STOPPED = 1;

using namespace std;

class ThreadPool {
public:
    // 采用单例模式设计线程池
    static ThreadPool* getSingleInstance();

    // 释放所有线程池中的任务，需要在这个函数推出前将多任务进行保存
    static void revokeSingleInstance();

private:
    ThreadPool();
    ~ThreadPool();

private:
    static ThreadPool* p_ThreadPool;

public:
    int initialize_threadpool();
    int destroy_threadpool();

    //主要是当线程池运行起来之后会显示增在运行的容量，如果线程池没有运行起来，则放回-1
    int runningNumbers();

    //当线程池正常创建时，返回线程池的可以接受的最大容量，当线程池没有运行起来的时候，返回-1
    int getPoolCapacity();
    int add_task(Task* task, const string& task_id);
    int fetchResultByTaskID(const string task_id, TaskPackStruct& res);

    void* execute_task(pthread_t thread_id);

private:
    volatile int m_pool_state;

    Mutex m_task_mutex;
    Mutex m_finishMap_mutex;
    Mutex m_taskMap_mutex;
    CondVar m_task_cond_var;

    // 线程池使用生产者消费者模型进行设计
    // 生产者是任务的发起者，可以产生特定的任务，放入到线程池中
    // 消费者就是worker将不断的从任务队列中选择不同的任务，去执行，将执行的结果返回
    // 任务队列，生产者将任务直接添加到队列中，如果添加的任务多，超多了规定的容量，则不会接受新的任务
    // 消费者从队列的头部不断的取数据，如果队列为空则阻塞或者怎样
    std::deque<Task*> m_tasks;

    std::vector<pthread_t> m_threads;
    std::set<pthread_t> m_run_threads;

    map<string, Task*> m_taskMap; // for <task_id, task*>
    map<string, TaskPackStruct> m_finishMap; // for <task_id, task_all_parameters>


    // 一个任务一旦被执行就会产生一个任务ID，用户可以根据任务fetch到任务的ID，通过任务ID查询的任务的状态
    
};

