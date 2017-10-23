#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include "help.h"
#include <fcntl.h>



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
    int det;
    while(read(fd, &header, sizeof(struct MyHeader))){
        read(fd, &record, sizeof(struct MyType));
        if (!strcmp(filename, header.name)){
            off_t position = lseek(fd, 0, SEEK_CUR);
            lseek(fd, sizeof(int), SEEK_SET);
            return 1;
        }
    }
    off_t position = lseek(fd, 0, SEEK_CUR);
    lseek(fd, sizeof(int), SEEK_SET);
    return 0;
}

int empty_file(int fd){
    off_t size = lseek (fd, 0, SEEK_END);
    if (size == 0) {
        lseek(fd, 0, SEEK_SET);
        return 1;
    }
    lseek(fd, 0, SEEK_SET);
    return 0;
}

int format_err(int fd, int unic){
    int det;
    read(fd, &det, sizeof(int));
    if (det != unic){
        close(fd);
        return 1;
    }
    return 0;
}

int find_free_files(int fd){
    int det;
    struct MyType type;
    struct MyHeader header;


    while(read(fd, &header, sizeof(struct MyHeader))){
        if (header.del){
            off_t offset = lseek( fd, 0, SEEK_CUR );
            lseek(fd, offset-sizeof(struct MyHeader), SEEK_SET);
            return fd;
        }
        read(fd, &type, sizeof(struct MyType));
    }
    return 0;
}
