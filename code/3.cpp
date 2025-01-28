#include <iostream>
#include <cstdlib>
#include <cstring>
#include <windows.h>
#include <time.h>
#define len 30
// 日期-年月日
struct Date
{
    int year;
    int month;
    int day;
};
struct Info
{
    int children_count; // 孩子数-hzshu
    int alive;
    int alive_day;            // 活了多少日-ri
    int marry;                // 婚否-jiehun
    char name[20];            // 姓名
    char birth_place[20];     // 出生地点 -birthplace[20]
    Date birth_date;          // 结构date定义的出生日期
    Date death_date;          // 结构date定义的死亡日期
    int sex;                  // 性别
    char wife_or_husband[20]; // 配偶
    char address[50];         // 家庭住址
    char resume[100];         // 简历
    int height;               // 高度
    char occupation[20];      // 职业
    char education[20];       // 受教育程度
    char parent_name[20];     // 父亲姓名，用于添加节点时用
    int Depth;                // 二叉树深度，输出二叉树时用
};
typedef struct CSNode
{
    Info data;                                   // 个人信息类型结构
    CSNode *first_child, *next_brother, *parent; // csnode的第一个孩子节点，下一个兄弟节点，双亲节点
} *person;
typedef person QElemType;
typedef struct QNode
{
    QElemType data;
    struct QNode *next;
} QNode, *QueuePtr;
typedef struct
{
    QueuePtr front, rear;
} LinkQueue;
// 函数声明
void start_up(person &T);                  // 给结点置空
void main_menu(person &);                  // 主菜单
int Create_familytree(person &, person &); // 创建家谱树
person find_name(person &, char Name[]);   // 按姓名查找指定成员函数
int birth_date(person &);                  // 输入出生日期
int death_date(person &);                  // 输入死者死亡日期
int judge_birth_date(person &);            // 判断出生日期是否在今天之前
int judge_death_date(person &);            // 判断死亡日期死亡日期是否正确
void level(person &);                      // 计算每位家族成员的辈分
void level_every(person &);                // 计算一位家族成员的辈分
void sort(person &);                       // 排序
void sort_every(person &);                 // 为一个父亲的孩子按出生日期排序
void add_new_member(person &);             // 添加成员函数
void preserve(person &);                   // 保存成员信息函数
void preserve_member(person &, FILE *);    // 保存一个成员信息函数
void preserve_family(person &, FILE *);    // 保存所有成员信息函数
int DeQueue(LinkQueue &, QElemType &);     // 出队列操作
void InitQueue(LinkQueue &);               // 构造一个空队列
int QueueEmpty(LinkQueue);                 // 判断链式队列是否为空
void EnQueue(LinkQueue &, QElemType);      // 进队列操作
void show(person &);                       // 显示家谱树函数
void read_node(person &, FILE *);          // 从文件中读取所有成员节点
void read_node_every(person &, FILE *);    // 从文件中读取一个成员节点
void show_tree_member(person &);           // 每一个具体展现
void show_tree(person &);                  // 把家谱成员姓名通过一个层次化的家谱树显示出来
void lookup(person &);                     // 通过名字查找相应的成员，并显示其信息
void show_member(person &);                // 显示一个成员所有信息的函数
void Delete(person &);                     // 采用后续遍历的方法删除成员节点
void find(person &);                       // 通过关键字搜索家谱成员
int kmp(char str[], char ptrn[]);          // KMP算法，用于搜索简历内容
void relation(person &);
void survival_day_sum(int &, int &, int &, int &, int &, int &); // 计算生存天数
void start_up(person &T)                                         // 初始化，把指针置为空
{
    T->parent = NULL;
    T->first_child = NULL;
    T->next_brother = NULL;
    return;
}
void main_menu(person &T) // 主菜单程序
{
    int j = 1;
    char i;
    while (j)
    {
        system("cls"); // 清屏
        HANDLE consolehwnd;
        consolehwnd = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(consolehwnd, FOREGROUND_INTENSITY | FOREGROUND_GREEN); // 设置屏幕字体颜色
        printf("\t");
        SetConsoleTextAttribute(consolehwnd, FOREGROUND_INTENSITY | FOREGROUND_BLUE);
        printf("\n\t\t---------------------------------------------\n\t");
        SetConsoleTextAttribute(consolehwnd, FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE);
        printf("\t\t   0. 退出系统              \n\t");
        printf("\t\t   1. 新创建家谱树          \t\n\t");
        printf("\t\t   2. 添加家谱成员          \t\n\t");
        printf("\t\t   3. 保存家谱信息          \t\n\t");
        printf("\t\t   4. 显示家谱    \t\n\t");
        printf("\t\t   5. 按姓名查找/修改家谱成员    \t\n\t");
        printf("\t\t   6. 按关键字查找家谱成员  \t\n\t");
        printf("\t\t   7. 通过名字确定两人关系  \t\n\t");
        SetConsoleTextAttribute(consolehwnd, FOREGROUND_INTENSITY | FOREGROUND_BLUE);
        printf("\t---------------------------------------------\n\t");
        printf("\t请输入你的操作(0~8):\n");
        i = getchar();
        fflush(stdin); // 刷新缓冲区,将缓冲区内的数据清空并丢弃
        switch (i)
        {
        case '0':
            j = 0;
            break;
        case '1':
            system("cls"); // 清屏
            if (strcmp(T->data.name, "0") == 0)
            {
                printf("下面请输入祖先的信息\n");
                person b;
                b = (person)malloc(sizeof(CSNode));
                start_up(b);
                if (Create_familytree(b, T))
                {
                    T = b;
                    level(T);
                    sort(T);
                    printf("\n\t        *****  家谱树已建立完成 ***** \n\t");
                }
            }
            else
                printf("家谱树已存在，要新建家谱树请先删除原有家谱树！\n");
            fflush(stdin);
            printf("\n即将返回主菜单！\n");
            system("pause"); // 程序运行到此处会暂停，等待键盘命令
            break;
        case '2':
            system("cls"); // 清屏
            add_new_member(T);
            fflush(stdin);
            printf("\n即将返回主菜单！\n");
            system("pause");
            break;
        case '3':
            system("cls"); // 清屏
            if (strcmp(T->data.name, "0"))
                preserve(T);
            else
                printf("家谱树为空，无法保存！\n");
            fflush(stdin);
            printf("\n即将返回主菜单！\n");
            system("pause");
            break;
        case '4':
            system("cls"); // 清屏
            show(T);
            fflush(stdin);
            printf("\n即将返回主菜单！\n");
            system("pause");
            break;
        case '5':
            system("cls"); // 清屏
            lookup(T);
            fflush(stdin);
            printf("\n即将返回主菜单！\n");
            system("pause");
            break;
        case '6':
            system("cls"); // 清屏
            find(T);
            fflush(stdin);
            printf("\n即将返回主菜单！\n");
            system("pause");
            break;
        case '7':
            system("cls"); // 清屏
            relation(T);
            fflush(stdin);
            printf("\n即将返回主菜单！\n");
            system("pause");
            break;
        default:
            printf("你的操作有误，请重新选择!\n");
            fflush(stdin);
            system("pause");
        }
    }
    return;
}
int Create_familytree(person &F, person &T) // 创建家谱树函数，即为空家谱添加一个祖先
{
    int i, j = 1;
    person a;
    a = (person)malloc(sizeof(CSNode));
    start_up(a);
    printf("请输入此人名字:\n");
    scanf("%s", F->data.name);
    fflush(stdin);
    a = find_name(T, F->data.name);
    if (a)
    {
        printf("此人已存在该家谱中！\n");
        return 0;
    }
    printf("请输入此人性别(男输入1，女输入0):\n");
    while (j)
    {
        scanf("%d", &(F->data.sex));
        fflush(stdin);
        switch (F->data.sex)
        {
        case 1:
            j = 0;
            break;
        case 0:
            j = 0;
            break;
        default:
            printf("输入数据有误,请重新输入!\n");
            fflush(stdin);
            break;
        }
    }
    printf("请输入此人身高(单位:cm):\n");
    scanf("%d", &(F->data.height));
    fflush(stdin);
    printf("请输入此人居住地址:\n");
    scanf("%s", F->data.address);
    fflush(stdin);
    printf("请输入此人父亲的名字(无父亲请输入0):\n你有3次机会\n");
    i = 3;
    while (i)
    {
        scanf("%s", F->data.parent_name);
        fflush(stdin);
        if (strcmp(F->data.parent_name, "0") == 0)
            break;
        else
        {
            a = find_name(T, F->data.parent_name);
            if (a == 0 || a->data.sex == 0)
            {
                i--;
                printf("父亲姓名输入错误\n你还有%d次机会\n", i);
            }
            else
            {
                printf("父亲姓名输入正确\n");
                F->parent = a;
                if (a->first_child == NULL)
                    a->first_child = F;
                else
                {
                    a = a->first_child; // 有儿子就使他变成儿子的兄弟
                    while (a->next_brother)
                        a = a->next_brother;
                    a->next_brother = F;
                }
                break;
            }
        }
    }
    if (!i)
    {
        fflush(stdin);
        return 0;
    }
    printf("请说明此人是否已婚(已婚输入1，未婚输入0)\n");
    j = 1;
    while (j)
    {
        scanf("%d", &(F->data.marry));
        fflush(stdin);
        switch (F->data.marry)
        {
        case 1:
            j = 0;
            printf("请输入此人配偶的名字:\n");
            scanf("%s", F->data.wife_or_husband);
            fflush(stdin);
            break;
        case 0:
            j = 0;
            strcpy(F->data.wife_or_husband, "0");
            break;
        default:
            printf("输入数据有误,请重新输入!\n");
            fflush(stdin);
            break;
        }
    }
    printf("请输入此人出生地址:\n");
    scanf("%s", F->data.birth_place);
    fflush(stdin);
    printf("请输入此人学历:\n");
    scanf("%s", F->data.education);
    fflush(stdin);
    printf("请输入此人的职业:\n");
    scanf("%s", F->data.occupation);
    fflush(stdin);
    printf("请输入此人的简历(字数控制在50字以内):\n");
    scanf("%s", F->data.resume);
    fflush(stdin);
    printf("请说明此人是否还健在(健在输入1,死亡输入0):\n");
    j = 1;
    while (j)
    {
        scanf("%d", &(F->data.alive));
        fflush(stdin);
        switch (F->data.alive)
        {
        case 1:
            i = 0;
            j = 0;
            while (i == 0)
                i = birth_date(F);
            F->data.death_date.year = F->data.death_date.month = F->data.death_date.day = 0;
            break;
        case 0:
            i = 0;
            j = 0;
            while (i == 0)
                i = birth_date(F);
            i = 0;
            while (i == 0)
                i = death_date(F);
            break;
        default:
            printf("输入数据有误,请重新输入!\n");
            fflush(stdin);
            break;
        }
    }
    return 1;
}
// 按姓名查找指定成员函数
person find_name(person &T, char Name[])
{

    LinkQueue q;
    QElemType a;
    if (T)
    {
        InitQueue(q); // 构建一个空队列q
        EnQueue(q, T);
        while (!QueueEmpty(q))
        {
            DeQueue(q, a);
            if (strcmp(a->data.name, Name) == 0)
                return a;
            if (a->first_child)
                EnQueue(q, a->first_child);
            if (a->next_brother)
                EnQueue(q, a->next_brother);
        }
    }
    return 0;
}
// 输入出生日期
int birth_date(person &T)
{
    printf("请输入%s的出生年份:\n", T->data.name);
    scanf("%d", &(T->data.birth_date.year));
    fflush(stdin);
    printf("请输入%s的出生月份:\n", T->data.name);
    scanf("%d", &(T->data.birth_date.month));
    fflush(stdin);
    printf("请输入%s的出生日:\n", T->data.name);
    scanf("%d", &(T->data.birth_date.day));
    fflush(stdin);
    if (T->data.birth_date.month < 1 || T->data.birth_date.month > 12)
    {
        printf("输入日期有误，请重新输入！\n");
        return 0;
    }

    switch (T->data.birth_date.month)
    {
    case 1:
    case 3:
    case 5:
    case 7:
    case 8:
    case 10:
    case 12:
        if (T->data.birth_date.day < 1 || T->data.birth_date.day > 31)
        {
            printf("输入日期有误，请重新输入！\n");
            return 0;
        }
        break;
    case 4:
    case 6:
    case 9:
    case 11:
        if (T->data.birth_date.day < 1 || T->data.birth_date.day > 30)
        {
            printf("输入日期有误，请重新输入！\n");
            return 0;
        }
        break;
    case 2:
        if (T->data.birth_date.year % 4 == 0 && T->data.birth_date.year % 100 != 0 || T->data.birth_date.year % 400 == 0) /*是闰年*/
        {
            if (T->data.birth_date.day < 1 || T->data.birth_date.day > 29)
            {
                printf("输入日期有误，请重新输入！\n");
                return 0;
            }
        }
        else
        {
            if (T->data.birth_date.day < 1 || T->data.birth_date.day > 28)
            {
                printf("输入日期有误，请重新输入！\n");
                return 0;
            }
        }
        break;
    }
    return judge_birth_date(T);
}
int judge_birth_date(person &T) // 判断出生日期是否在今天之前
{
    int W, a, b, c;
    int U = 0, V = 0, X = 0, D = 0, J = 0, L = 0;
    time_t tval;
    struct tm *now; // struct tm是系统预定义的时间结构体类型
    tval = time(NULL);
    now = localtime(&tval);
    a = now->tm_year + 1900;
    b = now->tm_mon + 1;
    c = now->tm_mday;
    U = T->data.birth_date.year;
    V = T->data.birth_date.month;
    W = T->data.birth_date.day;
    if ((U > a) || (U == a && V > b) || (U == a && V == b && W > c))
    {
        printf("现在时间: %4d年 %d月 %02d日 %d:%02d:%02d\n", a, b, c, now->tm_hour, now->tm_min, now->tm_sec);
        printf("输入日期有误！请重新输入！\n");
        return 0;
    }
    else
    {
        for (; U < a; V++)
            survival_day_sum(U, V, X, D, J, L);
        for (; U == a && V < b; V++)
            survival_day_sum(U, V, X, D, J, L);
        T->data.alive_day = 31 * D + 30 * X + 28 * J + L - W + c;
    }
    return 1;
}
// 输入死者死亡日期
int death_date(person &T)
{
    printf("请输入%s的死亡年份:\n", T->data.name);
    scanf("%d", &(T->data.death_date.year));
    fflush(stdin);
    printf("请输入%s的死亡月份:\n", T->data.name);
    scanf("%d", &(T->data.death_date.month));
    fflush(stdin);
    printf("请输入%s的死亡日:\n", T->data.name);
    scanf("%d", &(T->data.death_date.day));
    fflush(stdin);
    if (T->data.death_date.month < 1 || T->data.death_date.month > 12)
    {
        printf("输入日期有误，请重新输入！\n");
        return 0;
    }

    switch (T->data.death_date.month)
    {
    case 1:
    case 3:
    case 5:
    case 7:
    case 8:
    case 10:
    case 12:
        if (T->data.death_date.day < 1 || T->data.death_date.day > 31)
        {
            printf("输入日期有误，请重新输入！\n");
            return 0;
        }
        break;
    case 4:
    case 6:
    case 9:
    case 11:
        if (T->data.death_date.day < 1 || T->data.death_date.day > 30)
        {
            printf("输入日期有误，请重新输入！\n");
            return 0;
        }
        break;
    case 2:
        if (T->data.death_date.year % 4 == 0 && T->data.death_date.year % 100 != 0 || T->data.death_date.year % 400 == 0) /*是闰年*/
        {
            if (T->data.death_date.day < 1 || T->data.death_date.day > 29)
            {
                printf("输入日期有误，请重新输入！\n");
                return 0;
            }
        }
        else
        {
            if (T->data.death_date.day < 1 || T->data.death_date.day > 28)
            {
                printf("输入日期有误，请重新输入！\n");
                return 0;
            }
        }
        break;
    }
    return judge_death_date(T);
}
// 判断死亡日期死亡日期是否正确
int judge_death_date(person &T)
{
    int W, a, b, c, i, j, k;
    int U = 0, V = 0, X = 0, D = 0, J = 0, L = 0;
    time_t tval;
    struct tm *now;
    tval = time(NULL);
    now = localtime(&tval);
    U = T->data.birth_date.year;
    V = T->data.birth_date.month;
    W = T->data.birth_date.day;
    a = T->data.death_date.year;
    b = T->data.death_date.month;
    c = T->data.death_date.day;
    i = now->tm_year + 1900;
    j = now->tm_mon + 1;
    k = now->tm_mday;
    if ((U > a) || (U == a && V > b) || (U == a && V == b && W > c))
    {
        printf("输入死亡日期早于出生日期！请重新输入！\n");
        return 0;
    }
    if ((a > i) || (a == i && b > j) || (a == i && b == j && c > k))
    {
        printf("现在时间: %4d年 %d月 %02d日 %d:%02d:%02d\n", i, j, k, now->tm_hour, now->tm_min, now->tm_sec);
        printf("输入日期有误！请重新输入！\n");
        return 0;
    }
    for (; U < a; V++)
        survival_day_sum(U, V, X, D, J, L);
    for (; U == a && V < b; V++)
        survival_day_sum(U, V, X, D, J, L);
    T->data.alive_day = 31 * D + 30 * X + 28 * J + L - W + c;
    printf("%s活了 %d 天。\n", T->data.name, T->data.alive_day);
    return 1;
}
void survival_day_sum(int &m, int &n, int &o, int &p, int &q, int &r) // 计算生存天数
{
    switch (n)
    {
    case 1:
        p = p + 1;
        break;
    case 2:
        q = q + 1;
        if (!(m % 400) || !(m % 4) && m % 100)
            r = r + 1;
        break;
    case 3:
        p = p + 1;
        break;
    case 4:
        o = o + 1;
        break;
    case 5:
        p = p + 1;
        break;
    case 6:
        o = o + 1;
        break;
    case 7:
        p = p + 1;
        break;
    case 8:
        p = p + 1;
        break;
    case 9:
        o = o + 1;
        break;
    case 10:
        p = p + 1;
        break;
    case 11:
        o = o + 1;
        break;
    case 12:
        p = p + 1, m = m + 1, n = 0;
        break;
    }
    return;
}
void add_new_member(person &T) // 添加成员函数
{
    person Tree;
    Tree = (person)malloc(sizeof(CSNode));
    start_up(Tree);
    int h, j = 1;
    if (!strcmp(T->data.name, "0"))
    {
        printf("家谱不存在，请新建家谱树\n");
        return;
    }
    else
    {
        printf("0:返回主菜单\n1:添加新成员作为整个家谱的祖先\n2:添加新成员作为某一成员的孩子\n");
        printf("请输入你的选项(0~2):");
        while (j)
        {
            scanf("%d", &h);
            fflush(stdin);
            switch (h)
            {
            case 0:
                return;
                break;
            case 1:
                if (Create_familytree(Tree, T))
                {
                    Tree->first_child = T;
                    T->parent = Tree;
                    strcpy(T->data.parent_name, Tree->data.name);
                    T = Tree;
                    printf("添加成功！\n");
                }
                else
                    return;
                j = 0;
                break;
            case 2:
                printf("请输入所添加成员的信息\n");
                if (!Create_familytree(Tree, T))
                    return;
                printf("添加成功！\n");
                j = 0;
                break;
            default:
                printf("\n输入有误！");
                printf("\n请重新输入:\n");
                fflush(stdin);
                break;
            }
        }
    }
    level(T);
    sort(T);
    return;
}

