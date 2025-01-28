#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <iomanip>
#include <queue>
using namespace std;

// 定义AVL树节点
struct Node
{
    int key;
    int value;
    int height;
    Node *left;
    Node *right;

    // 构造函数
    Node(int k, int v) : key(k), value(v), height(1), left(nullptr), right(nullptr) {}
};

// 获取节点的高度
int getHeight(Node *node)
{
    if (node == nullptr)
        return 0;
    return node->height;
}

// 获取节点的平衡因子
int getBalanceFactor(Node *node)
{
    if (node == nullptr)
        return 0;
    return getHeight(node->left) - getHeight(node->right);
}

// 更新节点的高度
void updateHeight(Node *node)
{
    node->height = max(getHeight(node->left), getHeight(node->right)) + 1;
}

// 左旋
Node *leftRotate(Node *node)
{
    Node *temp = node->right;
    node->right = temp->left;
    temp->left = node;

    updateHeight(node);
    updateHeight(temp);

    return temp;
}

// 右旋
Node *rightRotate(Node *node)
{
    Node *temp = node->left;
    node->left = temp->right;
    temp->right = node;

    updateHeight(node);
    updateHeight(temp);

    return temp;
}
Node *findmax(Node *node)
{
    if (node == nullptr || node->left == nullptr)
        return node;
    return findmax(node->right);
}

// 在AVL树中插入新节点
Node *insert(Node *node, int key, int value)
{
    if (node == nullptr)
        return new Node(key, value);

    if (key < node->key)
        node->left = insert(node->left, key, value);
    else if (key > node->key)
        node->right = insert(node->right, key, value);
    else
    {
        // 如果key已经存在，则更新节点的值
        node->value = value;
        return node;
    }

    // 更新节点的高度
    updateHeight(node);

    // 计算节点的平衡因子
    int balanceFactor = getBalanceFactor(node);

    // 如果节点的平衡因子大于1，则需要进行旋转操作
    if (balanceFactor > 1)
    {
        // 如果新插入的节点在节点的左子树的左侧，则进行右旋
        if (key < node->left->key)
            return rightRotate(node);
        // 如果新插入的节点在节点的左子树的右侧，则先进行左旋再进行右旋
        if (key > node->left->key)
        {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }
    }
    // 如果节点的平衡因子小于-1，则需要进行旋转操作
    if (balanceFactor < -1)
    {
        // 如果新插入的节点在节点的右子树的右侧，则进行左旋
        if (key > node->right->key)
            return leftRotate(node);
        // 如果新插入的节点在节点的右子树的左侧，则先进行右旋再进行左旋
        if (key < node->right->key)
        {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }
    }

    return node;
}

