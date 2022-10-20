#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>

#define B(piece) "\033[34m"#piece"\033[0m"//藍色棋子
#define R(piece) "\033[31m"#piece"\033[0m"//紅色棋子
#define GAP "\033[33m口\033[0m"//棋盤格子
#define W(piece) "\033[37m"#piece"\033[0m"//白色數字
#define ROW 10
#define COL 10
//定義外部變量,棋盤坐標
char* array[10][10];
int b1,xi,yi;//要移動的棋子位置
int b2,xj,yj;//移動的目標位置
bool isStandard = 1;//是否符合規則，初始值1，符合
bool gameOverSign = 1;//遊戲是否結束，0結束
bool restart = 0;
//生成棋盤
void chessboardBuilding();
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

//**************************主函數******************************
int main()
{
    //生成棋盤 test
    chessboardBuilding();
    //打印棋盤
    printChessboard();
    //開始下棋
    int turn = -1;
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
        isGameOver();
        
    }
    printf("遊戲結束!\n");
    
  
    
}//主函數結束


//*************************自定義函數*****************************

//生成棋盤
void chessboardBuilding()
{
    for (int i = 0; i < ROW; i ++) {
        for (int j = 0; j < COL; j ++) {
            array[i][j] =GAP;
        }
        printf("\n");
    }
   
    //布置紅棋
    array[1][0] = array[1][8] = R(香);
    array[1][1] = array[1][7] = R(桂);
    array[1][2] = array[1][6] = R(銀);
    array[1][3] = array[1][5] = R(金);
    array[1][4] = R(王);
    array[2][1] = R(飛);
    array[2][7] = R(角);
    for(int k=0;k<9;k++){
        array[3][k]=R(步);
    }
    
    //布置藍棋
    array[9][0] = array[9][8] = B(香);
    array[9][1] = array[9][7] = B(桂);
    array[9][2] = array[9][6] = B(銀);
    array[9][3] = array[9][5] = B(金);
    array[9][4] = B(王);
    array[8][1] = B(角);
    array[8][7] = B(飛);
     for(int k=0;k<9;k++){
        array[7][k]=B(步);
    }
    array[0][0]=W(9|);array[0][1]=W(8|);array[0][2]=W(7|);
    array[0][3]=W(6|);array[0][4]=W(5|);array[0][5]=W(4|);
    array[0][6]=W(3|);array[0][7]=W(2|);array[0][8]=W(1|);
    array[0][9]=W();
    array[1][9]=W(一);array[2][9]=W(二);array[3][9]=W(三);
    array[4][9]=W(四);array[5][9]=W(五);array[6][9]=W(六);
    array[7][9]=W(七);array[8][9]=W(八);array[9][9]=W(九);
}

//打印棋盤
void printChessboard()
{
	//system("clear");//清除控制台屏幕
    
    //顯示
    printf("     \033[37;30m日本將棋\033[0m\n\n");

    for (int i = 0; i < ROW;  i ++) {
        for (int j = 0; j < COL; j ++) {
            printf("%s",array[i][j]);
        }
        printf("\n");
    }

}

//判斷是紅棋還是藍棋,紅棋返回 -1,藍棋返回 1,否則返回0
int redOrBlue(int x,int y)
{
    if (array[x][y] == R(飛) || array[x][y] == R(桂) || array[x][y] == R(銀) || array[x][y] == R(金) || array[x][y] == R(王) || array[x][y] == R(角) || array[x][y] == R(步)|| array[x][y] == R(香))
    {
        return -1;
    }
    else if (array[x][y] == B(飛) || array[x][y] == B(桂) || array[x][y] == B(銀) || array[x][y] == B(金) || array[x][y] == B(王) || array[x][y] == B(角) || array[x][y] == B(步)|| array[x][y] == B(香))
    {
        return 1;
    }
    else
        return 0;
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
    if(redOrBlue(xi, yi) != -1)isStandard = 0;
    printf("玩家Y[紅棋]請輸入你要放置的位置:\n");
    scanf("%d %d",&xj,&b2);
    yj=9-b2;
    rulesOfAllKindsOfChessPieces();
    printChessboard();
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
    if(redOrBlue(xi, yi) != 1)isStandard = 0;

    printf("玩家X[藍棋]請輸入你要放置的位置:\n");
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
        for (int j = 0; j < COL; j ++) {
            if (array[i][j] == R(王)) {
                sign_r = 1;
            }
            else if (array[i][j] == B(王))
            {
                sign_b = 1;
            }
        }
    }
    if ((sign_r == 0)||(sign_b == 0)) {
        gameOverSign = 0;
    }
}

