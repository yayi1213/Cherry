#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
#include "def.h"
#include <ev.h>

FILE *record=NULL;//檔案名稱
//定義外部變量,棋盤坐標
char* chessPosition[10][10];
char* s_chessPosition[10][10];
int b1,xi,yi;//要移動的棋子位置
int b2,xj,yj;//移動的目標位置
bool isStandard = 1;//是否符合規則，初始值1，符合
bool gameOverSign = 1;//遊戲是否結束，0結束
bool restart = 0;//雙方是否交換
int turn = -1;
int count=0;
//生成棋盤
void chessBoardBuilding();
//打印棋盤
void printChessboard();
//判斷是紅棋還是藍棋,紅旗返回-1,藍棋返回1,否則返回0
int redOrBlue(int x,int y);
//紅棋移動
void redMove();
//藍棋移動
void blueMove();
//每種棋子的規則
void rulesOfAllKindsOfChessPieces();
//判斷遊戲結束
void isGameOver();
//建立檔案
void createRecord();
//查詢舊檔
void readOldGame();
//讀取舊檔
void reviesOldGame();
//存取目前資料
void saveRecord();
//刪除資料
void deleteRecord(FILE *fptr);
//加入鏈結
void insert(int data1, int data2,int data3,int data4,char *data5);
//重播
void replay();
//印出過去棋盤
void printPastChessboard();
//紀錄棋盤
void keepBoard(void);
//**************************鏈結*******************************
//結構生成
struct node {
   int xfrom,yfrom,xto,yto;
   char *destination;
   struct node *next;
   struct node *prev;
};
typedef struct node NODE;
NODE *stack=NULL;

//insert link at the last location
void insert(int data1, int data2,int data3,int data4,char *data5) {
   //產生link
    NODE *link = (NODE*) malloc(sizeof(NODE));
    if(link!=NULL){
    link->xfrom = data1;
	link->yfrom = data2;
    link->xto = data3;
    link->yto = data4;
    link->destination = data5;
    link->next = stack;
    stack=link;
    }
}

//紀錄棋盤
void keepBoard(void) 
{
 for(int i = 0;i<ROW;i++)
 {
  for(int j=0;j<COLUMN;j++)
  {
   s_chessPosition[i][j] = chessPosition[i][j];
  } 
 }
}
//悔棋
int goback()
{
 NODE *link=NULL;
  if(stack==NULL){return -1;}
    int a,b,c,d;
    char *tmp;
    link = stack;
    stack = stack->next;
    a=link->xfrom;b=link->yfrom;c=link->xto;d=link->yto;
    tmp= link->destination;
    chessPosition[a][b]=chessPosition[c][d];
    chessPosition[c][d]=tmp;
    deleteRecord(record);
    printChessboard(); 
    free(link);
    return 0;
}
NODE *inverse()
{
 NODE *front, *fnext, *list;
 list = stack;
 front = NULL;
 while(list)
 {
  fnext = front;
  front = list;
  list = list->next;
  front->next = fnext;
 }
 return(front);
}
/*印出開始後的所有步伐*/
void replay() 
{
 NODE *link;
 if(link->prev==NULL){printf("can't replay");}
 int t = 0;
  for(int i = 0;i < ROW;i++)
 {
  for(int j = 0;j < COLUMN;j++)
  {
   chessPosition[i][j] = s_chessPosition[i][j];
  }
 }
 link = inverse();
 do{
    int a,b,c,d;
    a=link->xfrom;b=link->yfrom;c=link->xto;d=link->yto;
    chessPosition[c][d]=chessPosition[a][b];
    chessPosition[a][b]=GAP;
 
  printf("\n****** %d step ********\n", t+1);
  printPastChessboard();
  link = link->next;
  t++;
 }
 while(link);
}
//**************************主函數******************************
int main()
{
    
    //生成棋盤
    chessBoardBuilding(); 
    //打印棋盤
    printChessboard();
    keepBoard();
     if ( ( record= fopen ("record.txt", "w+")) == NULL) //建立檔案
    {
        printf("Fail to open the file\n");
    }
    else
    {
        //printf("Record File open successfully\n");
    }
    //開始下棋
    while (gameOverSign) {
        isStandard = 1;
        turn *= (-1);//雙方交替下棋     
        switch (turn) {
            case 1:                
                blueMove();
                turn = (restart) ? (turn*-1) : turn;
                break;
            case -1:
                redMove();
                turn = (restart) ? (turn*-1) : turn;
                break;
        }       
        isGameOver();//判斷遊戲結束與否   
    }

    bool redWin = 0;bool blueWin = 0;//判斷獲勝方
    for (int i = 0; i < ROW; i ++) {
        for (int j = 0; j < COLUMN; j ++) {
            if (chessPosition[i][j] == Red(王)) redWin = 1;
            else if (chessPosition[i][j] == Blue(王))blueWin = 1;   
        }
    }
    if (redWin == 0){printf("VICTORY!玩家X獲勝\n\n");}
    if (blueWin == 0){printf("GAMEOVER!玩家X輸了\n\n");}
    
}//主函數結束


