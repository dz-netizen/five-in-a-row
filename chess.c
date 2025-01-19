/* C语言课程五子棋大作业
张超超 2023K8009906024 */ 
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define SIZE 15
#define CHARSIZE 3
#define min(a,b) ((a)<(b)? (a):(b))//定义最大最小函数
#define max(a,b) ((a)>(b)? (a):(b))
void initRecordBoard(void);//心中记录的棋盘 
void innerLayoutToDisplayArray(int lastX,int lastY);//心中棋盘转化为实际棋盘 
void displayBoard(void);//画出棋盘 
int player;
struct point{//结构体记录每个位置信息 
	int x;
	int y;
	int up[4];
	int down[4];
	int left[4];
	int right[4];
	int left_up[4];
	int left_down[4];
	int right_up[4];
	int right_down[4];
	int score1;
	int score2;
	char *state1;
	char *state2;
}p[SIZE][SIZE];
//棋盘使用的是GBK编码，每一个中文字符占用2个字节。
//空棋盘模板 
char arrayForEmptyBoard[SIZE][SIZE*CHARSIZE+1] = 
{
		"┏┯┯┯┯┯┯┯┯┯┯┯┯┯┓",
		"┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
		"┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
		"┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
		"┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
		"┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
		"┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
		"┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
		"┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
		"┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
		"┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
		"┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
		"┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
		"┠┼┼┼┼┼┼┼┼┼┼┼┼┼┨",
		"┗┷┷┷┷┷┷┷┷┷┷┷┷┷┛"
};
//此数组存储用于显示的棋盘 
char arrayForDisplayBoard[SIZE][SIZE*CHARSIZE+1];
char play1Pic[]="●";//黑棋子;
char play1CurrentPic[]="▲"; 

char play2Pic[]="◎";//白棋子;
char play2CurrentPic[]="△";
int isInput(char *input, int *x, int *y);
//此数组用于记录当前的棋盘的格局 
int arrayForInnerBoardLayout[SIZE][SIZE];
void placePiece(int x, int y, int player);
//三种模式 
void modle_ptop(); 
void modle_ptom();
void modle_mtop();
//判断输赢函数 
int is_win(int x, int y, int player);
//检查行列是否五连 
int checkLine(int x, int y, int dx, int dy, int player);
//评分函数 
struct point value(int x,int y);
//检查五连 
int if_5_lian(int s1[],int s2[],int p);
//检查活四 
int if_live4(int s1[],int s2[],int p);
//检查连三 
int if_lian_live3(int s1[],int s2[],int p);
int if_live3(int s1[],int s2[],int p);
int if_tiao_live3(int s1[],int s2[],int p);
int if_lian_live2(int s1[],int s2[],int p);
int if_tiao_live2(int s1[],int s2[],int p);
int if_single(int s1[],int s2[],int p);
//找出最优位置 
struct point max_value(int player);

int main() {
     // 初始化一个空棋盘
    int lastX=-1,lastY=-1;
    int i,j;
    for(i=0;i<SIZE;i++){	//初始化每个落点的相关信息
    	for(j=0;j<SIZE;j++){
    	    p[i][j].x = 0;
            p[i][j].y = 0;
            memset(p[i][j].up, 0, sizeof(p[i][j].up));
            memset(p[i][j].down, 0, sizeof(p[i][j].down));
            memset(p[i][j].left, 0, sizeof(p[i][j].left));
            memset(p[i][j].right, 0, sizeof(p[i][j].right));
            memset(p[i][j].left_up, 0, sizeof(p[i][j].left_up));
            memset(p[i][j].left_down, 0, sizeof(p[i][j].left_down));
            memset(p[i][j].right_up, 0, sizeof(p[i][j].right_up));
            memset(p[i][j].right_down, 0, sizeof(p[i][j].right_down));
            p[i][j].score1 = 0;
            p[i][j].score2 = 0;
            p[i][j].state2 = NULL;
            p[i][j].state1 = NULL; 
	}
    }
    initRecordBoard();
    int modle;
    innerLayoutToDisplayArray(lastX,lastY);
    displayBoard(); // 显示棋盘 
    printf("choose modle:0(ptop);1(ptom)2(mtop)");
    scanf("%d",&modle); 
    switch(modle){
    	case 0:modle_ptop();
    	break;
    	case 1:modle_ptom();
    	break;
    	case 2:modle_mtop();
    	break;
    }
	printf("you win");
    return 0;
}
	