//每種棋子的規則
void rulesOfAllKindsOfChessPieces()
{
//R（飛）----------------------------------------
    if (array[xi][yi] == R(飛))
    {
       
        if (yi == yj)//列坐標不變，同列移動
        {
            for (int i = xi+1; i < xj; i ++)
            {
                if (array[xi][yi] != GAP)
                    isStandard = 0;//如果初始位置和目標位置之間有棋子，則不符合規則
            }
            for (int i = xi-1; i > xj; i --)
            {
                if (array[xi][yi] != GAP)
                    isStandard = 0;
            }
        }
        else if (xi == xj)//行坐標不變，同行移動
        {
            for (int i = yi+1; i < yj; i ++)
                if (redOrBlue(xj, yj) != 1)
                    isStandard = 0;
            for (int i = yi-1; i > yj; i --)
                if (array[xi][i] != GAP)
                    isStandard = 0;
        }
        
        if ((xi == xj || yi == yj)&& isStandard && (redOrBlue(xj, yj) != -1))//如果棋子直行、沒有犯規且落點不是紅棋，可以移動
        {
            
            array[xi][yi] = GAP;
            array[xj][yj] = R(飛);
        }
        else
        {
            restart = 1;
        }
    }

//B（飛）----------------------------------------
    else if (array[xi][yi] == B(飛))
    {
        
        if (yi == yj)//列坐標不變，同行移動
        {
            for (int i = xi+1; i < xj; i ++)
            {
                
                if (array[i][yi] != GAP)
                    isStandard = 0;//如果初始位置和目標位置之間有棋子，則不符合規則
            }
            for (int i = xi-1; i > xj; i --)
            {
                
                if (array[i][yi] != GAP)
                    isStandard = 0;
            }
        }
        else if (xi == xj)//行坐標不變，同列移動
        {
            for (int i = yi+1; i < yj; i ++)
                if (array[xi][i] != GAP)
                    isStandard = 0;
            for (int i = yi-1; i > yj; i --)
                if (array[xi][i] != GAP)
                    isStandard = 0;
        }
        if ((xi == xj || yi == yj)&& isStandard && redOrBlue(xj, yj) != 1)//如果棋子直行、沒有犯規且落點不是紅棋，可以移動
        {
            array[xi][yi] = GAP;
            array[xj][yj] = B(飛);
        }
        else
        {
            restart = 1;
        }
    }

//R（桂）----------------------------------------
    else if (array[xi][yi] == R(桂))
    {
        
        if ((redOrBlue(xj, yj) != -1) && ((xj == xi+2&& yj == yi-1) || (xj == xi+2 && yj == yi+1 )))
        {
            array[xi][yi] = GAP;
            array[xj][yj] = R(桂);
        }
        else
        {
            restart = 1;
        }
    }
    
//B（桂）----------------------------------------
    else if (array[xi][yi] == B(桂))
    {
        
        if ((redOrBlue(xj, yj) != 1) && ((xj == xi-2 && yj == yi-1)||(xj == xi-2 && yj == yi+1)))
            {array[xi][yi] = GAP;
            array[xj][yj] = B(桂);
        }
        else
        {
            restart = 1;
        }
    }
    
    
//R（角）----------------------------------------
    // else if (array[xi][yi] == R(角))
    // {
    //     if ((xi == xj || yi == yj) && redOrBlue(xj, yj) != 1)//如果棋子直行、沒有犯規且落點不是紅棋，可以移動
    //     {
    //         array[xi][yi] = GAP;
    //         array[xj][yj] = R(角);
    //     }
    //     else
    //     {
    //         restart = 1;
    //     }
    // }
    
//B（角）----------------------------------------
    // else if (array[xi][yi] == B(角))
    // {
    //     int count = 0;//起始位置間棋子的個數
    //     if (yi == yj)//列坐標不變，同行移動
    //     {
    //         for (int i = xi+1; i < xj; i ++)
    //         {
    //             if (i == 5)
    //                 continue;//如果行等於5，跳過
    //             if (redOrBlue(i, yi) != 0)
    //                 count++;
                
    //         }
    //         for (int i = xi-1; i > xj; i --)
    //         {
    //             if (i == 5)
    //                 continue;//如果行等於5，跳過
    //             if (redOrBlue(i, yi) != 0)
    //                 count++;
    //         }
    //     }
    //     else if (xi == xj)//行坐標不變，同行移動
    //     {
    //         for (int i = yi+1; i < yj; i ++)
    //             if (redOrBlue(xi, i) != 0)
    //                 count++;
    //         for (int i = yi-1; i > yj; i --)
    //             if (redOrBlue(xi, i) != 0)
    //                 count++;
    //     }
        
    //     if ((xi == xj || yi == yj)&& (count <= 1) && redOrBlue(xj, yj) != -1)//如果棋子直行、沒有犯規且落點不是紅棋，可以移動
    //     {
    //         array[xi][yi] = GAP;
    //         array[xj][yj] = B(角);
    //     }
    //     else
    //     {
    //         restart = 1;
    //     }
    // }
    
//R（步）----------------------------------------
    else if (array[xi][yi] == R(步))
    {
        
        if (xi > xj)
            isStandard = 0;//如果倒退，則不符合規範

        // if (xi >= 7) {
        //     if ((xj == xi+1 && yi ==yj)|| (xj == xi && yi ==yj+1)||(xj == xi && yi ==yj-1))
        //     {
                
        //     }
        //     else
        //         isStandard = 0;
        // }
        if ((xi == xj || yi == yj)&& isStandard && redOrBlue(xj, yj) != -1&&(xj == xi+1&& yj == yi))//
        {
            array[xi][yi] = GAP;
            array[xj][yj] = R (步);
        }
        else
        {
            restart = 1;
        }
    }
    
//B（步）----------------------------------------
    else if (array[xi][yi] == B(步))
    {
        
        if (xi < xj)
            isStandard = 0;//如果倒退，則不符合規範
        // if (xi < 4) {
        //     if ((xj == xi-1 && yi ==yj)|| (xj == xi && yi ==yj+1)||(xj == xi && yi ==yj-1))
        //     {
                
        //     }
        //     else
        //         isStandard = 0;
        // }
        if (isStandard && redOrBlue(xj, yj) != 1&&(xj == xi-1&& yj == yi))//
        {
            array[xi][yi] = GAP;
            array[xj][yj] = B(步);
        }
        else
        {
            restart = 1;
        }
    }

// //R（銀）----------------------------------------
     else if (array[xi][yi] == R(銀))
    {
        if ((redOrBlue(xj, yj) != -1) && ((xj == xi+2&& yj == yi-1) || (xj == xi+2 && yj == yi+1 )))
            {array[xi][yi] = GAP;
            array[xj][yj] = B(銀);
        }
        else
        {
            restart = 1;
        }
    }
    
//B（銀）----------------------------------------
    else if (array[xi][yi] == B(銀))
    {
        if ((redOrBlue(xj, yj) != 1) && ((xj == xi-1 && yj == yi-1 &&redOrBlue(xi-1, yi-1) == 0) || (xj == xi-2 && yj == yi+2 &&redOrBlue(xi-1, yi+1) == 0) || (xj == xi+2 && yj == yi-2 &&redOrBlue(xi+1, yi-1) == 0) || (xj == xi+2 && yj == yi+2 &&redOrBlue(xi+1, yi+1) == 0)))
        {
            array[xi][yi] = GAP;
            array[xj][yj] = B(銀);
        }
        else
        {
            restart = 1;
        }
    }

// //R（金）----------------------------------------
//     else if (array[xi][yi] == R(金))
//     {
//         if ((xj <= 2)&&(redOrBlue(xj, yj) != 1) && ((xj == xi-1 && yj == yi-1 ) || (xj == xi-1 && yj == yi+1 ) || (xj == xi+1 && yj == yi-1 ) || (xj == xi+1 && yj == yi+1 )))
//         {
//             array[xi][yi] = GAP;
//             array[xj][yj] = R(金);
//         }
//         else
//         {
//             restart = 1;
//         }
//     }

// //B（金）----------------------------------------
//     else if (array[xi][yi] == B(金))
//     {
//         if ((xj >= 8)&&(redOrBlue(xj, yj) != 1) && ((xj == xi-1 && yj == yi-1 ) || (xj == xi-1 && yj == yi+1 ) || (xj == xi+1 && yj == yi-1 ) || (xj == xi+1 && yj == yi+1 )))
//         {
//             array[xi][yi] = GAP;
//             array[xj][yj] = B(金);
//         }
//         else
//         {
//             restart = 1;
//         }
//     }
//R（香）----------------------------------------
    else if (array[xi][yi] == R(香))
    {
        
        if (xi > xj)
            isStandard = 0;//如果倒退，則不符合規範

        // if (xi >= 7) {
        //     if ((xj == xi+1 && yi ==yj)|| (xj == xi && yi ==yj+1)||(xj == xi && yi ==yj-1))
        //     {
                
        //     }
        //     else
        //         isStandard = 0;
        // }
          if (yi == yj)//列坐標不變，同行移動
        {

            for (int i = xi+1; i < xj; i ++)
            {
                
                if (array[i][yj] != GAP) isStandard = 0;//如果初始位置和目標位置之間有棋子，則不符合規則
            }
            for (int i = xi-1; i > xj; i --)
            {
                if (array[i][yj] != GAP)
                    isStandard = 0;
            }
        }
        if ((yi == yj)&& isStandard && (redOrBlue(xj, yj)!= -1))
        {
            array[xi][yi] = GAP;
            array[xj][yj] = R (香);
        }
        else
        {
            restart = 1;
        }
    }
    
//B（香）----------------------------------------
    else if (array[xi][yi] == B(香))
    {

      if (xi < xj)
            isStandard = 0;//如果倒退，則不符合規範

        // if (xi < 4) {
        //     if ((xj == xi-1 && yi ==yj)|| (xj == xi && yi ==yj+1)||(xj == xi && yi ==yj-1))
        //     {
                
        //     }
        //     else
        //         isStandard = 0;
        // }
        if (yi == yj)//列坐標不變，同列移動
        {
            
            for (int i = xi-1; i > xj; i --)
            {
                
                if (array[i][yi] != GAP) isStandard = 0;
            }
        }
        printf("%d\n", isStandard);
        if ((yi == yj)&& isStandard&& (redOrBlue(xj, yj)!= 1))//
        {
            
            array[xi][yi] = GAP;
            array[xj][yj] = B(香);
        }
        else
        {
            restart = 1;
        }
    }


//R（王）----------------------------------------
    // else if (array[xi][yi] == R(王))
    // {
    //     if ((xj <= 2 && yj <= 5 && yj >=3)&&(redOrBlue(xj, yj) != 1) && (((xj == xi)&&(yj == yi + 1 || yj == yi - 1))||((yj == yi)&&(xj == xi + 1 || xj == xi - 1))))
    //     {
    //         array[xi][yi] = GAP;
    //         array[xj][yj] = R(王);
    //     }
    //     else
    //     {
    //         restart = 1;
    //     }
    // }

//B（王）----------------------------------------
    // else if (array[xi][yi] == B(王))
    // {
    //     if ((xj >= 8 && yj <= 5 && yj >=3)&&(redOrBlue(xj, yj) != -1) && (((xj == xi)&&(yj == yi + 1 || yj == yi - 1))||((yj == yi)&&(xj == xi + 1 || xj == xi - 1))))
    //     {
    //         array[xi][yi] = GAP;
    //         array[xj][yj] = B(王);
    //     }
    //     else
    //     {
    //         restart = 1;
    //     }
    // }
    // else {
    //     restart = 1;
    // }
}