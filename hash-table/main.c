#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define METHOD_CHAINING

const int MOD = 10000849;
const int BASE = 26;

const int SUCCESS = 1;
const int FAILURE = 0;

typedef char Name[32];

#ifdef METHOD_CHAINING
typedef struct Node {
    Name name;
    struct Node *next;
} Node;
#endif

int convert(char ch) {
    if (ch >= 'a' && ch <= 'z') { // Is lowercase.
        ch -= 32;
    }
    if (ch < 'A' || ch > 'Z') {
        return 0;
    }
    return ch - 'A';
}

// It convert a string to a base 26 number,
// and mod a large prime "MOD".
int hash(Name name) {
    int result = 0, tmp = 1;
    for (size_t i = 0; i < strlen(name); i++) {
        result = (convert(name[i]) * tmp % MOD + result) % MOD;
        tmp = tmp * BASE % MOD;
    }
    return result;
}

bool str_eq(Name a, Name b) {
    return strcmp(a, b) == 0;
}

#ifdef METHOD_CHAINING
int insert(Node *t, Name key) {
    int h = hash(key);
    Node *tmp = t[h].next;
    Node *cur = &t[h];
    while (tmp != NULL) {
        cur = tmp;
        tmp = tmp->next;
        if (str_eq(cur->name, key)) {
            printf("无法插入%s: 已存在.", key);
            return FAILURE;
        }
    }
    cur->next = malloc(sizeof(Node));
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
            printf("查找到\"%s\", 比较次数: %d.\n", key, cnt);
            return cnt;
        }
        cur = cur->next;
        cnt++;
    }
    printf("找不到\"%s\", 比较次数: %d.\n", key, cnt);
    return 0;
}
#endif

int main(void) {

// For "chaining" method //
#ifdef METHOD_CHAINING
    // Initialize the hash table
    Node *hash_table = malloc(MOD * sizeof(Node));
    Name *names_list = NULL;
    memset(hash_table, 0, MOD * sizeof(Node));
    int cnt = 0;
    puts("哈希表演示程序(拉链法)");
    printf("输入您要插入的人名个数: ");
    scanf("%d", &cnt);
    names_list = malloc((size_t)cnt * sizeof(Name));
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
    int tot = 0;
    for (int i = 0; i < cnt; i++) {
        tot += find(hash_table, names_list[i]);
    }
    printf("ASL = %d/%d = %.2lf\n", tot, cnt, (double)tot / (double)cnt);
#endif
    return 0;
}
