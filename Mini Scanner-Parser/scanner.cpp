#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>

using namespace std;

#define CONST 2
#define ID 1
#define NIL -1
unordered_map<string, int> symbolTable;
unordered_map<string, int> specialSymbolTable;
vector<pair<int, int>> tokens;
vector<string> identifier;
vector<string> constant;

#pragma region MyRegion

bool isDigit(char c)
{
    return c >= '0' && c <= '9';
}

bool isAlpha(char c)
{
    return (c >= 65 && c <= 90) || (c >= 97 && c <= 122);
}

bool isInSymbolTable(string s)
{
    return !(symbolTable.find(s) == symbolTable.end());
}

bool isInSpecSymbolTable(string s)
{
    return !(specialSymbolTable.find(s) == specialSymbolTable.end());
}

bool isInID(string s)
{
    return !(find(identifier.begin(), identifier.end(), s) == identifier.end());
}

bool isInConstant(string s)
{
    return !(find(constant.begin(), constant.end(), s) == constant.end());
}

void pushSymbol(string s)
{
    int n = symbolTable.find(s)->second;
    tokens.push_back({ n, NIL });
}

void pushSpecSymbol(string s)
{
    int n = specialSymbolTable.find(s)->second;
    tokens.push_back({ n, NIL });
}

void pushID(string s)
{
    int n = find(identifier.begin(), identifier.end(), s) - identifier.begin() + 1;
    tokens.push_back({ ID, n });
}

void pushConstant(string s)
{
    int n = find(constant.begin(), constant.end(), s) - constant.begin() + 1;
    tokens.push_back({ CONST, n });
}

void init()
{
    // 初始化关键字表
    symbolTable.insert({ "if", 3 });
    symbolTable.insert({ "else", 5 });
    symbolTable.insert({ "while", 6 });
    symbolTable.insert({ "void", 7 });
    symbolTable.insert({ "main", 8 });
    symbolTable.insert({ "int", 9 });
    symbolTable.insert({ "return", 10 });

    specialSymbolTable.insert({ "!", 30 });
    specialSymbolTable.insert({ "+", 31 });
    specialSymbolTable.insert({ "-", 32 });
    specialSymbolTable.insert({ "*", 33 });
    specialSymbolTable.insert({ "/", 34 });
    specialSymbolTable.insert({ "<", 35 });
    specialSymbolTable.insert({ ">", 36 });
    specialSymbolTable.insert({ "=", 37 });
    specialSymbolTable.insert({ "<=", 38 });
    specialSymbolTable.insert({ ">=", 39 });
    specialSymbolTable.insert({ "!=", 40 });
    specialSymbolTable.insert({ "==", 41 });
    specialSymbolTable.insert({ "(", 52 });
    specialSymbolTable.insert({ ")", 53 });
    specialSymbolTable.insert({ "{", 54 });
    specialSymbolTable.insert({ "}", 55 });
    specialSymbolTable.insert({ ",", 56 });
    specialSymbolTable.insert({ ";", 57 });
}

#pragma endregion

int scanner()
{
    init();
    string str;       // 存放文件读入的字符
    bool flag = true; // 是否打印tokens串，程序出错时不打印
    int numLine = 1;  // 存放当前程序在哪一行

    ifstream in("in.txt");
    if (in) // 有该文件
    {
        string line;
        while (getline(in, line) && line != "#") // line中不包括每行的换行符
        {
            str += line;
            str += "\n";
        }
    }
    else // 没有该文件
    {
        cout << "no such file" << endl;
        in.close();
        return 0;
    }
    in.close();
    ofstream os;
    os.open("out.txt", ios::out);
    if (!os.is_open())
        return 0;
    int length = str.length();
    for (int i = 0; i < length; i++)
    {
        if (str[i] == ' ' || str[i] == '\n' || str[i] == '\t')
        {
            if (str[i] == '\n')
                numLine++;
            continue;
        }
        if (isDigit(str[i]))
        {
            string digit;
            while (isDigit(str[i])) // 读后面的字符是否也是数字
            {
                digit += str[i];
                i++;
            }
            i--;                      // i自减回到最后一个数字字符
            if (!isInConstant(digit)) // 当前常数不在常数表
            {
                constant.push_back(digit);
            }
            pushConstant(digit); // 生成常数的token序列
        }
        else if (isAlpha(str[i])) // 读到字母
        {
            string lett;
            while (isDigit(str[i]) || isAlpha(str[i])) // 读后面的字符是否也是数字或字母
            {
                lett += str[i];
                i++;
            }
            i--;
            if (isInSymbolTable(lett)) // 是否为关键字
                pushSymbol(lett);      // 生成关键字的token序列
            else
            {
                if (!isInID(lett))
                {
                    identifier.push_back(lett); // 是否在标识符表中
                }
                pushID(lett); // 生成标识符的token序列
            }
        }
        else
        {
            string lett;
            lett += str[i];
            switch (str[i])
            {
            case '<':
            case '>':
            case '!':
            case '=':
                i++;
                if (str[i] == '=')
                    lett += str[i];
                else
                    i--;
                break;
            }
            if (isInSpecSymbolTable(lett)) // 是否在特殊符号表
                pushSpecSymbol(lett);      // 生成运算符或界限符的token序列
            else
            {
                flag = false; // 词法未定义的单词符号，报告错误信息和错误行数
                os << " Error! Undefined character in a lexicon in LINE " << numLine << endl;
            }
        }
    }
    if (flag)
    {
        os << "转换后的单词串：" << endl;
        for (auto t : tokens)
        {
            if (t.second != NIL)
                os << "<" << t.first << ", " << t.second << ">";
            else
                os << "<" << t.first << ">";
        }
        os << endl
            << "关键字表：" << endl;
        os << setw(20) << "单词符号" << setw(20) << "类别编码" << endl;
        for (auto p : symbolTable)
        {
            os << setw(20) << p.first << setw(20) << p.second << endl;
        }

        os << endl
            << "运算符/界限符表：" << endl;
        os << setw(20) << "单词符号" << setw(20) << "类别编码" << endl;
        for (auto p : specialSymbolTable)
        {
            os << setw(20) << p.first << setw(20) << p.second << endl;
        }

        os << endl
            << "标识符（类别编码为1）表：" << endl;
        os << setw(20) << "标识符名" << setw(20) << "内码值" << endl;
        for (int i = 0; i < identifier.size(); i++)
        {
            os << setw(20) << identifier[i] << setw(20) << i + 1 << endl;
        }

        os << endl
            << "常数（类别编码为2）表：" << endl;
        os << setw(20) << "常数值" << setw(20) << "内码值" << endl;
        for (int i = 0; i < constant.size(); i++)
        {
            os << setw(20) << constant[i] << setw(20) << i + 1 << endl;
        }
    }
    os.close();
    return 0;
}