#include <iostream>
#include <algorithm>
#include <fstream>
#include <windows.h>
#include <unordered_map>
#include <queue>
using namespace std;
int n, m; // n端点总数，m边数，mincost,tot记录最终答案，k已经连接了多少边
double tot, mincost;
int fat[200010]; // 并查集记录根
unordered_map<int, string> g;
unordered_map<int, vector<pair<int, double>>> graph;
struct node
{
    int from, to; // 结构体储存边
    double dis;
} edge[200010];

bool cmp(const node &a, const node &b) // 重写排序
{
    return a.dis < b.dis;
}

int find(int x) // 找根
{
    if (fat[x] == x)
        return x;
    return fat[x] = find(fat[x]);
}

void unionn(int x, int y) // 加入集，并查集的一部分
{
    fat[find(y)] = find(x);
}
void kruskal()
{
    int k = 0;
    for (int i = 1; i <= n; i++)
        fat[i] = i;                    // 最开始自己就是自己的根
    sort(edge + 1, edge + 1 + m, cmp); // 按权值排序
    for (int i = 1; i <= m; i++)       // 从小到大遍历
    {
        if (k == n - 1)
            break;                                  // n个点需要n-1条边连接
        if (find(edge[i].from) != find(edge[i].to)) // 假如不在一个团体
        {
            unionn(edge[i].from, edge[i].to); // 加入
            tot += edge[i].dis;               // 记录边权
            k++;                              // 已连接边数+1
            cout << "使用了" << g[edge[i].from] << "->" << g[edge[i].to] << "的边\n";
        }
    }
    printf("总共的权值之和为%.2lf\n", tot);
}
void prim()
{
    vector<bool> vis(n + 1, false);
    vis[1] = true;
    vector<double> dis(n + 1, INT_MAX);
    priority_queue<tuple<double, int, int>, vector<tuple<double, int, int>>, greater<>> q;
    for (auto x : graph[1])
    {
        if (!vis[x.first] && dis[x.first] > x.second)
        {
            dis[x.first] = x.second;
            q.emplace(x.second, x.first, 1);
        }
    }
    while (!q.empty())
    {
        auto [f, s, t] = q.top();
        q.pop();
        if (f > dis[s])
            continue;
        vis[s] = true;
        mincost += f;
        cout << "使用了" << g[t] << "->" << g[s] << "的边\n";
        for (auto &x : graph[s])
        {
            if (!vis[x.first] && dis[x.first] > x.second)
            {
                dis[x.first] = x.second;
                q.emplace(x.second, x.first, s);
            }
        }
    }
    printf("总共的权值之和为%.2lf\n", mincost);
}
void input()
{

    ifstream in("mintree.txt");
    if (!in.is_open())
    {
        cout << "cannot open this file!";
        return;
    }
    in >> n; // 输入点数
    for (int i = 1; i <= n; ++i)
        in >> g[i];
    in >> m;
    for (int i = 1; i <= m; i++)
    {
        in >> edge[i].from >> edge[i].to >> edge[i].dis; // 输入边的信息
        graph[edge[i].from].emplace_back(edge[i].to, edge[i].dis);
        graph[edge[i].to].emplace_back(edge[i].from, edge[i].dis);
    }
}
int main(void)
{
    input();
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_RED);
    cout << "kruskal\n";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    kruskal();
    system("pause");
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_RED);
    cout << "prim\n";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    prim();
    system("pause");
    return 0;
}