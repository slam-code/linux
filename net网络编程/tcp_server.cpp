#include <cstddef>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <cstdlib>
#include <iostream>
#include <unistd.h>
using  namespace std;
int main()
{
    int listenfd,connfd;
    pid_t  childpid;
    socklen_t clilen;
    struct sockaddr_in cliaddr,servaddr;
    listenfd=socket(AF_INET,SOCK_STREAM,0);
    if(listenfd==-1)
    {
        perror("socket()=-1");
        exit(1);
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family=AF_INET;
    servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
    servaddr.sin_port=htons(8000);//不能是htonl。
    int bd=bind(listenfd,(struct sockaddr*)&servaddr, sizeof(servaddr));
    if(bd==-1)
    {
        perror("bind()=-1");
        exit(1);
    }

    char replay[1024]="ok.i got it...";
    char recvbuff[1024]="";
    int li=listen(listenfd,1024);
    if(li!=0)
    {
        perror("listen()!=0");
        exit(1);
    }
    char cliaddrbuff[1024];
    cout<<"begin: listenfd="<<listenfd<<" "<<endl;
    int count=0;
    while (1)
    {
        clilen=sizeof(cliaddr);
        connfd=accept(listenfd,(struct sockaddr*)&cliaddr,&clilen);
        if(connfd==-1)
        {
            perror("accpet()=-1");
            exit(1);
        }

        inet_ntop(AF_INET,&cliaddr.sin_addr,cliaddrbuff,1024);
        cout <<count<<" connfd="<<connfd<<" client ip="<<cliaddrbuff<<endl;
        if((childpid=fork())==0)//child
        {
            close(listenfd);
            int j=0;
            while(j<100000)
            {
                read(connfd,recvbuff,36);
                write(connfd,replay,1024);
                cout<<j<<" [recv]:"<<recvbuff<<endl;
                ++j;
            }

            exit(0);

        }

        close(connfd);//parent and child。父子进程都要close(fd).tlpi,fork().

        ++count;

    }
}