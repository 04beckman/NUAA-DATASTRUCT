#include <iostream>
#include <string>
#include <string.h>
#include <fstream>
#include <windows.h>
#include <vector>
#define SIZE 6000
#define NEWSIZE 1024
#define Infinity 100000000 // 表示无限大

using namespace std;
#define white SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE) // 白色
#define red SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_RED);                      // 红色
#define green SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);                                   // 绿色
typedef struct SiteArcNode                                                                                                                         // 边的结点（站点）结构类型
{
    int adjvex;                  // 该边的站点编号
    int path;                    // 站点所在的路线
    struct SiteArcNode *nextarc; // 指向下一条边的指针
} SiteArcNode;
typedef struct SiteVexNode // 站点结构
{
    int num;               // 站点编号（从0开始）
    char name[50];         // 站点名称
    SiteArcNode *firstarc; // 指向第一条与该站点有关的边的指针
} SiteVexNode;
typedef struct SiteGraph // 站点的邻接表结构类型
{
    SiteVexNode *SVNode; // 定义邻接表
    int vexnum;          // 站点数
    int size;            // 邻接表的大小
} SiteGraph;

// 这里的最短距离可以指经过站点最少的距离，也可以指转车次数最少的距离
int *Route;                                // 从起始站点到达某站点会经过的路线
int *Adjvex;                               // 从起始站点到达某站点会经过的站点编号
int *Lowcost;                              // 从起始站点到某站点的最短距离
int *Flag;                                 // 标注站点是否已被选中（初始化为0）
void Create(SiteGraph &G1, SiteGraph &G2); // 创建两张图（G1：连接同一路线上的站点；G2：连接相邻站点）
void Dijkstra(SiteGraph G1, int v);        // 迪杰斯特拉算法求单源最短路线
void Find_Route(SiteGraph G1);             // 对任意两站点，给出转车次数最少的乘车路线
void Find_Site(SiteGraph G2);              // 对任意两站点，给出经过站点最少的乘车路线
void menu();
// 主函数
int main()
{
    SiteGraph G1, G2;
    Create(G1, G2);
    char ch = 'y';
    while (ch == 'y')
    {
        system("cls");
        menu();
        int op;
        cin >> op;
        Adjvex = (int *)malloc((G1.vexnum + 10) * sizeof(int));
        Route = (int *)malloc((G1.vexnum + 10) * sizeof(int));
        Lowcost = (int *)malloc((G1.vexnum + 10) * sizeof(int));
        Flag = (int *)malloc((G1.vexnum + 10) * sizeof(int));
        if (op == 1)
        {
            cout << "（1）输入任意两站点，给出转车次数最少的乘车路线" << endl;
            Find_Route(G1);
        }
        else if (op == 2)
        {
            cout << "（2）输入任意两站点，给出经过站点最少的乘车路线" << endl;
            Find_Site(G2);
        }
        else
            break;
        red;
        cout << "是否进行查询y/n\n";
        cin >> ch;
        white;
    }
    return 0;
}

