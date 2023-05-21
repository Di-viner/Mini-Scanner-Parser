#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>

using namespace std;
extern unordered_map<string, int> symbolTable;
extern unordered_map<string, int> specialSymbolTable;
extern vector<string> identifier;
extern vector<string> constant;
extern vector<pair<int, int>> tokens;
extern int scanner();

int lookAhead; // 存放当前读出的输入符号的类别编码
int k = 0;     // 存放当前读出的输入符号的下标，即lookAhead = tokens[k].first

void error();
void get_w();

void PR();
void VDSS();
void VDS();
void TYPE();
void ES();
void ESS();
void IDS();
void WS();
void IFS();
void AS();
void RE();
void ROP();
void E();
void E1();
void T();
void T1();
void F();
void OP1();
void OP2();

void grammar();

void error()
{
    cout << "Expression error in position " << k << endl;
    cout << "Current words: ";
    int classNum = tokens[k].first;
    int inNum = tokens[k].second;
    switch (classNum)
    {
    case 1:
        cout << "(" << tokens[k].first << ", " << tokens[k].second << ") ";
        cout << identifier[inNum - 1] << endl;
        break;
    case 2:
        cout << "(" << tokens[k].first << ", " << tokens[k].second << ") ";
        cout << constant[inNum - 1] << endl;
        break;
    default:
        cout << "(" << tokens[k].first << ")";
        for (auto p : symbolTable)
            if (p.second == classNum)
                cout << p.first << endl;
        for (auto p : specialSymbolTable)
            if (p.second == classNum)
                cout << p.first << endl;
        break;
    }
    exit(0);
}

void get_w()
{
    k++;
    if (k < tokens.size())
        lookAhead = tokens[k].first;
}

void PR()
{
    // 1. < PR > → <VDSS> <ESS>
    VDSS();
    ESS();
}

void VDSS()
{
    // 2.<VDSS> → <VDS><VDSS>| ε
    if (lookAhead == 9)
    {
        VDS();
        VDSS();
    }
}

void ESS()
{
    // 6.<ESS> → <ES><ESS>|ε
    if (lookAhead == 1 || lookAhead == 3 || lookAhead == 6)
    {
        ES();
        ESS();
    }
}

void VDS()
{
    // 3.<VDS> → <TYPE><id><IDS>;
    TYPE();
    if (lookAhead == 1)
    {
        get_w();
        IDS();
        if (lookAhead == 57)
        {
            get_w();
        }
        else
        {
            error();
        }
    }
    else
    {
        error();
    }
}

void TYPE()
{
    // 5.<TYPE> → int
    if (lookAhead == 9)
    {
        get_w();
    }
    else
    {
        error();
    }
}

void IDS()
{
    // 4.<IDS> → ,<id><IDS>| ε
    if (lookAhead == 56)
    {
        get_w();
        if (lookAhead == 1)
        {
            get_w();
            IDS();
        }
    }
}

void ES()
{
    // 7.<ES> → <WS>|<IFS>|<AS>
    if (lookAhead == 6) // FIRST(<WS>) = while, while的类别码为6
    {
        WS();
    }
    else if (lookAhead == 3) // FIRST(<IFS>) = if, if的类别码为3
    {
        IFS();
    }
    else if (lookAhead == 1) // FIRST(<AS>) = <id>, <id>的类别码为1
    {
        AS();
    }
    else
    {
        error();
    }
}

void WS()
{
    // 8.<WS> → while(<RE>){<ESS>}
    if (lookAhead == 6)
    {
        get_w();
        if (lookAhead == 52)
        {
            get_w();
            RE();
            if (lookAhead == 53)
            {
                get_w();
                if (lookAhead == 54)
                {
                    get_w();
                    ESS();
                    if (lookAhead == 55)
                    {
                        get_w();
                    }
                    else
                    {
                        error();
                    }
                }
                else
                {
                    error();
                }
            }
            else
            {
                error();
            }
        }
        else
        {
            error();
        }
    }
    else
    {
        error;
    }
}

void IFS()
{
    // 9.<IFS> → if(<RE>){<ESS>}else{<ESS>}
    if (lookAhead == 3)
    {
        get_w();
        if (lookAhead == 52)
        {
            get_w();
            RE();
            if (lookAhead == 53)
            {
                get_w();
                if (lookAhead == 54)
                {
                    get_w();
                    ESS();
                    if (lookAhead == 55)
                    {
                        get_w();
                        if (lookAhead == 5)
                        {
                            get_w();
                            if (lookAhead == 54)
                            {
                                get_w();
                                ESS();
                                if (lookAhead == 55)
                                {
                                    get_w();
                                }
                                else
                                {
                                    error();
                                }
                            }
                            else
                            {
                                error();
                            }
                        }
                        else
                        {
                            error();
                        }
                    }
                    else
                    {
                        error();
                    }
                }
                else
                {
                    error();
                }
            }
            else
            {
                error();
            }
        }
        else
        {
            error();
        }
    }
    else
    {
        error();
    }
}

void AS()
{
    // 10.<AS> → <id> = <E>;
    if (lookAhead == 1)
    {
        get_w();
        if (lookAhead == 37)
        {
            get_w();
            E();
            if (lookAhead == 57)
            {
                get_w();
            }
            else
            {
                error();
            }
        }
        else
        {
            error();
        }
    }
    else
    {
        error();
    }
}

void RE()
{
    // 11. < RE > → <E><ROP><E>
    E();
    ROP();
    E();
}

void ROP()
{
    // 12.<ROP> → <|< =|>|> =|==|!=
    switch (lookAhead)
    {
    case 35:
    case 36:
    case 38:
    case 39:
    case 40:
    case 41:
        get_w();
        break;
    default:
        error();
        break;
    }
}

void E()
{
    // 13.<E> → <T><E1>
    T();
    E1();
}

void E1()
{
    // 14.<E1> → <OP1><T><E1>|ε
    if (lookAhead == 31 || lookAhead == 32)
    {
        OP1();
        T();
        E1();
    }
}

void T()
{
    // 15.<T> → <F><T1>
    F();
    T1();
}

void T1()
{
    // 16.<T1> → <OP2><F><T1>|ε
    if (lookAhead == 33 || lookAhead == 34)
    {
        OP2();
        F();
        T1();
    }
}

void F()
{
    // 17.<F> →(E)|<id>|<digits>
    if (lookAhead == 52) // 识别"("
    {
        get_w();
        E();
        if (lookAhead == 53) // 识别")"
        {
            get_w();
        }
        else
        {
            error();
        }
    }
    else if (lookAhead == 1 || lookAhead == 2) // 识别标识符或常数
    {
        get_w();
    }
    else
    {
        error();
    }
}

void OP1()
{
    // 18.<OP1> → +|-
    if (lookAhead == 31 || lookAhead == 32)
    {
        get_w();
    }
    else
    {
        error();
    }
}

void OP2()
{
    // 19.<OP2> → *| /
    if (lookAhead == 33 || lookAhead == 34)
    {
        get_w();
    }
    else
    {
        error();
    }
}

void grammar()
{
    PR();
    if (k == tokens.size())
        cout << "Syntax check passed and the program is correct." << endl;
}