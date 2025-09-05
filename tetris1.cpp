#include<iostream>
#include<ctime>
#include<string>//字符串处理
#include<cstring>
#include<sstream>//字符串处理
#include<cstdlib>
#include<cstdio>
#include<windows.h>
#include<easyx.h>//画图
#include<graphics.h>
#include<conio.h>//kbhit
#include<fstream>//文件
#include<stdio.h>

using namespace std;


#define GREY 0xC0C0C0//灰
#define WHITE 0xFFFFFF
#define PURPLE 0x8A2BE2//violet;
#define PINK 0xDA70D6//淡紫
#define FORESTGREEN 0x228B22//森林绿
#define MINTGREEN 0xBDFCC9//薄荷
#define SIZE 30//方块三十像素
 

int score=0,level=1;
int length=10, height = 20;
int block_x, block_y;//现在形状的随机数，现在颜色的随机数，下一个形状的随机数，下一个颜色的随机数
int lose;//是否输，输了就return
int speed=1;//速度
int direction;//方块初始方向
int map[20][20] ;//0为空1为下落2为已下落
int setmap[20][20];//残局map
int qflag = 0;//q是否
int seed = -1;
int maxscore = 0;
string name;
ofstream f;
ifstream f1;
int block[28][4][4] = {
    {{1,1,1,1},{0,0,0,0},{0,0,0,0},{0,0,0,0}},
    {{1,0,0,0},{1,0,0,0},{1,0,0,0},{1,0,0,0}},
    {{1,1,1,1},{0,0,0,0},{0,0,0,0},{0,0,0,0}},
    {{1,0,0,0},{1,0,0,0},{1,0,0,0},{1,0,0,0}},
    //I
    {{1,1,1,0},{0,0,1,0},{0,0,0,0},{0,0,0,0}},
    {{1,1,0,0},{1,0,0,0},{1,0,0,0},{0,0,0,0}},
    {{1,0,0,0},{1,1,1,0},{0,0,0,0},{0,0,0,0}},
    {{0,1,0,0},{0,1,0,0},{1,1,0,0},{0,0,0,0}},
    //J
    {{1,1,1,0},{1,0,0,0},{0,0,0,0},{0,0,0,0}},
    {{1,0,0,0},{1,0,0,0},{1,1,0,0},{0,0,0,0}},
    {{0,0,1,0},{1,1,1,0},{0,0,0,0},{0,0,0,0}},
    {{1,1,0,0},{0,1,0,0},{0,1,0,0},{0,0,0,0}},
    //L
    {{1,1,0,0},{1,1,0,0},{0,0,0,0},{0,0,0,0}},
    {{1,1,0,0},{1,1,0,0},{0,0,0,0},{0,0,0,0}},
    {{1,1,0,0},{1,1,0,0},{0,0,0,0},{0,0,0,0}},
    {{1,1,0,0},{1,1,0,0},{0,0,0,0},{0,0,0,0}},
    //O
    {{1,1,1,0},{0,1,0,0},{0,0,0,0},{0,0,0,0}},
    {{1,0,0,0},{1,1,0,0},{1,0,0,0},{0,0,0,0}},
    {{0,1,0,0},{1,1,1,0},{0,0,0,0},{0,0,0,0}},
    {{0,1,0,0},{1,1,0,0},{0,1,0,0},{0,0,0,0}},
    //T
    {{1,1,0,0},{0,1,1,0},{0,0,0,0},{0,0,0,0}},
    {{0,1,0,0},{1,1,0,0},{1,0,0,0},{0,0,0,0}},
    {{1,1,0,0},{0,1,1,0},{0,0,0,0},{0,0,0,0}},
    {{0,1,0,0},{1,1,0,0},{1,0,0,0},{0,0,0,0}},
    //Z
    {{0,1,1,0},{1,1,0,0},{0,0,0,0},{0,0,0,0}},
    {{1,0,0,0},{1,1,0,0},{0,1,0,0},{0,0,0,0}},
    {{0,1,1,0},{1,1,0,0},{0,0,0,0},{0,0,0,0}},
    {{1,0,0,0},{1,1,0,0},{0,1,0,0},{0,0,0,0}},
    //S
};
typedef enum {
    MOVELEFT,
    MOVERIGHT,
    MOVEDOWN,
}move_dir;

int color[7] = {RED,YELLOW,LIGHTBLUE,FORESTGREEN,MINTGREEN,PURPLE,PINK};

