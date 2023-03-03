#include "boole.h"
#include "y.tab.h"
#include <string.h>
#include <sys/time.h>
#include <sys/resource.h>


#if defined(__STDC__)
extern int getrusage(int, struct rusage *);
#else
extern int getrusage();
#endif


int interactive;
int first_line;
int line_number;
hash_table symbol_table;
hash_table name_table;
bdd_manager bddm;
int timer_running;
double user_start;
double system_start;
double user_last;
double system_last;


char *
#if defined(__STDC__)
copy_str(char *s)
#else
copy_str(s)
     char *s;
#endif
{
  return (strcpy((char *)mem_get_block((SIZE_T)(strlen(s)+1)), s));
}


char *
#if defined(__STDC__)
undefined(char *id)
#else
undefined(id)
     char *id;
#endif
{
  static char buff[1000];

  sprintf(buff, "`%s' undefined", id);
  return (buff);
}


static
void
#if defined(__STDC__)
make_keyword(char *word, int code)
#else
make_keyword(word, code)
     char *word;
     int code;
#endif
{
  st_entry e;

  e=(st_entry)mem_get_block((SIZE_T)sizeof(struct st_entry_));
  e->type=KEYWORD;
  e->id=copy_str(word);
  e->info.keyword.code=code;
  hash_insert(symbol_table, (pointer)word, (pointer)e);
}


void
#if defined(__STDC__)
init_tables(void)
#else
init_tables()
#endif
{
  name_table=new_pointer_hash_table();
  symbol_table=new_hash_table();
  make_keyword("clear", CLEAR);
  make_keyword("all", ALL);
  make_keyword("funcprof", FUNCPROFILE);
  make_keyword("gc", GC);
  make_keyword("limit", LIMIT);
  make_keyword("print", PRINT);
  make_keyword("profile", PROFILE);
  make_keyword("reorder", REORDER);
  make_keyword("sift", SIFT);
  make_keyword("window", WINDOW);
  make_keyword("hybrid", HYBRID);
  make_keyword("none", NONE);
  make_keyword("satisfy", SATISFY);
  make_keyword("satsupport", SATISFYSUPPORT);
  make_keyword("satfrac", SATISFYFRACTION);
  make_keyword("size", SIZE);
  make_keyword("sop", PRINTSOP);
  make_keyword("stats", STATS);
  make_keyword("timer", TIMER);
  make_keyword("off", OFF);
  make_keyword("vars", VARS);
  make_keyword("exists", EXISTS);
  make_keyword("forall", FORALL);
  make_keyword("subst", SUBST);
}


void
#if defined(__STDC__)
free_entry(st_entry e)
#else
free_entry(e)
     st_entry e;
#endif
{
  switch (e->type)
    {
    case EXPR:
      bdd_free(bddm, e->info.expr.f);
      break;
    case VARASSOC:
      free_assoc(e->info.varassoc.a);
      break;
    default:
      fputs("free_entry: unexpected entry type\n", stderr);
      exit(1);
    }
}


void
#if defined(__STDC__)
free_full_entry(pointer junk1, pointer e_ptr, pointer junk2)
#else
free_full_entry(junk1, e_ptr, junk2)
     pointer junk1;
     pointer e_ptr;
     pointer junk2;
#endif
{
  st_entry e;

  e=(st_entry)e_ptr;
  if (e->type == EXPR || e->type == VARASSOC)
    free_entry(e);
  mem_free_block((pointer)e->id);
  mem_free_block((pointer)e);
}


char *
#if defined(__STDC__)
naming_fn(bdd_manager bddm, bdd v, pointer junk)
#else
naming_fn(bddm, v, junk)
     bdd_manager bddm;
     bdd v;
     pointer junk;
#endif
{
  return (hash_lookup(name_table, (pointer)v));
}


void
#if defined(__STDC__)
free_name(pointer junk1, pointer s, pointer junk2)
#else
free_name(junk1, s, junk2)
     pointer junk1;
     pointer s;
     pointer junk2;
#endif
{
  mem_free_block(s);
}


void
#if defined(__STDC__)
print_bdd(bdd f)
#else
print_bdd(f)
     bdd f;
#endif
{
  bdd_print_bdd(bddm, f, naming_fn, bdd_terminal_id_fn_none, (pointer)0, stdout);
}


static
void
#if defined(__STDC__)
print_cube(bdd cube)
#else
print_cube(cube)
     bdd cube;
#endif
{
  bdd v;
  bdd next;
  bdd temp;

  cube=bdd_identity(bddm, cube);
  while (1)
    {
      v=bdd_if(bddm, cube);
      temp=bdd_and(bddm, v, cube);
      if (temp == bdd_zero(bddm))
	putchar('!');
      else
	bdd_free(bddm, temp);
      fputs(naming_fn(bddm, v, (pointer)0), stdout);
      next=bdd_then(bddm, cube);
      if (next == bdd_zero(bddm))
	next=bdd_else(bddm, cube);
      bdd_free(bddm, cube);
      cube=next;
      if (cube != bdd_one(bddm))
	fputs(" & ", stdout);
      else
	break;
    }
}


