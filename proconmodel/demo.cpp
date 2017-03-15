#include <iostream>
#include <vector>
#include <string>
#include <queue>

#include <unistd.h>
#include <pthread.h>
#include <time.h>

using namespace std;

#define MAX 3
#define debugMsg(info) cout << info << endl;

pthread_mutex_t m_mutex = PTHREAD_MUTEX_INITIALIZER; // 主要用来互斥操作
pthread_cond_t notfull = PTHREAD_COND_INITIALIZER; // 用来判断队列是否满
pthread_cond_t notempty = PTHREAD_COND_INITIALIZER; // 用来判断队列是否是空

queue<int> gQ;

void showQue() {
	queue<int> t;
	while(!gQ.empty()) {
		int a = gQ.front();
		t.push(a);
		cout << a << ",";
		gQ.pop();
	}
	cout << endl;
	while(!t.empty()) {
		int a = t.front();
		gQ.push(a);
		t.pop();
	}
}

void* producer(void* arg) {
	// sleep(2);
	for(int i=0;i<MAX*2;i++) {
		pthread_mutex_lock(&m_mutex);
		while( gQ.size() >= MAX ) { // 该线程是生产者，当队列的容量>=设定的容量时，需要将阻塞生产者进程
			debugMsg("queue full ! producer is waiting");
			// 这里先将互斥锁m_mutex解锁，同时将该线程阻塞
			// 当另外的线程将触发 pthread_cond_signal(&notfull) 后，改阻塞的函数将互斥锁在上锁
			// 同时继续下一步操作
			pthread_cond_wait(&notfull, &m_mutex);
		}
		gQ.push(i+1);
		cout << "In Producer, ";
		showQue();
		pthread_cond_signal(&notempty);
		pthread_mutex_unlock(&m_mutex);
	}
	return (void*)1;
}

void* consumer(void* arg) {
	sleep(1);
	for(int i=0;i<MAX*2;i++) {
		pthread_mutex_lock(&m_mutex);
		while( gQ.empty() ) { // 该线程时消费者，当队列的容量为零，无法消费时，需要将消费者进程阻塞
			debugMsg("empty ! consumer is waiting !");
			pthread_cond_wait(&notempty, &m_mutex);
		}
		gQ.pop();
		cout << "In Consumer, ";
		showQue();
		pthread_cond_signal(&notfull);
		pthread_mutex_unlock(&m_mutex);
	}
	return (void*)2;
}


int main() {
	while(!gQ.empty()) gQ.pop();

	int ret1; pthread_t thread1;
	int ret2; pthread_t thread2;

	pthread_create(&thread1, NULL, producer, NULL);
	pthread_create(&thread2, NULL, consumer, NULL);

	pthread_join(thread1, (void**)&ret1);
	pthread_join(thread2, (void**)&ret2);

	return 0;
}
