#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <time.h>
#include <ctype.h>
#include <limits.h>

#define OK 1
#define ERROR 0
#define TRUE 1
#define FALSE 0
#define OVERFLOW -1
#define m 3              // B树的阶，此设为3
#define NOT_INIT 0       // 没有初始化
#define HAVE_BEEN_INIT 1 // 已经初始化
typedef int Status;      // 用作函数值类型，表示函数结果状态

int btreeStatus = NOT_INIT; // B树状态，初始值为未初始化
typedef int KeyType;
typedef struct
{
    KeyType key;
    char data;
} Record;

typedef struct BTNode
{
    int keynum;                // 结点中关键字个数，即结点的大小
    struct BTNode *parent;     // 指向双亲结点
    KeyType key[m + 1];        // 关键字向量，0号单元未用
    struct BTNode *ptr[m + 1]; // 子树指针向量
    Record *recptr[m + 1];     // 记录指针向量，0号单元未用
} BTNode, *BTree;              // B树结点和B树的类型

typedef struct
{
    BTree pt; // 指向找到的结点
    int i;    // 1..m，在结点中的关键字序号
    int tag;  // 1:查找成功，0:查找失败
} Result;     // 在B树的查找结果类型

void SearchBTree(BTree t, KeyType key, Result &r);
int Search(BTree p, KeyType key);
void InsertBTree(BTree &t, KeyType key, BTree q, int i);
void split(BTree &q, int s, BTree &ap);
void newRoot(BTree &t, BTree p, KeyType key, BTree ap);
void Insert(BTree &q, int i, KeyType key, BTree ap);
void DeleteBTree(BTree &t, BTree &p, int i);
void Successor(BTree &p, int i);
void Remove(BTree &p, int i);
void Restore(BTree &t, BTree &p);
void BorrowFromBrother(BTree &p, BTree &leftBrother, BTree &rightBrother, BTree &parent, int &i);
void MegerWithLeftBrother(BTree &leftBrother, BTree &parent, BTree &p, BTree &t, int &i);
void MegerWithRightBrother(BTree &rightBrother, BTree &parent, BTree &p, BTree &t, int &i);
void OrderTraverseBTree(BTree t);
void PrintBTree(BTree t, int tab);
int BTreeKeyNumber(BTree t);
void DestoryBTree(BTree &t);
void InitBTree(BTree &t);
void CreateBTreeRandom(BTree &t, int keyNumber);
void InitBTreeOperation(BTree &t)
{
    if (btreeStatus == HAVE_BEEN_INIT)
    {
        printf("您有一棵B树已经创建，是否销毁它重新创建？（Y：是, 其他任意键为否）");
        char choice = getchar();
        fflush(stdin);
        if (toupper(choice) == 'Y')
        {
            DestoryBTree(t);
            btreeStatus = NOT_INIT;
        }
        else
            return;
    }
    InitBTree(t);
    btreeStatus = HAVE_BEEN_INIT;
}

void InsertKeyOperation(BTree &t)
{
    Result r;
    KeyType key;
    if (NOT_INIT == btreeStatus)
    {
        printf("B树还未初始化，请先初始化！\n");
        return;
    }
    if (NULL == t)
        printf("现在B树是空的，");
    else
    {
        printf("B树的凹入表显示如下：\n\n");
        printf("------------------------------------------------------\n");
        PrintBTree(t, 1);
        printf("\n------------------------------------------------------\n");
    }
    while (1)
    {
        printf("请输入您想插入的关键字：");
        while (1 != scanf("%d", &key))
        {
            getchar();
            fflush(stdin);
            printf("请输入正确的值！\n");
            printf("请输入您想插入的关键字：");
        }
        SearchBTree(t, key, r);
        if (1 == r.tag)
            printf("该关键字已经存在于B-树中。\n");
        else if (0 == r.tag)
        {
            InsertBTree(t, key, r.pt, r.i);
            printf("插入成功，插入后的B-树如下：\n");
            printf("------------------------------------------------------\n");
            PrintBTree(t, 1);
            printf("\n------------------------------------------------------\n");
        }
        printf("是否继续插入？（Y/y：是, 其他任意键为否）：");
        char c;
        getchar();
        scanf("%c", &c);
        fflush(stdin);
        if (toupper(c) != 'Y')
            break;
        printf("\n");
    }
}