int stringtoint(string str);
void initmap();
void produceblock(int x);
void shownextblock(int x);
void newblock();
bool ispossibletomove(move_dir MOVE);
void clearblock();
bool rotatable();
void rotate();
void wait(int v);
void down(int i);
void Score(int lines);
void upgrade();
void checkdelete();
void operate();
void filename();
void filename_config();
void readfile();
void inputfilename();
void inputLength();
void inputHeight();
void inputLevel();
void inputSpeed();
void inputSeed();
void setting();
void welcome();
void playtetris();

int main() {
    block_x = rand() % 7;
    block_y = rand() % 7;

    welcome();

    closegraph();

    return 0;
}



inline int stringtoint(string str) {//stoi
    if (str.length() == 1) {
        return str[0] - 48;
    }
    if (str.length() > 1) {
        if (str[0] == 45) return -1;
        return (str[0] - 48) * 10 + str[1] - 48;
    }
    return -1;
}

inline void initmap() {//画地图
    level = speed;
    score = (speed - 1) * 50;
    cleardevice();
    TCHAR number1[31];
    setlinestyle(PS_SOLID, 3);
    rectangle(22, 22, 28+SIZE*length, 28+SIZE*height);
    setlinestyle(PS_SOLID,1);
    settextstyle(30, 0, _T("微软雅黑"));
    setcolor(0);
    outtextxy(length * 30 + 100, 25, _T("NEXT:"));
    outtextxy(length * 30 + 100, 200, _T("SCORE:"));
    RECT r1 = { length * 30 + 100,230,length * 30 + 300,260 };
    swprintf_s(number1, _T("%d"), score);
    drawtext(number1, &r1, DT_LEFT);
    outtextxy(length * 30 + 100, 260, _T("LEVEL:"));
    r1 = { length * 30 + 100,290,length * 30 + 300,320 };
    swprintf_s(number1, _T("%d"), level);
    drawtext(number1, &r1, DT_LEFT);
    outtextxy(length * 30 + 100, 330, _T("GUIDE:"));
    outtextxy(length * 30 + 100, 360, _T("a->left;"));
    outtextxy(length * 30 + 100, 390, _T("d->right;"));
    outtextxy(length * 30 + 100, 420, _T("s->accelerate;"));
    outtextxy(length * 30 + 100, 450, _T("w->rotate;"));
    outtextxy(length * 30 + 100, 510, _T("q->exit without saving;"));
    outtextxy(length * 30 + 100, 540, _T("p->pause;"));
    outtextxy(length * 30 + 100, 570, _T("o->saveasendgame;"));
    outtextxy(length * 30 + 100, 640, _T("Max Score:"));
    r1 = { length * 30 + 100,670,length * 30 + 300,700 };
    swprintf_s(number1, _T("%d"), maxscore);
    drawtext(number1, &r1, DT_LEFT);
    for (int i = 0; i < length; i++) {
        for (int j = 0; j < height; j++) {
            if (map[i][j] == 2) {
                rectangle(i*30 + 25 + 1, j*30 + 25 + 1, i*30 + 55 - 1, j*30 + 55 - 1);
                setfillcolor(GREY);
                fillrectangle(i*30 + 25 + 1, j*30 + 25 + 1, i*30 + 55 - 1, j*30 + 55 - 1);
            }
        }
    }
    lose = 0;
    f.open("./config/previous.config");
    f << speed;
    f << seed;
    f.close();
    Sleep(1000);//计时,1秒钟后开始；
}

//x是种类，y是颜色
inline void produceblock(int x) {//在显示第一行加入整个block，即两行
    setfillcolor(color[x]);
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (block[4*x][j][i]) {
                fillrectangle(25 + SIZE * (i+(length-4)/2) + 1, SIZE * j + 25 + 1, 25 + SIZE * (i + (length - 2) / 2) -1, SIZE * (j + 1) + 25-1);
                map[i+(length-4)/2][j] = 1;
            }
        }
    }
}

inline void shownextblock(int x) {//拓展功能2  下一个方块展示
    setlinecolor(BLACK);
    clearrectangle(length * 30 + 100, 55, length * 30 + 220, 175);
    setfillcolor(color[x]);
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (block[4*x][j][i]) fillrectangle(100 + SIZE * (i+length), 55 + SIZE * j, 100 + SIZE * (i + 1 + length), 55 + SIZE * (j + 1));
        }
    }
}

inline void newblock() {//同时刷新
    block_x = block_y;
    block_y = rand() % 7;
    produceblock(block_x);
    shownextblock(block_y);
    Sleep(1000/speed);
}







