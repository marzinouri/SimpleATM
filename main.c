#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>

FILE *fptr;

struct date {
    int sec;
    int min;
    int hour;
    int day;
    int month;
    int year;
    long long amount;
    char type[20];
};//to save a structure of transactions

struct info {
    int transactionNo;
    char cardNo[5];
    char pass[3];
    char Name[51];
    long long accounts;
    struct date transaction[21];
};
typedef struct info info;

info data[10000]; //a global array of structs
int filled_elements=0; // a global parameter which keeps the number of filled elements

void mainMenu (void);
int card_exists (char *CardNO);
int check_pass(char *pass);
void new_acc();
int matched (char* card_number,char *password);
void menu();
void return_info (char *card_number);
int able_to_withdraw(char *card_number,long long cash);
int withdraw(char *card_number,long long cash);
void save_transaction(int z,long long cash,char *type);
int able_to_deposit(char *card_number,long long cash);
int deposit(char *card_number,long long cash);
void change_pass(char *card_number,char *confirmed_pass);
int able_to_transfer(char *card_number ,long long cash);
void transfer (char *card_number);
int compare (void *a,void *b);
void transactions(char *card_number);
void write ();
void read();

int main ()
{
    read();
    int choice=0;
    while(choice!=3)
    {   mainMenu();
        printf("Please Enter Your Choice:\n");
        scanf("%d",&choice);
        if (choice==1)
        {
            new_acc();
            write();
        }
        else if (choice==2)
        {
            menu();
            write();
        }
        else
            return 0;
    }
    write();
}

void mainMenu(void)
{
    printf("\n        Banking Management System\n\n\n");
    printf("$$$$$$$ WELCOME TO THE MAIN MENU $$$$$$$\n\n");
    printf("1.Add A New Account\n\n2.Current Account\n\n3.Exit\n\n\n");
    system("color 17");
} //to show the first menu with 3 choices

int check_number(char *CardNo) //to find out whether the card number is valid or not.If yes,it returns 1,else it returns 0
{
     if(strlen(CardNo)!=4) //check number of digits
    {
       return 0;
    }
    int i=0;
    while (CardNo[i]!=NULL)
    {
          if(CardNo[i]<'0' || CardNo[i]>'9') //check if the characters are digits
          {
              return 0;
          }
          i++;
    }

    return 1;
}

int check_pass(char *pass) //to find out whether the password is valid or not.If yes,it returns 1,else it returns 0
{
    if(strlen(pass)!=2)
    {
        return 0;
    }
    int i=0;
    while(pass[i]!=NULL)
    {
        if(pass[i]<'0' || pass[i]>'9')
        {
            return 0;
        }
        i++;
    }
    return 1;
}

int matched (char* card_number,char *password) //to find out whether the card number which already exists and the given number as password match or not.if yes it returns 1,else it returns 0
{
    int i;
    for(i=0;i<filled_elements;i++)
    {
        if(strcmp(data[i].cardNo,card_number)==0) //if the card number is found...
        {
            if(strcmp(data[i].pass,password)==0) //and the password matches ...
            {
                return 1;
            }
            else
                return 0;
        }
    }
    return 0;
}

void return_info (char *card_number) //returns the card number,password and the balance
{
    int z;
    for(z=0;z<filled_elements;z++)
    {
        if(strcmp(data[z].cardNo,card_number)==0) // if the card number is found
        {
            printf("\nCard number:%s\tPassword:%s\tBalance:%lld\n\n\n",data[z].cardNo,data[z].pass,data[z].accounts);
        }

    }
}

int able_to_withdraw(char *card_number,long long cash) //checks if the account is ok with withdraw!.if it is,it returns 1,else it returns 0
{
    int c;
    if(cash>2000000)
    {
        return 0;
    }
    for(c=0;c<filled_elements;c++)
    {
        if(strcmp(data[c].cardNo,card_number)==0)
        {
            if(data[c].accounts<cash)
            {
                return 0;
            }
            else
            {
                return 1;
            }

        }
    }
    return 0;
}

void save_transaction(int z,long long cash,char *type)
{
    time_t t = time(NULL);
    struct tm* tm = localtime(&t);
    data[z].transaction[data[z].transactionNo].amount=cash;
    strcpy(data[z].transaction[data[z].transactionNo].type,type);


    data[z].transaction[data[z].transactionNo].day=tm->tm_mday;
    data[z].transaction[data[z].transactionNo].month=tm->tm_mon+1;
    data[z].transaction[data[z].transactionNo].year=tm->tm_year+1900;
    data[z].transactionNo++;
}

int able_to_deposit(char *card_number,long long cash) //checks if the account is ok with withdraw!.if it is,it returns 1,else it returns 0
{
    int c;
    if(cash>30000000)
    {
        return 0;
    }
    else
        return 1;
}

