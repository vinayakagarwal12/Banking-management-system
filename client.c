#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<stdlib.h>
#include<sys/stat.h>
#include <arpa/inet.h>
#include"init.h"
#include<string.h>

void interface(int sd)
{
    printf("Welcome to SBI!. Hope you are doing well!\n");
    printf("How can I serve you today\n");
    printf("Press 1 for Sign up\n");
    printf("Press 2 for Sign in\n");
    int op;
    scanf("%d",&op);
	int user_type;
	char username[buf_size];
	char password[buf_size];
	char user_type_string[buf_size];
	char reply[buf_size];
	int mode;
	if(op==1)
    {
        // sign up 
        printf("Please enter the user type for sign up\n");
        printf("Press 1 for Normal User\n");
        printf("Press 2 for Joint Account User\n");
        printf("Press 3 for Administrator\n");
        scanf("%d",&user_type);
        if(user_type==1)
        {
            mode=SIGN_UP_NORMAL;
        }
        else if(user_type==2)
        {
            mode=SIGN_UP_JOINT;
        }
        else if(user_type==3)
        {
            mode=SIGN_UP_ADMIN;
        }
        if(user_type==1 || user_type==2 || user_type==3)
        {
            printf("Enter username : \n");
            scanf("%s",username);
            printf("Enter password : \n");
            scanf("%s",password);
            sprintf(user_type_string,"%d",user_type);
    		write(sd,user_type_string,sizeof(user_type_string)); 
			write(sd,username,sizeof(username)); 
			write(sd,password,sizeof(password)); 
            // printf("Data sent\n");
            int a=read(sd,reply,buf_size*sizeof(char)); 
            // printf("%d",a);
            printf("%s\n",reply); 
	        if(!strcmp(reply,"Sign up failed\n")) 
            {
				exit(1);
            }
        }
        else
        {
            exit(1);
        }
    }
    else
    {
        // sign in
		printf("Please enter the user type for sign in\n");
        printf("Press 1 for Normal User\n");
        printf("Press 2 for Joint Account User\n");
        printf("Press 3 for Administrator\n");
        scanf("%d",&user_type);
        if(user_type==1)
        {
            mode=SIGN_IN_NORMAL;
        }
        else if(user_type==2)
        {
            mode=SIGN_IN_JOINT;
        }
        else if(user_type==3)
        {
            mode=SIGN_IN_ADMIN;
        }
		if(user_type==1 || user_type==2 || user_type==3)
        {
			printf("Enter username : \n");
			scanf("%s",username);
			printf("Enter password : \n");
			scanf("%s",password);
			sprintf(user_type_string,"%d",mode);
			write(sd,user_type_string,sizeof(user_type_string)); 
			write(sd,username,sizeof(username)); 
			write(sd,password,sizeof(password)); 
			// printf("Data sent\n");
			int a=read(sd,reply,buf_size*sizeof(char)); 
			// printf("%d",a);
			printf("%s\n",reply); 
			if(!strcmp(reply,"Sign in failed\n")) 
			{
				exit(1);
			}
		}
		else
		{
			exit(1);
		}
    }
	while (1)
	{
		if(user_type == 1 || user_type==2)
		{
			printf("What would you like us to do for you?\n");
			printf("Press 1 to Check Balance\n");
			printf("Press 2 to Deposit\n");
			printf("Press 3 to Withdraw\n");
			printf("Press 4 to change Password\n");
			printf("Press 5 to  view Details\n");
			printf("Press 6 to Exit\n");
			scanf("%d",&op);
			if(op== 1)
			{
				mode=BALANCE_EQ;
				char mode_str[buf_size];
				sprintf(mode_str,"%d",mode);
				write(sd,mode_str,sizeof(mode_str)); 
			}
			else if(op == 2)
			{
				mode=DEPOSIT;
				char mode_str[buf_size];
				sprintf(mode_str,"%d",mode);
				printf("Enter amount to be deposited\n");
				int amt;
				char amt_str[buf_size];
				scanf("%d",&amt);
				sprintf(amt_str,"%d",amt);	
				write(sd,mode_str,sizeof(mode_str)); 
				write(sd,amt_str,sizeof(amt_str));
				// printf("bytes written :- %d\n",bytes);
			}
			else if(op == 3)
			{
				mode=WITHDRAW;
				char mode_str[buf_size];
				sprintf(mode_str,"%d",mode);
				write(sd,mode_str,sizeof(mode_str)); 
				printf("Enter amount to be withdrawn\n");
				int amt;
				char amt_str[buf_size];
				scanf("%d",&amt);
				sprintf(amt_str,"%d",amt);
				write(sd, amt_str, sizeof(amt_str));
			}
			else if(op == 4)
			{
				mode=PASSWORD_CHNG;
				char mode_str[buf_size];
				sprintf(mode_str,"%d",mode);
				write(sd,mode_str,sizeof(mode_str)); 
				printf("Enter new password!\n");
				scanf("%s",password);
				write(sd,password,sizeof(password));
			}
			else if(op==5)
			{
				mode=VIEW_DETAILS;
				char mode_str[buf_size];
				sprintf(mode_str,"%d",mode);
				write(sd,mode_str,sizeof(mode_str)); 
			}
			else if(op == 6)
			{
				mode=EXIT;
				char mode_str[buf_size];
				sprintf(mode_str,"%d",mode);
				write(sd,mode_str,sizeof(mode_str)); 
				exit(0);
			}
			else
			{
				break;
			}
			int a=read(sd,reply,buf_size*sizeof(char)); 
			printf("%s\n",reply); 
		}
		else if(user_type==3)
		{
			printf("Press 1 to get User Account Details\n");
			printf("Press 2 to Add User\n");
			printf("Press 3 to Delete User\n");
			printf("Press 4 to Modify User details\n");
			printf("Press 5 to Exit\n");
			scanf("%d",&op);
			if(op==1)
			{
				mode=VIEW_DETAILS_ADMIN;
				char mode_str[buf_size];
				sprintf(mode_str,"%d",mode);
				printf("Enter username : ");
				scanf("%s",username);
				write(sd,mode_str,sizeof(mode_str)); 
				write(sd,username,sizeof(username)); 
			}
			else if(op==2)
			{
				mode=ADD_ACC;	
				char mode_str[buf_size];
				sprintf(mode_str,"%d",mode);
				int type;
				printf("Press one of the following\n");
				printf("Press 1 to add Normal user\n");
				printf("Press 2 to add Joint user\n");
				scanf("%d",&type);
				if(type==1 || type==2)
				{
					printf("Enter username\n");
					scanf("%s",username);
					printf("Enter password\n");
					scanf("%s",password);
					char type_string[buf_size];
					sprintf(type_string,"%d",type);
					write(sd,mode_str,sizeof(mode_str)); 
					write(sd,type_string,sizeof(type_string));
					write(sd,username,sizeof(username));
					write(sd,password,sizeof(password));	
				}
				else
				{
					exit(1);
				}
			}
			else if(op==3)
			{
				mode=DEL_ACC;	
				char mode_str[buf_size];
				sprintf(mode_str,"%d",mode);
				printf("Enter username\n");
				scanf("%s",username);
				write(sd,mode_str,sizeof(mode_str)); 
				write(sd,username,sizeof(username));
			}
			else if(op==4)
			{
				mode=MOD_ACC;	
				char mode_str[buf_size];
				sprintf(mode_str,"%d",mode);
				printf("Enter old username\n");
				scanf("%s",username); 
				printf("Enter new username\n");
				char newusername[buf_size];
				scanf("%s",newusername);
				printf("Enter new password\n");
				scanf("%s",password);
				printf("Enter username\n");
				scanf("%s",username);
				write(sd,mode_str,sizeof(mode_str));
				write(sd,username ,sizeof(username));
				write(sd,newusername,sizeof(newusername)); 
				write(sd,password ,sizeof(password)); 
			}
			else if(op==5)
			{
				mode=EXIT;
				char mode_str[buf_size];
				sprintf(mode_str,"%d",mode);
				write(sd,mode_str,sizeof(mode_str));
			}
			else
			{
				exit(1);
			}
		}
		// else
		// {
		// 	exit(1);
		// }
	}
}
int main()
{
    int client_socket_sd;
    struct sockaddr_in server;
    int addrlen=sizeof(server);
    

    if ((client_socket_sd = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
	{ 
		printf("Creating Socket failed...\n"); 
		return -1; 
	} 

	server.sin_family = AF_INET; 
	server.sin_port = htons(8000); 
    // server.sin_addr.s_addr = inet_addr("127.0.0.1");
	if(inet_pton(AF_INET, "127.0.0.1", &server.sin_addr)<=0) 
	{ 
		printf("Invalid address/ Address not supported \n"); 
		return -1; 
	} 

	if (connect(client_socket_sd, (struct sockaddr *)&server, sizeof(server)) < 0) 
	{ 
		printf("Connection to Server Failed \n"); 
		return -1; 
	} 
	interface(client_socket_sd);


	return 0;
    // printf("%d\n",sd);
}