inline bool ispossibletomove(move_dir MOVE) {
    int flag = 1;
    if (MOVE == MOVELEFT) {
        for (int i = 1; i <= length;i++) {//列
            for (int j = 1; j <= height; j++) {
                if (map[i - 1][j - 1] == 1 && (i == 1 || map[i - 2][j - 1] == 2)) flag = 0;
            }
        }
    }
    if (MOVE==MOVERIGHT) {
        for (int i = length; i >= 1; i--) {//列
            for (int j = 1; j <= height; j++) {
                if (map[i - 1][j - 1] == 1 && (i == length || map[i][j - 1] == 2)) flag = 0;
            }
        }
    }
    if (MOVE==MOVEDOWN) {
        for (int i = height; i >=1; i--) {//行
            for (int j = 1; j <= length; j++) {
                if (map[j - 1][i - 1] == 1 && (i == height || map[j-1][i] == 2)) flag = 0;
            }
        }
    }
    return flag;
}
 
inline void clearblock() {
    for (int i = 0; i < length; i++) {
        for (int j = 0; j < height; j++) {
            if (map[i][j] == 1) {
                clearrectangle(25 + SIZE * i, 25 + SIZE * j, 55 + SIZE * i, 55 + SIZE * j);                
            }
        }
    }    
}

inline bool rotatable() {
    if (!ispossibletomove(MOVEDOWN)) return 0;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < length; j++) {
            if (map[j][i] == 1) {               
                for (int k = 0; k < 4; k++) {//天才般的想法！旋转方法是以00为轴旋转，因此只需检测右下是否越界！
                    for (int l = 0; l < 4; l++) {
                        if (block[4 * block_x + (direction + 1) % 4][k][l] == 1 && (j + l >= length || map[j + l][k + i] == 2))  return 0;
                    }
                }
                return 1;
            }
        }
    }
}

inline void rotate() {    
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < length; j++) {
            if (map[j][i] == 1) {                
                for (int a = 0; a < height; a++) {//clear
                    for (int b = 0; b < length; b++) {
                        if (map[b][a] == 1){
                            map[b][a] = 0;  
                        }
                    }
                }
                for (int k = 0; k < 4; k++) {
                    for (int l = 0; l < 4; l++) {
                        if (block[4 * block_x + (direction + 1) % 4][k][l] == 1) {                            
                            map[j + l][k + i] = 1;   
                            rectangle(25 + 30 * (j + l) + 1, 25 + 30 * (k + i) + 1, 55 + 30 * (j + l) - 1, 55 + 30 * (i + k) - 1);
                            fillrectangle(25 + 30 * (j + l)+1, 25 + 30 * (k + i)+1, 55 + 30 * (j + l)-1, 55 + 30 * (i + k)-1);
                        }
                    }
                }                
                direction = (direction + 1) % 4;
                return;
            }
        }
    }
}

inline void wait(int v) {
    int count = (1000 / v) / 10;
    for (int i = 0; i < count; i++) {
        Sleep(10);
        if (_kbhit()) return;
    }
}

inline void down(int i) {
    setfillcolor(GREY);
    for (int j = i; j >0; j--) {
        for (int k = 0; k < length; k++) {
            if (map[k][j-1] == 2) {
                map[k][j] = 2;
                rectangle(25 + 30 * k+1, 25 + 30 * j+1, 55 + 30 * k-1, 55 + 30 * j-1);
                fillrectangle(25 + 30 * k+1, 25 + 30 * j+1, 55 + 30 * k-1, 55 + 30 * j-1);
            }
            else {
                map[k][j] = 0;
                clearrectangle(25 + 30 * k, 25 + 30 * j, 55 + 30 * k, 55 + 30 * j);
            }
        }
    }
    for (int k = 0; k < length; k++) {
        map[k][0] = 0;
        clearrectangle(25 + 30 * k, 25, 55 + 30 * k, 55);
    }
}

inline void Score(int lines) {
    TCHAR number1[31];
    if (lines == 1) {
        score += 10;
    }
    if(lines>1){
        score = score + 10 * lines + 10;//bonus
    }
    RECT r1 = { length*30+100,230,length * 30 + 300,260 };
    swprintf_s(number1, _T("%d"), score);
    drawtext(number1, &r1, DT_LEFT);
}

inline void upgrade() {
    level = (score / 50)+1;
    if (speed < 10) {
        speed = level;
    }
    TCHAR number2[31];
    RECT r2 = { length * 30 + 100,290,length * 30 + 300,320 };
    swprintf_s(number2, _T("%d"), level);
    drawtext(number2, &r2, DT_LEFT);
}

inline void checkdelete() {
    int lines = 0;
    int i, j;
    for (i = height-1; i >= 0; i--) {
        for (j = 0; j < length && map[j][i]; j++);
        if (j >= length){
            down(i);
            i++;//++跟--抵消，判断下一行，也就是落下来的这一行
            lines++;
        }
    }
    Score(lines);
    upgrade();
}