// 为一个父亲的孩子按出生日期排序
void sort_every(person &T)
{
    int n = 0, i, j, U, V, W, a, b, c;
    person p, temp;
    p = (person)malloc(sizeof(CSNode));
    if (!p)
        exit(0);
    temp = (person)malloc(sizeof(CSNode));
    if (!temp)
        exit(0);
    start_up(temp);
    start_up(p);
    p = T->first_child;
    while (p) // 计算T有多少个孩子
    {
        n = n + 1;
        p = p->next_brother;
    }
    T->data.children_count = n;
    if (n == 0 || n == 1)
    {
        return;
    }
    else
    {
        p = T->first_child; // q指向T的第一个孩子
        for (i = 1; i < n; i++)
        {
            for (j = 0; j < n - i; j++)
            {
                U = p->data.birth_date.year;
                V = p->data.birth_date.month;
                W = p->data.birth_date.day;
                a = p->next_brother->data.birth_date.year;
                b = p->next_brother->data.birth_date.month;
                c = p->next_brother->data.birth_date.day;
                if ((U > a) || (U == a && V > b) || (U == a && V == b && W > c))
                {
                    temp->data = p->data;
                    temp->first_child = p->first_child;
                    p->data = p->next_brother->data;
                    p->first_child = p->next_brother->first_child;
                    p->next_brother->data = temp->data;
                    p->next_brother->first_child = temp->first_child;
                }
                p = p->next_brother; // 指向下一个兄弟
            }
            p = T->first_child; // 重新定位到第一个孩子
        }
    }
    return;
}
void sort(person &T) // 排序
{
    if (T)
    {
        sort_every(T);
        sort(T->first_child);
        sort(T->next_brother);
    }
    return;
}

