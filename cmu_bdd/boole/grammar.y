%{
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
%}


%union
{
  st_entry e;
  bdd f;
  assoc a;
  vec v;
  long i;
}


%token UNKNOWNCHAR
%token <e> ID NEWID
%token <i> NUM
%token CLEAR ALL
%token FUNCPROFILE
%token GC
%token LIMIT
%token PRINT
%token PRINTSOP
%token PROFILE
%token REORDER SIFT WINDOW HYBRID NONE
%token SATISFY
%token SATISFYSUPPORT
%token SATISFYFRACTION
%token SIZE
%token STATS
%token TIMER OFF
%token VARS
%token AGETS
%token GETS
%token EXISTS
%token FORALL
%token SUBST
%nonassoc '?'
%nonassoc ':'
%nonassoc '['
%left '^'
%left '|'
%left '&'
%nonassoc '='
%nonassoc '!'


%type <e> assignatom
%type <f> expr
%type <f> expraux
%type <a> assoc
%type <v> exprs
%type <f> var
%type <v> pairs
%type <v> ids


%start file


%%


file
:
file command
{
  first_line=1;
}
|
;


command
:
assignment
|
clear
|
funcprofile
|
gc
|
limit
|
print
|
printsop
|
profile
|
reorder
|
satisfy
|
satisfysupport
|
satisfyfraction
|
size
|
stats
|
timer
|
vars
|
errorcmd
;


assignment
:
assignatom GETS expr ';'
{
  $1->type=EXPR;
  $1->info.expr.f=$3;
}
|
assignatom AGETS assoc ';'
{
  $1->type=VARASSOC;
  $1->info.varassoc.a=$3;
}
;


assignatom
:
NEWID
{
  $$=$1;
}
|
ID
{
  if ($1->type == KEYWORD)
    {
      yyerror("identifier expected");
      YYERROR;
    }
  free_entry($1);
  $1->type=NEW;
  $$=$1;
}


expr
:
expraux
{
  if ($1)
    $$=$1;
  else
    {
      yyerror("node limit exceeded");
      YYERROR;
    }
}


expraux
:
NEWID
{
  yyerror(undefined($1->id));
  YYERROR;
}
|
ID
{
  if ($1->type != EXPR)
    {
      yyerror("expression expected");
      YYERROR;
    }
  $$=bdd_identity(bddm, $1->info.expr.f);
}
|
'0'
{
  $$=bdd_zero(bddm);
}
|
'1'
{
  $$=bdd_one(bddm);
}
|
'(' expraux ')'
{
  $$=$2;
}
|
expraux '[' var GETS expraux ']'
{
  $$=bdd_compose(bddm, $1, $3, $5);
  bdd_free(bddm, $1);
  bdd_free(bddm, $5);
}
|
expraux '?' expraux ':' expraux
{
  $$=bdd_ite(bddm, $1, $3, $5);
  bdd_free(bddm, $1);
  bdd_free(bddm, $3);
  bdd_free(bddm, $5);
}
|
expraux '&' expraux
{
  $$=bdd_and(bddm, $1, $3);
  bdd_free(bddm, $1);
  bdd_free(bddm, $3);
}
|
expraux '|' expraux
{
  $$=bdd_or(bddm, $1, $3);
  bdd_free(bddm, $1);
  bdd_free(bddm, $3);
}
|
expraux '^' expraux
{
  $$=bdd_xor(bddm, $1, $3);
  bdd_free(bddm, $1);
  bdd_free(bddm, $3);
}
|
expraux '=' expraux
{
  $$=bdd_xnor(bddm, $1, $3);
  bdd_free(bddm, $1);
  bdd_free(bddm, $3);
}
|
'!' expraux
{
  $$=bdd_not(bddm, $2);
  bdd_free(bddm, $2);
}
|
EXISTS assoc '(' expraux ')'
{
  use_assoc($2);
  $$=bdd_exists(bddm, $4);
  free_assoc($2);
  bdd_free(bddm, $4);
}
|
FORALL assoc '(' expraux ')'
{
  use_assoc($2);
  $$=bdd_forall(bddm, $4);
  free_assoc($2);
  bdd_free(bddm, $4);
}
|
SUBST assoc '(' expraux ')'
{
  use_assoc($2);
  $$=bdd_substitute(bddm, $4);
  free_assoc($2);
  bdd_free(bddm, $4);
}
;


