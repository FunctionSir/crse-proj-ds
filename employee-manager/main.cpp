#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <map>
#include <string>
#define ERROR 0
#define OK 1
#define INIT_SIZE 5 /*初始分配的顺序表长度*/
#define INCREM 5
using namespace std;
int vis[20]; // 查找时记录序号
char s[20];
int pivot;
map<string, int> degreerank;
void clear_screen() {
    system("cls");
}
typedef struct T {
    char eno[20];
    char name[20];
    char sex[20];
    char date[20];
    char degree[20];
    char job[20];
    char phonenumber[20];
    char address[20];
} ElemType; /*定义表元素的类型*/
typedef struct Sqlist {
    ElemType *slist; /*存储空间的基地址*/
    int length;      /*顺序表的当前长度*/
    int listsize;    /*当前分配的存储空间*/
} Sqlist;
ElemType shuniu;            // 快排枢纽
int InitList_sq(Sqlist *L); /*   初始化顺序表                          */
int CreateList_sq(Sqlist *L, int n); /*创建顺序表，边输入边调用插入函数 */
int ListInsert_sq(
    Sqlist *L, int i,
    ElemType e); /*将e插入第i个位置之前,即是实际表中i位置（插入员工信息的函数）
                  */
int PrintList_sq(Sqlist *L);                   /*输出顺序表的元素*/
int ListDelete_sq(Sqlist *L, char num[20]);    /*删除员工信息的函数*/
void ListLocate(Sqlist *L, int t);             /*查询员工信息的函数*/
void Listupdate(Sqlist *L, int x, char y[20]); /*更新员工信息的函数*/
void Listsort(Sqlist *L, int choice, int i, int j); /*员工信息排序*/
int Partition(Sqlist *L, int choice, int i, int j); /*分割算法*/
void clear_screen();                                /*清空屏幕*/

int InitList_sq(Sqlist *L) { /*初始化*/
    L->slist = (ElemType *)malloc(INIT_SIZE * sizeof(ElemType));
    if (!L->slist)
        return ERROR;
    L->length = 0;
    L->listsize = INIT_SIZE;
    return OK;
} /*InitList*/

int CreateList_sq(Sqlist *L, int n) { /*创建顺序表*/
    ElemType e;
    int i;
    for (i = 0; i < n; i++) {
        printf("\n输入员工相关数据\n1.编号\n2.姓名\n3.性别\n4.出生年月\n5."
               "学历\n6.职位\n7.电话号码\n8.地址\n请输入第%d个人的相关数据\n",
               i + 1);
        scanf("%s %s %s %s %s %s %s %s", e.eno, e.name, e.sex, e.date, e.degree,
              e.job, e.phonenumber, e.address); // 输入员工相关信息
        if (!ListInsert_sq(L, i + 1, e))
            return ERROR;
    }
    return OK;
} /*CreateList*/

/*输出顺序表中的元素,输出员工信息*/
int PrintList_sq(Sqlist *L) {
    int i;
    for (i = 1; i <= L->length; i++)
        printf("\n在顺序表中的位置:%d\n编号:%s\n姓名:%s\n性别:%s\n出生年月:%"
               "s\n学历:%s\n职务:%s\n电话:%s\n地址:%s\n",
               i, L->slist[i - 1].eno, L->slist[i - 1].name,
               L->slist[i - 1].sex, L->slist[i - 1].date,
               L->slist[i - 1].degree, L->slist[i - 1].job,
               L->slist[i - 1].phonenumber, L->slist[i - 1].address);
    printf("\n共计有%d个人的数据", L->length);
    return OK;
} /*PrintList*/
/*插入员工信息*/
int ListInsert_sq(Sqlist *L, int i, ElemType e) {
    int k;
    if (i < 1 || i > L->length + 1) // 溢出数据
    {
        printf("\n插入失败\n");
        return ERROR;
    }
    if (L->length >= L->listsize) {
        L->slist = (ElemType *)realloc(L->slist,
                                       (INIT_SIZE + INCREM) *
                                           sizeof(ElemType)); // 扩大空间
        if (!L->slist)
            return ERROR;
        L->listsize += INCREM;
    }
    for (k = L->length - 1; k >= i - 1;
         k--) // 插入员工数据到顺序表中i-1位置，实际表中i位置
    {
        L->slist[k + 1] = L->slist[k];
    }
    L->slist[i - 1] = e;
    L->length++;
    return OK;
} /*ListInsert*/

