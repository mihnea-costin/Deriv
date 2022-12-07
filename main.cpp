#include <stdio.h>
#include <cstring>
#include <conio.h>
#include <iostream>
#include <stdlib.h>
#include <graphics.h>
#include <winbgim.h>
#include <fstream>
using namespace std;

ofstream fout("derivative.out");

char der[1000];  /// sirul in care se pune derivata
char sir[1000];  /// sirul in care se pune functia citita de la tastatura
int simplified;

/// structura stiva face o stiva cu elemente de tip char[]

struct nodst
{
    char info[50];
    nodst *next;
};

struct stiva
{
    nodst *varf;
    int nrelem;
};

/// structura nodarb face un arbore binar cu elemente de tip char[]

struct nodarb
{
    nodarb *st;
    nodarb *dr;
    char info[500];
};

typedef nodarb *tree;
tree a,arb;

/// structura stivaarb face o stiva cu elemente de tip arbore, definit anterior

struct nodsta
{
    tree t;
    nodsta *next;
};

struct stivaarb
{
    nodsta *varf;
    int elem;
};

/// functia t_push() adauaga in stiva cu elemente de tip arbore un nod nou

void t_push(stivaarb &S, tree x)
{
    nodsta *nou;
    if(S.elem==0)
    {
        S.elem=1;
        S.varf= new nodsta;
        S.varf->t=x;
        S.varf->next=NULL;
    }
    else
    {
        S.elem++;
        nou=new nodsta;
        nou->t=x;
        nou->next=S.varf;
        S.varf=nou;
    }
}

/// functia push() adauga in stiva cu elemente de tip char[] un nod nou

void push(stiva &S, char elem[])
{
    nodst *nou;
    if(S.nrelem==0)
    {
        S.nrelem=1;
        S.varf=new nodst;
        strcpy(S.varf->info,elem);
        S.varf->next=NULL;
    }
    else
    {
        S.nrelem++;
        nou=new nodst;
        strcpy(nou->info,elem);
        nou->next=S.varf;
        S.varf=nou;
    }
}

/// functia t_pop() scoate din stiva cu elemente de tip arbore ultimul nod

void t_pop(stivaarb &S)
{
    if(S.elem!=0)
    {
        nodsta *nou;
        nou=S.varf->next;
        delete S.varf;
        S.varf=nou;
        S.elem--;
    }
}

/// functia pop() scoate din stiva cu elemente de tip char[] ultimul nod

char pop(stiva &S)
{
    if(S.nrelem!=0)
    {
        nodst *nou;
        nou=S.varf->next;
        delete S.varf;
        S.varf=nou;
        S.nrelem--;
    }
}

/// functia InitializeTree() initializeaza cu NULL un arbore

void InitializeTree(tree &a)
{
    a=NULL;
}

/// functia Priority() verifica prioritatea unui operator

int Priority(char x[])
{
    if(x==0) return 0;
    if(strcmp(x,"+")==0 || strcmp(x,"-")==0) return 1;
    if(strcmp(x,"*")==0 || strcmp(x,"/")==0) return 2;
    if(strcmp(x,"^")==0) return 3;
    if(strcmp(x,"sin")==0 || strcmp(x,"cos")==0 || strcmp(x,"tg")==0 || strcmp(x,"ctg")==0 || strcmp(x,"ln")==0 || strcmp(x,"log")==0 ||
            strcmp(x,"arcsin")==0 || strcmp(x,"arccos")==0 || strcmp(x,"arctg")==0 || strcmp(x,"arcctg")==0 || strcmp(x,"sqrt")==0) return 4;
    if(strcmp(x,"(")==0) return 5;
    return 0;
}

/// funtia Op() verifica daca un sir de caractere este operator si returneaza 1 daca este, 0 in caz contrar

int Op(char x[])
{
    if(strcmp(x,"+")==0 ||strcmp(x,"-")==0 || strcmp(x,"*")==0 || strcmp(x,"/")==0 || strcmp(x,"^")==0 || strcmp(x,"(")==0 || strcmp(x,")")==0 ||
            strcmp(x,"sin")==0 || strcmp(x,"cos")==0 || strcmp(x,"tg")==0 || strcmp(x,"ctg")==0 || strcmp(x,"ln")==0 || strcmp(x,"log")==0 ||
            strcmp(x,"arcsin")==0 || strcmp(x,"arccos")==0 || strcmp(x,"arctg")==0 || strcmp(x,"arcctg")==0 || strcmp(x,"sqrt")==0) return 1;
    return 0;
}

/// functia Infix2Postfix() ia elementele din vectorul de cuvinte expr[][] si pune in vectorul de cuvinte postfix[][] expresia citita initial de la tastatura in forma postfixata

void Infix2Postfix(char expr[1000][1000],int nrex, char postfix[1000][1000], int &nrpost)
{
    int i=0;
    stiva S;
    S.varf=NULL;
    S.nrelem=0;
    while(i<=nrex)
    {
        if(Op(expr[i])==0)
            strcpy(postfix[nrpost++],expr[i]);
        else
        {
            if(strcmp(expr[i],")")==0)
            {
                while(strcmp(S.varf->info,"(")!=0)
                {
                    strcpy(postfix[nrpost++],S.varf->info);
                    pop(S);
                }
                pop(S);
            }
            else
            {
                while(S.nrelem!=0 && strcmp(S.varf->info,"(")!=0 && Priority(S.varf->info)>=Priority(expr[i]))
                {
                    strcpy(postfix[nrpost++],S.varf->info);
                    pop(S);
                }
                push(S,expr[i]);
            }
        }
        i++;
    }
    while(S.nrelem!=0)
    {
        strcpy(postfix[nrpost++],S.varf->info);
        pop(S);
    }
}

/// functia Initialize() creeaza un nod al unui arbore si ii initializeaza fii cu NULL

void Initialize(tree &a)
{
    a=new nodarb;
    a->dr=NULL;
    a->st=NULL;
}

/// functia PrintWithParenthesis() citeste arborele in inordine, concateneaza informatia din nod in sirul char[] der si concateneaza paranteze cand intalneste un operator cu priotitate mai mare

