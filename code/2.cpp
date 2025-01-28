#include <iostream>
#include <fstream>
#include <iomanip>
#include <stack>
#include <Windows.h>
using namespace std;
#define white SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE) // 白色
#define MAZEMAXSIZE 100
// 1为墙壁，0为通路

struct point
{
    int x;
    int y;
};
// 迷宫
typedef struct maze
{
    char Map[MAZEMAXSIZE][MAZEMAXSIZE];
} maze, *mazepoint;

bool check(int x, int mx)
{
    if (x < 0)
    {
        cout << "输入非法（超出边界）" << endl;
        return false;
    }
    else if (x >= mx)
    {
        cout << "输入非法（超出边界）" << endl;
        return false;
    }
    else
        return true;
}

// 创建迷宫
void Create(mazepoint p, point &start, point &end, int &x, int &y)
{
    ifstream in;
    in.open("迷宫样例.txt");
    if (!in)
    {
        cout << "打开迷宫.txt 失败" << endl;
        exit(1);
    }
    // 输入迷宫
    in >> x >> y;
    for (int i = 0; i < x; i++)
    {
        for (int j = 0; j < y; j++)
        {
            in >> p->Map[i][j];
        }
    }
    in.close();
    // 输出迷宫
    cout << "原迷宫为:\n(用1表示墙壁)    (用0表示通路)" << endl;
    cout << "   ";
    for (int j = 0; j < y; j++)
        cout << setw(3) << setiosflags(ios::left) << j;
    cout << endl;
    for (int i = 0; i < x; i++)
    {
        cout << setw(3) << setiosflags(ios::left) << i;
        for (int j = 0; j < y; j++)
        {
            if (p->Map[i][j] == '1')
            {
                // 墙壁
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
                cout << p->Map[i][j] << "  ";
                white;
            }
            else if (p->Map[i][j] == '0')
            {
                // 通路
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_RED);
                cout << p->Map[i][j] << "  ";
                white;
            }
            if (j == y - 1)
                cout << endl;
        }
    }
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_RED);
    cout << endl;
    cout << "!!!  可输入的横坐标范围为 0-" << x - 1 << endl;
    cout << "!!!  可输入的纵坐标范围为 0-" << y - 1 << endl;
    white;
    while (1)
    {
        cout << "请输入迷宫的入口坐标:";

        cin >> start.x >> start.y;
        if (check(start.x, x) && check(start.y, y))
        {
            if (p->Map[start.x][start.y] == '1')
            {
                cout << "输入的入口坐标非法，请重新输入\n"
                     << endl;
                continue;
            }
            else
                break;
        }
        else
        {
            cout << "输入入口非法，请重新输入\n"
                 << endl;
            continue;
        }
    }
    while (1)
    {
        cout << "请输入迷宫的出口坐标:";
        cin >> end.x >> end.y;
        if (check(end.x, x) && check(end.y, y))
        {
            if (p->Map[start.x][start.y] == '1')
            {
                cout << "输入的出口坐标为墙壁，请重新输入\n"
                     << endl;
                continue;
            }
            else
                break;
        }
        else
        {
            cout << "输入出口非法，请重新输入\n"
                 << endl;
            continue;
        }
    }
}

// 走迷宫
void grids(mazepoint p, point &start, point &end, int &boardX, int &boardY)
{
    stack<point> Stack;
    Stack.push(start);
    // cur表示当前位置
    point cur = start, q;
    int cur_x = start.x;
    int cur_y = start.y;
    // 是否走过标志
    bool flag = false;
    // 依次下左上右表示四个方向的移动，i表示几个方向被尝试过；
    int moveing[4][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
    int i = 0;
    while (!Stack.empty() && ((cur.x != end.x) || (cur.y != end.y)))
    {
        cur = Stack.top();
        flag = 0;
        i = 0;
        // 从当前点开始尝试四个方向
        while (flag == 0 && i < 4)
        {
            cur_x = cur.x + moveing[i][0];
            cur_y = cur.y + moveing[i][1];
            if (p->Map[cur_x][cur_y] == '0')
            {
                cur.x = cur_x;
                cur.y = cur_y;
                Stack.push(cur);
                p->Map[cur_x][cur_y] = '*';
                flag = 1; // 该点已经走过
            }
            i++;
        }
        if (i == 4 && flag == 0)
        {
            p->Map[cur.x][cur.y] = '#'; // 那么将当前点置为墙壁
            Stack.pop();
        }
    }
    p->Map[start.x][start.y] = '*';
    p->Map[end.x][end.y] = '*';
    if (Stack.empty())
    {
        cout << "没有路径" << endl;
    }
    else
    {
        // 输出迷宫路径图
        cout << "迷宫答案路径(用*表示路径)" << endl;
        cout << "   ";
        for (int j = 0; j < boardY; j++)
            cout << setw(3) << setiosflags(ios::left) << j;
        cout << endl;
        for (int i = 0; i < boardX; i++)
        {
            cout << setw(3) << setiosflags(ios::left) << i;
            for (int j = 0; j < boardY; j++)
            {
                if (p->Map[i][j] == '1')
                {
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
                    cout << p->Map[i][j] << "  ";
                    white;
                }
                else if (p->Map[i][j] == '0')
                {
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_RED);
                    cout << p->Map[i][j] << "  ";
                    white;
                }
                else if (p->Map[i][j] == '*')
                {
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
                    cout << p->Map[i][j] << "  ";
                    white;
                }
                else if (p->Map[i][j] == '#')
                {
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_RED);
                    cout << 0 << "  ";
                    white;
                }
                if (j == boardY - 1)
                    cout << endl;
            }
        }
        cout << endl;
        // 输出迷宫路径表
        stack<point> temp;
        while (!Stack.empty())
        {
            q = Stack.top();
            Stack.pop();
            temp.push(q);
        }
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
        cout << "路径如下：" << endl;
        white;
        int huanhang = 0;
        while (!temp.empty())
        {
            q = temp.top();
            temp.pop();
            huanhang++;
            cout << "(" << q.x << "," << q.y << ")";
            if (!temp.empty())
                cout << "->";
            if (huanhang % 10 == 0)
                cout << endl;
        }
    }
}

int main(void)
{
    maze map;
    mazepoint p = &map;
    point start, end;
    int x, y;
    Create(p, start, end, x, y);
    grids(p, start, end, x, y);
    system("pause");
    return 0;
}