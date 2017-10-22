
#include "help.h"

int unic = 13123;
const int user_max_filesize = 1024;
const int user_max_filename = 128;

int main(int argc, char *argv[])
{
    int fd = open("file.lol", O_RDWR);
    if (fd == -1){
        printf("%s\n", "Error in fileopen");
        close(fd);
        return 1;
    }
    if (argc >= 2){
        if (!strcmp(argv[1], "-r")){
            my_read(fd, argv[2]);
        } else if (!strcmp (argv[1], "-w")){
            my_write(fd, argv[2]);
        } else if (!strcmp (argv[1], "-d")){
            delete_file(fd, argv[2]);
        } else if (!strcmp (argv[1], "-l")){
            files(fd);
        }
    }
    close(fd);
    return 0;
}

int my_write(int fd, char *filename){
    struct MyType file_data;
    struct MyHeader header;
    struct MyType temp_file_data;
    struct MyHeader temp_header;
    int magic_numb;
    char *user_data = read_user_file(filename);

    header.del = 0;
    strncpy(header.name, filename, user_max_filename);
    strncpy(file_data.data, user_data, user_max_filesize);


    if (!empty_file(fd)){
        if (format_err(fd, unic)){
            printf("%s\n", "Format error");
            close(fd);
            return 1;
        }
        while(read(fd, &temp_header, sizeof(struct MyHeader))){
            if (temp_header.del){
                off_t offset = lseek( fd, 0, SEEK_CUR );
                lseek(fd, offset-sizeof(struct MyHeader), SEEK_SET);
                break;
            } else if (!strcmp(temp_header.name, filename)){
                printf("%s\n", "exist!!!!");
                return 0;
            }
            read(fd, &temp_file_data, sizeof(struct MyType));
        }
    } else {
        write(fd, &unic, sizeof(int));
    }

    write(fd, &header, sizeof(struct MyHeader));
    write(fd, &file_data, sizeof(struct MyType));
    return 0;
}

int my_read(int fd, char *filename){

    int det;
    struct MyType type;
    struct MyHeader header;
    if (format_err(fd, unic)){
        printf("%s\n", "Format error");
        close(fd);
        return 1;
    }

    while(read(fd, &header, sizeof(struct MyHeader))){
        read(fd, &type, sizeof(struct MyType));
        if (!strcmp(filename, header.name) && !header.del){
            printf("\nFILENAME: %s\n", header.name);
            printf("\nDATA:\n%s\n", type.data);
            return 0;
        }
    }

    return 0;
}


int delete_file(int fd, char* filename){
    struct MyType type;
    struct MyHeader header;
    if (format_err(fd, unic)){
        printf("%s\n", "Format error");
        close(fd);
        return 1;
    }
    while(read(fd, &header, sizeof(struct MyHeader))){
        if (!strcmp(filename, header.name)){
            off_t offset = lseek( fd, 0, SEEK_CUR );
            lseek(fd, offset-sizeof(struct MyHeader), SEEK_SET);
            header.del = 1;
            write(fd, &header, sizeof(struct MyHeader));
            return 0;
        }
        read(fd, &type, sizeof(struct MyType));
    }
}

void files(int fd){
    char files[1024] = "";
    struct MyHeader header;
    if (format_err(fd, unic)){
        printf("%s\n", "Format error");
        close(fd);
        exit(0);
    }
    while(read(fd, &header, sizeof(struct MyHeader))){
        if (!header.del){
            strcat(files, header.name);
            strcat(files, "\n");
        }
        lseek(fd, sizeof(struct MyType), SEEK_CUR);
    }
    printf("%s\n", files);
}
