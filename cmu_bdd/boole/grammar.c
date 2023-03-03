extern char *malloc(), *realloc();

# line 2 "grammar.y"
#include "boole.h"


void
#if defined(__STDC__)
yyerror(char *s)
#else
yyerror(s)
  char *s;
#endif
{
  if (interactive)
    fprintf(stderr, "Error: %s\n", s);
  else
    fprintf(stderr, "Error: %s at line %d\n", s, line_number);
}


int
#if defined(__STDC__)
yywrap(void)
#else
yywrap()
#endif
{
  return (1);
}

# line 32 "grammar.y"
typedef union 
{
  st_entry e;
  bdd f;
  assoc a;
  vec v;
  long i;
} YYSTYPE;
# define UNKNOWNCHAR 257
# define ID 258
# define NEWID 259
# define NUM 260
# define CLEAR 261
# define ALL 262
# define FUNCPROFILE 263
# define GC 264
# define LIMIT 265
# define PRINT 266
# define PRINTSOP 267
# define PROFILE 268
# define REORDER 269
# define SIFT 270
# define WINDOW 271
# define HYBRID 272
# define NONE 273
# define SATISFY 274
# define SATISFYSUPPORT 275
# define SATISFYFRACTION 276
# define SIZE 277
# define STATS 278
# define TIMER 279
# define OFF 280
# define VARS 281
# define AGETS 282
# define GETS 283
# define EXISTS 284
# define FORALL 285
# define SUBST 286
#define yyclearin yychar = -1
#define yyerrok yyerrflag = 0
extern int yychar;
extern int yyerrflag;
#ifndef YYMAXDEPTH
#define YYMAXDEPTH 150
#endif
YYSTYPE yylval, yyval;
# define YYERRCODE 256

# line 766 "grammar.y"

int yyexca[] ={
-1, 1,
	0, -1,
	-2, 0,
-1, 122,
	61, 0,
	-2, 35,
	};
# define YYNPROD 78
# define YYLAST 300
int yyact[]={

    36,   135,    38,    37,   130,    21,    75,    22,    23,    24,
    25,    26,    27,    28,    40,    39,   128,    55,    29,    30,
    31,    32,    33,    34,    54,    35,    65,    46,    43,    55,
   111,    41,    52,    53,    55,    70,    54,   117,   116,    49,
    47,    54,    81,   109,    52,    53,    55,   115,    90,    52,
    53,    20,    90,    54,    90,   100,    93,   140,    90,    90,
    83,    52,    53,    90,    90,   127,   139,   138,    90,   126,
   114,    93,   125,    89,   113,    93,   110,    93,   131,    89,
    76,    93,    93,    89,    84,   124,    93,    93,    89,    89,
    90,    93,   108,   123,    94,    95,   105,   104,    90,   103,
    84,    88,   102,   142,    92,    88,   101,    88,    92,    99,
    92,    88,    84,    93,    92,    89,    88,    88,    87,    92,
    92,    93,    86,    89,    85,    80,    77,    73,   112,   118,
   119,   120,   121,   122,    91,    45,    19,    67,    91,    44,
    91,    18,    17,    88,    91,    91,    92,    48,    16,    91,
    91,    88,    15,    14,    92,   129,    13,    12,    11,    10,
     9,     8,     7,     6,   132,   133,   134,    60,     5,     4,
   136,   137,    72,     3,    59,   141,    91,    66,    79,    71,
     2,   117,   116,     1,    91,    42,   107,    78,     0,     0,
     0,     0,     0,     0,    96,    97,    98,     0,     0,     0,
     0,     0,    69,    68,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,   106,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,    74,     0,    38,
    37,     0,     0,     0,     0,     0,     0,    61,    62,    63,
    64,    82,    51,    50,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,    51,    50,     0,     0,     0,    51,
    50,     0,     0,     0,     0,     0,     0,     0,    56,    57,
    58,    51,    50,     0,     0,     0,     0,     0,     0,     0,
    56,    57,    58,     0,     0,    56,    57,    58,     0,     0,
     0,     0,     0,     0,     0,     0,     0,    56,    57,    58 };
int yypact[]={

 -1000,  -256, -1000, -1000, -1000, -1000, -1000, -1000, -1000, -1000,
 -1000, -1000, -1000, -1000, -1000, -1000, -1000, -1000, -1000, -1000,
  -268,  -231, -1000,    76,  -233,    13,    13, -1000,   -33,    13,
   -56,    13, -1000,    68,   -53, -1000,    67, -1000, -1000,    13,
   -56,    66,   -17, -1000,     1, -1000,    65,    63,    59,    60,
 -1000, -1000, -1000, -1000,    13,    13,   -56,   -56,   -56,    50,
    -4,    47,    43,    40,    38, -1000,    37,    13, -1000, -1000,
 -1000,    33,   -16, -1000,    17, -1000,   -29, -1000,    15,    11,
 -1000, -1000, -1000, -1000,    60, -1000, -1000, -1000,  -221,    13,
    13,    13,    13,    13,    52, -1000,    45,    32,    29, -1000,
 -1000, -1000, -1000, -1000, -1000, -1000,     6,   -77, -1000, -1000,
 -1000, -1000, -1000, -1000, -1000,  -279, -1000, -1000,    20,    -5,
    30,    21, -1000, -1000,    13,    13,    13, -1000, -1000,  -282,
    13,    13,    26,    25,    16,    13,    10,    14, -1000, -1000,
 -1000,    60, -1000 };
