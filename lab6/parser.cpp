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
#line 1 "parser.y"

	#include "tree.h"
	#define YYSTYPE TreeNode *

	TreeNode* root = new TreeNode(0, NODE_PROG);
	extern int lineno;
	extern bool parserError;

	// max_scope_id 是堆栈下一层结点的最大编号
	unsigned char max_scope_id = SCOPT_ID_BASE;
	string presentScope = "1";
	unsigned int top = 0;

	// multimap <标识符名称， 作用域> 变量名列表
	extern multimap<string, string> idNameList;
	// map <<标识符名称， 作用域>, 结点指针> 变量列表
	extern map<pair<string, string>, TreeNode*> idList;


	int yylex();
	int yyerror( char const * );
	int scopeCmp(string preScope, string varScope);
	void scopePush();
	void scopePop();

#line 97 "parser.cpp"

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

#include "parser.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_T_INT = 3,                      /* T_INT  */
  YYSYMBOL_T_VOID = 4,                     /* T_VOID  */
  YYSYMBOL_ASSIGN = 5,                     /* ASSIGN  */
  YYSYMBOL_SEMICOLON = 6,                  /* SEMICOLON  */
  YYSYMBOL_COMMA = 7,                      /* COMMA  */
  YYSYMBOL_LPAREN = 8,                     /* LPAREN  */
  YYSYMBOL_RPAREN = 9,                     /* RPAREN  */
  YYSYMBOL_LBRACE = 10,                    /* LBRACE  */
  YYSYMBOL_RBRACE = 11,                    /* RBRACE  */
  YYSYMBOL_LBRACKET = 12,                  /* LBRACKET  */
  YYSYMBOL_RBRACKET = 13,                  /* RBRACKET  */
  YYSYMBOL_CONST = 14,                     /* CONST  */
  YYSYMBOL_IF_ = 15,                       /* IF_  */
  YYSYMBOL_ELSE = 16,                      /* ELSE  */
  YYSYMBOL_WHILE_ = 17,                    /* WHILE_  */
  YYSYMBOL_BREAK = 18,                     /* BREAK  */
  YYSYMBOL_CONTINUE = 19,                  /* CONTINUE  */
  YYSYMBOL_RETURN = 20,                    /* RETURN  */
  YYSYMBOL_EQ = 21,                        /* EQ  */
  YYSYMBOL_GRAEQ = 22,                     /* GRAEQ  */
  YYSYMBOL_LESEQ = 23,                     /* LESEQ  */
  YYSYMBOL_NEQ = 24,                       /* NEQ  */
  YYSYMBOL_GRA = 25,                       /* GRA  */
  YYSYMBOL_LES = 26,                       /* LES  */
  YYSYMBOL_PLUS = 27,                      /* PLUS  */
  YYSYMBOL_MINUS = 28,                     /* MINUS  */
  YYSYMBOL_MUL = 29,                       /* MUL  */
  YYSYMBOL_DIV = 30,                       /* DIV  */
  YYSYMBOL_MOD = 31,                       /* MOD  */
  YYSYMBOL_AND = 32,                       /* AND  */
  YYSYMBOL_OR = 33,                        /* OR  */
  YYSYMBOL_NOT = 34,                       /* NOT  */
  YYSYMBOL_IDENTIFIER = 35,                /* IDENTIFIER  */
  YYSYMBOL_INTEGER = 36,                   /* INTEGER  */
  YYSYMBOL_YYACCEPT = 37,                  /* $accept  */
  YYSYMBOL_ComUnit = 38,                   /* ComUnit  */
  YYSYMBOL_Decl = 39,                      /* Decl  */
  YYSYMBOL_ConstDecl = 40,                 /* ConstDecl  */
  YYSYMBOL_ConstDefs = 41,                 /* ConstDefs  */
  YYSYMBOL_ConstDef = 42,                  /* ConstDef  */
  YYSYMBOL_BasicType = 43,                 /* BasicType  */
  YYSYMBOL_IntConst = 44,                  /* IntConst  */
  YYSYMBOL_CompIdentifier = 45,            /* CompIdentifier  */
  YYSYMBOL_ArrayIdentifier = 46,           /* ArrayIdentifier  */
  YYSYMBOL_Identifier = 47,                /* Identifier  */
  YYSYMBOL_DeclCompIdentifier = 48,        /* DeclCompIdentifier  */
  YYSYMBOL_pDeclIdentifier = 49,           /* pDeclIdentifier  */
  YYSYMBOL_ArrayDeclIdentifier = 50,       /* ArrayDeclIdentifier  */
  YYSYMBOL_DeclIdentifier = 51,            /* DeclIdentifier  */
  YYSYMBOL_ArrayInitVal = 52,              /* ArrayInitVal  */
  YYSYMBOL_VarDecl = 53,                   /* VarDecl  */
  YYSYMBOL_VarDefs = 54,                   /* VarDefs  */
  YYSYMBOL_VarDef = 55,                    /* VarDef  */
  YYSYMBOL_FuncDef = 56,                   /* FuncDef  */
  YYSYMBOL_FuncLPAREN = 57,                /* FuncLPAREN  */
  YYSYMBOL_FuncFParams = 58,               /* FuncFParams  */
  YYSYMBOL_FuncFParam = 59,                /* FuncFParam  */
  YYSYMBOL_Block = 60,                     /* Block  */
  YYSYMBOL_BlockLBRACE = 61,               /* BlockLBRACE  */
  YYSYMBOL_BlockRBRACE = 62,               /* BlockRBRACE  */
  YYSYMBOL_BlockItems = 63,                /* BlockItems  */
  YYSYMBOL_BlockItem = 64,                 /* BlockItem  */
  YYSYMBOL_Stmt_ = 65,                     /* Stmt_  */
  YYSYMBOL_Stmt = 66,                      /* Stmt  */
  YYSYMBOL_IF = 67,                        /* IF  */
  YYSYMBOL_WHILE = 68,                     /* WHILE  */
  YYSYMBOL_Exp = 69,                       /* Exp  */
  YYSYMBOL_Cond = 70,                      /* Cond  */
  YYSYMBOL_addExp = 71,                    /* addExp  */
  YYSYMBOL_mulExp = 72,                    /* mulExp  */
  YYSYMBOL_unaryExp = 73,                  /* unaryExp  */
  YYSYMBOL_primaryExp = 74,                /* primaryExp  */
  YYSYMBOL_FuncRParams = 75,               /* FuncRParams  */
  YYSYMBOL_LOrExp = 76,                    /* LOrExp  */
  YYSYMBOL_LAndExp = 77,                   /* LAndExp  */
  YYSYMBOL_eqExp = 78,                     /* eqExp  */
  YYSYMBOL_relExp = 79                     /* relExp  */
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
typedef yytype_uint8 yy_state_t;

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
#define YYFINAL  11
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   303

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  37
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  43
/* YYNRULES -- Number of rules.  */
#define YYNRULES  95
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  177

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   291


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
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    40,    40,    41,    42,    43,    45,    46,    48,    64,
      65,    68,    74,    82,    83,    86,    89,    90,    97,   127,
     158,   187,   188,   192,   197,   204,   212,   234,   235,   239,
     264,   265,   269,   270,   276,   287,   309,   325,   328,   329,
     333,   344,   350,   351,   354,   355,   359,   360,   364,   369,
     373,   374,   375,   376,   387,   397,   408,   413,   418,   419,
     422,   423,   428,   429,   438,   442,   443,   444,   449,   450,
     451,   452,   457,   458,   459,   460,   465,   466,   467,   468,
     474,   480,   485,   486,   491,   492,   497,   498,   503,   504,
     505,   510,   511,   512,   513,   514
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
  "\"end of file\"", "error", "\"invalid token\"", "T_INT", "T_VOID",
  "ASSIGN", "SEMICOLON", "COMMA", "LPAREN", "RPAREN", "LBRACE", "RBRACE",
  "LBRACKET", "RBRACKET", "CONST", "IF_", "ELSE", "WHILE_", "BREAK",
  "CONTINUE", "RETURN", "EQ", "GRAEQ", "LESEQ", "NEQ", "GRA", "LES",
  "PLUS", "MINUS", "MUL", "DIV", "MOD", "AND", "OR", "NOT", "IDENTIFIER",
  "INTEGER", "$accept", "ComUnit", "Decl", "ConstDecl", "ConstDefs",
  "ConstDef", "BasicType", "IntConst", "CompIdentifier", "ArrayIdentifier",
  "Identifier", "DeclCompIdentifier", "pDeclIdentifier",
  "ArrayDeclIdentifier", "DeclIdentifier", "ArrayInitVal", "VarDecl",
  "VarDefs", "VarDef", "FuncDef", "FuncLPAREN", "FuncFParams",
  "FuncFParam", "Block", "BlockLBRACE", "BlockRBRACE", "BlockItems",
  "BlockItem", "Stmt_", "Stmt", "IF", "WHILE", "Exp", "Cond", "addExp",
  "mulExp", "unaryExp", "primaryExp", "FuncRParams", "LOrExp", "LAndExp",
  "eqExp", "relExp", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-146)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      17,  -146,  -146,    75,    13,  -146,  -146,   -29,  -146,  -146,
     -29,  -146,  -146,  -146,  -146,    21,    53,    23,  -146,   169,
    -146,   178,  -146,    50,    52,    76,  -146,   -18,    77,    24,
      55,  -146,   -29,  -146,   -29,    66,   108,    76,     6,     6,
      76,  -146,  -146,  -146,    83,   117,    93,  -146,    81,   122,
    -146,  -146,   123,   132,   -29,   112,  -146,    66,   142,   146,
    -146,  -146,  -146,    66,   153,   170,  -146,   139,   156,   102,
    -146,  -146,  -146,  -146,  -146,    76,    76,   267,    76,    76,
      76,    76,    76,    76,  -146,   231,  -146,    75,   179,  -146,
     127,  -146,   134,  -146,  -146,    76,    76,    76,    76,    76,
      76,    76,    76,  -146,   181,  -146,  -146,   161,   182,   122,
     122,  -146,  -146,  -146,  -146,  -146,  -146,  -146,   190,   196,
      32,  -146,   -29,  -146,   231,    79,  -146,  -146,   198,   201,
     204,  -146,   231,    66,  -146,  -146,  -146,  -146,  -146,  -146,
    -146,  -146,  -146,  -146,  -146,    76,  -146,  -146,  -146,  -146,
    -146,   207,   129,  -146,  -146,    76,    76,  -146,   163,  -146,
    -146,  -146,  -146,  -146,   195,   209,  -146,   254,   254,   231,
     203,  -146,  -146,   197,   254,  -146,  -146
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,    13,    14,     0,     0,     2,     6,     0,     7,     3,
       0,     1,     4,     5,    26,    32,    21,    22,    23,     0,
      30,     0,     9,     0,     0,     0,    37,     0,     0,     0,
       0,    29,     0,     8,     0,     0,     0,     0,     0,     0,
       0,    20,    15,    76,    81,    17,    16,    33,    62,    65,
      68,    72,     0,     0,     0,     0,    38,     0,     0,    21,
      31,    10,    11,     0,    91,     0,    64,    84,    86,    88,
      81,    74,    75,    91,    73,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    24,     0,    40,     0,     0,    27,
       0,    25,     0,    77,    78,     0,     0,     0,     0,     0,
       0,     0,     0,    63,     0,    80,    82,     0,     0,    66,
      67,    69,    70,    71,    50,    42,    60,    61,     0,     0,
       0,    46,     0,    52,     0,     0,    44,    47,     0,     0,
       0,    39,     0,     0,    34,    12,    85,    87,    89,    94,
      95,    90,    92,    93,    19,     0,    79,    18,    56,    57,
      58,     0,     0,    36,    45,     0,     0,    51,     0,    28,
      83,    59,    43,    41,     0,     0,    35,     0,     0,     0,
      54,    49,    55,     0,     0,    48,    53
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -146,  -146,   150,  -146,  -146,   186,     5,   -33,   -35,  -146,
    -146,  -146,     0,    15,  -146,   158,  -146,  -146,   191,   218,
    -146,  -146,   140,  -146,  -146,  -146,  -113,  -115,  -105,  -145,
    -146,  -146,   -25,   -39,  -146,   107,    78,   154,  -146,   131,
     133,   -61,  -146
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,     4,   121,     6,    21,    22,   122,    43,    44,    45,
      46,    15,    23,    17,    18,    90,     8,    19,    20,     9,
      28,    55,    56,   123,   124,   163,   125,   126,   170,   127,
     128,   129,   130,    65,    48,    49,    50,    51,   107,    66,
      67,    68,    69
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      47,    74,    62,    70,    70,     7,    14,    16,    10,     7,
     154,   152,    64,    11,    37,    73,     1,     2,    52,   158,
       1,     2,   171,   171,    89,    24,    25,     3,    29,   171,
      89,     3,    59,    54,    57,    30,   138,   154,   150,   141,
      37,    41,    42,   154,    70,    70,    70,    70,    70,    24,
     103,   104,   106,   108,    86,    35,   173,    36,   154,    38,
      39,    26,    27,   172,    30,    27,    40,    41,    42,   176,
      73,    73,    73,   139,   140,    73,   142,   143,     1,     2,
       1,     2,     1,     2,    37,   114,    53,    37,    75,   115,
     153,    58,    54,     3,   116,   151,   117,   118,   119,   120,
     159,    77,    42,    38,    39,    78,    38,    39,    79,    80,
      40,    41,    42,    40,    41,    42,   164,   165,    63,    87,
     160,    88,    59,    97,    98,    99,   100,   101,   102,    76,
      73,    73,     1,     2,   133,   114,    84,    37,   134,   115,
     162,   133,    85,     3,   116,   135,   117,   118,   119,   120,
       5,    81,    82,    83,    12,    91,    38,    39,    27,   111,
     112,   113,    93,    40,    41,    42,     1,     2,   145,   114,
     146,    37,    95,   115,   166,    31,    32,     3,   116,    94,
     117,   118,   119,   120,    33,    34,   109,   110,    96,   132,
      38,    39,    71,    72,   144,   147,   148,    40,    41,    42,
       1,     2,   149,   114,   167,    37,   155,   115,   175,   156,
     157,     3,   116,   161,   117,   118,   119,   120,   168,   174,
      61,    92,    13,    60,    38,    39,   136,   131,     0,   137,
       0,    40,    41,    42,     1,     2,     0,   114,     0,    37,
       0,   115,     0,     0,     0,     3,   116,     0,   117,   118,
     119,   120,     0,     0,     0,     0,     0,     0,    38,    39,
     114,     0,    37,     0,   169,    40,    41,    42,     0,   116,
       0,   117,   118,   119,   120,    37,   105,     0,     0,     0,
       0,    38,    39,     0,     0,     0,     0,     0,    40,    41,
      42,     0,     0,     0,    38,    39,     0,     0,     0,     0,
       0,    40,    41,    42
};

