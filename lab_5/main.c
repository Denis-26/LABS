#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h>
#include <signal.h>

const int SIZE = 512;
int shm_id;
void* shmem;
int semid = 0 ;
pthread_t tid[10];
struct sembuf sb;

void sig_handler(int sign){
    if(sign == SIGINT){
        shmdt(shmem);
        shmctl(shm_id, IPC_RMID, NULL);
        exit(0);
    }
}

void *myThreadRead(){
  while(1){
    sb.sem_op = -1;
    if(semop(semid, &sb, 1) == -1)
      perror("semop error");
    printf("%s %s\n", (char*) shmem, "child");
    sb.sem_op = 1;
    semop(semid, &sb, 1);
  }
  return NULL;
}

void *myThreadWrite(){
  char time_str[100] = "";
  while(1){
    sb.sem_op = -1;
    semop(semid, &sb, 1);
    sleep(rand() % 5);

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(time_str, "ID:%d:%d:%d parent time", tm.tm_hour, tm.tm_min, tm.tm_sec);
    printf("%s\n", time_str);

    sprintf((char*) (shmem), "%s ", time_str);
    sb.sem_op = 1;
    semop(semid, &sb, 1);
  }
  return NULL;
}


int main(){
  //setbuf(stdout, NULL);
  signal(SIGINT, sig_handler);
  key_t shm_key = ftok("/dev/random", 1);
  int exist = 1;
  shm_id = shmget(shm_key, SIZE, 0);

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
  sb.sem_num = 0;
  sb.sem_flg = 0;
  if (exist){
    semid = semget(shm_key, 1, 0660);
    myThreadRead();
  } else {
    semid = semget(shm_key, 1, IPC_CREAT | IPC_EXCL | 0666);
    semctl(semid, 0, SETVAL, 1);
    myThreadWrite();
  }

  shmdt(shmem);
  shmctl(shm_id, IPC_RMID, NULL);

  return 0;
}
