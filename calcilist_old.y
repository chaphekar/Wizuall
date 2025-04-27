%debug
%{
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <error.h>
#include <errno.h>

#include "calcilist.h"
#include "calcilist.tab.h"
extern int yylex(void);

list *DeepCopy(list *original) {
    if (!original) return NULL;

    list *copy = NewNode();
    if (original->first) {
        copy->first = DeepCopy(original->first);
        copy->rest = DeepCopy(original->rest);
    } else {
        copy->value = original->value; // Copy atomic value
    }
    return copy;
}

// Define the symbol table structure
typedef struct Symbol {
    char name[32];          // Identifier name
    list* value;
    struct Symbol *next; // Pointer to the next symbol (linked list)
} Symbol;

// Global symbol table
Symbol *symbolTable = NULL;

// Function to look up a symbol in the table
Symbol* lookup(char *name) {
    Symbol *sym = symbolTable;
    while (sym) {
        if (strcmp(sym->name, name) == 0) return sym; // Found
        sym = sym->next;
    }
    return NULL; // Not found
}

// Function to insert a new symbol into the table
Symbol* insert(char *name, list* value) {
    Symbol *sym = (Symbol *)malloc(sizeof(Symbol));
    if (!sym) {
        fprintf(stderr, "Error: Memory allocation failed for symbol '%s'\n", name);
        exit(EXIT_FAILURE);
    }
    strcpy(sym->name,name);   //copy the name
    sym->value = value;       // Set the value
    sym->next = symbolTable;  // Insert at the head of the list
    symbolTable = sym;
    return sym;
}

void assignVal(Symbol* sym, list* val)
{
    sym->value = val;
}

void yyerror(char *s)
{
fprintf(stderr,"%s\n",s);
return;
}

int yywrap()
{
    return 1;
}

#define printob printf("[")
#define printcb printf("]")
#define println printf("\n")
#define printlist(l) PrintList(l); println

void PrintList(list *l) {
    if(l) {
        if(l->first) {
            printob;
            PrintList(l->first);
            PrintList(l->rest);
            printcb;
        }
        else printf("%g ",l->value);
    }
}

int nodecount = 0;
list *NewNode(void) {
    list *temp = (list*)malloc(sizeof(list));
    if(temp) {
        nodecount++;
        temp->first = NULL;
        temp->rest = NULL;
        temp->value = M_E;
    }
    else error(-1,errno,"Allocation failed at %dth new node.", nodecount+1);
    return temp;
}

void FreeRecursive(list *l) {
    if(l) {
        FreeRecursive(l->first);
        FreeRecursive(l->rest);
        free(l);
        nodecount--;
    }
}

void AddAtomToList(double num, list *l) {
    if(l) {
        if(l->first) {
            AddAtomToList(num, l->first);
            AddAtomToList(num, l->rest);
        }
        else l->value += num;
    }
}

list *Add(list *one, list *two) {
    if(!(one || two)) return NULL;
    if(one && !two) return one;
    if(!one && two) return two;

    if(one->first) {
        if(two->first) {
            one->first = Add(one->first, two->first);
            one->rest = Add(one->rest, two->rest);
            return one;
        }
        else {
            AddAtomToList(two->value, one->first);
            free(two);
            return one;
        }
    }
    else {
        AddAtomToList(one->value, two);
        free(one);
        return two;
    }
}

list *Subtract(list *one, list *two) {
    if(!(one || two)) return NULL;
    if(one && !two) return one;
    if(!one && two) return two;

    if(one->first) {
        if(two->first) {
            one->first = Add(one->first, two->first);
            one->rest = Add(one->rest, two->rest);
            return one;
        }
        else {
            AddAtomToList(two->value, one->first);
            free(two);
            return one;
        }
    }
    else {
        AddAtomToList(one->value, two);
        free(one);
        return two;
    }
}

void MultiplyAtomToList(double num, list *l) {
    if(l) {
        if(l->first) {
            MultiplyAtomToList(num, l->first);
            MultiplyAtomToList(num, l->rest);
        }
        else l->value *= num;
    }
}

list *Multiply(list *one, list *two) {
    if(!(one || two)) return NULL;
    if(one && !two) return one;
    if(!one && two) return two;

    if(one->first) {
        if(two->first) {
            one->first = Multiply(one->first, two->first);
            one->rest = Multiply(one->rest, two->rest);
            return one;
        }
        else {
            MultiplyAtomToList(two->value, one);
            free(two);
            return one;
        }
    }
    else {
        MultiplyAtomToList(one->value, two);
        free(one);
        return two;
    }
}

list *lst; /* for debugging. temp. */

struct CodeNode {
    std::string code;
    std::string name;
}
%}

%union {
    char* op_val;
    struct CodeNode* codenode;
}

%token NUM
%token VAR
%token <codenode> functions

%%
LINES       :   LINES LINE
            |
            ;

LINE        : VAR '=' EXPR '\n' {
          Symbol *sym = lookup((char*)$1);  // Use the identifier name from Symbol
          if (!sym) sym = insert((char*)$1, $3); // Insert if not found
          else sym->value = $3;                 // Assign the expression value
        //   printf("%s = %g\n", $1, sym->value);
      }
            | EXPR '\n' { printlist($1); }
            | '\n';

EXPR        :   EXPR    '+'     TERM            { printf("addition of vectors\n");}
            | EXPR '-' TERM { printf("subtraction of vectors\n"); }
            |   TERM                            { $$ = $1; }
            ;
TERM        :   TERM    '*'     FACTOR          { printf("multiplication of term and factor\n"); }
            | TERM '/' FACTOR { printf("division of term and factor\n"); }
            |   FACTOR                          { $$ = $1; }
            ;
FACTOR : VAR {
             Symbol *sym = lookup((char*)$1);  // Use the identifier name from Symbol
             if (!sym) {
                 yyerror("Undefined variable");
                 YYABORT;
             }
             $$ = DeepCopy(sym->value); // Use the value from the symbol table
         }
        | NUM { $$ = $1; }
        | '(' EXPR ')' { $$ = $2; }
        | '[' LIST ']' { $$ = $2; };

LIST        :   NUM     EXTEND                  { $$ = NewNode(); $$->first = $1; $$->rest = $2; }
            |   '['     LIST    ']'     EXTEND  { $$ = NewNode(); $$->first = $2; $$->rest = $4; }
            ;
EXTEND      :   LIST                            { $$ = $1; }
            |                                   { $$ = NULL; }
            ;

%%

int main(int argc, char *argv[])
{
yyparse();
return 0;
}