assoc
:
NEWID
{
  yyerror(undefined($1->id));
  YYERROR;
}
|
ID
{
  if ($1->type != VARASSOC)
    {
      yyerror("variable association expected");
      YYERROR;
    }
  $$=copy_assoc($1->info.varassoc.a);
}
|
'[' pairs ']'
{
  $$=new_assoc($2);
}
;


pairs
:
pairs var
{
  vec_add($1, (pointer)$2);
  vec_add($1, (pointer)bdd_one(bddm));
  $$=$1;
}
|
pairs var GETS expraux
{
  pointer *p;

  if ($4)
    {
      vec_add($1, (pointer)$2);
      vec_add($1, (pointer)$4);
      $$=$1;
    }
  else
    {
      for (p=vec_elements($1); *p; ++p)
	bdd_free(bddm, (bdd)*p);
      free_vec($1);
      yyerror("node limit exceeded");
      YYERROR;
    }
}
|
{
  $$=new_vec();
}
;


var
:
NEWID
{
  yyerror(undefined($1->id));
  YYERROR;
}
|
ID
{
  if ($1->type != EXPR || bdd_type(bddm, $1->info.expr.f) != BDD_TYPE_POSVAR)
    {
      yyerror("variable expected");
      YYERROR;
    }
  $$=$1->info.expr.f;
}
;


clear
:
CLEAR ALL ';'
{
  hash_table_walk(symbol_table, free_full_entry, (pointer)0);
  free_hash_table(symbol_table);
  hash_table_walk(name_table, free_name, (pointer)0);
  free_hash_table(name_table);
  bdd_quit(bddm);
  bddm=bdd_init();
  init_tables();
}
|
CLEAR ids ';'
{
  pointer *p;
  st_entry e;

  /* Have to be careful not to nuke something and then reference it. */
  /* Therefore, mark things to be killed first, then really kill them. */
  for (p=vec_elements($2); *p; ++p)
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
  for (p=vec_elements($2); *p; ++p)
    if (*p != (pointer)1)
      {
	e=(st_entry)*p;
	mem_free_block((pointer)e->id);
	mem_free_block((pointer)e);
      }
  free_vec($2);
}
;


ids
:
NEWID
{
  yyerror(undefined($1->id));
  YYERROR;
}
|
ids ID
{
  if ($2->type == KEYWORD)
    {
      free_vec($1);
      yyerror("identifier expected");
      YYERROR;
    }
  vec_add($1, (pointer)$2);
  $$=$1;
}
|
{
  $$=new_vec();
}
;


funcprofile
:
FUNCPROFILE exprs ';'
{
  pointer *p;

  switch (vec_len($2))
    {
    case 0:
      break;
    case 1:
      bdd_print_function_profile(bddm, (bdd)*vec_elements($2), naming_fn, (pointer)0, 80, stdout);
      break;
    default:
      bdd_print_function_profile_multiple(bddm, (bdd *)vec_elements($2), naming_fn, (pointer)0, 80, stdout);
      break;
    }
  for (p=vec_elements($2); *p; ++p)
    bdd_free(bddm, (bdd)*p);
  free_vec($2);
}
;


exprs
:
exprs expraux
{
  pointer *p;

  if ($2)
    {
      vec_add($1, (pointer)$2);
      $$=$1;
    }
  else
    {
      for (p=vec_elements($1); *p; ++p)
	bdd_free(bddm, (bdd)*p);
      free_vec($1);
      yyerror("node limit exceeded");
      YYERROR;
    }
}
|
{
  $$=new_vec();
}
;


gc
:
GC ';'
{
  bdd_gc(bddm);
}
;