void modle_ptop(){//人人模式
    int x, y;
    char input[10];
    int lastX = -1, lastY = -1; // 刚落子的位置
    player=1;
    struct point p_max;
    while (1) {
       
        printf("Player %d, please enter your move (e.g., 12H or H12):", player);
        scanf("%s", input);

        if (isInput(input, &x, &y)==1) {
            if (arrayForInnerBoardLayout[x][y] == 0) { // 棋盘上该位置为空
            	placePiece(x, y, player); // 落子
                lastX = x;
                lastY = y;
                innerLayoutToDisplayArray(lastX, lastY); // 更新显示数组
                displayBoard();
                if(is_win(x,y,player)==1){
                	printf("player%d win\n",player);
               	break;
		}else if(p[x][y].state1=="双活三"){//判断三三禁手 
			printf("forbidden hands,you lose\n");
		}
                player = 3 - player; // 切换玩家
             } 
             else {
             	displayBoard();
                printf("This position is already occupied!\n");
             }
        } 
        else if(isInput(input,&x,&y)==0) {
        	break;
        }
        else if(isInput(input,&x,&y)==2){
        	arrayForInnerBoardLayout[lastX][lastY]=0;
        	innerLayoutToDisplayArray(lastX,lastY);
        	displayBoard();
        	lastX=-1;
        	lastY=-1;
        	player=3-player;
        }
        else{
        displayBoard();
        printf("error\n");
    	}
  	p_max=max_value(1);
    }
    

}
void modle_mtop(){//人机模式 机器执黑棋
    int x, y;
    char input[10];
    int lastX = -1, lastY = -1; // 刚落子的位置
    player=1;
    struct point p_max;
    x=y=7;//机器第一颗子落在中心
    placePiece(x,y,player);
    lastX=x;
    lastY=y;
    innerLayoutToDisplayArray(lastX,lastY);
    displayBoard();
    player=3-player;
     while (1) {
     	printf("please enter your move (e.g., 12H or H12):");
        scanf("%s", input);

        if (isInput(input, &x, &y)==1) {//输入合法
        	if(arrayForInnerBoardLayout[x][y] == 0) { // 棋盘上该位置为空
                	placePiece(x, y, player); // 落子
                	lastX = x;
                	lastY = y;
                	innerLayoutToDisplayArray(lastX, lastY); // 更新显示数组
                	displayBoard();
                	if(is_win(x,y,player)==1){//判断输赢
               		printf("you win\n");
               		break;
			}
                	player = 3 - player; // 切换玩家
                	p_max=max_value(player);
                	x=p_max.x;
                	y=p_max.y;
       		placePiece(x,y,player);
       		lastX=x;
       		lastY=y;
       		innerLayoutToDisplayArray(lastX,lastY);
       		displayBoard();
       		if(is_win(x,y,player)==1){//判断输赢
               		printf("you lose \n");
               		break;
			}
       		player=3-player;
            	} 
           	else {
            		displayBoard();
                	printf("This position is already occupied!\n");
            	}
        	} 
        else if(isInput(input,&x,&y)==0) { //输入为quit
        	break;
        }
        else{//输入错误
        	displayBoard();
        	printf("error\n");
    	}

}
}
void modle_ptom(){//人机模式 机器执白棋
    int x, y;
    char input[10];
    struct point p_max;
    int lastX = -1, lastY = -1; // 刚落子的位置
    player=1;
    while (1) {
        
        printf("please enter your move (e.g., 12H or H12):");
        scanf("%s", input);

        if (isInput(input, &x, &y)==1) {
            if (arrayForInnerBoardLayout[x][y] == 0) { // 棋盘上该位置为空
            	placePiece(x, y, player); // 落子
                lastX = x;
                lastY = y;
                innerLayoutToDisplayArray(lastX, lastY); // 更新显示数组
                displayBoard();
                if(is_win(x,y,player)==1){//赢了
                	printf("you win\n");
                	break;
		}else if(p[x][y].state1=="双活三"){
			printf("forbidden hands,you lose\n");
		}
                player = 3 - player; // 切换玩家
        	p_max=max_value(player);
        	x=p_max.x;
        	y=p_max.y;
       	placePiece(x,y,player);
       	lastX=x;
       	lastY=y;
       	innerLayoutToDisplayArray(lastX,lastY);
       	displayBoard();
       	if(is_win(x,y,player)==1){//输了
               	printf("you lose\n");
               	break;
		}
       	player=3-player;
       	max_value(1);
       	
            } 
            else {//该位置不为空
            	displayBoard();
                printf("This position is already occupied!\n");
            }
        } 
        else if(isInput(input,&x,&y)==0) {//输入为quit
        	break;
        }
        else{//其他输入
        	displayBoard();
        	printf("error\n");
    	}

}


}
int is_win(int x, int y, int player) {
    // 检查四个方向：水平、垂直、两条对角线
    if (checkLine(x, y, 0, 1, player) || // 水平方向
        checkLine(x, y, 1, 0, player) || // 垂直方向
        checkLine(x, y, 1, 1, player) || // 右上到左下对角线
        checkLine(x, y, 1, -1, player)) { // 左上到右下对角线
        return 1; // 玩家赢了
    }
    return 0; // 没有赢
}
// 检查指定方向上是否有连续的五个棋子
int checkLine(int x, int y, int dx, int dy, int player) {
    int count = 1; // 当前位置已经有一个棋子
    int i, j;

    // 检查一个方向
    for (i = x + dx, j = y + dy; i >= 0 && i < SIZE && j >= 0 && j < SIZE && arrayForInnerBoardLayout[i][j] == player; i += dx, j += dy) {
        count++;
    }
    // 检查相反方向
    for (i = x - dx, j = y - dy; i >= 0 && i < SIZE && j >= 0 && j < SIZE && arrayForInnerBoardLayout[i][j] == player; i -= dx, j -= dy) {
        count++;
    }

    // 如果连续棋子数达到5，则赢棋
    return count >= 5;
}