void PrintWithParenthesis(tree arb)
{
    int par=0;
    if(arb!=NULL)
    {
        if(Op(arb->info)==1 && arb->st!=NULL && arb->dr!=NULL)
            if(Priority(arb->info)>=Priority(arb->st->info) && Op(arb->st->info)==1)
            {
                strcat(der,"(");
                par=1;
            }
        if(strcmp(arb->info,"/")==0 && arb->st!=NULL && arb->dr!=NULL)
            if(strcmp(arb->st->info,"^")==0)
            {
                strcat(der,"(");
                par=1;
            }
        PrintWithParenthesis(arb->st);
        if(par==1)
        {
            strcat(der,")");
            par=0;
        }
        strcat(der,arb->info);
        if(Op(arb->info)==1 && arb->st!=NULL && arb->dr!=NULL)
            if(Priority(arb->info)>=Priority(arb->dr->info) && Op(arb->dr->info)==1)
            {
                strcat(der,"(");
                par=1;
            }
        if(strcmp(arb->info,"/")==0 && arb->st!=NULL && arb->dr!=NULL)
            if(strcmp(arb->dr->info,"^")==0)
            {
                strcat(der,"(");
                par=1;
            }
        if(strcmp(arb->info,"sin")==0 || strcmp(arb->info,"cos")==0 || strcmp(arb->info,"tg")==0 || strcmp(arb->info,"ctg")==0 || strcmp(arb->info,"ln")==0
                ||strcmp(arb->info,"arcsin")==0 || strcmp(arb->info,"arccos")==0 || strcmp(arb->info,"arctg")==0 || strcmp(arb->info,"arcctg")==0 || strcmp(arb->info,"sqrt")==0)
        {
            strcat(der,"(");
            par=1;
        }
        PrintWithParenthesis(arb->dr);
    }
    if(par==1)  strcat(der,")");
}

/// functia DoTree() creeaza arborele expresiei folosind vectorul de cuvinte postfix[][] unde expresia initiala este in forma postfixata

void DoTree(tree &a, char postfix[1000][1000], int nrpost)
{
    stivaarb S;
    S.elem=0;
    S.varf=NULL;
    tree arb1,arb2,arb3,arb4;
    for(int i=0; i<nrpost; i++)
    {
        if(Op(postfix[i])==0)
        {
            Initialize(arb1);
            strcpy(arb1->info, postfix[i]);
            t_push(S,arb1);
        }
        else if(strcmp(postfix[i],"+")==0 ||strcmp(postfix[i],"-")==0 || strcmp(postfix[i],"*")==0 || strcmp(postfix[i],"/")==0 || strcmp(postfix[i],"^")==0 || strcmp(postfix[i],"(")==0 || strcmp(postfix[i],")")==0 || strcmp(postfix[i],"log")==0)
        {
            Initialize(arb1);
            Initialize(arb2);
            if(S.varf!=NULL)
            {
                arb1=S.varf->t;
                t_pop(S);
            }
            if(S.varf!=NULL)
            {
                arb2=S.varf->t;
                t_pop(S);
            }
            Initialize(arb3);
            strcpy(arb3->info,postfix[i]);
            arb3->dr=arb1;
            arb3->st=arb2;
            t_push(S,arb3);
        }
        else
        {
            Initialize(arb2);
            arb2=S.varf->t;
            t_pop(S);
            Initialize(arb3);
            arb3->dr=arb2;
            strcpy(arb3->info,postfix[i]);
            t_push(S,arb3);
        }
    }
    a=S.varf->t;
}

/// functia isNumber() returneaza 1 daca un sir de caractere este numar, 0 in caz contrar

int isNumber(char x[])
{
    for(int i=0; x[i]!=0; i++)
        if(x[i]<'0' || x[i]>'9') return 0;
    return 1;
}

/// functia Derivative() face derivarea propriu zisa, recursiv, facand modificari la arbore cand este cazul