int yypgo[]={

     0,    51,   147,    39,   137,   139,    47,   186,   185,   183,
   180,   173,   169,   168,   163,   162,   161,   160,   159,   158,
   157,   156,   153,   152,   148,   142,   141,   136,    80 };
int yyr1[]={

     0,     9,     9,    10,    10,    10,    10,    10,    10,    10,
    10,    10,    10,    10,    10,    10,    10,    10,    10,    10,
    11,    11,     1,     1,     2,     3,     3,     3,     3,     3,
     3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
     4,     4,     4,     7,     7,     7,     6,     6,    12,    12,
     8,     8,     8,    13,     5,     5,    14,    15,    15,    16,
    17,    18,    19,    19,    19,    19,    19,    20,    21,    22,
    23,    24,    25,    25,    26,    28,    28,    27 };
int yyr2[]={

     0,     5,     0,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     9,     9,     3,     3,     3,     3,     3,     3,     3,     7,
    13,    11,     7,     7,     7,     7,     5,    11,    11,    11,
     3,     3,     7,     5,     9,     1,     3,     3,     7,     7,
     3,     5,     1,     7,     5,     1,     5,     7,     7,     7,
     7,     7,     7,     7,     7,     7,     5,     7,     9,     7,
     7,     5,     7,     5,     7,     5,     1,     5 };
int yychk[]={

 -1000,    -9,   -10,   -11,   -12,   -13,   -14,   -15,   -16,   -17,
   -18,   -19,   -20,   -21,   -22,   -23,   -24,   -25,   -26,   -27,
    -1,   261,   263,   264,   265,   266,   267,   268,   269,   274,
   275,   276,   277,   278,   279,   281,   256,   259,   258,   283,
   282,   262,    -8,   259,    -5,    59,   260,   273,    -2,    -3,
   259,   258,    48,    49,    40,    33,   284,   285,   286,    -2,
    -5,   270,   271,   272,   273,    59,    -2,    -4,   259,   258,
    91,    -2,    -5,    59,   280,    59,   -28,    59,    -2,    -4,
    59,    59,   258,    59,    -3,    59,    59,    59,    91,    63,
    38,   124,    94,    61,    -3,    -3,    -4,    -4,    -4,    59,
    59,    59,    59,    59,    59,    59,    -2,    -7,    59,    59,
    59,    59,    -1,    59,    59,    -6,   259,   258,    -3,    -3,
    -3,    -3,    -3,    41,    40,    40,    40,    59,    93,    -6,
   283,    58,    -3,    -3,    -3,   283,    -3,    -3,    41,    41,
    41,    -3,    93 };
int yydef[]={

     2,    -2,     1,     3,     4,     5,     6,     7,     8,     9,
    10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
     0,    52,    55,     0,     0,     0,     0,    55,     0,     0,
     0,     0,    55,     0,     0,    76,     0,    22,    23,     0,
     0,     0,     0,    50,     0,    56,     0,     0,     0,    24,
    25,    26,    27,    28,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,    66,     0,     0,    40,    41,
    45,     0,     0,    71,     0,    73,     0,    77,     0,     0,
    48,    49,    51,    53,    54,    57,    58,    59,     0,     0,
     0,     0,     0,     0,     0,    36,     0,     0,     0,    60,
    61,    62,    63,    64,    65,    67,     0,     0,    69,    70,
    72,    74,    75,    20,    21,     0,    46,    47,     0,    32,
    33,    34,    -2,    29,     0,     0,     0,    68,    42,    43,
     0,     0,     0,     0,     0,     0,     0,    31,    37,    38,
    39,    44,    30 };
typedef struct { char *t_name; int t_val; } yytoktype;
#ifndef YYDEBUG
#	define YYDEBUG	0	/* don't allow debugging */
#endif

#if YYDEBUG

yytoktype yytoks[] =
{
	"UNKNOWNCHAR",	257,
	"ID",	258,
	"NEWID",	259,
	"NUM",	260,
	"CLEAR",	261,
	"ALL",	262,
	"FUNCPROFILE",	263,
	"GC",	264,
	"LIMIT",	265,
	"PRINT",	266,
	"PRINTSOP",	267,
	"PROFILE",	268,
	"REORDER",	269,
	"SIFT",	270,
	"WINDOW",	271,
	"HYBRID",	272,
	"NONE",	273,
	"SATISFY",	274,
	"SATISFYSUPPORT",	275,
	"SATISFYFRACTION",	276,
	"SIZE",	277,
	"STATS",	278,
	"TIMER",	279,
	"OFF",	280,
	"VARS",	281,
	"AGETS",	282,
	"GETS",	283,
	"EXISTS",	284,
	"FORALL",	285,
	"SUBST",	286,
	"?",	63,
	":",	58,
	"[",	91,
	"^",	94,
	"|",	124,
	"&",	38,
	"=",	61,
	"!",	33,
	"-unknown-",	-1	/* ends search */
};

