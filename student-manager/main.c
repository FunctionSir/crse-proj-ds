/*
 * @Author: FunctionSir
 * @License: AGPLv3
 * @Date: 2024-12-16 08:58:16
 * @LastEditTime: 2024-12-19 10:06:40
 * @LastEditors: FunctionSir
 * @Description: 学生成绩管理系统
 * @FilePath: /crse-proj-ds/student-manager/main.c
 */

#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Software info */
const char *VER = "0.0.1";

/* Input prompt*/
const char *PROMPT = ">>> ";

/* For functions */
const int SUCCESS = 1;
const int FAILURE = 0;

/* Cross-platform clear screen */
#ifdef _WIN32 // For MS Windows.
void clear_screen(void) {
    system("cls");
}
#else // For Linux, Mac, xBSD, UNIX and other.
void clear_screen(void) {
    system("clear");
}
#endif

/* Prompt */
void put_prompt(void) {
    printf("%s", PROMPT);
}

/* Type Course */
typedef struct Course {
    char name[256];
    double weight;
} Course;

/* Type Student */
typedef struct Student {
    char id[256];
    char name[256];
    double score;
    double score_with_weight;
    size_t detail_id;
} Student;

/* Type Student Node */
typedef struct StudentNode {
    Student data;
    struct StudentNode *next;
} StudentNode;

/* Courses related */
bool COURSES_SET;
Course *COURSES;
size_t COURSES_CNT;

/* Students related */
bool STUDENTS_SET;
StudentNode STUDENTS_HEAD;
double **DETAILS;
size_t STUDENTS_CNT;

/*
Set courses.
Return SUCCESS = OK;
Return FAILURE = Not proceed or error occurred.
*/
int set_courses(void) {
    if (STUDENTS_SET) {
        puts("错误: 您已经设定了学生, 要修改课程设定, 请先清空学生信息.");
        return FAILURE;
    }
    if (COURSES_SET) {
        short choice;
        puts("看起来您已经设定过课程了, 您要继续么?");
        puts("要继续请输入1, 否则请输入其他数字, 最后按下回车.");
        put_prompt();
        scanf("%hd", &choice);
        if (choice != 1) {
            return FAILURE;
        }
    }
    puts("首先, 明确贵校的课程数目.");
    put_prompt();
    scanf("%lu", &COURSES_CNT);
    if (COURSES != NULL) { // To avoid memory leaks.
        free(COURSES);
    }
    COURSES = malloc(COURSES_CNT * sizeof(Course));
    if (COURSES == NULL) {
        puts("糟糕! 无法为课程设定分配内存.");
        puts("您也许需要缩减课程数量或者更换更好的机器.");
        COURSES_SET = false;
        return FAILURE;
    }
    puts("接下来, 输入每门课程的名称和权值.");
    puts("每行一个课程, 先输入名称(不大于255个英文字符), 然后一个空格,");
    puts("后加权值(整数百分比, 无需加百分号). 权值大者代表重要性高.");
    int tot_percentage = 0;
    for (size_t i = 0; i < COURSES_CNT; i++) {
        int percentage;
        printf("[%lu] ", i + 1);
        put_prompt();
        scanf("%s%d", COURSES[i].name, &percentage);
        COURSES[i].weight = (double)percentage / (double)100;
        tot_percentage += percentage;
    }
    if (tot_percentage != 100) {
        puts("警告: 所有课程的权值之和不等于100%!");
    }
    COURSES_SET = true;
    puts("课程信息已成功设定.");
    return SUCCESS;
}

/* List courses. */
int list_courses(void) {
    if (!COURSES_SET) {
        puts("错误: 您还没有设定过课程!");
        return FAILURE;
    }
    puts("以下是您的课程和对应的权值:");
    for (size_t i = 0; i < COURSES_CNT; i++) {
        double weight = round(COURSES[i].weight * 100);
        printf("课程: %s, 权值: %.0lf%%.\n", COURSES[i].name, weight);
    }
    return SUCCESS;
}

void free_chain(StudentNode *head) {
    if (head == NULL) {
        return;
    }
    free_chain(head->next);
    free(head);
}