void level(person &T) // 计算每位家族成员的辈分
{
    if (T)
    {
        level_every(T);
        level(T->first_child);
        level(T->next_brother);
    }
    return;
}
void level_every(person &T) // 计算一位家族成员的辈分
{
    person F;
    F = (person)malloc(sizeof(CSNode));
    start_up(F);
    F = T;
    T->data.Depth = 1;
    while (F->parent)
    {
        T->data.Depth++;
        F = F->parent;
    }
    return;
}
void preserve(person &T) // 保存成员信息函数
{
    FILE *fp;
    if ((fp = fopen("jiapu", "wb")) == NULL)
    {
        printf("家谱文件不存在！\n");
        return;
    }
    preserve_family(T, fp);
    fclose(fp);
    printf("家谱保存成功！\n");
    return;
}
void preserve_family(person &T, FILE *fp) // 保存所有成员信息函数
{
    preserve_member(T, fp);
    int i;
    LinkQueue q;
    QElemType a;
    person b;
    if (T)
    {
        InitQueue(q); // 构建一个空队列q
        EnQueue(q, T);
        while (!QueueEmpty(q))
        {
            DeQueue(q, a);
            if (a->data.children_count > 0)
            {
                b = a->first_child;
                preserve_member(b, fp);
                if (a->data.children_count > 1)
                {
                    LinkQueue f;
                    QElemType c;
                    InitQueue(f);
                    EnQueue(f, a->first_child);
                    for (i = 1; i < a->data.children_count; i++)
                    {
                        DeQueue(f, c);
                        b = c->next_brother;
                        preserve_member(b, fp);
                        EnQueue(f, c->next_brother);
                    }
                }
            }
            if (a->next_brother)
                EnQueue(q, a->next_brother);
            if (a->first_child)
                EnQueue(q, a->first_child);
        }
    }
    return;
}