limit
:
LIMIT NUM ';'
{
  bdd_node_limit(bddm, $2);
}
|
LIMIT NONE ';'
{
  bdd_node_limit(bddm, 0l);
}
;


print
:
PRINT expr ';'
{
  print_bdd($2);
  bdd_free(bddm, $2);
}


printsop
:
PRINTSOP expr ';'
{
  print_sop($2);
  bdd_free(bddm, $2);
}


profile
:
PROFILE exprs ';'
{
  pointer *p;

  switch (vec_len($2))
    {
    case 0:
      break;
    case 1:
      bdd_print_profile(bddm, (bdd)*vec_elements($2), naming_fn, (pointer)0, 80, stdout);
      break;
    default:
      bdd_print_profile_multiple(bddm, (bdd *)vec_elements($2), naming_fn, (pointer)0, 80, stdout);
      break;
    }
  for (p=vec_elements($2); *p; ++p)
    bdd_free(bddm, (bdd)*p);
  free_vec($2);
}
;


reorder
:
REORDER SIFT ';'
{
  bdd_dynamic_reordering(bddm, bdd_reorder_sift);
}
|
REORDER WINDOW ';'
{
  bdd_dynamic_reordering(bddm, bdd_reorder_stable_window3);
}
|
REORDER HYBRID ';'
{
  bdd_dynamic_reordering(bddm, bdd_reorder_hybrid);
}
|
REORDER NONE ';'
{
  bdd_dynamic_reordering(bddm, bdd_reorder_none);
}
|
REORDER ';'
{
  bdd_reorder(bddm);
}
;


satisfy
:
SATISFY expr ';'
{
  bdd f;

  if ($2 == bdd_zero(bddm))
    puts("Expression is identically false");
  else
    {
      f=bdd_satisfy(bddm, $2);
      print_sop(f);
      bdd_free(bddm, f);
      bdd_free(bddm, $2);
    }
}
;


satisfysupport
:
SATISFYSUPPORT assoc expr ';'
{
  bdd f;

  if ($3 == bdd_zero(bddm))
    puts("Expression is identically false");
  else
    {
      use_assoc($2);
      f=bdd_satisfy_support(bddm, $3);
      print_sop(f);
      bdd_free(bddm, f);
      bdd_free(bddm, $3);
    }
  free_assoc($2);
}
;


satisfyfraction
:
SATISFYFRACTION expr ';'
{
  printf("%g%% of the possible valuations\n", 100*bdd_satisfying_fraction(bddm, $2));
  bdd_free(bddm, $2);
}
;


size
:
SIZE exprs ';'
{
  pointer *p;

  switch (vec_len($2))
    {
    case 0:
      break;
    case 1:
      printf("%ld nodes (%ld without output negation)\n",
	     bdd_size(bddm, (bdd)*vec_elements($2), 1),
	     bdd_size(bddm, (bdd)*vec_elements($2), 0));
      break;
    default:
      printf("%ld nodes (%ld without output negation)\n",
	     bdd_size_multiple(bddm, (bdd *)vec_elements($2), 1),
	     bdd_size_multiple(bddm, (bdd *)vec_elements($2), 0));
      break;
    }
  for (p=vec_elements($2); *p; ++p)
    bdd_free(bddm, (bdd)*p);
  free_vec($2);
}
;


stats
:
STATS ';'
{
  bdd_stats(bddm, stdout);
}
;


timer
:
TIMER OFF ';'
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
}
|
TIMER ';'
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
}
;

vars
:
VARS vardecls ';'
{
}
;


vardecls
:
vardecls assignatom
{
  if (($2->info.expr.f=bdd_new_var_last(bddm)))
    {
      $2->type=EXPR;
      hash_insert(name_table, (pointer)$2->info.expr.f, copy_str($2->id));
    }
  else
    {
      $2->type=NEW;
      yyerror("node limit exceeded");
      YYERROR;
    }
}
|
{
}
;


errorcmd
:
error ';'
{
  yyerrok;
}
;

  
%%