/* Clear students. */
int clear_students(void) {
    if (!STUDENTS_SET) {
        return SUCCESS;
    }
    if (STUDENTS_SET) {
        short choice;
        puts("看起来您已经设定过学生了, 您真的要继续么?");
        puts("要继续请输入1, 否则请输入其他数字, 最后按下回车.");
        put_prompt();
        scanf("%hd", &choice);
        if (choice != 1) {
            return FAILURE;
        }
    }
    free_chain(STUDENTS_HEAD.next);
    STUDENTS_HEAD.next = NULL;
    for (size_t i = 0; i < STUDENTS_CNT; i++) {
        free(DETAILS[i]);
    }
    free(DETAILS);
    STUDENTS_CNT = 0;
    STUDENTS_SET = false;
    return SUCCESS;
}

bool student_better(Student *a, Student *b) {
    if (a->score_with_weight != b->score_with_weight) {
        return a->score_with_weight > b->score_with_weight;
    }
    return strcmp(a->id, b->id) < 0;
}

/*
Set students.
Return SUCCESS = OK;
Return FAILURE = Not processed or error occurred.
*/
int set_students(void) {
    if (STUDENTS_SET) {
        short process;
        puts("看起来您已经设定过学生了, 您要继续么?");
        puts("要继续请输入1, 否则请输入其他数字, 最后按下回车.");
        put_prompt();
        scanf("%hd", &process);
        if (process != 1) {
            return FAILURE;
        }
    }
    if (list_courses() != SUCCESS) {
        STUDENTS_SET = false;
        return FAILURE;
    }
    puts("请先输入学生的个数:");
    put_prompt();
    scanf("%lu", &STUDENTS_CNT);
    if (clear_students() != SUCCESS) {
        puts("错误: 无法清除现有的学生信息.");
        return FAILURE;
    }
    DETAILS = malloc(STUDENTS_CNT * sizeof(double *));
    if (DETAILS == NULL) {
        puts("糟糕! 无法为学生信息分配内存.");
        puts("您也许需要缩减学生个数或者更换更好的机器.");
        exit(EXIT_FAILURE);
    }
    for (size_t i = 0; i < STUDENTS_CNT; i++) {
        DETAILS[i] = malloc(COURSES_CNT * sizeof(double));
        if (DETAILS[i] == NULL) {
            puts("糟糕! 无法为学生信息分配内存.");
            puts("您也许需要缩减学生个数或者更换更好的机器.");
            exit(EXIT_FAILURE);
        }
    }
    puts("请按照学号, 姓名(不得有空格), 各科成绩(按上方课程顺序), 来输入数据.");
    puts("学号和姓名均不大于255个英文字符, 各数据间用空格隔开, 每行一个学生.");
    StudentNode *first = STUDENTS_HEAD.next, *tail = &STUDENTS_HEAD;
    StudentNode *cur, *before, *tmp_ptr;
    Student tmp;
    for (size_t i = 0; i < STUDENTS_CNT; i++) {
        printf("[%lu] ", i + 1);
        put_prompt();
        scanf("%s%s", tmp.id, tmp.name);
        tmp.detail_id = i;
        tmp.score = 0;
        tmp.score_with_weight = 0;
        for (size_t j = 0; j < COURSES_CNT; j++) {
            scanf("%lf", &DETAILS[i][j]);
            tmp.score += DETAILS[i][j];
            tmp.score_with_weight += DETAILS[i][j] * COURSES[j].weight;
        }
        if (first == NULL) { // First node.
            STUDENTS_HEAD.next = malloc(sizeof(StudentNode));
            if (STUDENTS_HEAD.next == NULL) {
                puts("糟糕! 无法为学生信息分配内存.");
                puts("您也许需要缩减学生个数或者更换更好的机器.");
                exit(EXIT_FAILURE);
            }
            first = STUDENTS_HEAD.next;
            tail = STUDENTS_HEAD.next;
            first->next = NULL;
            first->data = tmp;
            continue;
        }
        if (student_better(&tmp, &first->data)) { // If the student is the best.
            tmp_ptr = malloc(sizeof(StudentNode));
            if (tmp_ptr == NULL) {
                puts("糟糕! 无法为学生信息分配内存.");
                puts("您也许需要缩减学生个数或者更换更好的机器.");
                exit(EXIT_FAILURE);
            }
            STUDENTS_HEAD.next = tmp_ptr;
            STUDENTS_HEAD.next->next = first;
            first = STUDENTS_HEAD.next;
            first->data = tmp;
            continue;
        }
        if (student_better(&tail->data, &tmp)) { // If the student is the worst.
            tail->next = malloc(sizeof(StudentNode));
            if (tail->next == NULL) {
                puts("糟糕! 无法为学生信息分配内存.");
                puts("您也许需要缩减学生个数或者更换更好的机器.");
                exit(EXIT_FAILURE);
            }
            tail = tail->next;
            tail->next = NULL;
            tail->data = tmp;
            continue;
        }
        before = NULL;
        cur = first;
        while (student_better(&cur->data, &tmp)) {
            before = cur;
            cur = cur->next;
        }
        tmp_ptr = malloc(sizeof(StudentNode));
        if (tmp_ptr == NULL) {
            puts("糟糕! 无法为学生信息分配内存.");
            puts("您也许需要缩减学生个数或者更换更好的机器.");
            exit(EXIT_FAILURE);
        }
        before->next = tmp_ptr;
        tmp_ptr->next = cur;
        tmp_ptr->data = tmp;
    }
    STUDENTS_SET = true;
    puts("学生信息已成功输入.");
    return SUCCESS;
}

