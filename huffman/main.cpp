/*
 * @Author: FunctionSir
 * @License: AGPLv3
 * @Date: 2024-12-23 08:23:59
 * @LastEditTime: 2024-12-23 15:07:40
 * @LastEditors: FunctionSir
 * @Description: -
 * @FilePath: /crse-proj-ds/huffman/main.cpp
 */
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>
using namespace std;

/* If not initialized, escape */
#define ESCAPE_IF_NOT_INITIALIZED                                              \
    if (!INITIALIZED) {                                                        \
        cout << "您还没有初始化!" << endl;                                     \
        continue;                                                              \
    }

/* If not built the tree yet, escape */
#define ESCAPE_IF_NOT_BUILT                                                    \
    if (!BUILT) {                                                              \
        cout << "您还没有建立过哈夫曼树!" << endl;                             \
        continue;                                                              \
    }

/* If not encoded, escape */
#define ESCAPE_IF_NOT_ENCODED                                                  \
    if (!ENCODED) {                                                            \
        cout << "您还没有编码过!" << endl;                                     \
        continue;                                                              \
    }

/* Converted choice */
#define CONVERTED_CHOICE ('a' <= choice && choice <= 'z' ? choice - 32 : choice)

/* Cross-platform clear screen */
void clear_screen(void) {
#ifdef _WIN32 // For MS Windows.
    system("cls");
#else // For Linux, Mac, xBSD, UNIX and other.
    system("clear");
#endif
}

/* A blackhole */
string _;

/* Version info */
const string VER = "0.1.0";

/* Frequent info entry */
struct Freq {
    char ch;  // Char
    int freq; // Frequent
    /* Impl operator < for Freq */
    bool operator<(const Freq &b) const {
        return freq < b.freq;
    }
};

struct TreeNode {
    char ch;         // Char
    int freq;        // Frequent
    TreeNode *left;  // Left child
    TreeNode *right; // Right child
    /* Impl operator < for TreeNode */
    bool operator<(const TreeNode &b) const {
        return freq < b.freq;
    }
};

struct TreeNodeBox {
    TreeNode *ptr;
    bool operator<(const TreeNodeBox &b) const {
        return ptr->freq < b.ptr->freq;
    }
};

TreeNodeBox TREE = {NULL};
multiset<Freq> FREQ_INFO;
set<char> CHARSET;
map<char, string> CODE;

/* Input prompt */
string PROMPT = ">>> ";

/* Initialized yet */
bool INITIALIZED = false;

/* Tree built yet */
bool BUILT = false;

/* Code gened yet */
bool CODE_GENED = false;

/* Encoded yet */
bool ENCODED = false;

/* Put prompt */
void put_prompt(void) {
    cout << PROMPT;
}

/* Print menu */
void print_menu(void) {
    cout << endl;
    cout << "选择您要进行的操作:" << endl;
    cout << "[I]nit           初始化" << endl;
    cout << "[S]tat       字符集信息" << endl;
    cout << "[B]uild    构建哈夫曼树" << endl;
    cout << "[L]oad     加载哈夫曼树" << endl;
    cout << "[T]ree     输出哈夫曼树" << endl;
    cout << "[E]ncode           编码" << endl;
    cout << "[P]rint    输出编码结果" << endl;
    cout << "[D]ecode           解码" << endl;
    cout << "[O]utput   输出解码结果" << endl;
    cout << "[C]lear            清屏" << endl;
    cout << "[Q]uit             退出" << endl;
}

