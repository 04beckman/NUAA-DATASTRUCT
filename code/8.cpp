#include <iostream>
#include <time.h>
#include <vector>
#include <stack>
#include <fstream>

using namespace std;
#define arrNumber 50000

int startTime;
int endTime;
int num = arrNumber;
int len = arrNumber;
int origin[arrNumber];        // 初始数组
int *a;                       // 临时数组 用于排序
void ShowSort(int info = -1); // info表示为第几个样本
void copyArr();               // 每种排序前的初始化
void StraightInsertionSort();
void ShellSort();
void SimpleSelectionSort();
void HeapSort();
void HeapMax(int start, int end); // 大根堆，维护堆的函数
void BubbleSort();
int NumberOfThree(int low, int high); // 快排辅助函数
void QuickSort(int left, int right);
int QuickSortNonRecursive(int low, int high); // 快排的非递归辅助函数
void MergeSort();                             // 2路归并非递归
void Merge(int low, int mid, int high);       // 归并过程
void MSort(int T_len);                        // 2路归并的非递归调用
void RadixSort();
int maxbit(); // 基数排序的辅助函数，用于求数据的最大位数

// 冒泡
void BubbleSort()
{
    startTime = clock();
    for (int i = 0; i < len - 1; i++)
    {
        for (int j = 0; j < len - i - 1; j++)
        {
            if (a[j] > a[j + 1])
            {
                int temp = a[j + 1];
                a[j] = a[j + 1];
                a[j + 1] = temp;
            }
        }
    }
    endTime = clock();
}
// 选择
void SimpleSelectionSort()
{
    startTime = clock();
    for (int i = 0; i < len - 1; i++)
    {
        int min = i;
        for (int j = i + 1; j < len; j++)
        {
            if (a[j] < a[min])
                min = j;
        }
        int temp = a[i];
        a[i] = a[min];
        a[min] = temp;
    }
    endTime = clock();
}
// 插入
void StraightInsertionSort()
{
    startTime = clock();
    for (int i = 1; i < len; i++)
    {
        int key = a[i];
        int j = i - 1;
        while ((j >= 0) && (key < a[j]))
        {
            a[j + 1] = a[j];
            j--;
        }
        a[j + 1] = key;
    }
    endTime = clock();
}
// 希尔
void ShellSort()
{
    startTime = clock();
    int h = 1;
    while (h < len / 3)
    {
        h = 3 * h + 1;
    }
    while (h >= 1)
    {
        for (int i = h; i < len; i++)
        {
            for (int j = i; j >= h && a[j] < a[j - h]; j -= h)
            {
                int temp = a[j];
                a[j] = a[j - h];
                a[j - h] = temp;
            }
        }
        h = h / 3;
    }
    endTime = clock();
}
// 归并
void Merge(int low, int mid, int high)
{
    int *T = new int[high - low + 1];
    int i = low, j = mid + 1, k = 0;
    while (i <= mid && j <= high)
    {
        if (a[i] <= a[j])
        {
            T[k] = a[i];
            i++;
            k++;
        }
        else
        {
            T[k] = a[j];
            j++;
            k++;
        }
    }
    while (i <= mid)
    {
        T[k] = a[j];
        i++;
        k++;
    }
    while (j <= high)
    {
        T[k] = a[j];
        j++;
        k++;
    }
    for (k = 0, i = low; i <= high; k++, i++)
        a[i] = T[k];
}
void MSort(int T_len)
{
    int i = 1;
    while (i + 2 * T_len <= len)
    {
        // 归并两个子序列
        Merge(i, i + T_len - 1, i + 2 * T_len - 1);
        i = i + 2 * T_len;
    }
    if (i + T_len <= len)
        Merge(i, i + T_len - 1, len);
}
void MergeSort()
{
    startTime = clock();
    for (int T_len = 1; T_len <= len; T_len = 2 * T_len)
        MSort(T_len);
    endTime = clock();
}
// 快排
int NumberOfThree(int low, int high)
{
    int mid = low + ((high - low) >> 1);

    if (a[mid] > a[high])
        swap(a[mid], a[high]);
    if (a[low] > a[high])
        swap(a[low], a[high]);
    if (a[mid] > a[low])
        swap(a[mid], a[low]);
    return a[low];
}
int QuickSortNonRecursive(int low, int high)
{
    int pivot = NumberOfThree(low, high);
    while (low < high)
    {
        while (low < high && a[high] >= pivot)
            --high;
        a[low] = a[high];
        while (low < high && a[low] <= pivot)
            ++low;
        a[high] = a[low];
    }
    a[low] = pivot;
    return low;
}
void QuickSort(int left, int right)
{
    startTime = clock();
    stack<int> s;
    if (left < right)
    {
        int boundary = QuickSortNonRecursive(left, right);

        if (boundary - 1 > left) // 确保左分区存在
        {
            // 将左分区端点入栈
            s.push(left);
            s.push(boundary - 1);
        }
        if (boundary + 1 < right) // 确保右分区存在
        {
            s.push(boundary + 1);
            s.push(right);
        }

        while (!s.empty())
        {
            // 得到某分区的左右边界
            int r = s.top();
            s.pop();
            int l = s.top();
            s.pop();

            boundary = QuickSortNonRecursive(l, r);
            if (boundary - 1 > l) // 确保左分区存在
            {
                // 将左分区端点入栈
                s.push(l);
                s.push(boundary - 1);
            }
            if (boundary + 1 < r) // 确保右分区存在
            {
                s.push(boundary + 1);
                s.push(r);
            }
        }
    }
    endTime = clock();
}
// 堆排序
void HeapMax(int start, int end)
{
    int parent = start;
    int child = parent * 2 + 1;
    // 建立堆
    while (child <= end)
    {
        if (child + 1 <= end && a[child] < a[child + 1])
            child++;
        if (a[parent] > a[child])
            return; // 维护完成
        else
        {
            swap(a[parent], a[child]);
            parent = child;
            child = parent * 2 + 1;
        }
    }
}
void HeapSort()
{
    int i;
    startTime = clock();
    for (i = len / 2 - 1; i >= 0; i--)
        HeapMax(i, len - 1);
    for (i = len - 1; i > 0; i--)
    {
        swap(a[0], a[i]);
        HeapMax(0, i - 1);
    }
    endTime = clock();
}
// 基数排序
int maxbit()
{
    int maxData = a[0];
    for (int i = 1; i < len; ++i)
        if (maxData < a[i])
            maxData = a[i];
    int d = 1;
    int p = 10;
    while (maxData >= p)
    {
        maxData /= 10;
        ++d;
    }
    return d;
}

