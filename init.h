#define SIGN_UP_NORMAL 1
#define SIGN_UP_JOINT 2
#define SIGN_UP_ADMIN 3
#define SIGN_IN_NORMAL 4
#define SIGN_IN_JOINT 5
#define SIGN_IN_ADMIN 6
#define DEPOSIT 7
#define WITHDRAW 8
#define BALANCE_EQ 9
#define PASSWORD_CHNG 10
#define VIEW_DETAILS 11
#define ADD_ACC 12
#define DEL_ACC 13
#define MOD_ACC 14
#define SEARCH_ACC 15
#define VIEW_DETAILS_ADMIN 16
#define EXIT 17
#define buf_size 1000
#define SIGN_UP 18
#define SIGN_IN 19
struct User 
{
    char username[50];
    char password[50];
    int type;
    int active;
} User;
struct Account
{
    int balance;
} Account;