void Derivative(tree &a, tree &arb)
{
    tree arb1,arb2,arb3,arb4,arb5,arb6,arb7,arb8,arb9,arb10,arb11;
    Initialize(arb);
    if(a->info[0]>='0' && a->info[0]<='9') strcpy(arb->info,"0");
    if(a->info[0]=='x') strcpy(arb->info,"1");
    if(a->info[0]=='+')
    {
        Derivative(a->st,arb3);
        arb->st=arb3;
        Derivative(a->dr,arb4);
        arb->dr=arb4;
        strcpy(arb->info,"+");
    }
    if(a->info[0]=='-' && a->st!=NULL && a->dr!=NULL)
    {
        Derivative(a->st,arb3);
        arb->st=arb3;
        Derivative(a->dr,arb4);
        arb->dr=arb4;
        strcpy(arb->info,"-");
    }
    if(a->info[0]=='*')
    {
        Initialize(arb1);
        Initialize(arb2);
        Derivative(a->st,arb3);
        Derivative(a->dr,arb4);
        strcpy(arb->info,"+");
        arb1->st=arb3;
        arb1->dr=a->dr;
        strcpy(arb1->info,"*");
        arb2->st=a->st;
        arb2->dr=arb4;
        strcpy(arb2->info,"*");
        arb->st=arb1;
        arb->dr=arb2;
    }
    if(a->info[0]=='/')
    {
        if(strcmp(a->st->info,"1")==0)
        {
            Initialize(arb1);
            Initialize(arb2);
            Initialize(arb5);
            Derivative(a->st,arb3);
            Derivative(a->dr,arb4);
            strcpy(arb1->info,"*");
            arb1->st=a->st;
            arb1->dr=arb4;
            strcpy(arb2->info,"*");
            arb2->st=arb3;
            arb2->dr=a->dr;
            strcpy(arb5->info,"-");
            arb5->st=arb1;
            arb5->dr=arb2;
            Initialize(arb6);
            Initialize(arb7);
            strcpy(arb6->info,"2");
            strcpy(arb7->info,"^");
            arb7->st=a->dr;
            arb7->dr=arb6;
            Initialize(arb8);
            strcpy(arb8->info,"/");
            arb8->st=arb5;
            arb8->dr=arb7;
            strcpy(arb->info,"-");
            arb->dr=arb8;
        }
        else
        {
            Initialize(arb1);
            Initialize(arb2);
            Initialize(arb5);
            Derivative(a->st,arb3);
            Derivative(a->dr,arb4);
            strcpy(arb1->info,"*");
            arb1->st=a->st;
            arb1->dr=arb4;
            strcpy(arb2->info,"*");
            arb2->st=arb3;
            arb2->dr=a->dr;
            strcpy(arb5->info,"-");
            arb5->st=arb1;
            arb5->dr=arb2;
            Initialize(arb6);
            Initialize(arb7);
            strcpy(arb6->info,"2");
            strcpy(arb7->info,"^");
            arb7->st=a->dr;
            arb7->dr=arb6;
            strcpy(arb->info,"/");
            arb->st=arb5;
            arb->dr=arb7;
        }
    }
    if(a->info[0]=='^')
    {
        if(strcmp(a->st->info,"e")==0)
        {
            Initialize(arb1);
            strcpy(arb1->info,a->info);
            arb1->dr=a->dr;
            arb1->st=a->st;
            Derivative(a->dr,arb2);
            strcpy(arb->info,"*");
            arb->dr=arb1;
            arb->st=arb2;
        }
        else if(isNumber(a->st->info)==1)
        {
            Initialize(arb1);
            Initialize(arb2);
            Initialize(arb3);
            strcpy(arb1->info,"^");
            arb1->st=a->st;
            arb1->dr=a->dr;
            strcpy(arb2->info,"ln");
            arb2->dr=a->st;
            strcpy(arb3->info,"*");
            arb3->st=arb1;
            arb3->dr=arb2;
            strcpy(arb->info,"*");
            Derivative(a->dr,arb4);
            arb->st=arb3;
            arb->dr=arb4;
        }
        else
        {
            Derivative(a->st,arb1);
            Initialize(arb5);
            Initialize(arb2);
            strcpy(arb5->info,"1");
            strcpy(arb2->info,"-");
            arb2->st=a->dr;
            arb2->dr=arb5;
            Initialize(arb3);
            strcpy(arb3->info,"^");
            arb3->st=a->st;
            arb3->dr=arb2;
            Initialize(arb4);
            strcpy(arb4->info,"*");
            arb4->st=a->dr;
            arb4->dr=arb3;
            strcpy(arb->info,"*");
            arb->st=arb4;
            arb->dr=arb1;
        }
    }
    if(strcmp(a->info,"sqrt")==0)
    {
        Initialize(arb1);
        Initialize(arb2);
        Initialize(arb3);
        arb1=a;
        strcpy(arb2->info,"*");
        strcpy(arb3->info,"2");
        arb2->st=arb3;
        arb2->dr=arb1;
        strcpy(arb->info,"/");
        arb->dr=arb2;
        Derivative(a->dr,arb4);
        arb->st=arb4;
    }
    if(strcmp(a->info,"sin")==0)
    {
        Initialize(arb1);
        strcpy(arb1->info,"cos");
        arb1->dr=a->dr;
        Derivative(a->dr,arb2);
        strcpy(arb->info,"*");
        arb->st=arb1;
        arb->dr=arb2;
    }
    if(strcmp(a->info,"cos")==0)
    {
        Derivative(a->dr,arb4);
        Initialize(arb1);
        Initialize(arb2);
        Initialize(arb3);
        strcpy(arb2->info,"sin");
        arb2->dr=a->dr;
        strcpy(arb3->info,"*");
        arb3->dr=arb4;
        arb3->st=arb2;
        strcpy(arb->info,"-");
        arb->dr=arb3;
    }
    if(strcmp(a->info,"ln")==0)
    {
        Initialize(arb1);
        arb1=a->dr;
        Derivative(a->dr,arb2);
        strcpy(arb->info,"/");
        arb->st=arb2;
        arb->dr=arb1;
    }
    if(strcmp(a->info,"log")==0)
    {
        Initialize(arb1);
        Initialize(arb2);
        Initialize(arb3);
        strcpy(arb1->info,"ln");
        arb1->dr=a->st;
        strcpy(arb2->info,"*");
        arb2->st=a->dr;
        arb2->dr=arb1;
        Initialize(arb4);
        Initialize(arb5);
        strcpy(arb4->info,"1");
        strcpy(arb5->info,"/");
        arb5->st=arb4;
        arb5->dr=arb2;
        Derivative(a->dr,arb6);
        strcpy(arb->info,"*");
        arb->st=arb5;
        arb->dr=arb6;
    }
    if(strcmp(a->info,"tg")==0)
    {
        Initialize(arb1);
        Initialize(arb2);
        Initialize(arb3);
        Initialize(arb4);
        strcpy(arb2->info,"cos");
        arb1=a->dr;
        arb2->dr=arb1;
        strcpy(arb3->info,"^");
        strcpy(arb4->info,"2");
        arb3->dr=arb4;
        arb3->st=arb2;
        strcpy(arb->info,"/");
        Derivative(a->dr,arb5);
        arb->dr=arb3;
        arb->st=arb5;
    }
    if(strcmp(a->info,"ctg")==0)
    {
        Initialize(arb1);
        Initialize(arb2);
        Initialize(arb3);
        Initialize(arb4);
        Initialize(arb5);
        arb1=a->dr;
        strcpy(arb2->info,"sin");
        arb2->dr=arb1;
        strcpy(arb3->info,"^");
        strcpy(arb4->info,"2");
        arb3->st=arb2;
        arb3->dr=arb4;
        strcpy(arb5->info,"/");
        Derivative(a->dr,arb6);
        arb5->st=arb6;
        arb5->dr=arb3;
        strcpy(arb->info,"-");
        arb->dr=arb5;
    }
    if(strcmp(a->info,"arcsin")==0)
    {
        Initialize(arb1);
        Initialize(arb2);
        Initialize(arb3);
        Initialize(arb4);
        Initialize(arb5);
        Initialize(arb6);
        arb1=a->dr;
        strcpy(arb2->info,"2");
        strcpy(arb3->info,"^");
        arb3->st=arb1;
        arb3->dr=arb2;
        strcpy(arb4->info,"-");
        strcpy(arb5->info,"1");
        arb4->st=arb5;
        arb4->dr=arb3;
        strcpy(arb6->info,"sqrt");
        arb6->dr=arb4;
        arb->dr=arb6;
        Derivative(a->dr,arb7);
        strcpy(arb->info,"/");
        arb->st=arb7;
    }
    if(strcmp(a->info,"arccos")==0)
    {
        Initialize(arb1);
        Initialize(arb2);
        Initialize(arb3);
        Initialize(arb4);
        Initialize(arb5);
        Initialize(arb6);
        arb1=a->dr;
        strcpy(arb2->info,"2");
        strcpy(arb3->info,"^");
        arb3->st=arb1;
        arb3->dr=arb2;
        strcpy(arb4->info,"-");
        strcpy(arb5->info,"1");
        arb4->st=arb5;
        arb4->dr=arb3;
        strcpy(arb6->info,"sqrt");
        arb6->dr=arb4;
        Initialize(arb7);
        strcpy(arb7->info,"/");
        arb7->dr=arb6;
        Derivative(a->dr,arb8);
        arb7->st=arb8;
        strcpy(arb->info,"-");
        arb->dr=arb7;
    }
    if(strcmp(a->info,"arctg")==0)
    {
        Initialize(arb1);
        Initialize(arb2);
        Initialize(arb3);
        Initialize(arb4);
        Initialize(arb5);
        Initialize(arb6);
        arb1=a->dr;
        strcpy(arb2->info,"2");
        strcpy(arb3->info,"^");
        arb3->st=arb1;
        arb3->dr=arb2;
        strcpy(arb4->info,"+");
        strcpy(arb5->info,"1");
        arb4->st=arb5;
        arb4->dr=arb3;
        Derivative(a->dr,arb6);
        strcpy(arb->info,"/");
        arb->st=arb6;
        arb->dr=arb4;
    }
    if(strcmp(a->info,"arcctg")==0)
    {
        Initialize(arb1);
        Initialize(arb2);
        Initialize(arb3);
        Initialize(arb4);
        Initialize(arb5);
        Initialize(arb6);
        arb1=a->dr;
        strcpy(arb2->info,"2");
        strcpy(arb3->info,"^");
        arb3->st=arb1;
        arb3->dr=arb2;
        strcpy(arb4->info,"+");
        strcpy(arb5->info,"1");
        arb4->st=arb5;
        arb4->dr=arb3;
        Derivative(a->dr,arb6);
        Initialize(arb7);
        strcpy(arb7->info,"/");
        arb7->st=arb6;
        arb7->dr=arb4;
        strcpy(arb->info,"-");
        arb->dr=arb7;
    }

}