inline void operate() {//实现移动，判断固定，游戏失败等功能
    if (!ispossibletomove(MOVEDOWN)) {
        setcolor(RED);
        settextstyle(100, 0, _T("微软雅黑"));
        outtextxy(60, 100, _T("YOU LOSE!"));
        Sleep(1000);
        setcolor(0);
        outtextxy(60, 100, _T("Press any key to return!"));
        _getch();
        cleardevice();
        closegraph();
        lose = 1;
        f.open("record.txt");
        f << score;
        f.close();
        Sleep(1000);
        return;
    }
    int v = speed;
    while (1) {
        clearblock();
        if (_kbhit()) {
            char key = char(_getch());
            if (key == 'q') {
                lose = 1;
                return;
            }
            if (key == 'a') {
                if (ispossibletomove(MOVELEFT)) {
                    for (int i = 0; i < height; i++) {
                        for (int j = 0; j < length; j++) {
                            if (map[j][i] == 1) {
                                setfillcolor(color[block_x]);
                                rectangle(j * 30 - 5+1, 25 + i * 30+1, 25 + j * 30-1, 55 + i * 30-1);
                                fillrectangle(j * 30-5+1, 25 + i * 30+1, 25 + j * 30-1, 55 + i * 30-1);
                                map[j][i] = 0;
                                map[j - 1][i] = 1;
                            }
                        }
                    }
                }
            }
            if (key == 'd') {
                if (ispossibletomove(MOVERIGHT)) {
                    for (int i = 0; i < height; i++) {
                        for (int j = length-1; j >=0; j--) {
                            if (map[j][i] == 1) {
                                setfillcolor(color[block_x]);
                                rectangle(j * 30 + 55+1, 25 + i * 30+1, 85 + j * 30-1, 55 + i * 30-1);
                                fillrectangle(j * 30 +55+1, 25 + i * 30+1, 85 + j * 30-1, 55 + i * 30-1);
                                map[j][i] = 0;
                                map[j + 1][i] = 1;
                            }
                        }
                    }
                }
            }
            if (key == 'w') {
                if (rotatable()==1) {
                    rotate();
                }
            }
            if (key == 's') {
                v = 50;
            }
            if (key == 'p') {
                Sleep(5000);
                
            }
            if (key == 'o') {
                filename();
                f.open(name);
                f << length << endl;
                f << height << endl;
                f << level << endl;
                for (int i = 0; i < length; i++) {
                    for (int j = 0; j < height; j++) {
                        if (map[i][j] == 2) {
                            f << to_string(i) << " " << to_string(j) << endl;
                        }
                    }
                }
                f.close();
                lose = 1;
                return;
            }
        }
        if (ispossibletomove(MOVEDOWN)) {
            for (int i = height-1; i >= 0; i--) {//下落
                for (int j = 0; j < length; j++) {
                    if (map[j][i] == 1) {
                        setfillcolor(color[block_x]);
                        clearrectangle(25 + j * 30, 25 + i * 30, 55 + j * 30, 55 + i * 30);
                        rectangle(25 + j * 30+1, i * 30 + 55+1, 55 + j * 30-1, 85 + i * 30-1);
                        fillrectangle(25 + j * 30+1, i * 30 + 55+1, 55 + j * 30-1, 85 + i * 30-1);
                        map[j][i] = 0;
                        map[j][i + 1] = 1;
                    }
                }
            }
        }        
        wait(v);
        if (!ispossibletomove(MOVEDOWN)) {
            for (int i = 0; i < height; i++) {
                for (int j = 0; j < length; j++) {
                    if (map[j][i] == 1) {
                        setfillcolor(GREY);
                        rectangle(25 + j * 30 + 1, i * 30 + 25 + 1, 55 + j * 30 - 1, 55 + i * 30 - 1);
                        fillrectangle(25 + j * 30+1, 25 + i * 30+1, 55 + j * 30-1, 55 + i * 30-1);
                        map[j][i] = 2;
                    }
                }
            }
            checkdelete();
            break;
        }//fix
    }
    Sleep(1000/speed);
}
    

