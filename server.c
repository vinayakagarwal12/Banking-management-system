#include<stdio.h>
#include<unistd.h> 
#include<sys/socket.h> 
#include<stdlib.h> 
#include<netinet/in.h> 
#include <arpa/inet.h>
#include<string.h> 
#include<pthread.h>
#include<fcntl.h>
#include"init.h"
extern int signup(int,char*,char*); 
extern int signin(int,char*,char*); 
extern int deposit(char*,int); 
extern int password_change(char*,char*); 
extern int withdraw(char*,int); 
extern int get_balance(char*);
extern int del_account(char*);
extern char* view_details(char*);
extern char* user_modify(char*,char*,char*);
void* socket_cn(void* vgp)
{
    // int nsd = *(int*)vgp;
    // char buff[buf_size];
    // read(nsd,buff,sizeof(buff));
    // printf("Client message : %s\n",buff);
    // write(nsd,"ACK for receving message by server\n",sizeof("ACK for receving message by server\n")); 
    int sd = *(int*)vgp;
    char username[buf_size];
    char password[buf_size];
    char reply[buf_size];
    char mode_str[buf_size];
    char amount_dp[buf_size];
    char amount_wh[buf_size];
    int flag=1;
    int mode;
    while(1)
    {
        read(sd,mode_str,sizeof(mode_str));
        mode=atoi(mode_str);
        if(mode==SIGN_UP_NORMAL || mode==SIGN_UP_JOINT || mode==SIGN_UP_ADMIN)
        {
            read(sd,username,sizeof(username));
            read(sd,password,sizeof(password));
            int type;
            if(mode==SIGN_UP_NORMAL)
            {
                type=1;
            }
            else if(mode==SIGN_UP_JOINT)
            {
                type=2;
            }
            else
            {
                type=3;
            }
            int flag=signup(type,username,password);
            if(flag==-1)
            {
                strncpy(reply,"Sign up failed",buf_size);
                break;
            }
            else
            {
                strncpy(reply,"You have signed up successfully",buf_size);
            }
        }
        else if(mode==SIGN_IN_NORMAL || mode==SIGN_IN_JOINT || mode==SIGN_IN_ADMIN)
        {
            read(sd,username,sizeof(mode_str));
            read(sd,password,sizeof(mode_str));
            int type;
            if(mode==SIGN_IN_NORMAL)
            {
                type=1;
            }
            else if(mode==SIGN_IN_JOINT)
            {
                type=2;
            }
            else
            {
                type=3;
            }
            int flag=signin(mode,username,password);
            if(flag==-1)
            {
                strncpy(reply,"Sign in failed",buf_size);
                break;
            }
            else
            {
                strncpy(reply,"You have signed in successfully",buf_size);
            }
        }
        else if(mode==DEPOSIT)
        {
            read(sd,amount_dp,sizeof(amount_dp));
            int flag=deposit(username,atoi(amount_dp));
            if(flag==-1)
            {
                strncpy(reply,"Failure in Deposit",buf_size);
            }
            else
            {
                strncpy(reply,"Amount deposited successfully",buf_size);
            }
        }
        else if(mode == BALANCE_EQ)
        {
            int amt=get_balance(username);
            if(amt==-1)
            {
                strncpy(reply,"Failure in checking balance",buf_size);
            }
            else
            {
                char amt_str[buf_size];
                sprintf(amt_str,"%d",amt);
                strncpy(reply,"Your account balance is :- ",buf_size);
                strncat(reply,amt_str,sizeof(amt_str));
            }
        }
        else if(mode==WITHDRAW)
        {
            read(sd,amount_wh,sizeof(amount_wh));
            int flag=withdraw(username,atoi(amount_wh));
            if(flag==-1)
            {
                strncpy(reply,"Failure in Withdraw",buf_size);
            }
            else
            {
                strncpy(reply,"Amount withdraw successful",buf_size);
            }
        }
        else if(mode==PASSWORD_CHNG)
        {
            char new_password[buf_size];
            read(sd,new_password,sizeof(new_password));
            int flag=password_change(username,new_password);
            if(flag==-1)
            {
                strncpy(reply,"Failure in password change",buf_size);
            }
            else
            {
                strncpy(reply,"Password changed successfuly",buf_size);
            }
        }
        else if(mode==VIEW_DETAILS)
        {
            strncpy(reply,view_details(username),buf_size);
        }
        else if(mode==ADD_ACC)
        {
            char *username1=malloc(buf_size*sizeof(char));
            char *password1=malloc(buf_size*sizeof(char));
            char *type=malloc(buf_size*sizeof(char));
            read(sd,type,sizeof(type));
            read(sd,username1,sizeof(username1));
            read(sd,password1,sizeof(password1));
            int flag=signup(type,username1,password1);
            if(flag==-1)
            {
                strncpy(reply,"Unable to add account",buf_size);
            }
            else
            {
                strncpy(reply,"Account added successfully",buf_size);
            }
        }
        else if(mode==DEL_ACC)
        {
            char *username1=malloc(buf_size*sizeof(char));
            read(sd,username1,sizeof(username1));
            int flag=del_account(username1);
            if(flag==-1)
            {
                strncpy(reply,"Unable to delete account",buf_size);
            }
            else
            {
                strncpy(reply,"Account deleted successfully",buf_size);
            }
        }
        else if(mode==MOD_ACC)
        {
            char* username[buf_size];
			char* password[buf_size];
            char* new_username[buf_size];
			read(sd,username,sizeof(username));
			read(sd,new_username,sizeof(new_username));
			read(sd,password,sizeof(password));
            int flag=user_modify(username, new_username,password);
			if (flag == -1) 
            {
                strncpy(reply,"Unable to modify account",buf_size);
            }
            else
            {
                strncpy(reply,"modified user details successfuly",buf_size);
            }
        }
        else if(mode==VIEW_DETAILS_ADMIN)
        {
            char *username1=malloc(buf_size*sizeof(char));
            read(sd,username1,sizeof(username1));
            strncpy(reply,view_details(username1),buf_size);
        }
        write(sd,reply,buf_size*sizeof(char));
    }
    printf("%d\n",flag);
    return 0;
}
int main()
{
    int server_fd,new_socket_sd,opt=1;
    struct sockaddr_in server, client;
    int addrlen=sizeof(server);
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    { 
		perror("Creating Socket failed..."); 
		exit(EXIT_FAILURE); 
	} 
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) 
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(8000);

    if (bind(server_fd, (struct sockaddr *)&server, sizeof(server))<0) { 
		perror("Binding Socket failed..."); 
		exit(EXIT_FAILURE); 
	} 
	if (listen(server_fd, 3) < 0) { 
		perror("Listening error..."); 
		exit(EXIT_FAILURE); 
	} 
    printf("Bank is opened Successfully!\n");
    while(1)
    {
        if ((new_socket_sd = accept(server_fd, (struct sockaddr *)&server,(socklen_t *)&addrlen))<0) { 
            perror("Accepting Connection failed\n"); 
            exit(EXIT_FAILURE); 
        } 
        pthread_t thread_id;
        if(pthread_create(&thread_id,NULL,socket_cn,(void*)&new_socket_sd)<0)
        {
            perror("Could not create thread to handle client\n");
            return 1;
        }
        printf("Handler assigned\n");
    }
    printf("server shutdown\n");
    close(server_fd);
    close(new_socket_sd);
	return 0;
}