char * yyreds[] =
{
	"-no such reduction-",
	"file : file command",
	"file : /* empty */",
	"command : assignment",
	"command : clear",
	"command : funcprofile",
	"command : gc",
	"command : limit",
	"command : print",
	"command : printsop",
	"command : profile",
	"command : reorder",
	"command : satisfy",
	"command : satisfysupport",
	"command : satisfyfraction",
	"command : size",
	"command : stats",
	"command : timer",
	"command : vars",
	"command : errorcmd",
	"assignment : assignatom GETS expr ';'",
	"assignment : assignatom AGETS assoc ';'",
	"assignatom : NEWID",
	"assignatom : ID",
	"expr : expraux",
	"expraux : NEWID",
	"expraux : ID",
	"expraux : '0'",
	"expraux : '1'",
	"expraux : '(' expraux ')'",
	"expraux : expraux '[' var GETS expraux ']'",
	"expraux : expraux '?' expraux ':' expraux",
	"expraux : expraux '&' expraux",
	"expraux : expraux '|' expraux",
	"expraux : expraux '^' expraux",
	"expraux : expraux '=' expraux",
	"expraux : '!' expraux",
	"expraux : EXISTS assoc '(' expraux ')'",
	"expraux : FORALL assoc '(' expraux ')'",
	"expraux : SUBST assoc '(' expraux ')'",
	"assoc : NEWID",
	"assoc : ID",
	"assoc : '[' pairs ']'",
	"pairs : pairs var",
	"pairs : pairs var GETS expraux",
	"pairs : /* empty */",
	"var : NEWID",
	"var : ID",
	"clear : CLEAR ALL ';'",
	"clear : CLEAR ids ';'",
	"ids : NEWID",
	"ids : ids ID",
	"ids : /* empty */",
	"funcprofile : FUNCPROFILE exprs ';'",
	"exprs : exprs expraux",
	"exprs : /* empty */",
	"gc : GC ';'",
	"limit : LIMIT NUM ';'",
	"limit : LIMIT NONE ';'",
	"print : PRINT expr ';'",
	"printsop : PRINTSOP expr ';'",
	"profile : PROFILE exprs ';'",
	"reorder : REORDER SIFT ';'",
	"reorder : REORDER WINDOW ';'",
	"reorder : REORDER HYBRID ';'",
	"reorder : REORDER NONE ';'",
	"reorder : REORDER ';'",
	"satisfy : SATISFY expr ';'",
	"satisfysupport : SATISFYSUPPORT assoc expr ';'",
	"satisfyfraction : SATISFYFRACTION expr ';'",
	"size : SIZE exprs ';'",
	"stats : STATS ';'",
	"timer : TIMER OFF ';'",
	"timer : TIMER ';'",
	"vars : VARS vardecls ';'",
	"vardecls : vardecls assignatom",
	"vardecls : /* empty */",
	"errorcmd : error ';'",
};
#endif /* YYDEBUG */
#line 1 "/usr/lib/yaccpar"
/*	@(#)yaccpar 1.10 89/04/04 SMI; from S5R3 1.10	*/

/*
** Skeleton parser driver for yacc output
*/

/*
** yacc user known macros and defines
*/
#define YYERROR		goto yyerrlab
#define YYACCEPT	{ free(yys); free(yyv); return(0); }
#define YYABORT		{ free(yys); free(yyv); return(1); }
#define YYBACKUP( newtoken, newvalue )\
{\
	if ( yychar >= 0 || ( yyr2[ yytmp ] >> 1 ) != 1 )\
	{\
		yyerror( "syntax error - cannot backup" );\
		goto yyerrlab;\
	}\
	yychar = newtoken;\
	yystate = *yyps;\
	yylval = newvalue;\
	goto yynewstate;\
}
#define YYRECOVERING()	(!!yyerrflag)
#ifndef YYDEBUG
#	define YYDEBUG	1	/* make debugging available */
#endif

/*
** user known globals
*/
int yydebug;			/* set to 1 to get debugging */

/*
** driver internal defines
*/
#define YYFLAG		(-1000)

/*
** static variables used by the parser
*/
static YYSTYPE *yyv;			/* value stack */
static int *yys;			/* state stack */

static YYSTYPE *yypv;			/* top of value stack */
static int *yyps;			/* top of state stack */

static int yystate;			/* current state */
static int yytmp;			/* extra var (lasts between blocks) */

int yynerrs;			/* number of errors */

int yyerrflag;			/* error recovery flag */
int yychar;			/* current input token number */


