#define _USE_MATH_DEFINES
#include "token.h"
#include "help.h"
extern int error;
int printf(const char* format, ...);

struct TOKEN* head=0;
struct TOKEN* buffer[250];
struct TOKEN* final[250];

struct BinParce* top = 0;

struct TSTACK* bottom=0;

unsigned int TokenSize = sizeof(struct TOKEN);
unsigned int Tokens = 0;
unsigned int stackpointer = 0;

void Tparse(char* str)
{
    char* curr;
    while (*str)
    {
        curr = str;
        while (((*curr) >= '0' && (*curr) <= '9') || (*curr) == ',' || (*curr) == '.')curr++;
        if (curr != str) { Taddnum(str_to_num(str, curr)); str = curr; }

        switch (*str)
        {
        case 'x':
        case 'X':Taddtoken(TOKEN_X); break;            
        case '+':Taddtoken(TOKEN_PLUS); break;
        case '-':if(Tokens==0 || (*(str-1))=='(')
                    Taddtoken(TOKEN_UNARMINUS);
                else 
                    Taddtoken(TOKEN_MINUS); break;
        case '*':Taddtoken(TOKEN_MULTIPLY); break;
        case '/':Taddtoken(TOKEN_DIVIDE); break;
        case '(':Taddtoken(TOKEN_OPEN_BRACKET); break;
        case ')':Taddtoken(TOKEN_CLOSE_BRACKET); break;
        case '^':Taddtoken(TOKEN_POWER); break;
        case 'e':
        case 'E':Taddnum(2.718281828459); break;
        case 'p':
        case 'P':if (fract_str_cmp_NR(str, "pi", 2)) { Taddnum(3.141592653); str++; break; }
                else { error = 1; return; };
        case 't':
        case 'T': if (fract_str_cmp_NR(str, "tg", 2)) { Taddtoken(TOKEN_TG); str += 1; break;}
                    else {error=7;return;}
        case 'l':
        case 'L':
            if (fract_str_cmp_NR(str, "lg", 2)) { Taddtoken(TOKEN_LG); str++; }
            else
                if (fract_str_cmp_NR(str, "ln", 2)) { Taddtoken(TOKEN_LN); str++; }
                else 
                { 
                    error = 2; return; 
                };
            break;
        case 'c':
        case 'C':if (fract_str_cmp_NR(str, "cos", 3)) { Taddtoken(TOKEN_COS); str += 2; break; }
                 else if(fract_str_cmp_NR(str, "ctg", 3)){Taddtoken(TOKEN_CTG); str += 2; break;}
                else { error = 3; return; };
        case 's':
        case 'S':if (fract_str_cmp_NR(str, "sin", 3)) { Taddtoken(TOKEN_SIN); str += 2; break; }
                else if(fract_str_cmp_NR(str, "sqrt", 4)){Taddtoken(TOKEN_SQRT); str += 3; break;}
                else{ error = 4; return; };
        case 'a':
        case 'A':
            if (fract_str_cmp_NR(str, "arctg", 5)) { Taddtoken(TOKEN_ARCTG); str += 4; }
            else
                if (fract_str_cmp_NR(str, "arcctg", 6)) { Taddtoken(TOKEN_ARCCTG); str += 5; }
                else
                    if (fract_str_cmp_NR(str, "arcsin", 6)) { Taddtoken(TOKEN_ARCSIN); str += 5; }
                    else
                        if (fract_str_cmp_NR(str, "arccos", 6)) { Taddtoken(TOKEN_ARCCOS); str += 5; }
                        else { error = 5; return; }
            break;
        case '\0': break;
             default: {error = 6; return; };
        }str++;
    }
    memset(buffer, 0, 8 * (Tokens+10));
    memset(final, 0, 8 * Tokens);
    struct TOKEN* current = head;
    for (int i=0;current;current = current->next,i++)
        buffer[i] = current;
}

