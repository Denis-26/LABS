#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <time.h>
int main()
{
    pid_t pid;
    int rv;
    switch(pid=fork()) {
        case -1: {
            perror("fork");
            exit(1);

        }
        case 0: {
            FILE *fp = fopen("file.txt", "w");
            if(fp != NULL) {
                char str_id[12];
                sprintf(str_id, "%d", getpid());
                printf("%s", "Я дочерний процесс\nМой ID: ");
                printf("%s", strcat(str_id, "\n"));
                for (int i = 0; i < (1024*1024*10)/19; ++i){
                    fputs("Python is the BEST!", fp);
                }
            }
            fclose(fp);
            exit(rv);
        }
        default:{
            time_t f1 = time(NULL);
            char str_id[12];
            int status = 0;
            sprintf(str_id, "%d", getpid());
            printf("%s", "Я родительский процесс, жду дочерний\nМой ID: ");
            printf("%s", strcat(str_id, "\n"));
			waitpid(pid, &status, 0);
            printf("%s", "Я родительский процесс, дождался\n");
            time_t f2 = time(NULL);
            printf("Time: %ld\n", f2 - f1);
  }
  }
}