/* Some attempt at printing reasonably small SOP expressions... */

void
#if defined(__STDC__)
print_sop(bdd f)
#else
print_sop(f)
     bdd f;
#endif
{
  bdd outside;
  bdd cube;
  bdd temp1, temp2;

  if (f == bdd_one(bddm))
    puts("1");
  else if (f == bdd_zero(bddm))
    puts("0");
  else
    {
      f=bdd_identity(bddm, f);
      outside=bdd_not(bddm, f);
      while (1)
	{
	  /* Pick a cube. */
	  cube=bdd_satisfy(bddm, f);
	  temp1=bdd_or(bddm, outside, cube);
	  temp2=cube;
	  /* Expand the cube, but don't include anything outside of f. */
	  cube=bdd_reduce(bddm, temp2, temp1);
	  bdd_free(bddm, temp1);
	  bdd_free(bddm, temp2);
	  print_cube(cube);
	  temp1=bdd_not(bddm, cube);
	  bdd_free(bddm, cube);
	  temp2=f;
	  /* Remove cube from f. */
	  f=bdd_and(bddm, temp1, temp2);
	  bdd_free(bddm, temp1);
	  bdd_free(bddm, temp2);
	  if (f != bdd_zero(bddm))
	    puts(" |");
	  else
	    {
	      putchar('\n');
	      break;
	    }
	}
      bdd_free(bddm, outside);
    }
}


void
#if defined(__STDC__)
use_assoc(assoc a)
#else
use_assoc(a)
     assoc a;
#endif
{
  if (!a->times_used)
    bdd_temp_assoc(bddm, (bdd *)vec_elements(a->assoc), 1);
  else if (a->times_used == 1)
    a->assoc_id=bdd_new_assoc(bddm, (bdd *)vec_elements(a->assoc), 1);
  a->times_used++;
  bdd_assoc(bddm, a->assoc_id);
}


void
#if defined(__STDC__)
free_assoc(assoc a)
#else
free_assoc(a)
     assoc a;
#endif
{
  pointer *p;

  if (!--(a->refs))
    {
      if (a->assoc_id != -1)
	bdd_free_assoc(bddm, a->assoc_id);
      for (p=vec_elements(a->assoc); *p; ++p)
	bdd_free(bddm, (bdd)*p);
      free_vec(a->assoc);
      mem_free_block((pointer)a);
    }
}


assoc
#if defined(__STDC__)
copy_assoc(assoc a)
#else
copy_assoc(a)
     assoc a;
#endif
{
  a->refs++;
  return (a);
}


assoc
#if defined(__STDC__)
new_assoc(vec assc)
#else
new_assoc(assc)
     vec assc;
#endif
{
  assoc a;

  a=(assoc)mem_get_block((SIZE_T)sizeof(struct assoc_));
  a->refs=1;
  a->times_used=0;
  a->assoc_id= -1;
  a->assoc=assc;
  return (a);
}


void
#if defined(__STDC__)
get_cpu_time(double *user, double *system)
#else
get_cpu_time(user, system)
     double *user;
     double *system;
#endif
{
  struct rusage resources;

  getrusage(RUSAGE_SELF, &resources);
  *user=resources.ru_utime.tv_sec+resources.ru_utime.tv_usec/1.0e6;
  *system=resources.ru_stime.tv_sec+resources.ru_stime.tv_usec/1.0e6;
}


void
#if defined(__STDC__)
prompt(void)
#else
prompt()
#endif
{
  if (interactive)
    {
      if (first_line)
	{
	  fputs("(boole) ", stdout);
	  first_line=0;
	}
      else
	fputs("[boole] ", stdout);
      fflush(stdout);
    }
}

FILE* yyin = NULL;
FILE* yyout = NULL;
int
#if defined(__STDC__)
main(int argc, char **argv)
#else
main(argc, argv)
     int argc;
     char **argv;
#endif
{
  yyin = stdin;
  yyout = stdout;
  ++argv;
  while (--argc)
    {
      /* Would be nice to detect automatically when input is from a */
      /* terminal. */
      if (strcmp(*argv, "-i") == 0)
	interactive=1;
      else
	{
	  fprintf(stderr, "unrecognized option: %s\n", *argv);
	  exit(1);
	}
      ++argv;
    }
  bddm=bdd_init();
  init_tables();
  line_number=1;
  first_line=1;
  if (interactive)
    fputs("Boole version 1.0\n", stdout);
  prompt();
  yyparse();
  if (interactive)
    putchar('\n');
  return (0);
}