int deposit(char *card_number,long long cash) //it does the withdraw action
{
    char confirm[2];
    int z;
    if(able_to_deposit(card_number,cash))
    {
        printf("Are you sure?\nIf yes,press Y,else press N\n");
        scanf("%s",confirm);
    }
    else
    {
        printf("Sorry.The amount of money you can deposit is between your balance and 3 million tomans.\n\n\n");
    }
    if(strcmp(confirm,"Y")==0)
        {
            for(z=0;z<filled_elements;z++) //it tries to find the card number
            {
                if(strcmp(data[z].cardNo,card_number)==0)
                {
                    data[z].accounts+=cash;
                    printf("\ncash=%lld\nNew balance=%lld\n\n\n",cash,data[z].accounts);
                    save_transaction(z,cash,"Deposit");
                    break;
                }
            }
            return 1;
        }
        else
            return 0;
}

int withdraw(char *card_number,long long cash) //it does the withdraw action
{
    char confirm[2];
    int z;
    if(able_to_withdraw(card_number,cash))
    {
        printf("Are you sure?\nIf yes,press Y,else press N\n");
        scanf("%s",confirm);
    }
    else
    {
        printf("Sorry.The amount of money you can withdraw is between your balance and 5million tomans.\n");
    }
    if(strcmp(confirm,"Y")==0)
        {
            for(z=0;z<filled_elements;z++) //it tries to find the card number
            {
                if(strcmp(data[z].cardNo,card_number)==0)
                {
                    data[z].accounts-=cash;
                    printf("cash=%lld\nremained=%lld\n\n\n",cash,data[z].accounts);
                    save_transaction(z,cash,"Withdraw");
                    break;
                }
            }
            return 1;
        }
        else
            return 0;
}

void change_pass(char *card_number,char *confirmed_pass) //it gets 2 arguments.1:card number and 2:pass which has been checked before
{

    int m=0;
    for (m=0;m < filled_elements;m++)
    {
        if(strcmp(data[m].cardNo,card_number)==0)
        {
            strcpy(data[m].pass,confirmed_pass);
        }
    }
}

int able_to_transfer(char *card_number ,long long cash) //checks if the money can be transfered
{
    int c;
    if(cash>50000000)
    {
        return 0;
    }
    for(c=0;c<filled_elements;c++)
    {
        if(strcmp(data[c].cardNo,card_number)==0)
        {
            if(data[c].accounts<cash)
            {
                return 0;
            }
            else
            {
                return 1;
            }

        }
    }
    return 0;
}

void transfer (char *card_number)
{
    char to[20];
    long long amount;
    int n=0;
    printf("Please enter the amount of money you want to transfer:\n");
    scanf("%lld",&amount);
    if(able_to_transfer(card_number,amount))
    {
        printf("Please enter destination card number:\n");
        scanf("%s",to);
        if(card_exists(to))
        {
            for(n=0;n<filled_elements;n++)
            {
                if(strcmp(to,data[n].cardNo)==0)
                {
                    char h[2];
                    int k=0;
                    printf("Destination card number:%s\nreceiver\'s name:%s\namount:%lld\n\n",data[n].cardNo,data[n].Name,amount);
                    printf("Are you sure?\nIf yes,press Y,else press N:\n");
                    scanf("%s",h);
                    if(strcmp("Y",h)==0)
                    {
                        data[n].accounts+=amount;
                        save_transaction(n,amount,"Deposit(Transfer)");
                        for(k=0;k<filled_elements;k++)
                        {
                            if(strcmp(data[k].cardNo,card_number)==0)
                            {
                                data[k].accounts-=amount;
                                save_transaction(k,amount,"Withdraw(Transfer)");
                                break;
                            }
                        }
                        printf("source card number:%s\ndestination card number:%s\namount:%lld",card_number,to,amount);
                    }
                    else
                    {
                        return;
                    }
                    }//if

                    }//for
                    }//if
                        else
                        {
                            printf("Cannot transfer.\n\n\n");
                        }
    }

}

int compare (void *a,void *b)
{
    struct date *n1,*n2;
    n1=(struct date*)a;
    n2=(struct date*)b;
    if(n1->year > n2->year)
    {
        return -1;
    }
    else if(n1->year < n2->year)
    {
        return 1;
    }
    else if(n1->year==n2->year)
    {
        if(n1->month > n2->month)
            return -1;
        else if(n1->month < n2->month)
            return 1;
        else
        {
            if(n1->day > n2->day)
                return -1;
            else if (n1->day < n2->day)
                return 1;
            else
            {
                if(n1->hour > n2->hour)
                    return -1;
                else if(n1->hour < n2->hour)
                    return 1;
                else
                {
                    if(n1->min > n2->min)
                        return -1;
                    else if (n1->min < n2->min)
                        return 1;
                    else
                    {
                        if(n1->sec > n2->sec)
                            return -1;
                        else if(n1->sec < n2->sec)
                            return 1;
                        else
                            return 0;
                    }
                }

            }
                return 0;
        }
    }
    return 0;

}

