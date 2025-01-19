/* C���Կγ����������ҵ
�ų��� 2023K8009906024 */ 
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define SIZE 15
#define CHARSIZE 3
#define min(a,b) ((a)<(b)? (a):(b))//���������С����
#define max(a,b) ((a)>(b)? (a):(b))
void initRecordBoard(void);//���м�¼������ 
void innerLayoutToDisplayArray(int lastX,int lastY);//��������ת��Ϊʵ������ 
void displayBoard(void);//�������� 
int player;
struct point{//�ṹ���¼ÿ��λ����Ϣ 
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
//����ʹ�õ���GBK���룬ÿһ�������ַ�ռ��2���ֽڡ�
//������ģ�� 
char arrayForEmptyBoard[SIZE][SIZE*CHARSIZE+1] = 
{
		"���өөөөөөөөөөөөө�",
		"�ĩ��������������",
		"�ĩ��������������",
		"�ĩ��������������",
		"�ĩ��������������",
		"�ĩ��������������",
		"�ĩ��������������",
		"�ĩ��������������",
		"�ĩ��������������",
		"�ĩ��������������",
		"�ĩ��������������",
		"�ĩ��������������",
		"�ĩ��������������",
		"�ĩ��������������",
		"���۩۩۩۩۩۩۩۩۩۩۩۩۩�"
};
//������洢������ʾ������ 
char arrayForDisplayBoard[SIZE][SIZE*CHARSIZE+1];
char play1Pic[]="��";//������;
char play1CurrentPic[]="��"; 

char play2Pic[]="��";//������;
char play2CurrentPic[]="��";
int isInput(char *input, int *x, int *y);
//���������ڼ�¼��ǰ�����̵ĸ�� 
int arrayForInnerBoardLayout[SIZE][SIZE];
void placePiece(int x, int y, int player);
//����ģʽ 
void modle_ptop(); 
void modle_ptom();
void modle_mtop();
//�ж���Ӯ���� 
int is_win(int x, int y, int player);
//��������Ƿ����� 
int checkLine(int x, int y, int dx, int dy, int player);
//���ֺ��� 
struct point value(int x,int y);
//������� 
int if_5_lian(int s1[],int s2[],int p);
//������ 
int if_live4(int s1[],int s2[],int p);
//������� 
int if_lian_live3(int s1[],int s2[],int p);
int if_live3(int s1[],int s2[],int p);
int if_tiao_live3(int s1[],int s2[],int p);
int if_lian_live2(int s1[],int s2[],int p);
int if_tiao_live2(int s1[],int s2[],int p);
int if_single(int s1[],int s2[],int p);
//�ҳ�����λ�� 
struct point max_value(int player);

int main() {
     // ��ʼ��һ��������
    int lastX=-1,lastY=-1;
    int i,j;
    for(i=0;i<SIZE;i++){	//��ʼ��ÿ�����������Ϣ
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
    displayBoard(); // ��ʾ���� 
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
	
void modle_ptop(){//����ģʽ
    int x, y;
    char input[10];
    int lastX = -1, lastY = -1; // �����ӵ�λ��
    player=1;
    struct point p_max;
    while (1) {
       
        printf("Player %d, please enter your move (e.g., 12H or H12):", player);
        scanf("%s", input);

        if (isInput(input, &x, &y)==1) {
            if (arrayForInnerBoardLayout[x][y] == 0) { // �����ϸ�λ��Ϊ��
            	placePiece(x, y, player); // ����
                lastX = x;
                lastY = y;
                innerLayoutToDisplayArray(lastX, lastY); // ������ʾ����
                displayBoard();
                if(is_win(x,y,player)==1){
                	printf("player%d win\n",player);
               	break;
		}else if(p[x][y].state1=="˫����"){//�ж��������� 
			printf("forbidden hands,you lose\n");
		}
                player = 3 - player; // �л����
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
void modle_mtop(){//�˻�ģʽ ����ִ����
    int x, y;
    char input[10];
    int lastX = -1, lastY = -1; // �����ӵ�λ��
    player=1;
    struct point p_max;
    x=y=7;//������һ������������
    placePiece(x,y,player);
    lastX=x;
    lastY=y;
    innerLayoutToDisplayArray(lastX,lastY);
    displayBoard();
    player=3-player;
     while (1) {
     	printf("please enter your move (e.g., 12H or H12):");
        scanf("%s", input);

        if (isInput(input, &x, &y)==1) {//����Ϸ�
        	if(arrayForInnerBoardLayout[x][y] == 0) { // �����ϸ�λ��Ϊ��
                	placePiece(x, y, player); // ����
                	lastX = x;
                	lastY = y;
                	innerLayoutToDisplayArray(lastX, lastY); // ������ʾ����
                	displayBoard();
                	if(is_win(x,y,player)==1){//�ж���Ӯ
               		printf("you win\n");
               		break;
			}
                	player = 3 - player; // �л����
                	p_max=max_value(player);
                	x=p_max.x;
                	y=p_max.y;
       		placePiece(x,y,player);
       		lastX=x;
       		lastY=y;
       		innerLayoutToDisplayArray(lastX,lastY);
       		displayBoard();
       		if(is_win(x,y,player)==1){//�ж���Ӯ
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
        else if(isInput(input,&x,&y)==0) { //����Ϊquit
        	break;
        }
        else{//�������
        	displayBoard();
        	printf("error\n");
    	}

}
}
void modle_ptom(){//�˻�ģʽ ����ִ����
    int x, y;
    char input[10];
    struct point p_max;
    int lastX = -1, lastY = -1; // �����ӵ�λ��
    player=1;
    while (1) {
        
        printf("please enter your move (e.g., 12H or H12):");
        scanf("%s", input);

        if (isInput(input, &x, &y)==1) {
            if (arrayForInnerBoardLayout[x][y] == 0) { // �����ϸ�λ��Ϊ��
            	placePiece(x, y, player); // ����
                lastX = x;
                lastY = y;
                innerLayoutToDisplayArray(lastX, lastY); // ������ʾ����
                displayBoard();
                if(is_win(x,y,player)==1){//Ӯ��
                	printf("you win\n");
                	break;
		}else if(p[x][y].state1=="˫����"){
			printf("forbidden hands,you lose\n");
		}
                player = 3 - player; // �л����
        	p_max=max_value(player);
        	x=p_max.x;
        	y=p_max.y;
       	placePiece(x,y,player);
       	lastX=x;
       	lastY=y;
       	innerLayoutToDisplayArray(lastX,lastY);
       	displayBoard();
       	if(is_win(x,y,player)==1){//����
               	printf("you lose\n");
               	break;
		}
       	player=3-player;
       	max_value(1);
       	
            } 
            else {//��λ�ò�Ϊ��
            	displayBoard();
                printf("This position is already occupied!\n");
            }
        } 
        else if(isInput(input,&x,&y)==0) {//����Ϊquit
        	break;
        }
        else{//��������
        	displayBoard();
        	printf("error\n");
    	}

}


}
int is_win(int x, int y, int player) {
    // ����ĸ�����ˮƽ����ֱ�������Խ���
    if (checkLine(x, y, 0, 1, player) || // ˮƽ����
        checkLine(x, y, 1, 0, player) || // ��ֱ����
        checkLine(x, y, 1, 1, player) || // ���ϵ����¶Խ���
        checkLine(x, y, 1, -1, player)) { // ���ϵ����¶Խ���
        return 1; // ���Ӯ��
    }
    return 0; // û��Ӯ
}
// ���ָ���������Ƿ����������������
int checkLine(int x, int y, int dx, int dy, int player) {
    int count = 1; // ��ǰλ���Ѿ���һ������
    int i, j;

    // ���һ������
    for (i = x + dx, j = y + dy; i >= 0 && i < SIZE && j >= 0 && j < SIZE && arrayForInnerBoardLayout[i][j] == player; i += dx, j += dy) {
        count++;
    }
    // ����෴����
    for (i = x - dx, j = y - dy; i >= 0 && i < SIZE && j >= 0 && j < SIZE && arrayForInnerBoardLayout[i][j] == player; i -= dx, j -= dy) {
        count++;
    }

    // ��������������ﵽ5����Ӯ��
    return count >= 5;
}


//��ʼ��һ�������̸�� 
void initRecordBoard(void){
	//ͨ��˫��ѭ������arrayForInnerBoardLayout��0
        for (int i = 0; i < SIZE; i++) {
        	for (int j = 0; j < SIZE; j++) {
            		arrayForInnerBoardLayout[i][j] = 0;
        	}
    	}
}

//��arrayForInnerBoardLayout�м�¼������λ�ã�ת����arrayForDisplayBoard��
void innerLayoutToDisplayArray(int lastX,int lastY){
	//��һ������arrayForEmptyBoard�м�¼�Ŀ����̣����Ƶ�arrayForDisplayBoard��
	for (int i = 0; i < SIZE; i++) {
        	for (int j = 0; j < SIZE*CHARSIZE; j++) {
            		arrayForDisplayBoard[i][j] = arrayForEmptyBoard[i][j];
       	 }
        	arrayForDisplayBoard[i][SIZE*CHARSIZE] = '\0';
    	}



	//�ڶ�����ɨ��arrayForInnerBoardLayout����������0��Ԫ�أ�������ߡ��Ƶ�arrayForDisplayBoard����Ӧλ����

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


//��ʾ���̸�� 
void displayBoard(void){
	int i;
        system("clear");//��һ��������  
        
        printf("work of �ų���\n");
    	for (i = 0; i < SIZE; i++) {//�ڶ�������arrayForDisplayBoard�������Ļ�� 
    		printf("%3d",SIZE-i);
        	printf("%s\n", arrayForDisplayBoard[i]);
    	}
  	printf("%*s",3," ");
    	for (i = 0; i < SIZE; i++) {//������������������һ����ĸA B ....
       	 printf("%c ", 'A'+ i);
    	}
    	printf("\n");

} 
int isInput(char *input, int *x, int *y) {//�ж�����
    int len = strlen(input);
    if(strcmp(input,"quit")==0){//����quit���˳�
    	return 0;
	}
    if(strcmp(input,"back")==0){//����back��س�
    return 2;
    }
    if (len < 2 || len > 3) return -1;

    if (input[0] >= 'A' && input[0] <= 'O') { // ��ʽΪ H12
        *y =input[0] - 'A';
        *x =SIZE-atoi(&input[1]);
        
    } else if (input[len-1] >= 'A' && input[len-1] <= 'O') { // ��ʽΪ 12H
        *x = SIZE-atoi(input);
        *y = input[len-1] - 'A';
    } else if(input[0]>='a'&&input[0]<='o'){//��ʽΪh12
    	*y=input[0]-'a';
    	*x=SIZE-atoi(&input[1]);
    }else if(input[len-1]>='a'&&input[len-1]<='o'){//��ʽΪ12h
    	*x=SIZE-atoi(input);
    	*y=input[len-1]-'a';
    }
    else {
        return -1;
    }

    if (*x < 0 || *x >= SIZE || *y < 0 || *y >= SIZE) return -1;//�жϺϷ���
    return 1;
}
void placePiece(int x, int y, int player) {//����
    arrayForInnerBoardLayout[x][y] = player;
}
struct point value(int x,int y){//��һ�����������
    struct point temp; 
    temp.x=x;
    temp.y=y;
    temp.score1 = 0; 
    temp.score2 =0;  // ��ʼ������
    	
    	//��¼ÿ����˸���������
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
	
	//�жϸõ���ں�������ּ�״̬
	if(if_5_lian(temp.up,temp.down,1)==1||if_5_lian(temp.left,temp.right,1)==1||\
	if_5_lian(temp.left_down,temp.right_up,1)==1||if_5_lian(temp.left_up,temp.right_down,1)==1){
		temp.state1="����";
		temp.score1=10001; 
	}
	else if(if_live4(temp.up,temp.down,1)==1||if_live4(temp.left,temp.right,1)==1||\
	if_live4(temp.left_down,temp.right_up,1)==1||if_live4(temp.left_up,temp.right_down,1)==1){
		temp.state1="����";
		temp.score1=10000;
	}else if(if_live3(temp.up,temp.down,1)+if_live3(temp.left,temp.right,1)+\
	if_live3(temp.left_down,temp.right_up,1)+if_live3(temp.left_up,temp.right_down,1)>1){
		temp.state1="˫����";
		temp.score1=9999;
	}
	else if(if_lian_live3(temp.up,temp.down,1)==1||if_lian_live3(temp.left,temp.right,1)==1||\
	if_lian_live3(temp.left_down,temp.right_up,1)==1||if_lian_live3(temp.left_up,temp.right_down,1)==1){
		temp.state1="������";
		temp.score1=3000;
	}
	else if(if_tiao_live3(temp.up,temp.down,1)==1||if_tiao_live3(temp.left,temp.right,1)==1||\
	if_tiao_live3(temp.left_down,temp.right_up,1)==1||if_tiao_live3(temp.left_up,temp.right_down,1)==1){
		temp.state1="������";
		temp.score1=2000;
	}else if(if_lian_live2(temp.up,temp.down,1)==1||if_lian_live2(temp.left,temp.right,1)==1||\
	if_lian_live2(temp.left_down,temp.right_up,1)==1||if_lian_live2(temp.left_up,temp.right_down,1)==1){ 
	temp.score1=1000;
	temp.state1="�����";
	}else if(if_tiao_live2(temp.up,temp.down,1)==1||if_tiao_live2(temp.left,temp.right,1)==1||\
	if_tiao_live2(temp.left_down,temp.right_up,1)==1||if_tiao_live2(temp.left_up,temp.right_down,1)==1){ 
	temp.score1=100;
	temp.state1="�����";
	}else if(if_single(temp.up,temp.down,1)==1||if_single(temp.left,temp.right,1)==1||\
	if_single(temp.left_down,temp.right_up,1)==1||if_single(temp.left_up,temp.right_down,1)==1){
	temp.state1="��һ";
	temp.score1=10; 
	}else{
		temp.state1="null";
		temp.score1=1;
	}
	
	
	//�жϸõ���ڰ�������ּ�״̬
	if(if_5_lian(temp.up,temp.down,2)==1||if_5_lian(temp.left,temp.right,2)==1||\
	if_5_lian(temp.left_down,temp.right_up,2)==1||if_5_lian(temp.left_up,temp.right_down,2)==1){
		temp.state2="����";
		temp.score2=10001; 
	}else if(if_live4(temp.up,temp.down,2)==1||if_live4(temp.left,temp.right,2)==1||\
	if_live4(temp.left_down,temp.right_up,2)==1||if_live4(temp.left_up,temp.right_down,2)==1){
		temp.state2="����";
		temp.score2=10000;
	}else if(if_live3(temp.up,temp.down,2)+if_live3(temp.left,temp.right,2)+\
	if_live3(temp.left_down,temp.right_up,2)+if_live3(temp.left_up,temp.right_down,2)>1){
		temp.state2="˫����";
		temp.score2=9999;
	}else if(if_lian_live3(temp.up,temp.down,2)==1||if_lian_live3(temp.left,temp.right,2)==1||\
	if_lian_live3(temp.left_down,temp.right_up,2)==1||if_lian_live3(temp.left_up,temp.right_down,2)==1){
		temp.state2="������";
		temp.score2=3000;
	}
	else if(if_tiao_live3(temp.up,temp.down,2)==1||if_tiao_live3(temp.left,temp.right,2)==1||\
	if_tiao_live3(temp.left_down,temp.right_up,2)==1||if_tiao_live3(temp.left_up,temp.right_down,2)==1){
		temp.state2="������";
		temp.score2=2000;
	}else if(if_lian_live2(temp.up,temp.down,2)==1||if_lian_live2(temp.left,temp.right,2)==1||\
	if_lian_live2(temp.left_down,temp.right_up,2)==1||if_lian_live2(temp.left_up,temp.right_down,2)==1){ 
		temp.score2=1000;
		temp.state2="�����";
	}else if(if_tiao_live2(temp.up,temp.down,2)==1||if_tiao_live2(temp.left,temp.right,2)==1||\
	if_tiao_live2(temp.left_down,temp.right_up,2)==1||if_tiao_live2(temp.left_up,temp.right_down,2)==1){ 
		temp.score2=100;
		temp.state2="�����";
	}else if(if_single(temp.up,temp.down,2)==1||if_single(temp.left,temp.right,2)==1||\
	if_single(temp.left_down,temp.right_up,2)==1||if_single(temp.left_up,temp.right_down,2)==1){
	temp.state2="��һ";
	temp.score2=10; 
	}else{
		temp.state2="null";
		temp.score2=1;
	}
	return temp;
}
int if_5_lian(int s1[],int s2[],int p){//�жϸ÷����Ƿ�����
	if(
	((s1[0]==p&&s1[1]==p&&s1[2]==p&&s1[3]==p)||(s2[0]==p&&s2[1]==p&&s2[2]==p&&s2[3]==p))|| \
	((s1[0]==p&&s1[1]==p&&s1[2]==p&&s2[0]==p)||(s2[0]==p&&s2[1]==p&&s2[2]==p&&s1[0]==p))|| \
	(s1[0]==p&&s1[1]==p&&s2[0]==p&&s2[1]==p)
	){
		return 1;
	}
	return 0;
}

int if_live4(int s1[],int s2[],int p){//�жϸ÷����Ƿ����
	if(
	(s1[0]==p&&s1[1]==p&&s1[2]==p&&s1[3]==0&&s2[0]==0)||(s2[0]==p&&s2[1]==p&&s2[2]==p&&s2[3]==0&&s1[0]==0)||\
	((s1[0]==p&&s1[1]==p&&s1[2]==0&&s2[0]==p&&s2[1]==0)||(s2[0]==p&&s2[1]==p&&s2[2]==0&&s1[0]==p&&s1[1]==0))
	){
		return 1;
	}
	return 0;
}
int if_live3(int s1[],int s2[],int p){//�жϸ÷����Ƿ����
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
int if_lian_live3(int s1[],int s2[],int p){//�жϸ÷����Ƿ�������
	if(
	(s1[0]==p&&s1[1]==p&&s1[2]==0&&s1[3]==0&&s2[0]==0)||(s2[0]==p&&s2[1]==p&&s2[2]==0&&s2[3]==0&&s1[0]==0)||\
	(s1[0]==p&&s1[1]==0&&s1[2]==0&&s2[0]==p&&s2[1]==0)||(s2[0]==p&&s2[1]==0&&s2[2]==0&&s1[0]==p&&s1[1]==0)||\
	(s1[0]==p&&s1[1]==p&&s1[2]==0&&s2[0]==0&&s2[1]==0)||(s2[0]==p&&s2[1]==p&&s2[2]==0&&s1[0]==0&&s1[1]==0)
	){
		return 1;
	}
	return 0;
}
int if_tiao_live3(int s1[],int s2[],int p){//�жϸ÷����Ƿ�������
	if(
	(s1[0]==0&&s1[1]==p&&s1[2]==p&&s1[3]==0&&s2[0]==0)||(s2[0]==0&&s2[1]==p&&s2[2]==p&&s2[3]==0&&s1[0]==0)||\
	(s1[0]==0&&s1[1]==p&&s1[2]==0&&s2[0]==p&&s2[1]==0)||(s2[0]==0&&s2[1]==p&&s2[2]==0&&s1[0]==p&&s1[1]==0)||\
	(s1[0]==p&&s1[1]==0&&s1[2]==p&&s1[3]==0&&s2[0]==0)||(s2[0]==p&&s2[1]==0&&s2[2]==p&&s2[3]==0&&s1[0]==0)
	){
		return 1;
	}
	return 0;
}
int if_lian_live2(int s1[],int s2[],int p){//�жϸ÷����Ƿ������
	if(
	(s1[0]==p&&s1[1]==0&&s1[2]==0&&s1[3]==0&&s2[0]==0)||(s2[0]==p&&s2[1]==0&&s2[2]==0&&s2[3]==0&&s1[0]==0)||\
	(s1[0]==0&&s1[1]==0&&s1[2]==0&&s2[0]==p&&s2[1]==0)||(s2[0]==0&&s2[1]==0&&s2[2]==0&&s1[0]==p&&s1[1]==0)||\
	(s1[0]==p&&s1[1]==0&&s1[2]==0&&s2[0]==0&&s2[1]==0)||(s2[0]==p&&s2[1]==0&&s2[2]==0&&s1[0]==0&&s1[1]==0)\
	){
		return 1;
	}
	return 0;
}
int if_tiao_live2(int s1[],int s2[],int p){//�жϸ÷����Ƿ�Ϊ�����
	if(
	(s1[0]==0&&s1[1]==p&&s1[2]==0&&s1[3]==0&&s2[0]==0)||(s2[0]==0&&s2[1]==p&&s2[2]==0&&s2[3]==0&&s1[0]==0)||\
	(s1[0]==0&&s1[1]==p&&s1[2]==0&&s2[0]==0&&s2[1]==0)||(s2[0]==0&&s2[1]==p&&s2[2]==0&&s1[0]==0&&s1[1]==0)||\
	(s1[0]==0&&s1[1]==0&&s1[2]==p&&s1[3]==0&&s2[0]==0)||(s2[0]==0&&s2[1]==0&&s2[2]==p&&s2[3]==0&&s1[0]==0)\
	){
	return 1;
	}
	return 0;
}
int if_single(int s1[],int s2[],int p){//�жϸ÷����Ƿ���
	if(s1[0]==0&&s1[1]==0&&s1[2]==0&&s1[3]==0&&s2[0]==0&&s2[1]==0&&s2[2]==0&&s2[3]==0){
		return 1;
	}
	return 0;
}
struct point max_value(int player) {//�ҳ��������������㣬�����ǵ�ǰһ�� 
    int i,j;
    struct point temp;
    temp.score1=0;
    temp.score2=0;
    for(i=0;i<SIZE;i++){
    	for(j=0;j<SIZE;j++){
    	if(arrayForInnerBoardLayout[i][j]==0){
    		p[i][j]=value(i,j);
    		if(player==1){
    			if(temp.state1=="����"){
    			;
    			}
    			else if(p[i][j].state1=="����"){
    				temp=p[i][j];
    			}
				else if(p[i][j].state1=="˫����"){
					;
				}
    			else if(temp.state2!="����"&&p[i][j].state1=="����"){
    				temp=p[i][j];
				}else if(temp.state1=="����"&&p[i][j].state2!="����"){
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
    			if(temp.state2=="����"){
    				;
    			}
    			else if(p[i][j].state2=="����"){
    				temp=p[i][j];
    			}
    			else if(temp.state1!="����"&&p[i][j].state2=="����"){
    				temp=p[i][j];
    			}
				else if(temp.state2=="����"&&p[i][j].state1!="����"){
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

