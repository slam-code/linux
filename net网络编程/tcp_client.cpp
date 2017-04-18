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
int main() {


    int sockfd;
    struct sockaddr_in servaddr;
    sockfd=socket(AF_INET,SOCK_STREAM,0);
    if(sockfd==-1)
    {
        perror("socket()=-1");
        exit(1);
    }
    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family=AF_INET;
    servaddr.sin_port=htons(8000);
    inet_pton(AF_INET,"115.28.72.50",&servaddr.sin_addr);
    //inet_pton(AF_INET,"127.0.0.1",&servaddr.sin_addr);
    int conn=connect(sockfd,(struct sockaddr*)&servaddr,sizeof(servaddr));
    if(conn!=0)
    {
        perror("connect()!=0");
        exit(1);
    }

    char message[]="hello,world...";
    char recvbuff[1024];

   int count=0;
    while(count<10000)
    {
        write(sockfd,message,sizeof(message));
        int n;
        //while((n=read(sockfd,recvbuff,1024))>0)
        {
            read(sockfd,recvbuff,24);
            if((*recvbuff)!=' ')
            cout<<count<<" [recv]:"<<recvbuff<<endl;
            else
                cout<<count<<" ,";
        }
        ++count;
    }
    close(sockfd);

}