inline void filename() {
    name = "";
    cleardevice();
    outtextxy(25, 25, _T("输入文件名称以新建残局文件，20字符以内，限英文字母数字和下划线"));
    outtextxy(25, 55, _T("回车以结束输入,按q退出"));
    int count = 0;
    while (1) {
        if (_kbhit()) {            
            char key1 = char(_getch());
            if (key1 == 13) break;
            if (key1 == 'q') {
                qflag = 1;
                return;
            }
            if ((48 <= key1 && key1 <= 57) || (65 <= key1 && key1 <= 90) || key1 == 95 || (97 <= key1 && key1 <= 122)) {
                TCHAR number1[31];
                name = name + key1;
                swprintf_s(number1, _T("%c"), key1);
                outtextxy(25 + 15 * count, 85, number1);
                count += 1;
            }
            if (key1 == 0x8) {
                name = name.substr(0, name.size() - 1);
                count--;
                clearrectangle(25 + 15 * count, 85, 25 + 15 * count + 100, 115);
            }
            if (count == 20) break;
        }
    }
    name = "./endgames/" + name + ".end";
}

inline void filename_config() {
    name = "";
    cleardevice();
    outtextxy(25, 25, _T("输入文件名称以新建配置文件，20字符以内，限英文字母数字和下划线"));
    outtextxy(25, 55, _T("回车以结束输入,按q退出"));
    int count = 0;
    while (1) {
        if (_kbhit()) {
            char key1 = char(_getch());
            if (key1 == 13) break;
            if (key1 == 'q') {
                qflag = 1;
                return;
            }
            if ((48 <= key1 && key1 <= 57) || (65 <= key1 && key1 <= 90) || key1 == 95 || (97 <= key1 && key1 <= 122)) {
                TCHAR number1[31];
                name = name + key1;
                swprintf_s(number1, _T("%c"), key1);
                outtextxy(25 + 15 * count, 85, number1);
                count += 1;
            }
            if (key1 == 0x8) {
                name = name.substr(0, name.size() - 1);
                count--;
                clearrectangle(25 + 15 * count, 85, 25 + 15 * count + 100, 115);
            }
            if (count == 20) break;
        }
    }
    name = "./config/" + name + ".config";
}

inline void readfile() {
    string length1;
    getline(f1, length1);
    length = stringtoint(length1);
    string height1;
    getline(f1, height1);
    height = stringtoint(height1);
    string level1;
    getline(f1, level1);
    level = stringtoint(level1);
    string line;
    while (getline(f1, line)) {
        istringstream iss(line);
        string x, y;
        iss >> x >> y;
        map[stringtoint(x)][stringtoint(y)] = 2;        
    }
    f1.close();
    return;
}
inline void inputfilename() {
    name = "";
    cleardevice();
    outtextxy(25, 25, _T("file to load:"));
    outtextxy(25, 55, _T("(enter to confirm, q to quit)"));
    int count = 0;
    while (1) {
        if (_kbhit()) {
            char key1 = char(_getch());
            if (key1 == 13) break;
            if (key1 == 'q') {
                qflag = 1;
                return;
            }
            if ((48<=key1&&key1<=57) ||(65<=key1&&key1<=90) || key1 == 95 || (97<=key1&&key1<=122)) {
                TCHAR number1[31];
                name = name + key1;
                swprintf_s(number1, _T("%c"), key1);
                outtextxy(25 + 15 * count, 85, number1);
                count ++;
            }
            if (key1 == 0x8) {
                name = name.substr(0, name.size() - 1);
                count--;
                clearrectangle(25 + 15 * count, 85, 25 + 15 * count + 100, 115);
            }
            if (count == 20) break;
        }
    }
}

inline void inputLength() {
    cleardevice();
    outtextxy(25, 25, _T("输入地图宽度(8-20)："));
    string length1="";
    int count = 0;
    while (1) {
        if (_kbhit()) {
            char key2 = char(_getch());
            if (key2 == 'q') {
                qflag = 1;
                return;
            }
            if (48 <= key2 &&key2 <= 57) {
                TCHAR number1[31];
                length1 = length1 + key2;
                swprintf_s(number1, _T("%c"), key2);
                outtextxy(25+count*15, 55, number1);
                if (8 <= stringtoint(length1) &&stringtoint(length1)<= 20) {
                    length = stringtoint(length1);   
                    Sleep(100);
                    break;
                }
                if (stringtoint(length1) > 20) {
                    length1 = "";
                    clearrectangle(25, 55, 100, 85);
                }
                count ++;
            }
        }
    }
}

inline void inputHeight() {
    cleardevice();
    outtextxy(25, 25, _T("输入地图高度(8-20)："));
    string height1="";
    int count = 0;
    while (1) {
        if (_kbhit()) {
            char key2 = char(_getch());
            if (key2 == 'q') {
                qflag = 1;
                return;
            }
            if (48 <= key2 && key2 <= 57) {
                TCHAR number1[31];
                height1 = height1 + key2;
                swprintf_s(number1, _T("%c"), key2);
                outtextxy(25+count*15, 55, number1);
                if (8 <= stringtoint(height1) &&stringtoint(height1)<= 20) {
                    height = stringtoint(height1);
                    Sleep(100);
                    break;
                }
                if (stringtoint(height1) > 20) {
                    height1 = "";
                    clearrectangle(25, 55, 100, 85);
                }
                count ++;
            }
        }        
    }

}

