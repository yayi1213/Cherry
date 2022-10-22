#include"stack.h"
#include<stdio.h>
#define MAX 400

// void pushFunction(void);/*新增函數*/
// void popFunction(void);/*刪除函數*/
// void listFunction (void); /*翰出函數*/
// char item[MAX][20];
// int top=-1;/*top的起始值為-1*/
int main(){
char option;
while (1) {
printf("\n******************************\n");
printf("       <1> insert (push) \n");
printf("       <0> delete (pop) \n") ;
// printf("       <3> list \n");
// printf("       <4> quit\n");
printf("******************************\n");
printf(" Please enter your choice...");
option = getchar();
while (getchar()!='\n')
continue;
    switch(option) {
    case'1':
    pushFunction();
    break;
    case'0':
    popFunction();
    break;
    // case'3':
    // listFunction();
    // break;
    // case '4':
    printf("\n");
    return 0;
        }
    }
}
    void pushFunction(void)
    {
    if (top >= MAX-1)/* 當堆疊已滿•則顯示錯誤 */
        printf("\n Stack is full !\n");
    else {
        top++;
        //printf("\n\n Please enter item to insert: ");
        fgets(item[top]);
         }
    }

    void popFunction(void)
    {
    if (top < 0)/*當堆疊没有資料存在，則顯示錯誤 */
        printf("\n\n No item, stack is empty ! \n");
    else {
        printf(" \n\n Item %s deleted\n", item[top]);
        top--;
    }
    }

    void listFunction(void)
    {
        int count = 0,i;
    if(top < 0)
        printf("\n\n No item, Stack is empty");
    else{
        printf("\n\n ITEM");    
        printf("-------------------\n");
        for(i=0;i<=top;i++){
            printf(" %-20s\n ",item[i]);
            count++;
        }
        printf("-------------------\n");
        printf("TOTAL item:%d\n",count);
        }   
    }

