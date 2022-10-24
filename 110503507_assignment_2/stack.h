//#include<string.h>
#define MAXSTACK 500/*定義最大堆疊容量*/
int stack[MAXSTACK];  //堆疊的陣列宣告 
int top1=-1,top2=-1,top3=-1,top4=-1,top5=-1,top6=-1;		//堆疊的頂端
int isEmpty();
void push1(int data); 
int pop1();
void push2(int data); 
int pop2();
void push3(int data); 
int pop3();
void push4(int data); 
int pop4();
void push5(char data5[]); 
char pop5();
void push6(char data6[]); 
char pop6();
int initRow[MAXSTACK],initCol[MAXSTACK],goalRow[MAXSTACK],goalCol[MAXSTACK];
char initPlace[MAXSTACK][500],goalPlace[MAXSTACK][500];