void preserve_member(person &T, FILE *fp) // 保存一个成员信息函数
{
    fwrite(T, sizeof(CSNode), 1, fp);
    return;
}

void show(person &T) // 显示家谱树函数
{
    FILE *fp;
    if ((fp = fopen("jiapu", "rb")) == NULL)
    {
        printf("家谱文件不存在！\n");
        return;
    }
    fread(T, sizeof(CSNode), 1, fp);
    read_node(T, fp);
    fclose(fp);
    printf("家谱树层次化显示如下:\n\n");
    show_tree(T);
    return;
}
void read_node_every(person &T, FILE *fp) // 从文件中读取一个成员节点
{
    T = (person)malloc(sizeof(CSNode));
    start_up(T);
    fread(T, sizeof(CSNode), 1, fp);
    return;
}

void read_node(person &T, FILE *fp) // 从文件中读取所有成员节点
{
    int i;
    LinkQueue q, f;
    QElemType a, c;
    person b;
    if (T)
    {
        InitQueue(q); // 构建一个空队列q
        EnQueue(q, T);
        while (!QueueEmpty(q))
        {
            DeQueue(q, a);
            if (a->data.children_count > 0)
            {
                read_node_every(b, fp);
                b->parent = a;
                a->first_child = b;
                if (a->data.children_count > 1)
                {
                    InitQueue(f);
                    EnQueue(f, a->first_child);
                    for (i = 1; i < a->data.children_count; i++)
                    {
                        DeQueue(f, c);
                        read_node_every(b, fp);
                        b->parent = a;
                        c->next_brother = b;
                        EnQueue(f, c->next_brother);
                    }
                }
            }
            if (a->next_brother)
                EnQueue(q, a->next_brother);
            if (a->first_child)
                EnQueue(q, a->first_child);
        }
    }
    return;
}

void show_tree_member(person &T) // 把家谱成员姓名通过一个层次化的家谱树显示出来
{
    int i, b, c;
    time_t tval;
    struct tm *now;
    tval = time(NULL);
    now = localtime(&tval);
    b = now->tm_mon + 1;
    c = now->tm_mday;
    for (i = 1; i < T->data.Depth; i++)
        printf("      ");
    printf("%s\n", T->data.name);
    HANDLE consolehwnd;
    consolehwnd = GetStdHandle(STD_OUTPUT_HANDLE);
    return;
}

