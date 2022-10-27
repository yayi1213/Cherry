#include "def.h"
int stack[MAXSTACK];//堆疊的陣列宣告 
int top1=-1,top2=-1,top3=-1,top4=-1; int wtop =-1;//堆疊的頂端
int isEmpty();/*判斷是否為空堆疊*/
void push1(int data); /*將xi放入堆疊*/
int pop1();  /*從堆疊取出xi*/
void push2(int data); /*將yi放入堆疊*/
int pop2();/*從堆疊取出yi*/
void push3(int data); /*將xj放入堆疊*/
int pop3();/*從堆疊取出xj*/
void push4(int data); /*將yj放入堆疊*/
int pop4();/*從堆疊取出yj*/
void wpush(char *data);/*從堆疊取出最終位置之值*/
char *wpop();/*從堆疊取出最終位置之值*/
int initRow[MAXSTACK],initCol[MAXSTACK],goalRow[MAXSTACK],goalCol[MAXSTACK];char *wstack[MAXSTACK];//堆疊之陣列

