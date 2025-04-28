#ifndef CALCILIST_H
#define CALCILIST_H

#define YYSTYPE list*
typedef struct _list {
    struct _list *first, *rest;
    double value;
    char* code;
} list;
extern char *yytext;

list *NewNode(void);
#endif