/*删除员工信息,在顺序表中删除第i个元素*/
int ListDelete_sq(Sqlist *L, char num[20]) {
    int j;
    int i;
    for (i = 0; i < L->length; i++) {
        if (strcmp(L->slist[i].eno, num) == 0) {
            j = i + 1;
            break;
        }
    }
    if (j < 1 || j > L->length) {
        printf("\n删除失败!!!\n");
        return ERROR;
    }
    for (; j < L->length; j++) {
        L->slist[j - 1] = L->slist[j];
    }
    L->length--;
    printf("\n删除成功\n");
    return 1;
}
/*查询员工信息*/
void ListLocate(Sqlist *L, int t) {
    ElemType *p;
    int i;
    p = L->slist;
    i = 1;
    switch (t) {
    case 1:
        while (i <= L->length) {
            if (strcmp((*p++).eno, s) != 0)
                i++;
            else {
                vis[i]++;
                i++;
            }
        }
        break;
    case 2:
        while (i <= L->length) {
            if (strcmp((*p++).name, s) != 0)
                i++;
            else {
                vis[i]++;
                i++;
            }
        }
        break;
    case 3:
        while (i <= L->length) {
            if (strcmp((*p++).sex, s) != 0)
                i++;
            else {
                vis[i]++;
                i++;
            }
        }
        break;
    case 4:
        while (i <= L->length) {
            if (strcmp((*p++).date, s) != 0)
                i++;
            else {
                vis[i]++;
                i++;
            }
        }
        break;
    case 5:
        while (i <= L->length) {
            if (strcmp((*p++).degree, s) != 0)
                i++;
            else {
                vis[i]++;
                i++;
            }
        }
        break;
    case 6:
        while (i <= L->length) {
            if (strcmp((*p++).job, s) != 0)
                i++;
            else {
                vis[i]++;
                i++;
            }
        }
        break;
    case 7:
        while (i <= L->length) {
            if (strcmp((*p++).phonenumber, s) != 0)
                i++;
            else {
                vis[i]++;
                i++;
            }
        }
        break;
    case 8:
        while (i <= L->length) {
            if (strcmp((*p++).address, s) != 0)
                i++;
            else {
                vis[i]++;
                i++;
            }
        }
        break;
    }
}
void Listupdate(Sqlist *L, int x, char y[20]) /*跟新员工信息*/
{
    ElemType *p;
    int i;
    p = L->slist;
    while (i <= L->length) {
        i++;
        if (strcmp((*p++).eno, y) == 0) {
            switch (x) {
            case 1:
                strcpy((*--p).eno, s);
                break;
            case 2:
                strcpy((*--p).name, s);
                break;
            case 3:
                strcpy((*--p).sex, s);
                break;
            case 4:
                strcpy((*--p).date, s);
                break;
            case 5:
                strcpy((*--p).degree, s);
                break;
            case 6:
                strcpy((*--p).job, s);
                break;
            case 7:
                strcpy((*--p).phonenumber, s);
                break;
            case 8:
                strcpy((*--p).address, s);
                break;
            }
            p++;
            printf("\n更新成功\n");
            return;
        }
    }
    printf("\n更新失败!!!\n");
}
int Partition(Sqlist *L, int choice, int i, int j) {
    shuniu = L->slist[i];
    switch (choice) {
    case 1: {
        int pivotkey = atoi(L->slist[i].eno);
        while (i < j && atoi(L->slist[j].eno) >= pivotkey)
            --j;
        L->slist[i] = L->slist[j];
        while (i < j && atoi(L->slist[i].eno) <= pivotkey)
            ++i;
        L->slist[j] = L->slist[i];
    } break;
    case 2: {
        char pivotkey[20];
        strcpy(pivotkey, L->slist[i].name);
        while (i < j && strcmp(L->slist[j].name, pivotkey) >= 0)
            --j;
        L->slist[i] = L->slist[j];
        while (i < j && strcmp(L->slist[i].name, pivotkey) <= 0)
            ++i;
        L->slist[j] = L->slist[i];
    } break;
    case 3: {
        char pivotkey[20];
        strcpy(pivotkey, L->slist[i].date);
        while (i < j && strcmp(L->slist[j].date, pivotkey) >= 0)
            --j;
        L->slist[i] = L->slist[j];
        while (i < j && strcmp(L->slist[i].date, pivotkey) <= 0)
            ++i;
        L->slist[j] = L->slist[i];
    } break;
    case 4: {
        int pivotkey = degreerank[L->slist[i].degree];
        while (i < j && degreerank[L->slist[j].degree] >= pivotkey)
            --j;
        L->slist[i] = L->slist[j];
        while (i < j && degreerank[L->slist[i].degree] <= pivotkey)
            ++i;
        L->slist[j] = L->slist[i];
    } break;
    case 5: {
        int pivotkey = atoi(L->slist[i].eno);
        while (i < j && atoi(L->slist[j].eno) <= pivotkey)
            --j;
        L->slist[i] = L->slist[j];
        while (i < j && atoi(L->slist[i].eno) >= pivotkey)
            ++i;
        L->slist[j] = L->slist[i];
    } break;
    case 6: {
        char pivotkey[20];
        strcpy(pivotkey, L->slist[i].name);
        while (i < j && strcmp(L->slist[j].name, pivotkey) <= 0)
            --j;
        L->slist[i] = L->slist[j];
        while (i < j && strcmp(L->slist[i].name, pivotkey) >= 0)
            ++i;
        L->slist[j] = L->slist[i];
    } break;
    case 7: {
        char pivotkey[20];
        strcpy(pivotkey, L->slist[i].date);
        while (i < j && strcmp(L->slist[j].date, pivotkey) <= 0)
            --j;
        L->slist[i] = L->slist[j];
        while (i < j && strcmp(L->slist[i].date, pivotkey) >= 0)
            ++i;
        L->slist[j] = L->slist[i];
    } break;
    case 8: {
        int pivotkey = degreerank[L->slist[i].degree];
        while (i < j && degreerank[L->slist[j].degree] <= pivotkey)
            --j;
        L->slist[i] = L->slist[j];
        while (i < j && degreerank[L->slist[i].degree] >= pivotkey)
            ++i;
        L->slist[j] = L->slist[i];
    } break;
    }
    L->slist[i] = shuniu;
    return i;
}
void Listsort(Sqlist *L, int choice, int i, int j) {
    if (i < j) {
        pivot = Partition(L, choice, i, j);
        Listsort(L, choice, i, pivot - 1);
        Listsort(L, choice, pivot + 1, j);
    }
}
int main() {
    degreerank["无"] = 0;
    degreerank["幼儿园"] = 1;
    degreerank["小学"] = 2;
    degreerank["初中"] = 3;
    degreerank["高中"] = 4;
    degreerank["大专"] = 5;
    degreerank["本科"] = 6;
    degreerank["研究生"] = 7;
    degreerank["硕士"] = 8;
    degreerank["博士"] = 9;
    Sqlist sl;
    int n;
    printf("欢迎来到员工管理系统\n");
    printf("请输入员工人数 n:"); /*输入顺序表的元素个数*/
    scanf("%d", &n);
    char opt[20];
    int condition[20];
    if (n > 0) {
        printf("\n-创建线性表:\n");
        InitList_sq(&sl); // 初始化
        CreateList_sq(&sl, n);
        while (1) {
            printf("\n请选择你要执行的操作:\nA-查询员工信息\nB-更新员工信息\nC-"
                   "插入员工信息\nD-删除员工信息\nE-排序\nF-打印员工表\nG-"
                   "退出\nH-清屏\n");
            scanf("%s", opt);
            int cnt = 0;
            if (opt[0] == 'G') {
                printf("成功退出系统\n");
                return 0;
            }
            switch (opt[0]) {
            case 'A': {
                memset(vis, 0, sizeof(vis));
                printf("\nA-查询员工信息:\n1-依照编号查询\n2-依照姓名查询\n3-"
                       "依照性别\n4-依照出生年月\n5-依照学历\n6-依照职务\n7-"
                       "依照电话\n8-依照住址\n");
                printf("\n请选择你的条件数:\n");
                scanf("%d", &cnt);
                printf("\n请输入你要依据的条件:\n");
                for (int i = 1; i <= cnt; i++) {
                    printf("\n请输入你选择的条件\n");
                    scanf("%d", &condition[i]);
                    printf("\n请输入条件的信息\n");
                    scanf("%s", s);
                    ListLocate(&sl, condition[i]);
                }
                int ans = 0;
                printf("\n查询结果\n");
                for (int i = 1; i <= sl.length; i++) {
                    if (vis[i] == cnt) {
                        ans++;
                        printf("\n在顺序表中的位置:%d\n编号:%s\n姓名:%s\n性别:%"
                               "s\n出生年月:%s\n学历:%s\n职务:%s\n电话:%"
                               "s\n地址:%s\n",
                               i, sl.slist[i - 1].eno, sl.slist[i - 1].name,
                               sl.slist[i - 1].sex, sl.slist[i - 1].date,
                               sl.slist[i - 1].degree, sl.slist[i - 1].job,
                               sl.slist[i - 1].phonenumber,
                               sl.slist[i - 1].address);
                    }
                }
                if (ans == 0) {
                    printf("\n查询失败!!!(条件有误)\n");
                } else {
                    printf("\n共计查询出符合条件的对象数:%d\n", ans);
                }
            } break;
            case 'B': {
                printf("\nB-更新员工信息\n");
                printf("\n请输入你要更新的员工的编号:\n");
                char num[20];
                scanf(" %s", num);
                printf("\n请输入你要更新的信息的类型:\n1-编号\n2-姓名\n3-"
                       "性别\n4-出生年月\n5-学历\n6-职务\n7-电话\n8-住址\n");
                int type;
                scanf("%d", &type);
                printf("\n请输入你要更新的信息:\n");
                scanf(" %s", s);
                Listupdate(&sl, type, num);
            } break;
            case 'C': {
                printf("\nC-插入员工信息\n");
                printf("\n请输入插入位置:\n");
                int site;
                scanf("%d", &site);
                printf("\n请输入插入的员工的信息\n");
                ElemType e;
                scanf("%s %s %s %s %s %s %s %s", e.eno, e.name, e.sex, e.date,
                      e.degree, e.job, e.phonenumber, e.address);
                if (ListInsert_sq(&sl, site, e) != 0)
                    printf("\n插入成功\n");
            } break;
            case 'D': {
                printf("\nD-删除员工信息\n");
                printf("\n请输入被删除的员工的编号:\n");
                char num[20];
                scanf(" %s", num);
                ListDelete_sq(&sl, num);
            } break;
            case 'E': {
                printf("\nE-员工信息排序\n");
                printf("请选择升序还是降序(a-升序，b-降序)\n");
                char xy[10];
                scanf(" %s", xy);
                if (xy[0] == 'a')
                    printf("\n请输入排序的关键字:\n1-编号(升序)\n2-姓名("
                           "只能排英文名,字典序)(升序)\n3-出生年月(标准格式:"
                           "XXXXYY,X为年,Y为月)(升序)\n4-学历(升序)\n");
                else
                    printf("\n5-编号(降序)\n6-姓名(只能排英文名,字典序)(降序)"
                           "\n7-出生年月(标准格式:XXXXYY,X为年,Y为月)(降序)\n8-"
                           "学历(降序)\n");
                int choice;
                scanf("%d", &choice);
                if (choice < 1 && choice > 4) {
                    printf("\n排序出错!!!\n");
                    break;
                }
                switch (choice) {
                case 1:
                    Listsort(&sl, choice, 0, sl.length - 1);
                    break;
                case 2:
                    Listsort(&sl, choice, 0, sl.length - 1);
                    break;
                case 3:
                    Listsort(&sl, choice, 0, sl.length - 1);
                    break;
                case 4:
                    Listsort(&sl, choice, 0, sl.length - 1);
                    break;
                case 5:
                    Listsort(&sl, choice, 0, sl.length - 1);
                    break;
                case 6:
                    Listsort(&sl, choice, 0, sl.length - 1);
                    break;
                case 7:
                    Listsort(&sl, choice, 0, sl.length - 1);
                    break;
                case 8:
                    Listsort(&sl, choice, 0, sl.length - 1);
                    break;
                }
                printf("\n排序成功\n");
            } break;
            case 'F': {
                PrintList_sq(&sl);
                printf("\n输出成功\n");
            } break;
            case 'H': {
                clear_screen();
            }
            }
        }
    } else
        printf("ERROR");
    return 0;
}
