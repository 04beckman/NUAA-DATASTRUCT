#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <algorithm>
#include <stack>
#include <sstream>
#include <array>
#include <functional>
using namespace std;

const int mo = 1e9 + 7;
#define CONST -1
#define VAR -2
#define OP -3
vector<int> l;    // 左子树节点索引
vector<int> r;    // 右子树节点索引
vector<int> info; // 节点信息，存储操作符、常量值或变量索引
vector<int> kind; // 节点类型，操作符、常量或变量
stack<int> id;    // 节点索引栈

int main(void)
{
    ios::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);

    string s;
    int n, m;
    cin >> n >> m;
    getline(cin, s); // 读取换行符 '\n'
    getline(cin, s); // 读取表达式字符串
    istringstream qwq(s);

    int node_id = 0;

    // 解析表达式字符串，构建表达式树
    while (getline(qwq, s, ' '))
    {
        if (s.size() == 1 && (s[0] == '+' || s[0] == '*' || s[0] == '-'))
        {
            int rson = id.top();
            id.pop();
            int lson = id.top();
            id.pop();
            l.push_back(lson);
            r.push_back(rson);
            info.push_back(s[0]);
            kind.push_back(OP);

            id.push(node_id);
            ++node_id;
        }
        else if (s[0] == 'x')
        {
            int x = stoi(s.substr(1));
            --x;
            l.push_back(-1);
            r.push_back(-1);
            info.push_back(x);
            kind.push_back(VAR);

            id.push(node_id);
            ++node_id;
        }
        else
        {
            int x = stoi(s);
            l.push_back(-1);
            r.push_back(-1);
            info.push_back(x);
            kind.push_back(CONST);

            id.push(node_id);
            ++node_id;
        }
    }

    int root = id.top(); // 树的根节点索引
    vector<int> a(n);

    // 定义递归函数，解决表达式树的值和导数
    function<array<int, 2>(int, int)> solve = [&](int u, int x)
    {
        // 如果当前节点是变量节点
        if (kind[u] == VAR)
        {
            // 返回包含值和导数的数组，值为变量 x 的值，导数为1（如果当前节点为 x），否则为0
            return array<int, 2>{a[info[u]], (info[u] == x)};
        }
        // 如果当前节点是常量节点
        else if (kind[u] == CONST)
        {
            // 返回包含值和导数的数组，值为常量值，导数为0
            return array<int, 2>{info[u], 0};
        }
        // 如果当前节点是操作符节点
        else
        {
            // 递归求解左右子树的值和导数
            auto lans = solve(l[u], x), rans = solve(r[u], x);
            int sum = 0, dsum = 0;

            // 根据操作符类型计算当前节点的值和导数
            if (info[u] == '+')
            {
                sum = lans[0] + rans[0];
                dsum = lans[1] + rans[1];
                if (sum >= mo)
                    sum -= mo;
                if (dsum >= mo)
                    dsum -= mo;
            }
            else if (info[u] == '-')
            {
                sum = lans[0] - rans[0];
                dsum = lans[1] - rans[1];
                if (sum >= mo)
                    sum -= mo;
                if (dsum >= mo)
                    dsum -= mo;
            }
            else
            {
                sum = 1ll * lans[0] * rans[0] % mo;
                dsum = (1ll * lans[0] * rans[1] % mo + 1ll * lans[1] * rans[0] % mo);
                if (dsum >= mo)
                    dsum -= mo;
            }

            // 处理结果值和导数小于0的情况
            if (sum < 0)
                sum += mo;
            if (dsum < 0)
                dsum += mo;

            // 返回包含值和导数的数组
            return array<int, 2>{sum, dsum};
        }
    };

    // 处理每个查询
    for (int i = 0; i < m; ++i)
    {
        int x;
        cin >> x;
        --x;
        for (auto &i : a)
            cin >> i;
        cout << solve(root, x)[1] << '\n'; // 输出导数
    }

    return 0;
}
/*
http://118.190.20.162/view.page?gpid=T173
*/