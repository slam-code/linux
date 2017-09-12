

#include <cstddef>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <cstdlib>
#include <iostream>
using  namespace std;

int main()
{

    int sockfd;
    struct  sockaddr_in servaddr;
    bzero(&servaddr,sizeof(servaddr));

    servaddr.sin_family=AF_INET;
    servaddr.sin_port=htons(8001);
    inet_pton(AF_INET,"115.28.72.50",&servaddr.sin_addr);

    sockfd=socket(AF_INET,SOCK_DGRAM,0);
    if(sockfd==-1)
    {
        perror("sock()=-1");
        exit(1);
    }

    int count=0;
    //char sendline[]="hello world .... ";

    string str1k=string(1000,'hello world ...');
    char recvline[1024];


    while (count<1000)
    {

        sendto(sockfd,str1k.c_str(),str1k.size(),0,(struct sockaddr *)&servaddr,sizeof(servaddr));

        int n=recvfrom(sockfd,recvline,1024,0,NULL,NULL);
        //recvfrom存在阻塞的可能性，如何提高系统的可靠性？select,poll,epoll。
        cout<<"[recv]:"<<recvline<<" "<<count<<endl;
        ++count;
    }
    return 0;
}
