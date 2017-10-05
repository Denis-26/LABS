#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    struct stat buf;
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir (".")) != NULL) {
        while ((ent = readdir (dir)) != NULL) {
            uid_t uid=getuid(), euid=geteuid();
            struct passwd *p = getpwuid(uid);
            struct group *cur_gr = getgrgid(uid);
            stat(ent->d_name, &buf);
            char prefix[100] = "";
            char date[36];
            strftime(date, 36, "%b %d %H:%M ", localtime(&(buf.st_mtime)));
            sprintf(prefix, "%s%s%s%s%s%s%s%s%s %ld %s %s %7ld %s %s\n",
                (S_ISDIR(buf.st_mode)) ?  "d" : "-",
                (buf.st_mode & S_IRUSR) ? "r" : "-",
                (buf.st_mode & S_IWUSR) ? "w" : "-",
                (buf.st_mode & S_IXUSR) ? "x" : "-",
                (buf.st_mode & S_IRGRP) ? "r" : "-",
                (buf.st_mode & S_IWGRP) ? "w" : "-",
                (buf.st_mode & S_IROTH) ? "r" : "-",
                (buf.st_mode & S_IWOTH) ? "w" : "-",
                (buf.st_mode & S_IXOTH) ? "x" : "-",
                buf.st_nlink,
                (uid < 0 || uid != euid) ? "root " : p->pw_name,
                (uid < 0 || uid != euid) ? "root " : cur_gr->gr_name,
                buf.st_size,
                date,
                ent->d_name
            );
            printf("%s", prefix);
        }
        closedir (dir);
    } else {
        perror ("");
        return 1;
    }
    return 0;
}
