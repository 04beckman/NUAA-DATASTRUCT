#include <vector>
#include <iostream>
#include <algorithm>
#include <unordered_set>
#include <unordered_map>
using namespace std;
#define ll long long
int w[2010];                                 // 存储节点权重的数组
int p[2010];                                 // 存储父节点的数组
vector<ll> sum(2010);                        // 存储每个节点子树权重之和的向量
unordered_map<int, unordered_set<int>> allc; // 存储每个节点的所有后代及本身
unordered_map<int, vector<int>> g;           // 存储有向图
unordered_map<int, vector<int>> rg;          // 存储反向有向图

// 深度优先搜索+剪枝，计算以节点为根的权重之和
ll dfs(int x)
{
    if (sum[x] != 0)
        return sum[x];
    ll temp = w[x];
    for (auto &y : g[x])
        temp += dfs(y);
    return sum[x] = temp;
}

// 深度优先搜索+剪枝，找到每个节点的所有祖先
void dfs1(int x)
{
    if (allc[x].size() != 0)
        return;
    unordered_set<int> temp;
    temp.insert(x);
    for (auto &y : g[x])
    {
        dfs1(y);
        for (auto &i : allc[y])
            temp.insert(i);
    }
    allc[x] = temp;
}

// 更新移除节点后的权重和祖先集合
void ddfs(int x, ll temp, vector<ll> &tsum, unordered_map<int, unordered_set<int>> &tallc, int target)
{
    if (x != target)
    {
        tsum[x] -= temp;
        for (auto &y : tallc[target])
            tallc[x].erase(y);
    }
    for (auto &y : rg[x])
        ddfs(y, temp, tsum, tallc, target);
}

int main(void)
{
    ios::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);
    int n, m;
    cin >> n >> m;
    // 输入每个节点的权重
    for (int i = 1; i <= n; ++i)
        cin >> w[i];
    // 构建有向图和反向有向图
    for (int i = 2; i <= n; ++i)
    {
        cin >> p[i];
        g[p[i]].push_back(i);
        rg[i].push_back(p[i]);
    }
    // 计算权重之和并找到每个节点的所有祖先
    for (int i = 1; i <= n; ++i)
    {
        dfs(i);
        dfs1(i);
    }
    // 处理每个查询
    for (int i = 0; i < m; ++i)
    {
        int target;
        cin >> target;
        unordered_set<int> s;
        // 使用所有节点初始化集合
        for (int j = 1; j <= n; ++j)
            s.insert(j);
        // 创建权重和祖先的副本，在查询期间进行操作
        vector<ll> tsum = sum;
        unordered_map<int, unordered_set<int>> tallc = allc;
        // 执行查询，直到集合中只剩一个节点
        while (s.size() != 1)
        {
            // 计算移除每个节点的代价，并选择代价最小的节点
            vector<pair<ll, int>> cost;
            ll ssum = 0;
            for (auto &x : s)
                ssum += w[x];
            for (auto &x : s)
                cost.emplace_back(abs(ssum - 2 * tsum[x]), x);
            sort(cost.begin(), cost.end());
            auto &to = cost[0].second;
            cout << to << " ";

            // 检查目标是否在所选节点的祖先中
            if (tallc[to].find(target) != tallc[to].end())
                s = tallc[to];
            else
            {
                // 如果不在，更新集合和祖先，移除所选节点
                ddfs(to, tsum[to], tsum, tallc, to);
                for (auto &x : tallc[to])
                    s.erase(x);
            }
        }
        cout << endl;
    }
    return 0;
}

/*
http://118.190.20.162/view.page?gpid=T178
*/