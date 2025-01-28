#include <windows.h>
#include <stdio.h>
#include <iomanip>
#include <sstream>
#define OK 1
#define ERROR 0
#define MAXSIZE 100
#define TIME 1000
#define white SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE) // 白色
#define red SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_RED);
typedef struct DLNode
// 存储当前进程  单向链表
{
    int PID;
    int PMemory;
    char PName[MAXSIZE];
    DLNode *next;
} DLNode, *DLinkList;

typedef struct SLNode
// 存储已结束进程 双向链表
{
    int PID;
    char PName[MAXSIZE];
    int endT;      // 进程结束时间
    int durationT; // 进程持续时间
    SLNode *next;
    SLNode *pre;
} SLNode, *SLinkList;
void InitDL(DLinkList &L)
{
    L = (DLinkList)malloc(sizeof(DLNode));
    L->PID = 0;
    L->PMemory = 0;
    L->next = NULL;
}

void InitSL(SLinkList &L)
{
    L = (SLinkList)malloc(sizeof(SLNode));
    L->endT = 0;
    L->durationT = 0;
    L->PID = 0;
    L->pre = NULL;
    L->next = NULL;
}

void Clear()
{
    FILE *fp;
    if ((fp = fopen("systemdata_1.txt", "w")) == NULL)
    {
        printf("打开文件失败...\n");
        exit(0);
    }
    fclose(fp);
}

int WriteSystemData()
// 将系统进程写入systemdata_1.txt中
{
    Clear();
    if (system("tasklist /FO CSV >> systemdata_1.txt"))
        return OK;
    else
        return ERROR;
}

SLinkList LocateOverInSL(SLinkList L, int PID)
// 在存储已结束进程的SLinkList L中查找名称为PID的已结束进程
// 若查找成功 则返回该进程在L中的位置 否则返回空
{
    SLinkList p = L->next;
    while (p)
    {
        if (p->PID == PID)
        {
            return p;
        }
        else
            p = p->next;
    }
    return NULL;
}

int ListLengthDL(DLinkList L)
// 返回元素个数
{
    DLinkList p;
    p = L->next;
    int count = 0; // count用于标记元素个数
    while (p)
    {
        p = p->next;
        count++;
    }
    return count;
}
void Sort_DLinkList(DLinkList &L)
// 对DLinkList L使用冒泡排序使其元素按内存从大到小排列
{
    int i, j;
    int sum = ListLengthDL(L);
    DLinkList p, q = (DLinkList)malloc(sizeof(DLNode));
    for (i = 1; i <= sum - 1; i++)
    {
        for (p = L->next, j = 1; j <= sum - i - 1 && p->next != NULL; p = p->next, j++)
        {
            if (p->PMemory < p->next->PMemory) // 交换 p与p->next
            {
                q->PID = p->PID;
                q->PMemory = p->PMemory;
                strcpy(q->PName, p->PName); //
                p->PID = p->next->PID;
                p->PMemory = p->next->PMemory;
                strcpy(p->PName, p->next->PName); //
                p->next->PID = q->PID;
                p->next->PMemory = q->PMemory;
                strcpy(p->next->PName, q->PName);
            }
        }
    }
}