void DeleteKeyOperation(BTree &t)
{
    Result r;
    KeyType key;
    if (NOT_INIT == btreeStatus)
    {
        printf("B树还未初始化，请先初始化！\n");
        return;
    }
    if (NULL == t)
    {
        printf("现在B树是空的，无法进行删除操作！\n");
        return;
    }
    else
    {
        printf("B树的凹入表显示如下：\n\n");
        printf("------------------------------------------------------\n");
        PrintBTree(t, 1);
        printf("\n------------------------------------------------------\n");
        while (1)
        {
            printf("请输入您想删除的关键字：");
            while (1 != scanf("%d", &key))
            {
                getchar();
                fflush(stdin);
                printf("请输入正确的值！\n");
                printf("请输入您想删除的关键字：");
            }
            SearchBTree(t, key, r);
            if (1 == r.tag)
            {
                DeleteBTree(t, r.pt, r.i);
                if (BTreeKeyNumber(t) == 0)
                {
                    t = NULL;
                    printf("删除成功B树的关键字已经全部被删除了！\n");
                    break;
                }
                else
                {
                    printf("删除成功，删除后的B-树如下：\n");
                    printf("------------------------------------------------------\n");
                    PrintBTree(t, 1);
                    printf("\n------------------------------------------------------\n");
                }
            }
            else if (0 == r.tag)
                printf("该关键字不在B树中，无法删除！\n");
            printf("是否继续删除？（Y/y：是, 其他任意键为否）：");
            char c;
            getchar();
            scanf("%c", &c);
            fflush(stdin);
            if (toupper(c) != 'Y')
                break;
            printf("\n");
        }
    }
}

void SearchOperation(BTree t)
{
    KeyType key;
    Result r;
    if (NOT_INIT == btreeStatus)
    {
        printf("B树还未初始化，请先初始化！\n");
        return;
    }
    if (NULL == t)
    {
        printf("现在B树是空的，无法进行查找操作！\n");
        return;
    }
    else
    {
        printf("B树的凹入表显示如下：\n\n");
        printf("------------------------------------------------------\n");
        PrintBTree(t, 1);
        printf("\n------------------------------------------------------\n");
        printf("请输入您要查找的关键字：");
        scanf("%d", &key);
        SearchBTree(t, key, r);
        if (0 == r.tag)
            printf("没有查到该关键字。\n");
        else
        {
            printf("以关键字所在结点的子树如下：\n");
            printf("------------------------------------------------------\n");
            PrintBTree(r.pt, 1);
            printf("\n------------------------------------------------------\n");
        }
    }
}

void CreateBTreeRandomOperation(BTree &t)
{
    if (btreeStatus == HAVE_BEEN_INIT)
    {
        printf("您有一棵B树已经创建，是否销毁它重新创建？（Y：是, 其他任意键为否）");
        char choice = getchar();
        fflush(stdin);
        if (toupper(choice) == 'Y')
        {
            DestoryBTree(t);
            btreeStatus = NOT_INIT;
        }
        else
            return;
    }
    printf("输入您想生成的B树关键字数量（只支持1-1000的输入）：");
    int keyNumber;
    while (1)
    {
        int result = scanf("%d", &keyNumber);
        getchar();
        fflush(stdin);
        if (result == 1)
        {
            if (keyNumber >= 1 && keyNumber <= 1000)
                break;
        }
        printf("请输入正确的值！\n");
        printf("请输入您想生成的B树关键字数量（只支持1-1000的输入）：");
    }
    CreateBTreeRandom(t, keyNumber);
    printf("\n生成成功，下面是B树的凹入表显示：\n");
    printf("------------------------------------------------------\n");
    PrintBTree(t, 1);
    printf("\n------------------------------------------------------\n");
    btreeStatus = HAVE_BEEN_INIT;
}

void OrderTraverseBTreeOperation(BTree t)
{
    if (NOT_INIT == btreeStatus)
        printf("B树还没初始化！请先初始化\n");
    else if (NULL == t)
        printf("B树为空\n");
    else
    {
        printf("B树的关键字数量为%d，顺序输出如下：\n", BTreeKeyNumber(t));
        OrderTraverseBTree(t);
        printf("\n");
    }
}

void PrintBTreeOperation(BTree t)
{
    if (NOT_INIT == btreeStatus)
        printf("B树还没初始化！请先初始化\n");
    else if (NULL == t)
        printf("B树为空\n");
    else
    {
        printf("B树的凹入表显示如下：\n\n");
        printf("------------------------------------------------------\n");
        PrintBTree(t, 1);
        printf("\n------------------------------------------------------\n");
    }
}