//*************************自定義函數*****************************
/*選擇進行之動作*/
void choose_Option1()
{
 char option[10];
 fflush(stdin);
 while(1)
 {
  printf("玩家X\nchoose the option(1:play/0:back to last step/s:save the record/x:replat the board): ");
  count++;
  scanf(" %s", option);
  if(option[0] == '1')//輸入1,下棋
  {
   printf("continue\n");
   return ;
  }
 
  if(option[0] == '0')//輸入0,悔棋
  {
    
   if(goback())
   {
    printf("can't go back\n");
    continue;
   }
   else
   {
    redMove();
   continue;
   }
  }
  
    if(option[0] =='s')//輸入s,存取資料
    {
        saveRecord();
        continue;
    }
    if(option[0] =='x')//輸入x,重播
    {
        replay();
        return;
    }
  }
  
}
/*選擇進行之動作*/
void choose_Option2()
{
 char option[10];
 fflush(stdin);
 while(1)
 {
  printf("玩家Y\nchoose the option(1:play/0:back to last step/s:save the record/x:replat the board): ");
  count++;
  scanf(" %s", option);
  if(option[0] == '1')//輸入1,下棋
  {
   printf("continue\n");
   return ;
  }
 
  if(option[0] == '0')//輸入0,悔棋
  {
    
   if(goback())
   {
    printf("can't go back\n");
    continue;
   }
   else
   {
    blueMove();
   continue;
   }
  }
  
    if(option[0] =='s')//輸入s,存取資料
    {
        saveRecord();
        continue;
    }
        if(option[0] =='x')//輸入x,重播
    {
        replay();
        return;
    }
  }
  
}


