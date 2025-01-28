#include <iostream>
#include <string>
#include <cstring>
#include <vector>
using namespace std;
const int N = 110;

int n, m;

struct Url
{
    string path, name;
} url[N];

// 确定是否是数字，不是的话返回空字符串
string get_number(string &str)
{
    string res;
    for (auto c : str)
    {
        if (c >= '0' && c <= '9')
            res += c;
        else
        {
            res.clear();
            return res;
        }
    }
    // 去掉前导零
    int k = 0;
    // k +1 < str.size()保证可以正常输出0
    while (k + 1 < str.size() && str[k] == '0')
        k++;
    return res.substr(k);
}

// 待匹配的ulr和规则是否匹配，不匹配返回空
vector<string> get(string &path, string &str)
{
    vector<string> res(1);
    // 双指针找到//之间的一段 存在a和b中
    int i, j;
    for (i = 1, j = 1; i < path.size() && j < str.size();)
    {
        int u = i + 1, v = j + 1;
        while (u < path.size() && path[u] != '/')
            u++;
        while (v < str.size() && str[v] != '/')
            v++;
        string a = path.substr(i, u - i), b = str.substr(j, v - j);

        if (a == "<str>")
        {
            res.push_back(b);
            i = u + 1, j = v + 1;
        }
        else if (a == "<int>")
        {
            // 判断b是否为数字
            auto t = get_number(b);
            if (t.empty())
            {
                res.clear();
                return res;
            }
            res.push_back(t);
            i = u + 1, j = v + 1;
        }
        else if (a == "<path>")
        {
            res.push_back(str.substr(j));
            return res;
        }
        else if (a != b)
        {
            res.clear();
            return res;
        }
        else
            i = u + 1, j = v + 1;
    }
    // 判断最后是否有/，
    if (i - path.size() != j - str.size())
        res.clear();
    return res;
}

void solve(string str)
{
    for (int i = 0; i < n; i++)
    {
        // 是否匹配
        auto res = get(url[i].path, str);
        if (res.size())
        {
            cout << url[i].name;
            for (int j = 1; j < res.size(); j++)
            {
                cout << " " << res[j];
            }
            cout << endl;
            return;
        }
    }
    puts("404");
}

int main(void)
{
    cin >> n >> m;
    for (int i = 0; i < n; i++)
        cin >> url[i].path >> url[i].name;
    while (m--)
    {
        string str;
        cin >> str;
        solve(str);
    }
    return 0;
}