inline void inputLevel() {
    cleardevice();
    outtextxy(25, 25, _T("输入等级(1-10)："));
    string level1="";
    int count = 0;
    while (1) {
        if (_kbhit()) {
            char key2 = char(_getch());
            if (key2 == 'q') {
                qflag = 1;
                return;
            }
            if (48 <= key2 && key2 <= 57) {
                TCHAR number1[31];
                level1 = level1 + key2;
                swprintf_s(number1, _T("%c"), key2);
                outtextxy(25+count*15, 55, number1);
                if (1 <= stringtoint(level1) &&stringtoint(level1)<= 10) {
                    level = stringtoint(level1);
                    Sleep(100);
                    break;
                }
                else{
                    level1 = "";
                    cleardevice();
                    outtextxy(25, 25, _T("重新输入："));
                    count = 0;
                    continue;
                }
                count++;
            }
        }
    }
}

inline void inputSpeed() {
    cleardevice();
    outtextxy(25, 25, _T("输入速度(1-10)："));
    string speed1 = "";
    int count = 0;
    while (1) {
        if (_kbhit()) {
            char key2 = char(_getch());
            if (key2 == 'q') {
                qflag = 1;
                return;
            }
            if (48 <= key2 && key2 <= 57) {
                TCHAR number1[31];
                speed1 = speed1 + key2;
                swprintf_s(number1, _T("%c"), key2);
                outtextxy(25 + count * 15, 55, number1);
                if (1 <= stringtoint(speed1) && stringtoint(speed1) <= 10) {
                    speed = stringtoint(speed1);
                    Sleep(100);
                    break;
                }
                else {
                    speed1 = "";
                    cleardevice();
                    outtextxy(25, 25, _T("重新输入："));
                    count = 0;
                    continue;
                }
                count++;
            }
        }
    }
}

inline void inputSeed() {
    cleardevice();
    outtextxy(25, 25, _T("输入随机种子"));
    string seed1 = "";
    int count = 0;
    while (1) {
        if (_kbhit()) {
            char key1 = char(_getch());
            if (key1 == 13) break;
            if (key1 == 'q') {
                qflag = 1;
                return;
            }
            if (48 <= key1 && key1 <= 57 ) {
                if (count == 0 && key1 == 'q') {
                    qflag = 1;
                    return;
                }
                TCHAR number1[31];
                seed1 = seed1 + key1;
                swprintf_s(number1, _T("%c"), key1);
                outtextxy(25 + 15 * count, 55, number1);
                count += 1;
            }
            if (count==0&&key1 == 45) {
                TCHAR number1[31];
                seed1 = seed1 + key1;
                swprintf_s(number1, _T("%c"), key1);
                outtextxy(25, 55, number1);
                count += 1;
                while (1) {
                    if (_kbhit()) {
                        char key2 = char(_getch());
                        seed1 = seed1 + key1;
                        swprintf_s(number1, _T("%c"), key1);
                        outtextxy(40, 55, number1);
                        if (key2 == 49) {
                            seed = -1;
                            return;
                        }
                        else {
                            seed1 = "";
                            clearrectangle(25, 55, 25 + 500, 85);
                            count = 0;
                            break;
                        }
                    }
                }
            }
            if (key1 == 0x8) {
                seed1 = seed1.substr(0, seed1.size() - 1);
                count--;
                clearrectangle(25 + 15 * count, 55, 25 + 15 * count + 100, 85);
            }
            if (count == 10) break;
        }
    }
    seed = stringtoint(seed1);
}