/// functia Simplify() simplifica operatiile care nu influenteaza rezultatul final

void Simplify(tree &arb)
{
    if(arb->st!=NULL)
        Simplify(arb->st);
    if(strcmp(arb->info,"")==0) arb->st=NULL;
    if(strcmp(arb->info,"+")==0 || strcmp(arb->info,"-")==0 && arb->st!=NULL)
        if(strcmp(arb->st->info,"0")==0)
        {
            arb=arb->dr;
            Simplify(arb);
            simplified=1;
        }
    if(strcmp(arb->info,"+")==0 || strcmp(arb->info,"-")==0 && arb->dr!=NULL)
        if(strcmp(arb->dr->info,"0")==0)
        {
            arb=arb->st;
            Simplify(arb);
            simplified=1;
        }
    if(strcmp(arb->info,"*")==0 && arb->st!=NULL && arb->st->dr!=NULL)
        if(strcmp(arb->st->dr->info,"0")==0)
        {
            strcpy(arb->info,"0");
            arb->dr=NULL;
            arb->st=NULL;
            Simplify(arb);
            simplified=1;
        }
    if(strcmp(arb->info,"*")==0 && a->dr!=NULL)
        if(strcmp(arb->dr->info,"0")==0)
        {
            strcpy(arb->info,"0");
            arb->st=NULL;
            arb->dr=NULL;
            Simplify(arb);
            simplified=1;
        }
    if(strcmp(arb->info,"*")==0 && a->st!=NULL)
        if(strcmp(arb->st->info,"0")==0)
        {
            strcpy(arb->info,"0");
            arb->st=NULL;
            arb->dr=NULL;
            Simplify(arb);
            simplified=1;
        }
    if(strcmp(arb->info,"/")==0 && a->st!=NULL)
        if(strcmp(arb->st->info,"0")==0)
        {
            strcpy(arb->info,"0");
            arb->st=NULL;
            arb->dr=NULL;
            Simplify(arb);
            simplified=1;
        }
    if(strcmp(arb->info,"*")==0 && arb->dr!=NULL)
        if(strcmp(arb->dr->info,"1")==0)
        {
            arb=arb->st;
            Simplify(arb);
            simplified=1;
        }
    if(strcmp(arb->info,"*")==0 && arb->st!=NULL)
        if(strcmp(arb->st->info,"1")==0)
        {
            arb=arb->dr;
            Simplify(arb);
            simplified=1;
        }
    if(strcmp(arb->info,"/")==0 && a->dr!=NULL)
        if(strcmp(arb->dr->info,"1")==0)
        {
            arb=arb->st;
            Simplify(arb);
            simplified=1;
        }
    if(strcmp(arb->info,"^")==0 && strcmp(arb->dr->info,"1")==0)
    {
        arb=arb->st;
        Simplify(arb);
        simplified=1;
    }
    if(arb->dr!=NULL)
        Simplify(arb->dr);
}

/// functia DoOperations() face operatiile posibile, cand ambii fii al unui nod sunt numere, modificand arborele

