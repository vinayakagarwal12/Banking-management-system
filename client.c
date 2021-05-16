#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<init.h>
int main()
{
    struct sockaddr_in serv;
    int sd=socket(AF_UNIX,SOCK_STREAM,0);
    if(sd==-1)
    {
        perror("Socket Failed");
        exit(EXIT_FAILURE);
    }
    serv.sin_family = AF_UNIX;
    serv.sin_addr.s_addr = INADDR_ANY;
    serv.sin_port = htons(15000);
    int cn=connect(sd,(struct sockaddr *)&serv,sizeof(serv));
    if(cn==-1)
    {
        perror("connection failed");
        exit(EXIT_FAILURE);
    }
    handler(sd);
}
void handler(int sd)
{
    int mode;
    mode=get_menu(START_MENU);
}