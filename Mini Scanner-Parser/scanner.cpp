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
    // ��ʼ���ؼ��ֱ�
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
    string str;       // ����ļ�������ַ�
    bool flag = true; // �Ƿ��ӡtokens�����������ʱ����ӡ
    int numLine = 1;  // ��ŵ�ǰ��������һ��

    ifstream in("in.txt");
    if (in) // �и��ļ�
    {
        string line;
        while (getline(in, line) && line != "#") // line�в�����ÿ�еĻ��з�
        {
            str += line;
            str += "\n";
        }
    }
    else // û�и��ļ�
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
            while (isDigit(str[i])) // ��������ַ��Ƿ�Ҳ������
            {
                digit += str[i];
                i++;
            }
            i--;                      // i�Լ��ص����һ�������ַ�
            if (!isInConstant(digit)) // ��ǰ�������ڳ�����
            {
                constant.push_back(digit);
            }
            pushConstant(digit); // ���ɳ�����token����
        }
        else if (isAlpha(str[i])) // ������ĸ
        {
            string lett;
            while (isDigit(str[i]) || isAlpha(str[i])) // ��������ַ��Ƿ�Ҳ�����ֻ���ĸ
            {
                lett += str[i];
                i++;
            }
            i--;
            if (isInSymbolTable(lett)) // �Ƿ�Ϊ�ؼ���
                pushSymbol(lett);      // ���ɹؼ��ֵ�token����
            else
            {
                if (!isInID(lett))
                {
                    identifier.push_back(lett); // �Ƿ��ڱ�ʶ������
                }
                pushID(lett); // ���ɱ�ʶ����token����
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
            if (isInSpecSymbolTable(lett)) // �Ƿ���������ű�
                pushSpecSymbol(lett);      // �������������޷���token����
            else
            {
                flag = false; // �ʷ�δ����ĵ��ʷ��ţ����������Ϣ�ʹ�������
                os << " Error! Undefined character in a lexicon in LINE " << numLine << endl;
            }
        }
    }
    if (flag)
    {
        os << "ת����ĵ��ʴ���" << endl;
        for (auto t : tokens)
        {
            if (t.second != NIL)
                os << "<" << t.first << ", " << t.second << ">";
            else
                os << "<" << t.first << ">";
        }
        os << endl
            << "�ؼ��ֱ�" << endl;
        os << setw(20) << "���ʷ���" << setw(20) << "������" << endl;
        for (auto p : symbolTable)
        {
            os << setw(20) << p.first << setw(20) << p.second << endl;
        }

        os << endl
            << "�����/���޷���" << endl;
        os << setw(20) << "���ʷ���" << setw(20) << "������" << endl;
        for (auto p : specialSymbolTable)
        {
            os << setw(20) << p.first << setw(20) << p.second << endl;
        }

        os << endl
            << "��ʶ����������Ϊ1����" << endl;
        os << setw(20) << "��ʶ����" << setw(20) << "����ֵ" << endl;
        for (int i = 0; i < identifier.size(); i++)
        {
            os << setw(20) << identifier[i] << setw(20) << i + 1 << endl;
        }

        os << endl
            << "������������Ϊ2����" << endl;
        os << setw(20) << "����ֵ" << setw(20) << "����ֵ" << endl;
        for (int i = 0; i < constant.size(); i++)
        {
            os << setw(20) << constant[i] << setw(20) << i + 1 << endl;
        }
    }
    os.close();
    return 0;
}