void show_tree(person &T)
{
    if (T)
    {
        show_tree_member(T);
        show_tree(T->first_child);
        show_tree(T->next_brother);
    }
    return;
}
void lookup(person &T) // 通过名字查找相应的成员，并显示其信息
{
    int i, k, m, n, j = 1, h = 1;
    char Name[20];
    person a;
    a = (person)malloc(sizeof(CSNode));
    start_up(a);
    printf("请输入要查找成员的姓名：\n");
    scanf("%s", Name);
    fflush(stdin);
    a = find_name(T, Name);
    if (a == 0)
    {
        printf("该家谱无此人信息!\n");
        return;
    }
    show_member(a);
    printf("请输入您的操作：\n");
    printf("0:返回主菜单\n1:修改该成员信息\n2:删除该成员\n"); // 对查找到的成员所可以进行的操作
    while (j)
    {
        scanf("%d", &i);
        fflush(stdin);
        switch (i)
        {
        case 0:
            j = 0;
            break;
        case 1:
            j = 0;
            HANDLE consolehwnd;
            consolehwnd = GetStdHandle(STD_OUTPUT_HANDLE);
            SetConsoleTextAttribute(consolehwnd, FOREGROUND_INTENSITY | FOREGROUND_BLUE);
            printf("\n\t\t------------------------------------------------\n");
            SetConsoleTextAttribute(consolehwnd, FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE);
            printf("\t\t           请选修改内容        \n");
            printf("\t\t   1. 姓名      7.学历             \n");
            printf("\t\t   2. 性别      8.职业             \n");
            printf("\t\t   3. 身高      9.简历             \n");
            printf("\t\t   4. 居住地址  10.出生日期        \n");
            printf("\t\t   5. 婚否      11.健在否(死亡日期)\n");
            printf("\t\t   6. 出生地址  12.父亲姓名        \n");
            SetConsoleTextAttribute(consolehwnd, FOREGROUND_INTENSITY | FOREGROUND_BLUE);
            printf("\t\t------------------------------------------------\n");
            printf("\t\t请输入你的选择(不修改请输入0):\n");
            while (h)
            {
                scanf("%d", &k);
                fflush(stdin);
                switch (k)
                {
                case 0:
                    h = 0;
                    break;
                case 1:
                    h = 0;
                    char name[20];
                    printf("请输入此人名字:\n");
                    scanf("%s", name);
                    fflush(stdin);
                    person e;
                    e = (person)malloc(sizeof(CSNode));
                    start_up(e);
                    e = find_name(T, name);
                    if (e)
                    {
                        printf("该名字与家谱成员姓名重复，修改失败！\n");
                        break;
                    }
                    strcpy(a->data.name, name);
                    if (a->data.children_count > 0)
                    {
                        strcpy(a->first_child->data.parent_name, a->data.name);
                        if (a->data.children_count > 1)
                        {
                            LinkQueue f;
                            QElemType c;
                            InitQueue(f);
                            EnQueue(f, a->first_child);
                            for (i = 1; i < a->data.children_count; i++)
                            {
                                DeQueue(f, c);
                                strcpy(c->next_brother->data.parent_name, a->data.name);
                                EnQueue(f, c->next_brother);
                            }
                        }
                    }
                    printf("修改成功！\n");
                    break;
                case 2:
                    h = 0;
                    printf("请输入此人性别(男输入1，女输入0):\n");
                    m = 1;
                    while (m)
                    {
                        scanf("%d", &(a->data.sex));
                        fflush(stdin);
                        switch (a->data.sex)
                        {
                        case 1:
                            m = 0;
                            break;
                        case 0:
                            m = 0;
                            Delete(a->first_child);
                            a->first_child = NULL;
                            break;
                        default:
                            printf("输入数据有误,请重新输入!\n");
                            fflush(stdin);
                            break;
                        }
                    }
                    printf("修改成功！\n");
                    break;
                case 3:
                    h = 0;
                    printf("请输入此人身高(单位cm):\n");
                    scanf("%d", &(a->data.height));
                    fflush(stdin);
                    printf("修改成功！\n");
                    break;
                case 4:
                    h = 0;
                    printf("请输入此人居住地址:\n");
                    scanf("%s", a->data.address);
                    fflush(stdin);
                    printf("修改成功！\n");
                    break;
                case 5:
                    h = 0;
                    printf("请说明此人是否已婚(已婚输入1，未婚输入0)\n");
                    m = 1;
                    while (m)
                    {
                        scanf("%d", &(a->data.marry));
                        fflush(stdin);
                        switch (a->data.marry)
                        {
                        case 0:
                            m = 0;
                            strcpy(a->data.wife_or_husband, "0");
                            fflush(stdin);
                            break;
                        case 1:
                            m = 0;
                            printf("请输入此人配偶的名字:\n");
                            scanf("%s", a->data.wife_or_husband);
                            fflush(stdin);
                            break;
                        default:
                            printf("输入数据有误,请重新输入!\n");
                            fflush(stdin);
                            break;
                        }
                    }
                    printf("修改成功！\n");
                    break;
                case 6:
                    h = 0;
                    printf("请输入此人出生地址:\n");
                    scanf("%s", a->data.birth_place);
                    fflush(stdin);
                    printf("修改成功！\n");
                    break;
                case 7:
                    h = 0;
                    printf("请输入此人学历:\n");
                    scanf("%s", a->data.education);
                    fflush(stdin);
                    printf("修改成功！\n");
                    break;
                case 8:
                    h = 0;
                    printf("请输入此人的职业:\n");
                    scanf("%s", a->data.occupation);
                    fflush(stdin);
                    printf("修改成功！\n");
                    break;
                case 9:
                    h = 0;
                    printf("请输入此人的简历(字数控制在50字以内):\n");
                    scanf("%s", a->data.resume);
                    fflush(stdin);
                    printf("修改成功！\n");
                    break;
                case 10:
                    h = 0;
                    n = 0;
                    while (n == 0)
                    {
                        n = birth_date(a);
                    }
                    printf("修改成功！\n");
                    break;
                case 11:
                    h = 0;
                    printf("请说明此人是否还健在(健在输入1,死亡输入0):\n");
                    m = 1;
                    while (m)
                    {
                        scanf("%d", &(a->data.alive));
                        fflush(stdin);
                        switch (a->data.alive)
                        {
                        case 1:
                            n = 0;
                            m = 0;
                            a->data.death_date.year = a->data.death_date.month = a->data.death_date.day = 0;
                            break;
                        case 0:
                            n = 0;
                            m = 0;
                            while (n == 0)
                            {
                                n = death_date(a);
                            }
                            break;

                        default:
                            printf("输入数据有误,请重新输入!\n");
                            fflush(stdin);
                            break;
                        }
                    }
                    printf("修改成功！\n");
                    break;
                case 12:
                    h = 0;
                    person d;
                    d = (person)malloc(sizeof(CSNode));
                    start_up(d);
                    person b;
                    b = (person)malloc(sizeof(CSNode));
                    start_up(b);
                    if (a->parent)
                    {
                        b = a->parent;
                        if (!strcmp(b->first_child->data.name, Name))
                        {
                            b->first_child = a->next_brother;
                        }
                        else
                        {
                            b = b->first_child;
                            while (strcmp(b->next_brother->data.name, Name))
                            {
                                b = b->next_brother;
                            }
                            b->next_brother = a->next_brother;
                        }
                    }
                    else
                    {
                        printf("%s是整个家族的祖先，不可修改父亲姓名！\n", Name);
                        break;
                    }
                    printf("请输入此人父亲的名字:\n");
                    scanf("%s", a->data.parent_name);
                    fflush(stdin);
                    d = find_name(T, a->data.parent_name);
                    if (d == 0 || d->data.sex == 0)
                    {
                        printf("父亲姓名输入错误！修改失败！\n");
                        strcpy(a->data.parent_name, a->parent->data.name);
                        d = a->parent;
                        if (d->first_child == NULL)
                        {
                            d->first_child = a;
                            a->next_brother = NULL;
                        }
                        else
                        {
                            d = d->first_child;
                            while (d->next_brother)
                                d = d->next_brother;
                            d->next_brother = a;
                            a->next_brother = NULL;
                        }
                    }
                    else
                    {
                        printf("父亲姓名输入正确！\n");
                        a->parent = d;
                        if (d->first_child == NULL)
                        {
                            d->first_child = a;
                            a->next_brother = NULL;
                        }
                        else
                        {
                            d = d->first_child;
                            while (d->next_brother)
                                d = d->next_brother;
                            d->next_brother = a;
                            a->next_brother = NULL;
                        }
                    }
                    printf("修改成功！\n");
                    break;
                default:
                    printf("输入数据有误,请重新输入!\n");
                    fflush(stdin);
                    break;
                }
            }
            break;
        case 2:
            j = 0;
            Delete(a->first_child);
            a->first_child = NULL;
            if (a->parent)
            {
                person b;
                b = (person)malloc(sizeof(CSNode));
                start_up(b);
                b = a->parent;
                if (!strcmp(b->first_child->data.name, Name))
                {
                    b->first_child = a->next_brother;
                    free(a);
                }
                else
                {
                    b = b->first_child;
                    while (strcmp(b->next_brother->data.name, Name))
                        b = b->next_brother;
                    b->next_brother = a->next_brother;
                    free(a);
                }
            }
            else
            {
                free(a);
                T = (person)malloc(sizeof(CSNode));
                start_up(T);
                strcpy(T->data.name, "0");
            }
            printf("删除成功！\n");
            break;
        default:
            printf("\n输入有误！");
            printf("\n请重新输入:\n");
            fflush(stdin);
            break;
        }
    }
    if (strcmp(T->data.name, "0"))
    {
        sort(T);
        level(T);
    }
    return;
}