void DoOperations(tree &arb)
{
    tree arb1,arb2,arb3;
    if(arb->st!=NULL)
        DoOperations(arb->st);
    if(arb->st!=NULL && arb->dr!=NULL)
        if(strcmp(arb->info,"+")==0 && isNumber(arb->st->info)==1 && isNumber(arb->dr->info)==1)
        {
            int x,y,z;
            x=atoi(arb->st->info);
            y=atoi(arb->dr->info);
            z=x+y;
            itoa(z,arb->info,10);
            arb->dr=NULL;
            arb->st=NULL;
            simplified=1;
        }
    if(arb->st!=NULL && arb->dr!=NULL)
        if(strcmp(arb->info,"-")==0 && isNumber(arb->st->info)==1 && isNumber(arb->dr->info)==1)
        {
            int x,y,z;
            x=atoi(arb->st->info);
            y=atoi(arb->dr->info);
            z=x-y;
            itoa(z,arb->info,10);
            arb->dr=NULL;
            arb->st=NULL;
            simplified=1;
        }
    if(strcmp(arb->info,"*")==0 && isNumber(arb->st->info)==1 && isNumber(arb->dr->info)==1)
    {
        int x,y,z;
        x=atoi(arb->st->info);
        y=atoi(arb->dr->info);
        z=x*y;
        itoa(z,arb->info,10);
        arb->dr=NULL;
        arb->st=NULL;
        simplified=1;
    }
    if(arb->st!=NULL && arb->dr!=NULL && arb->dr->st!=NULL)
        if(strcmp(arb->info,"*")==0 && isNumber(arb->st->info)==1 && isNumber(arb->dr->st->info)==1)
        {
            Initialize(arb1);
            Initialize(arb2);
            Initialize(arb3);
            int x,y,z;
            x=atoi(arb->st->info);
            y=atoi(arb->dr->st->info);
            z=x*y;
            itoa(z,arb2->info,10);
            arb1=arb->dr->dr;
            strcpy(arb3->info,"*");
            arb3->dr=arb1;
            arb3->st=arb2;
            arb=arb3;
            simplified=1;
        }
    if(arb->st!=NULL && arb->dr!=NULL)
        if(strcmp(arb->info,"^")==0 && isNumber(arb->st->info)==1 && isNumber(arb->dr->info)==1)
        {
            int x,y,z=1;
            x=atoi(arb->st->info);
            y=atoi(arb->dr->info);
            for(int i=1; i<=y; i++)
                z=z*x;
            itoa(z,arb->info,10);
            arb->dr=NULL;
            arb->st=NULL;
            simplified=1;
        }
    if(arb->st!=NULL && arb->st->dr!=NULL && arb->dr!=NULL)
        if(strcmp(arb->info,"^")==0 && isNumber(arb->dr->info)==1 && isNumber(arb->st->dr->info)==1)
        {
            Initialize(arb1);
            Initialize(arb2);
            Initialize(arb3);
            int x,y,z=1;
            x=atoi(arb->st->dr->info);
            y=atoi(arb->dr->info);
            for(int i=1; i<=y; i++)
                z=z*x;
            arb1=arb->st->st;
            itoa(z,arb2->info,10);
            strcpy(arb3->info,"^");
            arb3->st=arb1;
            arb3->dr=arb2;
            arb=arb3;
            simplified=1;
        }
    if(strcmp(arb->info,"^")==0 && a->dr!=NULL)
        if(strcmp(arb->dr->info,"1")==0)
        {
            Initialize(arb1);
            Initialize(arb2);
            arb1=arb->st;
            arb=arb1;
            simplified=1;
        }
    if(strcmp(arb->info,"^")==0 && a->dr!=NULL)
        if(strcmp(arb->dr->info,"0")==0)
        {
            arb->st=NULL;
            arb->dr=NULL;
            strcpy(arb->info,"1");
            simplified=1;
        }
    if(arb->dr!=NULL)
        DoOperations(arb->dr);

}

/// functia FakeMain() pune sirul citit de la tastatura in vectorul de cuvinte expr[][], separand operatorii de operanzi
/// apoi apeleaza functiile definite mai sus pentru a creea forma postfixata, apoi formeaza arborele asociat expresiei,
/// dupa care deriveaza expresia pe baza arborelui, apoi face operatiile si simplificarile posibile.
/// In final formeaza sirul dev citind in inordine arborele

void FakeMain(char sir[1000])
{
    char expr[1000][1000]= {""}, postfix[1000][1000]= {""};
    int nrex=0,nrpost=0;
    int i=0,ct=0;
    InitializeTree(a);
    while(sir[i]!=0)
    {
        if(sir[i]=='x')
        {
            nrex++;
            ct=0;
            expr[nrex][ct]=sir[i];
            nrex++;
        }
        else if(sir[i]==')')
        {
            nrex++;
            ct=0;
            expr[nrex][ct]=sir[i];
            nrex++;
        }
        else if(sir[i]=='+')
        {
            nrex++;
            ct=0;
            expr[nrex][ct]=sir[i];
            nrex++;
        }
        else if(sir[i]=='-')
        {
            nrex++;
            ct=0;
            expr[nrex][ct]=sir[i];
            nrex++;
        }
        else if(sir[i]=='*')
        {
            nrex++;
            ct=0;
            expr[nrex][ct]=sir[i];
            nrex++;
        }
        else if(sir[i]=='/')
        {
            nrex++;
            ct=0;
            expr[nrex][ct]=sir[i];
            nrex++;
        }
        else if(sir[i]=='(')
        {
            nrex++;
            ct=0;
            expr[nrex][ct]=sir[i];
            nrex++;
        }
        else if(sir[i]=='^')
        {
            nrex++;
            ct=0;
            expr[nrex][ct]=sir[i];
            nrex++;
        }
        else if(sir[i]=='s' && sir[i+1]=='q')
        {
            nrex++;
            ct=0;
            expr[nrex][ct]=sir[i];
            expr[nrex][++ct]=sir[++i];
            expr[nrex][++ct]=sir[++i];
            expr[nrex][++ct]=sir[++i];
            ct=0;
            nrex++;

        }
        else if(sir[i]=='s')
        {
            nrex++;
            ct=0;
            expr[nrex][ct]=sir[i];
            expr[nrex][++ct]=sir[++i];
            expr[nrex][++ct]=sir[++i];
            ct=0;
            nrex++;
        }
        else if(sir[i]=='c')
        {
            nrex++;
            ct=0;
            expr[nrex][ct]=sir[i];
            expr[nrex][++ct]=sir[++i];
            expr[nrex][++ct]=sir[++i];
            ct=0;
            nrex++;
        }
        else if(sir[i]=='t')
        {
            nrex++;
            ct=0;
            expr[nrex][ct]=sir[i];
            expr[nrex][++ct]=sir[++i];
            ct=0;
            nrex++;
        }
        else if(sir[i]=='l')
        {
            nrex++;
            ct=0;
            expr[nrex][ct]=sir[i++];
            if(sir[i]=='n')
            {
                expr[nrex][++ct]=sir[i];
                ct=0;
                nrex++;
            }
            else
            {
                expr[nrex][++ct]=sir[i++];
                expr[nrex][++ct]=sir[i];
                ct=0;
                nrex++;
            }
        }
        else if(sir[i]=='a')
        {
            nrex++;
            ct=0;
            expr[nrex][ct]=sir[i++];
            expr[nrex][++ct]=sir[i++];
            expr[nrex][++ct]=sir[i++];
            if(sir[i]=='t')
            {
                expr[nrex][++ct]=sir[i++];
                expr[nrex][++ct]=sir[i];
                ct=0;
                nrex++;
            }
            else
            {
                expr[nrex][++ct]=sir[i++];
                expr[nrex][++ct]=sir[i++];
                expr[nrex][++ct]=sir[i];
                ct=0;
                nrex++;
            }
        }
        else expr[nrex][ct++]=sir[i];
        i++;
    }
    for(i=0; i<=nrex; i++)
        if(strcmp(expr[i],"")==0)
        {
            for(int j=i; j<nrex; j++)
                strcpy(expr[j],expr[j+1]);
            nrex--;
        }
    Infix2Postfix(expr,nrex,postfix,nrpost);
    DoTree(a,postfix,nrpost);
    DoOperations(a);
    Derivative(a,arb);
    simplified=1;
    while(simplified==1)
    {
        simplified=0;
        Simplify(arb);
        DoOperations(arb);
    }
    PrintWithParenthesis(arb);
}