// 在AVL树中查找节点
Node *find(Node *node, int key)
{
    if (node == nullptr)
        return nullptr;

    if (key < node->key)
        return find(node->left, key);
    else if (key > node->key)
        return find(node->right, key);
    else
        return node;
}
// 在AVL树中删除节点
Node *remove(Node *node, int key)
{
    if (node == nullptr)
        return nullptr;

    if (key < node->key)
        node->left = remove(node->left, key);
    else if (key > node->key)
        node->right = remove(node->right, key);
    else
    {
        if (node->left == nullptr && node->right == nullptr)
        {
            delete node;
            return nullptr;
        }
        else if (node->left == nullptr)
        {
            Node *temp = node->right;
            delete node;
            return temp;
        }

        else if (node->right == nullptr)
        {
            Node *temp = node->left;
            delete node;
            return temp;
        }
        else
        {
            // 如果节点既有左子树又有右子树，则找到右子树中的最小节点来替换当前节点
            Node *successor = findmax(node->left);
            node->key = successor->key;
            node->value = successor->value;
            node->left = remove(node->left, successor->key);
        }
    }
    // 更新节点的高度
    updateHeight(node);
    // 计算节点的平衡因子
    int balanceFactor = getBalanceFactor(node);
    // 如果节点的平衡因子大于1，则需要进行旋转操作
    if (balanceFactor > 1)
    {
        // 如果节点的左子树的平衡因子大于0，则进行右旋
        if (getBalanceFactor(node->left) > 0)
            return rightRotate(node);
        // 如果节点的左子树的平衡因子小于0，则先进行左旋再进行右旋
        if (getBalanceFactor(node->left) < 0)
        {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }
    }
    // 如果节点的平衡因子小于-1，则需要进行旋转操作
    if (balanceFactor < -1)
    {
        // 如果节点的右子树的平衡因子小于0，则进行左旋
        if (getBalanceFactor(node->right) < 0)
            return leftRotate(node);
        // 如果节点的右子树的平衡因子大于0，则先进行右旋再进行左旋
        if (getBalanceFactor(node->right) > 0)
        {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }
    }
    return node;
}
// 打印二叉树
void printTree(Node *node)
{
    if (node == nullptr)
        return;
    queue<Node *> q;
    q.push(node);
    Node *temp;
    cout << "key lchild rchild" << endl;
    while (!q.empty())
    {
        temp = q.front();
        q.pop();

        cout << std::left << setw(4) << temp->key;
        if (temp->left)
        {
            q.push(temp->left);
            cout << std::left << setw(7) << temp->left->key;
        }
        else
            cout << std::left << setw(7) << " ";
        if (temp->right)
        {
            q.push(temp->right);
            cout << std::left << setw(7) << temp->right->key << endl;
        }
        else
            cout << endl;
    }
    cout << endl;
}
// 存文件
void printTreeFile(Node *root, ofstream &fout)
{
    if (root == nullptr)
        return;
    fout << root->key << " " << root->value << endl;
    printTreeFile(root->left, fout);
    printTreeFile(root->right, fout);
}
int main()
{
    // 从文件中读取数据
    ifstream fin("data.txt");
    if (!fin)
    {
        cerr << "打开文件data.txt失败." << endl;
        return 1;
    }

    // 建立AVL树
    Node *root = nullptr;
    int key, value;
    while (fin >> key >> value)
    {
        root = insert(root, key, value);
    }
    while (1)
    {
        // 菜单
        int command;
        cout << "****************" << endl;
        cout << "** 1.查找节点 **" << endl;
        cout << "** 2.插入节点 **" << endl;
        cout << "** 3.删除节点 **" << endl;
        cout << "** 4.输出AVL树**" << endl;
        cout << "** 5.  退出   **" << endl;
        cout << "****************" << endl;
        cin >> command;
        if (command == 1)
        {
            // 查找节点
            int searchKey;
            cout << "请输入一个整数关键字Key进行查找节点: ";
            cin >> searchKey;
            Node *node = find(root, searchKey);
            if (node != nullptr)
                cout << "Key: " << node->key << ", Value: " << node->value << endl;
            else
                cout << "关键字未找到." << endl;
            system("pause");
        }
        else if (command == 2)
        {
            // 插入节点
            int insertKey, insertValue;
            cout << "请输入一个整数关键字Key和一个整数Value进行插入节点: ";
            cin >> insertKey >> insertValue;
            root = insert(root, insertKey, insertValue);
            cout << "插入成功." << endl;
            system("pause");
        }
        else if (command == 3)
        {
            // 删除节点
            int removeKey;
            cout << "请输入一个整数关键字Key进行删除节点: ";
            cin >> removeKey;
            root = remove(root, removeKey);
            cout << "删除成功，接下来将输出整棵树观察结果" << endl;
            // 以可验证的方式输出结果
            cout << "AVL Tree: " << endl;
            printTree(root);
            system("pause");
        }
        else if (command == 4)
        {
            // 以可验证的方式输出结果
            cout << "AVL Tree: " << endl;
            printTree(root);
            system("pause");
        }
        else if (command == 5)
        {
            // 修改后结果存放至data.txt，以覆盖的形式
            ofstream fout("data.txt");
            if (!fout)
            {
                cerr << "打开文件data.txt失败." << endl;
                return 1;
            }
            printTreeFile(root, fout);
            break;
        }
        else
        {
            cout << "输入指令无效，请重新输入" << endl;
            system("pause");
        }
        // 清屏
        system("cls");
    }
    system("pause");
    return 0;
}