void DestoryBTreeOperation(BTree &t)
{
    if (btreeStatus == NOT_INIT)
        printf("B树还没创建，不用销毁\n");
    else if (t == NULL)
        printf("B树为空，不用销毁\n");
    else
    {
        DestoryBTree(t);
        btreeStatus = NOT_INIT;
        printf("销毁成功\n");
    }
}

/**
 * B-树功能演示选择菜单
 */
void BTreeMenu()
{
    printf("**************************************************************************\n");
    printf("*                                                                        *\n");
    printf("***************************B-树功能选择菜单*******************************\n");
    printf("*                                                                        *\n");
    printf("*                         1. 创建一棵空B-树                              *\n");
    printf("*                         2. 随机生成一棵B-树                            *\n");
    printf("*                         3. 插入关键字                                  *\n");
    printf("*                         4. 删除关键字                                  *\n");
    printf("*                         5. 查找关键字                                  *\n");
    printf("*                         6. 关键字数量                                  *\n");
    printf("*                         7. 升序输出B-树关键字                          *\n");
    printf("*                         8. 凹入表打印B-树                              *\n");
    printf("*                         9. 销毁B-树                                    *\n");
    printf("*                         0. 退出                                        *\n");
    printf("*                                                                        *\n");
    printf("**************************************************************************\n");
}

int main()
{
    BTree t;
    int selectNum;
    while (1)
    {
        BTreeMenu();
        printf("请输入您的选择：");
        scanf("%d", &selectNum);
        getchar();
        fflush(stdin);
        if (0 == selectNum)
        {
            printf("\n是否退出？（Y：是, 其他任意键为否）");
            char option;
            option = getchar();
            fflush(stdin);
            if (toupper(option) == 'Y')
            {
                if (btreeStatus == HAVE_BEEN_INIT)
                    DestoryBTree(t);
                exit(EXIT_SUCCESS);
            }
        }
        else
        {
            switch (selectNum)
            {
            case 1:
                InitBTreeOperation(t);
                break;
            case 2:
                CreateBTreeRandomOperation(t);
                break;
            case 3:
                InsertKeyOperation(t);
                break;
            case 4:
                DeleteKeyOperation(t);
                break;
            case 5:
                SearchOperation(t);
                break;
            case 6:
            {
                if (NOT_INIT == btreeStatus)
                    printf("B树还没初始化！请先初始化\n");
                else
                    printf("\n关键字数量为: %d\n\n", BTreeKeyNumber(t));
                break;
            }
            case 7:
                OrderTraverseBTreeOperation(t);
                break;
            case 8:
                PrintBTreeOperation(t);
                break;
            case 9:
                DestoryBTreeOperation(t);
                break;
            default:
                printf("您输入的选择不正确，请重新输入！\n");
                break;
            }
        }
        system("PAUSE");
        system("cls"); // 清屏操作
    }
    system("PAUSE");
    getch();
    return 0;
}
void SearchBTree(BTree t, KeyType key, Result &r)
{
    int i = 0;
    int found = 0;
    BTree p = t;    // 一开始指向根结点，之后指向待查结点
    BTree q = NULL; // 指向待查结点的双亲
    while (p != NULL && found == 0)
    {
        i = Search(p, key);
        if (i <= p->keynum && p->key[i] == key)
            found = 1;
        else
        {
            q = p;
            p = p->ptr[i - 1]; // 指针下移
        }
    }
    if (1 == found)
    { // 查找成功，返回key的位置p和i
        r.pt = p;
        r.i = i;
        r.tag = 1;
    }
    else
    { // 查找失败，返回key的插入位置q和i
        r.pt = q;
        r.i = i;
        r.tag = 0;
    }
}

int Search(BTree p, KeyType key)
{
    int i = 1;
    while (i <= p->keynum && key > p->key[i])
        i++;

    return i;
}

void InsertBTree(BTree &t, KeyType key, BTree q, int i)
{
    KeyType x;
    int s;
    int finished = FALSE;
    int needNewRoot = FALSE;
    BTree ap;
    if (NULL == q)
        newRoot(t, NULL, key, NULL);
    else
    {
        x = key;
        ap = NULL;
        while (FALSE == needNewRoot && FALSE == finished)
        {
            Insert(q, i, x, ap); // x和ap分别插入到q->key[i]和q->ptr[i]
            if (q->keynum < m)
                finished = TRUE;
            else
            {
                // 分裂q结点
                s = (m + 1) / 2; // 得到中间结点位置
                split(q, s, ap);
                x = q->key[s];
                // 在双亲位置插入关键字x
                if (q->parent != NULL)
                {
                    q = q->parent;
                    i = Search(q, x); // 寻找插入的位置
                }
                else
                    needNewRoot = TRUE;
            }
        }
        if (TRUE == needNewRoot)
            newRoot(t, q, x, ap);
    }
}

