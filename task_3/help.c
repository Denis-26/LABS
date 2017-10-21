#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include "help.h"
#include <fcntl.h>

const int det = 13123;

char* read_user_file(char *filename){
    int f = open(filename, O_RDONLY);
    if (f == -1){
        printf("%s\n", "Error in fileopen");
        return "";
    }
    off_t fsize;

    fsize = lseek(f, 0, SEEK_END);
    char *data_string = malloc(fsize + 1);
    lseek(f, 0, SEEK_SET);
    read(f, data_string, fsize);

    data_string[fsize] = 0;
    close(f);
    return data_string;
}

int exist(int fd, char *filename){
    struct MyType record;
    struct MyHeader header;
    for(int i = 1; ; ++i){
        ssize_t r_bytes = read(fd, &header, sizeof(struct MyHeader));
        lseek(fd, sizeof(struct MyHeader)*i, SEEK_SET);
        r_bytes = read(fd, &record, sizeof(struct MyType));
        lseek(fd, sizeof(struct MyType)*i, SEEK_SET);
        if (r_bytes == 0){
            return 0;
        }
        if (!strcmp(filename, record.name)){
            return fd;
        }

    }
    return 0;
}

int empty_file(int fd){
    if (fd != -1) {
        off_t size = lseek (fd, 0, SEEK_END);
        if (size == 0) {
            lseek(fd, 0, SEEK_SET);
            return 1;
        }
    } else {
        return -1;
    }
    lseek(fd, 0, SEEK_SET);
    return 0;
}
