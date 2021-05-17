#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<netinet/in.h>
#include<stdlib.h>
#include"init.h"
#include<string.h>
int signup(int mode, char* username, char* password)
{
    char file[100],ext[5] = ".txt";
    strcpy(file,username);
    strncat(file,ext,sizeof(ext));
    // opening existing file
    int fd=open(file,O_WRONLY);
    // if file already exist
    if(fd != -1)
    {
        return -1;
    } 
    // if file does not exist
    else 
    {
        close(fd);
    }
    // creating new file for user
    fd=open(file,O_WRONLY | O_CREAT,0744);
    if(fd == -1)
    {
        perror("signup");
        return -1;
    }
    struct User usr;
    strcpy(usr.username,username);
    strcpy(usr.password,password);
    usr.type=mode;
    struct Account acnt;
    acnt.balance=0;
    // writing user details in the user file
    write(fd,&usr,sizeof(User));
    // writing account balance in the user file
    write(fd,&acnt,sizeof(Account));
    // closing file desc
    close(fd);
    return 0;
}
int signin(int mode, char* username, char* password)
{
    static struct flock lock;
    lock.l_type = F_RDLCK;
    lock.l_start = 0;
    lock.l_whence = SEEK_SET;
    lock.l_len = sizeof(struct User);
    lock.l_pid = getpid();
    char filename[buf_size];
    strcpy(filename,username);
    char extension[5] = ".txt";
    strncat(filename,extension,sizeof(extension));
    int fd = open(filename,O_RDONLY,0644);
    if(fd == -1)
    {
        perror("Sign in"); return -1;
    }
    struct User usr;
    if(fcntl(fd, F_SETLKW, &lock)==-1) {
        perror("fcntl") ; 
        return -1;
    }
    // critical section start
    lseek(fd,0,SEEK_SET);
    read(fd,&usr,sizeof(struct User));
    if((strcmp(usr.password,password)!=0) || (mode==SIGN_IN_NORMAL && (usr.type!=1)) || (mode==SIGN_IN_ADMIN && (usr.type!=2))|| (mode==SIGN_IN_JOINT && (usr.type!=2))) 
    {
        return -1;
    }
    // critical section end
    lock.l_type = F_UNLCK;
    fcntl(fd,F_SETLKW,&lock);
    close(fd);
    return 0;
}
int deposit(char* username, int amount)
{
    // printf("deposit function\n");
    static struct flock lock;
    lock.l_type = F_WRLCK;
    lock.l_start = sizeof(struct User);
    lock.l_whence = SEEK_SET;
    lock.l_len = sizeof(struct Account);
    lock.l_pid = getpid();
    char filename[buf_size];
    strcpy(filename,username);
    char extension[5] = ".txt";
    int fd;
    strncat(filename,extension,sizeof(extension));
    fd = open(filename,O_RDWR,0744);
    if(fd == -1)
    {
        perror("signin");
        return -1;
    }
    struct Account acct;
    if(fcntl(fd, F_SETLKW, &lock)==-1) 
    {
        perror("fcntl") ; 
        return -1;
    }
    // critical section start
    lseek(fd,sizeof(struct User),SEEK_SET);
    if(read(fd,&acct,sizeof(struct Account))==-1) 
    {
        perror("read"); 
        return -1;
    }
    // printf("%d\n",acct.balance);
    acct.balance= acct.balance+amount;
    // printf("%d\n",acct.balance);
    lseek(fd,sizeof(struct User),SEEK_SET);
    if(write(fd,&acct,sizeof(struct Account))==-1) 
    {
        perror("write"); 
        return -1;
    }
    // critical section end
    lock.l_type = F_UNLCK;
    fcntl(fd,F_SETLKW,&lock);
    close(fd);
    return 0;
}
int password_change(char* username, char* password)
{
    static struct flock lock;
    lock.l_type = F_WRLCK;
    lock.l_start = 0;
    lock.l_whence = SEEK_SET;
    lock.l_len = sizeof(struct User);
    lock.l_pid = getpid();
    char filename[buf_size];
    strcpy(filename,username);
    char extension[5] = ".txt";
    int fd;
    strncat(filename,extension,sizeof(extension));
    fd = open(filename,O_RDWR,0744);
    if(fd == -1)
    {
        perror("change pwd"); 
        return -1;
    }
    struct User usr;
    lseek(fd,0,SEEK_SET);
    if(fcntl(fd, F_SETLKW, &lock)==-1) {
        perror("fcntl") ; 
        return -1;
    }
    // critical section start
    if(read(fd,&usr,sizeof(struct User))==-1) 
    { 
        perror("read"); 
        return -1; 
    }
    strcpy(usr.password,password);
    lseek(fd,0,SEEK_SET);
    if(write(fd,&usr,sizeof(struct User))==-1) 
    { 
        perror("write"); 
        return -1; 
    }
    // critical section end
    lock.l_type = F_UNLCK;
    fcntl(fd,F_SETLKW,&lock);
    close(fd);
    return 0;
}
int withdraw(char* username, int amt)
{
    static struct flock lock;
    lock.l_type = F_WRLCK;
    lock.l_start = sizeof(struct User);
    lock.l_whence = SEEK_SET;
    lock.l_len = sizeof(struct Account);
    lock.l_pid = getpid();
    char filename[buf_size];
    strcpy(filename,username);
    char extension[5] = ".txt";
    strncat(filename,extension,sizeof(extension));
    int fd = open(filename,O_RDWR,0744);
    if(fd == -1)
    {
        perror("signin"); 
        return -1;
    }
    struct Account acct;
    if(fcntl(fd, F_SETLKW, &lock)==-1) {
        perror("fcntl") ; 
        return -1;
    }
    // critical section start
    lseek(fd,sizeof(struct User),SEEK_SET);
    if(read(fd,&acct,sizeof(struct Account)) == -1) {
        perror("read"); 
        return -1;
    }
    printf("balance = %d\n",acct.balance);
    acct.balance = acct.balance-amt;
    if(acct.balance < 0) return -1;
    lseek(fd,sizeof(struct User),SEEK_SET);
    if(write(fd,&acct,sizeof(struct Account))==-1) {
        perror("write"); 
        return -1;
    }
    // critical section end
    lock.l_type = F_UNLCK;
    fcntl(fd,F_SETLKW,&lock);
    close(fd);
    return 0;
}
int get_balance(char* username)
{
    static struct flock lock;
    lock.l_type = F_RDLCK;
    lock.l_start = sizeof(struct User);
    lock.l_whence = SEEK_SET;
    lock.l_len = sizeof(struct Account);
    lock.l_pid = getpid();
    char filename[buf_size];
    strcpy(filename,username);
    char extension[5] = ".txt";
    int fd;
    strncat(filename,extension,sizeof(extension));
    fd = open(filename,O_RDONLY,0644);
    if(fd == -1)
    {
        perror("signin"); return -1;
    }
    struct Account acct;
    if(fcntl(fd, F_SETLKW, &lock)==-1) 
    {
        perror("fcntl") ; 
        return -1;
    }
    // critical section start
    lseek(fd,sizeof(struct User),SEEK_SET);
    if(read(fd,&acct,sizeof(struct Account))==-1) 
    {
        perror("read");
    }
    // critical section end
    lock.l_type = F_UNLCK;
    fcntl(fd,F_SETLKW,&lock);
    close(fd);
    return acct.balance;
}
char* view_details(char* username)
{
    static struct flock lock;
    lock.l_type = F_RDLCK;
    lock.l_start = 0;
    lock.l_whence = SEEK_SET;
    lock.l_len = 0;
    lock.l_pid = getpid();
    char filename[buf_size];
    strcpy(filename,username);
    char extension[5] = ".txt";
    int fd;
    strncat(filename,extension,sizeof(extension));
    fd = open(filename,O_RDWR,0644);
    if(fd == -1)
    {
        perror("open"); 
        return "user does not exist\n";
    }
    struct Account acct;
    struct User usr;
    if(fcntl(fd, F_SETLKW, &lock)==-1) 
    {
        perror("fcntl"); 
        return "sorry, section is locked\n";
    }
    // start of critical section
    lseek(fd,0,SEEK_SET);
    if(read(fd,&usr,sizeof(struct User))==-1)    
    {
        perror("read"); return "unable to read file\n";
    }
    if(read(fd,&acct,sizeof(struct Account))==-1)
    {
        perror("read"); return "unable to read file\n";
    }
    // end of critical section
    lock.l_type = F_UNLCK;
    fcntl(fd,F_SETLKW,&lock);
    char* str = (char*)malloc(buf_size * sizeof(char));
    char usr_string[buf_size];
    if(usr.type==1)
    {
        strncpy(usr_string,"Normal User",buf_size);
    }
    else if(usr.type==2)
    {
        strncpy(usr_string,"Joint User",buf_size);
    }
    else if(usr.type==3)
    {
        strncpy(usr_string,"Admin",buf_size);
    }
    sprintf(str,"username - %s \npassword - %s \nuser_type - %s\nAccount Balance : Rs %d\n",usr.username,usr.password,usr_string,acct.balance);
    close(fd);
    return str;
}