inline void setting() {
    int mapwidth;
    if (length <= 10) mapwidth = 700;
    else mapwidth = 400 + length * 30;
    initgraph(700, mapwidth);
    setbkcolor(WHITE);cleardevice();
    setlinestyle(PS_SOLID, 3);
    setcolor(BLACK);
    rectangle(22, 22, length*30+28, height*30+28);
    settextstyle(20, 0, _T("微软雅黑"));


    outtextxy(length*30+100, 25, _T("GUIDE:"));
    outtextxy(length * 30 + 100, 55, _T("click leftbutton to create a block"));
    outtextxy(length * 30 + 100, 85, _T("click rightbutton to delete a block"));
    outtextxy(length * 30 + 100, 115, _T("click middlebutton to end"));
    outtextxy(length * 30 + 100, 145, _T("esc to quit"));//画好界面
    setlinestyle(PS_SOLID, 1);
    ExMessage m;
    while (1) {
        m = getmessage(EX_MOUSE | EX_KEY);
        if (m.message == WM_LBUTTONDOWN) {
            if (25 <= ((m.x - 25) / 30) * 30 + 25 && ((m.x - 25) / 30) * 30 + 25<=25+30*length && 25<=((m.y - 25) / 30) * 30 + 25 && ((m.y - 25) / 30) * 30 + 25<=height*30+25) {
                rectangle((int(m.x - 25) / 30) * 30 + 25 + 1, (int(m.y - 25) / 30) * 30 + 25 + 1, (int(m.x - 25) / 30) * 30 + 55 - 1, (int(m.y - 25) / 30) * 30 + 55 - 1);
                setfillcolor(GREY);
                fillrectangle((int(m.x - 25) / 30) * 30 + 25 + 1, (int(m.y - 25) / 30) * 30 + 25 + 1, (int(m.x - 25) / 30) * 30 + 55 - 1, (int(m.y - 25) / 30) * 30 + 55 - 1);
                setmap[int(m.x - 25) / 30][int(m.y - 25) / 30] = 1;
            }
        }
        if (m.message == WM_RBUTTONDOWN) {
            clearrectangle((int(m.x - 25) / 30) * 30 + 25, (int(m.y - 25) / 30) * 30 + 25, (int(m.x - 25) / 30) * 30 + 55, (int(m.y - 25) / 30) * 30 + 55);
            setmap[int(m.x - 25) / 30][int(m.y - 25) / 30] = 0;
        }
        if (m.message == WM_MBUTTONDOWN) {
            f.open(name);
            f << length << endl;
            f << height << endl;
            f << level << endl;
            for (int i = 0; i < length; i++) {
                for (int j = 0; j < height; j++) {
                    if (setmap[i][j]==1) {
                        f << to_string(i) << " " << to_string(j) << endl;
                    }
                }
            }           
            f.close();
            break;
        }
        if (m.message == WM_KEYDOWN) if (m.vkcode == VK_ESCAPE) {
            break;
        }
    }
}