//初始化一个空棋盘格局 
void initRecordBoard(void){
	//通过双重循环，将arrayForInnerBoardLayout清0
        for (int i = 0; i < SIZE; i++) {
        	for (int j = 0; j < SIZE; j++) {
            		arrayForInnerBoardLayout[i][j] = 0;
        	}
    	}
}

//将arrayForInnerBoardLayout中记录的棋子位置，转化到arrayForDisplayBoard中
void innerLayoutToDisplayArray(int lastX,int lastY){
	//第一步：将arrayForEmptyBoard中记录的空棋盘，复制到arrayForDisplayBoard中
	for (int i = 0; i < SIZE; i++) {
        	for (int j = 0; j < SIZE*CHARSIZE; j++) {
            		arrayForDisplayBoard[i][j] = arrayForEmptyBoard[i][j];
       	 }
        	arrayForDisplayBoard[i][SIZE*CHARSIZE] = '\0';
    	}



	//第二步：扫描arrayForInnerBoardLayout，当遇到非0的元素，将●或者◎复制到arrayForDisplayBoard的相应位置上

    	for (int i = 0; i < SIZE; i++) {
        	for (int j = 0; j < SIZE; j++) {
            		if (arrayForInnerBoardLayout[i][j] == 1) {
            			if(i==lastX&&j==lastY){
           				 arrayForDisplayBoard[i][j*CHARSIZE] = play1CurrentPic[0];
               			 arrayForDisplayBoard[i][j*CHARSIZE+1] = play1CurrentPic[1];
                			 arrayForDisplayBoard[i][j*CHARSIZE+2] =play1CurrentPic[2];
            			}else {
                			arrayForDisplayBoard[i][j*CHARSIZE] = play1Pic[0];
                			arrayForDisplayBoard[i][j*CHARSIZE+1] = play1Pic[1];
               		 	arrayForDisplayBoard[i][j*CHARSIZE+2] =play1Pic[2];}
            		} 
            		else if (arrayForInnerBoardLayout[i][j] == 2) {
            			if(i==lastX&&j==lastY){
            				arrayForDisplayBoard[i][j*CHARSIZE] = play2CurrentPic[0];
               	 		arrayForDisplayBoard[i][j*CHARSIZE+1] = play2CurrentPic[1];
                			arrayForDisplayBoard[i][j*CHARSIZE+2] =play2CurrentPic[2];}
                		else{
                			arrayForDisplayBoard[i][j*CHARSIZE] = play2Pic[0];
                			arrayForDisplayBoard[i][j*CHARSIZE+1] = play2Pic[1];
                			arrayForDisplayBoard[i][j*CHARSIZE+2] =play2Pic[2];}
            		}
       	}
    } 

}