/// functia LineNumber() returneaza numarul de nivele din arbore

int LineNumber(tree arb)
{
    if(arb==NULL) return 0;
    else return 1+max(LineNumber(arb->st), LineNumber(arb->dr));
}

/// functia ColumnNumber() returneaza numarul de coloane din arbore

int ColumnNumber(tree arb)
{
    if(arb==NULL) return 0;
    else return 1+ColumnNumber(arb->st)+ColumnNumber(arb->dr);
}

/// functia Draw() deseneaza nodurile si liniile dintre noduri, apelata recursiv in functia DrawTree()

void Draw(tree arb, int niv, int col, int nl, int nc)
{
    int x,y;
    x=col*nc+nc/2;
    y=(niv-1)*nl+nl/2;
    if(arb->st!=NULL)
    {
        int x1=nc/2+nc*(col-ColumnNumber(arb->st->dr)-1);
        line(x,y,x1,y+nl);
    }
    if(arb->dr!=NULL)
    {
        int x2=nc/2+nc*(col+ColumnNumber(arb->dr->st)+1);
        line(x,y,x2,y+nl);
    }
    setcolor(CYAN);
    circle(x, y, 20);
    setfillstyle(SOLID_FILL,RED);
    floodfill(x, y, CYAN);
    setcolor(WHITE);
    setcolor(YELLOW);
    floodfill(x, y, RED);
    outtextxy(x-5,y-5,arb->info);
    setcolor(WHITE);
}

void DrawTree(tree arb, int niv, int col, int nl, int nc)
{
    if(arb!=NULL)
    {
        Draw(arb,niv,col,nl,nc);
        if(arb->st!=NULL)
            DrawTree(arb->st,niv+1,col-ColumnNumber(arb->st->dr)-1,nl,nc);
        if(arb->dr!=NULL)
            DrawTree(arb->dr,niv+1,col+ColumnNumber(arb->dr->st)+1,nl,nc);
    }
}

/// functia MakeTree() initializeaza fereastra pentru afisarea grafica a arborelui si apeleaza functia DrawTree()

void MakeTree()
{
    initwindow(1800,800);
    int nl=800/LineNumber(arb);
    int nc=1800/ColumnNumber(arb);
    int col=ColumnNumber(arb);
    DrawTree(arb,1,ColumnNumber(arb->st),nl,nc);
    getch();
    clearviewport();
}

/// functia Menu() afiseaza limbile posibile dintre care poate alege utilizatorul
/// apoi citeste expresia de la tastatura, dupa care apeleaza functia FakeMain() pentru a deriva expresia
/// Daca se doreste efectuarea derivatei de ordin II, se va apela din nou functia FakeMain() si se va afisa
/// Daca utilizatorul doreste afisarea arborelui asociat derivatea, acesta va fi al ultimei derivate rezolvate

