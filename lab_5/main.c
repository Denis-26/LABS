#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <semaphore.h>

const int SIZE = 512;
int shm_id;
void* shmem;
pthread_t tid[10];
sem_t semaphore;

void sig_handler(int sign){
    if(sign == SIGINT){
        shmdt(shmem);
        shmctl(shm_id, IPC_RMID, NULL);
        exit(0);
    }
}

void *myThreadRead(void *vargp){
  while(1){
    sleep(rand() % 5);
    sem_wait(&semaphore);
    printf("%s %s\n", (char*) shmem, "child");
    sem_post(&semaphore);
  }
  return NULL;
}

void *myThreadWrite(void *vargp){
  char time_str[100] = "";
  while(1){
    sleep(rand() % 5);
    sem_wait(&semaphore);
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(time_str, "ID:%ld %d:%d:%d parent time", pthread_self(), tm.tm_hour, tm.tm_min, tm.tm_sec);
    printf("%s\n", time_str);
    sprintf((char*) (shmem), "%s ", time_str);
    sem_post(&semaphore);
  }
  return NULL;
}


int main(){
  //setbuf(stdout, NULL);
  signal(SIGINT, sig_handler);
  key_t shm_key = ftok("/dev/random", 1);
  int exist = 1;
  shm_id = shmget(shm_key, SIZE, 0);
  sem_init(&semaphore, 0, 1);

  if (shm_id == -1){
    exist = 0;
    shm_id = shmget(shm_key, SIZE, IPC_CREAT | 0666);
    if(shm_id == -1){
      printf("%s\n", "Error shmget");
    }
  }

  shmem = shmat(shm_id, NULL, 0);
  if (shm_id < 0){
    printf("%s\n", "attach error");
    exit(1);
  }

  if (exist){
    int i;
    for (i = 0; i < 10; i++)
        pthread_create(&(tid[i]), NULL, myThreadRead, NULL);
  } else {
    int i;
    for (i = 0; i < 10; i++)
        pthread_create(&(tid[i]), NULL, myThreadWrite, NULL);
  }

  pthread_join(tid[0], NULL);
  shmdt(shmem);
  shmctl(shm_id, IPC_RMID, NULL);

  return 0;
}