void RadixSort()
{
    startTime = clock();
    int d = maxbit();
    int *tmp = new int[len];
    int *count = new int[10]; // 末位计数器
    int i, j, k;              // 循环变量
    int radix = 1;
    for (i = 1; i <= d; i++)
    {
        for (j = 0; j < 10; j++)
            count[j] = 0; // 每次分配前清空计数器
        for (j = 0; j < len; j++)
        {
            k = (a[j] / radix) % 10; // 统计每个桶中的记录数
            count[k]++;
        }
        for (j = 1; j < 10; j++)
            count[j] = count[j - 1] + count[j]; // 将tmp中的位置依次分配给每个桶
        for (j = len - 1; j >= 0; j--)
        { // 将所有桶中记录依次收集到tmp中
            k = (a[j] / radix) % 10;
            tmp[count[k] - 1] = a[j];
            count[k]--;
        }
        for (j = 0; j < len; j++) // 将临时数组的内容复制到data中
            a[j] = tmp[j];
        radix = radix * 10;
    }
    delete[] tmp;
    delete[] count;
    endTime = clock();
}

void copyArr()
{
    a = (int *)malloc(sizeof(int) * num);
    for (int i = 0; i < len; i++)
        a[i] = origin[i];
}
void ShowSort(int info)
{
    cout << "第" << info << "个样本的排序用时如下:" << endl;

    copyArr();
    BubbleSort();
    free(a);
    cout << "该样本冒泡排序用时：" << endTime - startTime << " ms" << endl;

    copyArr();
    SimpleSelectionSort();
    free(a);
    cout << "该样本选择排序用时：" << endTime - startTime << " ms" << endl;

    copyArr();
    StraightInsertionSort();
    free(a);
    cout << "该样本插入排序用时：" << endTime - startTime << " ms" << endl;

    copyArr();
    ShellSort();
    free(a);
    cout << "该样本希尔排序用时：" << endTime - startTime << " ms" << endl;

    copyArr();
    MergeSort();
    free(a);
    cout << "该样本归并排序用时：" << endTime - startTime << " ms" << endl;

    copyArr();
    HeapSort();
    free(a);
    cout << "该样本堆排序用时：" << endTime - startTime << " ms" << endl;

    copyArr();
    QuickSort(0, 49999);
    free(a);
    cout << "该样本快速排序用时：" << endTime - startTime << " ms" << endl;

    copyArr();
    RadixSort();
    cout << "该样本基数排序用时：" << endTime - startTime << " ms" << endl;
}

/* run this program using the console pauser or add your own getch, system("pause") or input loop */
// 随机创建样本
void CreateRandomTxt()
{
    srand((int)time(0));
    int i = 1;
    ofstream out;
    out.open("input.txt");
    if (!out)
    {
        cout << "打开文件失败!";
        exit(1);
    }
    for (i; i < 50001; i++)
        out << rand() % 50000 + 1 << ' ';
    out.close();
}
// 创建升序样本
void CreateOrderTxtp()
{
    int i = 1;
    ofstream out;
    out.open("input.txt");
    if (!out)
    {
        cout << "打开文件失败!";
        exit(1);
    }
    for (i; i < 50001; i++)
        out << i << ' ';
    cout << "创建升序样本完成" << endl;
    out.close();
}

// 创建降序样本
void CreateOrderTxtn()
{
    int i = 50000;
    ofstream out;
    out.open("input.txt");
    if (!out)
    {
        cout << "打开文件失败!";
        exit(1);
    }
    for (i; i > 0; i--)
        out << i << ' ';
    cout << "创建降序样本完成" << endl;
    out.close();
}
// 将样本输入数组
void Inarr(int a[])
{
    int i = 1;
    ifstream in;
    in.open("input.txt");
    if (!in)
    {
        cout << "打开样本文件失败!";
        exit(0);
    }
    for (i; i <= 50000; i++)
        in >> a[i];
    in.close();
}
int main(void)
{
    cout << "第一个样本（升序）排序如下" << endl;
    CreateOrderTxtp();
    Inarr(origin);
    ShowSort(1);
    system("pause");
    cout << endl;
    cout << "第二个样本（降序）排序如下" << endl;
    CreateOrderTxtn();
    Inarr(origin);
    ShowSort(2);
    system("pause");
    cout << endl;
    for (int i = 3; i <= 10; i++)
    {
        CreateRandomTxt();
        Inarr(origin);
        ShowSort(i);
        system("pause");
        cout << endl;
    }
    return 0;
}
