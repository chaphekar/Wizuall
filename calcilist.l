%{
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include "calcilist.h"
#include "calcilist.tab.h"

void yyerror(char*);

%}

%%

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

[a-zA-Z_][a-zA-Z0-9_]* { yylval = (list*)strdup(yytext); return VAR; }
[0-9][0-9]*		  { yylval = NewNode(); yylval->value = atof(yytext); return NUM; }
[0-9][0-9]*\.[0-9]+	  { yylval = NewNode(); yylval->value = atof(yytext); return NUM; }
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
