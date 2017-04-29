#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>

#define BUFFER_SIZE 2
typedef char buffer_elem;
buffer_elem buffer[BUFFER_SIZE];
pthread_mutex_t mutex;
sem_t empty, full;
int counter;

void Initialize(){
/*
@ 对互斥量mutex、信号量empty、信号量full、缓冲区进行初始化
@ 返回值：void
*/
	pthread_mutex_init(&mutex, NULL);
	sem_init(&empty, 0, BUFFER_SIZE);
	sem_init(&full, 0, 0);
	counter = 0;
	int i;
	for (i = 0; i < BUFFER_SIZE; i++){
		buffer[i] = -1;
	}
}

buffer_elem readFile(){
/*
@ 从文件中读一个字符
@ 返回值：读到的字符
*/
	FILE *fp;
	if((fp=fopen("input.txt","rb"))==NULL) {
	    printf("Wrong in file open");
	    exit(1);
	}

	buffer_elem a = fgetc(fp);
	fclose(fp);

	return a;
}

void * producer(void *param){
/*
@ 生产者对应函数，从文件中读一个字符，放入缓冲区
@ 返回值：void
*/
	buffer_elem item;

	int* pi = (int*)param;
  int number = *pi;//参数转换

	while(1){
		sem_wait(&empty);						//申请empty信号量
		pthread_mutex_lock(&mutex); //缓冲池加锁

		//进入临界区
		sleep(1);
		item = readFile();          //从文件中读一个字符
		buffer[counter] = item;     //放入缓冲区
		counter++;
		printf("%d号生产者生产了:%c\n", number+1, item);
		//退出临界区

		pthread_mutex_unlock(&mutex);//缓冲池解锁
		sem_post(&full);						 //满缓冲池数量+1
	}

	  pthread_exit(0);
}

void * consumer(void *param){
/*
@ 消费者对应函数，从缓冲区取出一个产品，stdout输出
@ 返回值：void
*/
	buffer_elem item;

	int* pi = (int*)param;
	int number = *pi;//参数转换

	while(1){
		sem_wait(&full);						//申请full信号量
		pthread_mutex_lock(&mutex); //缓冲池加锁

    //进入临界区
		item = buffer[counter-1];  //取出产品
		buffer[counter-1] = -1;
		counter--;
		//退出临界区

		printf("%d号消费者取出了:%c\n", number+1,item);	//打印取出的产品
		pthread_mutex_unlock(&mutex);		  //缓冲池解锁
		sem_post(&empty);					   		  //空缓冲池数量+1
		sleep(1);
	}

	 pthread_exit(0);
}

int main(int argc, char *argv[]){
	//定义：3个生产者，4个消费者
	int numProducers = 3;
	int numConsumers = 4;
	//对互斥量、信号量、缓冲区进行初始化
	Initialize();
	pthread_t prod_tid[numProducers];
	pthread_t con_tid[numConsumers];
	int i =0;
	int* pi;
	//创建生产者线程
	for(i = 0; i < numProducers; i++){
		pi = &i;
		pthread_create(&prod_tid[i], NULL, producer, (void*)pi);
		sleep(1);
	}
	//创建消费者线程
	for(i = 0; i < numConsumers; i++){
		pi = &i;
		pthread_create(&con_tid[i], NULL, consumer, (void*)pi);
		sleep(1);
	}

	//等待线程退出
	for(int i = 1;i < numProducers;i++){
		pthread_join(prod_tid[i],NULL);
	}
	for(int i = 1;i < numConsumers;i++){
		pthread_join(con_tid[i],NULL);
	}

	//退出
	pthread_mutex_destroy(&mutex);
	sleep(2);
	return 0;
}
