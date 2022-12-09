#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<stdlib.h>
#include<fcntl.h>
#include<string.h>


int main(){

    //2.创建进程
    int fd;
    fd = fork();
    if(fd == 0){

        //1.判断管道是否存在
        int ret = access("f1",F_OK);
        if(ret == -1){
            printf("管道不存在，创建有名管道\n");
            ret = mkfifo("f1",0664);
            if(ret == -1){
                perror("mkfifo");
                exit(0);
            }
        }

        //子进程只读
        //以只写读方式打开f1

        int fdr = open("f1",O_RDONLY);
        if(fdr == -1){
            perror("open");
            exit(0);
        }
        printf("打开管道1成功，等待读取\n");

        char buf[128];
        //循环读数据
        while(1){
            memset(buf,0,128);
            ret = read(fdr,buf,128);
            if(ret == -1){
                perror("read");
                exit(0);
            }
            printf("buf:%s\n",buf);
        }

        //关闭文件描述符
        close(fdr);


    }else if(fd > 0){

        int ret = access("f2",F_OK);
        if(ret == -1){
            printf("管道不存在，创建有名管道\n");
            ret = mkfifo("f2",0664);
            if(ret == -1){
                perror("mkfifo");
                exit(0);
            }
        }

        //父进程只写
        //以只写方式打开f2
        int fdw = open("f2",O_WRONLY);
        if(fdw == -1){
            perror("open");
            exit(0);
        }
        printf("打开管道2成功，等待写入\n");

        char buf[128];
        //循环写数据
        while(1){
            
            memset(buf,0,128);
            fgets(buf,128,stdin);
            ret = write(fdw,buf,strlen(buf));
            if(ret == -1){
                perror("write");
                exit(0);
            }
            
        }
        //关闭文件描述符
        close(fdw);

    }

}