void split(BTree &q, int s, BTree &ap)
{
    int i, j;
    int n = q->keynum; // 关键字数量
    ap = (BTree)malloc(sizeof(BTNode));
    ap->ptr[0] = q->ptr[s];
    for (i = s + 1, j = 1; i <= n; i++, j++)
    {
        ap->key[j] = q->key[i];
        ap->ptr[j] = q->ptr[i];
    }
    ap->keynum = n - s;
    ap->parent = q->parent;
    for (i = 0; i <= n - s; i++)
    {
        // 修改新结点的子结点的parent域
        if (ap->ptr[i] != NULL)
            ap->ptr[i]->parent = ap;
    }
    q->keynum = s - 1; // 修改q结点的关键字数量
}

void newRoot(BTree &t, BTree p, KeyType key, BTree ap)
{
    t = (BTree)malloc(sizeof(BTNode));
    t->keynum = 1;
    t->ptr[0] = p;
    t->ptr[1] = ap;
    t->key[1] = key;
    if (p != NULL)
        p->parent = t;
    if (ap != NULL)
        ap->parent = t;
    t->parent = NULL;
}

void Insert(BTree &q, int i, KeyType key, BTree ap)
{
    int j;
    int n = q->keynum;
    for (j = n; j >= i; j--)
    {
        // 后移
        q->key[j + 1] = q->key[j];
        q->ptr[j + 1] = q->ptr[j];
    }
    q->key[i] = key;
    q->ptr[i] = ap;
    if (ap != NULL)
        ap->parent = q;
    q->keynum++;
}

void DeleteBTree(BTree &t, BTree &p, int i)
{
    if (p->ptr[i] != NULL)
    {
        // 不是最下层非终端结点
        Successor(p, i);      // 找到后继最下层非终端结点的最小关键字代替它
        DeleteBTree(t, p, 1); // 删除最下层非终端结点中的最小关键字
    }
    else
    {
        Remove(p, i); // 从结点p中删除key[i]
        if (p->keynum < (m - 1) / 2)
            Restore(t, p); // 调整B树
    }
}

void Successor(BTree &p, int i)
{
    BTree leaf = p;
    if (NULL == p)
        return;
    leaf = leaf->ptr[i]; // 指向子树
    while (NULL != leaf->ptr[0])
    {
        // 找到最下层非终端结点
        leaf = leaf->ptr[0];
    }
    p->key[i] = leaf->key[1];
    p = leaf;
}
void Remove(BTree &p, int i)
{
    int k;
    // 指针与key都向左移
    for (k = i; k < p->keynum; k++)
    {
        p->key[k] = p->key[k + 1];
        p->ptr[k] = p->ptr[k + 1];
    }
    p->keynum--;
}

