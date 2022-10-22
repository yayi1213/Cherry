#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "stack.h"
//#include <ev.h>

#define Blue(piece) "\033[34m"#piece"\033[0m"//藍色棋子
#define Red(piece) "\033[31m"#piece"\033[0m"//紅色棋子
#define GAP "\033[33m口\033[0m"//棋盤格子
#define White(piece) "\033[37m"#piece"\033[0m"//白色數字
#define ROW 10
#define COLUMN 10
FILE *record=NULL;
//定義外部變量,棋盤坐標
char* chessPosition[10][10];
int b1,xi,yi;//要移動的棋子位置
int b2,xj,yj;//移動的目標位置
bool isStandard = 1;//是否符合規則，初始值1，符合
bool gameOverSign = 1;//遊戲是否結束，0結束
bool restart = 0;

//生成棋盤
void chessBoardBuilding();
//打印棋盤
void printChessboard();
//判斷是紅棋還是藍棋,紅旗返回-1,x06藍棋返回1,否則返回0
int redOrBlue(int x,int y);
//紅棋移動
void redMove();
//藍棋移動
void blueMove();
//每種棋子的規則
void rulesOfAllKindsOfChessPieces();
//判斷遊戲結束
void isGameOver();
//悔棋
//void turnBack();
//建立檔案
void createRecord();
//查詢舊檔
//void review_old_game();
//刪除一行
//void deleteLine(char* FileName, int lineno);
//void pushFunction();
//**************************主函數******************************
int main()
{
    //生成棋盤
    chessBoardBuilding();
    //打印棋盤
    printChessboard();

     if ( ( record= fopen ("record.txt", "w+")) == NULL) 
    {
        printf("Fail to open the file\n");
    }
    else
    {
        printf("record File open successfully\n");
    }
    //開始下棋
    int turn = -1;
    //read_old_game();
    while (gameOverSign) {
        isStandard = 1;
        turn *= (-1);//雙方交替下棋
        
        switch (turn) {
            case 1:                
                blueMove();
                fprintf(record,"playerX -> xi:%d,yi:%d,xj:%d,yj:%d\n",xi,b1,xj,b2);
                //printf("success");
                turn = (restart) ? (turn*-1) : turn;
                break;
            case -1:
                redMove();
                fprintf(record,"playerY -> xi:%d,yi:%d,xj:%d,yj:%d\n",xi,b1,xj,b2);         
                turn = (restart) ? (turn*-1) : turn;
                break;
        }
        
        isGameOver();       
    }

    bool redWin = 0;
    bool blueWin = 0;
    for (int i = 0; i < ROW; i ++) {
        for (int j = 0; j < COLUMN; j ++) {
            if (chessPosition[i][j] == Red(王)) {
                redWin = 1;
            }
            else if (chessPosition[i][j] == Blue(王))
            {
                blueWin = 1;
            }
        }
    }
    if (redWin == 0){printf("VICTORY!玩家X獲勝\n\n");}
    if (blueWin == 0){printf("GAMEOVER!玩家X輸了\n\n");}
    
}//主函數結束


//*************************自定義函數*****************************

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
    printf("玩家X[藍棋]請輸入你要移動的棋子:\n");
    scanf("%d %d",&xi,&b1);
    yi=9-b1;
    //turnBack();
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
    printf("玩家Y[紅棋]請輸入你要移動的棋子:\n");
    scanf("%d %d",&xi,&b1);
    yi=9-b1;
    //turnBack();

    if(redOrBlue(xi, yi) != -1) {isStandard = 0;}
    printf("玩家Y[紅棋]請輸入你要放置的位置:\n");
    scanf("%d %d",&xj,&b2);
    yj=9-b2;
    //fprintf(boardRecord,"playerY|xi:%d,yi:%d,xj:%d,yj:%d",xi,yi,xj,yj);
    rulesOfAllKindsOfChessPieces();
    printChessboard();
    //fprintf(old_game,"playerX -> xi:%d,yi:%d,xj:%d,yj:%d\n",xi,b1,xj,b2);
    
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

void pushFunction()
    {
    if (top >= MAX-1)/* 當堆疊已滿•則顯示錯誤 */
        printf("\n Stack is full !\n");
    else {
        top++;
        //printf("\n\n Please enter item to insert: ");
        //fgets(old_game);
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
        //pushFunction(old_game);

    }
    if(gameOverSign = 1)
    fclose(record);
}

// //查詢舊檔
// void review_old_game(){
//     char step;
//     top=-1;
//     change=1;
//     if(review_old_game()){

//     }


// }

// //刪除一行
// void deleteLine(char* FileName, int lineno)  
// {  
//    int   Lid=0;   
//    FILE*   fp=NULL;  
//    char   Buf[256]="";  
//    char   tmp[20][256]={0};  
//    char   *p   =   Buf;    
   
//       if ((fp = fopen(*record, "r+")) == NULL)  
//    {  
//     printf("Can't   open   file!/n");  
//     return;  
//    } 