void Menu()
{
    int x;
    char raspuns[500];
    cout<<"*****************************************************"<<endl;
    cout<<"DERIV - DERIVARE FORMALA ****************************"<<endl;
    cout<<"*****************************************************"<<endl;
    cout<<"Select the language:"<<endl;
    cout<<" "<<endl;
    cout<<"1-english"<<endl;
    cout<<"2-romana"<<endl;
    cout<<"3-francais"<<endl;
    cout<<"4-deutsch"<<endl;
    cout<<"5-espanol"<<endl;
    cout<<"6-italiano"<<endl;
    cout<<"7-magyar"<<endl;
    cout<<" "<<endl;
    cout<<"Selection: ";
    cin>>x;
    switch(x)
    {
    case 1:
    {
        cout<<"*****************************************************"<<endl;
        cout<<"For optimal operation of the program, please use integer\n constants, parentheses and the following notations: \nsqrt, +, -, *, /, ^, logbx, ln, sin, cos, tg, ctg, \narcsin, arccos, arctg, arcctg."<<endl;
        cout<<"*****************************************************"<<endl;
        cout<<"Enter the expression: ";
        cin.get();
        cin.getline(sir,1000);
        fout<<"The initial expression is: "<<endl<<endl;
        fout<<sir<<endl<<endl;
        FakeMain(sir);
        cout<<"*****************************************************"<<endl;
        cout<<"The first order derivative of the expression is: "<<der;
        fout<<"The first order derivative of the expression is: "<<endl<<endl;
        fout<<der<<endl<<endl;
        cout<<" "<<endl;
        cout<<"*****************************************************"<<endl;
        cout<<"Would you like to perform the second order derivative of the expression?(yes/no)";
        cin.getline(raspuns,500);
        if(strstr(raspuns,"yes")!=NULL)
        {
            cout<<"*****************************************************"<<endl;
            strcpy(sir,der);
            strcpy(der,"");
            FakeMain(sir);
            cout<<"The second order derivative of the expression is: "<<der;
            fout<<"The second order derivative of the expression is: "<<endl<<endl;
            fout<<der<<endl<<endl;
            cout<<" "<<endl;
            cout<<"*****************************************************"<<endl;
        }
        else
        {
            cout<<"*****************************************************"<<endl;
        }
        cout<<"Do you want to view the tree associated with the derivative?(yes/no)";
        cin.getline(raspuns,500);
        if(strstr(raspuns,"yes")!=NULL)
        {
            cout<<"*****************************************************"<<endl;
            MakeTree();
        }
        if(strstr(raspuns,"no")!=NULL)
        {
            cout<<"*****************************************************"<<endl;
        }
        return;
    }
    case 2:
    {
        cout<<"*****************************************************"<<endl;
        cout<<"Pentru o functionare optima a programului, va rugam sa \nutilizati constante intregi, paranteze si notatiile urmatoare:\n sqrt, +, -, *, /, ^, logbx, ln, sin, cos, tg, ctg,\n arcsin, arccos, arctg, arcctg."<<endl;
        cout<<"*****************************************************"<<endl;
        cout<<"Introduceti expresia: ";
        cin.get();
        cin.getline(sir,1000);
        fout<<"Functia initiala este: "<<endl<<endl;
        fout<<sir<<endl<<endl;
        FakeMain(sir);
        cout<<"*****************************************************"<<endl;
        cout<<"Derivata expresiei este: "<<der;
        fout<<"Derivata de ordin I este:"<<endl<<endl;
        fout<<der<<endl<<endl;
        cout<<" "<<endl;
        cout<<"*****************************************************"<<endl;
        cout<<"Doriti sa efectuati derivata de ordin II?(da/nu)";
        cin.getline(raspuns,500);
        if(strstr(raspuns,"da")!=NULL)
        {
            cout<<"*****************************************************"<<endl;
            strcpy(sir,der);
            strcpy(der,"");
            FakeMain(sir);
            cout<<"Derivata de ordin II a expresiei este: "<<der;
            fout<<"Derivata de ordin II este:"<<endl<<endl;
            fout<<der<<endl;
            cout<<" "<<endl;
            cout<<"*****************************************************"<<endl;
        }
        else
        {
            cout<<"*****************************************************"<<endl;
        }
        cout<<"Doriti sa vizualizati arborele asociat derivatei?(da/nu)";
        cin.getline(raspuns,500);
        if(strstr(raspuns,"da")!=NULL)
        {
            cout<<"*****************************************************"<<endl;
            MakeTree();
        }
        if(strstr(raspuns,"nu")!=NULL)
        {
            cout<<"*****************************************************"<<endl;
        }
        return;

    }
    case 3:
    {
        cout<<"*****************************************************"<<endl;
        cout<<"Pour un fonctionnement optimal du programme, veuillez utiliser \ndes constantes entieres, des parentheses et les notations  \nsuivantes: sqrt, +, -, *, /, ^, logbx, ln, sin, cos, tg, ctg, \narcsin, arccos, arctg, arcctg."<<endl;
        cout<<"*****************************************************"<<endl;
        cout<<"Entrez l'expression: ";
        cin.get();
        cin.getline(sir,1000);
        fout<<"L'expression est: "<<endl<<endl;
        fout<<sir<<endl<<endl;
        FakeMain(sir);
        cout<<"*****************************************************"<<endl;
        cout<<"Le derive de premier ordre de l'expression est: "<<der;
        fout<<"Le derive de premier ordre de l'expression est: "<<endl<<endl;
        fout<<der<<endl<<endl;
        cout<<" "<<endl;
        cout<<"*****************************************************"<<endl;
        cout<<"Souhaitez-vous executer la derivee de second ordre de l'expression?(oui/non)";
        cin.getline(raspuns,500);
        if(strstr(raspuns,"oui")!=NULL)
        {
            cout<<"*****************************************************"<<endl;
            strcpy(sir,der);
            strcpy(der,"");
            FakeMain(sir);
            cout<<"Le derive de second ordre de l'expression est: "<<der;
            fout<<"Le derive de second ordre de l'expression est: "<<endl<<endl;
            fout<<der<<endl;
            cout<<" "<<endl;
            cout<<"*****************************************************"<<endl;
        }
        else
        {
            cout<<"*****************************************************"<<endl;
        }
        cout<<"Voulez-vous voir l'arborescence associee au derive?(oui/non)";
        cin.getline(raspuns,500);
        if(strstr(raspuns,"oui")!=NULL)
        {
            cout<<"*****************************************************"<<endl;
            MakeTree();
        }
        if(strstr(raspuns,"non")!=NULL)
        {
            cout<<"*****************************************************"<<endl;
        }
        return;
    }
    case 4:
    {
        cout<<"*****************************************************"<<endl;
        cout<<"Fur einen optimalen betrieb des Programms bitte Verwenden\n Sie ganzzahlige Konstanten, Klammern und die folgenden  \nNotationen: sqrt, +, -, *, /, ^, logbx, ln, sin, cos, tg, \nctg, arcsin, arccos, arctg, arcctg."<<endl;
        cout<<"*****************************************************"<<endl;
        cout<<"Geben Sie den Ausdruck ein: ";
        cin.get();
        cin.getline(sir,1000);
        fout<<"Die anfängliche Funktion ist: "<<endl<<endl;
        fout<<sir<<endl<<endl;
        FakeMain(sir);
        cout<<"*****************************************************"<<endl;
        cout<<"Die Ableitung erster Ordnung des Ausdrucks ist: "<<der;
        fout<<"Die Ableitung erster Ordnung des Ausdrucks ist: "<<endl<<endl;
        fout<<der<<endl<<endl;
        cout<<" "<<endl;
        cout<<"*****************************************************"<<endl;
        cout<<"Mochten Sie die Ableitung zweiter Ordnung des Ausdrucks durchfuhren?(ja/nein)";
        cin.getline(raspuns,500);
        if(strstr(raspuns,"ja")!=NULL)
        {
            cout<<"*****************************************************"<<endl;
            strcpy(sir,der);
            strcpy(der,"");
            FakeMain(sir);
            cout<<"Die Ableitung zweiter Ordnung des Ausdrucks ist: "<<der;
            fout<<"Die Ableitung zweiter Ordnung des Ausdrucks ist: "<<endl<<endl;;
            fout<<der<<endl;
            cout<<" "<<endl;
            cout<<"*****************************************************"<<endl;
        }
        else
        {
            cout<<"*****************************************************"<<endl;
        }
        cout<<"Mochten Sie den mit der Ableitung verknupften Baum sehen? (ja/nein)";
        cin.getline(raspuns,500);
        if(strstr(raspuns,"ja")!=NULL)
        {
            cout<<"*****************************************************"<<endl;
            MakeTree();
        }
        if(strstr(raspuns,"nein")!=NULL)
        {
            cout<<"*****************************************************"<<endl;
        }
        return;
    }
    case 5:
    {
        cout<<"*****************************************************"<<endl;
        cout<<"Per un funzionamento ottimale del programma, per favore \nutilizzare costanti intere, parentesi e le seguenti notazioni:\n sqrt, +, -, *, /, ^, logbx, ln, sin, cos, tg, ctg, \narcsin, arccos, arctg, arcctg."<<endl;
        cout<<"*****************************************************"<<endl;
        cout<<"Ingrese la expresion: ";
        cin.get();
        cin.getline(sir,1000);
        fout<<"La función inicial es: "<<endl<<endl;
        fout<<sir<<endl<<endl;
        FakeMain(sir);
        cout<<"*****************************************************"<<endl;
        cout<<"La derivada de primer orden de la expresion es: "<<der;
        fout<<"La derivada de primer orden de la expresion es: "<<endl<<endl;
        fout<<der<<endl<<endl;
        cout<<" "<<endl;
        cout<<"*****************************************************"<<endl;
        cout<<"Le gustaria realizar la derivacion de segundo orden de la expresion?(si/no)";
        cin.getline(raspuns,500);
        if(strstr(raspuns,"si")!=NULL)
        {
            cout<<"*****************************************************"<<endl;
            strcpy(sir,der);
            strcpy(der,"");
            FakeMain(sir);
            cout<<"La derivada de segundo orden de la expresion es: "<<der;
            fout<<"La derivada de segundo orden de la expresion es: "<<endl<<endl;
            fout<<der<<endl;
            cout<<" "<<endl;
            cout<<"*****************************************************"<<endl;
        }
        else
        {
            cout<<"*****************************************************"<<endl;
        }
        cout<<"Le gustaria ver el arbol asociado con la derivada?(si/no)";
        cin.getline(raspuns,500);
        if(strstr(raspuns,"si")!=NULL)
        {
            cout<<"*****************************************************"<<endl;
            MakeTree();
        }
        if(strstr(raspuns,"no")!=NULL)
        {
            cout<<"*****************************************************"<<endl;
        }
        return;
    }
    case 6:
    {
        cout<<"*****************************************************"<<endl;
        cout<<"Para un funcionamiento optimo del programa, use constantes\n enteras, parentesis y las siguientes notaciones:\n sqrt, +, -, *, /, ^, logbx, ln, sin, cos, tg, ctg, \narcsin, arccos, arctg, arcctg."<<endl;
        cout<<"*****************************************************"<<endl;
        cout<<"Inserisci l'espressione: ";
        cin.get();
        cin.getline(sir,1000);
        fout<<"La funzione iniziale è: "<<endl<<endl;
        fout<<sir<<endl<<endl;
        FakeMain(sir);
        cout<<"*****************************************************"<<endl;
        cout<<"La derivata del primo ordine dell'espressione e: "<<der;
        fout<<"La derivata del primo ordine dell'espressione e: "<<endl<<endl;
        fout<<der<<endl<<endl;
        cout<<" "<<endl;
        cout<<"*****************************************************"<<endl;
        cout<<"Desideri eseguire la derivazione di secondo ordine dell'espressione?(si/no)";
        cin.getline(raspuns,500);
        if(strstr(raspuns,"si")!=NULL)
        {
            cout<<"*****************************************************"<<endl;
            strcpy(sir,der);
            strcpy(der,"");
            FakeMain(sir);
            cout<<"La derivata di secondo ordine dell'espressione e: "<<der;
            fout<<"La derivata di secondo ordine dell'espressione e: "<<endl<<endl;
            fout<<der<<endl;
            cout<<" "<<endl;
            cout<<"*****************************************************"<<endl;
        }
        else
        {
            cout<<"*****************************************************"<<endl;
        }
        cout<<"Vuoi vedere l'albero associato alla derivata?(si/no)";
        cin.getline(raspuns,500);
        if(strstr(raspuns,"si")!=NULL)
        {
            cout<<"*****************************************************"<<endl;
            MakeTree();
        }
        if(strstr(raspuns,"no")!=NULL)
        {
            cout<<"*****************************************************"<<endl;
        }
        return;
    }
    case 7:
    {
        cout<<"*****************************************************"<<endl;
        cout<<"A program optimalis mukodese erdekeben kerjuk hasznaljon \negesz konstansokat, zarojeleket es a kovetkezo  jeloleseket:\n sqrt, +, -, *, /, ^, logbx, ln, sin, cos, tg, ctg, \narcsin, arccos, arctg, arcctg."<<endl;
        cout<<"*****************************************************"<<endl;
        cout<<"Irja be a kifejezest: ";
        cin.get();
        cin.getline(sir,1000);
        fout<<"A kezdeti funkció az: "<<endl<<endl;
        fout<<sir<<endl<<endl;
        FakeMain(sir);
        cout<<"*****************************************************"<<endl;
        cout<<"A kifejezes elsorendu szarmazeka: "<<der;
        fout<<"A kifejezes elsorendu szarmazeka: "<<endl<<endl;
        fout<<der<<endl<<endl;
        cout<<" "<<endl;
        cout<<"*****************************************************"<<endl;
        cout<<"El akarja hajtani a kifejezes masodrendu levezeteset?(igen/nem)";
        cin.getline(raspuns,500);
        if(strstr(raspuns,"igen")!=NULL)
        {
            cout<<"*****************************************************"<<endl;
            strcpy(sir,der);
            strcpy(der,"");
            FakeMain(sir);
            cout<<"A kifejezes masodrendu szarmazeka: "<<der;
            fout<<"A kifejezes masodrendu szarmazeka: "<<endl<<endl;
            fout<<der<<endl;
            cout<<" "<<endl;
            cout<<"*****************************************************"<<endl;
        }
        else
        {
            cout<<"*****************************************************"<<endl;
        }
        cout<<"Szeretne latni a szarmaztatott fat?(igen/nem)";
        cin.getline(raspuns,500);
        if(strstr(raspuns,"igen")!=NULL)
        {
            cout<<"*****************************************************"<<endl;
            MakeTree();
        }
        if(strstr(raspuns,"nem")!=NULL)
        {
            cout<<"*****************************************************"<<endl;
        }
        return;
    }
    }
}

int main()
{
    Menu();
    return 0;
}






