int ListLengthSL(SLinkList L)
// 返回元素个数
{
    SLinkList p;
    p = L->next;
    int count = 0; // count用于标记元素个数
    while (p)
    {
        p = p->next;
        count++;
    }
    return count;
}
void Sort_SLinkList(SLinkList &L)
// 对已结束进程存储链表SLinkList L按结束时间进行从前到后的排序
{
    int i, j;
    int length = ListLengthSL(L);
    SLinkList p, q = (SLinkList)malloc(sizeof(SLNode));
    for (i = 1; i <= length - 1; i++)
    {
        for (p = L->next, j = 1; j <= length - i - 1; p = p->next, j++)
        {
            if (p->durationT < p->next->durationT)
            {
                q->PID = p->PID;
                q->durationT = p->durationT;
                q->endT = p->endT;
                strcpy(q->PName, p->PName); //
                p->PID = p->next->PID;
                p->durationT = p->next->durationT;
                p->endT = p->next->endT;
                strcpy(p->PName, p->next->PName); //
                p->next->PID = q->PID;
                p->next->durationT = q->durationT;
                p->next->endT = q->endT;
                strcpy(p->next->PName, q->PName);
            }
        }
    }
}
void Show_DLinkListOriginal(DLinkList DL, SLinkList SL)
// 每秒在窗口内更新一次当前系统进程情况，输出内容包括：进程名，持续时间，内存使用情况。
{
    SLinkList p;
    p = SL->next;
    DLinkList q;
    q = DL->next;
    int numDLO = 0;
    while (q)
    {
        p = LocateOverInSL(SL, q->PID);
        if (p)
        // if(p && p->durationT!=0)
        {
            numDLO++;
            printf("%d.PName: ", numDLO);
            for (int i = 1; i < strlen(q->PName); i++)
            {
                printf("%c", q->PName[i]);
            }
            printf("\t\nPDuration:%d\tPMemory:%d\n\n", p->durationT, q->PMemory);
        }
        q = q->next;
    }
}
void Show_DLinkList(DLinkList DL, SLinkList SL)
// 每秒在窗口内更新一次当前系统进程情况，输出内容包括：进程名，持续时间，内存使用情况。
{
    SLinkList p;
    p = SL->next;
    DLinkList q;
    q = DL->next;
    int numDL = 0;
    while (q)
    {
        p = LocateOverInSL(SL, q->PID);
        if (p && p->durationT != 0)
        {
            numDL++;
            printf("%d.PName: ", numDL);
            for (int i = 1; i < strlen(q->PName); i++)
            {
                printf("%c", q->PName[i]);
            }
            printf("\t\nPDuration:%d\tPMemory:%d\n\n", p->durationT, q->PMemory);
        }
        q = q->next;
    }
}
void Show_SLinkList(SLinkList L)
// 每秒在窗口内更新一次已结束进程情况，输出内容包括：进程名，持续时间，结束时间。
{
    SLinkList s;
    s = L->next;
    int numSL = 0;
    while (s)
    {
        if (s->endT != 0)
        {
            numSL = numSL + 1;
            printf("%d.PName: ", numSL);
            for (int i = 1; i < strlen(s->PName); i++)
            {
                printf("%c", s->PName[i]);
            }
            printf("\t\nPDuration:%d\tPendT:%d\n\n", s->durationT, s->endT);
        }
        s = s->next;
    }
}
int Str_Int(char *elem, int n)
{
    int temp = 0;
    for (int i = 1; i <= n; i++)
    {
        if (elem[i] == ',')
            i = i + 1;
        temp = temp * 10 + (elem[i] - 48);
    }
    return temp;
}
int CreateList(DLinkList &L)
{
    WriteSystemData();
    char buffer[1024], buffer1[1024], buffer2[1024];
    char str[100];
    int i, j, k, flag = 0; // flag用于标记遇到了几次引号
    DLinkList p = L, q;
    FILE *fp;
    if ((fp = fopen("systemdata_1.txt", "r")) == NULL)
    {
        printf("\nCannot open the file systamdata_1.txt...\n");
        exit(0);
    }
    fgets(buffer, 1024, (FILE *)fp);
    while (1)
    {
        q = (DLinkList)malloc(sizeof(DLNode));
        flag = 0;
        fgets(buffer, 1024, (FILE *)fp);
        if (strcmp(buffer1, buffer) == 0)
            break;
        strcpy(buffer1, buffer); // buffer1存储上次读取的数据
        for (i = 0; i < strlen(buffer); i++)
        {
            if (buffer[i] == '"')
            {
                flag = flag + 1;
                if (flag % 2 == 1) // 是一个需要读取的字符串第一个引号
                {
                    for (j = i + 1, k = 1; buffer[j] != '"'; j++, k++)
                    {
                        if (buffer[j] == ' ' && buffer[j + 1] == ' ')
                            break;
                        if (buffer[j] != ' ' && buffer[j + 1] != 'K')
                            str[k] = buffer[j];
                    }
                    str[k] = '\0';
                    if (flag == 1) // str中存的是PName
                    {
                        strcpy(q->PName, str);
                    }
                    else if (flag == 3) // str中存的是PID
                    {
                        q->PID = Str_Int(str, k - 1);
                    }
                    else if (flag == 9) // str中存的是PMemory
                    {
                        q->PMemory = Str_Int(str, k - 1);
                    }
                }
            }
        }
        p->next = q;
        q->next = NULL;
        p = q;
    }
    fclose(fp);
    return OK;
}
int StartProgram(DLinkList &L, DLinkList currentL, SLinkList &S)
// 统计新进程
{
    int judge = 1, i = 1;
    SLinkList p_S = S->next, q_S = S->next, temp_S = (SLinkList)malloc(sizeof(SLNode));
    DLinkList p_L = L->next, p_currentL = currentL->next, q_L = L->next, temp_L = (DLinkList)malloc(sizeof(DLNode));
    while (p_currentL) // 遍历当前进程表
    {
        i = 1;
        judge = 1;
        p_L = L->next; // p_L指向L的第一个元素
        while (p_L)
        {
            if (p_currentL->PID == p_L->PID)
            // 该进程已在进程表L中
            {
                judge = 0;
                break;
            }
            p_L = p_L->next;
        }
        p_S = S->next;
        while (p_S && i <= S->PID)
        {
            if (strcmp(p_currentL->PName, p_S->PName) == 0) // 进程已结束
            {
                p_S->endT = 0; // 重新启用进程
                break;
            }
            i++;
            p_S = p_S->next;
        }
        if (judge) // 进程不在L中 则将其添加在L，S中
        {
            while (q_L->next)
                q_L = q_L->next;
            temp_L = (DLinkList)malloc(sizeof(DLNode));
            temp_L->PID = p_currentL->PID;
            temp_L->PMemory = p_currentL->PMemory;
            strcpy(temp_L->PName, p_currentL->PName);
            q_L->next = temp_L;
            temp_L->next = NULL;
            L->PMemory++;
            while (q_S->next)
                q_S = q_S->next;
            temp_S = (SLinkList)malloc(sizeof(SLNode));
            temp_S->PID = p_currentL->PID;
            temp_S->durationT = 0;
            temp_S->endT = 0;
            strcpy(temp_S->PName, p_currentL->PName);
            q_S->next = temp_S;
            temp_S->next = NULL;
            S->endT++;
        }
        p_currentL = p_currentL->next;
    }
    return OK;
}
int CreateOverList(SLinkList &S, DLinkList L)
// 创建已结束进程链表
{
    S = (SLinkList)malloc(sizeof(SLNode));
    S->PID = 0;
    S->durationT = 0;
    S->endT = L->PMemory;
    DLinkList p_L = L->next;
    SLinkList p_S = S, q_S = (SLinkList)malloc(sizeof(SLNode));
    while (p_L)
    {
        q_S = (SLinkList)malloc(sizeof(SLNode));
        strcpy(q_S->PName, p_L->PName);
        q_S->durationT = 0;
        q_S->endT = 0;
        q_S->PID = p_L->PID;
        p_S->next = q_S;
        q_S->pre = p_S;
        p_S = q_S;
        p_L = p_L->next;
    }
    q_S->next = NULL;
    return OK;
}
int OverTime(DLinkList L, DLinkList currentL, SLinkList &S)
// 统计结束进程
{
    int judge = 1;
    SLinkList p_S = S->next;
    DLinkList p_L = L->next, p_currentL = currentL->next;
    while (p_L)
    {
        judge = 1;
        // p_currentL=currentL->next;
        while (p_currentL)
        {
            if (p_currentL->PID == p_L->PID)
            // PID是各进程的代号，每个进程有唯一的PID编号。
            // 该进程未结束
            {
                judge = 0;
                break;
            }
            p_currentL = p_currentL->next;
        }
        if (judge)
        {
            p_S = LocateOverInSL(S, p_L->PID);
            if (p_S->endT == 0)
            // 该进程为新结束的进程
            {
                p_S->durationT += TIME / 1000;
                p_S->endT = S->durationT;
                S->PID++;
            }
        }
        else
        {
            p_S = LocateOverInSL(S, p_L->PID);
            p_S->durationT += TIME / 1000;
        }
        p_L = p_L->next;
        p_currentL = currentL->next;
    }
    return OK;
}
int GetData()
{
    FILE *fp;
    char cmd[1024];
    char buf[1024];
    char result[4096];
    sprintf(cmd, "systemdata_1.txt");
    if ((fp = popen(cmd, "r")) != NULL)
    {
        while (fgets(buf, 1024, fp) != NULL)
        {
            strcat(result, buf);
        }
        pclose(fp);
        fp = NULL;
    }
    // printf("%s",result);
    return OK;
}
int main(void)
{
    // WriteSystemData();
    DLinkList L, currentL; // L总进程表 currentL当前进程
    InitDL(L);
    InitDL(currentL);
    SLinkList S; // 已结束进程
    InitSL(S);
    if (CreateList(L))
        printf("L创建成功\n");
    CreateOverList(S, L);         // 创建结束进程表
    Sort_DLinkList(L);            // 排序
    Show_DLinkListOriginal(L, S); // 打印总进程
    printf("\n初始链表显示完毕\n");
    int num = 0;
    while (1)
    {
        num = num + 1;
        Sleep(TIME); // 每过TIME/1000s获取一次当前进程
        S->durationT += TIME / 1000;
        // 总持续时间增加
        CreateList(currentL);
        // 获取当前进程表
        Sort_DLinkList(currentL); // 排序
        OverTime(L, currentL, S); // 调整结束时间及结束进程
        StartProgram(L, currentL, S);
        // 调整重新调用的程序及新调用的程序
        red;
        printf("*当前运行程序*：\n");
        white;
        Show_DLinkList(currentL, S);
        red;
        printf("\n*第%d次显示当前进程完毕*\n", num);
        printf("已结束进程：\n");
        white;
        Sort_SLinkList(S);
        Show_SLinkList(S);
        red;
        printf("\n*第%d次显示结束进程完毕*\n", num);
        white;
        system("pause");
    }
    system("pause");
    return 0;
}
