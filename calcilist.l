%{
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include "calcilist.h"
#include "calcilist.tab.h"

void yyerror(char*);

%}

%%

"plot" {return PLOT;}
"func" {return FUNC;}
"mean" {return MEAN;}
"max" {return MAX;}
"min" {return MIN;}
"sum" {return SUM;}
"moving_mean" {return MOVMEAN;}
"reverse" {return REVERSE;}
"dot_product" {return DOTPRODUCT;}
"scatter_plot" {return SCATTERPLOT;}
"histogram" {return HISTOGRAM;}
"power" {return POWER;}
"if" {return IF;}
"else" {return ELSE;}
"&&"    { return AND; }
"||"    { return OR; }
">"|"<"|"=="|"!="|">="|"<=" { yylval = NewNode(); yylval->code = strdup(yytext); return RELOP; }
"{"     { return LBRACE; }
"}"     { return RBRACE; }

[a-zA-Z_][a-zA-Z0-9_]* { list *node = NewNode(); node->code = strdup(yytext); yylval = node; return VAR; }
[0-9][0-9]*		  { yylval = NewNode(); yylval->value = atof(yytext); yylval->code=strdup(yytext); return NUM; }
[0-9][0-9]*\.[0-9]+	  { yylval = NewNode(); yylval->value = atof(yytext); yylval->code=strdup(yytext); return NUM; }
\n                        { return *yytext; }
[ \t]*			  { /* Do nothing, drop them. */ }
.                         { return *yytext; }
%%

#ifdef STANDALONE_LEXER
int main(void)
	{
	while(yylex()!='q');
	}
#endif