/*
** yyparse - return 0 if worked, 1 if syntax error not recovered from
*/
int
yyparse()
{
	register YYSTYPE *yypvt;	/* top of value stack for $vars */
	unsigned yymaxdepth = YYMAXDEPTH;

	/*
	** Initialize externals - yyparse may be called more than once
	*/
	yyv = (YYSTYPE*)malloc(yymaxdepth*sizeof(YYSTYPE));
	yys = (int*)malloc(yymaxdepth*sizeof(int));
	if (!yyv || !yys)
	{
		yyerror( "out of memory" );
		return(1);
	}
	yypv = &yyv[-1];
	yyps = &yys[-1];
	yystate = 0;
	yytmp = 0;
	yynerrs = 0;
	yyerrflag = 0;
	yychar = -1;

	goto yystack;
	{
		register YYSTYPE *yy_pv;	/* top of value stack */
		register int *yy_ps;		/* top of state stack */
		register int yy_state;		/* current state */
		register int  yy_n;		/* internal state number info */

		/*
		** get globals into registers.
		** branch to here only if YYBACKUP was called.
		*/
	yynewstate:
		yy_pv = yypv;
		yy_ps = yyps;
		yy_state = yystate;
		goto yy_newstate;

		/*
		** get globals into registers.
		** either we just started, or we just finished a reduction
		*/
	yystack:
		yy_pv = yypv;
		yy_ps = yyps;
		yy_state = yystate;

		/*
		** top of for (;;) loop while no reductions done
		*/
	yy_stack:
		/*
		** put a state and value onto the stacks
		*/
#if YYDEBUG
		/*
		** if debugging, look up token value in list of value vs.
		** name pairs.  0 and negative (-1) are special values.
		** Note: linear search is used since time is not a real
		** consideration while debugging.
		*/
		if ( yydebug )
		{
			register int yy_i;

			(void)printf( "State %d, token ", yy_state );
			if ( yychar == 0 )
				(void)printf( "end-of-file\n" );
			else if ( yychar < 0 )
				(void)printf( "-none-\n" );
			else
			{
				for ( yy_i = 0; yytoks[yy_i].t_val >= 0;
					yy_i++ )
				{
					if ( yytoks[yy_i].t_val == yychar )
						break;
				}
				(void)printf( "%s\n", yytoks[yy_i].t_name );
			}
		}
#endif /* YYDEBUG */
		if ( ++yy_ps >= &yys[ yymaxdepth ] )	/* room on stack? */
		{
			/*
			** reallocate and recover.  Note that pointers
			** have to be reset, or bad things will happen
			*/
			int yyps_index = (yy_ps - yys);
			int yypv_index = (yy_pv - yyv);
			int yypvt_index = (yypvt - yyv);
			yymaxdepth += YYMAXDEPTH;
			yyv = (YYSTYPE*)realloc((char*)yyv,
				yymaxdepth * sizeof(YYSTYPE));
			yys = (int*)realloc((char*)yys,
				yymaxdepth * sizeof(int));
			if (!yyv || !yys)
			{
				yyerror( "yacc stack overflow" );
				return(1);
			}
			yy_ps = yys + yyps_index;
			yy_pv = yyv + yypv_index;
			yypvt = yyv + yypvt_index;
		}
		*yy_ps = yy_state;
		*++yy_pv = yyval;

		/*
		** we have a new state - find out what to do
		*/
	yy_newstate:
		if ( ( yy_n = yypact[ yy_state ] ) <= YYFLAG )
			goto yydefault;		/* simple state */
#if YYDEBUG
		/*
		** if debugging, need to mark whether new token grabbed
		*/
		yytmp = yychar < 0;
#endif
		if ( ( yychar < 0 ) && ( ( yychar = yylex() ) < 0 ) )
			yychar = 0;		/* reached EOF */
#if YYDEBUG
		if ( yydebug && yytmp )
		{
			register int yy_i;

			(void)printf( "Received token " );
			if ( yychar == 0 )
				(void)printf( "end-of-file\n" );
			else if ( yychar < 0 )
				(void)printf( "-none-\n" );
			else
			{
				for ( yy_i = 0; yytoks[yy_i].t_val >= 0;
					yy_i++ )
				{
					if ( yytoks[yy_i].t_val == yychar )
						break;
				}
				(void)printf( "%s\n", yytoks[yy_i].t_name );
			}
		}
#endif /* YYDEBUG */
		if ( ( ( yy_n += yychar ) < 0 ) || ( yy_n >= YYLAST ) )
			goto yydefault;
		if ( yychk[ yy_n = yyact[ yy_n ] ] == yychar )	/*valid shift*/
		{
			yychar = -1;
			yyval = yylval;
			yy_state = yy_n;
			if ( yyerrflag > 0 )
				yyerrflag--;
			goto yy_stack;
		}

	yydefault:
		if ( ( yy_n = yydef[ yy_state ] ) == -2 )
		{
#if YYDEBUG
			yytmp = yychar < 0;
#endif
			if ( ( yychar < 0 ) && ( ( yychar = yylex() ) < 0 ) )
				yychar = 0;		/* reached EOF */
#if YYDEBUG
			if ( yydebug && yytmp )
			{
				register int yy_i;

				(void)printf( "Received token " );
				if ( yychar == 0 )
					(void)printf( "end-of-file\n" );
				else if ( yychar < 0 )
					(void)printf( "-none-\n" );
				else
				{
					for ( yy_i = 0;
						yytoks[yy_i].t_val >= 0;
						yy_i++ )
					{
						if ( yytoks[yy_i].t_val
							== yychar )
						{
							break;
						}
					}
					(void)printf( "%s\n", yytoks[yy_i].t_name );
				}
			}
#endif /* YYDEBUG */
			/*
			** look through exception table
			*/
			{
				register int *yyxi = yyexca;

				while ( ( *yyxi != -1 ) ||
					( yyxi[1] != yy_state ) )
				{
					yyxi += 2;
				}
				while ( ( *(yyxi += 2) >= 0 ) &&
					( *yyxi != yychar ) )
					;
				if ( ( yy_n = yyxi[1] ) < 0 )
					YYACCEPT;
			}
		}

		/*
		** check for syntax error
		*/
		if ( yy_n == 0 )	/* have an error */
		{
			/* no worry about speed here! */
			switch ( yyerrflag )
			{
			case 0:		/* new error */
				yyerror( "syntax error" );
				goto skip_init;
			yyerrlab:
				/*
				** get globals into registers.
				** we have a user generated syntax type error
				*/
				yy_pv = yypv;
				yy_ps = yyps;
				yy_state = yystate;
				yynerrs++;
			skip_init:
			case 1:
			case 2:		/* incompletely recovered error */
					/* try again... */
				yyerrflag = 3;
				/*
				** find state where "error" is a legal
				** shift action
				*/
				while ( yy_ps >= yys )
				{
					yy_n = yypact[ *yy_ps ] + YYERRCODE;
					if ( yy_n >= 0 && yy_n < YYLAST &&
						yychk[yyact[yy_n]] == YYERRCODE)					{
						/*
						** simulate shift of "error"
						*/
						yy_state = yyact[ yy_n ];
						goto yy_stack;
					}
					/*
					** current state has no shift on
					** "error", pop stack
					*/
#if YYDEBUG
#	define _POP_ "Error recovery pops state %d, uncovers state %d\n"
					if ( yydebug )
						(void)printf( _POP_, *yy_ps,
							yy_ps[-1] );
#	undef _POP_
#endif
					yy_ps--;
					yy_pv--;
				}
				/*
				** there is no state on stack with "error" as
				** a valid shift.  give up.
				*/
				YYABORT;
			case 3:		/* no shift yet; eat a token */
#if YYDEBUG
				/*
				** if debugging, look up token in list of
				** pairs.  0 and negative shouldn't occur,
				** but since timing doesn't matter when
				** debugging, it doesn't hurt to leave the
				** tests here.
				*/
				if ( yydebug )
				{
					register int yy_i;

					(void)printf( "Error recovery discards " );
					if ( yychar == 0 )
						(void)printf( "token end-of-file\n" );
					else if ( yychar < 0 )
						(void)printf( "token -none-\n" );
					else
					{
						for ( yy_i = 0;
							yytoks[yy_i].t_val >= 0;
							yy_i++ )
						{
							if ( yytoks[yy_i].t_val
								== yychar )
							{
								break;
							}
						}
						(void)printf( "token %s\n",
							yytoks[yy_i].t_name );
					}
				}
#endif /* YYDEBUG */
				if ( yychar == 0 )	/* reached EOF. quit */
					YYABORT;
				yychar = -1;
				goto yy_newstate;
			}
		}/* end if ( yy_n == 0 ) */
		/*
		** reduction by production yy_n
		** put stack tops, etc. so things right after switch
		*/
#if YYDEBUG
		/*
		** if debugging, print the string that is the user's
		** specification of the reduction which is just about
		** to be done.
		*/
		if ( yydebug )
			(void)printf( "Reduce by (%d) \"%s\"\n",
				yy_n, yyreds[ yy_n ] );
#endif
		yytmp = yy_n;			/* value to switch over */
		yypvt = yy_pv;			/* $vars top of value stack */
		/*
		** Look in goto table for next state
		** Sorry about using yy_state here as temporary
		** register variable, but why not, if it works...
		** If yyr2[ yy_n ] doesn't have the low order bit
		** set, then there is no action to be done for
		** this reduction.  So, no saving & unsaving of
		** registers done.  The only difference between the
		** code just after the if and the body of the if is
		** the goto yy_stack in the body.  This way the test
		** can be made before the choice of what to do is needed.
		*/
		{
			/* length of production doubled with extra bit */
			register int yy_len = yyr2[ yy_n ];

			if ( !( yy_len & 01 ) )
			{
				yy_len >>= 1;
				yyval = ( yy_pv -= yy_len )[1];	/* $$ = $1 */
				yy_state = yypgo[ yy_n = yyr1[ yy_n ] ] +
					*( yy_ps -= yy_len ) + 1;
				if ( yy_state >= YYLAST ||
					yychk[ yy_state =
					yyact[ yy_state ] ] != -yy_n )
				{
					yy_state = yyact[ yypgo[ yy_n ] ];
				}
				goto yy_stack;
			}
			yy_len >>= 1;
			yyval = ( yy_pv -= yy_len )[1];	/* $$ = $1 */
			yy_state = yypgo[ yy_n = yyr1[ yy_n ] ] +
				*( yy_ps -= yy_len ) + 1;
			if ( yy_state >= YYLAST ||
				yychk[ yy_state = yyact[ yy_state ] ] != -yy_n )
			{
				yy_state = yyact[ yypgo[ yy_n ] ];
			}
		}
					/* save until reenter driver code */
		yystate = yy_state;
		yyps = yy_ps;
		yypv = yy_pv;
	}
	/*
	** code supplied by user is placed in this switch
	*/
	switch( yytmp )
	{
		
case 1:
# line 94 "grammar.y"
{
  first_line=1;
} break;
case 20:
# line 142 "grammar.y"
{
  yypvt[-3].e->type=EXPR;
  yypvt[-3].e->info.expr.f=yypvt[-1].f;
} break;
case 21:
# line 148 "grammar.y"
{
  yypvt[-3].e->type=VARASSOC;
  yypvt[-3].e->info.varassoc.a=yypvt[-1].a;
} break;
case 22:
# line 158 "grammar.y"
{
  yyval.e=yypvt[-0].e;
} break;
case 23:
# line 163 "grammar.y"
{
  if (yypvt[-0].e->type == KEYWORD)
    {
      yyerror("identifier expected");
      YYERROR;
    }
  free_entry(yypvt[-0].e);
  yypvt[-0].e->type=NEW;
  yyval.e=yypvt[-0].e;
} break;
case 24:
# line 178 "grammar.y"
{
  if (yypvt[-0].f)
    yyval.f=yypvt[-0].f;
  else
    {
      yyerror("node limit exceeded");
      YYERROR;
    }
} break;
case 25:
# line 192 "grammar.y"
{
  yyerror(undefined(yypvt[-0].e->id));
  YYERROR;
} break;
case 26:
# line 198 "grammar.y"
{
  if (yypvt[-0].e->type != EXPR)
    {
      yyerror("expression expected");
      YYERROR;
    }
  yyval.f=bdd_identity(bddm, yypvt[-0].e->info.expr.f);
} break;
case 27:
# line 208 "grammar.y"
{
  yyval.f=bdd_zero(bddm);
} break;
case 28:
# line 213 "grammar.y"
{
  yyval.f=bdd_one(bddm);
} break;
case 29:
# line 218 "grammar.y"
{
  yyval.f=yypvt[-1].f;
} break;
case 30:
# line 223 "grammar.y"
{
  yyval.f=bdd_compose(bddm, yypvt[-5].f, yypvt[-3].f, yypvt[-1].f);
  bdd_free(bddm, yypvt[-5].f);
  bdd_free(bddm, yypvt[-1].f);
} break;
case 31:
# line 230 "grammar.y"
{
  yyval.f=bdd_ite(bddm, yypvt[-4].f, yypvt[-2].f, yypvt[-0].f);
  bdd_free(bddm, yypvt[-4].f);
  bdd_free(bddm, yypvt[-2].f);
  bdd_free(bddm, yypvt[-0].f);
} break;
case 32:
# line 238 "grammar.y"
{
  yyval.f=bdd_and(bddm, yypvt[-2].f, yypvt[-0].f);
  bdd_free(bddm, yypvt[-2].f);
  bdd_free(bddm, yypvt[-0].f);
} break;
case 33:
# line 245 "grammar.y"
{
  yyval.f=bdd_or(bddm, yypvt[-2].f, yypvt[-0].f);
  bdd_free(bddm, yypvt[-2].f);
  bdd_free(bddm, yypvt[-0].f);
} break;
case 34:
# line 252 "grammar.y"
{
  yyval.f=bdd_xor(bddm, yypvt[-2].f, yypvt[-0].f);
  bdd_free(bddm, yypvt[-2].f);
  bdd_free(bddm, yypvt[-0].f);
} break;
case 35:
# line 259 "grammar.y"
{
  yyval.f=bdd_xnor(bddm, yypvt[-2].f, yypvt[-0].f);
  bdd_free(bddm, yypvt[-2].f);
  bdd_free(bddm, yypvt[-0].f);
} break;
case 36:
# line 266 "grammar.y"
{
  yyval.f=bdd_not(bddm, yypvt[-0].f);
  bdd_free(bddm, yypvt[-0].f);
} break;
case 37:
# line 272 "grammar.y"
{
  use_assoc(yypvt[-3].a);
  yyval.f=bdd_exists(bddm, yypvt[-1].f);
  free_assoc(yypvt[-3].a);
  bdd_free(bddm, yypvt[-1].f);
} break;
case 38:
# line 280 "grammar.y"
{
  use_assoc(yypvt[-3].a);
  yyval.f=bdd_forall(bddm, yypvt[-1].f);
  free_assoc(yypvt[-3].a);
  bdd_free(bddm, yypvt[-1].f);
} break;
case 39:
# line 288 "grammar.y"
{
  use_assoc(yypvt[-3].a);
  yyval.f=bdd_substitute(bddm, yypvt[-1].f);
  free_assoc(yypvt[-3].a);
  bdd_free(bddm, yypvt[-1].f);
} break;
case 40:
# line 300 "grammar.y"
{
  yyerror(undefined(yypvt[-0].e->id));
  YYERROR;
} break;
case 41:
# line 306 "grammar.y"
{
  if (yypvt[-0].e->type != VARASSOC)
    {
      yyerror("variable association expected");
      YYERROR;
    }
  yyval.a=copy_assoc(yypvt[-0].e->info.varassoc.a);
} break;
case 42:
# line 316 "grammar.y"
{
  yyval.a=new_assoc(yypvt[-1].v);
} break;
case 43:
# line 325 "grammar.y"
{
  vec_add(yypvt[-1].v, (pointer)yypvt[-0].f);
  vec_add(yypvt[-1].v, (pointer)bdd_one(bddm));
  yyval.v=yypvt[-1].v;
} break;
case 44:
# line 332 "grammar.y"
{
  pointer *p;

  if (yypvt[-0].f)
    {
      vec_add(yypvt[-3].v, (pointer)yypvt[-2].f);
      vec_add(yypvt[-3].v, (pointer)yypvt[-0].f);
      yyval.v=yypvt[-3].v;
    }
  else
    {
      for (p=vec_elements(yypvt[-3].v); *p; ++p)
	bdd_free(bddm, (bdd)*p);
      free_vec(yypvt[-3].v);
      yyerror("node limit exceeded");
      YYERROR;
    }
} break;
case 45:
# line 351 "grammar.y"
{
  yyval.v=new_vec();
} break;
case 46:
# line 360 "grammar.y"
{
  yyerror(undefined(yypvt[-0].e->id));
  YYERROR;
} break;
case 47:
# line 366 "grammar.y"
{
  if (yypvt[-0].e->type != EXPR || bdd_type(bddm, yypvt[-0].e->info.expr.f) != BDD_TYPE_POSVAR)
    {
      yyerror("variable expected");
      YYERROR;
    }
  yyval.f=yypvt[-0].e->info.expr.f;
} break;
case 48:
# line 380 "grammar.y"
{
  hash_table_walk(symbol_table, free_full_entry, (pointer)0);
  free_hash_table(symbol_table);
  hash_table_walk(name_table, free_name, (pointer)0);
  free_hash_table(name_table);
  bdd_quit(bddm);
  bddm=bdd_init();
  init_tables();
} break;
case 49:
# line 391 "grammar.y"
{
  pointer *p;
  st_entry e;

  /* Have to be careful not to nuke something and then reference it. */
  /* Therefore, mark things to be killed first, then really kill them. */
  for (p=vec_elements(yypvt[-1].v); *p; ++p)
    {
      e=(st_entry)*p;
      if (e->type != KILLED)
	{
	  hash_delete(symbol_table, e->id);
	  free_entry(e);
	  e->type=KILLED;
	}
      else
	/* If it's already scheduled for nuking, toss this pointer. */
	/* Don't make it null though, since we use null to detect */
	/* termination below. */
	*p=(pointer)1;
    }
  for (p=vec_elements(yypvt[-1].v); *p; ++p)
    if (*p != (pointer)1)
      {
	e=(st_entry)*p;
	mem_free_block((pointer)e->id);
	mem_free_block((pointer)e);
      }
  free_vec(yypvt[-1].v);
} break;
case 50:
# line 427 "grammar.y"
{
  yyerror(undefined(yypvt[-0].e->id));
  YYERROR;
} break;
case 51:
# line 433 "grammar.y"
{
  if (yypvt[-0].e->type == KEYWORD)
    {
      free_vec(yypvt[-1].v);
      yyerror("identifier expected");
      YYERROR;
    }
  vec_add(yypvt[-1].v, (pointer)yypvt[-0].e);
  yyval.v=yypvt[-1].v;
} break;
case 52:
# line 444 "grammar.y"
{
  yyval.v=new_vec();
} break;
case 53:
# line 453 "grammar.y"
{
  pointer *p;

  switch (vec_len(yypvt[-1].v))
    {
    case 0:
      break;
    case 1:
      bdd_print_function_profile(bddm, (bdd)*vec_elements(yypvt[-1].v), naming_fn, (pointer)0, 80, stdout);
      break;
    default:
      bdd_print_function_profile_multiple(bddm, (bdd *)vec_elements(yypvt[-1].v), naming_fn, (pointer)0, 80, stdout);
      break;
    }
  for (p=vec_elements(yypvt[-1].v); *p; ++p)
    bdd_free(bddm, (bdd)*p);
  free_vec(yypvt[-1].v);
} break;
case 54:
# line 477 "grammar.y"
{
  pointer *p;

  if (yypvt[-0].f)
    {
      vec_add(yypvt[-1].v, (pointer)yypvt[-0].f);
      yyval.v=yypvt[-1].v;
    }
  else
    {
      for (p=vec_elements(yypvt[-1].v); *p; ++p)
	bdd_free(bddm, (bdd)*p);
      free_vec(yypvt[-1].v);
      yyerror("node limit exceeded");
      YYERROR;
    }
} break;
case 55:
# line 495 "grammar.y"
{
  yyval.v=new_vec();
} break;
case 56:
# line 504 "grammar.y"
{
  bdd_gc(bddm);
} break;
case 57:
# line 513 "grammar.y"
{
  bdd_node_limit(bddm, yypvt[-1].i);
} break;
case 58:
# line 518 "grammar.y"
{
  bdd_node_limit(bddm, 0l);
} break;
case 59:
# line 527 "grammar.y"
{
  print_bdd(yypvt[-1].f);
  bdd_free(bddm, yypvt[-1].f);
} break;
case 60:
# line 536 "grammar.y"
{
  print_sop(yypvt[-1].f);
  bdd_free(bddm, yypvt[-1].f);
} break;
case 61:
# line 545 "grammar.y"
{
  pointer *p;

  switch (vec_len(yypvt[-1].v))
    {
    case 0:
      break;
    case 1:
      bdd_print_profile(bddm, (bdd)*vec_elements(yypvt[-1].v), naming_fn, (pointer)0, 80, stdout);
      break;
    default:
      bdd_print_profile_multiple(bddm, (bdd *)vec_elements(yypvt[-1].v), naming_fn, (pointer)0, 80, stdout);
      break;
    }
  for (p=vec_elements(yypvt[-1].v); *p; ++p)
    bdd_free(bddm, (bdd)*p);
  free_vec(yypvt[-1].v);
} break;
case 62:
# line 569 "grammar.y"
{
  bdd_dynamic_reordering(bddm, bdd_reorder_sift);
} break;
case 63:
# line 574 "grammar.y"
{
  bdd_dynamic_reordering(bddm, bdd_reorder_stable_window3);
} break;
case 64:
# line 579 "grammar.y"
{
  bdd_dynamic_reordering(bddm, bdd_reorder_hybrid);
} break;
case 65:
# line 584 "grammar.y"
{
  bdd_dynamic_reordering(bddm, bdd_reorder_none);
} break;
case 66:
# line 589 "grammar.y"
{
  bdd_reorder(bddm);
} break;
case 67:
# line 598 "grammar.y"
{
  bdd f;

  if (yypvt[-1].f == bdd_zero(bddm))
    puts("Expression is identically false");
  else
    {
      f=bdd_satisfy(bddm, yypvt[-1].f);
      print_sop(f);
      bdd_free(bddm, f);
      bdd_free(bddm, yypvt[-1].f);
    }
} break;
case 68:
# line 617 "grammar.y"
{
  bdd f;

  if (yypvt[-1].f == bdd_zero(bddm))
    puts("Expression is identically false");
  else
    {
      use_assoc(yypvt[-2].a);
      f=bdd_satisfy_support(bddm, yypvt[-1].f);
      print_sop(f);
      bdd_free(bddm, f);
      bdd_free(bddm, yypvt[-1].f);
    }
  free_assoc(yypvt[-2].a);
} break;
case 69:
# line 638 "grammar.y"
{
  printf("%g%% of the possible valuations\n", 100*bdd_satisfying_fraction(bddm, yypvt[-1].f));
  bdd_free(bddm, yypvt[-1].f);
} break;
case 70:
# line 648 "grammar.y"
{
  pointer *p;

  switch (vec_len(yypvt[-1].v))
    {
    case 0:
      break;
    case 1:
      printf("%ld nodes (%ld without output negation)\n",
	     bdd_size(bddm, (bdd)*vec_elements(yypvt[-1].v), 1),
	     bdd_size(bddm, (bdd)*vec_elements(yypvt[-1].v), 0));
      break;
    default:
      printf("%ld nodes (%ld without output negation)\n",
	     bdd_size_multiple(bddm, (bdd *)vec_elements(yypvt[-1].v), 1),
	     bdd_size_multiple(bddm, (bdd *)vec_elements(yypvt[-1].v), 0));
      break;
    }
  for (p=vec_elements(yypvt[-1].v); *p; ++p)
    bdd_free(bddm, (bdd)*p);
  free_vec(yypvt[-1].v);
} break;
case 71:
# line 676 "grammar.y"
{
  bdd_stats(bddm, stdout);
} break;
case 72:
# line 685 "grammar.y"
{
  double user_time, system_time;

  if (!timer_running)
    puts("Timer is already off");
  else
    {
      get_cpu_time(&user_time, &system_time);
      user_time-=user_start;
      system_time-=system_start;
      printf("%g user, %g system since timer started\n", user_time, system_time);
      printf("%g user, %g system since last timer command\n", user_time-user_last, system_time-system_last);
      timer_running=0;
    }
} break;
case 73:
# line 702 "grammar.y"
{
  double user_time, system_time;

  get_cpu_time(&user_time, &system_time);
  if (!timer_running)
    {
      user_start=user_time;
      system_start=system_time;
      user_last=0.0;
      system_last=0.0;
      timer_running=1;
      puts("Timer started");
    }
  else
    {
      user_time-=user_start;
      system_time-=system_start;
      printf("%g user, %g system since timer started\n", user_time, system_time);
      printf("%g user, %g system since last timer command\n", user_time-user_last, system_time-system_last);
      user_last=user_time;
      system_last=system_time;
    }
} break;
case 74:
# line 730 "grammar.y"
{
} break;
case 75:
# line 738 "grammar.y"
{
  if ((yypvt[-0].e->info.expr.f=bdd_new_var_last(bddm)))
    {
      yypvt[-0].e->type=EXPR;
      hash_insert(name_table, (pointer)yypvt[-0].e->info.expr.f, copy_str(yypvt[-0].e->id));
    }
  else
    {
      yypvt[-0].e->type=NEW;
      yyerror("node limit exceeded");
      YYERROR;
    }
} break;
case 76:
# line 752 "grammar.y"
{
} break;
case 77:
# line 760 "grammar.y"
{
  yyerrok;
} break;
	}
	goto yystack;		/* reset registers in driver code */
}