static const yytype_int16 yycheck[] =
{
      25,    40,    35,    38,    39,     0,    35,     7,     3,     4,
     125,   124,    37,     0,     8,    40,     3,     4,    36,   132,
       3,     4,   167,   168,    57,    10,     5,    14,     5,   174,
      63,    14,    32,    28,    10,    12,    97,   152,     6,   100,
       8,    35,    36,   158,    79,    80,    81,    82,    83,    34,
      75,    76,    77,    78,    54,     5,   169,     5,   173,    27,
      28,     8,    12,   168,    12,    12,    34,    35,    36,   174,
      95,    96,    97,    98,    99,   100,   101,   102,     3,     4,
       3,     4,     3,     4,     8,     6,     9,     8,     5,    10,
      11,    36,    87,    14,    15,   120,    17,    18,    19,    20,
     133,     8,    36,    27,    28,    12,    27,    28,    27,    28,
      34,    35,    36,    34,    35,    36,   155,   156,    10,     7,
     145,     9,   122,    21,    22,    23,    24,    25,    26,    12,
     155,   156,     3,     4,     7,     6,    13,     8,    11,    10,
      11,     7,    10,    14,    15,    11,    17,    18,    19,    20,
       0,    29,    30,    31,     4,    13,    27,    28,    12,    81,
      82,    83,     9,    34,    35,    36,     3,     4,     7,     6,
       9,     8,    33,    10,    11,     6,     7,    14,    15,     9,
      17,    18,    19,    20,     6,     7,    79,    80,    32,    10,
      27,    28,    38,    39,    13,    13,     6,    34,    35,    36,
       3,     4,     6,     6,     9,     8,     8,    10,    11,     8,
       6,    14,    15,     6,    17,    18,    19,    20,     9,    16,
      34,    63,     4,    32,    27,    28,    95,    87,    -1,    96,
      -1,    34,    35,    36,     3,     4,    -1,     6,    -1,     8,
      -1,    10,    -1,    -1,    -1,    14,    15,    -1,    17,    18,
      19,    20,    -1,    -1,    -1,    -1,    -1,    -1,    27,    28,
       6,    -1,     8,    -1,    10,    34,    35,    36,    -1,    15,
      -1,    17,    18,    19,    20,     8,     9,    -1,    -1,    -1,
      -1,    27,    28,    -1,    -1,    -1,    -1,    -1,    34,    35,
      36,    -1,    -1,    -1,    27,    28,    -1,    -1,    -1,    -1,
      -1,    34,    35,    36
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     3,     4,    14,    38,    39,    40,    43,    53,    56,
      43,     0,    39,    56,    35,    48,    49,    50,    51,    54,
      55,    41,    42,    49,    50,     5,     8,    12,    57,     5,
      12,     6,     7,     6,     7,     5,     5,     8,    27,    28,
      34,    35,    36,    44,    45,    46,    47,    69,    71,    72,
      73,    74,    36,     9,    43,    58,    59,    10,    36,    49,
      55,    42,    44,    10,    69,    70,    76,    77,    78,    79,
      45,    74,    74,    69,    70,     5,    12,     8,    12,    27,
      28,    29,    30,    31,    13,    10,    49,     7,     9,    44,
      52,    13,    52,     9,     9,    33,    32,    21,    22,    23,
      24,    25,    26,    69,    69,     9,    69,    75,    69,    72,
      72,    73,    73,    73,     6,    10,    15,    17,    18,    19,
      20,    39,    43,    60,    61,    63,    64,    66,    67,    68,
      69,    59,    10,     7,    11,    11,    76,    77,    78,    69,
      69,    78,    69,    69,    13,     7,     9,    13,     6,     6,
       6,    69,    63,    11,    64,     8,     8,     6,    63,    44,
      69,     6,    11,    62,    70,    70,    11,     9,     9,    10,
      65,    66,    65,    63,    16,    11,    65
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    37,    38,    38,    38,    38,    39,    39,    40,    41,
      41,    42,    42,    43,    43,    44,    45,    45,    46,    46,
      47,    48,    48,    49,    50,    50,    51,    52,    52,    53,
      54,    54,    55,    55,    55,    56,    56,    57,    58,    58,
      59,    60,    61,    62,    63,    63,    64,    64,    65,    65,
      66,    66,    66,    66,    66,    66,    66,    66,    66,    66,
      67,    68,    69,    69,    70,    71,    71,    71,    72,    72,
      72,    72,    73,    73,    73,    73,    74,    74,    74,    74,
      74,    74,    75,    75,    76,    76,    77,    77,    78,    78,
      78,    79,    79,    79,    79,    79
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     1,     2,     2,     1,     1,     4,     1,
       3,     3,     5,     1,     1,     1,     1,     1,     4,     4,
       1,     1,     1,     1,     4,     4,     1,     1,     3,     3,
       1,     3,     1,     3,     5,     8,     7,     1,     1,     3,
       2,     3,     1,     1,     1,     2,     1,     1,     3,     1,
       1,     2,     1,     7,     5,     5,     2,     2,     2,     3,
       1,     1,     1,     3,     1,     1,     3,     3,     1,     3,
       3,     3,     1,     2,     2,     2,     1,     3,     3,     4,
       3,     1,     1,     3,     1,     3,     1,     3,     1,     3,
       3,     1,     3,     3,     3,     3
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
  case 2: /* ComUnit: Decl  */
#line 40 "parser.y"
               {root->addChild(yyvsp[0]);}
#line 1311 "parser.cpp"
    break;

  case 3: /* ComUnit: FuncDef  */
#line 41 "parser.y"
                          {root->addChild(yyvsp[0]);}
#line 1317 "parser.cpp"
    break;

  case 4: /* ComUnit: ComUnit Decl  */
#line 42 "parser.y"
                               {root->addChild(yyvsp[0]);}
#line 1323 "parser.cpp"
    break;

  case 5: /* ComUnit: ComUnit FuncDef  */
#line 43 "parser.y"
                                  {root->addChild(yyvsp[0]);}
#line 1329 "parser.cpp"
    break;

  case 6: /* Decl: ConstDecl  */
#line 45 "parser.y"
                 {yyval = yyvsp[0];}
#line 1335 "parser.cpp"
    break;

  case 7: /* Decl: VarDecl  */
#line 46 "parser.y"
                   {yyval = yyvsp[0];}
#line 1341 "parser.cpp"
    break;

  case 8: /* ConstDecl: CONST BasicType ConstDefs SEMICOLON  */
#line 48 "parser.y"
                                                {
				yyval = new TreeNode(lineno, NODE_STMT);
				yyval->stype = STMT_CONSTDECL;
				yyval->type = TYPE_NONE;
				yyval->addChild(yyvsp[-2]);
				yyval->addChild(yyvsp[-1]);
				TreeNode* p = yyvsp[-1]->child;
				while(p != nullptr) {
					p->child->type->copy(yyvsp[-2]->type);
					p->child->type->constvar = true;
					p = p->sibling;
				}
			}
#line 1359 "parser.cpp"
    break;

  case 9: /* ConstDefs: ConstDef  */
#line 64 "parser.y"
                     {yyval = new TreeNode(lineno, NODE_VARLIST); yyval->addChild(yyvsp[0]);}
#line 1365 "parser.cpp"
    break;

  case 10: /* ConstDefs: ConstDefs COMMA ConstDef  */
#line 65 "parser.y"
                                             {yyval = yyvsp[-2]; yyval->addChild(yyvsp[0]);}
#line 1371 "parser.cpp"
    break;

  case 11: /* ConstDef: pDeclIdentifier ASSIGN IntConst  */
#line 68 "parser.y"
                                           {
				yyval = new TreeNode(lineno, NODE_OP);
				yyval->optype = OP_DECLASSIGN;
				yyval->addChild(yyvsp[-2]);
				yyval->addChild(yyvsp[0]);
			}
#line 1382 "parser.cpp"
    break;

  case 12: /* ConstDef: ArrayDeclIdentifier ASSIGN LBRACE ArrayInitVal RBRACE  */
#line 74 "parser.y"
                                                                        {
				yyval = new TreeNode(lineno, NODE_OP);
				yyval->optype = OP_DECLASSIGN;
				yyval->addChild(yyvsp[-4]);
				yyval->addChild(yyvsp[-1]);
			}
#line 1393 "parser.cpp"
    break;

  case 13: /* BasicType: T_INT  */
#line 82 "parser.y"
                  {yyval = new TreeNode(lineno, NODE_TYPE); yyval->type = TYPE_INT;}
#line 1399 "parser.cpp"
    break;

  case 14: /* BasicType: T_VOID  */
#line 83 "parser.y"
                           {yyval = new TreeNode(lineno, NODE_TYPE); yyval->type = TYPE_VOID;}
#line 1405 "parser.cpp"
    break;

  case 15: /* IntConst: INTEGER  */
#line 86 "parser.y"
                   {yyval = new TreeNode(lineno, NODE_EXPR); yyval->addChild(yyvsp[0]);}
#line 1411 "parser.cpp"
    break;

  case 16: /* CompIdentifier: Identifier  */
#line 89 "parser.y"
                            {yyval = new TreeNode(yyvsp[0]);}
#line 1417 "parser.cpp"
    break;

  case 17: /* CompIdentifier: ArrayIdentifier  */
#line 90 "parser.y"
                                             {
					yyval = yyvsp[0];
					yyval->child->type->visitDim = 0;
				}
#line 1426 "parser.cpp"
    break;

  case 18: /* ArrayIdentifier: Identifier LBRACKET Exp RBRACKET  */
#line 97 "parser.y"
                                                   {
						TreeNode*tmp = new TreeNode(yyvsp[-3]);
						yyval = new TreeNode(lineno, NODE_OP);
						yyval->optype = OP_INDEX;
						yyval->addChild(tmp);
						// 计算数组偏移量倍数
						int biasRate = 1;
						for (unsigned int i = yyvsp[-3]->type->visitDim + 1; i < yyvsp[-3]->type->dim; i++) {
							biasRate *= yyvsp[-3]->type->dimSize[i];
						}
						TreeNode* biasNode;
						if (biasRate == 1) {
							// 偏移倍数为1时省略乘法结点
							biasNode = yyvsp[-1];
						}
						else {
							biasNode = new TreeNode(lineno, NODE_OP);
							biasNode->optype = OP_MUL;
							biasNode->addChild(yyvsp[-1]);
							TreeNode* biasRateExpr = new TreeNode(lineno, NODE_EXPR);
							TreeNode* biasRateConst = new TreeNode(lineno, NODE_CONST);
							biasRateConst->type = TYPE_INT;
							biasRateConst->int_val = biasRate;
							biasRateExpr->addChild(biasRateConst);
							biasNode->addChild(biasRateExpr);
						}
						yyvsp[-3]->type->visitDim++;

						yyval->addChild(biasNode);
					}
#line 1461 "parser.cpp"
    break;

  case 19: /* ArrayIdentifier: ArrayIdentifier LBRACKET Exp RBRACKET  */
#line 127 "parser.y"
                                                                        {
						yyval = yyvsp[-3];
						TreeNode* newBiasNode = new TreeNode(lineno, NODE_OP);
						newBiasNode->optype = OP_ADD;
						newBiasNode->addChild(yyval->child->sibling);
						yyval->child->sibling = newBiasNode;
						// 计算数组偏移量倍数
						int biasRate = 1;
						for (unsigned int i = yyval->child->type->visitDim + 1; i < yyval->child->type->dim; i++) {
							biasRate *= yyval->child->type->dimSize[i];
						}
						TreeNode* biasNode;
						if (biasRate == 1) {
							// 偏移倍数为1时省略乘法结点
							biasNode = yyvsp[-1];
						}
						else {
							biasNode->optype = OP_MUL;
							biasNode->addChild(yyvsp[-1]);
							TreeNode* biasRateExpr = new TreeNode(lineno, NODE_EXPR);
							TreeNode* biasRateConst = new TreeNode(lineno, NODE_CONST);
							biasRateConst->type = TYPE_INT;
							biasRateConst->int_val = biasRate;
							biasRateExpr->addChild(biasRateConst);
							biasNode->addChild(biasRateExpr);
						}
						yyval->child->type->visitDim++;
						newBiasNode->addChild(biasNode);
				}
#line 1495 "parser.cpp"
    break;

  case 20: /* Identifier: IDENTIFIER  */
#line 158 "parser.y"
                        {
				yyval = yyvsp[0];
				int idNameCount = idNameList.count(yyval->var_name);
				int DeclCnt = 0;
				int minDefDis = MAX_SCOPE_STACK;
				// 搜索变量是否已经声明
				auto it = idNameList.find(yyval->var_name);
				int resScoptCmp;
				while (idNameCount--) {
					resScoptCmp = scopeCmp(presentScope, it->second);
					if (resScoptCmp >= 0){
						// 寻找最近的定义
						if (resScoptCmp < minDefDis) {
							minDefDis = resScoptCmp;
							yyval = idList[make_pair(it->first, it->second)];
						}
						DeclCnt++;
					}
					it++;
				}
				if (DeclCnt == 0) {
					string t = "UnDeclared Identifier :'" + yyvsp[0]->var_name + "', scope : " + to_string(resScoptCmp);
					yyerror(t.c_str());
				}
			}
#line 1525 "parser.cpp"
    break;

  case 21: /* DeclCompIdentifier: pDeclIdentifier  */
#line 187 "parser.y"
                  {yyval = yyvsp[0];}
#line 1531 "parser.cpp"
    break;

  case 22: /* DeclCompIdentifier: ArrayDeclIdentifier  */
#line 188 "parser.y"
                      {yyval = yyvsp[0];}
#line 1537 "parser.cpp"
    break;

  case 23: /* pDeclIdentifier: DeclIdentifier  */
#line 192 "parser.y"
                 {yyval = yyvsp[0];}
#line 1543 "parser.cpp"
    break;

  case 24: /* ArrayDeclIdentifier: pDeclIdentifier LBRACKET INTEGER RBRACKET  */
#line 197 "parser.y"
                                            {
  yyval = yyvsp[-3];
  yyval->type->type = VALUE_ARRAY;
  yyval->type->elementType = yyvsp[-3]->type->type;
  yyval->type->dimSize[yyval->type->dim] = yyvsp[-1]->int_val;
  yyval->type->dim++;
}
#line 1555 "parser.cpp"
    break;

  case 25: /* ArrayDeclIdentifier: ArrayDeclIdentifier LBRACKET INTEGER RBRACKET  */
#line 204 "parser.y"
                                                {
  yyval = yyvsp[-3];
  yyval->type->dimSize[yyval->type->dim] = yyvsp[-1]->int_val;
  yyval->type->dim++;
}
#line 1565 "parser.cpp"
    break;

  case 26: /* DeclIdentifier: IDENTIFIER  */
#line 212 "parser.y"
             {
	yyval = yyvsp[0];
	yyval->var_scope = presentScope;
	yyval->type = new Type(NOTYPE);
	#ifdef ID_REDUCE_DEBUG
		cout<<"# $ reduce DeclIdentifier : "<<yyval->var_name<<", at scope :"<<presentScope<<endl;
	#endif
	if (idList.count(make_pair(yyval->var_name, yyval->var_scope)) != 0) {
		string t = "ReDeclared Identifier : " + yyval->var_name;
		yyerror(t.c_str());
	}
	idNameList.insert(make_pair(yyval->var_name, yyval->var_scope));
	idList[make_pair(yyval->var_name, yyval->var_scope)] = yyval;
}
#line 1584 "parser.cpp"
    break;

  case 27: /* ArrayInitVal: IntConst  */
#line 234 "parser.y"
           {yyval = new TreeNode(lineno, NODE_VARLIST); yyval->addChild(yyvsp[0]);}
#line 1590 "parser.cpp"
    break;

  case 28: /* ArrayInitVal: ArrayInitVal COMMA IntConst  */
#line 235 "parser.y"
                              {yyval = yyvsp[-2]; yyval->addChild(yyvsp[0]);}
#line 1596 "parser.cpp"
    break;

  case 29: /* VarDecl: BasicType VarDefs SEMICOLON  */
#line 239 "parser.y"
                              {
  yyval = new TreeNode(lineno, NODE_STMT);
  yyval->stype = STMT_DECL;
  yyval->type = TYPE_NONE;
  yyval->addChild(yyvsp[-2]);
  yyval->addChild(yyvsp[-1]);
  TreeNode* p = yyvsp[-1]->child;
  while(p != nullptr) {
	  if (p->nodeType == NODE_OP) {
		  p->child->type->copy(yyvsp[-2]->type);
	  }
	  else {
	  	p->type->copy(yyvsp[-2]->type);
	  }
	  p = p->sibling;
  }
  #ifdef DECL_DEBUG
	cout << "# $ reduce VarDecl type = " << yyvsp[-2]->type->getTypeInfo() << endl;
	// $$->printAST();
  #endif
}
#line 1622 "parser.cpp"
    break;

  case 30: /* VarDefs: VarDef  */
#line 264 "parser.y"
         {yyval = new TreeNode(lineno, NODE_VARLIST); yyval->addChild(yyvsp[0]);}
#line 1628 "parser.cpp"
    break;

  case 31: /* VarDefs: VarDefs COMMA VarDef  */
#line 265 "parser.y"
                       {yyval = yyvsp[-2]; yyval->addChild(yyvsp[0]);}
#line 1634 "parser.cpp"
    break;

  case 32: /* VarDef: DeclCompIdentifier  */
#line 269 "parser.y"
                     {yyval = yyvsp[0];}
#line 1640 "parser.cpp"
    break;

  case 33: /* VarDef: DeclCompIdentifier ASSIGN Exp  */
#line 270 "parser.y"
                                {
	yyval = new TreeNode(lineno, NODE_OP);
	yyval->optype = OP_DECLASSIGN;
	yyval->addChild(yyvsp[-2]);
	yyval->addChild(yyvsp[0]);
  }
#line 1651 "parser.cpp"
    break;

  case 34: /* VarDef: ArrayDeclIdentifier ASSIGN LBRACE ArrayInitVal RBRACE  */
#line 276 "parser.y"
                                                        {
	yyval = new TreeNode(lineno, NODE_OP);
	yyval->optype = OP_DECLASSIGN;
	yyval->addChild(yyvsp[-4]);
	yyval->addChild(yyvsp[-1]);
  }
#line 1662 "parser.cpp"
    break;

  case 35: /* FuncDef: BasicType pDeclIdentifier FuncLPAREN FuncFParams RPAREN LBRACE BlockItems RBRACE  */
#line 287 "parser.y"
                                                                                   {
	yyval = new TreeNode(lineno, NODE_STMT);
	yyval->stype = STMT_FUNCDECL;
	yyvsp[-6]->type->type = COMPOSE_FUNCTION;
	TreeNode* param = yyvsp[-4];
	while (param != nullptr) {
		yyvsp[-6]->type->paramType[yyvsp[-6]->type->paramNum] = param->child->type;
		yyvsp[-6]->type->paramNum++;
		param = param->sibling;
	}
	yyvsp[-6]->type->retType = yyvsp[-7]->type;
	yyval->addChild(yyvsp[-7]);
	yyval->addChild(yyvsp[-6]);
	TreeNode* params = new TreeNode(lineno, NODE_VARLIST);
	params->addChild(yyvsp[-4]);
	yyval->addChild(params);
	TreeNode* FuncBlock = new TreeNode(lineno, NODE_STMT);
	FuncBlock->stype = STMT_BLOCK;
	FuncBlock->addChild(yyvsp[-1]);
	yyval->addChild(FuncBlock);
	scopePop();
  }
#line 1689 "parser.cpp"
    break;

  case 36: /* FuncDef: BasicType pDeclIdentifier FuncLPAREN RPAREN LBRACE BlockItems RBRACE  */
#line 309 "parser.y"
                                                                       {
	yyval = new TreeNode(lineno, NODE_STMT);
	yyval->stype = STMT_FUNCDECL;
	yyvsp[-5]->type->type = COMPOSE_FUNCTION;
	yyvsp[-5]->type->retType = yyvsp[-6]->type;
	yyval->addChild(yyvsp[-6]);
	yyval->addChild(yyvsp[-5]);
	yyval->addChild(new TreeNode(lineno, NODE_VARLIST));
	TreeNode* FuncBlock = new TreeNode(lineno, NODE_STMT);
	FuncBlock->stype = STMT_BLOCK;
	FuncBlock->addChild(yyvsp[-1]);
	yyval->addChild(FuncBlock);
	scopePop();
  }
#line 1708 "parser.cpp"
    break;

  case 37: /* FuncLPAREN: LPAREN  */
#line 325 "parser.y"
                    {scopePush();}
#line 1714 "parser.cpp"
    break;

  case 38: /* FuncFParams: FuncFParam  */
#line 328 "parser.y"
             {yyval = yyvsp[0];}
#line 1720 "parser.cpp"
    break;

  case 39: /* FuncFParams: FuncFParams COMMA FuncFParam  */
#line 329 "parser.y"
                               {yyval = yyvsp[-2]; yyval->addSibling(yyvsp[0]);}
#line 1726 "parser.cpp"
    break;

  case 40: /* FuncFParam: BasicType pDeclIdentifier  */
#line 333 "parser.y"
                            {
	yyval = new TreeNode(lineno, NODE_PARAM); 
	yyval->addChild(yyvsp[-1]); 
	yyval->addChild(yyvsp[0]);
	yyvsp[0]->type->copy(yyvsp[-1]->type);
  }
#line 1737 "parser.cpp"
    break;

  case 41: /* Block: BlockLBRACE BlockItems BlockRBRACE  */
#line 344 "parser.y"
                                     {
	yyval = new TreeNode(lineno, NODE_STMT);
	yyval->stype = STMT_BLOCK;
	yyval->addChild(yyvsp[-1]);
}
#line 1747 "parser.cpp"
    break;

  case 42: /* BlockLBRACE: LBRACE  */
#line 350 "parser.y"
                     {scopePush();}
#line 1753 "parser.cpp"
    break;

  case 43: /* BlockRBRACE: RBRACE  */
#line 351 "parser.y"
                     {scopePop();}
#line 1759 "parser.cpp"
    break;

  case 44: /* BlockItems: BlockItem  */
#line 354 "parser.y"
            {yyval = yyvsp[0];}
#line 1765 "parser.cpp"
    break;

  case 45: /* BlockItems: BlockItems BlockItem  */
#line 355 "parser.y"
                       {yyval = yyvsp[-1]; yyval->addSibling(yyvsp[0]);}
#line 1771 "parser.cpp"
    break;

  case 46: /* BlockItem: Decl  */
#line 359 "parser.y"
       {yyval = yyvsp[0];}
#line 1777 "parser.cpp"
    break;

  case 47: /* BlockItem: Stmt  */
#line 360 "parser.y"
       {yyval = yyvsp[0];}
#line 1783 "parser.cpp"
    break;

  case 48: /* Stmt_: LBRACE BlockItems RBRACE  */
#line 364 "parser.y"
                           {
	yyval = new TreeNode(lineno, NODE_STMT);
	yyval->stype = STMT_BLOCK;
	yyval->addChild(yyvsp[-1]);
  }
#line 1793 "parser.cpp"
    break;

  case 49: /* Stmt_: Stmt  */
#line 369 "parser.y"
       {yyval = yyvsp[0];}
#line 1799 "parser.cpp"
    break;

  case 50: /* Stmt: SEMICOLON  */
#line 373 "parser.y"
            {yyval = new TreeNode(lineno, NODE_STMT); yyval->stype = STMT_SKIP;}
#line 1805 "parser.cpp"
    break;

  case 51: /* Stmt: Exp SEMICOLON  */
#line 374 "parser.y"
                {yyval = yyvsp[-1];}
#line 1811 "parser.cpp"
    break;

  case 52: /* Stmt: Block  */
#line 375 "parser.y"
        {yyval = yyvsp[0];}
#line 1817 "parser.cpp"
    break;

  case 53: /* Stmt: IF LPAREN Cond RPAREN Stmt_ ELSE Stmt_  */
#line 376 "parser.y"
                                         {
	yyval = new TreeNode(lineno, NODE_STMT);
	yyval->stype = STMT_IFELSE;
	yyval->addChild(yyvsp[-4]);
	yyval->addChild(yyvsp[-2]);
	yyval->addChild(yyvsp[0]);
	scopePop();
	#ifdef IFELSE_DEBUG
		cout << "# $ reduce IF-ELSE at scope : " << presentScope << ", at line " << lineno << endl;
	#endif
  }
#line 1833 "parser.cpp"
    break;

  case 54: /* Stmt: IF LPAREN Cond RPAREN Stmt_  */
#line 387 "parser.y"
                              {
	yyval = new TreeNode(lineno, NODE_STMT);
	yyval->stype = STMT_IF;
	yyval->addChild(yyvsp[-2]);
	yyval->addChild(yyvsp[0]);
	scopePop();
	#ifdef IF_DEBUG
		cout << "% # $ reduce IF at scope : " << presentScope << ", at line " << lineno << endl;
	#endif
  }
#line 1848 "parser.cpp"
    break;

  case 55: /* Stmt: WHILE LPAREN Cond RPAREN Stmt_  */
#line 397 "parser.y"
                                 {
	yyval = new TreeNode(lineno, NODE_STMT);
	yyval->stype = STMT_WHILE;
	yyval->addChild(yyvsp[-2]);
	yyval->addChild(yyvsp[0]);
	scopePop();
	#ifdef WHILE
		cout << "# $ reduce WHILE at scope : " << presentScope << ", at line " << lineno << endl;
	#endif
  }
#line 1863 "parser.cpp"
    break;

  case 56: /* Stmt: BREAK SEMICOLON  */
#line 408 "parser.y"
                  {
	yyval = new TreeNode(lineno, NODE_STMT); 
	yyval->stype = STMT_BREAK; 
	yyval->type = TYPE_NONE;
  }
#line 1873 "parser.cpp"
    break;

  case 57: /* Stmt: CONTINUE SEMICOLON  */
#line 413 "parser.y"
                    {
	yyval = new TreeNode(lineno, NODE_STMT); 
	yyval->stype = STMT_CONTINUE; 
	yyval->type = TYPE_NONE;
  }
#line 1883 "parser.cpp"
    break;

  case 58: /* Stmt: RETURN SEMICOLON  */
#line 418 "parser.y"
                   {yyval = new TreeNode(lineno, NODE_STMT); yyval->stype = STMT_RETURN; yyval->type = TYPE_NONE;}
#line 1889 "parser.cpp"
    break;

  case 59: /* Stmt: RETURN Exp SEMICOLON  */
#line 419 "parser.y"
                       {yyval = new TreeNode(lineno, NODE_STMT); yyval->stype = STMT_RETURN; yyval->addChild(yyvsp[-1]); yyval->type = TYPE_NONE;}
#line 1895 "parser.cpp"
    break;

  case 60: /* IF: IF_  */
#line 422 "parser.y"
         {scopePush();}
#line 1901 "parser.cpp"
    break;

  case 61: /* WHILE: WHILE_  */
#line 423 "parser.y"
               { scopePush();}
#line 1907 "parser.cpp"
    break;

  case 62: /* Exp: addExp  */
#line 428 "parser.y"
         {yyval = yyvsp[0];}
#line 1913 "parser.cpp"
    break;

  case 63: /* Exp: CompIdentifier ASSIGN Exp  */
#line 429 "parser.y"
                            {
	yyval = new TreeNode(lineno, NODE_OP);
	yyval->optype = OP_ASSIGN;
	yyval->addChild(yyvsp[-2]);
	yyval->addChild(yyvsp[0]);
  }
#line 1924 "parser.cpp"
    break;

  case 64: /* Cond: LOrExp  */
#line 438 "parser.y"
         {yyval = yyvsp[0];}
#line 1930 "parser.cpp"
    break;

  case 65: /* addExp: mulExp  */
#line 442 "parser.y"
         {yyval = yyvsp[0];}
#line 1936 "parser.cpp"
    break;

  case 66: /* addExp: addExp PLUS mulExp  */
#line 443 "parser.y"
                     {yyval = new TreeNode(lineno, NODE_OP); yyval->optype = OP_ADD; yyval->addChild(yyvsp[-2]); yyval->addChild(yyvsp[0]);}
#line 1942 "parser.cpp"
    break;

  case 67: /* addExp: addExp MINUS mulExp  */
#line 444 "parser.y"
                      {yyval = new TreeNode(lineno, NODE_OP); yyval->optype = OP_SUB; yyval->addChild(yyvsp[-2]); yyval->addChild(yyvsp[0]);}
#line 1948 "parser.cpp"
    break;

  case 68: /* mulExp: unaryExp  */
#line 449 "parser.y"
           {yyval = yyvsp[0];}
#line 1954 "parser.cpp"
    break;

  case 69: /* mulExp: mulExp MUL unaryExp  */
#line 450 "parser.y"
                      {yyval = new TreeNode(lineno, NODE_OP); yyval->optype = OP_MUL; yyval->addChild(yyvsp[-2]); yyval->addChild(yyvsp[0]);}
#line 1960 "parser.cpp"
    break;

  case 70: /* mulExp: mulExp DIV unaryExp  */
#line 451 "parser.y"
                      {yyval = new TreeNode(lineno, NODE_OP); yyval->optype = OP_DIV; yyval->addChild(yyvsp[-2]); yyval->addChild(yyvsp[0]);}
#line 1966 "parser.cpp"
    break;

  case 71: /* mulExp: mulExp MOD unaryExp  */
#line 452 "parser.y"
                      {yyval = new TreeNode(lineno, NODE_OP); yyval->optype = OP_MOD; yyval->addChild(yyvsp[-2]); yyval->addChild(yyvsp[0]);}
#line 1972 "parser.cpp"
    break;

  case 72: /* unaryExp: primaryExp  */
#line 457 "parser.y"
             {yyval = yyvsp[0];}
#line 1978 "parser.cpp"
    break;

  case 73: /* unaryExp: NOT Cond  */
#line 458 "parser.y"
           {yyval = new TreeNode(lineno, NODE_OP); yyval->optype = OP_NOT; yyval->addChild(yyvsp[0]);}
#line 1984 "parser.cpp"
    break;

  case 74: /* unaryExp: PLUS primaryExp  */
#line 459 "parser.y"
                  {yyval = new TreeNode(lineno, NODE_OP); yyval->optype = OP_POS; yyval->addChild(yyvsp[0]);}
#line 1990 "parser.cpp"
    break;

  case 75: /* unaryExp: MINUS primaryExp  */
#line 460 "parser.y"
                   {yyval = new TreeNode(lineno, NODE_OP); yyval->optype = OP_NAG; yyval->addChild(yyvsp[0]);}
#line 1996 "parser.cpp"
    break;

  case 76: /* primaryExp: IntConst  */
#line 465 "parser.y"
           {yyval = yyvsp[0];}
#line 2002 "parser.cpp"
    break;

  case 77: /* primaryExp: LPAREN Exp RPAREN  */
#line 466 "parser.y"
                    {yyval = yyvsp[-1];}
#line 2008 "parser.cpp"
    break;

  case 78: /* primaryExp: LPAREN Cond RPAREN  */
#line 467 "parser.y"
                     {yyval = yyvsp[-1];}
#line 2014 "parser.cpp"
    break;

  case 79: /* primaryExp: Identifier LPAREN FuncRParams RPAREN  */
#line 468 "parser.y"
                                       {
	yyval = new TreeNode(lineno, NODE_FUNCALL);
	TreeNode *tmp = new TreeNode(yyvsp[-3]);
	yyval->addChild(tmp);
	yyval->addChild(yyvsp[-1]);
  }
#line 2025 "parser.cpp"
    break;

  case 80: /* primaryExp: Identifier LPAREN RPAREN  */
#line 474 "parser.y"
                           {
	yyval = new TreeNode(lineno, NODE_FUNCALL);
	TreeNode *tmp = new TreeNode(yyvsp[-2]);
	yyval->addChild(tmp);
	yyval->addChild(new TreeNode(lineno, NODE_VARLIST));
  }
#line 2036 "parser.cpp"
    break;

  case 81: /* primaryExp: CompIdentifier  */
#line 480 "parser.y"
                 {yyval = new TreeNode(lineno, NODE_EXPR); yyval->addChild(yyvsp[0]);}
#line 2042 "parser.cpp"
    break;

  case 82: /* FuncRParams: Exp  */
#line 485 "parser.y"
      {yyval = new TreeNode(lineno, NODE_VARLIST); yyval->addChild(yyvsp[0]);}
#line 2048 "parser.cpp"
    break;

  case 83: /* FuncRParams: FuncRParams COMMA Exp  */
#line 486 "parser.y"
                        {yyval = yyvsp[-2]; yyval->addChild(yyvsp[0]);}
#line 2054 "parser.cpp"
    break;

  case 84: /* LOrExp: LAndExp  */
#line 491 "parser.y"
          {yyval = yyvsp[0];}
#line 2060 "parser.cpp"
    break;

  case 85: /* LOrExp: LAndExp OR LOrExp  */
#line 492 "parser.y"
                    {yyval = new TreeNode(lineno, NODE_OP); yyval->optype = OP_OR; yyval->addChild(yyvsp[-2]); yyval->addChild(yyvsp[0]);}
#line 2066 "parser.cpp"
    break;

  case 86: /* LAndExp: eqExp  */
#line 497 "parser.y"
        {yyval = yyvsp[0];}
#line 2072 "parser.cpp"
    break;

  case 87: /* LAndExp: eqExp AND LAndExp  */
#line 498 "parser.y"
                    {yyval = new TreeNode(lineno, NODE_OP); yyval->optype = OP_AND; yyval->addChild(yyvsp[-2]); yyval->addChild(yyvsp[0]);}
#line 2078 "parser.cpp"
    break;

  case 88: /* eqExp: relExp  */
#line 503 "parser.y"
         {yyval = yyvsp[0];}
#line 2084 "parser.cpp"
    break;

  case 89: /* eqExp: relExp EQ eqExp  */
#line 504 "parser.y"
                  {yyval = new TreeNode(lineno, NODE_OP); yyval->optype = OP_EQ; yyval->addChild(yyvsp[-2]); yyval->addChild(yyvsp[0]);}
#line 2090 "parser.cpp"
    break;

  case 90: /* eqExp: relExp NEQ eqExp  */
#line 505 "parser.y"
                   {yyval = new TreeNode(lineno, NODE_OP); yyval->optype = OP_NEQ; yyval->addChild(yyvsp[-2]); yyval->addChild(yyvsp[0]);}
#line 2096 "parser.cpp"
    break;

  case 91: /* relExp: Exp  */
#line 510 "parser.y"
      {yyval = yyvsp[0];}
#line 2102 "parser.cpp"
    break;

  case 92: /* relExp: relExp GRA Exp  */
#line 511 "parser.y"
                 {yyval = new TreeNode(lineno, NODE_OP); yyval->optype = OP_GRA; yyval->addChild(yyvsp[-2]); yyval->addChild(yyvsp[0]);}
#line 2108 "parser.cpp"
    break;

  case 93: /* relExp: relExp LES Exp  */
#line 512 "parser.y"
                 {yyval = new TreeNode(lineno, NODE_OP); yyval->optype = OP_LES; yyval->addChild(yyvsp[-2]); yyval->addChild(yyvsp[0]);}
#line 2114 "parser.cpp"
    break;

  case 94: /* relExp: relExp GRAEQ Exp  */
#line 513 "parser.y"
                   {yyval = new TreeNode(lineno, NODE_OP); yyval->optype = OP_GRAEQ; yyval->addChild(yyvsp[-2]); yyval->addChild(yyvsp[0]);}
#line 2120 "parser.cpp"
    break;

  case 95: /* relExp: relExp LESEQ Exp  */
#line 514 "parser.y"
                   {yyval = new TreeNode(lineno, NODE_OP); yyval->optype = OP_LESEQ; yyval->addChild(yyvsp[-2]); yyval->addChild(yyvsp[0]);}
#line 2126 "parser.cpp"
    break;


#line 2130 "parser.cpp"

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

#line 517 "parser.y"


int yyerror(char const * message)
{
	cout << "error: " << message << ", at line " << lineno << endl;
	parserError = true;
	return 0;
}

int scopeCmp(string presScope, string varScope) {
	unsigned int plen = presScope.length(), vlen = varScope.length();
	unsigned int minlen = min(plen, vlen);
	if (presScope.substr(0, minlen) == varScope.substr(0, minlen)) {
		if (plen >= vlen)
			return plen - vlen;
		else
			return -1;
	}
	return -2;
}

void scopePush() {
	presentScope += max_scope_id;
	max_scope_id = SCOPT_ID_BASE;
	top++;
#ifdef SCOPE_DEBUG
	cout << "* push -> " << presentScope << ", at line " << lineno << endl;
#endif
}

void scopePop() {
	max_scope_id = presentScope[top] + 1;
	presentScope = presentScope.substr(0, presentScope.length() - 1);
	top--;
#ifdef SCOPE_DEBUG
	cout << "* pop -> " << presentScope << ", at line " << lineno << endl;
#endif
}

