# include <stdio.h>
# define U(x) x
# define NLSTATE yyprevious=YYNEWLINE
# define BEGIN yybgin = yysvec + 1 +
# define INITIAL 0
# define YYLERR yysvec
# define YYSTATE (yyestate-yysvec-1)
# define YYOPTIM 1
# define YYLMAX BUFSIZ
# define output(c) putc(c,yyout)
# define input() (((yytchar=yysptr>yysbuf?U(*--yysptr):getc(yyin))==10?(yylineno++,yytchar):yytchar)==EOF?0:yytchar)
# define unput(c) {yytchar= (c);if(yytchar=='\n')yylineno--;*yysptr++=yytchar;}
# define yymore() (yymorfg=1)
# define ECHO fprintf(yyout, "%s",yytext)
# define REJECT { nstr = yyreject(); goto yyfussy;}
int yyleng; extern char yytext[];
int yymorfg;
extern char *yysptr, yysbuf[];
int yytchar;
extern FILE *yyin, *yyout;
extern int yylineno;
struct yysvf { 
	struct yywork *yystoff;
	struct yysvf *yyother;
	int *yystops;};
struct yysvf *yyestate;
extern struct yysvf yysvec[], *yybgin;
#include "boole.h"
#include "y.tab.h"
# define YYNEWLINE 10
yylex(){
int nstr; extern int yyprevious;
while((nstr = yylook()) >= 0)
yyfussy: switch(nstr){
case 0:
if(yywrap()) return(0); break;
case 1:
{
  ;
}
break;
case 2:
{
  ++line_number;
  prompt();
}
break;
case 3:
{
  ++line_number;
  prompt();
}
break;
case 4:
{
  return (';');
}
break;
case 5:
{
  return ('(');
}
break;
case 6:
{
  return (')');
}
break;
case 7:
{
  return (GETS);
}
break;
case 8:
{
  return (AGETS);
}
break;
case 9:
{
  return ('=');
}
break;
case 10:
{
  return ('&');
}
break;
case 11:
{
  return ('&');
}
break;
case 12:
{
  return ('|');
}
break;
case 13:
{
  return ('|');
}
break;
case 14:
{
  return ('^');
}
break;
case 15:
{
  return ('!');
}
break;
case 16:
{
  return ('!');
}
break;
case 17:
{
  return ('[');
}
break;
case 18:
{
  return (']');
}
break;
case 19:
{
  return ('0');
}
break;
case 20:
{
  return ('1');
}
break;
case 21:
{
  return ('?');
}
break;
case 22:
{
  return (':');
}
break;
case 23:
{
  sscanf(yytext, "%ld", &yylval.i);
  return (NUM);
}
break;
case 24:
{
  st_entry entry;

  entry=(st_entry)hash_lookup(symbol_table, (pointer)yytext);
  if (!entry)
    {
      entry=(st_entry)mem_get_block((long)sizeof(struct st_entry_));
      entry->type=NEW;
      entry->id=copy_str(yytext);
      hash_insert(symbol_table, (pointer)entry->id, (pointer)entry);
      yylval.e=entry;
      return (NEWID);
    }
  if (entry->type == KEYWORD)
    return (entry->info.keyword.code);
  yylval.e=entry;
  if (entry->type == NEW)
    /* Entry exists, but NEW tag means we're supposed to pretend it */
    /* doesn't. */
    return (NEWID);
  return (ID);
}
break;
case 25:
{
  return (UNKNOWNCHAR);
}
break;
case -1:
break;
default:
fprintf(yyout,"bad switch yylook %d",nstr);
} return(0); }
/* end of yylex */
int yyvstop[] = {
0,

24,
0,

24,
0,

25,
0,

1,
25,
0,

2,
0,

15,
25,
0,

24,
25,
0,

10,
25,
0,

5,
25,
0,

6,
25,
0,

11,
25,
0,

13,
25,
0,

24,
25,
0,

19,
23,
24,
25,
0,

20,
23,
24,
25,
0,

23,
24,
25,
0,

22,
25,
0,

4,
25,
0,

9,
25,
0,

21,
25,
0,

17,
25,
0,

18,
25,
0,

14,
25,
0,

12,
25,
0,

16,
25,
0,

24,
0,

24,
0,

23,
24,
0,

7,
0,

8,
0,

3,
0,
0};
# define YYTYPE char
struct yywork { YYTYPE verify, advance; } yycrank[] = {
0,0,	0,0,	1,3,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	1,4,	1,5,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	1,6,	0,0,	
1,7,	0,0,	0,0,	1,8,	
0,0,	1,9,	1,10,	1,11,	
1,12,	0,0,	1,13,	0,0,	
0,0,	1,14,	1,15,	1,16,	
1,16,	1,16,	1,16,	1,16,	
1,16,	1,16,	1,16,	1,17,	
1,18,	2,6,	1,19,	17,29,	
1,20,	21,30,	2,8,	27,31,	
2,9,	2,10,	2,11,	2,12,	
0,0,	2,13,	0,0,	27,31,	
27,32,	2,15,	2,16,	2,16,	
2,16,	2,16,	2,16,	2,16,	
2,16,	2,16,	2,17,	2,18,	
0,0,	2,19,	31,31,	2,20,	
1,21,	0,0,	1,22,	1,23,	
0,0,	0,0,	0,0,	0,0,	
0,0,	27,27,	0,0,	31,31,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	27,27,	0,0,	
0,0,	0,0,	0,0,	2,21,	
0,0,	2,22,	2,23,	0,0,	
0,0,	1,24,	0,0,	1,25,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
2,24,	0,0,	2,25,	7,0,	
7,0,	7,0,	7,0,	7,0,	
7,0,	7,0,	7,0,	7,0,	
7,0,	7,0,	7,0,	7,0,	
7,0,	7,0,	7,0,	7,0,	
7,0,	7,0,	7,0,	7,0,	
7,0,	7,0,	7,0,	7,0,	
7,0,	7,0,	7,0,	7,0,	
7,0,	7,0,	7,0,	7,0,	
7,0,	7,26,	0,0,	0,0,	
7,0,	7,0,	7,0,	7,0,	
7,0,	7,0,	7,0,	0,0,	
0,0,	0,0,	7,26,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
7,0,	7,0,	0,0,	7,0,	
0,0,	7,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	7,0,	7,0,	7,0,	
7,0,	0,0,	7,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	7,0,	7,0,	7,0,	
7,0,	7,0,	13,0,	13,0,	
13,0,	13,0,	13,0,	13,0,	
13,0,	13,0,	13,0,	13,0,	
13,0,	13,0,	13,0,	13,0,	
13,0,	13,0,	13,0,	13,0,	
13,0,	13,0,	13,0,	13,0,	
13,0,	13,0,	13,0,	13,0,	
13,0,	13,0,	13,0,	13,0,	
13,0,	13,0,	13,0,	13,0,	
0,0,	0,0,	0,0,	13,0,	
13,0,	13,0,	13,0,	13,0,	
13,0,	13,0,	13,27,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	13,0,	
13,0,	0,0,	13,0,	0,0,	
13,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
13,0,	13,0,	13,0,	13,0,	
0,0,	13,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
13,0,	13,0,	13,0,	13,0,	
13,0,	14,0,	14,0,	14,0,	
14,0,	14,0,	14,0,	14,0,	
14,0,	14,0,	14,0,	14,0,	
14,0,	14,0,	14,0,	14,0,	
14,0,	14,0,	14,0,	14,0,	
14,0,	14,0,	14,0,	14,0,	
14,0,	14,0,	14,0,	14,0,	
14,0,	14,0,	14,0,	14,0,	
14,0,	14,0,	14,0,	0,0,	
0,0,	0,0,	14,0,	14,0,	
14,0,	14,0,	14,0,	14,0,	
14,0,	0,0,	0,0,	0,0,	
14,28,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	14,0,	14,0,	
0,0,	14,0,	0,0,	14,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	14,0,	
14,0,	14,0,	14,0,	0,0,	
14,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	14,0,	
14,0,	14,0,	14,0,	14,0,	
15,0,	15,0,	15,0,	15,0,	
15,0,	15,0,	15,0,	15,0,	
15,0,	15,0,	15,0,	15,0,	
15,0,	15,0,	15,0,	15,0,	
15,0,	15,0,	15,0,	15,0,	
15,0,	15,0,	15,0,	15,0,	
15,0,	15,0,	15,0,	15,0,	
15,0,	15,0,	15,0,	15,0,	
15,0,	15,0,	0,0,	0,0,	
0,0,	15,0,	15,0,	15,0,	
15,0,	15,0,	15,0,	15,0,	
0,0,	0,0,	0,0,	15,28,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	15,0,	15,0,	0,0,	
15,0,	0,0,	15,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	15,0,	15,0,	
15,0,	15,0,	0,0,	15,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	15,0,	15,0,	
15,0,	15,0,	15,0,	16,0,	
16,0,	16,0,	16,0,	16,0,	
16,0,	16,0,	16,0,	16,0,	
16,0,	16,0,	16,0,	16,0,	
16,0,	16,0,	16,0,	16,0,	
16,0,	16,0,	16,0,	16,0,	
16,0,	16,0,	16,0,	16,0,	
16,0,	16,0,	16,0,	16,0,	
16,0,	16,0,	16,0,	16,0,	
16,0,	0,0,	0,0,	0,0,	
16,0,	16,0,	16,0,	16,0,	
16,0,	16,0,	16,0,	0,0,	
0,0,	0,0,	16,28,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
16,0,	16,0,	0,0,	16,0,	
0,0,	16,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	16,0,	16,0,	16,0,	
16,0,	0,0,	16,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	16,0,	16,0,	16,0,	
16,0,	16,0,	26,0,	26,0,	
26,0,	26,0,	26,0,	26,0,	
26,0,	26,0,	26,0,	26,0,	
26,0,	26,0,	26,0,	26,0,	
26,0,	26,0,	26,0,	26,0,	
26,0,	26,0,	26,0,	26,0,	
26,0,	26,0,	26,0,	26,0,	
26,0,	26,0,	26,0,	26,0,	
26,0,	26,0,	26,0,	26,0,	
0,0,	0,0,	0,0,	26,0,	
26,0,	26,0,	26,0,	26,0,	
26,0,	26,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	26,0,	
26,0,	0,0,	26,0,	0,0,	
26,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
26,0,	26,0,	26,0,	26,0,	
0,0,	26,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
26,0,	26,0,	26,0,	26,0,	
26,0,	28,0,	28,0,	28,0,	
28,0,	28,0,	28,0,	28,0,	
28,0,	28,0,	28,0,	28,0,	
28,0,	28,0,	28,0,	28,0,	
28,0,	28,0,	28,0,	28,0,	
28,0,	28,0,	28,0,	28,0,	
28,0,	28,0,	28,0,	28,0,	
28,0,	28,0,	28,0,	28,0,	
28,0,	28,0,	28,0,	0,0,	
0,0,	0,0,	28,0,	28,0,	
28,0,	28,0,	28,0,	28,0,	
28,0,	0,0,	0,0,	0,0,	
28,28,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	28,0,	28,0,	
0,0,	28,0,	0,0,	28,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	28,0,	
28,0,	28,0,	28,0,	0,0,	
28,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	28,0,	
28,0,	28,0,	28,0,	28,0,	
0,0};
struct yysvf yysvec[] = {
0,	0,	0,
yycrank+-1,	0,		yyvstop+1,
yycrank+-28,	yysvec+1,	yyvstop+3,
yycrank+0,	0,		yyvstop+5,
yycrank+0,	0,		yyvstop+7,
yycrank+0,	0,		yyvstop+10,
yycrank+0,	0,		yyvstop+12,
yycrank+-154,	0,		yyvstop+15,
yycrank+0,	0,		yyvstop+18,
yycrank+0,	0,		yyvstop+21,
yycrank+0,	0,		yyvstop+24,
yycrank+0,	0,		yyvstop+27,
yycrank+0,	0,		yyvstop+30,
yycrank+-281,	yysvec+7,	yyvstop+33,
yycrank+-408,	yysvec+7,	yyvstop+36,
yycrank+-535,	yysvec+7,	yyvstop+41,
yycrank+-662,	yysvec+7,	yyvstop+46,
yycrank+2,	0,		yyvstop+50,
yycrank+0,	0,		yyvstop+53,
yycrank+0,	0,		yyvstop+56,
yycrank+0,	0,		yyvstop+59,
yycrank+4,	0,		yyvstop+62,
yycrank+0,	0,		yyvstop+65,
yycrank+0,	0,		yyvstop+68,
yycrank+0,	0,		yyvstop+71,
yycrank+0,	0,		yyvstop+74,
yycrank+-789,	yysvec+7,	yyvstop+77,
yycrank+-66,	0,		yyvstop+79,
yycrank+-916,	yysvec+7,	yyvstop+81,
yycrank+0,	0,		yyvstop+84,
yycrank+0,	0,		yyvstop+86,
yycrank+-55,	yysvec+27,	0,	
yycrank+0,	0,		yyvstop+88,
0,	0,	0};
struct yywork *yytop = yycrank+1043;
struct yysvf *yybgin = yysvec+1;
char yymatch[] = {
00  ,01  ,01  ,01  ,01  ,01  ,01  ,01  ,
01  ,011 ,012 ,01  ,011 ,01  ,01  ,01  ,
01  ,01  ,01  ,01  ,01  ,01  ,01  ,01  ,
01  ,01  ,01  ,01  ,01  ,01  ,01  ,01  ,
011 ,01  ,01  ,'#' ,'#' ,'#' ,01  ,01  ,
01  ,01  ,01  ,01  ,01  ,'#' ,'#' ,'#' ,
'0' ,'0' ,'0' ,'0' ,'0' ,'0' ,'0' ,'0' ,
'0' ,'0' ,01  ,01  ,'#' ,01  ,'#' ,01  ,
'#' ,'#' ,'#' ,'#' ,'#' ,'#' ,'#' ,'#' ,
'#' ,'#' ,'#' ,'#' ,'#' ,'#' ,'#' ,'#' ,
'#' ,'#' ,'#' ,'#' ,'#' ,'#' ,'#' ,'#' ,
'#' ,'#' ,'#' ,01  ,01  ,01  ,01  ,'#' ,
01  ,'#' ,'#' ,'#' ,'#' ,'#' ,'#' ,'#' ,
'#' ,'#' ,'#' ,'#' ,'#' ,'#' ,'#' ,'#' ,
'#' ,'#' ,'#' ,'#' ,'#' ,'#' ,'#' ,'#' ,
'#' ,'#' ,'#' ,01  ,01  ,01  ,01  ,01  ,
0};
char yyextra[] = {
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0};
#ifndef lint
static	char ncform_sccsid[] = "@(#)ncform 1.6 88/02/08 SMI"; /* from S5R2 1.2 */
#endif

