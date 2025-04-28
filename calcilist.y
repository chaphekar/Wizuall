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
        temp->code = NULL;
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
          Symbol *sym = lookup($1->code);  // Use the identifier name from Symbol
          if (!sym) sym = insert($1->code, NULL); // Insert if not found
        //   printf("%s = %g\n", $1, sym->value);

          printf("%s = %s\n", $1->code, $3->code);
        //   append_to_file("output.m", "%s = %s", $1->code, $3->code);

      }
            | EXPR '\n' { 
                printf("%s\n", $1->code);
                // append_to_file("output.m", "%s", $1->code);
            }
            | '\n';

EXPR        :   EXPR    '+'     TERM  { 
                $$ = NewNode();
                $$->code = (char*)malloc(strlen($1->code) + strlen($3->code) + 4);
                sprintf($$->code, "%s + %s", $1->code, $3->code);
            }
            | EXPR '-' TERM { 
                $$ = NewNode();
                $$->code = (char*)malloc(strlen($1->code) + strlen($3->code) + 4);
                sprintf($$->code, "%s - %s", $1->code, $3->code);
            }
            |   TERM                            { $$ = $1; }
            |  MEAN VAR { $$ = NewNode(); $$->code = (char*)malloc(strlen($2->code) + 10); sprintf($$->code, "mean(%s)", $2->code); }
            |  MAX VAR { $$ = NewNode(); $$->code = (char*)malloc(strlen($2->code) + 10); sprintf($$->code, "max(%s)", $2->code); }
            |  MIN VAR { $$ = NewNode(); $$->code = (char*)malloc(strlen($2->code) + 10); sprintf($$->code, "min(%s)", $2->code); }
            |  SUM VAR { $$ = NewNode(); $$->code = (char*)malloc(strlen($2->code) + 10); sprintf($$->code, "sum(%s)", $2->code); }
            |  MOVMEAN VAR NUM { $$ = NewNode(); $$->code = (char*)malloc(strlen($2->code) + strlen($3->code) + 10); sprintf($$->code, "movmean(%s, %s)", $2->code, $3->code); }
            |  REVERSE VAR { $$ = NewNode(); $$->code = (char*)malloc(strlen($2->code) + 10); sprintf($$->code, "flip(%s)", $2->code); }
            |  DOTPRODUCT VAR VAR { $$ = NewNode(); $$->code = (char*)malloc(strlen($2->code) + strlen($3->code) + 10); sprintf($$->code, "dot(%s,%s)", $2->code, $3->code); }
            |  SCATTERPLOT VAR VAR { $$ = NewNode(); $$->code = (char*)malloc(strlen($2->code) + strlen($3->code) + 13); sprintf($$->code, "scatter(%s,%s)", $2->code, $3->code); }
            |  HISTOGRAM VAR NUM { $$ = NewNode(); $$->code = (char*)malloc(strlen($2->code) + strlen($3->code) + 12); sprintf($$->code, "hist(%s,%s)", $2->code, $3->code); }
            |  POWER VAR NUM { $$ = NewNode(); $$->code = (char*)malloc(strlen($2->code) + 5); sprintf($$->code, "%s.^%s", $2->code, $3->code); }
            ;

TERM        :   TERM    '*'     FACTOR          { }
            | TERM '/' FACTOR { }
            |   FACTOR                          { $$ = $1; }
            ;
FACTOR : VAR {
             Symbol *sym = lookup($1->code);  // Use the identifier name from Symbol
             if (!sym) {
                 printf("undef: %s\n", $1->code);
                 yyerror("Undefined variable");
                 YYABORT;
             }
             $$ = NewNode();
             $$->code = strdup($1->code);  // Store the variable's code
         }
        | NUM { $$ = NewNode(); $$->code = (char*)malloc(32); sprintf($$->code, "%g", $1->value); }
        | '(' EXPR ')' { $$ = NewNode(); $$->code = (char*)malloc(strlen($2->code) + 4); sprintf($$->code, "(%s)", $2->code); }
        | '[' LIST ']' { $$ = NewNode(); $$->code = (char*)malloc(strlen($2->code) + 4); sprintf($$->code, "[%s]", $2->code); };

LIST        :   NUM EXTEND { 
                char buffer[32];
                sprintf(buffer, "%g", $1->value);
                if ($2) {
                    $$ = (list*)malloc(sizeof(list));
                    $$->code = (char*)malloc(strlen(buffer) + strlen($2->code) + 3);
                    sprintf($$->code, "%s, %s", buffer, $2->code);  // Concatenate the number with the extended list code
                } else {
                    $$ = (list*)malloc(sizeof(list));
                    $$->code = strdup(buffer);  // Just use the number's code if there's no extension
                }
            }
            | '[' LIST ']' EXTEND { 
                if ($4) {
                    $$ = (list*)malloc(sizeof(list));
                    $$->code = (char*)malloc(strlen($2->code) + strlen($4->code) + 4);
                    sprintf($$->code, "[%s], %s", $2->code, $4->code);  // Wrap the list in brackets and add the extended part
                } else {
                    $$ = (list*)malloc(sizeof(list));
                    $$->code = (char*)malloc(strlen($2->code) + 4);
                    sprintf($$->code, "[%s]", $2->code);  // Just wrap the list in brackets
                }
            }
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
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <output_filename>\n", argv[0]);
        return 1;
    }

    // Open the output file
    FILE *output_file = fopen(argv[1], "w");
    if (output_file == NULL) {
        perror("Error opening output file");
        return 1;
    }

    // Redirect stdout to the specified output file
    freopen(argv[1], "w", stdout);

    // Start parsing the input
    yyparse();

    // Close the output file
    fclose(output_file);

    return 0;
}