/* Initialization */
void init(void) {
    char choice;
    if (INITIALIZED) {
        cout << "看起来您已经初始化过了, 要继续么?" << endl;
        cout << "要继续, 输入Y或y, 否则, 输入其他内容." << endl;
        put_prompt();
        cin >> choice;
        if (CONVERTED_CHOICE != 'Y') {
            return;
        }
    }
    cout << "请选择字符集和频度数据来源:" << endl;
    cout << "[F]ile  从文件读入" << endl;
    cout << "[I]nput   手动输入" << endl;
    cout << "[A]uto    自动统计" << endl;
    put_prompt();
    cin >> choice;
    int charset_size = -1;
    string freq_path;
    ifstream freq_file;
    switch (CONVERTED_CHOICE) {
    case 'F':
        cout << "请输入文件路径:" << endl;
        put_prompt();
        getline(cin, _);
        getline(cin, freq_path);
        freq_file.open(freq_path);
        if (!freq_file.is_open()) {
            cout << "错误: 文件打开失败!" << endl;
            return;
        }
        freq_file >> charset_size;
        if (charset_size <= 0) {
            cout << "数据非法: 字符集大小不存在, 或不合法!" << endl;
            return;
        }
        FREQ_INFO.clear();
        CHARSET.clear();
        INITIALIZED = false;
        for (int i = 0; i < charset_size; i++) {
            char cur_ch = 0;
            int cur_freq = -1;
            freq_file >> cur_ch >> cur_freq;
            if (cur_ch == 0 || cur_freq <= 0) {
                cout << "数据非法: 存在非法的字符频度条目或条目数量不够!"
                     << endl;
                return;
            }
            if (CHARSET.count(cur_ch)) {
                cout << "数据非法: 存在重复的字符频度条目!" << endl;
                return;
            }
            FREQ_INFO.insert({cur_ch, cur_freq});
        }
        freq_file.close();
        break;
    case 'I':
        cout << "请输入字符集大小:" << endl;
        put_prompt();
        cin >> charset_size;
        if (charset_size <= 0) {
            cout << "数据非法: 字符集大小不合法!" << endl;
            return;
        }
        FREQ_INFO.clear();
        CHARSET.clear();
        INITIALIZED = false;
        cout << "接下来, 输入字符及其频度, 每行一组, 字符和频度间用空格隔开:"
             << endl;
        for (int i = 1; i <= charset_size; i++) {
            char cur_ch = 0;
            int cur_freq = -1;
            cout << "[" << i << "] "; // Line number
            put_prompt();
            cin >> cur_ch >> cur_freq;
            if (cur_ch == 0 || cur_freq <= 0) {
                cout << "数据非法: 字符频度条目非法或条目数量不够!" << endl;
                cout << "警告: 已忽略非法条目!" << endl;
                continue;
            }
            if (CHARSET.count(cur_ch)) {
                cout << "数据非法: 存在重复的字符频度条目!" << endl;
                cout << "警告: 已忽略非法条目!" << endl;
                continue;
            }
            FREQ_INFO.insert({cur_ch, cur_freq});
        }
        cout << "要输出您的输入到文件么?" << endl;
        cout << "要输出, 输入Y或y, 否则, 输入其他内容." << endl;
        put_prompt();
        cin >> choice;
        if (CONVERTED_CHOICE == 'Y') {
            cout << "请输入要保存到的文件的路径:" << endl;
            put_prompt();
            string outfile;
            getline(cin, _);
            getline(cin, outfile);
            ofstream outstream;
            outstream.open(outfile);
            if (!outstream.is_open()) {
                cout << "错误: 无法打开或创建输出文件!" << endl;
                break;
            }
            outstream << FREQ_INFO.size() << endl;
            for (auto x : FREQ_INFO) {
                outstream << x.ch << " " << x.freq << endl;
            }
            outstream.close();
        }
        break;
    case 'A':
        cout << "您要统计某文件内的信息还是手动输入?" << endl;
        put_prompt();
        cin >> choice;
        FREQ_INFO.clear();
        CHARSET.clear();
        INITIALIZED = false;
        switch (CONVERTED_CHOICE) {
        case 'F': {
            char cur_char;
            map<char, int> stat;
            cout << "请输入文件路径:" << endl;
            put_prompt();
            getline(cin, _);
            getline(cin, freq_path);
            freq_file.open(freq_path);
            if (!freq_file.is_open()) {
                cout << "错误: 文件打开失败!" << endl;
                return;
            }
            while (freq_file >> cur_char) {
                stat[cur_char]++;
                CHARSET.insert(cur_char);
            }
            for (auto x : stat) {
                FREQ_INFO.insert({x.first, x.second});
            }
            freq_file.close();
        } break;
        case 'I': {
            string text;
            cout << "输入内容, 按Enter结束:";
            put_prompt();
            getline(cin, _);
            getline(cin, text);
            map<char, int> stat;
            for (auto ch : text) {
                stat[ch]++;
                CHARSET.insert(ch);
            }
            for (auto x : stat) {
                FREQ_INFO.insert({x.first, x.second});
            }
        } break;
        default:
            cout << "错误: 无法理解的意图!" << endl;
            return;
        }
        cout << "要输出结果到文件么?" << endl;
        cout << "要输出, 输入Y或y, 否则, 输入其他内容." << endl;
        put_prompt();
        cin >> choice;
        if (CONVERTED_CHOICE == 'Y') {
            cout << "请输入要保存到的文件的路径:" << endl;
            put_prompt();
            string outfile;
            getline(cin, _);
            getline(cin, outfile);
            ofstream outstream;
            outstream.open(outfile);
            if (!outstream.is_open()) {
                cout << "错误: 无法打开或创建输出文件!" << endl;
                break;
            }
            outstream << FREQ_INFO.size() << endl;
            for (auto x : FREQ_INFO) {
                outstream << x.ch << " " << x.freq << endl;
            }
            outstream.close();
        }
        break;
    default:
        cout << "错误: 无法理解的意图!" << endl;
        return;
    }
    INITIALIZED = true;
}

