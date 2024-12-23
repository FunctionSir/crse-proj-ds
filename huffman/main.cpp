/*
 * @Author: FunctionSir
 * @License: AGPLv3
 * @Date: 2024-12-23 08:23:59
 * @LastEditTime: 2024-12-23 19:12:00
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
#include <sstream>
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
    if (!IS_ENCODED) {                                                         \
        cout << "您还没有编码过!" << endl;                                     \
        continue;                                                              \
    }

/* If not decoded, escape */
#define ESCAPE_IF_NOT_DECODED                                                  \
    if (!IS_DECODED) {                                                         \
        cout << "您还没有解码过!" << endl;                                     \
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
    int ch;   // Char
    int freq; // Frequent
    /* Impl operator < for Freq */
    bool operator<(const Freq &b) const {
        return freq < b.freq;
    }
};

struct TreeNode {
    int ch;          // Char
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
set<int> CHARSET;
map<int, string> CODE;
string ENCODED;
string DECODED;

/* Input prompt */
string PROMPT = ">>> ";

/* Initialized yet */
bool INITIALIZED = false;

/* Tree built yet */
bool BUILT = false;

/* Code gened yet */
bool CODE_GENED = false;

/* Encoded yet */
bool IS_ENCODED = false;

/* Decoded yet */
bool IS_DECODED = false;

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

void reset() {
    CODE_GENED = false;
    INITIALIZED = false;
    BUILT = false;
    IS_ENCODED = false;
    ENCODED = "";
    IS_DECODED = false;
    free_tree(TREE.ptr);
    TREE = {NULL};
    FREQ_INFO.clear();
    CHARSET.clear();
    CODE.clear();
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
        reset();
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
        for (int i = 0; i < charset_size; i++) {
            int cur_ch = 0;
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
            FREQ_INFO.insert({(char)cur_ch, cur_freq});
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
        cout << "接下来, 输入字符的ASCII及其频度, 每行一组, "
                "字符和频度间用空格隔开:"
             << endl;
        for (int i = 1; i <= charset_size; i++) {
            int cur_ch = 0;
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
        cout << "[F]ile  从文件读入" << endl;
        cout << "[I]nput   手动输入" << endl;
        put_prompt();
        cin >> choice;
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
        cout << "字符\"" << (char)x.ch << "\"的频数是: " << x.freq << "."
             << endl;
    }
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
        TREE = {NULL};
        BUILT = false;
        CODE.clear();
        CODE_GENED = false;
        ENCODED = "";
        IS_ENCODED = false;
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
        reset();
    }
    if (BUILT) {
        cout << "看起来您已经构建过哈夫曼树了, 要继续么?" << endl;
        cout << "要继续, 输入Y或y, 否则, 输入其他内容." << endl;
        put_prompt();
        cin >> choice;
        if (CONVERTED_CHOICE != 'Y') {
            return;
        }
        reset();
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

int get_tree_hight(TreeNode *tree, int cur_hight) {
    if (tree == NULL) {
        return cur_hight;
    }
    return max(get_tree_hight(tree->left, cur_hight + 1),
               get_tree_hight(tree->right, cur_hight + 1));
}

void print_part(TreeNode *tree, int max_len, int this_len, stringstream &os) {
    if (tree == NULL) {
        return;
    }
    stringstream to_print_stream;
    if (tree->ch == 0) {
        to_print_stream << "#" << "[" << tree->freq << "]";
    } else {
        to_print_stream << "#" << (char)tree->ch << "(" << tree->freq << ")";
    }
    string to_print = to_print_stream.str();
    for (int i = 0; i < max_len - this_len; i++) {
        os << " ";
    }
    os << to_print;
    for (int i = 0; i < this_len - (int)to_print.length(); i++) {
        os << '#';
    }
    os << endl;
    if (tree->left != NULL) {
        print_part(tree->left, max_len, this_len - 4, os);
    }
    if (tree->right != NULL) {
        print_part(tree->right, max_len, this_len - 4, os);
    }
}

void print_tree(void) {
    int max_len = 4 * get_tree_hight(TREE.ptr, 0) + 16;
    cout << "此哈夫曼树的凹入表示:" << endl << endl;
    stringstream outstream;
    print_part(TREE.ptr, max_len, max_len, outstream);
    string outstring = outstream.str();
    cout << outstring << endl;
    cout << "要输出到文件么?" << endl;
    cout << "要保存, 输入Y或y, 否则, 输入其他内容." << endl;
    put_prompt();
    char choice;
    cin >> choice;
    if (CONVERTED_CHOICE != 'Y') {
        return;
    }
    cout << "请输入要保存到的文件的路径:" << endl;
    put_prompt();
    string outfile;
    getline(cin, _);
    getline(cin, outfile);
    ofstream ofs(outfile);
    if (!ofs.is_open()) {
        cout << "错误: 无法打开文件!" << endl;
    }
    ofs << outstring;
    ofs.close();
}

void gen_code(TreeNode *t, string before) {
    if (t->ch != 0 && t->left == NULL && t->right == NULL) {
        CODE[t->ch] = before;
        return;
    }
    if (t->left != NULL) {
        gen_code(t->left, before + "0");
    }
    if (t->right != NULL) {
        gen_code(t->right, before + "1");
    }
}

void encode(void) {
    char choice;
    if (!CODE_GENED) {
        cout << "生成编码中, 请稍候..." << endl;
        gen_code(TREE.ptr, "");
        CODE_GENED = true;
    }
    cout << "您要编码什么?" << endl;
    cout << "[F]ile        文件内容" << endl;
    cout << "[I]nput 编码输入的内容" << endl;
    put_prompt();
    cin >> choice;
    ENCODED = "";
    switch (CONVERTED_CHOICE) {
    case 'F': {
        string path;
        cout << "请输入待编码文件的路径:" << endl;
        put_prompt();
        getline(cin, _);
        getline(cin, path);
        ifstream in_file(path);
        if (!in_file.is_open()) {
            cout << "错误: 无法打开文件!" << endl;
            return;
        }
        char cur_char;
        while (in_file >> cur_char) {
            if (!CHARSET.count(cur_char)) {
                cout << "警告: 字符\"" << cur_char
                     << "\"不存在于给定的字符集中! 已忽略!" << endl;
            }
            ENCODED += CODE[cur_char];
        }
    } break;
    case 'I': {
        string text;
        cout << "请输入待编码的内容(输入Enter结束):" << endl;
        put_prompt();
        getline(cin, _);
        getline(cin, text);
        for (auto cur_char : text) {
            if (!CHARSET.count(cur_char)) {
                cout << "警告: 字符\"" << cur_char
                     << "\"不存在于给定的字符集中! 已忽略!" << endl;
            }
            ENCODED += CODE[cur_char];
        }
    } break;
    default: {
        cout << "错误: 未知的意图!" << endl;
    } break;
    }
    IS_ENCODED = true;
    cout << "要输出结果到文件么?";
    cout << "要输出, 输入Y或y, 否则, 输入其他内容." << endl;
    put_prompt();
    cin >> choice;
    if (CONVERTED_CHOICE != 'Y') {
        return;
    }
    int per_line, now = 0;
    cout << "每行您希望有多少个字符?" << endl;
    put_prompt();
    cin >> per_line;
    string path;
    cout << "请输入文件的路径:" << endl;
    put_prompt();
    getline(cin, _);
    getline(cin, path);
    ofstream file_out(path);
    for (auto ch : ENCODED) {
        file_out << ch;
        now++;
        if (now == per_line) {
            file_out << endl;
            now = 0;
        }
    }
}

void print_encode_result(void) {
    int per_line, now = 0;
    cout << "每行您希望有多少个字符?" << endl;
    put_prompt();
    cin >> per_line;
    for (auto ch : ENCODED) {
        cout << ch;
        now++;
        if (now == per_line) {
            cout << endl;
            now = 0;
        }
    }
}

void decode(void) {
    TreeNode *cur = TREE.ptr;
    char choice;
    IS_DECODED = false;
    DECODED = "";
    cout << "您要解码什么?" << endl;
    cout << "[F]ile        文件内容" << endl;
    cout << "[I]nput 解码输入的内容" << endl;
    put_prompt();
    cin >> choice;
    switch (CONVERTED_CHOICE) {
    case 'F': {
        string path;
        cout << "请输入文件的路径:" << endl;
        put_prompt();
        getline(cin, _);
        getline(cin, path);
        ifstream in_file(path);
        if (!in_file.is_open()) {
            cout << "错误: 无法打开文件!";
            return;
        }
        char cur_bit;
        while (in_file >> cur_bit) {
            if (cur_bit == '0') {
                cur = cur->left;
            }
            if (cur_bit == '1') {
                cur = cur->right;
            }
            if (cur->ch != 0) {
                DECODED += (char)cur->ch;
                cur = TREE.ptr;
            }
        }
    } break;
    case 'I': {
        string text;
        cout << "请输入待解码的信息(按Enter结束):" << endl;
        put_prompt();
        getline(cin, _);
        getline(cin, text);
        for (auto cur_bit : text) {
            if (cur_bit == '0') {
                cur = cur->left;
            }
            if (cur_bit == '1') {
                cur = cur->right;
            }
            if (cur->ch != 0) {
                DECODED += (char)cur->ch;
                cur = TREE.ptr;
            }
        }
    } break;
    }
    IS_DECODED = true;
    if (cur->ch != 0) {
        cout << "警告: 解码时出现错误, 可能是有不合法的序列或文件已损坏!"
             << endl;
    }
    cout << "解码所得:" << endl << endl;
    cout << DECODED << endl << endl;
    cout << "要输出结果到文件么?";
    cout << "要输出, 输入Y或y, 否则, 输入其他内容." << endl;
    put_prompt();
    cin >> choice;
    if (CONVERTED_CHOICE != 'Y') {
        return;
    }
    string path;
    cout << "请输入文件的路径:" << endl;
    put_prompt();
    getline(cin, _);
    getline(cin, path);
    ofstream file_out(path);
    file_out << DECODED;
    file_out.close();
}

void print_decode_result(void) {
    cout << "解码所得:" << endl << endl;
    cout << DECODED << endl << endl;
}

int main(void) {
    cout << "哈夫曼编码解码程序 [ Ver " << VER << " ]" << endl;
    while (true) {
        char choice = 0;
        print_menu();
        put_prompt();
        if (!(cin >> choice)) {
            exit(EXIT_SUCCESS);
        }
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
            ESCAPE_IF_NOT_BUILT
            print_tree();
            break;
        case 'E': // Encode
            ESCAPE_IF_NOT_INITIALIZED
            ESCAPE_IF_NOT_BUILT
            encode();
            break;
        case 'P': // Print
            ESCAPE_IF_NOT_INITIALIZED
            ESCAPE_IF_NOT_BUILT
            ESCAPE_IF_NOT_ENCODED
            print_encode_result();
            break;
        case 'D': // Decode
            ESCAPE_IF_NOT_INITIALIZED
            ESCAPE_IF_NOT_BUILT
            decode();
            break;
        case 'O': // Output
            ESCAPE_IF_NOT_INITIALIZED
            ESCAPE_IF_NOT_BUILT
            ESCAPE_IF_NOT_DECODED
            print_decode_result();
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
