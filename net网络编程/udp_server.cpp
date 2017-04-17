//
// Created by shen on 17-4-17.
//

#include <cstddef>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <cstdlib>
#include <iostream>
using namespace std;
int main()
{

    int sockfd;
    struct  sockaddr_in servaddr,cliaddr;

    sockfd=socket(AF_INET,SOCK_DGRAM,0);
    if(sockfd==-1)
    {
        perror("sockfd()=-1 \n");
        exit(1);
    }

    memset(&servaddr,0,sizeof(servaddr));
    servaddr.sin_family=AF_INET;
    servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
    servaddr.sin_port=htons(8001);
    int b=bind(sockfd,(struct sockaddr*)&servaddr,sizeof(servaddr));
    if(b==-1)
    {
        perror("bind()==-1 \n");
        exit(1);
    }


    char buff[1024];
    socklen_t  len;
    char message[]="ok";
    int count=0;
    while(1)
    {

        recvfrom(sockfd,buff,1024,0,(struct sockaddr*) &cliaddr,&len);
        //recvfrom可能超时阻塞

        cout<<"[recv]:"<<buff<<" "<<count<<endl;
        sendto(sockfd,message,1024,0,(struct sockaddr*)&cliaddr,len);
        ++count;
    }

    return 0;
}

