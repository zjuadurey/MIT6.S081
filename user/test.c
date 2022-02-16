#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"


const char* this_dir=".";
const char* parent_dir="..";


char* fmtname(char *path)//把路径中最后一个名字分离出来
{
    static char buf[DIRSIZ+1];
    char *p;

    // find the filename before the last "/"
    for(p=path+strlen(path); p >= path && *p != '/'; p--)
        ;
    p++;

    // Return blank-padded name.
    if(strlen(p) >= DIRSIZ)
        return p;
    memmove(buf, p, strlen(p));
    //memset(buf+strlen(p), ' ', DIRSIZ-strlen(p)); 务必删掉这条语句
    return buf;
}


void search(char* dir_name,const char* file_name)
{
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;
    //打开文件描述符
    if((fd = open(dir_name, 0)) < 0)
    {
        fprintf(2, "find: cannot open dir %s\n", dir_name);
        return;
    }
    // fstat 将fd所指的文件状态装入st所指的结构中（struct stat）
    // stat是文件(夹)信息结构体
    if(fstat(fd, &st) < 0)
    {
        fprintf(2, "find: cannot stat dir %s\n", dir_name);
        close(fd);
        return;
    }
    // 如果传入的是文件名（递归边界条件）
    if(st.type==T_FILE)
    {
        //和file_name进行匹配
        if(!strcmp(fmtname(dir_name),file_name))
        {
            printf("%s\n",dir_name);//直接打印目录
        }
        return;
    }
    // 如果是文件夹
    if(st.type==T_DIR)
    {
        if(strlen(dir_name) + 1 + DIRSIZ + 1 > sizeof buf)
        {
            printf("find: path too long\n");
            return;
        }
        // dir_name写入buf数组
        strcpy(buf, dir_name);
        p = buf+strlen(buf);//p是定位指针
        // p指向buf字符串的后一位
        *p++ = '/';
        //临时文件信息结构体
        struct stat st_tmp;
        //遍历目录下的文件
        //将fd读入de(struct dirent 目录项结构)
        while(read(fd, &de, sizeof(de)) == sizeof(de))
        {
            if(de.inum == 0)
                continue;
            memmove(p, de.name, DIRSIZ);//把文件名复制到字符串buf的最后面
            p[DIRSIZ] = 0;//这里准备遍历文件名 准备好生成文件或者目录的相对路径，存在buf里面，把这串字符的后一位置为0来生成字符串
            // 将buf的路径填入st_tmp(struct stat)
            if(stat(buf, &st_tmp) < 0)
            {
                //printf("ls: cannot stat %s\n", buf);
                continue;
            }
            if(st_tmp.type==T_FILE)//如果是普通文件
            {
                if(!strcmp(de.name,file_name))//找到文件
                {
                    printf("%s\n",file_name);//打印文件的相对路径
                }
            }
            if(st_tmp.type==T_DIR)//如果是目录
            {
                //递归搜索，使用BFS遍历directory tree
                //禁止遍历. .. 这两个目录
                if((!strcmp(de.name,this_dir))||(!strcmp(de.name,parent_dir)))
                {
                    continue;
                }
                search(buf,file_name);//递归搜索
            }

        }
    }
    return;
}


int main(int argc, char *argv[])
{
    if(argc==2)
    {
        search(".",argv[2]);
    }
    else
    {
        search(argv[1],argv[2]);
    }
    exit(0);
}
