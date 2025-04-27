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
#include <stdarg.h>
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

// void PrintList(list *l) {
//     if(l) {
//         if(l->first) {
//             printob;
//             PrintList(l->first);
//             PrintList(l->rest);
//             printcb;
//         }
//         else printf("%g ",l->value);
//     }
// }

// void PrintList(list *l, list *init) {
//     if(l) {
//         if(l->first) {
//             // printob;
//             PrintList(l->first,init);
//             PrintList(l->rest,init);
//             // printcb;
//         }
//         else{
//             if(l == init)
//             printf("%g",l->value);
//             else printf(", %g", l->value);
        
//         }
//     }
// }
void append_to_file(const char* filename, const char* format, ...) {
    FILE* f = fopen(filename, "a");  // open in append mode (create if missing)
    if (f == NULL) {
        perror("Error opening file");
        return;
    }

    va_list args;
    va_start(args, format);
    vfprintf(f, format, args); // flexible writing
    va_end(args);

    fprintf(f, "\n"); // add newline after each write
    fclose(f);
}

void append_to_file_withoutln(const char* filename, const char* format, ...) {
    FILE* f = fopen(filename, "a");  // open in append mode (create if missing)
    if (f == NULL) {
        perror("Error opening file");
        return;
    }

    va_list args;
    va_start(args, format);
    vfprintf(f, format, args); // flexible writing
    va_end(args);

    fclose(f);
}

void printListHelper(list *l) {
    if (!l) return;

    if (l->first == NULL) {
        append_to_file_withoutln("output.m", "%g", l->value);
        printf("%g", l->value);
        if (l->rest) {
            append_to_file_withoutln("output.m", ", ");
            printf(", ");
            printListHelper(l->rest);
        }
    } else {
        printListHelper(l->first);
        if (l->rest) {
            append_to_file_withoutln("output.m", ", ");
            printf(", ");
            printListHelper(l->rest);
        }
    }
}

void PrintList(list *l) {
    printf("[");
    append_to_file_withoutln("output.m", "[");
    printListHelper(l);
    append_to_file_withoutln("output.m", "]");
    append_to_file_withoutln("output.m", "\n");
    printf("]\n");
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

// struct CodeNode {
//     std::string code;
//     std::string name;
// }
%}

// %union {
//     char* op_val;
//     struct CodeNode* codenode;
// }

%token NUM
%token VAR
%token FUNC
%token MEAN
%token MAX
%token MIN
%token SUM
%token MOVMEAN
%token REVERSE
%token DOTPRODUCT
%token SCATTERPLOT
%token HISTOGRAM
%token POWER


%%
LINES       :   LINES LINE { }
            |   FUNCTION
            |
            ;

LINE        : VAR '=' EXPR '\n' {
          Symbol *sym = lookup((char*)$1);  // Use the identifier name from Symbol
          if (!sym) sym = insert((char*)$1, $3); // Insert if not found
          else sym->value = $3;                 // Assign the expression value
        //   printf("%s = %g\n", $1, sym->value);

          printf("%s = ", (char*)$1);
          append_to_file_withoutln("output.m", "%s = ", (char*)$1);
          PrintList($3);

      }
            | EXPR '\n' { }
            | '\n';

EXPR        :   EXPR    '+'     TERM            { printf("addition of vectors\n");}
            | EXPR '-' TERM { printf("subtraction of vectors\n"); }
            |   TERM                            { $$ = $1; }
            |  MEAN VAR { printf("mean(%s)\n", (char*)$2); append_to_file("output.m", "mean(%s)", (char*)$2);}
            |  MAX VAR { printf("max(%s)\n", (char*)$2); append_to_file("output.m", "max(%s)", (char*)$2);}
            |  MIN VAR { printf("min(%s)\n", (char*)$2); append_to_file("output.m", "min(%s)", (char*)$2);}
            |  SUM VAR { printf("sum(%s)\n", (char*)$2); append_to_file("output.m", "sum(%s)", (char*)$2);}
            |  MOVMEAN VAR NUM { printf("movmean(%s, %g)\n", (char*)$2, $3->value); append_to_file("output.m", "movmean(%s, %g)", (char*)$2, $3->value);}
            |  REVERSE VAR { printf("flip(%s)\n", (char*)$2); append_to_file("output.m", "flip(%s)", (char*)$2);}
            |  DOTPRODUCT VAR VAR { printf("dot(%s, %s)\n", (char*)$2, (char*)$3); append_to_file("output.m", "dot(%s,%s)", (char*)$2, (char*)$3);}
            |  SCATTERPLOT VAR VAR { printf("scatter(%s, %s)\n", (char*)$2, (char*)$3); append_to_file("output.m", "scatter(%s,%s)", (char*)$2, (char*)$3);}
            |  HISTOGRAM VAR NUM { printf("hist(%s, %g)\n", (char*)$2, $3->value); append_to_file("output.m", "hist(%s,%g)", (char*)$2, $3->value);}
            |  POWER VAR NUM { printf("%s.^%g\n", (char*)$2, $3->value); append_to_file("output.m", "%s.^%g\n", (char*)$2, $3->value);}
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

FUNCTION    : FUNC VAR '(' PARAMS ')' NEWLINES '{' LINES '}' {printf("function spotted\n");}
            ;

NEWLINES    : '\n' NEWLINES
            | /* empty */
            ;

PARAMS      : VAR                        { /* single parameter */ }
            | PARAMS ',' VAR             { /* more parameters */ }
            ;
%%

int main(int argc, char *argv[])
{
yyparse();
return 0;
}