void menu()
{
    green;
    cout << "**************************************" << endl;
    cout << "**    欢迎使用公交查询系统          **" << endl;
    cout << "**    [1] 查询最少换乘路线          **" << endl;
    cout << "**    [2] 查询经过站点路线          **" << endl;
    cout << "**    [3]     退出                  **" << endl;
    cout << "**************************************" << endl;
    white;
}
// 创建图
void Create(SiteGraph &G1, SiteGraph &G2)
{
    fstream file;
    string s;
    G1.SVNode = (SiteVexNode *)malloc(SIZE * sizeof(SiteVexNode));
    G2.SVNode = (SiteVexNode *)malloc(SIZE * sizeof(SiteVexNode));
    G1.size = SIZE;
    G2.size = SIZE;
    G1.vexnum = 0;
    G2.vexnum = 0;
    file.open("南京公交线路.txt", ios::in);
    if (file.fail())
    {
        cout << "文件打开失败" << endl;
        exit(0);
    }
    int f = 1;
    int x = 0;
    getline(file, s);
    while (f)
    {
        if (file.eof())
        {
            f = 0;
        }
        int t = 1, a = 0, k = 0;
        vector<int> M; // 保存这条线路上的所有站点编号
        char site[100];
        for (int i = 0; i <= s.size(); i++)
        {
            if (s[i] == ' ')
            {
                t = 0;
            }
            else if (t && (s[i] >= 48 && s[i] <= 57))
            {
                a = a * 10 + s[i] - 48;
            }
            else if (!t)
            {
                // 开始处理站点数据
                if (s[i] == ',' || i == s.size())
                {
                    // 遇到“,”说明一个站点已输入完毕，建立该站点的结点
                    site[k] = '\0';
                    k = 0;
                    int t1 = 1;
                    int n; // 当前站点的编号
                    for (int j = 0; j < G2.vexnum; j++)
                    {
                        // 先查看该站点是否已建立头结点
                        if (strcmp(G2.SVNode[j].name, site) == 0)
                        {
                            t1 = 0; // 该站点已建立头结点，t1标注为0
                            n = j;  // n=当前站点的编号
                            break;
                        }
                    }
                    if (t1)
                    {
                        // 该站点未建立头结点
                        if (G1.size <= G1.vexnum)
                        {
                            // 根据点的个数动态分配空间
                            G1.SVNode = (SiteVexNode *)realloc(G1.SVNode, (G1.size + NEWSIZE) * sizeof(SiteVexNode));
                            G1.size += NEWSIZE;
                        }
                        if (G2.size <= G2.vexnum)
                        {
                            // 根据点的个数动态分配空间
                            G2.SVNode = (SiteVexNode *)realloc(G2.SVNode, (G2.size + NEWSIZE) * sizeof(SiteVexNode));
                            G2.size += NEWSIZE;
                        }
                        strcpy_s(G1.SVNode[G1.vexnum].name, site); // 头结点名称
                        strcpy_s(G2.SVNode[G2.vexnum].name, site); // 头结点名称
                        G1.SVNode[G1.vexnum].num = G1.vexnum;      // 头结点编号
                        G2.SVNode[G2.vexnum].num = G2.vexnum;      // 头结点编号
                        G1.SVNode[G1.vexnum].firstarc = NULL;
                        G2.SVNode[G2.vexnum].firstarc = NULL;
                        n = G2.vexnum; // n=当前站点的编号
                        G1.vexnum++;
                        G2.vexnum++;
                    }
                    if (!M.empty())
                    {
                        // 如果不是这条线路上的第一个站点
                        SiteArcNode *p, *q;
                        // 不是第一个站点，G1表要与这条线路上的所有站点建立连接
                        for (int j = 0; j < M.size(); j++)
                        {
                            // 当前站点加入到前面所有站点的链表中
                            p = (SiteArcNode *)malloc(sizeof(SiteArcNode)); // 创建一个用于存放当前边的结点p
                            p->nextarc = NULL;
                            p->adjvex = n;
                            p->path = a; // 保存这条边所在的线路
                            q = G1.SVNode[M[j]].firstarc;
                            // 将边按顺序插入到链表末尾
                            if (q == NULL)
                            {
                                G1.SVNode[M[j]].firstarc = p;
                            }
                            else
                            {
                                while (q->nextarc != NULL)
                                {
                                    q = q->nextarc;
                                }
                                q->nextarc = p;
                            }
                            // 前面所有站点加入到当前站点的链表中
                            p = (SiteArcNode *)malloc(sizeof(SiteArcNode)); // 创建一个用于存放当前边的结点p
                            p->nextarc = NULL;
                            p->adjvex = M[j];
                            p->path = a;               // 保存这条边所在的线路
                            q = G1.SVNode[n].firstarc; // 前一个站点加入到当前站点的链表中
                            // 将边按顺序插入到链表末尾
                            if (q == NULL)
                            {
                                G1.SVNode[n].firstarc = p;
                            }
                            else
                            {
                                while (q->nextarc != NULL)
                                {
                                    q = q->nextarc;
                                }
                                q->nextarc = p;
                            }
                        }
                        // 不是第一个站点，G2表要与前后站点建立连接
                        int m = M[M.size() - 1];
                        p = (SiteArcNode *)malloc(sizeof(SiteArcNode)); // 创建一个用于存放当前边的结点p
                        p->nextarc = NULL;
                        p->adjvex = n;
                        p->path = a;
                        q = G2.SVNode[m].firstarc; // 当前站点加入到前一个站点的链表中
                        // 将边按顺序插入到链表末尾
                        if (q == NULL)
                        {
                            G2.SVNode[m].firstarc = p;
                        }
                        else
                        {
                            while (q->nextarc != NULL)
                            {
                                q = q->nextarc;
                            }
                            q->nextarc = p;
                        }
                        p = (SiteArcNode *)malloc(sizeof(SiteArcNode)); // 创建一个用于存放当前边的结点p
                        p->nextarc = NULL;
                        p->adjvex = m;
                        p->path = a;
                        q = G2.SVNode[n].firstarc; // 前一个站点加入到当前站点的链表中
                        // 将边按顺序插入到链表末尾
                        if (q == NULL)
                        {
                            G2.SVNode[n].firstarc = p;
                        }
                        else
                        {
                            while (q->nextarc != NULL)
                            {
                                q = q->nextarc;
                            }
                            q->nextarc = p;
                        }
                    }
                    M.push_back(n);
                }
                else
                {
                    site[k] = s[i];
                    k++;
                }
            }
        }
        if (f)
        {
            getline(file, s);
        }
    }
    file.close();
}