/* Show charset stats */
void stat(void) {
    cout << "字符集和字符频数信息:" << endl;
    cout << "字符集大小: " << FREQ_INFO.size() << endl;
    cout << "详细信息:" << endl;
    for (auto x : FREQ_INFO) {
        cout << "字符\"" << x.ch << "\"的频数是: " << x.freq << "." << endl;
    }
}

void free_tree(TreeNode *tree) {
    if (tree == NULL) {
        return;
    }
    if (tree->left != NULL) {
        free_tree(tree->left);
    }
    if (tree->right != NULL) {
        free_tree(tree->right);
    }
    free(tree);
}

/* Serialize, convert a tree to a string */
string serialize(TreeNode *tree, int &cur_id) {
    string result = "", l_result = "", r_result = "";
    result += to_string(cur_id) + " " + to_string(tree->ch) + " " +
              to_string(tree->freq) + " ";
    if (tree->left != NULL) {
        result += to_string(++cur_id) + " ";
        l_result = serialize(tree->left, cur_id);
    } else {
        result += "0 ";
    }
    if (tree->right != NULL) {
        result += to_string(++cur_id) + "\n";
        r_result = serialize(tree->right, cur_id);
    } else {
        result += "0\n";
    }
    result += l_result;
    result += r_result;
    return result;
}

/* Build the tree */
void build(void) {
    char choice;
    if (BUILT) {
        cout << "看起来您已经构建过哈夫曼树了, 要继续么?" << endl;
        cout << "要继续, 输入Y或y, 否则, 输入其他内容." << endl;
        put_prompt();
        cin >> choice;
        if (CONVERTED_CHOICE != 'Y') {
            return;
        }
        free_tree(TREE.ptr);
        BUILT = false;
        TREE = {NULL};
    }
    multiset<TreeNodeBox> on_build;
    for (auto x : FREQ_INFO) {
        TreeNode *tmp = (TreeNode *)malloc(sizeof(TreeNode));
        if (tmp == NULL) {
            cout << "错误: 严重内存错误!" << endl;
            exit(EXIT_FAILURE);
        }
        tmp->ch = x.ch;
        tmp->freq = x.freq;
        tmp->left = NULL;
        tmp->right = NULL;
        on_build.insert({tmp});
    }
    while (on_build.size() > 1) {
        TreeNodeBox a, b;
        a = *on_build.begin();
        on_build.erase(on_build.begin());
        b = *on_build.begin();
        on_build.erase(on_build.begin());
        TreeNode *tmp = (TreeNode *)malloc(sizeof(TreeNode));
        if (tmp == NULL) {
            cout << "错误: 严重内存错误!" << endl;
            exit(EXIT_FAILURE);
        }
        tmp->ch = 0;
        tmp->freq = a.ptr->freq + b.ptr->freq;
        tmp->left = a.ptr;
        tmp->right = b.ptr;
        on_build.insert({tmp});
    }
    TREE = *on_build.begin();
    cout << "要保存哈夫曼树到文件么?" << endl;
    cout << "要保存, 输入Y或y, 否则, 输入其他内容." << endl;
    put_prompt();
    cin >> choice;
    if (CONVERTED_CHOICE == 'Y') {
        cout << "请输入要保存到的文件的路径:" << endl;
        put_prompt();
        string outfile;
        getline(cin, _);
        getline(cin, outfile);
        ofstream outstream;
        outstream.open(outfile);
        if (!outstream.is_open()) {
            cout << "错误: 无法打开或创建输出文件!" << endl;
            return;
        }
        int node_cnt = 1;
        string result = serialize(TREE.ptr, node_cnt);
        outstream << node_cnt << endl;
        outstream << result;
        outstream.close();
    }
}

