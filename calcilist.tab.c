/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 2 "calcilist.y"

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

#line 364 "calcilist.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "calcilist.tab.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_PLOT = 3,                       /* PLOT  */
  YYSYMBOL_NUM = 4,                        /* NUM  */
  YYSYMBOL_VAR = 5,                        /* VAR  */
  YYSYMBOL_FUNC = 6,                       /* FUNC  */
  YYSYMBOL_MEAN = 7,                       /* MEAN  */
  YYSYMBOL_MAX = 8,                        /* MAX  */
  YYSYMBOL_MIN = 9,                        /* MIN  */
  YYSYMBOL_SUM = 10,                       /* SUM  */
  YYSYMBOL_MOVMEAN = 11,                   /* MOVMEAN  */
  YYSYMBOL_REVERSE = 12,                   /* REVERSE  */
  YYSYMBOL_DOTPRODUCT = 13,                /* DOTPRODUCT  */
  YYSYMBOL_SCATTERPLOT = 14,               /* SCATTERPLOT  */
  YYSYMBOL_HISTOGRAM = 15,                 /* HISTOGRAM  */
  YYSYMBOL_POWER = 16,                     /* POWER  */
  YYSYMBOL_IF = 17,                        /* IF  */
  YYSYMBOL_ELSE = 18,                      /* ELSE  */
  YYSYMBOL_AND = 19,                       /* AND  */
  YYSYMBOL_OR = 20,                        /* OR  */
  YYSYMBOL_RELOP = 21,                     /* RELOP  */
  YYSYMBOL_LBRACE = 22,                    /* LBRACE  */
  YYSYMBOL_RBRACE = 23,                    /* RBRACE  */
  YYSYMBOL_PAUSE = 24,                     /* PAUSE  */
  YYSYMBOL_25_ = 25,                       /* '='  */
  YYSYMBOL_26_n_ = 26,                     /* '\n'  */
  YYSYMBOL_27_ = 27,                       /* '+'  */
  YYSYMBOL_28_ = 28,                       /* '-'  */
  YYSYMBOL_29_ = 29,                       /* '*'  */
  YYSYMBOL_30_ = 30,                       /* '/'  */
  YYSYMBOL_31_ = 31,                       /* '('  */
  YYSYMBOL_32_ = 32,                       /* ')'  */
  YYSYMBOL_33_ = 33,                       /* '['  */
  YYSYMBOL_34_ = 34,                       /* ']'  */
  YYSYMBOL_YYACCEPT = 35,                  /* $accept  */
  YYSYMBOL_START = 36,                     /* START  */
  YYSYMBOL_LINES = 37,                     /* LINES  */
  YYSYMBOL_LINE = 38,                      /* LINE  */
  YYSYMBOL_EXPR = 39,                      /* EXPR  */
  YYSYMBOL_TERM = 40,                      /* TERM  */
  YYSYMBOL_FACTOR = 41,                    /* FACTOR  */
  YYSYMBOL_LIST = 42,                      /* LIST  */
  YYSYMBOL_EXTEND = 43,                    /* EXTEND  */
  YYSYMBOL_NEWLINES = 44,                  /* NEWLINES  */
  YYSYMBOL_NEWLINE = 45,                   /* NEWLINE  */
  YYSYMBOL_VARI = 46,                      /* VARI  */
  YYSYMBOL_STMT = 47,                      /* STMT  */
  YYSYMBOL_CONDITION = 48,                 /* CONDITION  */
  YYSYMBOL_BLOCK = 49                      /* BLOCK  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_int8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  46
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   172

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  35
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  15
/* YYNRULES -- Number of rules.  */
#define YYNRULES  48
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  100

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   279


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      26,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      31,    32,    29,    27,     2,    28,     2,    30,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    25,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    33,     2,    34,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   317,   317,   318,   323,   327,   333,   345,   352,   353,
     356,   361,   366,   367,   368,   369,   370,   371,   372,   373,
     374,   375,   376,   377,   378,   381,   386,   391,   395,   405,
     406,   407,   409,   421,   434,   435,   438,   439,   442,   443,
     446,   457,   462,   470,   475,   480,   485,   490,   495
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "PLOT", "NUM", "VAR",
  "FUNC", "MEAN", "MAX", "MIN", "SUM", "MOVMEAN", "REVERSE", "DOTPRODUCT",
  "SCATTERPLOT", "HISTOGRAM", "POWER", "IF", "ELSE", "AND", "OR", "RELOP",
  "LBRACE", "RBRACE", "PAUSE", "'='", "'\\n'", "'+'", "'-'", "'*'", "'/'",
  "'('", "')'", "'['", "']'", "$accept", "START", "LINES", "LINE", "EXPR",
  "TERM", "FACTOR", "LIST", "EXTEND", "NEWLINES", "NEWLINE", "VARI",
  "STMT", "CONDITION", "BLOCK", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-72)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      38,    11,   -72,    -6,    11,    11,    11,    11,    11,    11,
      11,    11,    11,    11,    -9,    23,   -72,   102,    24,    68,
      38,   -72,    39,    64,   -72,   -72,   -72,    11,   102,   -72,
     -72,   -72,   -72,    59,   -72,    11,    11,    66,    73,   133,
     -72,   -72,     8,    24,    24,    58,   -72,   -72,   -72,    25,
      25,    25,    25,   -72,    63,   -72,   -72,   -72,   -72,   -72,
     133,    -7,   100,   -72,   -72,   -72,    62,   -72,    64,    64,
     -72,   -72,   -72,   140,   131,   102,   133,   133,    46,    24,
     -72,    10,    79,    79,   -72,    81,   -72,    82,    46,    82,
      38,   105,   -72,    71,    81,    82,   101,    46,   -72,   -72
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       5,     0,    29,    28,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     9,     0,     0,     0,
       2,     4,     0,    12,    27,     8,    40,     0,     0,    13,
      14,    15,    16,     0,    18,     0,     0,     0,     0,     0,
      24,    28,     0,    35,     0,     0,     1,     3,     7,     0,
       0,     0,     0,    21,     0,    17,    19,    20,    22,    23,
       0,    47,     0,    30,    34,    32,     0,    31,    10,    11,
      25,    26,     6,    47,     0,     0,     0,     0,    39,    35,
      46,    45,    43,    44,    38,     0,    33,    37,    39,    37,
       5,    42,    36,    37,     0,     9,     0,    39,    48,    41
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -72,   -72,    35,   -20,   -16,    51,    70,    -5,    48,   -69,
     -71,    -2,   -72,   -45,    34
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,    19,    20,    21,    22,    23,    24,    64,    65,    92,
      85,    27,    25,    62,    88
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int8 yytable[] =
{
      47,    42,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    54,    45,    75,    74,    26,    91,    90,    28,
      49,    50,    39,    61,    96,    53,    99,    40,    43,     2,
      41,    82,    83,    56,    57,    49,    50,    49,    50,    66,
      63,     1,     2,     3,    73,     4,     5,     6,     7,     8,
       9,    10,    11,    12,    13,    14,    17,    44,    18,    81,
      61,    61,    15,    55,    16,    48,    49,    50,    46,    17,
      58,    18,    84,    47,     1,     2,     3,    59,     4,     5,
       6,     7,     8,     9,    10,    11,    12,    13,    14,    72,
      49,    50,    67,    51,    52,    15,    79,    95,    76,    77,
      68,    69,    17,    87,    18,     1,     2,    41,    89,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    76,
      77,    70,    71,    94,    98,    93,    15,    86,    97,     0,
       0,     0,    78,    17,     0,    18,     1,     2,    41,     0,
       4,     5,     6,     7,     8,     9,    10,    11,    12,    13,
      76,    77,     0,     0,     0,     0,     0,    15,     0,     0,
       0,    75,     0,    80,    60,     0,    18,    49,    50,     0,
       0,     0,    63
};

