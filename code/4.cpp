#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <cmath>
#include <map>
using namespace std;

typedef struct huffmannode
{
    string ch;          // 字符(既可以是英文字符，也可以是汉字)
    int weight = 0;     // 权重
    int father = 0;     // 父节点
    int lc = 0, rc = 0; // 子节点
} huffmanNode;

map<string, int> weightMap;       // 权重图
vector<huffmanNode> huffmanTree;  // 哈夫曼树
map<string, string> passworldMap; // 译码表（存储每个字符对应的哈夫曼编码）

// 读取要压缩的文件,获取每个字符的权重
void getWeightMap(ifstream &fin, const char *fileName, map<string, int> &_weightMap)
{
    fin.open(fileName, ios::in);
    if (!fin.is_open())
    {
        cout << "open the file failure" << endl;
        return;
    }
    unsigned char c;
    string s;
    while (!fin.eof())
    {
        c = fin.get();
        if (fin.eof())
            break;
        s = "";
        s += c;
        _weightMap[s]++;
    }
    fin.close();
}

// 选择两个最小的权重
void selectTwo(vector<huffmannode> &_huffmanTree, int n, int &s1, int &s2)
{
    int min = 200000000;
    for (int i = 0; i < n; ++i)
    {
        if (_huffmanTree.at(i).father == 0)
        {
            if (_huffmanTree.at(i).weight < min)
            {
                min = _huffmanTree.at(i).weight;
                s1 = i;
            }
        }
    }
    min = 200000000;
    for (int i = 0; i < n; ++i)
    {
        if (_huffmanTree.at(i).father == 0)
        {
            if (_huffmanTree.at(i).weight < min && i != s1)
            {
                min = _huffmanTree.at(i).weight;
                s2 = i;
            }
        }
    }
}

// 构建哈夫曼树
void getHuffmanTree(map<string, int> &_weightMap, vector<huffmanNode> &_huffmanTree)
{
    int n = _weightMap.size();
    for (auto it : _weightMap)
    {
        huffmannode t;
        t.ch = it.first;
        t.weight = it.second;
        _huffmanTree.push_back(t);
    }
    int s1 = 0, s2 = 0;
    for (int i = n; i < 2 * n - 1; ++i)
    {
        huffmannode t;
        selectTwo(_huffmanTree, i, s1, s2);
        _huffmanTree.at(s1).father = i;
        _huffmanTree.at(s2).father = i;
        t.lc = s1;
        t.rc = s2;
        t.weight = _huffmanTree.at(s1).weight + _huffmanTree.at(s2).weight;
        _huffmanTree.push_back(t);
    }
}

// 根据哈夫曼树得到每个字符的哈夫曼编码
void getPassworld(vector<huffmannode> &_huffmanTree, map<string, string> &_passworldMap)
{
    int n = (_huffmanTree.size() + 1) / 2;
    for (int i = 0; i < n; ++i)
    {
        string passworld = "";
        int t = i;
        int fa = _huffmanTree.at(i).father;
        while (fa)
        {
            if (_huffmanTree.at(fa).lc == t)
                passworld = '0' + passworld;
            else if (_huffmanTree.at(fa).rc == t)
                passworld = '1' + passworld;
            t = fa;
            fa = _huffmanTree.at(t).father;
        }
        _passworldMap.emplace(huffmanTree.at(i).ch, passworld);
    }
}