void load(void) {
    char choice;
    if (INITIALIZED) {
        cout << "看起来您已经初始化过了, 要继续么?" << endl;
        cout << "要继续, 输入Y或y, 否则, 输入其他内容." << endl;
        put_prompt();
        cin >> choice;
        if (CONVERTED_CHOICE != 'Y') {
            return;
        }
    }
    if (BUILT) {
        cout << "看起来您已经构建过哈夫曼树了, 要继续么?" << endl;
        cout << "要继续, 输入Y或y, 否则, 输入其他内容." << endl;
        put_prompt();
        cin >> choice;
        if (CONVERTED_CHOICE != 'Y') {
            return;
        }
        free_tree(TREE.ptr);
        BUILT = false;
        TREE = {NULL};
    }
    string path;
    cout << "请输入树文件的路径:" << endl;
    put_prompt();
    getline(cin, _);
    getline(cin, path);
    ifstream file_in(path);
    if (!file_in.is_open()) {
        cout << "错误: 无法打开文件!" << endl;
        return;
    }
    int node_cnt = -1;
    file_in >> node_cnt;
    if (node_cnt <= 0) {
        cout << "错误: 非法的结点个数!" << endl;
        return;
    }
    free_tree(TREE.ptr);
    FREQ_INFO.clear();
    CHARSET.clear();
    INITIALIZED = false;
    BUILT = false;
    vector<TreeNodeBox> boxed_nodes((size_t)node_cnt + 1);
    vector<pair<size_t, size_t>> ptr_guide((size_t)node_cnt + 1);
    boxed_nodes[0].ptr = NULL;
    ptr_guide[0] = {0, 0};
    for (int i = 0; i < node_cnt; i++) {
        int id, ascii, freq, l_child, r_child;
        file_in >> id >> ascii >> freq >> l_child >> r_child;
        if (id <= 0 || ascii < 0 || freq <= 0 || l_child > node_cnt ||
            r_child > node_cnt || (ascii != 0 && CHARSET.count((char)ascii))) {
            cout << "错误: 非法的文件内容! 位置: 第" << i + 2 << "行.\n";
            return;
        }
        boxed_nodes[(size_t)id].ptr = (TreeNode *)malloc(sizeof(TreeNode));
        boxed_nodes[(size_t)id].ptr->ch = (char)ascii;
        boxed_nodes[(size_t)id].ptr->freq = freq;
        boxed_nodes[(size_t)id].ptr->left = NULL;
        boxed_nodes[(size_t)id].ptr->right = NULL;
        ptr_guide[(size_t)id] = {(size_t)l_child, (size_t)r_child};
        if (ascii != 0) {
            FREQ_INFO.insert({(char)ascii, freq});
            CHARSET.insert((char)ascii);
        }
    }
    for (size_t i = 1; i <= (size_t)node_cnt; i++) {
        boxed_nodes[i].ptr->left =
            (ptr_guide[i].first == 0 ? NULL
                                     : boxed_nodes[ptr_guide[i].first].ptr);
        boxed_nodes[i].ptr->right =
            (ptr_guide[i].second == 0 ? NULL
                                      : boxed_nodes[ptr_guide[i].second].ptr);
    }
    TREE = boxed_nodes[1];
    INITIALIZED = true;
    BUILT = true;
}

void gen_code(void) {
}

int main(void) {
    cout << "哈夫曼编码解码程序 [ Ver " << VER << " ]" << endl;
    while (true) {
        char choice = 0;
        print_menu();
        put_prompt();
        cin >> choice;
        switch (CONVERTED_CHOICE) {
        case 'I': // Init
            init();
            break;
        case 'S': // Stat
            ESCAPE_IF_NOT_INITIALIZED
            stat();
            break;
        case 'B': // Build
            ESCAPE_IF_NOT_INITIALIZED
            build();
            break;
        case 'L': // Load
            load();
            break;
        case 'T': // Tree
            ESCAPE_IF_NOT_INITIALIZED
            break;
        case 'E': // Encode
            ESCAPE_IF_NOT_INITIALIZED
            ESCAPE_IF_NOT_BUILT
            break;
        case 'P': // Print
            ESCAPE_IF_NOT_INITIALIZED
            ESCAPE_IF_NOT_BUILT
            ESCAPE_IF_NOT_ENCODED
            break;
        case 'D':
            ESCAPE_IF_NOT_INITIALIZED
            break;
        case 'C':
            clear_screen();
            cout << "哈夫曼编码解码程序 [ Ver " << VER << " ]" << endl;
            break;
        case 'Q':
            return EXIT_SUCCESS;
        default:
            cout << "错误: 无法理解的意图!" << endl;
            break;
        }
    }
    return EXIT_SUCCESS;
}
