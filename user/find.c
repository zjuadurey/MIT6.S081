#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
//https://blog.csdn.net/NP_hard/article/details/121237162
const char* this_dir = ".";
const char* parent_dir = "..";

char*
fmtname(char *path)
{
    static char buf[DIRSIZ+1];
    char *p;

    for(p=path+strlen(path); p >= path && *p != '/'; p--)
        ;
    p++;

    if(strlen(p) >= DIRSIZ)
        return p;
    memmove(buf, p, strlen(p));
    //memset(buf + strlen(p), ' ', DIRSIZ - strlen(p));
    return buf;
}

void
search(char *dir_name, const char* file_name)
{
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if ((fd = open(dir_name, 0)) < 0){
        fprintf(2, "find: cannot open %s\n", dir_name);
        return;
    }

    if (fstat(fd, &st) < 0){
        fprintf(2, "find: cannot stat %s\n", dir_name);
        close(fd);
        return;
    }
    
    if (st.type == T_FILE)
    {
        if (!strcmp(fmtname(dir_name), file_name))
        {
            printf("%s\n", file_name);
        }
        return;
    }
       
    if (st.type == T_DIR)
    {
        if (strlen(dir_name) + 1 + DIRSIZ + 1 > sizeof buf){
            printf("ls: path too long\n");
            return;
        }
        strcpy(buf, dir_name);
        p = buf + strlen(buf);
        *p++ = '/';

        struct stat st_tmp;

        while (read(fd, &de, sizeof(de)) == sizeof(de)){
            if(de.inum == 0)
                continue;
            memmove(p, de.name, DIRSIZ);
            p[DIRSIZ] = 0;
            if (stat(buf, &st_tmp) < 0){
                printf("ls: cannot stat %s\n", buf);

                continue;
            }
            if (st_tmp.type == T_FILE)
            {
                if (!strcmp(de.name, file_name))
                {
                    printf("%s\n", buf);
                }
            }
               
            if (st_tmp.type == T_DIR)
            {
                if ((!strcmp(de.name, this_dir)) || (!strcmp(de.name, parent_dir)))
                {
                    continue;
                }    
                search(buf, file_name);
            }
              
        }
    }
    
    return;
}

int
main(int argc, char *argv[])
{
    
    if (argc == 2){
        search(".", argv[2]);
        exit(0);
    }
    search(argv[1], argv[2]);
    exit(0);
}