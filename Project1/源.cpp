#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#define MAX 200

struct Tokens {
    char token[20];
    int kind;
    int isend;
} ;

Tokens tokens[MAX];

struct Table {
    char symbol[20];
    union {
        int i;
        float f;
    } value;
    int is_int;             //0代表无初值，1 表示int ,2表示float
};

Table table[MAX];

float after_equal(int start, int is_int, int row);
int is_have_f;
int increase_long;
int read(char file[]) {   //读取文件
    FILE* fp = NULL;
    int isfloat = 0;
    char ch;
    int i = 0;
    int j = 0;
    fp = fopen(file, "r");
    if (fp == NULL) {
        printf("该文件不存在");
        exit(1);
    }
    else {
        fscanf(fp, "%c", &ch);
        while (ch != '.') {                    
            if ((ch <= 'Z') && (ch >= 'A') || (ch <= 'z') && (ch >= 'a')) {             //变量检测
                tokens[i].token[0] = ch;
                fscanf(fp, "%c", &ch);
                while (ch != ' ' && ch != ';' && ch != '+' && ch != '-' && ch != '/' && ch != '*' && ch != '=' && ch != '.' && ch != '(' && ch != ')') {
                    j++;
                    tokens[i].token[j] = ch;
                    fscanf(fp, "%c", &ch);
                }
                if (strcmp(tokens[i].token, "float") == 0 || strcmp(tokens[i].token, "int") == 0) {
                    tokens[i].kind = 1;              //1表示关键字
                }
                else if(strcmp(tokens[i].token ,"write") == 0) {
                    tokens[i].kind = 7;            //7表示write
                } else tokens[i].kind = 2;           //2表示变量名
                j = 0;
                i++;
            }
            else if (ch >= '0' && ch <= '9') {                                        //数字
                tokens[i].token[0] = ch;
                fscanf(fp, "%c", &ch);
                while (ch != ' ' && ch != ';' && ch != '+' && ch != '-' && ch != '/' && ch != '*' && ch != '=' 
                    && ch != '.' && ch != '(' && ch != ')') {
                    if ((ch <= 'Z') && (ch >= 'A') || (ch <= 'z') && (ch >= 'a')) {
                        printf("变量输入错误");
                        exit(1);
                    }
                    j++;
                    tokens[i].token[j] = ch;
                    fscanf(fp, "%c", &ch);
                }
                if (ch == '.') {
                    if (!fscanf(fp, "%c", &ch)) {
                        ch = '.';                  
                    }
                    else {
                        tokens[i].kind = 3;         //3表示为浮点数
                        j++;
                        tokens[i].token[j] = '.';
                        while (ch != ' ' && ch != ';' && ch != '+' && ch != '-' && ch != '/' && ch != '*' && ch != '='
                            && ch != '.' && ch != '(' && ch != ')') {
                            if ((ch <= 'Z') && (ch >= 'A') || (ch <= 'z') && (ch >= 'a')) {
                                printf("变量输入错误");
                                exit(1);
                            }
                            j++;
                            tokens[i].token[j] = ch;
                            fscanf(fp, "%c", &ch);
                        }
                        j = 0;
                        i++;
                        continue;
                    }
                }
                tokens[i].kind = 4;                //4表示为整型
                j = 0;
                i++;
            }
            else if (ch == '+' || ch == '-' || ch == '/' || ch == '*' || ch == '=' || ch == '(' || ch == ')') {
                tokens[i].token[0] = ch;
                tokens[i].kind = 5;               //5表示运算符
                i++;
                fscanf(fp, "%c", &ch);
            }
            else if (ch == ';') {
                tokens[i].token[0] = ch;
                tokens[i].kind = 6;               //6表示分号
                i++;
                fscanf(fp, "%c", &ch);
            }
            else if (ch == ' ' || ch == '\n') {
                fscanf(fp, "%c", &ch);
            }
            
        }
    }
    fclose(fp);
    return i;
}

int is_declear(char a[]) {             //是否声明
    int i = 0;
    while (table[i].symbol[0] != '\0') {
        if (strcmp(table[i].symbol,a) == 0) {
            break;
        }
        i++;
    }
    return i;
}

