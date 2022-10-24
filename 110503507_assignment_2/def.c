#include "def.h"
#define ROW 10
#define COLUMN 10
#define MAXSTACK 500
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
char s_board[ROW][COLUMN];
char board[ROW][COLUMN];

struct data
{
 int from_s[2];
 int to_s[2];
 char f_chess;
 char t_chess;
}

movement[MAXSTACK];

int top=-1;

// int isEmpty()
// {
//  if(top==-1) 
//  {
//   return 1;
//  }
//  else
//  {
//   return 0;
//  }
// }

int push(int from1, int from2, int to1,int to2)
{
 if(top>=MAXSTACK)
 {
  printf("the stack is full\n");
  return 0;
 }
 else
 {
  top++;  
  movement[top].from_s[0] = from1;
  movement[top].from_s[1] = from2;
  movement[top].to_s[0] = to1;
  movement[top].to_s[1] = to2;
  movement[top].f_chess = board[from1][from2];
  movement[top].t_chess = board[to1][to2];
  return 1;
 }
}

// int goback()
// {
//  if(isEmpty())
//  {
//   return -1;
//  } 
//  else
//  {
//   board[movement[top].from_s[0]][movement[top].from_s[1]] = movement[top].f_chess;
//   board[movement[top].to_s[0]][movement[top].to_s[1]] = movement[top].t_chess;
//   top--;
//   return 0;
//  }
//}

void record_Board(void) 
{
 for(int i = 0;i<ROW;i++)
 {
  for(int j=0;j<COLUMN;j++)
  {
   s_board[i][j] = board[i][j];
  } 
 }
}

void replay(void) /*印出開始後的所有步伐*/
{
 for(int i = 0;i<ROW;i++)
 {
  for(int j =0;j<COLUMN;j++)
  {
   board[i][j] = s_board[i][j];
  }
 }
 for(int i = 0;i<=top;i++)
 {
  board[movement[i].to_s[0]][movement[i].to_s[1]] = board[movement[i].from_s[0]][movement[i].from_s[1]];
  board[movement[i].from_s[0]][movement[i].from_s[1]] = ' ';
  printf("*****%d*****\n", i+1);
  //chessboard();
 }
}