void welcome() {
    f1.open("./config/previous.config");//载入上次配置
    f1 >> speed;
    f1 >> seed;
    f1.close();
    if (seed == -1) {
        srand(time(NULL));
    }
    else srand(seed);
    f1.open("record.txt");
    f1 >> maxscore;
    f1.close();
    int mapwidth;
    if (length <= 10) mapwidth = 700;
    else mapwidth = 400 + length * 30;
    initgraph(700, mapwidth);
    for (int i = 0; i < length; i++) {
        for (int j = 0; j < height; j++) {
            map[i][j] = 0;
        }
    }
    setbkcolor(WHITE);
    cleardevice();
    settextstyle(60, 0, _T("微软雅黑"));
    setcolor(0);
    RECT r1 = { 0,200,mapwidth,260 };
    drawtext(_T("Welcome to Tetris!"), &r1, DT_CENTER);
    settextstyle(30, 0, _T("微软雅黑"));
    r1 = { 0,270,mapwidth,300 };
    drawtext(_T("Press g to start and q to quit!"), &r1, DT_CENTER);
    r1 = { 0,300,mapwidth,330 };
    drawtext(_T("Press n to set a new endgame and m to load endgame files!"), &r1, DT_CENTER);
    r1 = { 0,330,mapwidth,360 };
    drawtext(_T("Press i to create config file and u to load config files!"), &r1, DT_CENTER);
    while (1) {
        if (_kbhit()) {
            char key = char(_getch());
            if (key == 'i') {//new config
                filename_config();
                if (qflag == 1) {
                    name = "";
                    qflag = 0;
                    welcome();
                    break;
                }
                inputSpeed();
                if (qflag == 1) {
                    speed = 0;
                    qflag = 0;
                    welcome();
                    break;
                }
                inputSeed();
                if (qflag == 1) {
                    seed = -1;
                    qflag = 0;
                    welcome();
                    break;
                }
                f.open(name);
                f << speed << endl;
                f << seed << endl;
                f.close();
                name = "";
                welcome();
            }
            if (key == 'u') {//load config
                f1.open("currentfilename.txt");
                string currentname;
                f1 >> currentname;
                f1.close();
                currentname = "Current filename:" + currentname;
                name = "";
                
                while (1) {
                    cleardevice();
                    outtextxy(25, 25, _T("file to load:"));
                    outtextxy(25, 55, _T("(enter to confirm, q to quit)"));
                    for (int i = 0; i < currentname.length(); i++) {
                        TCHAR number1[31];
                        char c = currentname[i];
                        swprintf_s(number1, _T("%c"), c);
                        outtextxy(25 + 15 * i, 400, number1);
                    }
                    int count = 0;
                    while (1) {
                        if (_kbhit()) {
                            char key1 = char(_getch());
                            if (key1 == 13) break;
                            if (key1 == 'q') {
                                qflag = 1;
                                return;
                            }
                            if ((48 <= key1 && key1 <= 57) || (65 <= key1 && key1 <= 90) || key1 == 95 || (97 <= key1 && key1 <= 122)) {
                                TCHAR number1[31];
                                name = name + key1;
                                swprintf_s(number1, _T("%c"), key1);
                                outtextxy(25 + 15 * count, 85, number1);
                                count++;
                            }
                            if (key1 == 0x8) {
                                name = name.substr(0, name.size() - 1);
                                count--;
                                clearrectangle(25 + 15 * count, 85, 25 + 15 * count + 100, 115);
                            }
                            if (count == 20) break;
                        }
                    }                    
                    f1.open("./config/" + name + ".config");
                    if (qflag == 1) {
                        name = "";
                        qflag = 0;
                        welcome();
                        break;
                    }
                    if (!f1.is_open()) {
                        outtextxy(25, 85, _T("无该文件！按任意键重新输入！"));
                        _getch();
                    }
                    if (f1.is_open()) {
                        f1 >> speed;
                        f1 >> seed;
                        f1.close();
                        f.open("currentfilename.txt");
                        f << name;
                        f.close();
                        playtetris();
                        break;
                    }
                }
                
            }
            if (key == 'n') {//endgame set
                filename();
                if (qflag == 1) {
                    name = "";
                    qflag = 0;
                    welcome();
                    break;
                }
                inputLength();
                if (qflag == 1) {
                    length = 10;
                    qflag = 0;
                    welcome();
                    break;
                }
                inputHeight();
                if (qflag == 1) {
                    height = 20;
                    qflag = 0;
                    welcome();
                    break;
                }
                inputLevel();
                if (qflag == 1) {
                    level = 1;
                    qflag = 0;
                    welcome();
                    break;
                }
                for (int i = 0; i < length; i++) {
                    for (int j = 0; j < height; j++) {
                        setmap[i][j] = 0;
                    }
                }
                setting();
                welcome();
                break;
            }
            if (key == 'm') {//endgame load;
                while (1) {
                    name = "";
                    inputfilename();
                    f1.open("./endgames/" + name + ".end");
                    if (qflag == 1) {
                        name = "";
                        qflag = 0;
                        welcome();
                        break;
                    }
                    if (!f1.is_open()) {
                        outtextxy(25, 85, _T("无该文件！按任意键重新输入！"));
                        _getch();
                    }
                    if (f1.is_open()) {
                        readfile();
                        playtetris();
                        break;
                    }
                }
               
            }
            if (key == 'g') {//playtetris
                playtetris();
                break;
            }
            if (key == 'q') {
                break;
            }
        }
    }
    return;
}

inline void playtetris() {
    initmap();
    while (1) {
        direction = 0;
        newblock();
        operate();
        if (lose) {
            lose = 0;
            welcome();
            break;
        }
        Sleep(1000);
    }
}



/*
开发日志：
11.3：welcome界面；安装Visual Studio并自学easyx；
11.4：游戏界面绘制；方块数组和生成；主函数框架；方块操作（框架）；地图数组待解决；
11.9:地图数组已解决，move模块开发完成但debug未成功，rotate函数和分数、等级待解决；坐标问题待解决；
11.10:debug move完成；方块绘制完善；ispossibletomove debug;多局游戏功能debug待解决；
11.17：rotate函数完成，分数和速度完成，多局游戏debug成功；消行之后边框消失的问题待解决；//拓展功能
11.24:分工，做3.2残局挑战，重力规则待实现；
11.29:esc返回主界面，放弃重力规则因为这样会大大降低游戏难度，3.2
11.30:读取鼠标新建残局实现，文件存储未实现,美化界面（缩小方块尺寸）更改地图大小已完成，输入地图宽度出bug；
12.1:文件存储未实现，输入地图宽度debug，载入残局实现（文件存储除外），待解决启动载入上次地图，以及暂停残局录入（p）；
12.3:彻底完成残局生成和载入，完成暂停生成残局；esc to quit 未实现;
12.7:esc to quit, 读取文字bs删除未实现，游戏纪录，整合;previous完成，record完成，load config待debug；
*/