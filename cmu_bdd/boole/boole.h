#include <bdduser.h>
#include "hash.h"
#include "vec.h"


#if defined(__STDC__)
#define ARGS(args) args
#else
#define ARGS(args) ()
#endif


struct assoc_
{
  int refs;
  int times_used;
  int assoc_id;
  vec assoc;
};

typedef struct assoc_ *assoc;


#define KEYWORD 0
#define EXPR 1
#define VARASSOC 2
#define NEW 3
#define KILLED 4


struct st_entry_
{
  int type;
  char *id;
  union
    {
      struct
	{
	  int code;
	} keyword;
      struct
	{
	  bdd f;
	} expr;
      struct
	{
	  assoc a;
	} varassoc;
    } info;
};

typedef struct st_entry_ *st_entry;


extern int interactive;
extern int first_line;
extern int line_number;
extern hash_table symbol_table;
extern hash_table name_table;
extern bdd_manager bddm;
extern int timer_running;
extern double user_start;
extern double system_start;
extern double user_last;
extern double system_last;


extern void free_entry ARGS((st_entry));
extern void free_full_entry ARGS((pointer, pointer, pointer));
extern void free_name ARGS((pointer, pointer, pointer));

extern char *naming_fn ARGS((bdd_manager, bdd, pointer));
extern void print_bdd ARGS((bdd));
extern void print_sop ARGS((bdd));

extern void init_tables ARGS((void));

extern char *copy_str ARGS((char *));
extern char *undefined ARGS((char *));

extern void use_assoc ARGS((assoc));
extern void free_assoc ARGS((assoc));
extern assoc copy_assoc ARGS((assoc));
extern assoc new_assoc ARGS((vec));

extern void get_cpu_time ARGS((double *, double *));

extern int strcmp ARGS((const char *, const char *));
//extern int strlen ARGS((const char *));
extern char *strcpy ARGS((char *, const char *));
extern void exit ARGS((int));

extern int yyparse ARGS((void));
extern void prompt ARGS((void));