//显示棋盘格局 
void displayBoard(void){
	int i;
        system("clear");//第一步：清屏  
        
        printf("work of 张超超\n");
    	for (i = 0; i < SIZE; i++) {//第二步：将arrayForDisplayBoard输出到屏幕上 
    		printf("%3d",SIZE-i);
        	printf("%s\n", arrayForDisplayBoard[i]);
    	}
  	printf("%*s",3," ");
    	for (i = 0; i < SIZE; i++) {//第三步：输出最下面的一行字母A B ....
       	 printf("%c ", 'A'+ i);
    	}
    	printf("\n");

} 
int isInput(char *input, int *x, int *y) {//判断输入
    int len = strlen(input);
    if(strcmp(input,"quit")==0){//输入quit则退出
    	return 0;
	}
    if(strcmp(input,"back")==0){//输入back则回车
    return 2;
    }
    if (len < 2 || len > 3) return -1;

    if (input[0] >= 'A' && input[0] <= 'O') { // 格式为 H12
        *y =input[0] - 'A';
        *x =SIZE-atoi(&input[1]);
        
    } else if (input[len-1] >= 'A' && input[len-1] <= 'O') { // 格式为 12H
        *x = SIZE-atoi(input);
        *y = input[len-1] - 'A';
    } else if(input[0]>='a'&&input[0]<='o'){//格式为h12
    	*y=input[0]-'a';
    	*x=SIZE-atoi(&input[1]);
    }else if(input[len-1]>='a'&&input[len-1]<='o'){//格式为12h
    	*x=SIZE-atoi(input);
    	*y=input[len-1]-'a';
    }
    else {
        return -1;
    }

    if (*x < 0 || *x >= SIZE || *y < 0 || *y >= SIZE) return -1;//判断合法性
    return 1;
}
void placePiece(int x, int y, int player) {//落子
    arrayForInnerBoardLayout[x][y] = player;
}
struct point value(int x,int y){//对一个点进行评分
    struct point temp; 
    temp.x=x;
    temp.y=y;
    temp.score1 = 0; 
    temp.score2 =0;  // 初始化分数
    	
    	//记录每个点八个方向的情况
    	int i, j;
	for(i=x,j=y+1;j<y+5&&i>=0&&i<15&&j<15&&j>=0;j++){
		temp.right[j-y-1]=arrayForInnerBoardLayout[i][j];
	}
	for(i=x,j=y-1;j>y-5&&i>=0&&i<15&&j<15&&j>=0;j--){
		temp.left[y-j-1]=arrayForInnerBoardLayout[i][j];
	}
	for(i=x-1,j=y;i>x-5&&i>=0&&i<15&&j<15&&j>=0;i--){
		temp.up[x-i-1]=arrayForInnerBoardLayout[i][j];
	}
	for(i=x+1,j=y;i<x+5&&i>=0&&i<15&&j<15&&j>=0;i++){
		temp.down[i-x-1]=arrayForInnerBoardLayout[i][j];
	}
	for(i=x-1,j=y+1;j<y+5&&i>=0&&i<15&&j<15&&j>=0;i--,j++){
		temp.right_up[j-y-1]=arrayForInnerBoardLayout[i][j];
	}
	for(i=x-1,j=y-1;j>y-5&&i>=0&&i<15&&j<15&&j>=0;i--,j--){
		temp.left_up[x-i-1]=arrayForInnerBoardLayout[i][j];
	}
	for(i=x+1,j=y+1;j<y+5&&i>=0&&i<15&&j<15&&j>=0;i++,j++){
		temp.right_down[j-y-1]=arrayForInnerBoardLayout[i][j];
	}
	for(i=x+1,j=y-1;i<x+5&&i>=0&&i<15&&j<15&&j>=0;i++,j--){
		temp.left_down[i-x-1]=arrayForInnerBoardLayout[i][j];
	}
	
