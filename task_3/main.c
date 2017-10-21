#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <fcntl.h>
#include "help.h"



int my_write(char*);
int my_read(char *);

int main(int argc, char *argv[])
{
    if (argc == 3){
        if (!strcmp(argv[1], "r")){
            my_read(argv[2]);
        } else if (!strcmp (argv[1], "w")){
            my_write(argv[2]);
        }
    }
}

int my_write(char *filename){
    int file_sys;
    struct MyType file_data;
    struct MyHeader header;
    header.det = det;
    file_sys = open("file.lol", O_RDWR);
    char *user_data = read_user_file(filename);


    strncpy(file_data.name, filename, 128);
    strncpy(file_data.data, user_data, 1024);

    if (file_sys == -1){
        printf("%s\n", "Error in fileopen");
        return 1;
    }
    int empty = empty_file(file_sys);
    if (!empty){
        if (!exist(file_sys, filename)){
            write(file_sys, &header, sizeof(struct MyHeader));
            write(file_sys, &file_data, sizeof(struct MyType));
        } else {
            printf("%s\n", "exist!!!!");
        }
    } else {
        int res = write(file_sys, &header, sizeof(struct MyHeader));
        int res2 = write(file_sys, &file_data, sizeof(struct MyType));
    }

    close(file_sys);
    return 0;
}

int my_read(char *filename){
    int fd = open("file.lol", O_RDONLY);
    if (fd == -1){
        printf("%s\n", "Error in fileopen");
        return 1;
    }
    struct MyType type;
    struct MyHeader header;
    for(int i = 1; ; ++i){
        ssize_t r_bytes = read(fd, &header, sizeof(struct MyHeader));
        lseek(fd, sizeof(struct MyHeader)*i, SEEK_SET);
        r_bytes = read(fd, &type, sizeof(struct MyType));
        if (r_bytes == 0){
            return 0;
        }
        if (!strcmp(filename, type.name) && header.det == det){
            printf("%s\n", type.name);
            return 0;
        }
        off_t s = lseek(fd, sizeof(struct MyType)*i, SEEK_SET);
    }
    close(fd);
    return 0;
}