void Restore(BTree &t, BTree &p)
{
    BTree parent, leftBrother, rightBrother; // 被删结点的父结点、左右兄弟
    parent = p->parent;
    if (parent != NULL)
    { // 父结点不为空
        // 寻找左右兄弟
        int i;
        for (i = 0; i <= parent->keynum; i++)
        {
            if (parent->ptr[i] == p)
                break;
        }
        if (i > 0)
            leftBrother = parent->ptr[i - 1];
        else
            leftBrother = NULL;
        if (i < parent->keynum)
            rightBrother = parent->ptr[i + 1];
        else
            rightBrother = NULL;

        // 左兄弟或右兄弟有富余关键字
        if ((leftBrother != NULL && leftBrother->keynum >= (m + 1) / 2) ||
            (rightBrother != NULL && rightBrother->keynum >= (m + 1) / 2))
            BorrowFromBrother(p, leftBrother, rightBrother, parent, i);
        else
        {
            // 左右兄弟都没富余关键字，需要合并
            if (leftBrother != NULL)
                MegerWithLeftBrother(leftBrother, parent, p, t, i); // 与左兄弟合并
            else if (rightBrother != NULL)
                MegerWithRightBrother(rightBrother, parent, p, t, i);
            else
            {
                // 当左右子树不存在时改变根结点
                for (int j = 0; j <= p->keynum + 1; j++)
                {
                    if (p->ptr[j] != NULL)
                    {
                        t = p->ptr[j];
                        break;
                    }
                }
                t->parent = NULL;
            }
        }
    }
    else
    {
        // 根节点，去掉根节点，使树减一层
        BTree a;
        for (int j = 0; j <= p->keynum + 1; j++)
        {
            if (p->ptr[j] != NULL)
            {
                a = p;
                p = p->ptr[j];
                a->ptr[j] = NULL;
                free(a);
                break;
            }
        }
        t = p;
        t->parent = NULL;
    }
}
void BorrowFromBrother(BTree &p, BTree &leftBrother, BTree &rightBrother, BTree &parent, int &i)
{
    // 左兄弟有富余关键字，向左兄弟借
    if (leftBrother != NULL && leftBrother->keynum >= (m + 1) / 2)
    {
        for (int j = p->keynum + 1; j > 0; j--)
        {
            // 关键字与指针后移，腾出第一个位置
            if (j > 1)
                p->key[j] = p->key[j - 1];
            p->ptr[j] = p->ptr[j - 1];
        }
        p->ptr[0] = leftBrother->ptr[leftBrother->keynum];
        if (p->ptr[0] != NULL)
            p->ptr[0]->parent = p;
        leftBrother->ptr[leftBrother->keynum] = NULL;
        p->key[1] = parent->key[i];                             // 被删结点存父结点关键字
        parent->key[i] = leftBrother->key[leftBrother->keynum]; // 父结点的key变为被删结点左兄弟的最大关键字
        leftBrother->keynum--;
        p->keynum++;
    }
    else if (rightBrother != NULL && rightBrother->keynum >= (m + 1) / 2)
    { // 右兄弟有富余关键字
        p->key[p->keynum + 1] = parent->key[i + 1];
        p->ptr[p->keynum + 1] = rightBrother->ptr[0]; // 子树指针指向右兄弟最左边的子树指针
        if (p->ptr[p->keynum + 1] != NULL)
            p->ptr[p->keynum + 1]->parent = p;
        p->keynum++;
        parent->key[i + 1] = rightBrother->key[1]; // 父结点从右兄弟借关键字
        for (int j = 0; j < rightBrother->keynum; j++)
        {
            if (j > 0)
                rightBrother->key[j] = rightBrother->key[j + 1];
            rightBrother->ptr[j] = rightBrother->ptr[j + 1];
        }
        rightBrother->ptr[rightBrother->keynum] = NULL;
        rightBrother->keynum--;
    }
}
void MegerWithLeftBrother(BTree &leftBrother, BTree &parent, BTree &p, BTree &t, int &i)
{
    // 与左兄弟合并
    leftBrother->key[leftBrother->keynum + 1] = parent->key[i]; // 从父结点拿下分割本节点与左兄弟的关键字
    leftBrother->ptr[leftBrother->keynum + 1] = p->ptr[0];
    if (leftBrother->ptr[leftBrother->keynum + 1] != NULL)
        leftBrother->ptr[leftBrother->keynum + 1]->parent = leftBrother; // 给左兄弟的结点，当此结点存在时需要把其父亲指向指向左结点
    leftBrother->keynum++;                                               // 左兄弟关键数加1
    for (int j = 1; j <= p->keynum; j++)
    {
        // 把本结点的关键字和子树指针赋给左兄弟
        leftBrother->key[leftBrother->keynum + j] = p->key[j];
        leftBrother->ptr[leftBrother->keynum + j] = p->ptr[j];
        if (leftBrother->ptr[leftBrother->keynum + j] != NULL)
            leftBrother->ptr[leftBrother->keynum + j]->parent = leftBrother;
    }
    leftBrother->keynum += p->keynum;
    parent->ptr[i] = NULL;
    free(p); // 释放p结点
    for (int j = i; j < parent->keynum; j++)
    {
        // 左移
        parent->key[j] = parent->key[j + 1];
        parent->ptr[j] = parent->ptr[j + 1];
    }
    parent->ptr[parent->keynum] = NULL;
    parent->keynum--; // 父结点关键字个数减1
    if (t == parent)
    {
        // 如果此时父结点为根，则当父结点没有关键字时才调整
        if (0 == parent->keynum)
        {
            for (int j = 0; j <= parent->keynum + 1; j++)
            {
                if (parent->ptr[j] != NULL)
                {
                    t = parent->ptr[j];
                    break;
                }
                t->parent = NULL;
            }
        }
    }
    else
    {
        // 如果父结点不为根，则需要判断是否需要重新调整
        if (parent->keynum < (m - 1) / 2)
        {
            Restore(t, parent);
        }
    }
}
void MegerWithRightBrother(BTree &rightBrother, BTree &parent, BTree &p, BTree &t, int &i)
{
    // 与右兄弟合并
    for (int j = (rightBrother->keynum); j > 0; j--)
    {
        if (j > 0)
            rightBrother->key[j + 1 + p->keynum] = rightBrother->key[j];
        rightBrother->ptr[j + 1 + p->keynum] = rightBrother->ptr[j];
    }
    rightBrother->key[p->keynum + 1] = parent->key[i + 1]; // 把父结点的分割两个本兄弟和右兄弟的关键字拿下来使用
    for (int j = 0; j <= p->keynum; j++)
    {
        // 把本结点的关键字及子树指针移动右兄弟中去
        if (j > 0)
            rightBrother->key[j] = p->key[j];
        rightBrother->ptr[j] = p->ptr[j];
        if (rightBrother->ptr[j] != NULL)
            rightBrother->ptr[j]->parent = rightBrother; // 给右兄弟的结点需要把其父结点指向右兄弟
    }
    rightBrother->keynum += (p->keynum + 1);
    parent->ptr[i] = NULL;
    free(p); // 释放p结点
    for (int j = i; j < parent->keynum; j++)
    {
        if (j > i)
            parent->key[j] = parent->key[j + 1];
        parent->ptr[j] = parent->ptr[j + 1];
    }
    if (1 == parent->keynum)
    {
        // 如果父结点在关键字减少之前只有一个结点，那么需要把父结点的右孩子赋值给左孩子
        parent->ptr[0] = parent->ptr[1];
    }
    parent->ptr[parent->keynum] = NULL;
    parent->keynum--; // 父结点关键字数减1
    if (t == parent)
    {
        // 如果此时父结点为根，则当父结点没有关键字时才调整
        if (0 == parent->keynum)
        {
            for (int j = 0; j <= parent->keynum + 1; j++)
            {
                if (parent->ptr[j] != NULL)
                {
                    t = parent->ptr[j];
                    break;
                }
            }
            t->parent = NULL;
        }
    }
    else
    {
        // 如果父结点不为根，则需要判断是否需要重新调整
        if (parent->keynum < (m - 1) / 2)
            Restore(t, parent);
    }
}
void OrderTraverseBTree(BTree t)
{
    int i;
    if (NULL == t)
        return;
    for (i = 0; i <= t->keynum; i++)
    {
        if (i > 0)
            printf("%d  ", t->key[i]);
        if (t->ptr[i] != NULL)
            OrderTraverseBTree(t->ptr[i]);
    }
}
void PrintBTree(BTree t, int tab)
{
    if (NULL == t)
        return;
    int i;
    for (i = 1; i <= tab; i++)
        printf("    ");
    for (i = 1; i <= t->keynum; i++)
    {
        printf("%d", t->key[i]);
        if (i != t->keynum)
            printf(", ");
    }
    printf("\n");
    for (i = 0; i <= t->keynum; i++)
        PrintBTree(t->ptr[i], tab + 1);
}
int BTreeKeyNumber(BTree t)
{
    int i;
    int keyNumber = 0;
    if (NULL == t)
        return 0;
    keyNumber += t->keynum;
    for (i = 0; i <= t->keynum; i++)
    {
        if (t->ptr[i] != NULL)
            keyNumber += BTreeKeyNumber(t->ptr[i]);
    }
    return keyNumber;
}
void DestoryBTree(BTree &t)
{
    if (NULL == t)
        return;
    for (int i = 0; i <= t->keynum; i++)
    {
        if (t->ptr[i] != NULL)
            DestoryBTree(t->ptr[i]);
    }
    free(t);
}
void InitBTree(BTree &t)
{
    t = NULL;
}
void CreateBTreeRandom(BTree &t, int keyNumber)
{
    KeyType key;
    Result r;
    srand((unsigned int)time(NULL));
    t = NULL;
    for (int i = 0; i < keyNumber; i++)
    {
        key = rand() % 1000;
        SearchBTree(t, key, r);
        if (r.tag == 0)
            InsertBTree(t, key, r.pt, r.i);
    }
}