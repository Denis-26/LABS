#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h>
#include <signal.h>

#define SHM_SIZE 128

int shm_id;
void* shmem;
int create = 1;

int semid;
struct sembuf sb;

void sig_handler(int signo){
    if(signo == SIGINT){
        shmdt(shmem);

        if(!create){
            shmctl(shm_id, IPC_RMID, NULL);
            semctl(semid, 0, IPC_RMID);
        }

        exit(0);
    }
}

void *p_read(){
  while(1){
      sb.sem_op = -1;
      if(semop(semid, &sb, 1) == -1){
        perror("semop");
      }
      printf("%s %s\n", (char*) shmem, "child");
      sb.sem_op = 1;
      semop(semid, &sb, 1);
  }
  return NULL;
}

void *p_write(){
  char time_str[100] = "";
  while(1){
      sb.sem_op = -1;
      semop(semid, &sb, 1);

      sleep(1);

      time_t t = time(NULL);
      struct tm tm = *localtime(&t);
      sprintf(time_str, "%d:%d:%d parent time", tm.tm_hour, tm.tm_min, tm.tm_sec);
      printf("%s\n", time_str);
      sprintf((char*) (shmem), "%s ", time_str);

      sb.sem_op = 1;
      semop(semid, &sb, 1);
  }
  return NULL;
}

int main(int argc, char** argv){
    setbuf(stdout, NULL);
    signal(SIGINT, sig_handler);

    key_t shm_key = ftok("/dev/random", 132);

    shm_id = shmget(shm_key, SHM_SIZE, 0);
    if(shm_id == -1){
        create = 0;
        printf("No shared memory\n");
        shm_id = shmget(shm_key, SHM_SIZE, IPC_CREAT | 0666);
        if(shm_id == -1){
            perror("shmget");
        }
    }

    shmem = shmat(shm_id, NULL, 0);
    if(shmem == (void*)-1){
        perror("failed to attach shm");
        exit(-1);
    }

    sb.sem_num = 0;
    sb.sem_flg = 0;

    if(create){
        printf("child\n");
        semid = semget(shm_key, 1, 0660);
        p_read();
    }
    else{
        printf("parent\n");
        semid = semget(shm_key, 1, IPC_CREAT | IPC_EXCL | 0666);
        semctl(semid, 0, SETVAL, 1);
        p_write();
    }
    return 0;
}
