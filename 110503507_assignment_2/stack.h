#include<stdio.h>
#define MAX 400

void pushFunction(void);/*新增函數*/
void popFunction(void);/*刪除函數*/
void listFunction (void); /*翰出函數*/
char item[MAX][20];
int top=-1;/*top的起始值為-1*/