// 迪杰斯特拉算法求单源最短路线
void Dijkstra(SiteGraph G, int v)
{
    Lowcost[v] = 0; // 初始站点到初始站点的距离为0
    Flag[v] = 1;    // 标注初始点
    int num = 1;    // 记录目前被选中的站点数目
    SiteArcNode *p;
    while (num < G.vexnum)
    {
        p = G.SVNode[v].firstarc; // p为新选中的点的第一个邻接点
        while (p != NULL)
        {
            // 由于新点加入，更新起始点与其余未被选中的顶点之间的距离
            if (!Flag[p->adjvex] && Lowcost[p->adjvex] > Lowcost[v] + 1)
            {
                Lowcost[p->adjvex] = Lowcost[v] + 1;
                Adjvex[p->adjvex] = v;
                Route[p->adjvex] = p->path; // 保存该站点所在的路线
            }
            p = p->nextarc;
        }
        int min = Infinity;
        for (int i = 1; i <= G.vexnum; i++)
        {
            // 从未选择的站点中找下一个与起始站点距离最近的站点
            if (!Flag[i] && Lowcost[i] < min)
            {
                min = Lowcost[i];
                v = i; // 更新v为目前与起始站点距离最近的站点
            }
        }
        Flag[v] = 1; // 标记新选中的站点
        num++;       // 目前被选中的站点数目+1
    }
}

// 对任意两站点，给出转车次数最少的乘车路线
void Find_Route(SiteGraph G)
{
    string s1, s2;
    int v1 = -1, v2 = -1;
    cout << "请输入第一个站点：";
    cin >> s1;
    cout << "请输入第二个站点：";
    cin >> s2;
    for (int i = 0; i < G.vexnum; i++)
    {
        if (G.SVNode[i].name == s1)
        {
            v1 = i;
        }
        if (G.SVNode[i].name == s2)
        {
            v2 = i;
        }
    }
    if (v1 == -1 || v2 == -1)
    {
        red;
        cout << "站点不存在，返回主菜单" << endl;
        white;
        return;
    }
    if (s1 == s2)
    {
        red;
        cout << "两站一致，不必换乘" << endl;
        white;
        return;
    }
    for (int i = 1; i <= G.vexnum; i++)
    {
        // 初始化
        Adjvex[i] = v1;
        Route[i] = 0;
        Lowcost[i] = Infinity;
        Flag[i] = 0;
    }
    Dijkstra(G, v1); // 求从站点v1出发到站点v2的换乘次数最少的乘车路线
    cout << endl;
    cout << s1 << " 到 " << s2 << " 的换乘次数最少为" << Lowcost[v2] - 1 << "  "; // 换乘次数要减1
    // 输出起始点到站点v2的最少转车路径
    cout << "路径为：" << endl;
    int j = v2;
    vector<int> Path;
    while (j != v1)
    {
        Path.push_back(j);
        j = Adjvex[j];
    }
    int k = Route[Path[Path.size() - 1]];
    cout << k << "路：" << s1;
    for (int i = Path.size() - 1; i >= 0; i--)
    {
        if (Route[Path[i]] != k)
        {
            k = Route[Path[i]];
            cout << "\n换乘到" << k << "路：" << G.SVNode[Path[i + 1]].name;
        }
        cout << "->" << G.SVNode[Path[i]].name;
    }
    cout << endl;
}

// 对任意两站点，给出经过站点最少的乘车路线
void Find_Site(SiteGraph G)
{
    string s1, s2;
    int v1, v2;
    cout << "请输入第一个站点：";
    cin >> s1;
    cout << "请输入第二个站点：";
    cin >> s2;
    for (int i = 0; i < G.vexnum; i++)
    {
        if (G.SVNode[i].name == s1)
        {
            v1 = i;
        }
        if (G.SVNode[i].name == s2)
        {
            v2 = i;
        }
    }
    if (v1 == -1 || v2 == -1)
    {
        red;
        cout << "站点不存在，返回主菜单" << endl;
        white;
        return;
    }
    if (s1 == s2)
    {
        red;
        cout << "两站一致，不必乘车" << endl;
        white;
        return;
    }
    for (int i = 1; i <= G.vexnum; i++)
    {
        // 初始化
        Adjvex[i] = v1;
        Route[i] = 0;
        Lowcost[i] = Infinity;
        Flag[i] = 0;
    }
    Dijkstra(G, v1); // 求从站点v1出发到站点v2的经过站点最少的乘车路线
    // 输出起始站点到站点v2的最短距离
    cout << endl;
    cout << s1 << " 到 " << s2 << " 的经过站点次数最少为" << Lowcost[v2] + 1 << "  ";
    // 输出起始点到站点v2的最少转车路径
    cout << "路径为：" << endl;
    int j = v2;
    vector<int> Path;
    while (j != v1)
    {
        Path.push_back(j);
        j = Adjvex[j];
    }
    int k = Route[Path[Path.size() - 1]];
    cout << k << "路：" << s1;
    for (int i = Path.size() - 1; i >= 0; i--)
    {
        if (Route[Path[i]] != k)
        {
            k = Route[Path[i]];
            cout << "\n换乘到" << k << "路：" << G.SVNode[Path[i + 1]].name;
        }
        cout << "->" << G.SVNode[Path[i]].name;
    }
    cout << endl;
}