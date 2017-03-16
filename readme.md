## 并行化程序

### 并行化OMP（正交匹配追踪）

### 线程池（pthread版本）

### 生产者消费者模型

- C语言版本
```c++
thread 1:
    pthread_mutex_lock(&mutex);
    while (!condition)
        pthread_cond_wait(&cond, &mutex);
    /* do something that requires holding the mutex and condition is true */
    pthread_mutex_unlock(&mutex);

thread2:
    pthread_mutex_lock(&mutex);
    /* do something that might make condition true */
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);
```

- C++语言版本
```c++

```