void transactions(char *card_number)
{
    int k=0,z=0,wanted,printed=0;
    printf("Please enter the number of transactions you want to see:\n");
    scanf("%d",&wanted);
    for(k=0;k<filled_elements;k++)
    {
        if(strcmp(data[k].cardNo,card_number)==0)
        {
            if(data[k].transactionNo <wanted)
            {
                printed=data[k].transactionNo;
            }
            else
                printed=wanted;
            qsort(data[k].transaction,data[k].transactionNo,sizeof(struct date),compare);
            for(z=0;z<printed;z++)
            {
                printf("second  minute  hour\tday/month/year \tcash\ttype\n");
                printf("%6d  %6d  %4d\t%3d/%5d/%4d\t%4lld\t%4s\n",
                       data[k].transaction[z].sec,
                       data[k].transaction[z].min,
                       data[k].transaction[z].hour,
                       data[k].transaction[z].day,
                       data[k].transaction[z].month,
                       data[k].transaction[z].year,
                       data[k].transaction[z].amount,
                       data[k].transaction[z].type);
            }
           // printf("number of transactions:%d\n",data[k].transactionNo);
        }
    }
}

void menu (void) //if the second choice is token
{
    char card_number[100],password[10];
    printf("Please enter your card number:\n");
    scanf("%s",card_number);
    if (card_exists(card_number)==0 || check_number(card_number)==0) //checks 1:if the card exists 2:if the number is valid
    {
        printf("The card number is invalid.\n\n");
        return;
    }
    else
    {
        int j=0;
        printf("Please enter your password:\n");
        scanf("%s",password);

        for (j=0; j<2 && !matched(card_number,password);j++) //if goes for 3 times if the password is incorrect
        {
            printf("Your password did not match.Please try again:\n");
            scanf("%s",card_number);
            //uncompleted
        }
        if (j==2)
        {
            printf("Entered password did not match and your card is sentenced\n\n"); //the card will be recorded
        }
        else
        {
            int order=0;
            while(order!=7)
            {
                printf("1.Account balance\n\n2.Withdraw cash\n\n3.Deposit cash\n\n4.Change password\n\n5.Transfer\n\n6.View recent transaction reports\n\n7.Return to the main menu\n\n\n");
                printf("Please enter your choice:\n\n");
                scanf("%d",&order);
                if (order==1)
                {
                    return_info(card_number);
                }
                else if (order==2)
                {
                    long long cash;
                    printf("Please enter the amount of money you want to withdraw:\n");
                    scanf("%lld",&cash);
                    withdraw(card_number,cash);
                }
                else if (order==4)
                {
                    int k=0;
                    char new_pass[19],confirmed_pass[20];
                    printf("Please enter your current password:\n");
                    scanf("%s",password);
                    printf("Please enter the new password:\n");
                    scanf("%s",new_pass);//agar bishtar az 2 ragham bud...
                    printf("Please renter the new password to confirm:\n");
                    scanf("%s",confirmed_pass);
                    if(strcmp(new_pass,confirmed_pass)==0)
                    {
                        change_pass(card_number,confirmed_pass);
                    }
                    else
                    {
                        printf("The passwords did not match.\n");
                    }}
                    else if(order==3)
                    {
                        long long money;
                        printf("Please enter the amount of money you want to deposit:\n");
                        scanf("%lld",&money);
                        deposit(card_number,money);
                    }
                    else if (order==5)
                    {
                        transfer(card_number);
                    }
                    else if (order==6)
                    {
                        transactions(card_number);
                        write();
                    }
}
    return;

}}}

int card_exists (char *CardNo)
{
    int i=0;
    while (i<filled_elements)
    {
            if(strcmp(data[i].cardNo,CardNo)==0)
           {
               return 1;
           }
    i++;
    }
    return 0;
}

void write ()
{
    fptr=fopen("bank.bin","wb");
    if(fptr!=NULL)
    {
       fwrite (&filled_elements,sizeof(int),1,fptr);
       fwrite(data,sizeof(info),10000,fptr);
       fclose(fptr);
    }
}

void read()
{
    fptr=fopen("bank.bin","rb");
    if (fptr!=NULL)
    {
        fread (&filled_elements,sizeof(int),1,fptr);
        fread(data,sizeof(info),10000,fptr);
        fclose(fptr);
    }
    //else return;
}

void new_acc()
{
    char card_number[100],password[10],name[100];
    long long accounts=0;
    int i=0;
    printf("Please enter your account number:\n");
    scanf("%s",card_number);
    while (!check_number(card_number) || card_exists(card_number))//card exists
    {
        printf("Your card number has to be exactly 4 digits.or the entered card number is already in use.Please try again.\n");
        scanf("%s",card_number);
    }

    printf("Please enter your password:\n");
    scanf("%s",password);
    while(!check_pass(password))
    {
        printf("The password must contain exactly 2 digits.Please try again:\n");
        scanf("%s",password);
    }
    printf("Please enter your name:\n");
    scanf("%s",name);
    printf("Please enter balance\n");
    scanf("%lld",&accounts);
    printf("\n");
    strcpy(data[filled_elements].cardNo,card_number);
    strcpy(data[filled_elements].pass,password);
    strcpy(data[filled_elements].Name,name);
    data[filled_elements].accounts=accounts;
    data[filled_elements].transactionNo=0;
    filled_elements++;
    printf("\nYour account successfully created!\n\n");

}
