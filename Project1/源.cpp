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
    int is_int;             //0�����޳�ֵ��1 ��ʾint ,2��ʾfloat
};

Table table[MAX];

float after_equal(int start, int is_int, int row);
int is_have_f;
int increase_long;
int read(char file[]) {   //��ȡ�ļ�
    FILE* fp = NULL;
    int isfloat = 0;
    char ch;
    int i = 0;
    int j = 0;
    fp = fopen(file, "r");
    if (fp == NULL) {
        printf("���ļ�������");
        exit(1);
    }
    else {
        fscanf(fp, "%c", &ch);
        while (ch != '.') {                    
            if ((ch <= 'Z') && (ch >= 'A') || (ch <= 'z') && (ch >= 'a')) {             //�������
                tokens[i].token[0] = ch;
                fscanf(fp, "%c", &ch);
                while (ch != ' ' && ch != ';' && ch != '+' && ch != '-' && ch != '/' && ch != '*' && ch != '=' && ch != '.' && ch != '(' && ch != ')') {
                    j++;
                    tokens[i].token[j] = ch;
                    fscanf(fp, "%c", &ch);
                }
                if (strcmp(tokens[i].token, "float") == 0 || strcmp(tokens[i].token, "int") == 0) {
                    tokens[i].kind = 1;              //1��ʾ�ؼ���
                }
                else if(strcmp(tokens[i].token ,"write") == 0) {
                    tokens[i].kind = 7;            //7��ʾwrite
                } else tokens[i].kind = 2;           //2��ʾ������
                j = 0;
                i++;
            }
            else if (ch >= '0' && ch <= '9') {                                        //����
                tokens[i].token[0] = ch;
                fscanf(fp, "%c", &ch);
                while (ch != ' ' && ch != ';' && ch != '+' && ch != '-' && ch != '/' && ch != '*' && ch != '=' 
                    && ch != '.' && ch != '(' && ch != ')') {
                    if ((ch <= 'Z') && (ch >= 'A') || (ch <= 'z') && (ch >= 'a')) {
                        printf("�����������");
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
                        tokens[i].kind = 3;         //3��ʾΪ������
                        j++;
                        tokens[i].token[j] = '.';
                        while (ch != ' ' && ch != ';' && ch != '+' && ch != '-' && ch != '/' && ch != '*' && ch != '='
                            && ch != '.' && ch != '(' && ch != ')') {
                            if ((ch <= 'Z') && (ch >= 'A') || (ch <= 'z') && (ch >= 'a')) {
                                printf("�����������");
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
                tokens[i].kind = 4;                //4��ʾΪ����
                j = 0;
                i++;
            }
            else if (ch == '+' || ch == '-' || ch == '/' || ch == '*' || ch == '=' || ch == '(' || ch == ')') {
                tokens[i].token[0] = ch;
                tokens[i].kind = 5;               //5��ʾ�����
                i++;
                fscanf(fp, "%c", &ch);
            }
            else if (ch == ';') {
                tokens[i].token[0] = ch;
                tokens[i].kind = 6;               //6��ʾ�ֺ�
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

int is_declear(char a[]) {             //�Ƿ�����
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
    while (i <= row) {                                               //ÿһ��
        if (tokens[i].kind == 1) {                                   //�����ؼ���float,int ʱ
            if (strcmp(tokens[i].token, "int") == 0) {
                is_int = 1;
            }
            else is_int = 2;
            i++;                                                     //��ȡ��һ��
            if (tokens[i].kind == 2) {                               //����
                while (tokens[i].token[j] != '\0') {
                    table[z].symbol[j] = tokens[i].token[j];
                    j++;
                }
                table[z].is_int = is_int;
            }
            else {
                printf("��������");
                exit(1);
            }
            z++;
            i++;                                                     //������ȡ��һ��
            j = 0;
            if (tokens[i].kind == 6) {                               //�����һ��Ϊ�ֺţ�����һ��
                i++;
                continue;
            }
            else {
                printf("ȱ��;");
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
                printf("����%sδ����",tokens[i].token);
                exit(1);
            }
            i = increase_long + 1;
        }
        else if (tokens[i].kind == 7) {                //����write����ʱ
            i++;
            if (tokens[i].token[0] == '(') {
                i++;
            }
            else {
                printf("writeȱ�١�(��");
                exit(1);
            }
     
            if (tokens[i].kind == 2) {
                t_symbol = is_declear(tokens[i].token);
                if (table[t_symbol].is_int == 0) {
                    printf("�����޳�ֵ");
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
                printf("�޲���");
                exit(1);
            }
            i++;
            if (tokens[i].token[0] == ')') {
                i++;
            }
            else {
                printf("writeȱ�١�)��");
                exit(1);
            } 
            if (tokens[i].token[0] == ';') {
                i++;
            }
            else if (i >= row) {
                break;
            }
            else {
                printf("writeȱ��; or .");
                exit(1);
            }
        }
    }

}

/* ջ�ṹ�� */
typedef struct {
    short tag; //�ж�����
    union {
        float* idata;
        char* cdata;
    }u;
    int top;
}stack;

/* ���ʽ�ṹ�� */
typedef struct {
    short tag;//�ж�����
    union {
        float num;
        char ch;
    }u;
}EXPRESSION;

/* ջ������������ */
void push(stack*, float);
float pop(stack*);
float top(stack*);
int priority(char);
bool isnumber(char);
bool isempty(stack*);

float calculate(char str[])
{
    int i, j, len, cnt;//�ַ����±ꡢ���ȱ���
    int w = 1;

    stack istack, cstack;//����ջ ������ջ

    istack.tag = 1;
    istack.u.idata = (float*)malloc(sizeof(float) * 50);
    istack.top = -1;  //������ջ��

    cstack.tag = 0;
    cstack.u.cdata = (char*)malloc(sizeof(char) * 50);
    cstack.top = -1; //����ջ��

    EXPRESSION out[50];//��׺���ʽ�ṹ������    
    cnt = 0;//��׺���ʽ�����±�
    float sum = 0;//�ۼ�����
    float xiaoshu = 0;
    len = strlen(str);//��ȡ����׺���ʽ�ַ�������
    int flag = 0;//�����ֱ�ʶ

    /* 1.��׺���ʽת��׺���ʽ(�ò�����ջ) */
    for (i = 0; i <= len; i++)// 100*(2+3)-4   100 2 3 + * 4 -   
    {
        /* ���ַ�����ȡ���ַ�Ϊ���֣����뵽��׺���ʽ�ṹ������ */
        if (isnumber(str[i])) {
            sum = sum * 10 + str[i] - '0';//�ۼ�����            
            flag = 1;//�����ֱ�ʶ
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
        /* ���ֽ���׺���ʽ�ṹ����������� */
        if (flag)
        {
            out[cnt].tag = 1;
            out[cnt++].u.num = sum;
            flag = 0;
            sum = 0;
            xiaoshu = 0;
        }
        /* ��ȡ���ȼ��ߵķ������ַ�(�ַ�')'���ȼ�0) ���� ������ջΪ�� */
        if (priority(str[i]) > priority(top(&cstack)) || isempty(&cstack))
        {
            push(&cstack, str[i]);
            continue;
        }
        /* ��ȡ���ַ��������ţ�һ�δ���������������\������\����'('*/
        if (str[i] == ')')
        {
            while (top(&cstack) != '(')
            {
                out[cnt].tag = 2;
                out[cnt].u.ch = top(&cstack); //����������׺���ʽ������ַ�
                cnt++;
                pop(&cstack);
            }
            pop(&cstack);// '('
            continue;
        }
        /* �������������ּ�������*/
        while (!isempty(&cstack) && top(&cstack) != '(')
        {
            out[cnt].tag = 2;
            out[cnt++].u.ch = top(&cstack);//����������׺���ʽ������ַ�
            pop(&cstack);
        }
        /* �ͼ����������ջ */
        if (str[i])
            push(&cstack, str[i]);
    }
    ///* ��ӡ��׺���ʽ */  //100 2 3 + * 4 -   100*(2+3)-4
    //for (i = 0; i < cnt; ++i)
    //{
    //    if (out[i].tag == 1)
    //        printf("%d ", out[i].u.num);
    //    else printf("%c ", out[i].u.ch);
    //}
    //printf("\n");  
    /* 2.���ݺ�׺���ʽ����(�ò�����ջ\����ջ) */
    for (i = 0; i < cnt; i++)
    {
        /* �Ӻ�׺���ʽ�ṹ�������ȡ��������ѹ����ջ��
        Ȼ���жϽṹ�������Ƿ���� */
        if (out[i].tag == 1) {
            push(&istack, out[i].u.num);
            continue;
        }
        else if (out[i].u.ch == '+') {//����ջ��Ԫ�ؼӵ�����Ԫ�ز�����ջ��Ԫ��
            istack.u.idata[istack.top - 1] += top(&istack);
            pop(&istack);
        }
        else if (out[i].u.ch == '-') {//ͬ���������Ԫ�ز�����ջ��Ԫ��
            istack.u.idata[istack.top - 1] -= top(&istack);
            pop(&istack);
        }
        else if (out[i].u.ch == '*') { //ͬ��˵�����Ԫ�ز�����ջ��Ԫ��
            istack.u.idata[istack.top - 1] *= top(&istack);
            pop(&istack);
        }
        else if (out[i].u.ch == '/') { //ͬ���������Ԫ�ز�����ջ��Ԫ��
            istack.u.idata[istack.top - 1] /= top(&istack);
            pop(&istack);
        }
        else if (out[i].u.ch == '^') {//ͬ���ݵ�����Ԫ�ز�����ջ��Ԫ��
            istack.u.idata[istack.top - 1]
                = pow(istack.u.idata[istack.top - 1], top(&istack));
            pop(&istack);
        }

    }
    return(istack.u.idata[0]);
}

/* �Ƿ�ջ�պ��� */
bool isempty(stack* s) {
    return (s->top == -1);
}

/* ѹջ���� */
void push(stack* s, float ch) {
    if (s->tag == 0) {
        s->u.cdata[++s->top] = (char)ch;
    }
    else {
        s->u.idata[++s->top] = ch;
    }
}

/* ��ջ���� */
float pop(stack* s) {
    if (s->tag == 0) {
        return s->u.cdata[s->top--];
    }
    return s->u.idata[s->top--];
}

/* ȡջ��Ԫ�غ��� */
float top(stack* s) {
    if (s->tag == 0) {
        return s->u.cdata[s->top];
    }
    return s->u.idata[s->top];
}

/* �Ƿ����ֺ��� */
bool isnumber(char ch) {
    return (ch >= '0' && ch <= '9');
}

/* ���������ȼ����� */
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
        //        printf("˫Ŀ�����ȱ�ٶ���");
        //        exit(1);
        //    }
        //    if ((isnumber(tokens[i - 1].token[0]) || tokens[i-1].kind == 2 || tokens[i-1].token[0] == ')') 
        //        && (isnumber(tokens[i + 1].token[0]) || tokens[i + 1].kind == 2 || tokens[i + 1].token[0] == '('))   //������ֻ��߱����ұ��������ֻ��߱���
        //    {
        //        strcat(str, tokens[i].token);
        //        i++;
        //    }
        //    else {
        //        printf("˫Ŀ�����ȱ�ٶ���");
        //        exit(1);
        //    }
        //} 
        else if (tokens[i].token[0] == '*' || tokens[i].token[0] == '+' || tokens[i].token[0] == '/') {
            if (i == start) {
                printf("˫Ŀ�����ȱ�ٶ���");
                exit(1);
            }
            if ((isnumber(tokens[i - 1].token[0]) || tokens[i - 1].kind == 2 || tokens[i - 1].token[0] == ')')
                && (isnumber(tokens[i + 1].token[0]) || tokens[i + 1].kind == 2 || tokens[i + 1].token[0] == '('))   //������ֻ��߱����ұ��������ֻ��߱���
            {
                strcat(str, tokens[i].token);
                i++;
            }
            else {
                printf("˫Ŀ�����ȱ�ٶ���");
                exit(1);
            }
        }
        else if (tokens[i].token[0] == '-') {
            if ((isnumber(tokens[i - 1].token[0]) || tokens[i - 1].kind == 2 || tokens[i - 1].token[0] == ')')
                && (isnumber(tokens[i + 1].token[0]) || tokens[i + 1].kind == 2 || tokens[i + 1].token[0] == '('))   //������ֻ��߱����ұ��������ֻ��߱���
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
        //    printf("���Ͳ�����ת���ɸ�����");
        //    exit(1);
        //}
        else if (tokens[i].kind == 2) {
            /*if (t_symbol = (is_declear(tokens[i].token) + 1)) {
                  if (table[t_symbol - 1].is_int == 0) {
                      printf("�����޳�ֵ");
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
                    printf("�����޳�ֵ");
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
                printf("����δ����");
                exit(1);
            }
            i++;
        }
        else {
            printf("����ʽ�Ӳ��ϱ�׼");
            exit(1);
        }
    }
    increase_long = i;
    if (kuohaoshu == 0) {
         return calculate(str);
    }
    else if (kuohaoshu > 0) {
        printf("ȱ�١�)��");
        exit(1);
    }
    else {
        printf("ȱ�١�(��");
        exit(1);
    }
}

int main(int argc, char* argv[]) {
    int row;
    if (argc != 2)
    { 
        printf("�����ʽ����");
        exit(1);
    }
    else {
        row = read(argv[1]);
        grammar(row);
        //int i = 0;
        //while (row != i) {
        //    printf("%s ", tokens[i].token);
        //    i++; //����read�����Ƿ���ȷ
        //}
       /* grammar(row);*/
    }
    return 0;
}
