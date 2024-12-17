/*
 * @Author: FunctionSir
 * @License: AGPLv3
 * @Date: 2024-12-16 15:09:56
 * @LastEditTime: 2024-12-17 09:10:55
 * @LastEditors: FunctionSir
 * @Description: 哈希表演示
 * @FilePath: /crse-proj-ds/hash-table/main.c
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
Modify this to change conflict handling method.
Can be one of "CHAINING", "LINEAR" or "SQUARE".
Note: CHAINING is the best.
Warn: Method LINEAR or SQUARE might cause failure when inserting!
If use the "make.py", don't set it.
*/
// #define CHAINING

/*
Modify this to change hash function.
Can be one of "BASE26", "SUM" or "ASCII".
Note: BASE26 is the best.
Warn: Function SUM or ASCII might cause HUGE performance lose!
If use the "make.py", don't set it.
*/
// #define BASE26

#ifdef BASE26
const int BASE = 26;
#endif

#ifdef CHAINING
#ifdef BASE26
const int MOD = 10000849;
#endif
#ifdef ASCII
const int MOD = 10007;
#endif
#ifdef SUM
const int MOD = 1009;
#endif
const char *TITLE = "哈希表演示程序(拉链法)";
#else
#ifdef BASE26
const int MOD = 1000003;
#endif
#ifdef ASCII
const int MOD = 10007;
#endif
#ifdef SUM
const int MOD = 1009;
#endif
const size_t TABLE_LEN = MOD * 4;
#endif

#ifdef SQUARE
const char *TITLE = "哈希表演示程序(二次探测再散列)";
#endif

#ifdef LINEAR
const char *TITLE = "哈希表演示程序(线性探测再散列)";
#endif

const int SUCCESS = 1;
const int FAILURE = 0;

typedef char Name[32];

#ifdef CHAINING
typedef struct Node {
    Name name;
    struct Node *next;
} Node;
#endif

#ifndef ASCII
int convert(char ch) {
    if (ch >= 'a' && ch <= 'z') { // Is lowercase.
        ch -= 32;
    }
    if (ch < 'A' || ch > 'Z') {
        return 0;
    }
    return ch - 'A';
}
#endif

// It convert a string to a base 26 number,
// and mod a large prime "MOD".
#ifdef BASE26
int hash(Name name) {
    int result = 0, tmp = 1;
    for (size_t i = 0; i < strlen(name); i++) {
        result = (convert(name[i]) * tmp % MOD + result) % MOD;
        tmp = tmp * BASE % MOD;
    }
    return result;
}
#endif

#ifdef SUM
int hash(Name name) {
    int result = 0;
    for (size_t i = 0; i < strlen(name); i++) {
        result = (convert(name[i]) + result) % MOD;
    }
    return result;
}
#endif

#ifdef ASCII
int hash(Name name) {
    int result = 0;
    for (size_t i = 0; i < strlen(name); i++) {
        result = (name[i] + result) % MOD;
    }
    return result;
}
#endif

bool str_eq(Name a, Name b) {
    return strcmp(a, b) == 0;
}

// Impl "insert" and "find" for method "chaining" //
#ifdef CHAINING
int insert(Node *t, Name key) {
    int h = hash(key);
    Node *tmp = t[h].next;
    Node *cur = &t[h];
    while (tmp != NULL) {
        cur = tmp;
        tmp = tmp->next;
        if (str_eq(cur->name, key)) {
            printf("无法插入%s: 已存在.\n", key);
            return FAILURE;
        }
    }
    cur->next = (Node *)malloc(sizeof(Node));
    if (cur->next == NULL) {
        printf("无法插入%s: 无法成功分配内存!", key);
        return FAILURE;
    }
    cur->next->next = NULL;
    strcpy(cur->next->name, key);
    return SUCCESS;
}

int find(Node *t, Name key) {
    int cnt = 1;
    int h = hash(key);
    Node *cur = t[h].next;
    while (cur != NULL) {
        if (str_eq(cur->name, key)) {
            printf("查找到\"%s\", 哈希值: %d, 位置: %p, 比较次数: %d.\n", key,
                   h, (void *)cur, cnt);
            return cnt;
        }
        cur = cur->next;
        cnt++;
    }
    printf("找不到\"%s\", 比较次数: %d.\n", key, cnt);
    return 0;
}
#endif

#ifndef CHAINING
int *D_LIST;
size_t D_LIST_LEN;