void grammar(int row) {
    int i = 0;
    int j = 0;
    int z = 0;
    int t_symbol = 0;
    int is_int = 0;
    while (i <= row) {                                               //每一行
        if (tokens[i].kind == 1) {                                   //遇到关键字float,int 时
            if (strcmp(tokens[i].token, "int") == 0) {
                is_int = 1;
            }
            else is_int = 2;
            i++;                                                     //读取下一个
            if (tokens[i].kind == 2) {                               //变量
                while (tokens[i].token[j] != '\0') {
                    table[z].symbol[j] = tokens[i].token[j];
                    j++;
                }
                table[z].is_int = is_int;
            }
            else {
                printf("声明错误");
                exit(1);
            }
            z++;
            i++;                                                     //继续读取下一个
            j = 0;
            if (tokens[i].kind == 6) {                               //如果下一个为分号，则下一行
                i++;
                continue;
            }
            else {
                printf("缺少;");
                exit(1);
            }
        }
        else if (tokens[i].kind == 2) {
            if (t_symbol = (is_declear(tokens[i].token) + 1)) {
                i++;
                if (tokens[i].token[0] == '=') {
                    if (table[t_symbol - 1].is_int == 2) {
                        table[t_symbol - 1].value.f = after_equal(i + 1, table[t_symbol - 1].is_int, row);
                        /*table[t_symbol - 1].is_int = 2;*/
                    }
                    else {
                        table[t_symbol - 1].value.i = (int)after_equal(i + 1, table[t_symbol - 1].is_int, row);
                        /*table[t_symbol - 1].is_int = 1;*/
                    }
                }
            }
            else {
                printf("变量%s未声明",tokens[i].token);
                exit(1);
            }
            i = increase_long + 1;
        }
        else if (tokens[i].kind == 7) {                //遇到write函数时
            i++;
            if (tokens[i].token[0] == '(') {
                i++;
            }
            else {
                printf("write缺少“(”");
                exit(1);
            }
     
            if (tokens[i].kind == 2) {
                t_symbol = is_declear(tokens[i].token);
                if (table[t_symbol].is_int == 0) {
                    printf("变量无初值");
                    exit(1);
                }
                else if (table[t_symbol].is_int == 1) {
                    printf("%d\n",table[t_symbol].value.i);
                }
                else if (table[t_symbol].is_int == 2) {
                    printf("%g\n",table[t_symbol].value.f);
                }
            }
            else {
                printf("无参数");
                exit(1);
            }
            i++;
            if (tokens[i].token[0] == ')') {
                i++;
            }
            else {
                printf("write缺少“)”");
                exit(1);
            } 
            if (tokens[i].token[0] == ';') {
                i++;
            }
            else if (i >= row) {
                break;
            }
            else {
                printf("write缺少; or .");
                exit(1);
            }
        }
    }

}

/* 栈结构体 */
typedef struct {
    short tag; //判断类型
    union {
        float* idata;
        char* cdata;
    }u;
    int top;
}stack;

/* 表达式结构体 */
typedef struct {
    short tag;//判断类型
    union {
        float num;
        char ch;
    }u;
}EXPRESSION;

/* 栈操作函数声明 */
void push(stack*, float);
float pop(stack*);
float top(stack*);
int priority(char);
bool isnumber(char);
bool isempty(stack*);