static const yytype_int8 yycheck[] =
{
      20,    17,     4,     5,     6,     7,     8,     9,    10,    11,
      12,    13,    28,    18,    21,    60,     5,    88,    87,    25,
      27,    28,    31,    39,    93,    27,    97,     4,     4,     4,
       5,    76,    77,    35,    36,    27,    28,    27,    28,    44,
      32,     3,     4,     5,    60,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    31,    33,    33,    75,
      76,    77,    24,     4,    26,    26,    27,    28,     0,    31,
       4,    33,    26,    93,     3,     4,     5,     4,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    26,
      27,    28,    34,    29,    30,    24,    34,    26,    19,    20,
      49,    50,    31,    22,    33,     3,     4,     5,    26,     7,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    19,
      20,    51,    52,    18,    23,    90,    24,    79,    94,    -1,
      -1,    -1,    32,    31,    -1,    33,     3,     4,     5,    -1,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      19,    20,    -1,    -1,    -1,    -1,    -1,    24,    -1,    -1,
      -1,    21,    -1,    32,    31,    -1,    33,    27,    28,    -1,
      -1,    -1,    32
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     3,     4,     5,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    24,    26,    31,    33,    36,
      37,    38,    39,    40,    41,    47,     5,    46,    25,    46,
      46,    46,    46,    46,    46,    46,    46,    46,    46,    31,
       4,     5,    39,     4,    33,    42,     0,    38,    26,    27,
      28,    29,    30,    46,    39,     4,    46,    46,     4,     4,
      31,    39,    48,    32,    42,    43,    42,    34,    40,    40,
      41,    41,    26,    39,    48,    21,    19,    20,    32,    34,
      32,    39,    48,    48,    26,    45,    43,    22,    49,    26,
      44,    45,    44,    37,    18,    26,    44,    49,    23,    45
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    35,    36,    37,    37,    37,    38,    38,    38,    38,
      39,    39,    39,    39,    39,    39,    39,    39,    39,    39,
      39,    39,    39,    39,    39,    40,    40,    40,    41,    41,
      41,    41,    42,    42,    43,    43,    44,    44,    45,    45,
      46,    47,    47,    48,    48,    48,    48,    48,    49
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     2,     1,     0,     4,     2,     1,     1,
       3,     3,     1,     2,     2,     2,     2,     3,     2,     3,
       3,     3,     3,     3,     2,     3,     3,     1,     1,     1,
       3,     3,     2,     4,     1,     0,     2,     0,     1,     0,
       1,    10,     7,     3,     3,     3,     3,     1,     5
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* START: LINES  */
#line 317 "calcilist.y"
              { printf("%s\n", yyvsp[0]->code); }
#line 1468 "calcilist.tab.c"
    break;

  case 3: /* LINES: LINES LINE  */
#line 318 "calcilist.y"
                   {
    yyval = NewNode();
    yyval->code = (char*)malloc(strlen(yyvsp[-1]->code) + strlen(yyvsp[0]->code) + 2);
    sprintf(yyval->code, "%s\n%s", yyvsp[-1]->code, yyvsp[0]->code);
}
#line 1478 "calcilist.tab.c"
    break;

  case 4: /* LINES: LINE  */
#line 323 "calcilist.y"
       {
    yyval = NewNode();
    yyval->code = strdup(yyvsp[0]->code);
}
#line 1487 "calcilist.tab.c"
    break;

  case 5: /* LINES: %empty  */
#line 327 "calcilist.y"
              {
    yyval = NewNode();
    yyval->code = strdup(""); 
}
#line 1496 "calcilist.tab.c"
    break;

  case 6: /* LINE: VAR '=' EXPR '\n'  */
#line 333 "calcilist.y"
                                {
          Symbol *sym = lookup(yyvsp[-3]->code);  // Use the identifier name from Symbol
          if (!sym) sym = insert(yyvsp[-3]->code, NULL); // Insert if not found
        //   printf("%s = %g\n", $1, sym->value);

            // printf("%s = %s\n", $1->code, $3->code);
            yyval = NewNode();
            yyval->code = (char*)malloc(strlen(yyvsp[-3]->code) + strlen(yyvsp[-1]->code) + 5);
            sprintf(yyval->code, "%s = %s", yyvsp[-3]->code, yyvsp[-1]->code);
        //   append_to_file("output.m", "%s = %s", $1->code, $3->code);

      }
#line 1513 "calcilist.tab.c"
    break;

  case 7: /* LINE: EXPR '\n'  */
#line 345 "calcilist.y"
                        { 
                yyval = NewNode();
                yyval->code = (char*)malloc(strlen(yyvsp[-1]->code) + 3);
                sprintf(yyval->code, "%s", yyvsp[-1]->code);
                // printf("%s\n", $1->code);
                // append_to_file("output.m", "%s", $1->code);
            }
#line 1525 "calcilist.tab.c"
    break;

  case 8: /* LINE: STMT  */
#line 352 "calcilist.y"
                   { yyval = NewNode(); yyval->code = strdup(yyvsp[0]->code); }
#line 1531 "calcilist.tab.c"
    break;

  case 9: /* LINE: '\n'  */
#line 353 "calcilist.y"
                   { yyval = NewNode(); yyval->code = strdup(""); }
#line 1537 "calcilist.tab.c"
    break;

  case 10: /* EXPR: EXPR '+' TERM  */
#line 356 "calcilist.y"
                                      { 
                yyval = NewNode();
                yyval->code = (char*)malloc(strlen(yyvsp[-2]->code) + strlen(yyvsp[0]->code) + 4);
                sprintf(yyval->code, "%s + %s", yyvsp[-2]->code, yyvsp[0]->code);
            }
#line 1547 "calcilist.tab.c"
    break;

  case 11: /* EXPR: EXPR '-' TERM  */
#line 361 "calcilist.y"
                            { 
                yyval = NewNode();
                yyval->code = (char*)malloc(strlen(yyvsp[-2]->code) + strlen(yyvsp[0]->code) + 4);
                sprintf(yyval->code, "%s - %s", yyvsp[-2]->code, yyvsp[0]->code);
            }
#line 1557 "calcilist.tab.c"
    break;

  case 12: /* EXPR: TERM  */
#line 366 "calcilist.y"
                                                { yyval = yyvsp[0]; }
#line 1563 "calcilist.tab.c"
    break;

  case 13: /* EXPR: MEAN VARI  */
#line 367 "calcilist.y"
                         { yyval = NewNode(); yyval->code = (char*)malloc(strlen(yyvsp[0]->code) + 10); sprintf(yyval->code, "mean(%s)", yyvsp[0]->code); }
#line 1569 "calcilist.tab.c"
    break;

  case 14: /* EXPR: MAX VARI  */
#line 368 "calcilist.y"
                        { yyval = NewNode(); yyval->code = (char*)malloc(strlen(yyvsp[0]->code) + 10); sprintf(yyval->code, "max(%s)", yyvsp[0]->code); }
#line 1575 "calcilist.tab.c"
    break;

  case 15: /* EXPR: MIN VARI  */
#line 369 "calcilist.y"
                        { yyval = NewNode(); yyval->code = (char*)malloc(strlen(yyvsp[0]->code) + 10); sprintf(yyval->code, "min(%s)", yyvsp[0]->code); }
#line 1581 "calcilist.tab.c"
    break;

  case 16: /* EXPR: SUM VARI  */
#line 370 "calcilist.y"
                        { yyval = NewNode(); yyval->code = (char*)malloc(strlen(yyvsp[0]->code) + 10); sprintf(yyval->code, "sum(%s)", yyvsp[0]->code); }
#line 1587 "calcilist.tab.c"
    break;

  case 17: /* EXPR: MOVMEAN VARI NUM  */
#line 371 "calcilist.y"
                                { yyval = NewNode(); yyval->code = (char*)malloc(strlen(yyvsp[-1]->code) + strlen(yyvsp[0]->code) + 10); sprintf(yyval->code, "movmean(%s, %s)", yyvsp[-1]->code, yyvsp[0]->code); }
#line 1593 "calcilist.tab.c"
    break;

  case 18: /* EXPR: REVERSE VARI  */
#line 372 "calcilist.y"
                            { yyval = NewNode(); yyval->code = (char*)malloc(strlen(yyvsp[0]->code) + 10); sprintf(yyval->code, "flip(%s)", yyvsp[0]->code); }
#line 1599 "calcilist.tab.c"
    break;

  case 19: /* EXPR: DOTPRODUCT VARI VARI  */
#line 373 "calcilist.y"
                                    { yyval = NewNode(); yyval->code = (char*)malloc(strlen(yyvsp[-1]->code) + strlen(yyvsp[0]->code) + 10); sprintf(yyval->code, "dot(%s, %s)", yyvsp[-1]->code, yyvsp[0]->code); }
#line 1605 "calcilist.tab.c"
    break;

  case 20: /* EXPR: SCATTERPLOT VARI VARI  */
#line 374 "calcilist.y"
                                     { yyval = NewNode(); yyval->code = (char*)malloc(strlen(yyvsp[-1]->code) + strlen(yyvsp[0]->code) + 13); sprintf(yyval->code, "scatter(%s, %s)", yyvsp[-1]->code, yyvsp[0]->code); }
#line 1611 "calcilist.tab.c"
    break;

  case 21: /* EXPR: PLOT VARI VARI  */
#line 375 "calcilist.y"
                              { yyval = NewNode(); yyval->code = (char*)malloc(strlen(yyvsp[-1]->code) + strlen(yyvsp[0]->code) + 13); sprintf(yyval->code, "plot(%s, %s)", yyvsp[-1]->code, yyvsp[0]->code); }
#line 1617 "calcilist.tab.c"
    break;

  case 22: /* EXPR: HISTOGRAM VARI NUM  */
#line 376 "calcilist.y"
                                  { yyval = NewNode(); yyval->code = (char*)malloc(strlen(yyvsp[-1]->code) + strlen(yyvsp[0]->code) + 12); sprintf(yyval->code, "hist(%s, %s)", yyvsp[-1]->code, yyvsp[0]->code); }
#line 1623 "calcilist.tab.c"
    break;

  case 23: /* EXPR: POWER VARI NUM  */
#line 377 "calcilist.y"
                              { yyval = NewNode(); yyval->code = (char*)malloc(strlen(yyvsp[-1]->code) + 5); sprintf(yyval->code, "%s.^%s", yyvsp[-1]->code, yyvsp[0]->code); }
#line 1629 "calcilist.tab.c"
    break;

  case 24: /* EXPR: PAUSE NUM  */
#line 378 "calcilist.y"
                         { yyval = NewNode(); yyval->code = (char*)malloc(strlen(yyvsp[0]->code) + 5); sprintf(yyval->code, "pause(%s)", yyvsp[0]->code); }
#line 1635 "calcilist.tab.c"
    break;

  case 25: /* TERM: TERM '*' FACTOR  */
#line 381 "calcilist.y"
                       {
            yyval = NewNode();
            yyval->code = (char*)malloc(strlen(yyvsp[-2]->code) + strlen(yyvsp[0]->code) + 5);
            sprintf(yyval->code, "%s .* %s", yyvsp[-2]->code, yyvsp[0]->code);
        }
#line 1645 "calcilist.tab.c"
    break;

  case 26: /* TERM: TERM '/' FACTOR  */
#line 386 "calcilist.y"
                       {
            yyval = NewNode();
            yyval->code = (char*)malloc(strlen(yyvsp[-2]->code) + strlen(yyvsp[0]->code) + 5);
            sprintf(yyval->code, "%s ./ %s", yyvsp[-2]->code, yyvsp[0]->code);
        }
#line 1655 "calcilist.tab.c"
    break;

  case 27: /* TERM: FACTOR  */
#line 391 "calcilist.y"
              { 
            yyval = yyvsp[0]; 
        }
#line 1663 "calcilist.tab.c"
    break;

  case 28: /* FACTOR: VAR  */
#line 395 "calcilist.y"
             {
             Symbol *sym = lookup(yyvsp[0]->code);  // Use the identifier name from Symbol
             if (!sym) {
                 printf("undef: %s\n", yyvsp[0]->code);
                 yyerror("Undefined variable");
                 YYABORT;
             }
             yyval = NewNode();
             yyval->code = strdup(yyvsp[0]->code);  // Store the variable's code
         }
#line 1678 "calcilist.tab.c"
    break;

  case 29: /* FACTOR: NUM  */
#line 405 "calcilist.y"
              { yyval = NewNode(); yyval->code = (char*)malloc(32); sprintf(yyval->code, "%g", yyvsp[0]->value); }
#line 1684 "calcilist.tab.c"
    break;

  case 30: /* FACTOR: '(' EXPR ')'  */
#line 406 "calcilist.y"
                       { yyval = NewNode(); yyval->code = (char*)malloc(strlen(yyvsp[-1]->code) + 4); sprintf(yyval->code, "(%s)", yyvsp[-1]->code); }
#line 1690 "calcilist.tab.c"
    break;

  case 31: /* FACTOR: '[' LIST ']'  */
#line 407 "calcilist.y"
                       { yyval = NewNode(); yyval->code = (char*)malloc(strlen(yyvsp[-1]->code) + 4); sprintf(yyval->code, "[%s]", yyvsp[-1]->code); }
#line 1696 "calcilist.tab.c"
    break;

  case 32: /* LIST: NUM EXTEND  */
#line 409 "calcilist.y"
                           { 
                char buffer[32];
                sprintf(buffer, "%g", yyvsp[-1]->value);
                if (yyvsp[0]) {
                    yyval = (list*)malloc(sizeof(list));
                    yyval->code = (char*)malloc(strlen(buffer) + strlen(yyvsp[0]->code) + 3);
                    sprintf(yyval->code, "%s, %s", buffer, yyvsp[0]->code);  // Concatenate the number with the extended list code
                } else {
                    yyval = (list*)malloc(sizeof(list));
                    yyval->code = strdup(buffer);  // Just use the number's code if there's no extension
                }
            }
#line 1713 "calcilist.tab.c"
    break;

  case 33: /* LIST: '[' LIST ']' EXTEND  */
#line 421 "calcilist.y"
                                  { 
                if (yyvsp[0]) {
                    yyval = (list*)malloc(sizeof(list));
                    yyval->code = (char*)malloc(strlen(yyvsp[-2]->code) + strlen(yyvsp[0]->code) + 4);
                    sprintf(yyval->code, "[%s], %s", yyvsp[-2]->code, yyvsp[0]->code);  // Wrap the list in brackets and add the extended part
                } else {
                    yyval = (list*)malloc(sizeof(list));
                    yyval->code = (char*)malloc(strlen(yyvsp[-2]->code) + 4);
                    sprintf(yyval->code, "[%s]", yyvsp[-2]->code);  // Just wrap the list in brackets
                }
            }
#line 1729 "calcilist.tab.c"
    break;

  case 34: /* EXTEND: LIST  */
#line 434 "calcilist.y"
                                                { yyval = yyvsp[0]; }
#line 1735 "calcilist.tab.c"
    break;

  case 35: /* EXTEND: %empty  */
#line 435 "calcilist.y"
                                                { yyval = NULL; }
#line 1741 "calcilist.tab.c"
    break;

  case 40: /* VARI: VAR  */
#line 446 "calcilist.y"
           {
             Symbol *sym = lookup(yyvsp[0]->code);  // Use the identifier name from Symbol
             if (!sym) {
                 printf("undef: %s\n", yyvsp[0]->code);
                 yyerror("Undefined variable");
                 YYABORT;
             }
             yyval = NewNode();
             yyval->code = strdup(yyvsp[0]->code);  // Store the variable's code
         }
#line 1756 "calcilist.tab.c"
    break;

  case 41: /* STMT: IF '(' CONDITION ')' NEWLINE BLOCK NEWLINE ELSE BLOCK NEWLINE  */
#line 457 "calcilist.y"
                                                                        {
              yyval = NewNode();
              yyval->code = (char*)malloc(strlen(yyvsp[-7]->code) + strlen(yyvsp[-4]->code) + strlen(yyvsp[-1]->code) + 40);
              sprintf(yyval->code, "if (%s)\n%s\nelse\n%s\nendif", yyvsp[-7]->code, yyvsp[-4]->code, yyvsp[-1]->code);
         }
#line 1766 "calcilist.tab.c"
    break;

  case 42: /* STMT: IF '(' CONDITION ')' NEWLINE BLOCK NEWLINE  */
#line 462 "calcilist.y"
                                                     {
              yyval = NewNode();
              yyval->code = (char*)malloc(strlen(yyvsp[-4]->code) + strlen(yyvsp[-1]->code) + 20);
              sprintf(yyval->code, "if (%s)\n%s\nendif", yyvsp[-4]->code, yyvsp[-1]->code);
         }
#line 1776 "calcilist.tab.c"
    break;

  case 43: /* CONDITION: CONDITION AND CONDITION  */
#line 470 "calcilist.y"
                                  {
              yyval = NewNode();
              yyval->code = (char*)malloc(strlen(yyvsp[-2]->code) + strlen(yyvsp[0]->code) + 5);
              sprintf(yyval->code, "%s && %s", yyvsp[-2]->code, yyvsp[0]->code);
          }
#line 1786 "calcilist.tab.c"
    break;

  case 44: /* CONDITION: CONDITION OR CONDITION  */
#line 475 "calcilist.y"
                                 {
              yyval = NewNode();
              yyval->code = (char*)malloc(strlen(yyvsp[-2]->code) + strlen(yyvsp[0]->code) + 5);
              sprintf(yyval->code, "%s || %s", yyvsp[-2]->code, yyvsp[0]->code);
          }
#line 1796 "calcilist.tab.c"
    break;

  case 45: /* CONDITION: EXPR RELOP EXPR  */
#line 480 "calcilist.y"
                          {
              yyval = NewNode();
              yyval->code = (char*)malloc(strlen(yyvsp[-2]->code) + strlen(yyvsp[0]->code) + strlen(yyvsp[-1]->code) + 5);
              sprintf(yyval->code, "%s %s %s", yyvsp[-2]->code, yyvsp[-1]->code, yyvsp[0]->code);
          }
#line 1806 "calcilist.tab.c"
    break;

  case 46: /* CONDITION: '(' CONDITION ')'  */
#line 485 "calcilist.y"
                            {
              yyval = NewNode();
              yyval->code = (char*)malloc(strlen(yyvsp[-1]->code) + 3);
              sprintf(yyval->code, "(%s)", yyvsp[-1]->code);
          }
#line 1816 "calcilist.tab.c"
    break;

  case 47: /* CONDITION: EXPR  */
#line 490 "calcilist.y"
               {
              yyval = yyvsp[0];
          }
#line 1824 "calcilist.tab.c"
    break;

  case 48: /* BLOCK: LBRACE NEWLINES LINES NEWLINES RBRACE  */
#line 495 "calcilist.y"
                                                {
              yyval = NewNode();
              yyval->code = (char*)malloc(strlen(yyvsp[-2]->code) + 5);
              sprintf(yyval->code, "%s", yyvsp[-2]->code);
          }
#line 1834 "calcilist.tab.c"
    break;


#line 1838 "calcilist.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 501 "calcilist.y"


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