// 将得到的所有字符的哈夫曼编码每8位转换成一个uchar
unsigned char binaryStringToChar(string binarystring)
{
    int sum = 0;
    for (int i = 0; i < binarystring.size(); i++)
    {
        if (binarystring[i] == '1')
        {
            int j = pow(2, binarystring.size() - i - 1);
            sum += j;
        }
    }
    unsigned char ch = sum;
    return ch;
}
// 输出编码
void outputhuffman(ifstream &fin, const char *fileName, ofstream &fout)
{
    fout.open(fileName, ios::out);
    for (auto &x : passworldMap)
        fout << x.first << " " << x.second << '\n';
    fout.close();
}
// 压缩文件
void compressFile(ifstream &fin, const char *fileName, ofstream &fout, const char *compressFileName)
{
    if (!weightMap.empty())
        weightMap.clear();
    if (!huffmanTree.empty())
        huffmanTree.clear();
    if (!passworldMap.empty())
        passworldMap.clear();
    getWeightMap(fin, fileName, weightMap);
    getHuffmanTree(weightMap, huffmanTree);
    getPassworld(huffmanTree, passworldMap);

    string binary = "";
    fin.open(fileName, ios::in);
    unsigned char c;
    binary = "";
    while (!fin.eof())
    {
        c = fin.get();
        if (fin.eof())
            break;
        string s = "";
        s += c;

        binary += passworldMap[s];
    }
    fin.close();
    // 不足8位的后面补0
    int add0Num = binary.size() % 8;
    if (add0Num)
        add0Num = 8 - add0Num;
    for (int i = 0; i < add0Num; ++i, binary += '0')
        ;

    fout.open(compressFileName, ios::out | ios::binary);

    // 将得到的二进制字符串每8位转为一个uchar类型写入压缩文件
    for (int i = 0; i < binary.size(); i += 8)
    {
        string k = binary.substr(i, 8);
        c = binaryStringToChar(k);
        fout << c;
    }
    fout.close();
}

// 将uchar字符转换成2进制的字符串
string ucharToBinaryString(unsigned char value)
{
    string b = "";
    for (int n = 0x80; n > 0; n >>= 1)
    {
        if (n & value)
            b += '1';
        else
            b += '0';
    }
    return b;
}

// 解压文件
void decompressFile(ifstream &fin, const char *compressFileName, ofstream &fout, const char *decompressfileName)
{
    fin.open(compressFileName, ios::in | ios::binary);

    if (!fin.is_open())
        cout << "错误";

    unsigned char c;
    int size, add0;
    map<string, int> _weightmap;
    fin >> size >> add0; // 读取辅助信息中补充0的个数
    fin.get();
    for (int i = 0; i < size; ++i) // 读取辅助信息中每个字符及其权重，得到原文件的权重图
    {
        string s = "";
        int weight = 0;
        c = fin.get();
        s += c;
        c = fin.get();
        c = fin.get();
        for (; c != ' '; c = fin.get())
        {
            weight = weight * 10 + c - '0';
        }
        _weightmap.emplace(s, weight);
    }

    // 通过权重图还原哈夫曼树
    vector<huffmannode> _huffmantree;
    getHuffmanTree(_weightmap, _huffmantree);

    string binary = "";
    while (!fin.eof()) // 将压缩文件中的uchar转换成 2进制字符串(即原文件中的哈夫曼编码)
    {
        c = fin.get();
        if (fin.eof())
            break;
        binary += ucharToBinaryString(c);
    }
    fin.close();

    for (int i = 0; i < add0; binary.pop_back(), ++i)
        ;

    // 根据哈夫曼树将压缩文件的内容解压
    fout.open(decompressfileName, ios::out);
    int n = binary.size();
    int len = _huffmantree.size();
    int fa = len - 1;
    for (int k = 0; k < n; ++k)
    {
        if (binary.at(k) == '0')
            fa = _huffmantree.at(fa).lc;
        else if (binary.at(k) == '1')
            fa = _huffmantree.at(fa).rc;

        if (_huffmantree.at(fa).lc == _huffmantree.at(fa).rc)
        {
            fout << _huffmantree.at(fa).ch;
            fa = len - 1;
        }
    }
    fout.close();
}

int main(void)
{
    ifstream fin;
    ofstream fout;
    char filename[] = "source.txt";           // 需要压缩的文件路径
    char outhuffman[] = "Huffman.txt";        // 字符编码结果
    char compressfilename[] = "code.dat";     // 压缩后的文件路径
    char decompressfilename[] = "recode.txt"; // 解压缩后的文件路径
    compressFile(fin, filename, fout, compressfilename);
    outputhuffman(fin, outhuffman, fout);
    decompressFile(fin, compressfilename, fout, decompressfilename);
    return 0;
}