int insert(Name *t, Name key) {
    int h = hash(key);
    if (t[h][0] != 0 && str_eq(t[h], key)) {
        printf("无法插入%s: 已存在.\n", key);
        return FAILURE;
    }
    for (int i = 0, tmp = h; t[h][0] != 0 && i < (int)D_LIST_LEN; i++) {
        if (str_eq(t[h], key)) {
            printf("无法插入%s: 已存在.\n", key);
            return FAILURE;
        }
        if (tmp + (int)D_LIST[i] >= 0 &&
            tmp + (int)D_LIST[i] < (int)TABLE_LEN) {
            h = tmp + D_LIST[i];
        }
    }
    if (t[h][0] != 0) {
        printf("无法插入%s: 无法解决的哈希冲突.\n", key);
        return FAILURE;
    }
    strcpy(t[h], key);
    return SUCCESS;
}

int find(Name *t, Name key) {
    int h = hash(key);
    int original_hash = h;
    int cnt = 1;
    if (t[h][0] != 0) {
        for (int i = 0, tmp = h; i < (int)D_LIST_LEN; i++, cnt++) {
            if (str_eq(t[h], key)) {
                printf("查找到\"%s\", 哈希值: %d, 位置: %d, 比较次数: %d.\n",
                       key, original_hash, h, cnt);
                return cnt;
            }
            if (tmp + (int)D_LIST[i] >= 0 &&
                tmp + (int)D_LIST[i] < (int)TABLE_LEN) {
                h = tmp + (int)D_LIST[i];
            }
            cnt++;
        }
    }
    printf("找不到\"%s\", 比较次数: %d.\n", key, cnt);
    return 0;
}
#endif

#ifdef SQUARE
void init_d_list(void) {
    D_LIST = (int *)malloc(TABLE_LEN * sizeof(int));
    for (int i = 1; i * i <= (int)TABLE_LEN; i++) {
        D_LIST[D_LIST_LEN++] = i * i;
        D_LIST[D_LIST_LEN++] = -i * i;
    }
    void *new = realloc(D_LIST, D_LIST_LEN * sizeof(int));
    if (new == NULL) {
        puts("内存分配出错!");
        exit(EXIT_FAILURE);
    }
}
#endif

#ifdef LINEAR
void init_d_list(void) {
    D_LIST = (int *)malloc(TABLE_LEN * sizeof(int));
    for (int i = 1; i <= (int)TABLE_LEN; i++) {
        D_LIST[D_LIST_LEN++] = i;
    }
    void *new = realloc(D_LIST, D_LIST_LEN * sizeof(int));
    if (new == NULL) {
        puts("内存分配出错!");
        exit(EXIT_FAILURE);
    }
}
#endif

int main(void) {

// Initialize the hash table (and d list)
#ifdef CHAINING
    Node *hash_table = (Node *)malloc(MOD * sizeof(Node));
    memset(hash_table, 0, MOD * sizeof(Node));
#endif
#ifndef CHAINING
    Name *hash_table = (Node *)malloc(TABLE_LEN * sizeof(Name));
    memset(hash_table, 0, TABLE_LEN * sizeof(Name));
    init_d_list();
#endif

    Name *names_list = NULL;
    int cnt = 0;
    puts(TITLE);
    printf("输入您要插入的人名个数: ");
    scanf("%d", &cnt);
    names_list = (Name *)malloc((size_t)cnt * sizeof(Name));
    if (names_list == NULL) {
        puts("错误: 无法为人名列表分配内存!");
        return EXIT_FAILURE;
    }
    puts("输入人名(不含空格), 一行一个, 或以空格分隔:");
    for (int i = 0; i < cnt; i++) {
        scanf("%s", names_list[i]);
        insert(hash_table, names_list[i]);
    }
    puts("自动化的查找演示:");
    int tot = 0, valid = cnt;
    for (int i = 0; i < cnt; i++) {
        int result = find(hash_table, names_list[i]);
        if (result == 0) {
            printf("查询时发现%s未找到!\n", names_list[i]);
            valid--;
        }
        tot += result;
    }
    printf("查询总数: %d, 有效查询数: %d\n", cnt, valid);
    printf("成功率 = %d/%d = %.2lf%%\n", valid, cnt,
           (double)valid / (double)cnt * 100);
    printf("ASL = %d/%d = %.2lf\n", tot, valid, (double)tot / (double)valid);
    return EXIT_SUCCESS;
}