	//判断该点对于黑棋的评分及状态
	if(if_5_lian(temp.up,temp.down,1)==1||if_5_lian(temp.left,temp.right,1)==1||\
	if_5_lian(temp.left_down,temp.right_up,1)==1||if_5_lian(temp.left_up,temp.right_down,1)==1){
		temp.state1="五连";
		temp.score1=10001; 
	}
	else if(if_live4(temp.up,temp.down,1)==1||if_live4(temp.left,temp.right,1)==1||\
	if_live4(temp.left_down,temp.right_up,1)==1||if_live4(temp.left_up,temp.right_down,1)==1){
		temp.state1="活四";
		temp.score1=10000;
	}else if(if_live3(temp.up,temp.down,1)+if_live3(temp.left,temp.right,1)+\
	if_live3(temp.left_down,temp.right_up,1)+if_live3(temp.left_up,temp.right_down,1)>1){
		temp.state1="双活三";
		temp.score1=9999;
	}
	else if(if_lian_live3(temp.up,temp.down,1)==1||if_lian_live3(temp.left,temp.right,1)==1||\
	if_lian_live3(temp.left_down,temp.right_up,1)==1||if_lian_live3(temp.left_up,temp.right_down,1)==1){
		temp.state1="连活三";
		temp.score1=3000;
	}
	else if(if_tiao_live3(temp.up,temp.down,1)==1||if_tiao_live3(temp.left,temp.right,1)==1||\
	if_tiao_live3(temp.left_down,temp.right_up,1)==1||if_tiao_live3(temp.left_up,temp.right_down,1)==1){
		temp.state1="跳活三";
		temp.score1=2000;
	}else if(if_lian_live2(temp.up,temp.down,1)==1||if_lian_live2(temp.left,temp.right,1)==1||\
	if_lian_live2(temp.left_down,temp.right_up,1)==1||if_lian_live2(temp.left_up,temp.right_down,1)==1){ 
	temp.score1=1000;
	temp.state1="连活二";
	}else if(if_tiao_live2(temp.up,temp.down,1)==1||if_tiao_live2(temp.left,temp.right,1)==1||\
	if_tiao_live2(temp.left_down,temp.right_up,1)==1||if_tiao_live2(temp.left_up,temp.right_down,1)==1){ 
	temp.score1=100;
	temp.state1="跳活二";
	}else if(if_single(temp.up,temp.down,1)==1||if_single(temp.left,temp.right,1)==1||\
	if_single(temp.left_down,temp.right_up,1)==1||if_single(temp.left_up,temp.right_down,1)==1){
	temp.state1="活一";
	temp.score1=10; 
	}else{
		temp.state1="null";
		temp.score1=1;
	}
	
	
	//判断该点对于白棋的评分及状态
	if(if_5_lian(temp.up,temp.down,2)==1||if_5_lian(temp.left,temp.right,2)==1||\
	if_5_lian(temp.left_down,temp.right_up,2)==1||if_5_lian(temp.left_up,temp.right_down,2)==1){
		temp.state2="五连";
		temp.score2=10001; 
	}else if(if_live4(temp.up,temp.down,2)==1||if_live4(temp.left,temp.right,2)==1||\
	if_live4(temp.left_down,temp.right_up,2)==1||if_live4(temp.left_up,temp.right_down,2)==1){
		temp.state2="活四";
		temp.score2=10000;
	}else if(if_live3(temp.up,temp.down,2)+if_live3(temp.left,temp.right,2)+\
	if_live3(temp.left_down,temp.right_up,2)+if_live3(temp.left_up,temp.right_down,2)>1){
		temp.state2="双活三";
		temp.score2=9999;
	}else if(if_lian_live3(temp.up,temp.down,2)==1||if_lian_live3(temp.left,temp.right,2)==1||\
	if_lian_live3(temp.left_down,temp.right_up,2)==1||if_lian_live3(temp.left_up,temp.right_down,2)==1){
		temp.state2="连活三";
		temp.score2=3000;
	}
	else if(if_tiao_live3(temp.up,temp.down,2)==1||if_tiao_live3(temp.left,temp.right,2)==1||\
	if_tiao_live3(temp.left_down,temp.right_up,2)==1||if_tiao_live3(temp.left_up,temp.right_down,2)==1){
		temp.state2="跳活三";
		temp.score2=2000;
	}else if(if_lian_live2(temp.up,temp.down,2)==1||if_lian_live2(temp.left,temp.right,2)==1||\
	if_lian_live2(temp.left_down,temp.right_up,2)==1||if_lian_live2(temp.left_up,temp.right_down,2)==1){ 
		temp.score2=1000;
		temp.state2="连活二";
	}else if(if_tiao_live2(temp.up,temp.down,2)==1||if_tiao_live2(temp.left,temp.right,2)==1||\
	if_tiao_live2(temp.left_down,temp.right_up,2)==1||if_tiao_live2(temp.left_up,temp.right_down,2)==1){ 
		temp.score2=100;
		temp.state2="跳活二";
	}else if(if_single(temp.up,temp.down,2)==1||if_single(temp.left,temp.right,2)==1||\
	if_single(temp.left_down,temp.right_up,2)==1||if_single(temp.left_up,temp.right_down,2)==1){
	temp.state2="活一";
	temp.score2=10; 
	}else{
		temp.state2="null";
		temp.score2=1;
	}
	return temp;
}
int if_5_lian(int s1[],int s2[],int p){//判断该方向是否五连
	if(
	((s1[0]==p&&s1[1]==p&&s1[2]==p&&s1[3]==p)||(s2[0]==p&&s2[1]==p&&s2[2]==p&&s2[3]==p))|| \
	((s1[0]==p&&s1[1]==p&&s1[2]==p&&s2[0]==p)||(s2[0]==p&&s2[1]==p&&s2[2]==p&&s1[0]==p))|| \
	(s1[0]==p&&s1[1]==p&&s2[0]==p&&s2[1]==p)
	){
		return 1;
	}
	return 0;
}

