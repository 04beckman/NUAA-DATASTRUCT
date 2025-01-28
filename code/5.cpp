#include <iostream>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <queue>
using namespace std;

int n, m;                                     // n为节点数，m为边数
unordered_map<int, vector<pair<int, int>>> g; // 无向图的邻接表表示，每个节点及其相邻节点和边的权重

int main(void)
{
    ios::sync_with_stdio(false);
    cin.tie(0);

    cin >> n >> m; // 输入节点数和边数

    // 构建图的邻接表
    for (int i = 0; i < m; ++i)
    {
        int a, b, c;
        cin >> a >> b >> c;
        g[a].emplace_back(b, c);
        g[b].emplace_back(a, c);
    }

    vector<int> cost(n + 1, INT_MAX); // 存储从起点到每个节点的最小代价
    cost[1] = 0;                      // 起点到自身的代价为0

    queue<pair<int, int>> q; // BFS队列，存储当前路径上的最大边权和结点
    q.emplace(0, 1);         // 将起点加入队列，起点的最大边权为0

    // 开始BFS
    while (!q.empty())
    {
        auto temp = q.front();
        q.pop();

        // 如果当前路径的最大边权大于已知的最小代价，或者已经到达终点，则跳过
        if (temp.first > cost[temp.second] || temp.second == n)
            continue;

        // 遍历当前节点的邻居
        for (auto &x : g[temp.second])
        {
            int next = max(temp.first, x.second); // 计算最大边权
            if (cost[x.first] > next)
            {
                cost[x.first] = next;     // 更新到邻居节点的最小代价
                q.emplace(next, x.first); // 将邻居节点加入队列
            }
        }
    }

    cout << cost[n] << endl; // 输出从起点到终点的最小代价
    return 0;
}