void Taddtoken(enum typeTok tektype)
{
    Tokens++;
    if (!head) { head = malloc(TokenSize); head->type = tektype; head->next = 0; head->num = 0; }
    else
    {
        struct TOKEN* current = head;
        while (current->next)current = current->next;
        current->next = malloc(TokenSize);
        current = current->next;
        current->type = tektype; current->next = 0; current->num = 0;
    }
};

void Taddnum(double number)
{
    Tokens++;
    if (!head) { head = malloc(TokenSize); head->type = TOKEN_NUMBER; head->next = 0; head->num = number; }
    else
    {
        struct TOKEN* current = head;
        while (current->next)current = current->next;
        current->next = malloc(TokenSize);
        current = current->next;
        current->type = TOKEN_NUMBER; current->next = 0; current->num = number;
    }
}

void TClear()
{
    struct TOKEN* curr1, * curr2;
    if(head)
        {
            if (!(head->next)){free(head); head=0;}
        else
        {
            curr1 = head; curr2 = head->next;
            while (curr2) 
            { 
                free(curr1); 
                curr1 = curr2; 
                curr2 = curr2->next; 
            }
            free(curr1);
            head=0;
        }
        }
    if (top)TClearTree(top);
    top=0;
    Tokens=0;
    stackpointer=0;
}

double Tcalculate(double X, struct BinParce* current)
{
    switch (current->this->type)
    {
    case TOKEN_UNARMINUS:return -(Tcalculate(X, current->right));
    case TOKEN_PLUS: {return Tcalculate(X, current->left) + Tcalculate(X, current->right); }
    case TOKEN_MINUS: return Tcalculate(X, current->left) - Tcalculate(X, current->right);
    case TOKEN_MULTIPLY:return Tcalculate(X, current->left) * Tcalculate(X, current->right);
    case TOKEN_DIVIDE:return Tcalculate(X, current->left) / Tcalculate(X, current->right);
    case TOKEN_POWER:return pow(Tcalculate(X, current->left), Tcalculate(X, current->right));
    case TOKEN_SIN:return sin(Tcalculate(X, current->right));
    case TOKEN_COS:return cos(Tcalculate(X, current->right));
    case TOKEN_TG:return tan(Tcalculate(X, current->right));
    case TOKEN_CTG:return 1.0 / tan(Tcalculate(X, current->right));
    case TOKEN_ARCSIN:return asin(Tcalculate(X, current->right));
    case TOKEN_ARCCOS:return acos(Tcalculate(X, current->right));
    case TOKEN_ARCTG: return atan(Tcalculate(X, current->right));
    case TOKEN_ARCCTG:return M_PI / 2.0 - atan(Tcalculate(X, current->right));
    case TOKEN_LG: return log(Tcalculate(X, current->right));
    case TOKEN_LN:return log(Tcalculate(X, current->right)) / log(10);
    case TOKEN_SQRT: return sqrt(Tcalculate(X, current->right));
    case TOKEN_X: return X;
    case TOKEN_NUMBER: {return current->this->num; }
    }
}

void TClearTree(struct BinParce* ToFree)
{
    if (ToFree->left)TClearTree(ToFree->left);
    if (ToFree->right)TClearTree(ToFree->right);
    free(ToFree);
}

struct TOKEN* TPop()
{
    if (bottom)
    {
        struct TOKEN* answer = bottom->this;
        struct TSTACK* old = bottom;
        bottom = bottom->next;
        free(old);
        return answer;
    }
    else return 0;
}

void TPush(struct TOKEN* add, int pri)
{
    if (bottom)
    {
        struct TSTACK* new = malloc(sizeof(struct TSTACK));
        new->this = add;
        new->priority = pri;
        new->next = bottom;
        bottom = new;
    }
    else
    {
        bottom = malloc(sizeof(struct TSTACK));
        bottom->this = add;
        bottom->priority = pri;
        bottom->next = 0;
    }

}