void list_students(void) {
    if (!STUDENTS_SET) {
        puts("错误: 还未输入学生信息!");
        return;
    }
    printf("字段顺序: 排名, 学号, 姓名, 加权总成绩, 总成绩");
    for (size_t i = 0; i < COURSES_CNT; i++) {
        printf(", %s成绩", COURSES[i].name);
    }
    putchar('\n');
    int cur_rank = 1;
    int this_cnt = 1;
    StudentNode *cur = STUDENTS_HEAD.next;
    while (cur != NULL) {
        double this_weighted_score = cur->data.score_with_weight;
        double next_weighted_score = (cur->next != NULL)
                                         ? (cur->next->data.score_with_weight)
                                         : this_weighted_score - 1;
        printf("排名: %d 学号: %s 姓名: %s 加权总成绩: %.2lf 总成绩: %.2lf",
               cur_rank, cur->data.id, cur->data.name, this_weighted_score,
               cur->data.score);
        if (this_weighted_score == next_weighted_score) {
            this_cnt++;
        } else {
            cur_rank += this_cnt;
            this_cnt = 1;
        }
        for (size_t j = 0; j < COURSES_CNT; j++) {
            printf(" %s成绩: %.2lf (%.0lf%%)", COURSES[j].name,
                   DETAILS[cur->data.detail_id][j], COURSES[j].weight * 100);
        }
        putchar('\n');
        cur = cur->next;
    }
}

int main(void) {
    // Output name & ver //
    printf("学生成绩管理系统 [版本: %s]\n", VER);

    // Main loop //
    while (true) {
        short choice;
        puts("\n操作列表");
        puts("1: 设定课程信息");
        puts("2: 输入学生信息");
        puts("3: 列出课程信息");
        puts("4: 列出学生信息");
        puts("5: 清空学生信息");
        puts("6: 清除屏幕内容");
        puts("0: 退出本系统");

        // Get choice //
        put_prompt();
        if (scanf("%hd", &choice) != 1) {
            putchar('\n');
            exit(EXIT_SUCCESS);
        };

        switch (choice) {
        case 1: // Set courses.
            set_courses();
            break;
        case 2: // Set students.
            set_students();
            break;
        case 3: // List courses.
            list_courses();
            break;
        case 4: // List students.
            list_students();
            break;
        case 5: // Clear students info.
            clear_students();
            break;
        case 6: // Clear screen.
            clear_screen();
            printf("学生成绩管理系统 [版本: %s]\n", VER);
            break;
        case 0: // Exit.
            clear_students();
            if (COURSES != NULL) {
                free(COURSES);
            }
            puts("Goodbye!");
            return EXIT_SUCCESS;
        }
    }

    return EXIT_SUCCESS;
}