void Delete(person &T) // 采用后续遍历的方法删除成员节点
{
    if (T)
    {
        Delete(T->first_child);
        Delete(T->next_brother);
        free(T);
    }
    return;
}

void show_member(person &T) // 显示一个成员所有信息的函数
{
    printf("\n姓名:%s  ", T->data.name);
    switch (T->data.sex)
    {
    case 1:
        printf("性别:男  ");
        break;
    case 0:
        printf("性别:女  ");
        break;
    }
    printf("第%d代  ", T->data.Depth);
    printf("是否健在:");
    switch (T->data.alive)
    {
    case 1:
        printf("健在  ");
        printf("出生日期:%d年%d月%d日  ", T->data.birth_date.year, T->data.birth_date.month, T->data.birth_date.day);
        break;
    case 0:
        printf("已死亡  ");
        printf("出生日期:%d年%d月%d日  ", T->data.birth_date.year, T->data.birth_date.month, T->data.birth_date.day);
        printf("死亡日期:%d年%d月%d日  ", T->data.birth_date.year, T->data.birth_date.month, T->data.birth_date.day);
        break;
    }
    printf("身高:%dcm  ", T->data.height);
    printf("居住地址:%s  ", T->data.address);
    printf("婚否:");
    switch (T->data.marry)
    {
    case 1:
        printf("已婚  ");
        printf("此人配偶的名字:%s  ", T->data.wife_or_husband);
        break;
    case 0:
        printf("未婚  ");
        break;
    }
    printf("父亲姓名:%s  ", T->data.parent_name);
    printf("孩子数:%d  ", T->data.children_count);
    if (T->data.children_count)
    {
        printf("其孩子分为为:");
        auto iter = T->first_child;
        while (iter)
        {
            printf("%s ", iter->data.name);
            iter = iter->next_brother;
        }
    }
    printf("出生地址:%s  ", T->data.birth_place);
    printf("学历:%s  ", T->data.education);
    printf("职业:%s  ", T->data.occupation);
    printf("简历:%s  \n\n", T->data.resume);
    return;
}
void find(person &T) // 通过关键字搜索家谱成员
{
    int j = 1, k = 1;
    int i;
    LinkQueue q;
    QElemType a;
    while (j)
    {
        HANDLE consolehwnd;
        consolehwnd = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(consolehwnd, FOREGROUND_INTENSITY | FOREGROUND_BLUE);
        printf("\n\t        ------------------------------------------\n\t");
        SetConsoleTextAttribute(consolehwnd, FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE);
        printf("\t\t        请选择关键字     \n\t");
        printf("\t\t   1. 性别        8. 父亲姓名\n\t");
        printf("\t\t   2. 身高        9. 健在否  \n\t");
        printf("\t\t   3. 居住地址    10.简历    \n\t");
        printf("\t\t   4. 婚否        11.出生日期\n\t");
        printf("\t\t   5. 出生地址    12.死亡日期\n\t");
        printf("\t\t   6. 学历        13.辈分    \n\t");
        printf("\t\t   7. 职业        14.孩子数  \n\t");
        SetConsoleTextAttribute(consolehwnd, FOREGROUND_INTENSITY | FOREGROUND_BLUE);
        printf("\t------------------------------------------\n\t");
        printf("请输入你的选择(按0返回主菜单):\n");
        scanf("%d", &i);
        fflush(stdin);
        switch (i)
        {
        case 0:
            j = 0;
            break;
        case 1:
            j = 0;
            int sex;
            printf("请输入要搜索的性别(男输入1，女输入0):\n");
            scanf("%d", &sex);
            fflush(stdin);
            if (strcmp(T->data.name, "0"))
            {
                InitQueue(q); // 构建一个空队列q
                EnQueue(q, T);
                while (!QueueEmpty(q))
                {
                    DeQueue(q, a);
                    if (a->data.sex == sex)
                    {
                        show_member(a);
                        k = 0;
                    }
                    if (a->first_child)
                        EnQueue(q, a->first_child);
                    if (a->next_brother)
                        EnQueue(q, a->next_brother);
                }
                if (k)
                    printf("家谱中没有搜索到相关人员！\n");
            }
            else
                printf("家谱为空，请新建家谱！\n");

            break;
        case 2:
            j = 0;
            int height;
            printf("请输入要搜索的身高(单位:cm):\n");
            scanf("%d", &height);
            fflush(stdin);
            if (strcmp(T->data.name, "0"))
            {
                InitQueue(q); // 构建一个空队列q
                EnQueue(q, T);
                while (!QueueEmpty(q))
                {
                    DeQueue(q, a);
                    if (a->data.height == height)
                    {
                        show_member(a);
                        k = 0;
                    }
                    if (a->first_child)
                        EnQueue(q, a->first_child);
                    if (a->next_brother)
                        EnQueue(q, a->next_brother);
                }
                if (k)
                    printf("家谱中没有搜索到相关人员！\n");
            }
            else
                printf("家谱为空，请新建家谱！\n");

            break;
        case 3:
            j = 0;
            char address[50];
            printf("请输入要搜索的地址:\n");
            scanf("%s", address);
            fflush(stdin);
            if (strcmp(T->data.name, "0"))
            {
                InitQueue(q); // 构建一个空队列q
                EnQueue(q, T);
                while (!QueueEmpty(q))
                {
                    DeQueue(q, a);
                    if (strcmp(a->data.address, address) == 0)
                    {
                        show_member(a);
                        k = 0;
                    }
                    if (a->first_child)
                        EnQueue(q, a->first_child);
                    if (a->next_brother)
                        EnQueue(q, a->next_brother);
                }
                if (k)
                    printf("家谱中没有搜索到相关人员！\n");
            }
            else
                printf("家谱为空，请新建家谱！\n");
            break;
        case 4:
            j = 0;
            int jiehun;
            printf("请输入要搜索的婚姻状况(已婚输入1，未婚输入0):\n");
            scanf("%d", &jiehun);
            fflush(stdin);
            if (strcmp(T->data.name, "0"))
            {
                InitQueue(q); // 构建一个空队列q
                EnQueue(q, T);
                while (!QueueEmpty(q))
                {
                    DeQueue(q, a);
                    if (a->data.marry == jiehun)
                    {
                        show_member(a);
                        k = 0;
                    }
                    if (a->first_child)
                        EnQueue(q, a->first_child);
                    if (a->next_brother)
                        EnQueue(q, a->next_brother);
                }
                if (k)
                    printf("家谱中没有搜索到相关人员！\n");
            }
            else
                printf("家谱为空，请新建家谱！\n");
            break;
        case 5:
            j = 0;
            char birthplace[20];
            printf("请输入要搜索的地址:\n");
            scanf("%s", birthplace);
            fflush(stdin);
            if (strcmp(T->data.name, "0"))
            {
                InitQueue(q); // 构建一个空队列q
                EnQueue(q, T);
                while (!QueueEmpty(q))
                {
                    DeQueue(q, a);
                    if (strcmp(a->data.birth_place, birthplace) == 0)
                    {
                        show_member(a);
                        k = 0;
                    }
                    if (a->first_child)
                        EnQueue(q, a->first_child);
                    if (a->next_brother)
                        EnQueue(q, a->next_brother);
                }
                if (k)
                    printf("家谱中没有搜索到相关人员！\n");
            }
            else
                printf("家谱为空，请新建家谱！\n");
            break;
        case 6:
            j = 0;
            char education[20];
            printf("请输入要搜索的学历:\n");
            scanf("%s", education);
            fflush(stdin);
            if (strcmp(T->data.name, "0"))
            {
                InitQueue(q); // 构建一个空队列
                EnQueue(q, T);
                while (!QueueEmpty(q))
                {
                    DeQueue(q, a);
                    if (strcmp(a->data.education, education) == 0)
                    {
                        show_member(a);
                        k = 0;
                    }
                    if (a->first_child)
                        EnQueue(q, a->first_child);
                    if (a->next_brother)
                        EnQueue(q, a->next_brother);
                }
                if (k)
                    printf("家谱中没有搜索到相关人员！\n");
            }
            else
                printf("家谱为空，请新建家谱！\n");
            break;
        case 7:
            j = 0;
            char occupation[20];
            printf("请输入要搜索的职业:\n");
            scanf("%s", occupation);
            fflush(stdin);
            if (strcmp(T->data.name, "0"))
            {
                InitQueue(q); // 构建一个空队列q
                EnQueue(q, T);
                while (!QueueEmpty(q))
                {
                    DeQueue(q, a);
                    if (strcmp(a->data.occupation, occupation) == 0)
                    {
                        show_member(a);
                        k = 0;
                    }
                    if (a->first_child)
                        EnQueue(q, a->first_child);
                    if (a->next_brother)
                        EnQueue(q, a->next_brother);
                }
                if (k)
                    printf("家谱中没有搜索到相关人员！\n");
            }
            else
                printf("家谱为空，请新建家谱！\n");
            break;
        case 8:
            j = 0;
            char parentname[20];
            printf("请输入要搜索的名字:\n");
            scanf("%s", parentname);
            fflush(stdin);
            if (strcmp(T->data.name, "0"))
            {
                InitQueue(q); // 构建一个空队列q
                EnQueue(q, T);
                while (!QueueEmpty(q))
                {
                    DeQueue(q, a);
                    if (strcmp(a->data.parent_name, parentname) == 0)
                    {
                        show_member(a);
                        k = 0;
                    }
                    if (a->first_child)
                        EnQueue(q, a->first_child);
                    if (a->next_brother)
                        EnQueue(q, a->next_brother);
                }
                if (k)
                    printf("家谱中没有搜索到相关人员！\n");
            }
            else
                printf("家谱为空，请新建家谱！\n");
            break;
        case 9:
            j = 0;
            int alive;
            printf("请输入要搜索的生存状况(健在输入1，死亡输入0):\n");
            scanf("%d", &alive);
            fflush(stdin);
            if (strcmp(T->data.name, "0"))
            {
                InitQueue(q); // 构建一个空队列q
                EnQueue(q, T);
                while (!QueueEmpty(q))
                {
                    DeQueue(q, a);
                    if (a->data.alive == alive)
                    {
                        show_member(a);
                        k = 0;
                    }
                    if (a->first_child)
                        EnQueue(q, a->first_child);
                    if (a->next_brother)
                        EnQueue(q, a->next_brother);
                }
                if (k)
                    printf("家谱中没有搜索到相关人员！\n");
            }
            else
                printf("家谱为空，请新建家谱！\n");
            break;
        case 10:
            j = 0;
            printf("请输入简历中要搜索的关键词(关键词控制在10个字以内):\n");
            char resume[20];
            scanf("%s", resume);
            fflush(stdin);
            if (strcmp(T->data.name, "0"))
            {
                InitQueue(q); // 构建一个空队列q
                EnQueue(q, T);
                while (!QueueEmpty(q))
                {
                    DeQueue(q, a);
                    if (kmp(a->data.resume, resume))
                    {
                        show_member(a);
                        k = 0;
                    }
                    if (a->first_child)
                        EnQueue(q, a->first_child);
                    if (a->next_brother)
                        EnQueue(q, a->next_brother);
                }
                if (k)
                    printf("家谱中没有搜索到相关人员！\n");
            }
            else
                printf("家谱为空，请新建家谱！\n");
            break;
        case 11:
            j = 0;
            int nian, yue, ri;
            printf("请输入要搜索的日期:\n");
            printf("年:\n");
            scanf("%d", &nian);
            fflush(stdin);
            printf("月:\n");
            scanf("%d", &yue);
            fflush(stdin);
            printf("日:\n");
            scanf("%d", &ri);
            fflush(stdin);
            if (strcmp(T->data.name, "0"))
            {
                InitQueue(q); // 构建一个空队列q
                EnQueue(q, T);
                while (!QueueEmpty(q))
                {
                    DeQueue(q, a);
                    if (a->data.birth_date.year == nian && a->data.birth_date.month == yue && a->data.birth_date.day == ri)
                    {
                        show_member(a);
                        k = 0;
                    }
                    if (a->first_child)
                        EnQueue(q, a->first_child);
                    if (a->next_brother)
                        EnQueue(q, a->next_brother);
                }
                if (k)
                    printf("家谱中没有搜索到相关人员！\n");
            }
            else
                printf("家谱为空，请新建家谱！\n");
            break;
        case 12:
            j = 0;
            int nian1, yue1, ri1;
            printf("请输入要搜索的日期:\n");
            printf("年:\n");
            scanf("%d", &nian1);
            fflush(stdin);
            printf("月:\n");
            scanf("%d", &yue1);
            fflush(stdin);
            printf("日:\n");
            scanf("%d", &ri1);
            fflush(stdin);
            if (strcmp(T->data.name, "0"))
            {
                InitQueue(q); // 构建一个空队列q
                EnQueue(q, T);
                while (!QueueEmpty(q))
                {
                    DeQueue(q, a);
                    if (a->data.birth_date.year == nian1 && a->data.birth_date.month == yue1 && a->data.birth_date.day == ri1)
                    {
                        show_member(a);
                        k = 0;
                    }
                    if (a->first_child)
                        EnQueue(q, a->first_child);
                    if (a->next_brother)
                        EnQueue(q, a->next_brother);
                }
                if (k)
                    printf("家谱中没有搜索到相关人员！\n");
            }
            else
                printf("家谱为空，请新建家谱！\n");
            break;
        case 13:
            j = 0;
            int beifen;
            printf("请输入要搜索的辈分:\n");
            scanf("%d", &beifen);
            fflush(stdin);
            if (strcmp(T->data.name, "0"))
            {
                InitQueue(q); // 构建一个空队列q
                EnQueue(q, T);
                while (!QueueEmpty(q))
                {
                    DeQueue(q, a);
                    if (a->data.Depth == beifen)
                    {
                        show_member(a);
                        k = 0;
                    }
                    if (a->first_child)
                        EnQueue(q, a->first_child);
                    if (a->next_brother)
                        EnQueue(q, a->next_brother);
                }
                if (k)
                    printf("家谱中没有搜索到相关人员！\n");
            }
            else
                printf("家谱为空，请新建家谱！\n");
            break;
        case 14:
            j = 0;
            int hzshu;
            printf("请输入要搜索的成员的孩子数:\n");
            scanf("%d", &hzshu);
            fflush(stdin);
            if (strcmp(T->data.name, "0"))
            {
                InitQueue(q); // 构建一个空队列q
                EnQueue(q, T);
                while (!QueueEmpty(q))
                {
                    DeQueue(q, a);
                    if (a->data.children_count == hzshu)
                    {
                        show_member(a);
                        k = 0;
                    }
                    if (a->first_child)
                        EnQueue(q, a->first_child);
                    if (a->next_brother)
                        EnQueue(q, a->next_brother);
                }
                if (k)
                    printf("家谱中没有搜索到相关人员！\n");
            }
            else
                printf("家谱为空，请新建家谱！\n");
            break;
        default:
            printf("你的操作有误，请重新选择!\n");
            fflush(stdin);
            system("pause");
            system("cls");
        }
    }
    return;
}
int kmp(char str[], char ptrn[]) // KMP算法，用于搜索简历内容
{
    int lenstr = strlen(str);
    int lenptrn = strlen(ptrn);
    int next[20];
    next[0] = 0;
    next[1] = 0;
    int a = 1;
    while (a < lenptrn - 1)
    {
        int b = next[a];
        while (b > 0 && ptrn[b] != ptrn[a])
            b = next[b];
        if (ptrn[b] == ptrn[a])
            next[a + 1] = b + 1;
        else
            next[a + 1] = b;
        a++;
    }
    int i = 0, j = 0;
    while (i < lenstr && j < lenptrn)
    {
        if (str[i] == ptrn[j])
        {
            i++, j++;
        }
        else
        {
            j = next[j];
            if (j == 0)
                i++;
        }
        if (j == lenptrn)
            return 1; // 匹配成功
    }
    return 0;
}
void relation(person &T) // 通过两个成员名字查询他们的关系的函数
{
    int i;
    char name1[20];
    char name2[20];
    person a, b, c, d;
    a = b = c = d = (person)malloc(sizeof(CSNode));
    start_up(a);
    start_up(b);
    start_up(c);
    start_up(d);
    printf("输入第一个人的名字:\n");
    scanf("%s", name1);
    fflush(stdin);
    a = find_name(T, name1);
    if (a == 0)
    {
        printf("该家谱无此人信息!\n");
        return;
    }
    printf("输入第二个人的名字:\n");
    scanf("%s", name2);
    fflush(stdin);
    if (strcmp(name1, name2) == 0)
    {
        printf("两者为同一人！\n");
        return;
    }
    b = find_name(T, name2);
    if (b == 0)
    {
        printf("该家谱无此人信息!\n");
        return;
    }
    if (a->data.Depth == b->data.Depth) // 若辈分相同判断是几代堂兄弟
    {
        c = a->parent;
        d = b->parent;
        if (strcmp(c->data.name, d->data.name) == 0)
            printf("两人为亲兄弟姐妹！\n");
        else
        {
            i = 1;
            c = c->parent;
            d = d->parent;
            while (c && d)
            {
                if (strcmp(c->data.name, d->data.name) == 0)
                {
                    printf("两人为%d代堂兄弟姐妹\n", i);
                    break;
                }

                else
                {
                    c = c->parent;
                    d = d->parent;
                    i++;
                }
            }
        }
    }
    if (a->data.Depth < b->data.Depth) // 若辈分不同判断是几代长辈是否是直系长辈
    {
        i = (a->data.Depth - b->data.Depth);
        printf("%s是%s的第%d代长辈,", a->data.name, b->data.name, -i);
        d = b->parent;
        while (d)
        {
            if (strcmp(a->data.name, d->data.name) == 0)
            {
                printf("是直系长辈！\n");
                break;
            }
            else
            {
                d = d->parent;
                if (!d)
                    printf("不是直系长辈！\n");
            }
        }
    }
    if (b->data.Depth < a->data.Depth) // 若辈分不同判断是几代长辈，是否是直系长辈
    {
        i = (b->data.Depth - a->data.Depth);
        printf("%s是%s的第%d代长辈,", b->data.name, a->data.name, -i);
        d = a->parent;
        while (d)
        {
            if (strcmp(b->data.name, d->data.name) == 0)
            {
                printf("是直系长辈！\n");
                break;
            }
            else
            {
                d = d->parent;
                if (!d)
                    printf("不是直系长辈！\n");
            }
        }
    }
    return;
}
void InitQueue(LinkQueue &Q)
{
    Q.front = Q.rear = (QNode *)malloc(sizeof(QNode));
    if (Q.front == NULL)
        exit(0);
    Q.front->next = NULL;
}

int QueueEmpty(LinkQueue Q)
{
    if (Q.rear == Q.front)
        return 1;
    else
        return 0;
}

void EnQueue(LinkQueue &Q, QElemType e)
{
    QNode *s;
    s = (QNode *)malloc(sizeof(QNode));
    s->data = e;
    Q.rear->next = s;
    Q.rear = s;
}

int DeQueue(LinkQueue &Q, QElemType &e)
{
    if (Q.front == Q.rear)
        e = NULL;
    else
    {
        QNode *p;
        p = Q.front->next;
        e = p->data;
        Q.front->next = p->next;
        if (Q.rear == p)
            Q.rear = Q.front;
        free(p);
    }
    return 1;
}
int main(void) // 主函数
{
    person T;
    T = (person)malloc(sizeof(CSNode));
    start_up(T);
    strcpy(T->data.name, "0"); // 首先设置第一个节点的成员姓名为0，用于家族谱的判空操作
    main_menu(T);
    return 0;
}