void Tsort()
{
    int brackets = 0, j = 0;
    struct TOKEN* current;
    struct TOKEN* check;
    char flag = 0;

    for (int i = 0; i < Tokens; i++)
    {
        flag = 0;
        check = buffer[i];
        switch (buffer[i]->type)
        {
        case TOKEN_OPEN_BRACKET:TPush(buffer[i], 10 + 10 * brackets); brackets++; flag = 1; break;
        case TOKEN_CLOSE_BRACKET: while ((current = TPop())->type != TOKEN_OPEN_BRACKET) { final[j] = current; j++; }brackets--;flag = 1; break;
        case TOKEN_X:
        case TOKEN_NUMBER: final[j] = buffer[i]; j++; flag = 1; break;
        }
        int priority = 0;
        if (flag)continue;
        switch (buffer[i]->type)
        {
        case TOKEN_PLUS:case TOKEN_MINUS:       priority = 1; break;
        case TOKEN_MULTIPLY: case TOKEN_DIVIDE: priority = 2; break;
        case TOKEN_POWER:                       priority = 3; break;
        case TOKEN_SIN:case TOKEN_COS:case TOKEN_TG:case TOKEN_CTG:case TOKEN_ARCSIN:case TOKEN_ARCCOS:
        case TOKEN_ARCTG:case TOKEN_ARCCTG:case TOKEN_LG:case TOKEN_LN:case TOKEN_SQRT:
            priority = 3; break;
        case TOKEN_UNARMINUS: priority = 5; break;
        }

        if (bottom)
        {
            if ((priority + 10 * brackets) <= bottom->priority) 
            { 
                current = TPop(); 
                {final[j] = current; j++; }
                if(bottom)
                {
                    if (bottom->this->type == buffer[i]->type == TOKEN_MINUS)buffer[i]->type = TOKEN_PLUS;
                else if (bottom->this->type == TOKEN_MINUS && buffer[i]->type ==TOKEN_PLUS )buffer[i]->type = TOKEN_MINUS;
                }
                TPush(buffer[i], priority + 10 * brackets); 
            }
            else { TPush(buffer[i], priority + 10 * brackets); }
        }
        else TPush(buffer[i], priority + 10 * brackets);

    }
    while (current = TPop()) { final[j] = current; j++;}
    stackpointer = j - 1;
}

void TinitTree()
{
    printf("\n");
    for (int i = 0; i < Tokens; i++)if(buffer[i])printf ("%d ",buffer[i]->type);
    printf("\n");
    for (int i = 0; i < Tokens; i++)if(final[i])printf ("%d ",final[i]->type);
    top = malloc(sizeof(struct BinParce));
    memset(top,0, sizeof(struct BinParce));
    top->this = final[stackpointer];
    stackpointer--;
    if (top->this->type == TOKEN_NUMBER || top->this->type == TOKEN_X) return;

    top->right = malloc(sizeof(struct BinParce));
    memset(top->right, 0, sizeof(struct BinParce));
    TinitNode(top->right);

    if(top->this->type == TOKEN_PLUS || top->this->type == TOKEN_MINUS || 
    top->this->type == TOKEN_MULTIPLY || top->this->type == TOKEN_DIVIDE || 
    top->this->type == TOKEN_POWER)
    {top->left = malloc(sizeof(struct BinParce));
    memset(top->left, 0, sizeof(struct BinParce));
    TinitNode(top->left);}
}

void TinitNode(struct BinParce* tek)
{
    if (stackpointer == -1) return;

    tek->this = final[stackpointer];
    stackpointer--;
    if (tek->this->type == TOKEN_NUMBER || tek->this->type == TOKEN_X) return;

    tek->right = malloc(sizeof(struct BinParce));
    memset(tek->right, 0, sizeof(struct BinParce));
    TinitNode(tek->right);

    if(tek->this->type == TOKEN_PLUS || tek->this->type == TOKEN_MINUS || 
    tek->this->type == TOKEN_MULTIPLY || tek->this->type == TOKEN_DIVIDE || 
    tek->this->type == TOKEN_POWER)
    {tek->left = malloc(sizeof(struct BinParce));
    memset(tek->left, 0, sizeof(struct BinParce));
    TinitNode(tek->left);}
}