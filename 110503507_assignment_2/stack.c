#include <stdio.h>
#include <stdlib.h>
#include "stack.h"
/*將資料放入堆疊*/
 void push1(int data1){
	if(top1>=MAXSTACK){
		printf("堆疊已滿,無法再加入\n");	
	}else{
		top1++;
		initRow[top1]=data1;
	}
} 
/*從堆疊取出資料*/
int pop1(){
	int data1;
		data1=initRow[top1];
		top1--;
		return data1; 
}
/*將資料放入堆疊*/
void push2(int data2){
	if(top2>=MAXSTACK){
		printf("堆疊已滿,無法再加入\n");	
	}else{
		top2++;
		initCol[top2]=data2;
	}
} 
/*從堆疊取出資料*/
int pop2(){
	int data2;
		data2=initCol[top2];
		top2--;
		return data2; 
}
/*將資料放入堆疊*/
void push3(int data3){
	if(top3>=MAXSTACK){
		printf("堆疊已滿,無法再加入\n");	
	}else{
		top3++;
		goalRow[top3]=data3;
	}
} 
/*從堆疊取出資料*/
int pop3(){
	int data3;
		data3=goalRow[top3];
		top3--;
		return data3; 
}
/*將資料放入堆疊*/
void push4(int data4){
	if(top4>=MAXSTACK){
		printf("堆疊已滿,無法再加入\n");	
	}else{
		top4++;
		goalCol[top4]=data4;
	}
} 
/*從堆疊取出資料*/
int pop4(){
	int data4;
		data4=goalCol[top4];
		top4--;
		return data4; 
}

/*將資料放入堆疊*/
void push5(char data5)
{if(top5>=MAXSTACK){
		printf("堆疊已滿,無法再加入\n");	
	}else{
		top5++;
		initplace[top5]=data5;
	}
} 
/*從堆疊取出資料*/
char pop5()
{char data5;
		data5=goalCol[top5];
		top5--;
		return data5; 
}
/*將資料放入堆疊*/
void push6(char data6)
{if(top6>=MAXSTACK){
		printf("堆疊已滿,無法再加入\n");	
	}else{
		top6++;
		initplace[top6]=data6;
	}
} 
/*從堆疊取出資料*/
char pop6()
{char data6;
		data6=goalCol[top6];
		top6--;
		return data6; 
}
/*判斷是否為空堆疊*/
int isEmpty(){
	if(top1==-1){
		return 1; 
	}else{
		return 0;
	}
} 