float calculate(char str[])
{
    int i, j, len, cnt;//字符串下标、长度变量
    int w = 1;

    stack istack, cstack;//数字栈 操作符栈

    istack.tag = 1;
    istack.u.idata = (float*)malloc(sizeof(float) * 50);
    istack.top = -1;  //操作符栈空

    cstack.tag = 0;
    cstack.u.cdata = (char*)malloc(sizeof(char) * 50);
    cstack.top = -1; //数字栈空

    EXPRESSION out[50];//后缀表达式结构体数组    
    cnt = 0;//后缀表达式数组下标
    float sum = 0;//累加数字
    float xiaoshu = 0;
    len = strlen(str);//读取的中缀表达式字符串长度
    int flag = 0;//是数字标识

    /* 1.中缀表达式转后缀表达式(用操作符栈) */
    for (i = 0; i <= len; i++)// 100*(2+3)-4   100 2 3 + * 4 -   
    {
        /* 从字符串读取的字符为数字，插入到后缀表达式结构体数组 */
        if (isnumber(str[i])) {
            sum = sum * 10 + str[i] - '0';//累加数字            
            flag = 1;//是数字标识
            if (str[i + 1] == '.') {
                i++;
                while (isnumber(str[i + w])) {
                    xiaoshu = xiaoshu + (str[i + w] - '0') * pow(0.1, w);
                    w++;
                }
                i = i + w - 1;
                w = 1;
            }
            sum = sum + xiaoshu;
            continue;
        }
        /* 数字进后缀表达式结构体数组的数字 */
        if (flag)
        {
            out[cnt].tag = 1;
            out[cnt++].u.num = sum;
            flag = 0;
            sum = 0;
            xiaoshu = 0;
        }
        /* 读取优先级高的非数字字符(字符')'优先级0) 或者 操作符栈为空 */
        if (priority(str[i]) > priority(top(&cstack)) || isempty(&cstack))
        {
            push(&cstack, str[i]);
            continue;
        }
        /* 读取的字符是右括号，一次处理完括号内数字\操作符\包括'('*/
        if (str[i] == ')')
        {
            while (top(&cstack) != '(')
            {
                out[cnt].tag = 2;
                out[cnt].u.ch = top(&cstack); //操作符进后缀表达式数组的字符
                cnt++;
                pop(&cstack);
            }
            pop(&cstack);// '('
            continue;
        }
        /* 处理括号外数字及操作符*/
        while (!isempty(&cstack) && top(&cstack) != '(')
        {
            out[cnt].tag = 2;
            out[cnt++].u.ch = top(&cstack);//操作符进后缀表达式数组的字符
            pop(&cstack);
        }
        /* 低级别操作符入栈 */
        if (str[i])
            push(&cstack, str[i]);
    }
    ///* 打印后缀表达式 */  //100 2 3 + * 4 -   100*(2+3)-4
    //for (i = 0; i < cnt; ++i)
    //{
    //    if (out[i].tag == 1)
    //        printf("%d ", out[i].u.num);
    //    else printf("%c ", out[i].u.ch);
    //}
    //printf("\n");  
    /* 2.根据后缀表达式计算(用操作符栈\数字栈) */
    for (i = 0; i < cnt; i++)
    {
        /* 从后缀表达式结构体数组读取，是数字压数字栈，
        然后判断结构体数组是否读完 */
        if (out[i].tag == 1) {
            push(&istack, out[i].u.num);
            continue;
        }
        else if (out[i].u.ch == '+') {//数字栈顶元素加到下面元素并弹出栈顶元素
            istack.u.idata[istack.top - 1] += top(&istack);
            pop(&istack);
        }
        else if (out[i].u.ch == '-') {//同理减到下面元素并弹出栈顶元素
            istack.u.idata[istack.top - 1] -= top(&istack);
            pop(&istack);
        }
        else if (out[i].u.ch == '*') { //同理乘到下面元素并弹出栈顶元素
            istack.u.idata[istack.top - 1] *= top(&istack);
            pop(&istack);
        }
        else if (out[i].u.ch == '/') { //同理除到下面元素并弹出栈顶元素
            istack.u.idata[istack.top - 1] /= top(&istack);
            pop(&istack);
        }
        else if (out[i].u.ch == '^') {//同理幂到下面元素并弹出栈顶元素
            istack.u.idata[istack.top - 1]
                = pow(istack.u.idata[istack.top - 1], top(&istack));
            pop(&istack);
        }

    }
    return(istack.u.idata[0]);
}

/* 是否栈空函数 */
bool isempty(stack* s) {
    return (s->top == -1);
}

/* 压栈函数 */
void push(stack* s, float ch) {
    if (s->tag == 0) {
        s->u.cdata[++s->top] = (char)ch;
    }
    else {
        s->u.idata[++s->top] = ch;
    }
}

/* 弹栈函数 */
float pop(stack* s) {
    if (s->tag == 0) {
        return s->u.cdata[s->top--];
    }
    return s->u.idata[s->top--];
}

/* 取栈顶元素函数 */
float top(stack* s) {
    if (s->tag == 0) {
        return s->u.cdata[s->top];
    }
    return s->u.idata[s->top];
}

/* 是否数字函数 */
bool isnumber(char ch) {
    return (ch >= '0' && ch <= '9');
}

/* 操作符优先级函数 */
int priority(char ch) {
    if (ch == '+' || ch == '-')
        return 1;
    if (ch == '*' || ch == '/')
        return 2;
    if (ch == '^')
        return 3;
    if (ch == '(')
        return 4;
    return 0;
}


