void choose_Option1();/*選擇進行之動作*/
void choose_Option2();/*選擇進行之動作*/
int goback();/*悔棋*/
#define Blue(piece) "\033[34m"#piece"\033[0m"//藍色棋子
#define Red(piece) "\033[31m"#piece"\033[0m"//紅色棋子
#define GAP "\033[33m口\033[0m"//棋盤格子
#define White(piece) "\033[37m"#piece"\033[0m"//白色數字
#define ROW 10
#define COLUMN 10
#define MAXSTACK 500/*定義最大堆疊容量*/
