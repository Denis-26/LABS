#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <fcntl.h>


struct MyType{
    char data[1024];
};

struct MyHeader{
    char name[128];
    int del;
};

char *read_user_file(char*);
int empty_file(int);
int exist(int, char*);
int my_write(int, char*);
int my_read(int, char *);
void files(int);
int delete_file(int, char*);
int format_err(int, int);
