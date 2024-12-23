/*
 * @Author: FunctionSir
 * @License: AGPLv3
 * @Date: 2024-12-23 08:23:59
 * @LastEditTime: 2024-12-23 10:35:53
 * @LastEditors: FunctionSir
 * @Description: -
 * @FilePath: /crse-proj-ds/huffman/main.cpp
 */
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <string>
using namespace std;

/* If not initialized, escape */
#define ESCAPE_IF_NOT_INITIALIZED                                              \
    if (!INITIALIZED) {                                                        \
        cout << "您还没有初始化!" << endl;                                     \
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

multiset<Freq> FREQ_INFO;
set<char> USED_CHAR;

/* Input prompt */
string PROMPT = ">>> ";

/* Initialized yet */
bool INITIALIZED = false;

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
    cout << "[E]ncode           编码" << endl;
    cout << "[P]rint    输出编码结果" << endl;
    cout << "[T]ree     输出哈夫曼树" << endl;
    cout << "[D]ecode           解码" << endl;
    cout << "[C]lear            清屏" << endl;
    cout << "[Q]uit             退出" << endl;
}

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
        USED_CHAR.clear();
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
            if (USED_CHAR.count(cur_ch)) {
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
        USED_CHAR.clear();
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
            if (USED_CHAR.count(cur_ch)) {
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
        USED_CHAR.clear();
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

void stat(void) {
    cout << "字符集和字符频数信息:" << endl;
    cout << "字符集大小: " << FREQ_INFO.size() << endl;
    cout << "详细信息:" << endl;
    for (auto x : FREQ_INFO) {
        cout << "字符\"" << x.ch << "\"的频数是: " << x.freq << "." << endl;
    }
}

int main(void) {
    cout << "哈夫曼编码解码程序 [ Ver " << VER << " ]" << endl;
    while (true) {
        char choice = 0;
        print_menu();
        put_prompt();
        cin >> choice;
        switch (CONVERTED_CHOICE) {
        case 'I':
            init();
            break;
        case 'S':
            ESCAPE_IF_NOT_INITIALIZED
            stat();
            break;
        case 'E':
            ESCAPE_IF_NOT_INITIALIZED
            break;
        case 'P':
            ESCAPE_IF_NOT_INITIALIZED
            ESCAPE_IF_NOT_ENCODED
            break;
        case 'T':
            ESCAPE_IF_NOT_INITIALIZED
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
