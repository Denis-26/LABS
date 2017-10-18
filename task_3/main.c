#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>

int my_write(char*);
int my_read(char *);
char *read_user_file(char*);
int empty_file(FILE *);
int exist(FILE*, char*);

struct MyType{
    char name[128];
    char data[1024];
};

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
    FILE *file_sys, *user_file;
    file_sys = fopen("file.lol", "r+");
    char *user_data = read_user_file(filename);

    struct MyType current;
    strncpy(current.name, filename, 128);
    strncpy(current.data, user_data, 1024);

    if (!file_sys){
        printf("%s\n", "Error in fileopen");
        return 1;
    }
    int empty = empty_file(file_sys);
    if (!empty){
        if (!exist(file_sys, filename)){
            fwrite(&current, sizeof(struct MyType), 1, file_sys);
        } else {
            printf("%s\n", "exist!!!!");
        }
    } else {
        fwrite(&current, sizeof(struct MyType), 1, file_sys);
    }

    fclose(file_sys);
    return 0;
}

int my_read(char *filename){
    FILE *file_sys;
    file_sys = fopen("file.lol", "rb");
    if (!file_sys){
        printf("%s\n", "Error in fileopen");
        return 1;
    }
    struct MyType type;
    while (fread(&type, sizeof(struct MyType), 1, file_sys) == 1){
        if (!strcmp(filename, type.name)){
            printf("%s\n", type.data);
        }
    }
    fclose(file_sys);
}


char* read_user_file(char *filename){
    FILE *f = fopen(filename, "rb");
    if (!f){
        printf("%s\n", "Error in fileopen");
        return "";
    }
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);

    char *data_string = malloc(fsize + 1);
    fread(data_string, fsize, 1, f);
    data_string[fsize] = 0;
    fclose(f);
    return data_string;
}

int exist(FILE *fp, char *filename){
    struct MyType record;
    while (fread(&record, sizeof(struct MyType), 1, fp) == 1){
        if (!strcmp(record.name, filename)){
            return 1;
        }
    }
    return 0;
}

int empty_file(FILE *fp){
    if (NULL != fp) {
        fseek (fp, 0, SEEK_END);
        size_t size = ftell(fp);
        if (size == 0) {
            fseek(fp, 0, SEEK_SET);
            return 1;
        }
    }
    fseek(fp, 0, SEEK_SET);
    return 0;
}