//    while ((p = fgets(Buf, 256, fp)) != NULL)  
//    {  
//      Lid++;  
//      if (Lid == lineno)  
//      {  
//       if ((p = fgets(Buf, 256, fp)) != NULL) 
//       {  
//       strcpy(tmp[Lid], Buf);  
//      }  
//       }  
//     else  
//     {
//       strcpy(tmp[Lid], Buf);
//      }
//  } 

// //悔棋
// void turnBack(){
//     if(xi=0){deleteLine(fp,index);}
// }

//每種棋子的規則
void rulesOfAllKindsOfChessPieces()
{
//R（飛）----------------------------------------
    if (chessPosition[xi][yi] == Red(飛))
    {
       
        if (yi == yj)//列坐標不變，同列移動
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
        else if (xi == xj)//行坐標不變，同行移動
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
            
            chessPosition[xi][yi] = GAP;
            chessPosition[xj][yj] = Red(飛);
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
        
        if (yi == yj)//列坐標不變，同行移動
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
        else if (xi == xj)//行坐標不變，同列移動
        {
            for (int i = yi+1; i < yj; i ++)
                if (chessPosition[xi][i] != GAP)
                    isStandard = 0;
            for (int i = yi-1; i > yj; i --)
                if (chessPosition[xi][i] != GAP)
                    isStandard = 0;
        }
        if ((xi == xj || yi == yj)&& isStandard && redOrBlue(xj, yj) != 1)//如果棋子直行、沒有犯規且落點不是紅棋，可以移動
        {
            chessPosition[xi][yi] = GAP;
            chessPosition[xj][yj] = Blue(飛);
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
            chessPosition[xi][yi] = GAP;
            chessPosition[xj][yj] = Red(桂);
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
            {chessPosition[xi][yi] = GAP;
            chessPosition[xj][yj] = Blue(桂);
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
        if ((yi<yj)&& (xi<xj))//列坐標不變，同列移動
        {
            int j=yi+1;int i = xi+1;
            for (i,j; i < xj,j<yj; i ++,j++)
            {
                if (chessPosition[i][j] != GAP)
                    isStandard = 0;//如果初始位置和目標位置之間有棋子，則不符合規則
            }
            if(xj!=xi+diff||yj!=yi+diff){isStandard=0;}
        
        }
        if ((yi<yj)&& (xi>xj))//列坐標不變，同列移動
        {
            int j=yi+1;int i = xi-1;
            for (i,j; i > xj,j<yj; i--,j++)
            {
                if (chessPosition[i][j] != GAP)
                    isStandard = 0;//如果初始位置和目標位置之間有棋子，則不符合規則
            }
            if(xj!=xi-diff||yj!=yi+diff){isStandard=0;}
           
        }
        if ((yi>yj)&& (xi<xj))//列坐標不變，同列移動
        {
            int j=yi-1;int i = xi+1;
            for (i, j; i<xj, j>yj; i ++,j--)
            {
                if (chessPosition[i][j] != GAP)
                    isStandard = 0;//如果初始位置和目標位置之間有棋子，則不符合規則
            }
            if(xj!=xi+diff||yj!=yi-diff){isStandard=0;}
        
        }
        if ((yi>yj)&& (xi>xj))//列坐標不變，同列移動
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
            
            chessPosition[xi][yi] = GAP;
            chessPosition[xj][yj] = Red(角);
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
        if ((yi<yj)&& (xi<xj))//列坐標不變，同列移動
        {
            int j=yi+1;int i = xi+1;
            for (i,j; i < xj,j<yj; i ++,j++)
            {
                if (chessPosition[i][j] != GAP)
                    isStandard = 0;//如果初始位置和目標位置之間有棋子，則不符合規則
            }
            if(xj!=xi+diff||yj!=yi+diff){isStandard=0;}
        
        }
        if ((yi<yj)&& (xi>xj))//列坐標不變，同列移動
        {
            int j=yi+1;int i = xi-1;
            for (i,j; i > xj,j<yj; i--,j++)
            {
                if (chessPosition[i][j] != GAP)
                    isStandard = 0;//如果初始位置和目標位置之間有棋子，則不符合規則
            }
            if(xj!=xi-diff||yj!=yi+diff){isStandard=0;}
           
        }
        if ((yi>yj)&& (xi<xj))//列坐標不變，同列移動
        {
            int j=yi-1;int i = xi+1;
            for (i, j; i<xj, j>yj; i ++,j--)
            {
                if (chessPosition[i][j] != GAP)
                    isStandard = 0;//如果初始位置和目標位置之間有棋子，則不符合規則
            }
            if(xj!=xi+diff||yj!=yi-diff){isStandard=0;}
        
        }
        if ((yi>yj)&& (xi>xj))//列坐標不變，同列移動
        {
            int j=yi-1;int i = xi-1;
            for (i, j; i >xj, j>yj; i--,j--)
            {
                if (chessPosition[i][j] != GAP)
                    isStandard = 0;//如果初始位置和目標位置之間有棋子，則不符合規則
            }
            if(xj!=xi-diff||yj!=yi-diff){isStandard=0;}
        }
        if ((xi != xj && yi != yj)&& isStandard && (redOrBlue(xj, yj) != 1))//如果棋子直行、沒有犯規且落點不是紅棋，可以移動
        {
            
            chessPosition[xi][yi] = GAP;
            chessPosition[xj][yj] = Blue(角);
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
            isStandard = 0;//如果倒退，則不符合規範
        if ( isStandard && redOrBlue(xj, yj) != -1&&(xj == xi+1&& yj == yi))//
        {
            chessPosition[xi][yi] = GAP;
            chessPosition[xj][yj] = Red (步);
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
            isStandard = 0;//如果倒退，則不符合規範
        if (isStandard && redOrBlue(xj, yj) != 1&&(xj == xi-1&& yj == yi))//
        {
            chessPosition[xi][yi] = GAP;
            chessPosition[xj][yj] = Blue(步);
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
            chessPosition[xi][yi] = GAP;
            chessPosition[xj][yj] = Red(銀);
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
            chessPosition[xi][yi] = GAP;
            chessPosition[xj][yj] = Blue(銀);
        }
        else
        {
            restart = 1;
        }
    }

//R（金）----------------------------------------
    else if (chessPosition[xi][yi] == Red(金))
    {
        if ((redOrBlue(xj, yj) != -1) && ((xj == xi+1 && yj == yi-1 ) || (xj == xi+1 && yj == yi+1) || (xj == xi+1 && yj == yi ) || (xj == xi-1 && yj == yi) ||(xj== xi&&yj==yi-1)||(xj== xi&&yj==yi+1)))
        {
            chessPosition[xi][yi] = GAP;
            chessPosition[xj][yj] = Red(金);
        }
        else
        {
            restart = 1;
        }
    }

//B（金）----------------------------------------
    else if (chessPosition[xi][yi] == Blue(金))
    {
        if ((redOrBlue(xj, yj) != 1) && ((xj == xi-1 && yj == yi-1 ) || (xj == xi-1 && yj == yi+1) || (xj == xi-1 && yj == yi ) || (xj == xi+1 && yj == yi) ||(xj== xi&&yj==yi-1)||(xj== xi&&yj==yi+1)))
        {
            chessPosition[xi][yi] = GAP;
            chessPosition[xj][yj] = Blue(金);
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
            isStandard = 0;//如果倒退，則不符合規範
        if (yi == yj)//列坐標不變，同行移動
        {

            for (int i = xi+1; i < xj; i ++)
            {            
                if (chessPosition[i][yj] != GAP) isStandard = 0;//如果初始位置和目標位置之間有棋子，則不符合規則
            }
        }
        if ((yi == yj)&& isStandard && (redOrBlue(xj, yj)!= -1))
        {
            chessPosition[xi][yi] = GAP;
            chessPosition[xj][yj] = Red (香);
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
            isStandard = 0;//如果倒退，則不符合規範
        if (yi == yj)//列坐標不變，同列移動
        {
            
            for (int i = xi-1; i > xj; i --)
            {      
                if (chessPosition[i][yi] != GAP) isStandard = 0;
            }
        }

        if ((yi == yj)&& isStandard&& (redOrBlue(xj, yj)!= 1))//
        {         
            chessPosition[xi][yi] = GAP;
            chessPosition[xj][yj] = Blue(香);
        }
        else
        {
            restart = 1;
        }
    }


//R（王）----------------------------------------
    else if (chessPosition[xi][yi] == Red(王))
    {
        if ((redOrBlue(xj, yj) != -1) && ((xj == xi-1 && yj == yi-1 ) || (xj == xi-1 && yj == yi+1) || (xj == xi-1 && yj == yi ) ||(xj == xi+1 && yj == yi) ||(xj== xi+1&&yj==yi+1)|| (xj == xi+1 && yj == yi-1) ||(xj== xi&&yj==yi+1)||(xj== xi&&yj==yi-1)))
        {
            chessPosition[xi][yi] = GAP;
            chessPosition[xj][yj] = Red(王);
        }
        else
        {
            restart = 1;
        }
    }

//B（王）----------------------------------------
    else if (chessPosition[xi][yi] == Blue(王))
    {
        if ((redOrBlue(xj, yj) != 1) && ((xj == xi-1 && yj == yi-1 ) || (xj == xi-1 && yj == yi+1) || (xj == xi-1 && yj == yi ) ||(xj == xi+1 && yj == yi) ||(xj== xi+1&&yj==yi+1)|| (xj == xi+1 && yj == yi-1) ||(xj== xi&&yj==yi+1)||(xj== xi&&yj==yi-1)))
        {  
            chessPosition[xi][yi] = GAP;
            chessPosition[xj][yj] = Blue(王);
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