int if_live4(int s1[],int s2[],int p){//判断该方向是否活四
	if(
	(s1[0]==p&&s1[1]==p&&s1[2]==p&&s1[3]==0&&s2[0]==0)||(s2[0]==p&&s2[1]==p&&s2[2]==p&&s2[3]==0&&s1[0]==0)||\
	((s1[0]==p&&s1[1]==p&&s1[2]==0&&s2[0]==p&&s2[1]==0)||(s2[0]==p&&s2[1]==p&&s2[2]==0&&s1[0]==p&&s1[1]==0))
	){
		return 1;
	}
	return 0;
}
int if_live3(int s1[],int s2[],int p){//判断该方向是否活三
	if(
	(s1[0]==p&&s1[1]==p&&s1[2]==0&&s1[3]==0&&s2[0]==0)||(s2[0]==p&&s2[1]==p&&s2[2]==0&&s2[3]==0&&s1[0]==0)||\
	(s1[0]==p&&s1[1]==0&&s1[2]==0&&s2[0]==p&&s2[1]==0)||(s2[0]==p&&s2[1]==0&&s2[2]==0&&s1[0]==p&&s1[1]==0)||\
	(s1[0]==p&&s1[1]==p&&s1[2]==0&&s2[0]==0&&s2[1]==0)||(s2[0]==p&&s2[1]==p&&s2[2]==0&&s1[0]==0&&s1[1]==0)||\
	(s1[0]==0&&s1[1]==p&&s1[2]==p&&s1[3]==0&&s2[0]==0)||(s2[0]==0&&s2[1]==p&&s2[2]==p&&s2[3]==0&&s1[0]==0)||\
	(s1[0]==0&&s1[1]==p&&s1[2]==0&&s2[0]==p&&s2[1]==0)||(s2[0]==0&&s2[1]==p&&s2[2]==0&&s1[0]==p&&s1[1]==0)||\
	(s1[0]==p&&s1[1]==0&&s1[2]==p&&s1[3]==0&&s2[0]==0)||(s2[0]==p&&s2[1]==0&&s2[2]==p&&s2[3]==0&&s1[0]==0)
	){
		return 1;
	}
	return 0;
}
int if_lian_live3(int s1[],int s2[],int p){//判断该方向是否连活三
	if(
	(s1[0]==p&&s1[1]==p&&s1[2]==0&&s1[3]==0&&s2[0]==0)||(s2[0]==p&&s2[1]==p&&s2[2]==0&&s2[3]==0&&s1[0]==0)||\
	(s1[0]==p&&s1[1]==0&&s1[2]==0&&s2[0]==p&&s2[1]==0)||(s2[0]==p&&s2[1]==0&&s2[2]==0&&s1[0]==p&&s1[1]==0)||\
	(s1[0]==p&&s1[1]==p&&s1[2]==0&&s2[0]==0&&s2[1]==0)||(s2[0]==p&&s2[1]==p&&s2[2]==0&&s1[0]==0&&s1[1]==0)
	){
		return 1;
	}
	return 0;
}
int if_tiao_live3(int s1[],int s2[],int p){//判断该方向是否跳活三
	if(
	(s1[0]==0&&s1[1]==p&&s1[2]==p&&s1[3]==0&&s2[0]==0)||(s2[0]==0&&s2[1]==p&&s2[2]==p&&s2[3]==0&&s1[0]==0)||\
	(s1[0]==0&&s1[1]==p&&s1[2]==0&&s2[0]==p&&s2[1]==0)||(s2[0]==0&&s2[1]==p&&s2[2]==0&&s1[0]==p&&s1[1]==0)||\
	(s1[0]==p&&s1[1]==0&&s1[2]==p&&s1[3]==0&&s2[0]==0)||(s2[0]==p&&s2[1]==0&&s2[2]==p&&s2[3]==0&&s1[0]==0)
	){
		return 1;
	}
	return 0;
}
int if_lian_live2(int s1[],int s2[],int p){//判断该方向是否连活二
	if(
	(s1[0]==p&&s1[1]==0&&s1[2]==0&&s1[3]==0&&s2[0]==0)||(s2[0]==p&&s2[1]==0&&s2[2]==0&&s2[3]==0&&s1[0]==0)||\
	(s1[0]==0&&s1[1]==0&&s1[2]==0&&s2[0]==p&&s2[1]==0)||(s2[0]==0&&s2[1]==0&&s2[2]==0&&s1[0]==p&&s1[1]==0)||\
	(s1[0]==p&&s1[1]==0&&s1[2]==0&&s2[0]==0&&s2[1]==0)||(s2[0]==p&&s2[1]==0&&s2[2]==0&&s1[0]==0&&s1[1]==0)\
	){
		return 1;
	}
	return 0;
}
int if_tiao_live2(int s1[],int s2[],int p){//判断该方向是否为跳活二
	if(
	(s1[0]==0&&s1[1]==p&&s1[2]==0&&s1[3]==0&&s2[0]==0)||(s2[0]==0&&s2[1]==p&&s2[2]==0&&s2[3]==0&&s1[0]==0)||\
	(s1[0]==0&&s1[1]==p&&s1[2]==0&&s2[0]==0&&s2[1]==0)||(s2[0]==0&&s2[1]==p&&s2[2]==0&&s1[0]==0&&s1[1]==0)||\
	(s1[0]==0&&s1[1]==0&&s1[2]==p&&s1[3]==0&&s2[0]==0)||(s2[0]==0&&s2[1]==0&&s2[2]==p&&s2[3]==0&&s1[0]==0)\
	){
	return 1;
	}
	return 0;
}
int if_single(int s1[],int s2[],int p){//判断该方向是否单子
	if(s1[0]==0&&s1[1]==0&&s1[2]==0&&s1[3]==0&&s2[0]==0&&s2[1]==0&&s2[2]==0&&s2[3]==0){
		return 1;
	}
	return 0;
}
struct point max_value(int player) {//找出棋面分数最大的落点，仅考虑当前一步 
    int i,j;
    struct point temp;
    temp.score1=0;
    temp.score2=0;
    for(i=0;i<SIZE;i++){
    	for(j=0;j<SIZE;j++){
    	if(arrayForInnerBoardLayout[i][j]==0){
    		p[i][j]=value(i,j);
    		if(player==1){
    			if(temp.state1=="五连"){
    			;
    			}
    			else if(p[i][j].state1=="五连"){
    				temp=p[i][j];
    			}
				else if(p[i][j].state1=="双活三"){
					;
				}
    			else if(temp.state2!="五连"&&p[i][j].state1=="活四"){
    				temp=p[i][j];
				}else if(temp.state1=="活四"&&p[i][j].state2!="五连"){
					;
				}
    			else if(p[i][j].score2>temp.score2){
    				temp=p[i][j];	
    			}
    			else if(p[i][j].score2==temp.score2&&p[i][j].score1>temp.score1){
    				temp=p[i][j];
    			}
    		}
    		else{
    			if(temp.state2=="五连"){
    				;
    			}
    			else if(p[i][j].state2=="五连"){
    				temp=p[i][j];
    			}
    			else if(temp.state1!="五连"&&p[i][j].state2=="活四"){
    				temp=p[i][j];
    			}
				else if(temp.state2=="活四"&&p[i][j].state1!="五连"){
					;
				}	
    			else if(p[i][j].score1>temp.score1){
    			temp=p[i][j];
    			}
    			else if(p[i][j].score1==temp.score1&&p[i][j].score2>temp.score2){
    			temp=p[i][j];
    			}
    		}
    	    }
    	}
    }
    return temp;
    
}