//存取目前資料
void saveRecord(){
     fclose(record);
     printf("Save the record successfully\n");
    return;
}
//刪除資料
void deleteRecord(FILE *fptr){
    char null[]="";
    fseek(fptr,-55, SEEK_CUR );
    fwrite(null ,-55,SEEK_CUR,fptr);
    
}
//生成棋盤
void chessBoardBuilding()
{
    createRecord();
    for (int i = 0; i < ROW; i ++) {
        for (int j = 0; j < COLUMN; j ++) {
            chessPosition[i][j] =GAP;
        }
        printf("\n");
    }
   
    //布置紅棋
    chessPosition[1][0] = chessPosition[1][8] = Red(香);
    chessPosition[1][1] = chessPosition[1][7] = Red(桂);
    chessPosition[1][2] = chessPosition[1][6] = Red(銀);
    chessPosition[1][3] = chessPosition[1][5] = Red(金);
    chessPosition[1][4] = Red(王);
    chessPosition[2][1] = Red(飛);
    chessPosition[2][7] = Red(角);
    for(int k=0;k<9;k++){
        chessPosition[3][k]=Red(步);
    }
    
    //布置藍棋
    chessPosition[9][0] = chessPosition[9][8] = Blue(香);
    chessPosition[9][1] = chessPosition[9][7] = Blue(桂);
    chessPosition[9][2] = chessPosition[9][6] = Blue(銀);
    chessPosition[9][3] = chessPosition[9][5] = Blue(金);
    chessPosition[9][4] = Blue(王);
    chessPosition[8][1] = Blue(角);
    chessPosition[8][7] = Blue(飛);
     for(int k=0;k<9;k++){
        chessPosition[7][k]=Blue(步);
    }
    chessPosition[0][0]=White(9|);chessPosition[0][1]=White(8|);chessPosition[0][2]=White(7|);
    chessPosition[0][3]=White(6|);chessPosition[0][4]=White(5|);chessPosition[0][5]=White(4|);
    chessPosition[0][6]=White(3|);chessPosition[0][7]=White(2|);chessPosition[0][8]=White(1|);
    chessPosition[0][9]=White();
    chessPosition[1][9]=White(一);chessPosition[2][9]=White(二);chessPosition[3][9]=White(三);
    chessPosition[4][9]=White(四);chessPosition[5][9]=White(五);chessPosition[6][9]=White(六);
    chessPosition[7][9]=White(七);chessPosition[8][9]=White(八);chessPosition[9][9]=White(九);
}

//打印棋盤
void printChessboard()
{
	system("clear");//清除控制台屏幕
    
    //顯示
    printf("\n\n     \033[37;30m日本將棋\033[0m\n");

    for (int i = 0; i < ROW;  i ++) {
        for (int j = 0; j < COLUMN; j ++) {
            printf("%s",chessPosition[i][j]);
        }
        printf("\n");
    }

}
//印出過去棋盤
void printPastChessboard()
{
	//system("clear");//清除控制台屏幕
    
    //顯示
    printf("     \033[37;30m日本將棋\033[0m\n\n");

    for (int i = 0; i < ROW;  i ++) {
        for (int j = 0; j < COLUMN; j ++) {
            printf("%s",chessPosition[i][j]);
        }
        printf("\n");
    }

}


//判斷是紅棋還是藍棋,紅棋返回 -1,藍棋返回 1,否則返回0
int redOrBlue(int x,int y)
{
    if (chessPosition[x][y] == Red(飛) || chessPosition[x][y] == Red(桂) || chessPosition[x][y] == Red(銀) || chessPosition[x][y] == Red(金) || chessPosition[x][y] == Red(王) || chessPosition[x][y] == Red(角) || chessPosition[x][y] == Red(步)|| chessPosition[x][y] == Red(香))
    {
        return -1;
    }
    else if (chessPosition[x][y] == Blue(飛) || chessPosition[x][y] == Blue(桂) || chessPosition[x][y] == Blue(銀) || chessPosition[x][y] == Blue(金) || chessPosition[x][y] == Blue(王) || chessPosition[x][y] == Blue(角) || chessPosition[x][y] == Blue(步)|| chessPosition[x][y] == Blue(香))
    {
        return 1;
    }
    else
        return 0;
}
//藍棋移動
void blueMove()
{
    if (restart) {   
        printf("違反遊戲規則，請重新輸入\n");
        restart = 0;
    }
    //printf("玩家X\n");
    choose_Option1() ;
    printf("玩家X[藍棋]請輸入你要移動的棋子:\n");
    scanf("%d %d",&xi,&b1);
     yi=9-b1;

    if(redOrBlue(xi, yi) != 1){isStandard = 0;}
    
    printf("玩家X[藍棋]請輸入你要放置的位置:\n");
    scanf("%d %d",&xj,&b2);
    yj=9-b2;
    rulesOfAllKindsOfChessPieces();
    
    printChessboard(); 
}
//紅棋移動
void redMove()
{
    if (restart) {
        printf("違反遊戲規則，請重新輸入\n");
        restart = 0;
    }
    choose_Option2();
    printf("玩家Y[紅棋]請輸入你要移動的棋子:\n");
    scanf("%d %d",&xi,&b1);
    yi=9-b1;

    if(redOrBlue(xi, yi) != -1) {isStandard = 0;}
    printf("玩家Y[紅棋]請輸入你要放置的位置:\n");
    scanf("%d %d",&xj,&b2);
    yj=9-b2;
    rulesOfAllKindsOfChessPieces();
    printChessboard();
    
}

