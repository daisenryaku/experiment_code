#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>

int num_phil;
pthread_mutex_t mutex;
sem_t* S;

void Initialize(){
	int i;
	pthread_mutex_init(&mutex, NULL);
	S = calloc(num_phil, sizeof(sem_t));
	for(i=0; i < num_phil; i++){
		sem_init(&S[i],0,1);
	}
}

int take(int number)
{
  int left = number;
	int right = (number+1) % num_phil;
	sem_wait(&S[left]);								//先申请左边筷子
	if (sem_trywait(&S[right]) == 0){ //尝试申请右边筷子
		printf("%d号哲学家进餐\n",number);//两边都申请到，进餐
		return 1;
	}
	else {
		//右边筷子没申请到，释放掉左边
		sem_post(&S[left]);
		return 0;
	}
}

int put(int number){
	int left = number;
	int right = (number+1) % num_phil;
	sem_post(&S[left]);
	sem_post(&S[right]);
	printf("%d号哲学家放下两边筷子\n",number);
}

void *philospher(void *param)
{
	int* pi = (int*)param;
	int number = *pi;//参数转换

	while(1)
	{
		printf("%d号哲学家思考\n",number);//思考
		sleep(1);
		pthread_mutex_lock(&mutex); //互斥量加锁
		if ( take(number) ){//成功拿起两只筷子
			put(number);//放下筷子
		}
		pthread_mutex_unlock(&mutex);//互斥量解锁

	}
}

int main(int argc, char *argv[])
{
	num_phil = 5;

	//对互斥量、信号量进行初始化
	Initialize();

	// 创建哲学家进程
	int i;
	int *pi;
	pthread_t thread_id[num_phil];
	for(i=0; i<num_phil;i++){
		pi = &i;
		pthread_create(&thread_id[i],NULL,philospher,(void*)pi);
		sleep(1);
	}

	// 等待进程结束
	for(i=0;i<num_phil;i++){
		pthread_join(thread_id[i],NULL);
	}

}