int yylineno =1;
# define YYU(x) x
# define NLSTATE yyprevious=YYNEWLINE
char yytext[YYLMAX];
struct yysvf *yylstate [YYLMAX], **yylsp, **yyolsp;
char yysbuf[YYLMAX];
char *yysptr = yysbuf;
int *yyfnd;
extern struct yysvf *yyestate;
int yyprevious = YYNEWLINE;
yylook(){
	register struct yysvf *yystate, **lsp;
	register struct yywork *yyt;
	struct yysvf *yyz;
	int yych, yyfirst;
	struct yywork *yyr;
# ifdef LEXDEBUG
	int debug;
# endif
	char *yylastch;
	/* start off machines */
# ifdef LEXDEBUG
	debug = 0;
# endif
	yyfirst=1;
	if (!yymorfg)
		yylastch = yytext;
	else {
		yymorfg=0;
		yylastch = yytext+yyleng;
		}
	for(;;){
		lsp = yylstate;
		yyestate = yystate = yybgin;
		if (yyprevious==YYNEWLINE) yystate++;
		for (;;){
# ifdef LEXDEBUG
			if(debug)fprintf(yyout,"state %d\n",yystate-yysvec-1);
# endif
			yyt = yystate->yystoff;
			if(yyt == yycrank && !yyfirst){  /* may not be any transitions */
				yyz = yystate->yyother;
				if(yyz == 0)break;
				if(yyz->yystoff == yycrank)break;
				}
			*yylastch++ = yych = input();
			yyfirst=0;
		tryagain:
# ifdef LEXDEBUG
			if(debug){
				fprintf(yyout,"char ");
				allprint(yych);
				putchar('\n');
				}
# endif
			yyr = yyt;
			if ( (int)yyt > (int)yycrank){
				yyt = yyr + yych;
				if (yyt <= yytop && yyt->verify+yysvec == yystate){
					if(yyt->advance+yysvec == YYLERR)	/* error transitions */
						{unput(*--yylastch);break;}
					*lsp++ = yystate = yyt->advance+yysvec;
					goto contin;
					}
				}
# ifdef YYOPTIM
			else if((int)yyt < (int)yycrank) {		/* r < yycrank */
				yyt = yyr = yycrank+(yycrank-yyt);
# ifdef LEXDEBUG
				if(debug)fprintf(yyout,"compressed state\n");
# endif
				yyt = yyt + yych;
				if(yyt <= yytop && yyt->verify+yysvec == yystate){
					if(yyt->advance+yysvec == YYLERR)	/* error transitions */
						{unput(*--yylastch);break;}
					*lsp++ = yystate = yyt->advance+yysvec;
					goto contin;
					}
				yyt = yyr + YYU(yymatch[yych]);
# ifdef LEXDEBUG
				if(debug){
					fprintf(yyout,"try fall back character ");
					allprint(YYU(yymatch[yych]));
					putchar('\n');
					}
# endif
				if(yyt <= yytop && yyt->verify+yysvec == yystate){
					if(yyt->advance+yysvec == YYLERR)	/* error transition */
						{unput(*--yylastch);break;}
					*lsp++ = yystate = yyt->advance+yysvec;
					goto contin;
					}
				}
			if ((yystate = yystate->yyother) && (yyt= yystate->yystoff) != yycrank){
# ifdef LEXDEBUG
				if(debug)fprintf(yyout,"fall back to state %d\n",yystate-yysvec-1);
# endif
				goto tryagain;
				}
# endif
			else
				{unput(*--yylastch);break;}
		contin:
# ifdef LEXDEBUG
			if(debug){
				fprintf(yyout,"state %d char ",yystate-yysvec-1);
				allprint(yych);
				putchar('\n');
				}
# endif
			;
			}
# ifdef LEXDEBUG
		if(debug){
			fprintf(yyout,"stopped at %d with ",*(lsp-1)-yysvec-1);
			allprint(yych);
			putchar('\n');
			}
# endif
		while (lsp-- > yylstate){
			*yylastch-- = 0;
			if (*lsp != 0 && (yyfnd= (*lsp)->yystops) && *yyfnd > 0){
				yyolsp = lsp;
				if(yyextra[*yyfnd]){		/* must backup */
					while(yyback((*lsp)->yystops,-*yyfnd) != 1 && lsp > yylstate){
						lsp--;
						unput(*yylastch--);
						}
					}
				yyprevious = YYU(*yylastch);
				yylsp = lsp;
				yyleng = yylastch-yytext+1;
				yytext[yyleng] = 0;
# ifdef LEXDEBUG
				if(debug){
					fprintf(yyout,"\nmatch ");
					sprint(yytext);
					fprintf(yyout," action %d\n",*yyfnd);
					}
# endif
				return(*yyfnd++);
				}
			unput(*yylastch);
			}
		if (yytext[0] == 0  /* && feof(yyin) */)
			{
			yysptr=yysbuf;
			return(0);
			}
		yyprevious = yytext[0] = input();
		if (yyprevious>0)
			output(yyprevious);
		yylastch=yytext;
# ifdef LEXDEBUG
		if(debug)putchar('\n');
# endif
		}
	}
yyback(p, m)
	int *p;
{
if (p==0) return(0);
while (*p)
	{
	if (*p++ == m)
		return(1);
	}
return(0);
}
	/* the following are only used in the lex library */
yyinput(){
	return(input());
	}
yyoutput(c)
  int c; {
	output(c);
	}
yyunput(c)
   int c; {
	unput(c);
	}
