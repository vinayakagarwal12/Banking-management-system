#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<pthread.h>
#include<init.h>
void* socket_cn(void* vgp)
{
    int nsd = *(int*)vgp;
    char buff[100];
    read(nsd,buff,sizeof(buff));
    printf("Client message : %s\n",buff);
    write(nsd,"ACK for receving message by server\n",sizeof("ACK for receving message by server\n")); 
    return 0;
}
int main()
{
    struct sockaddr_in serv, cli;
    // AF_UNIX for local connection 
    int sd = socket(AF_UNIX,SOCK_STREAM,0); 
    if(sd==-1)
    {
        perror("Socket Failed");
        exit(EXIT_FAILURE);
    }
    int tr=1;
    setsockopt(sd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &tr, sizeof(tr));
    serv.sin_family = AF_UNIX;
    serv.sin_addr.s_addr = INADDR_ANY;
    //big endian (host to socket) 
    serv.sin_port = htons(15000); 
    int bd=bind(sd,(struct sockaddr *)&serv,sizeof(serv));
    if(bd==-1)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    // number of clients at a time;
    if(listen(sd,5)<0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }   
    int sz = sizeof(cli);
    while(1)
    {
        int nsd=accept(sd,(struct sockaddr *)&cli,&sz);
        if(nsd==-1)
        {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        pthread_t thid;
        if(pthread_create(&thid,NULL,handler,(void*)&nsd) == -1)
        {
            perror("thread Failed");
            exit(EXIT_FAILURE);
        } 
    }
}
void *handler(void *sockdesc)
{
    int sd=*(int*)sockdesc;
    char *mode_str=malloc(buf_size*sizeof(char));
    char *username=malloc(buf_size*sizeof(char));
    char *password=malloc(buf_size*sizeof(char));
    char *reply=malloc(buf_size*sizeof(char));
    char *amount_dp=malloc(buf_size*sizeof(char));
    char *amount_wh=malloc(buf_size*sizeof(char));
    int mode;
    while (1)
    {
        read(sd,mode_str,sizeof(mode_str));
        mode=atoi(mode_str);
        if(mode==SIGN_UP_NORMAL || mode==SIGN_UP_JOINT || mode==SIGN_UP_ADMIN)
        {
            read(sd,username,sizeof(username));
            read(sd,password,sizeof(password));
            int flag=signup(mode,username,password);
            if(flag==-1)
            {
                reply="Sign up failed!";
            }
            else
            {
                reply="You have signed up successfully";
            }
        }
        else if(mode==SIGN_IN_NORMAL || SIGN_IN_JOINT || SIGN_In_ADMIN)
        {
            read(sd,username,sizeof(mode_str));
            read(sd,password,sizeof(mode_str));
            int flag=signin(mode,username,password);
            if(flag==-1)
            {
                reply="Sign up failed!";
            }
            else
            {
                reply="You have signed up successfully";
            }
        }
        else if(mode==DEPOSIT)
        {
            read(sd,amount_dp,sizeof(amount_dp));
            int flag=deposit(username,atoi(amount_dp));
            if(flag==-1)
            {
                reply="Failure in Deposit";
            }
            else
            {
                reply="Amount deposited successfully";
            }
        }
        else if(mode==WITHDRAW)
        {
            read(sd,amount_wh,sizeof(amount_wh));
            int flag=withdraw(username,atoi(amount_wh));
            if(flag==-1)
            {
                reply="Failure in Withdraw";
            }
            else
            {
                reply="Amount withdraw successful";
            }
        }
        else if(mode==PASSWORD_CHNG)
        {
            char *new_password=malloc(buf_size*sizeof(char));
            read(sd,new_password,sizeof(new_password));
            int flag=password_change(username,new_password);
            if(flag==-1)
            {
                reply="Failure in password change";
            }
            else
            {
                reply="Password changed successfuly";
            }
        }
        else if(mode==VIEW_DETAILS)
        {
            reply=view_details(username);
        }
        else if(mode==ADD_ACC)
        {
            char *username1=malloc(buf_size*sizeof(char));
            char *password1=malloc(buf_size*sizeof(char));
            char *type=malloc(buf_size*sizeof(char));
            read(sd,type,sizeof(type));
            read(sd,username1,sizeof(username1));
            read(sd,password1,sizeof(password1));
            int flag=add_account(type,username1,password1);
            if(flag==-1)
            {
                reply="Unable to add account";
            }
            else
            {
                reply="Account added successfully";
            }
        }
        else if(mode==DEL_ACC)
        {
            char *username1=malloc(buf_size*sizeof(char));
            read(sd,username1,sizeof(username1));
            int flag=del_account(username1);
            if(flag==-1)
            {
                reply="Unable to delete account";
            }
            else
            {
                reply="Account deleted successfully";
            }
        }
        else if(mode==MOD_ACC)
        {
            // to be done
        }
        else if(mode==VIEW_DETAILS_ADMIN)
        {
            char *username1=malloc(buf_size*sizeof(char));
            read(sd,username1,sizeof(username1));
            reply=view_details(username1);
        }
        write(sd,reply,sizeof(reply));
    }
}