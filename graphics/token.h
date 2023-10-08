#include <math.h>
#include <string.h>
#include <windows.h>
enum typeTok
{
    TOKEN_PLUS, TOKEN_MINUS, TOKEN_MULTIPLY, TOKEN_DIVIDE, TOKEN_POWER, TOKEN_SIN,
    TOKEN_COS, TOKEN_TG, TOKEN_CTG, TOKEN_ARCSIN, TOKEN_ARCCOS, TOKEN_ARCTG, TOKEN_ARCCTG, TOKEN_LG, TOKEN_LN,
    TOKEN_SQRT, TOKEN_X, TOKEN_OPEN_BRACKET, TOKEN_CLOSE_BRACKET, TOKEN_NUMBER, TOKEN_UNARMINUS
};

struct TOKEN
{
    double num;
    struct TOKEN* next;
    enum typeTok type;
};

struct BinParce
{
    struct TOKEN* this;
    struct BinParce* right;
    struct BinParce* left;
};

struct TSTACK
{
    unsigned char priority;
    struct TSTACK* next;
    struct TOKEN* this;
};

void Tparse(char*);////////////////////////////////////
void Taddtoken(enum typeTok);/////////////////////////////
void Taddnum(double);////////////////////////

void Tsort();/////////////////
void TinitTree();
void TinitNode(struct BinParce*);

void TClear();///////////////////////////////
void TCheck();/////////////////////////////
void TClearTree(struct BinParce*);////////////////////


struct TOKEN* TPop();/////////////////////////////////////////////////////
void TPush(struct TOKEN*, int);////////////////////////////////////////
double Tcalculate(double X, struct BinParce*);////////////////////////