//判斷遊戲結束
void isGameOver()
{
    bool sign_r = 0;
    bool sign_b = 0;
    for (int i = 0; i < ROW; i ++) {
        for (int j = 0; j < COLUMN; j ++) {
            if (chessPosition[i][j] == Red(王)) {
                sign_r = 1;
            }
            else if (chessPosition[i][j] == Blue(王))
            {
                sign_b = 1;
            }
        }
    }
    if ((sign_r == 0)||(sign_b == 0)) {
        gameOverSign = 0;
    }
    
   
}
//建立檔案
void createRecord(){
    if ( ( record= fopen ("record.txt", "w+")) == NULL) 
    {
        printf("Fail to open the file\n");
    }
    else
    {
        printf("File open successfully\n");
        fseek(record, 0, SEEK_SET) ;
    }
}

//查詢舊檔
void readOldGame(){
    char step;
    char str1[55];
    int initRow=0,initCol=0,goalRow=0,goalCol=0,chess_index=0;
    fscanf(record," %d %d %d %d %s",&xi,&yi,&xj,&yj,chessPosition[xj][yj]);
    fclose(record);
    //reviesOldGame();
}

//每種棋子的規則
void rulesOfAllKindsOfChessPieces()
{
//R（飛）----------------------------------------
    if (chessPosition[xi][yi] == Red(飛))
    {
       
        if (yi == yj)
        {
            for (int i = xi+1; i < xj; i ++)
            {
                if (chessPosition[i][yi] != GAP)
                    isStandard = 0;//如果初始位置和目標位置之間有棋子，則不符合規則
            }
            for (int i = xi-1; i > xj; i --)
            {
                if (chessPosition[i][yi] != GAP)
                    isStandard = 0;
            }
        }
        else if (xi == xj)
        {
            for (int i = yi+1; i < yj; i ++)
                if (chessPosition[xi][i] != GAP)
                    isStandard = 0;
            for (int i = yi-1; i > yj; i --)
                if (chessPosition[xi][i] != GAP)
                    isStandard = 0;
        }
        
        if ((xi == xj || yi == yj)&& isStandard && (redOrBlue(xj, yj) != -1))//如果棋子直行、沒有犯規且落點不是紅棋，可以移動
        {
            insert(xi,yi,xj,yj,chessPosition[xj][yj]);
            chessPosition[xi][yi] = GAP;
            chessPosition[xj][yj] = Red(飛);
            fprintf(record,"player Y -> xi:%d,yi:%d,xj:%d,yj:%d,goalplace:%s\n",xi,yi,xj,yj,chessPosition[xj][yj]);
            return;
        }
        else
        {
            restart = 1;
        }
    }


//B（飛）----------------------------------------
    else if (chessPosition[xi][yi] == Blue(飛))
    {
        
        if (yi == yj)
        {
            for (int i = xi+1; i < xj; i ++)
            {
                
                if (chessPosition[i][yi] != GAP)
                    isStandard = 0;//如果初始位置和目標位置之間有棋子，則不符合規則
            }
            for (int i = xi-1; i > xj; i --)
            {
                
                if (chessPosition[i][yi] != GAP)
                    isStandard = 0;
            }
        }
        else if (xi == xj)
        {
            for (int i = yi+1; i < yj; i ++)
                if (chessPosition[xi][i] != GAP)
                    isStandard = 0;
            for (int i = yi-1; i > yj; i --)
                if (chessPosition[xi][i] != GAP)
                    isStandard = 0;
        }
        if ((xi == xj || yi == yj)&& isStandard && redOrBlue(xj, yj) != 1)//如果棋子直行、沒有犯規且落點不是藍棋，可以移動
        {
            insert(xi,yi,xj,yj,chessPosition[xj][yj]);
            chessPosition[xi][yi] = GAP;
            chessPosition[xj][yj] = Blue(飛);
            fprintf(record,"player X -> xi:%d,yi:%d,xj:%d,yj:%d,goalplace:%s\n",xi,yi,xj,yj,chessPosition[xj][yj]);
            return;
        }
        else
        {
            restart = 1;
        }
    }

//R（桂）----------------------------------------
    else if (chessPosition[xi][yi] == Red(桂))
    {
        
        if ((redOrBlue(xj, yj) != -1) && isStandard && ((xj == xi+2&& yj == yi-1) || (xj == xi+2 && yj == yi+1 )))
        {
            insert(xi,yi,xj,yj,chessPosition[xj][yj]);
            chessPosition[xi][yi] = GAP;
            chessPosition[xj][yj] = Red(桂);
            fprintf(record,"player Y -> xi:%d,yi:%d,xj:%d,yj:%d,goalplace:%s\n",xi,yi,xj,yj,chessPosition[xj][yj]);
            return;
        }
        else
        {
            restart = 1;
        }
    }
    
//B（桂）----------------------------------------
    else if (chessPosition[xi][yi] == Blue(桂))
    {
        
        if ((redOrBlue(xj, yj) != 1) && isStandard && ((xj == xi-2 && yj == yi-1)||(xj == xi-2 && yj == yi+1)))
            {
                insert(xi,yi,xj,yj,chessPosition[xj][yj]);
                chessPosition[xi][yi] = GAP;
                chessPosition[xj][yj] = Blue(桂);
                fprintf(record,"player X -> xi:%d,yi:%d,xj:%d,yj:%d,goalplace:%s\n",xi,yi,xj,yj,chessPosition[xj][yj]);
                return;
        }
        else
        {
            restart = 1;
        }
    }
    
    
//R（角）----------------------------------------
    else if (chessPosition[xi][yi] == Red(角))
    {
       
        int diff=0;
        diff=abs(xi-xj);
        if ((yi<yj)&& (xi<xj))
        {
            int j=yi+1;int i = xi+1;
            for (i,j; i < xj,j<yj; i ++,j++)
            {
                if (chessPosition[i][j] != GAP)
                    isStandard = 0;//如果初始位置和目標位置之間有棋子，則不符合規則
            }
            if(xj!=xi+diff||yj!=yi+diff){isStandard=0;}
        
        }
        if ((yi<yj)&& (xi>xj))
        {
            int j=yi+1;int i = xi-1;
            for (i,j; i > xj,j<yj; i--,j++)
            {
                if (chessPosition[i][j] != GAP)
                    isStandard = 0;//如果初始位置和目標位置之間有棋子，則不符合規則
            }
            if(xj!=xi-diff||yj!=yi+diff){isStandard=0;}
           
        }
        if ((yi>yj)&& (xi<xj))
        {
            int j=yi-1;int i = xi+1;
            for (i, j; i<xj, j>yj; i ++,j--)
            {
                if (chessPosition[i][j] != GAP)
                    isStandard = 0;//如果初始位置和目標位置之間有棋子，則不符合規則
            }
            if(xj!=xi+diff||yj!=yi-diff){isStandard=0;}
        
        }
        if ((yi>yj)&& (xi>xj))
        {
            int j=yi-1;int i = xi-1;
            for (i, j; i >xj, j>yj; i--,j--)
            {
                if (chessPosition[i][j] != GAP)
                    isStandard = 0;//如果初始位置和目標位置之間有棋子，則不符合規則
            }
            if(xj!=xi-diff||yj!=yi-diff){isStandard=0;}
        }
        if ((xi != xj && yi != yj)&& isStandard && (redOrBlue(xj, yj) != -1))//如果棋子直行、沒有犯規且落點不是紅棋，可以移動
        {
            insert(xi,yi,xj,yj,chessPosition[xj][yj]);
            chessPosition[xi][yi] = GAP;
            chessPosition[xj][yj] = Red(角);
            fprintf(record,"player Y -> xi:%d,yi:%d,xj:%d,yj:%d,goalplace:%s\n",xi,yi,xj,yj,chessPosition[xj][yj]);
            return;
        }
        else
        {
            restart = 1;
        }
    }
    
//B（角）----------------------------------------
if (chessPosition[xi][yi] == Blue(角))
    {
       int diff=0;
        diff=abs(xi-xj);
        if ((yi<yj)&& (xi<xj))
        {
            int j=yi+1;int i = xi+1;
            for (i,j; i < xj,j<yj; i ++,j++)
            {
                if (chessPosition[i][j] != GAP)
                    isStandard = 0;//如果初始位置和目標位置之間有棋子，則不符合規則
            }
            if(xj!=xi+diff||yj!=yi+diff){isStandard=0;}
        
        }
        if ((yi<yj)&& (xi>xj))
        {
            int j=yi+1;int i = xi-1;
            for (i,j; i > xj,j<yj; i--,j++)
            {
                if (chessPosition[i][j] != GAP)
                    isStandard = 0;//如果初始位置和目標位置之間有棋子，則不符合規則
            }
            if(xj!=xi-diff||yj!=yi+diff){isStandard=0;}
           
        }
        if ((yi>yj)&& (xi<xj))
        {
            int j=yi-1;int i = xi+1;
            for (i, j; i<xj, j>yj; i ++,j--)
            {
                if (chessPosition[i][j] != GAP)
                    isStandard = 0;//如果初始位置和目標位置之間有棋子，則不符合規則
            }
            if(xj!=xi+diff||yj!=yi-diff){isStandard=0;}
        
        }
        if ((yi>yj)&& (xi>xj))
        {
            int j=yi-1;int i = xi-1;
            for (i, j; i >xj, j>yj; i--,j--)
            {
                if (chessPosition[i][j] != GAP)
                    isStandard = 0;//如果初始位置和目標位置之間有棋子，則不符合規則
            }
            if(xj!=xi-diff||yj!=yi-diff){isStandard=0;}
        }
        if ((xi != xj && yi != yj)&& isStandard && (redOrBlue(xj, yj) != 1))//如果棋子直行、沒有犯規且落點不是藍棋，可以移動
        {
            insert(xi,yi,xj,yj,chessPosition[xj][yj]);
            chessPosition[xi][yi] = GAP;
            chessPosition[xj][yj] = Blue(角);
            fprintf(record,"player X -> xi:%d,yi:%d,xj:%d,yj:%d,goalplace:%s\n",xi,yi,xj,yj,chessPosition[xj][yj]);
        }
        else
        {
            restart = 1;
        }
    }
    
//R（步）----------------------------------------
    else if (chessPosition[xi][yi] == Red(步))
    {
        
        if (xi > xj)
            isStandard = 0;//如果倒退，則不符合規則
        if ( isStandard && redOrBlue(xj, yj) != -1&&(xj == xi+1&& yj == yi))//
        {
            insert(xi,yi,xj,yj,chessPosition[xj][yj]);
            chessPosition[xi][yi] = GAP;
            chessPosition[xj][yj] = Red (步);
            fprintf(record,"player Y -> xi:%d,yi:%d,xj:%d,yj:%d,goalplace:%s\n",xi,yi,xj,yj,chessPosition[xj][yj]);
        }
        else
        {
            restart = 1;
        }
    }
    
//B（步）----------------------------------------
    else if (chessPosition[xi][yi] == Blue(步))
    {
        
        if (xi < xj)
            isStandard = 0;//如果倒退，則不符合規則
        if (isStandard && redOrBlue(xj, yj) != 1&&(xj == xi-1&& yj == yi))//
        {
            insert(xi,yi,xj,yj,chessPosition[xj][yj]);
            chessPosition[xi][yi] = GAP;
            chessPosition[xj][yj] = Blue(步);
             fprintf(record,"player X -> xi:%d,yi:%d,xj:%d,yj:%d,goalplace:%s\n",xi,yi,xj,yj,chessPosition[xj][yj]);
        }
        else
        {
            restart = 1;
        }
    }

// //R（銀）----------------------------------------
     else if (chessPosition[xi][yi] == Red(銀))
    {
        if ((redOrBlue(xj, yj) != -1) && ((xj == xi+1 && yj == yi-1 ) || (xj == xi+1 && yj == yi+1) || (xj == xi+1 && yj == yi ) || (xj == xi-1 && yj == yi-1) ||(xj== xi-1&&yj==yi+1)))
        {
        {
            insert(xi,yi,xj,yj,chessPosition[xj][yj]);
            chessPosition[xi][yi] = GAP;
            chessPosition[xj][yj] = Red(銀);
            fprintf(record,"player Y -> xi:%d,yi:%d,xj:%d,yj:%d,goalplace:%s\n",xi,yi,xj,yj,chessPosition[xj][yj]);
        }
        }
        else
        {
            restart = 1;
        }
    }
    
//B（銀）----------------------------------------
    else if (chessPosition[xi][yi] == Blue(銀))
    {
        if ((redOrBlue(xj, yj) != 1) && ((xj == xi-1 && yj == yi-1 ) || (xj == xi-1 && yj == yi+1) || (xj == xi-1 && yj == yi ) || (xj == xi+1 && yj == yi-1) ||(xj== xi+1&&yj==yi+1)))
        {
            insert(xi,yi,xj,yj,chessPosition[xj][yj]);
            chessPosition[xi][yi] = GAP;
            chessPosition[xj][yj] = Blue(銀);
             fprintf(record,"player X -> xi:%d,yi:%d,xj:%d,yj:%d,goalplace:%s\n",xi,yi,xj,yj,chessPosition[xj][yj]);
        }
        else
        {
            restart = 1;
        }
    }

//R（金）----------------------------------------
    else if (chessPosition[xi][yi] == Red(金))
    {
        if ((redOrBlue(xj, yj) != -1) && ((xj == xi+1 && yj == yi-1 ) || (xj == xi+1 && yj == yi+1) || (xj == xi+1 && yj == yi ) 
        || (xj == xi-1 && yj == yi) ||(xj== xi&&yj==yi-1)||(xj== xi&&yj==yi+1)))
        {
            insert(xi,yi,xj,yj,chessPosition[xj][yj]);
            chessPosition[xi][yi] = GAP;
            chessPosition[xj][yj] = Red(金);
            fprintf(record,"player Y -> xi:%d,yi:%d,xj:%d,yj:%d,goalplace:%s\n",xi,yi,xj,yj,chessPosition[xj][yj]);
        }
        else
        {
            restart = 1;
        }
    }

//B（金）----------------------------------------
    else if (chessPosition[xi][yi] == Blue(金))
    {
        if ((redOrBlue(xj, yj) != 1) && ((xj == xi-1 && yj == yi-1 ) || (xj == xi-1 && yj == yi+1) || (xj == xi-1 && yj == yi ) 
        || (xj == xi+1 && yj == yi) ||(xj== xi&&yj==yi-1)||(xj== xi&&yj==yi+1)))
        {
            insert(xi,yi,xj,yj,chessPosition[xj][yj]);
            chessPosition[xi][yi] = GAP;
            chessPosition[xj][yj] = Blue(金);
            fprintf(record,"player X -> xi:%d,yi:%d,xj:%d,yj:%d,goalplace:%s\n",xi,yi,xj,yj,chessPosition[xj][yj]);
        }
        else
        {
            restart = 1;
        }
    }
//R（香）----------------------------------------
    else if (chessPosition[xi][yi] == Red(香))
    {
        
        if (xi > xj)
            isStandard = 0;//如果倒退，則不符合規則
        if (yi == yj)
        {

            for (int i = xi+1; i < xj; i ++)
            {            
                if (chessPosition[i][yj] != GAP) isStandard = 0;//如果初始位置和目標位置之間有棋子，則不符合規則
            }
        }
        if ((yi == yj)&& isStandard && (redOrBlue(xj, yj)!= -1))
        {
            insert(xi,yi,xj,yj,chessPosition[xj][yj]);
            chessPosition[xi][yi] = GAP;
            chessPosition[xj][yj] = Red (香);
            fprintf(record,"player Y -> xi:%d,yi:%d,xj:%d,yj:%d,goalplace:%s\n",xi,yi,xj,yj,chessPosition[xj][yj]);
        }
        else
        {
            restart = 1;
        }
    }
    
//B（香）----------------------------------------
    else if (chessPosition[xi][yi] == Blue(香))
    {

        if (xi < xj)
            isStandard = 0;//如果倒退，則不符合規則
        if (yi == yj)//列坐標不變，同列移動
        {
            
            for (int i = xi-1; i > xj; i --)
            {      
                if (chessPosition[i][yi] != GAP) isStandard = 0;
            }
        }

        if ((yi == yj)&& isStandard&& (redOrBlue(xj, yj)!= 1))//
        {      
            insert(xi,yi,xj,yj,chessPosition[xj][yj]);   
            chessPosition[xi][yi] = GAP;
            chessPosition[xj][yj] = Blue(香);
            fprintf(record,"player X -> xi:%d,yi:%d,xj:%d,yj:%d,goalplace:%s\n",xi,yi,xj,yj,chessPosition[xj][yj]);
        }
        else
        {
            restart = 1;
        }
    }
//R（王）----------------------------------------
    else if (chessPosition[xi][yi] == Red(王))
    {
        if ((redOrBlue(xj, yj) != -1) && ((xj == xi-1 && yj == yi-1 ) || (xj == xi-1 && yj == yi+1) || (xj == xi-1 && yj == yi ) 
        ||(xj == xi+1 && yj == yi) ||(xj== xi+1&&yj==yi+1)|| (xj == xi+1 && yj == yi-1) ||(xj== xi&&yj==yi+1)||(xj== xi&&yj==yi-1)))
        {
            insert(xi,yi,xj,yj,chessPosition[xj][yj]);
            chessPosition[xi][yi] = GAP;
            chessPosition[xj][yj] = Red(王);
            fprintf(record,"player Y -> xi:%d,yi:%d,xj:%d,yj:%d,goalplace:%s\n",xi,yi,xj,yj,chessPosition[xj][yj]);
        }
        else
        {
            restart = 1;
        }
    }
//B（王）----------------------------------------
    else if (chessPosition[xi][yi] == Blue(王))
    {
        if ((redOrBlue(xj, yj) != 1) && ((xj == xi-1 && yj == yi-1 ) || (xj == xi-1 && yj == yi+1) || (xj == xi-1 && yj == yi ) 
        ||(xj == xi+1 && yj == yi) ||(xj== xi+1&&yj==yi+1)|| (xj == xi+1 && yj == yi-1) ||(xj== xi&&yj==yi+1)||(xj== xi&&yj==yi-1)))
        {  
            insert(xi,yi,xj,yj,chessPosition[xj][yj]);
            chessPosition[xi][yi] = GAP;
            chessPosition[xj][yj] = Blue(王);
            fprintf(record,"player X -> xi:%d,yi:%d,xj:%d,yj:%d,goalplace:%s\n",xi,yi,xj,yj,chessPosition[xj][yj]);
        }
        else
        {
            restart = 1;
        }
    }

    else
        {
            restart = 1;
        }
}