float after_equal(int start, int is_int ,int row) {
    char str[100];
    memset(str, 0, 50);
    int kuohaoshu=0;
    int i = start;
    int t_symbol = 0;
    char s[20];
    char ss[20];
    memset(ss, 0, 20);
    memset(s, 0, 20);
    while (tokens[i].kind != 6 && i != row) {
        if (tokens[i].token[0] == '(') {
            kuohaoshu++;
            strcat(str, tokens[i].token);
            i++;
        }
        else if (tokens[i].token[0] == ')') {
            kuohaoshu--;
            strcat(str, tokens[i].token);
            i++;
        }
        //else if (tokens[i].token[0] == '*' || tokens[i].token[0] == '-' || tokens[i].token[0] == '+' || tokens[i].token[0] == '/') {
        //    if (i == start) {
        //        printf("双目运算符缺少对象");
        //        exit(1);
        //    }
        //    if ((isnumber(tokens[i - 1].token[0]) || tokens[i-1].kind == 2 || tokens[i-1].token[0] == ')') 
        //        && (isnumber(tokens[i + 1].token[0]) || tokens[i + 1].kind == 2 || tokens[i + 1].token[0] == '('))   //左边数字或者变量右边括号数字或者变量
        //    {
        //        strcat(str, tokens[i].token);
        //        i++;
        //    }
        //    else {
        //        printf("双目运算符缺少对象");
        //        exit(1);
        //    }
        //} 
        else if (tokens[i].token[0] == '*' || tokens[i].token[0] == '+' || tokens[i].token[0] == '/') {
            if (i == start) {
                printf("双目运算符缺少对象");
                exit(1);
            }
            if ((isnumber(tokens[i - 1].token[0]) || tokens[i - 1].kind == 2 || tokens[i - 1].token[0] == ')')
                && (isnumber(tokens[i + 1].token[0]) || tokens[i + 1].kind == 2 || tokens[i + 1].token[0] == '('))   //左边数字或者变量右边括号数字或者变量
            {
                strcat(str, tokens[i].token);
                i++;
            }
            else {
                printf("双目运算符缺少对象");
                exit(1);
            }
        }
        else if (tokens[i].token[0] == '-') {
            if ((isnumber(tokens[i - 1].token[0]) || tokens[i - 1].kind == 2 || tokens[i - 1].token[0] == ')')
                && (isnumber(tokens[i + 1].token[0]) || tokens[i + 1].kind == 2 || tokens[i + 1].token[0] == '('))   //左边数字或者变量右边括号数字或者变量
            {
                strcat(str, tokens[i].token);
                i++;
            }
            else if ((isnumber(tokens[i + 1].token[0]) || tokens[i + 1].kind == 2)) {
                strcat(str,"0");
                strcat(str, tokens[i].token);
                i++;
            }
        }
        else if (tokens[i].kind == 3) {
            strcat(str, tokens[i].token);
            i++;
            is_have_f = 1;
        }
        else if  (tokens[i].kind == 4) {
            strcat(str, tokens[i].token);
            i++;
        }
        //else if (tokens[i].kind == 4 && is_int == 2) {
        //    printf("整型不可以转换成浮点型");
        //    exit(1);
        //}
        else if (tokens[i].kind == 2) {
            /*if (t_symbol = (is_declear(tokens[i].token) + 1)) {
                  if (table[t_symbol - 1].is_int == 0) {
                      printf("变量无初值");
                      exit(1);
                  }
                  else if (table[t_symbol - 1].is_int == 1) {
                      itoa(table[t_symbol - 1].value.i, s, 10);
                  }
                  else if (table[t_symbol - 1].is_int == 2) {
                      ultoa(table[t_symbol - 1].value.f, s, 10);
                  }
                  strcat(str, s);
            }*/
            if (t_symbol = (is_declear(tokens[i].token) + 1)) {
                if (table[t_symbol - 1].is_int == 0) {
                    printf("变量无初值");
                    exit(1);
                }
                else if (table[t_symbol - 1].is_int == 1) {
                    if(table[t_symbol - 1].value.i >= 0) 
                    itoa(table[t_symbol - 1].value.i, s, 10);
                    else {
                        itoa(table[t_symbol - 1].value.i, ss, 10);
                        memset(s, 0, 20);
                        s[0] = '(';
                        s[1] = '0';
                        strcat(s, ss);
                        strcat(s, ")");
                    }
                }
                else if (table[t_symbol - 1].is_int == 2) {
                    if(table[t_symbol -1].value.f >= 0)
                    ultoa(table[t_symbol - 1].value.f, s, 10);
                    else {
                        itoa(table[t_symbol - 1].value.i, ss, 10);
                        memset(s, 0, 20);
                        s[0] = '(';
                        s[1] = '0';
                        strcat(s, ss);
                        strcat(s, ")");
                    }
                }
                strcat(str, s);
            }
            else {
                printf("变量未声明");
                exit(1);
            }
            i++;
        }
        else {
            printf("算数式子不合标准");
            exit(1);
        }
    }
    increase_long = i;
    if (kuohaoshu == 0) {
         return calculate(str);
    }
    else if (kuohaoshu > 0) {
        printf("缺少“)”");
        exit(1);
    }
    else {
        printf("缺少“(”");
        exit(1);
    }
}

int main(int argc, char* argv[]) {
    int row;
    if (argc != 2)
    { 
        printf("输入格式错误");
        exit(1);
    }
    else {
        row = read(argv[1]);
        grammar(row);
        //int i = 0;
        //while (row != i) {
        //    printf("%s ", tokens[i].token);
        //    i++; //测试read函数是否正确
        //}
       /* grammar(row);*/
    }
    return 0;
}
