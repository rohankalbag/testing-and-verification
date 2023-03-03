#include "truser.h"
#include "vec.h"
#include <stdlib.h>


#define SS_BASIC 0
#define SS_CONJUNCTIVE 1
#define SS_DISJUNCTIVE 2
#define SS_CONDITION 3
#define SS_REDUCE 4


struct step_stuff_
{
  int type;
  union
    {
      struct
	{
	  int c;
	  bdd f;
	} basic;
      struct
	{
	  int ic;
	  vec ss;
	} conjunctive;
      struct
	{
	  vec unch;
	  vec ss;
	} disjunctive;
      struct
	{
	  bdd pre;
	  bdd post;
	  struct step_stuff_ *s;
	} condition;
      struct
	{
	  bdd post;
	  struct step_stuff_ *s;
	} reduce;
    } info;
};

typedef struct step_stuff_ *step_stuff;


#define TR_BASIC 0
#define TR_CONJUNCTIVE 1
#define TR_DISJUNCTIVE 2
#define TR_CONDITION 3
#define TR_REDUCE 4
#define TR_DUMMY 42


struct tr_
{
  int ref_count;
  int type;
  union
    {
      struct
	{
	  vec changes;
	  vec uses;
	  vec produces;
	  bdd f;
	} basic;
      struct
	{
	  vec ts;
	} conjunctive;
      struct
	{
	  vec ts;
	} disjunctive;
      struct
	{
	  bdd pre;
	  bdd post;
	  vec uses;
	  vec produces;
	  struct tr_ *t;
	} condition;
      struct
	{
	  bdd pre;
	  bdd post;
	  struct tr_ *t;
	} reduce;
    } info;
  step_stuff s;
  int ns_cs;
};


struct ss_level_
{
  step_stuff s;
  int i;
  bdd reached;
  vec frontiers;
};

typedef struct ss_level_ *ss_level;


struct search_state_
{
  tr t;
  tr dummy;
  bdd reached;
  vec levels;
  int fixpoint;
};


struct reorder_rec_
{
  tr t;
  vec uses;
  vec produces;
};

typedef struct reorder_rec_ *reorder_rec;


static
void
#if defined(__STDC__)
fatal(char *msg)
#else
fatal(msg)
     char *msg;
#endif
{
  fprintf(stderr, "TR library: error: %s\n", msg);
  exit(1);
}


static
void
#if defined(__STDC__)
supports(bdd_manager bddm,
	 bdd f,
	 vec uses,
	 vec produces,
	 bdd (*vt_fn)(bdd_manager, bdd, int, pointer),
	 pointer vt_env)
#else
supports(bddm, f, uses, produces, vt_fn, vt_env)
     bdd_manager bddm;
     bdd f;
     vec uses;
     vec produces;
     bdd (*vt_fn)();
     pointer vt_env;
#endif
{
  long i;
  bdd *support;

  support=(bdd *)mem_get_block(sizeof(bdd)*(bdd_vars(bddm)+1));
  bdd_support(bddm, f, support);
  for (i=0; support[i]; ++i)
    {
      if ((*vt_fn)(bddm, support[i], VT_NORMAL, vt_env) == support[i])
	vec_add(uses, (pointer)support[i]);
      if ((*vt_fn)(bddm, support[i], VT_PRIME, vt_env) == support[i])
	vec_add(produces, (pointer)(*vt_fn)(bddm, support[i], VT_NORMAL, vt_env));
    }
  mem_free_block((pointer)support);
}


static
bdd
#if defined(__STDC__)
primed_form(bdd_manager bddm, bdd f, bdd (*vt_fn)(bdd_manager, bdd, int, pointer), pointer vt_env)
#else
primed_form(bddm, f, vt_fn, vt_env)
     bdd_manager bddm;
     bdd f;
     bdd (*vt_fn)();
     pointer vt_env;
#endif
{
  long i;
  bdd *assoc;
  bdd result;

  assoc=(bdd *)mem_get_block((SIZE_T)(sizeof(bdd)*(2*bdd_vars(bddm)+2)));
  bdd_support(bddm, f, assoc);
  for (i=0; assoc[i]; ++i);
  assoc[2*i]=0;
  assoc[2*i+1]=0;
  while (i)
    {
      --i;
      assoc[2*i]=assoc[i];
      assoc[2*i+1]=(*vt_fn)(bddm, assoc[i], VT_PRIME, vt_env);
    }
  bdd_temp_assoc(bddm, assoc, 1);
  bdd_assoc(bddm, -1);
  result=bdd_substitute(bddm, f);
  mem_free_block((pointer)assoc);
  return (result);
}


static
vec
#if defined(__STDC__)
bdds_to_vec(bdd *fs)
#else
bdds_to_vec(fs)
     bdd *fs;
#endif
{
  vec v;
  long i;

  v=new_vec();
  for (i=0; fs[i]; ++i)
    vec_add(v, fs[i]);
  return (v);
}


static
void
#if defined(__STDC__)
count_vars_aux1(bdd_manager bddm, vec vars, int *counts)
#else
count_vars_aux1(bddm, vars, counts)
     bdd_manager bddm;
     vec vars;
     int *counts;
#endif
{
  pointer *p;

  if (counts)
    for (p=vec_elements(vars); *p; ++p)
      counts[bdd_if_id(bddm, (bdd)*p)-1]++;
}


static
void
#if defined(__STDC__)
count_vars_aux2(bdd_manager bddm, tr t, int *changes, int *uses, int *produces)
#else
count_vars_aux2(bddm, t, changes, uses, produces)
     bdd_manager bddm;
     tr t;
     int *changes;
     int *uses;
     int *produces;
#endif
{
  pointer *p;

  switch (t->type)
    {
    case TR_BASIC:
      count_vars_aux1(bddm, t->info.basic.changes, changes);
      count_vars_aux1(bddm, t->info.basic.uses, uses);
      count_vars_aux1(bddm, t->info.basic.produces, produces);
      break;
    case TR_CONJUNCTIVE:
      for (p=vec_elements(t->info.conjunctive.ts); *p; ++p)
	count_vars_aux2(bddm, (tr)*p, changes, uses, produces);
      break;
    case TR_DISJUNCTIVE:
      for (p=vec_elements(t->info.disjunctive.ts); *p; ++p)
	count_vars_aux2(bddm, (tr)*p, changes, uses, produces);
      break;
    case TR_CONDITION:
      count_vars_aux1(bddm, t->info.condition.uses, uses);
      count_vars_aux1(bddm, t->info.condition.produces, produces);
      count_vars_aux2(bddm, t->info.condition.t, changes, uses, produces);
      break;
    case TR_REDUCE:
      count_vars_aux2(bddm, t->info.reduce.t, changes, uses, produces);
      break;
    default:
      fatal("count_vars_aux2: invalid TR type");
    }
}


static
void
#if defined(__STDC__)
count_vars(bdd_manager bddm, tr t, int *changes, int *uses, int *produces)
#else
count_vars(bddm, t, changes, uses, produces)
     bdd_manager bddm;
     tr t;
     int *changes;
     int *uses;
     int *produces;
#endif
{
  long i;

  if (changes)
    for (i=bdd_vars(bddm); i; --i)
      changes[i-1]=0;
  if (uses)
    for (i=bdd_vars(bddm); i; --i)
      uses[i-1]=0;
  if (produces)
    for (i=bdd_vars(bddm); i; --i)
      produces[i-1]=0;
  count_vars_aux2(bddm, t, changes, uses, produces);
}


static
void
#if defined(__STDC__)
frontier_trs(int type, tr t, vec v)
#else
frontier_trs(type, t, v)
     int type;
     tr t;
     vec v;
#endif
{
  pointer *p;

  if (t->type != type)
    vec_add(v, (pointer)t);
  else if (type == TR_CONJUNCTIVE)
    for (p=vec_elements(t->info.conjunctive.ts); *p; ++p)
      frontier_trs(type, (tr)*p, v);
  else
    for (p=vec_elements(t->info.disjunctive.ts); *p; ++p)
      frontier_trs(type, (tr)*p, v);
}


static
void
#if defined(__STDC__)
change(bdd_manager bddm, int *this_changes, int *changes, int *uses)
#else
change(bddm, this_changes, changes, uses)
     bdd_manager bddm;
     int *this_changes;
     int *changes;
     int *uses;
#endif
{
  long i;

  for (i=bdd_vars(bddm); i; --i)
    if (changes[i-1] && !uses[i-1])
      {
	this_changes[i-1]=1;
	changes[i-1]=0;
      }
    else
      this_changes[i-1]=0;
}


static
bdd
#if defined(__STDC__)
reduce_using(bdd_manager bddm, bdd f, vec v, int check_sizes)
#else
reduce_using(bddm, f, v, check_sizes)
     bdd_manager bddm;
     bdd f;
     vec v;
     int check_sizes;
#endif
{
  bdd temp;
  pointer *p;

  for (p=vec_elements(v); *p; ++p)
    {
      temp=f;
      f=bdd_reduce(bddm, temp, (bdd)*p);
      if (check_sizes && (bdd_size(bddm, f, 1) > bdd_size(bddm, temp, 1)))
	{
	  bdd_free(bddm, f);
	  f=temp;
	}
      else
	bdd_free(bddm, temp);
    }
  return (f);
}


static
vec
#if defined(__STDC__)
var_vec(bdd_manager bddm, int *vars)
#else
var_vec(bddm, vars)
     bdd_manager bddm;
     int *vars;
#endif
{
  vec v;
  long i;

  v=new_vec();
  for (i=bdd_vars(bddm); i; --i)
    if (vars[i-1])
      vec_add(v, (pointer)bdd_var_with_id(bddm, i));
  return (v);
}


static
bdd
#if defined(__STDC__)
unchanging(bdd_manager bddm, int *this_changes, int *changes, bdd (*vt_fn)(bdd_manager, bdd, int, pointer), pointer vt_env)
#else
unchanging(bddm, this_changes, changes, vt_fn, vt_env)
     bdd_manager bddm;
     int *this_changes;
     int *changes;
     bdd (*vt_fn)();
     pointer vt_env;
#endif
{
  long i;
  bdd temp1, temp2;
  bdd result;

  result=bdd_one(bddm);
  for (i=bdd_vars(bddm); i; --i)
    if (!this_changes[i-1] && changes[i-1])
      {
	temp1=bdd_var_with_id(bddm, i);
	temp2=temp1;
	temp1=bdd_xnor(bddm, temp2, (*vt_fn)(bddm, temp2, VT_PRIME, vt_env));
	temp2=result;
	result=bdd_and(bddm, temp1, temp2);
	bdd_free(bddm, temp1);
	bdd_free(bddm, temp2);
      }
  return (result);
}


static
step_stuff
#if defined(__STDC__)
compute_ss_basic(bdd_manager bddm, tr t, int *changes, vec reduce)
#else
compute_ss_basic(bddm, t, changes, reduce)
     bdd_manager bddm;
     tr t;
     int *changes;
     vec reduce;
#endif
{
  step_stuff s;
  vec c;

  s=(step_stuff)mem_get_block((SIZE_T)sizeof(struct step_stuff_));
  s->type=SS_BASIC;
  c=var_vec(bddm, changes);
  s->info.basic.c=bdd_new_assoc(bddm, (bdd *)vec_elements(c), 0);
  free_vec(c);
  s->info.basic.f=reduce_using(bddm, bdd_identity(bddm, t->info.basic.f), reduce, 1);
  return (s);
}


#if defined(__STDC__)
static step_stuff compute_ss(bdd_manager, tr, int *, int, vec, bdd (*)(bdd_manager, bdd, int, pointer), pointer);
#else
static step_stuff compute_ss();
#endif


static
step_stuff
#if defined(__STDC__)
compute_ss_conjunctive(bdd_manager bddm,
		       tr t,
		       int *changes,
		       vec reduce,
		       bdd (*vt_fn)(bdd_manager, bdd, int, pointer),
		       pointer vt_env)
#else
compute_ss_conjunctive(bddm, t, changes, reduce, vt_fn, vt_env)
     bdd_manager bddm;
     tr t;
     int *changes;
     vec reduce;
     bdd (*vt_fn)();
     pointer vt_env;
#endif
{
  step_stuff s;
  vec v;
  int *uses;
  int *this_uses;
  int *this_changes;
  int *saved_changes;
  pointer *p;
  vec ic;
  vec ss;
  long i;

  s=(step_stuff)mem_get_block((SIZE_T)sizeof(struct step_stuff_));
  s->type=SS_CONJUNCTIVE;
  v=new_vec();
  frontier_trs(TR_CONJUNCTIVE, t, v);
  uses=(int *)mem_get_block((SIZE_T)(sizeof(int)*bdd_vars(bddm)));
  for (p=vec_elements(v); *p; ++p)
    if (p == vec_elements(v))
      count_vars(bddm, (tr)*p, (int *)0, uses, (int *)0);
    else
      count_vars_aux2(bddm, (tr)*p, (int *)0, uses, (int *)0);
  saved_changes=(int *)mem_get_block((SIZE_T)(sizeof(int)*bdd_vars(bddm)));
  for (i=bdd_vars(bddm); i; --i)
    saved_changes[i-1]=changes[i-1];
  this_changes=(int *)mem_get_block((SIZE_T)(sizeof(int)*bdd_vars(bddm)));
  change(bddm, this_changes, changes, uses);
  ic=var_vec(bddm, this_changes);
  s->info.conjunctive.ic=bdd_new_assoc(bddm, (bdd *)vec_elements(ic), 0);
  free_vec(ic);
  this_uses=(int *)mem_get_block((SIZE_T)(sizeof(int)*bdd_vars(bddm)));
  ss=new_vec();
  for (p=vec_elements(v); *p; ++p)
    {
      count_vars(bddm, (tr)*p, (int *)0, this_uses, (int *)0);
      for (i=bdd_vars(bddm); i; --i)
	uses[i-1]-=this_uses[i-1];
      change(bddm, this_changes, changes, uses);
      vec_add(ss, (pointer)compute_ss(bddm, (tr)*p, this_changes, 0, reduce, vt_fn, vt_env));
    }
  for (i=bdd_vars(bddm); i; --i)
    changes[i-1]=saved_changes[i-1];
  s->info.conjunctive.ss=ss;
  mem_free_block((pointer)this_uses);
  mem_free_block((pointer)this_changes);
  mem_free_block((pointer)saved_changes);
  mem_free_block((pointer)uses);
  return (s);
}


static
step_stuff
#if defined(__STDC__)
compute_ss_disjunctive(bdd_manager bddm,
		       tr t,
		       int *changes,
		       int top_level_disj,
		       vec reduce,
		       bdd (*vt_fn)(bdd_manager, bdd, int, pointer),
		       pointer vt_env)
#else
compute_ss_disjunctive(bddm, t, changes, top_level_disj, reduce, vt_fn, vt_env)
     bdd_manager bddm;
     tr t;
     int *changes;
     int top_level_disj;
     vec reduce;
     bdd (*vt_fn)();
     pointer vt_env;
#endif
{
  step_stuff s;
  vec ss;
  vec unch;
  pointer *p;
  int *this_changes;
  int *all_changes;

  s=(step_stuff)mem_get_block((SIZE_T)sizeof(struct step_stuff_));
  s->type=SS_DISJUNCTIVE;
  ss=new_vec();
  unch=new_vec();
  all_changes=(int *)mem_get_block((SIZE_T)(sizeof(int)*bdd_vars(bddm)));
  count_vars(bddm, t, all_changes, (int *)0, (int *)0);
  this_changes=(int *)mem_get_block((SIZE_T)(sizeof(int)*bdd_vars(bddm)));
  for (p=vec_elements(t->info.disjunctive.ts); *p; ++p)
    {
      count_vars(bddm, (tr)*p, this_changes, (int *)0, (int *)0);
      if (top_level_disj)
	vec_add(unch, (pointer)bdd_one(bddm));
      else
	vec_add(unch, (pointer)unchanging(bddm, this_changes, all_changes, vt_fn, vt_env));
      if (top_level_disj)
	vec_add(ss, (pointer)compute_ss(bddm, (tr)*p, this_changes, top_level_disj, reduce, vt_fn, vt_env));
      else
	vec_add(ss, (pointer)compute_ss(bddm, (tr)*p, changes, top_level_disj, reduce, vt_fn, vt_env));
    }
  mem_free_block((pointer)all_changes);
  mem_free_block((pointer)this_changes);
  s->info.disjunctive.ss=ss;
  s->info.disjunctive.unch=unch;
  return (s);
}


static
step_stuff
#if defined(__STDC__)
compute_ss_condition(bdd_manager bddm,
		     tr t,
		     int *changes,
		     int top_level_disj,
		     vec reduce,
		     bdd (*vt_fn)(bdd_manager, bdd, int, pointer),
		     pointer vt_env)
#else
compute_ss_condition(bddm, t, changes, top_level_disj, reduce, vt_fn, vt_env)
     bdd_manager bddm;
     tr t;
     int *changes;
     int top_level_disj;
     vec reduce;
     bdd (*vt_fn)();
     pointer vt_env;
#endif
{
  step_stuff s;
  bdd pre;
  bdd post;

  s=(step_stuff)mem_get_block((SIZE_T)sizeof(struct step_stuff_));
  s->type=SS_CONDITION;
  pre=reduce_using(bddm, bdd_identity(bddm, t->info.condition.pre), reduce, 1);
  post=reduce_using(bddm, primed_form(bddm, t->info.condition.post, vt_fn, vt_env), reduce, 1);
  s->info.condition.pre=pre;
  s->info.condition.post=post;
  vec_add(reduce, (pointer)pre);
  vec_add(reduce, (pointer)post);
  s->info.condition.s=compute_ss(bddm, t->info.condition.t, changes, top_level_disj, reduce, vt_fn, vt_env);
  vec_del(reduce);
  vec_del(reduce);
  return (s);
}


static
step_stuff
#if defined(__STDC__)
compute_ss_reduce(bdd_manager bddm,
		  tr t,
		  int *changes,
		  int top_level_disj,
		  vec reduce,
		  bdd (*vt_fn)(bdd_manager, bdd, int, pointer),
		  pointer vt_env)
#else
compute_ss_reduce(bddm, t, changes, top_level_disj, reduce, vt_fn, vt_env)
     bdd_manager bddm;
     tr t;
     int *changes;
     int top_level_disj;
     vec reduce;
     bdd (*vt_fn)();
     pointer vt_env;
#endif
{
  step_stuff s;
  bdd post;

  s=(step_stuff)mem_get_block((SIZE_T)sizeof(struct step_stuff_));
  s->type=SS_REDUCE;
  post=primed_form(bddm, t->info.reduce.post, vt_fn, vt_env);
  s->info.reduce.post=post;
  vec_add(reduce, (pointer)t->info.reduce.pre);
  vec_add(reduce, (pointer)post);
  s->info.reduce.s=compute_ss(bddm, t->info.reduce.t, changes, top_level_disj, reduce, vt_fn, vt_env);
  vec_del(reduce);
  vec_del(reduce);
  return (s);
}


static
step_stuff
#if defined(__STDC__)
compute_ss(bdd_manager bddm,
	   tr t,
	   int *changes,
	   int top_level_disj,
	   vec reduce,
	   bdd (*vt_fn)(bdd_manager, bdd, int, pointer),
	   pointer vt_env)
#else
compute_ss(bddm, t, changes, top_level_disj, reduce, vt_fn, vt_env)
     bdd_manager bddm;
     tr t;
     int *changes;
     int top_level_disj;
     vec reduce;
     bdd (*vt_fn)();
     pointer vt_env;
#endif
{
  switch (t->type)
    {
    case TR_BASIC:
      return (compute_ss_basic(bddm, t, changes, reduce));
    case TR_CONJUNCTIVE:
      return (compute_ss_conjunctive(bddm, t, changes, reduce, vt_fn, vt_env));
    case TR_DISJUNCTIVE:
      return (compute_ss_disjunctive(bddm, t, changes, top_level_disj, reduce, vt_fn, vt_env));
    case TR_CONDITION:
      return (compute_ss_condition(bddm, t, changes, top_level_disj, reduce, vt_fn, vt_env));
    case TR_REDUCE:
      return (compute_ss_reduce(bddm, t, changes, top_level_disj, reduce, vt_fn, vt_env));
    default:
      fatal("compute_ss: invalid TR type");
      return ((step_stuff)0);
    }
}


static
void
#if defined(__STDC__)
compute_step_stuff(bdd_manager bddm, tr t, bdd (*vt_fn)(bdd_manager, bdd, int, pointer), pointer vt_env)
#else
compute_step_stuff(bddm, t, vt_fn, vt_env)
     bdd_manager bddm;
     tr t;
     bdd (*vt_fn)();
     pointer vt_env;
#endif
{
  int *changes;
  vec reduce;
  vec assoc;
  long i;
  bdd temp;

  if (!t->s)
    {
      changes=(int *)mem_get_block((SIZE_T)(sizeof(int)*bdd_vars(bddm)));
      count_vars(bddm, t, changes, (int *)0, (int *)0);
      assoc=new_vec();
      for (i=bdd_vars(bddm); i; --i)
	if (changes[i-1])
	  {
	    temp=bdd_var_with_id(bddm, i);
	    vec_add(assoc, (*vt_fn)(bddm, temp, VT_PRIME, vt_env));
	    vec_add(assoc, (pointer)temp);
	  }
      vec_add(assoc, (pointer)0);
      t->ns_cs=bdd_new_assoc(bddm, (bdd *)vec_elements(assoc), 1);
      free_vec(assoc);
      reduce=new_vec();
      t->s=compute_ss(bddm, t, changes, 1, reduce, vt_fn, vt_env);
      free_vec(reduce);
    }
}


static
bdd
#if defined(__STDC__)
step_aux(bdd_manager bddm, step_stuff s, bdd f, vec reduce)
#else
step_aux(bddm, s, f, reduce)
     bdd_manager bddm;
     step_stuff s;
     bdd f;
     vec reduce;
#endif
{
  bdd temp1, temp2;
  bdd result;
  pointer *p, *q;

  switch (s->type)
    {
    case SS_BASIC:
      temp1=reduce_using(bddm, bdd_identity(bddm, s->info.basic.f), reduce, 0);
      bdd_assoc(bddm, s->info.basic.c);
      result=bdd_rel_prod(bddm, temp1, f);
      bdd_free(bddm, temp1);
      bdd_free(bddm, f);
      return (reduce_using(bddm, result, reduce, 0));
    case SS_CONJUNCTIVE:
      bdd_assoc(bddm, s->info.conjunctive.ic);
      temp1=f;
      f=bdd_exists(bddm, temp1);
      bdd_free(bddm, temp1);
      for (p=vec_elements(s->info.conjunctive.ss); *p; ++p)
	f=step_aux(bddm, (step_stuff)*p, f, reduce);
      return (f);
    case SS_DISJUNCTIVE:
      result=bdd_zero(bddm);
      for (p=vec_elements(s->info.disjunctive.ss), q=vec_elements(s->info.disjunctive.unch); *p; ++p, ++q)
	{
	  temp1=bdd_and(bddm, f, (bdd)*q);
	  if (!*(p+1))
	    bdd_free(bddm, f);
	  temp1=step_aux(bddm, (step_stuff)*p, temp1, reduce);
	  temp2=result;
	  result=bdd_or(bddm, temp1, temp2);
	  bdd_free(bddm, temp1);
	  bdd_free(bddm, temp2);
	  result=reduce_using(bddm, result, reduce, 0);
	}
      return (result);
    case SS_CONDITION:
      temp1=f;
      f=bdd_and(bddm, temp1, s->info.condition.pre);
      bdd_free(bddm, temp1);
      vec_add(reduce, s->info.condition.post);
      result=step_aux(bddm, s->info.condition.s, f, reduce);
      vec_del(reduce);
      temp1=result;
      result=bdd_and(bddm, temp1, s->info.condition.post);
      bdd_free(bddm, temp1);
      return (reduce_using(bddm, result, reduce, 0));
    case SS_REDUCE:
      vec_add(reduce, s->info.reduce.post);
      result=step_aux(bddm, s->info.reduce.s, f, reduce);
      vec_del(reduce);
      return (result);
    default:
      fatal("step_aux: unknown TR type");
      return ((bdd)0);
    }
}


static
bdd
#if defined(__STDC__)
step(bdd_manager bddm,
     tr t,
     bdd f,
     bdd (*vt_fn)(bdd_manager, bdd, int, pointer),
     pointer vt_env,
     vec reduce)
#else
step(bddm, t, f, vt_fn, vt_env, reduce)
     bdd_manager bddm;
     tr t;
     bdd f;
     bdd (*vt_fn)();
     pointer vt_env;
     vec reduce;
#endif
{
  bdd temp;
  bdd result;

  compute_step_stuff(bddm, t, vt_fn, vt_env);
  result=step_aux(bddm, t->s, f, reduce);
  bdd_assoc(bddm, t->ns_cs);
  temp=result;
  result=bdd_substitute(bddm, temp);
  bdd_free(bddm, temp);
  return (result);
}


bdd
#if defined(__STDC__)
tr_step(bdd_manager bddm, tr t, bdd f, bdd (*vt_fn)(bdd_manager, bdd, int, pointer), pointer vt_env)
#else
tr_step(bddm, t, f, vt_fn, vt_env)
     bdd_manager bddm;
     tr t;
     bdd f;
     bdd (*vt_fn)();
     pointer vt_env;
#endif
{
  vec reduce;
  bdd result;

  reduce=new_vec();
  result=step(bddm, t, bdd_identity(bddm, f), vt_fn, vt_env, reduce);
  free_vec(reduce);
  return (result);
}


bdd
#if defined(__STDC__)
tr_reduce_step_multiple(bdd_manager bddm,
			tr t,
			bdd f,
			bdd *pre,
			bdd *post,
			bdd (*vt_fn)(bdd_manager, bdd, int, pointer),
			pointer vt_env)
#else
tr_reduce_step_multiple(bddm, t, f, pre, post, vt_fn, vt_env)
     bdd_manager bddm;
     tr t;
     bdd f;
     bdd *pre;
     bdd *post;
     bdd (*vt_fn)();
     pointer vt_env;
#endif
{
  vec reduce;
  bdd *fs;
  bdd result;
  bdd temp;
  pointer *p;

  reduce=new_vec();
  for (fs=pre; *fs; ++fs)
    vec_add(reduce, (pointer)*fs);
  f=reduce_using(bddm, bdd_identity(bddm, f), reduce, 0);
  free_vec(reduce);
  reduce=new_vec();
  for (fs=post; *fs; ++fs)
    {
      temp=primed_form(bddm, (bdd)*fs, vt_fn, vt_env);
      vec_add(reduce, (pointer)temp);
    }
  result=step(bddm, t, f, vt_fn, vt_env, reduce);
  for (p=vec_elements(reduce); *p; ++p)
    bdd_free(bddm, (bdd)*p);
  free_vec(reduce);
  return (result);
}


bdd
#if defined(__STDC__)
tr_reduce_step(bdd_manager bddm,
	       tr t,
	       bdd f,
	       bdd pre,
	       bdd post,
	       bdd (*vt_fn)(bdd_manager, bdd, int, pointer),
	       pointer vt_env)
#else
tr_reduce_step(bddm, t, f, pre, post, vt_fn, vt_env)
     bdd_manager bddm;
     tr t;
     bdd f;
     bdd pre;
     bdd post;
     bdd (*vt_fn)();
     pointer vt_env;
#endif
{
  bdd pres[2];
  bdd posts[2];

  pres[0]=pre;
  pres[1]=0;
  posts[0]=post;
  posts[1]=0;
  return (tr_reduce_step_multiple(bddm, t, f, pres, posts, vt_fn, vt_env));
}


tr
#if defined(__STDC__)
tr_copy(bdd_manager bddm, tr t)
#else
tr_copy(bddm, t)
     bdd_manager bddm;
     tr t;
#endif
{
  if (t->type == TR_DUMMY)
    fatal("tr_copy: illegal use of transition relation from tr_search");
  t->ref_count++;
  return (t);
}


tr
#if defined(__STDC__)
tr_basic(bdd_manager bddm, bdd t1, bdd *changes, bdd (*vt_fn)(bdd_manager, bdd, int, pointer), pointer vt_env)
#else
tr_basic(bddm, t1, changes, vt_fn, vt_env)
     bdd_manager bddm;
     bdd t1;
     bdd *changes;
     bdd (*vt_fn)();
     pointer vt_env;
#endif
{
  tr t;

  t=(tr)mem_get_block((long)sizeof(struct tr_));
  t->ref_count=1;
  t->type=TR_BASIC;
  t->info.basic.changes=bdds_to_vec(changes);
  t->info.basic.uses=new_vec();
  t->info.basic.produces=new_vec();
  supports(bddm, t1, t->info.basic.uses, t->info.basic.produces, vt_fn, vt_env);
  t->info.basic.f=bdd_identity(bddm, t1);
  t->s=0;
  return (t);
}


tr
#if defined(__STDC__)
tr_conjunctive_multiple(bdd_manager bddm, tr *ts)
#else
tr_conjunctive_multiple(bddm, ts)
     bdd_manager bddm;
     tr *ts;
#endif
{
  tr t;

  t=(tr)mem_get_block((long)sizeof(struct tr_));
  t->ref_count=1;
  t->type=TR_CONJUNCTIVE;
  t->info.conjunctive.ts=new_vec();
  while (*ts)
    {
      vec_add(t->info.conjunctive.ts, (pointer)tr_copy(bddm, *ts));
      ++ts;
    }
  t->s=0;
  return (t);
}


tr
#if defined(__STDC__)
tr_conjunctive(bdd_manager bddm, tr t1, tr t2)
#else
tr_conjunctive(bddm, t1, t2)
     bdd_manager bddm;
     tr t1;
     tr t2;
#endif
{
  tr ts[3];

  ts[0]=t1;
  ts[1]=t2;
  ts[2]=0;
  return (tr_conjunctive_multiple(bddm, ts));
}


tr
#if defined(__STDC__)
tr_disjunctive_multiple(bdd_manager bddm, tr *ts)
#else
tr_disjunctive_multiple(bddm, ts)
     bdd_manager bddm;
     tr *ts;
#endif
{
  tr t;

  t=(tr)mem_get_block((long)sizeof(struct tr_));
  t->ref_count=1;
  t->type=TR_DISJUNCTIVE;
  t->info.disjunctive.ts=new_vec();
  while (*ts)
    {
      vec_add(t->info.disjunctive.ts, (pointer)tr_copy(bddm, *ts));
      ++ts;
    }
  t->s=0;
  return (t);
}


tr
#if defined(__STDC__)
tr_disjunctive(bdd_manager bddm, tr t1, tr t2)
#else
tr_disjunctive(bddm, t1, t2)
     bdd_manager bddm;
     tr t1;
     tr t2;
#endif
{
  tr ts[3];

  ts[0]=t1;
  ts[1]=t2;
  ts[2]=0;
  return (tr_disjunctive_multiple(bddm, ts));
}


tr
#if defined(__STDC__)
tr_condition(bdd_manager bddm, tr t1, bdd pre, bdd post, bdd (*vt_fn)(bdd_manager, bdd, int, pointer), pointer vt_env)
#else
tr_condition(bddm, t1, pre, post, vt_fn, vt_env)
     bdd_manager bddm;
     tr t1;
     bdd pre;
     bdd post;
     bdd (*vt_fn)();
     pointer vt_env;
#endif
{
  tr t;
  bdd temp;

  t=(tr)mem_get_block((long)sizeof(struct tr_));
  t->ref_count=1;
  t->type=TR_CONDITION;
  t->info.condition.uses=new_vec();
  t->info.condition.produces=new_vec();
  supports(bddm, pre, t->info.condition.uses, t->info.condition.produces, vt_fn, vt_env);
  temp=primed_form(bddm, post, vt_fn, vt_env);
  supports(bddm, temp, t->info.condition.uses, t->info.condition.produces, vt_fn, vt_env);
  bdd_free(bddm, temp);
  t->info.condition.pre=bdd_identity(bddm, pre);
  t->info.condition.t=tr_copy(bddm, t1);
  t->info.condition.post=bdd_identity(bddm, post);
  t->s=0;
  return (t);
}


tr
#if defined(__STDC__)
tr_reduce(bdd_manager bddm, tr t1, bdd pre, bdd post)
#else
tr_reduce(bddm, t1, pre, post)
     bdd_manager bddm;
     tr t1;
     bdd pre;
     bdd post;
#endif
{
  tr t;

  t=(tr)mem_get_block((long)sizeof(struct tr_));
  t->ref_count=1;
  t->type=TR_REDUCE;
  t->info.reduce.pre=bdd_identity(bddm, pre);
  t->info.reduce.t=tr_copy(bddm, t1);
  t->info.reduce.post=bdd_identity(bddm, post);
  t->s=0;
  return (t);
}


static
void
#if defined(__STDC__)
free_step_stuff(bdd_manager bddm, step_stuff s)
#else
free_step_stuff(bddm, s)
     bdd_manager bddm;
     step_stuff s;
#endif
{
  pointer *p;

  switch (s->type)
    {
    case SS_BASIC:
      bdd_free_assoc(bddm, s->info.basic.c);
      bdd_free(bddm, s->info.basic.f);
      break;
    case SS_CONJUNCTIVE:
      bdd_free_assoc(bddm, s->info.conjunctive.ic);
      for (p=vec_elements(s->info.conjunctive.ss); *p; ++p)
	free_step_stuff(bddm, (step_stuff)*p);
      break;
    case SS_DISJUNCTIVE:
      for (p=vec_elements(s->info.disjunctive.ss); *p; ++p)
	free_step_stuff(bddm, (step_stuff)*p);
      break;
    case SS_CONDITION:
      bdd_free(bddm, s->info.condition.pre);
      bdd_free(bddm, s->info.condition.post);
      free_step_stuff(bddm, s->info.condition.s);
      break;
    case SS_REDUCE:
      bdd_free(bddm, s->info.reduce.post);
      free_step_stuff(bddm, s->info.reduce.s);
      break;
    }
}


void
#if defined(__STDC__)
tr_free(bdd_manager bddm, tr t)
#else
tr_free(bddm, t)
     bdd_manager bddm;
     tr t;
#endif
{
  pointer *p;

  if (--(t->ref_count))
    return;
  switch (t->type)
    {
    case TR_BASIC:
      bdd_free(bddm, t->info.basic.f);
      free_vec(t->info.basic.changes);
      free_vec(t->info.basic.uses);
      free_vec(t->info.basic.produces);
      break;
    case TR_CONJUNCTIVE:
      for (p=vec_elements(t->info.conjunctive.ts); *p; ++p)
	tr_free(bddm, (tr)*p);
      break;
    case TR_DISJUNCTIVE:
      for (p=vec_elements(t->info.conjunctive.ts); *p; ++p)
	tr_free(bddm, (tr)*p);
      break;
    case TR_CONDITION:
      bdd_free(bddm, t->info.condition.pre);
      bdd_free(bddm, t->info.condition.post);
      free_vec(t->info.condition.uses);
      free_vec(t->info.condition.produces);
      tr_free(bddm, t->info.condition.t);
      break;
    case TR_REDUCE:
      bdd_free(bddm, t->info.reduce.pre);
      bdd_free(bddm, t->info.reduce.post);
      tr_free(bddm, t->info.reduce.t);
      break;
    case TR_DUMMY:
      fatal("tr_free: illegal use of transition relation from tr_search");
    default:
      fatal("tr_free: unknown TR type");
    }
  if (t->s)
    free_step_stuff(bddm, t->s);
  mem_free_block((pointer)t);
}


static
tr
#if defined(__STDC__)
bdd_to_basic_tr(bdd_manager bddm, tr t, bdd t1, bdd (*vt_fn)(bdd_manager, bdd, int, pointer), pointer vt_env)
#else
bdd_to_basic_tr(bddm, t, t1, vt_fn, vt_env)
     bdd_manager bddm;
     tr t;
     bdd t1;
     bdd (*vt_fn)();
     pointer vt_env;
#endif
{
  int *changes;
  vec v;
  tr result;

  changes=(int *)mem_get_block((SIZE_T)(sizeof(int)*bdd_vars(bddm)));
  count_vars(bddm, t, changes, (int *)0, (int *)0);
  v=var_vec(bddm, changes);
  mem_free_block((pointer)changes);
  result=tr_basic(bddm, t1, (bdd *)vec_elements(v), vt_fn, vt_env);
  free_vec(v);
  bdd_free(bddm, t1);
  return (result);
}


static
int
#if defined(__STDC__)
tr_crunch_aux(bdd_manager bddm,
	      tr t,
	      long limit,
	      int keep_disj,
	      pointer *result,
	      bdd (*vt_fn)(bdd_manager, bdd, int, pointer),
	      pointer vt_env)
#else
tr_crunch_aux(bddm, t, limit, keep_disj, result, vt_fn, vt_env)
     bdd_manager bddm;
     tr t;
     long limit;
     int keep_disj;
     pointer *result;
     bdd (*vt_fn)();
     pointer vt_env;
#endif
{
  pointer *p, *q;
  vec v;
  int *changes;
  int *this_changes;
  bdd temp1, temp2, temp3;
  int all_small;

  switch (t->type)
    {
    case TR_BASIC:
      if (limit && bdd_size(bddm, t->info.basic.f, 1) > limit)
	{
	  *result=(pointer)tr_copy(bddm, t);
	  return (0);
	}
      *result=(pointer)bdd_identity(bddm, t->info.basic.f);
      return (1);
    case TR_CONJUNCTIVE:
      v=new_vec();
      all_small=1;
      for (p=vec_elements(t->info.conjunctive.ts); *p; ++p)
	{
	  if (!tr_crunch_aux(bddm, (tr)*p, limit, 0, result, vt_fn, vt_env))
	    {
	      if (all_small)
		for (p=vec_elements(t->info.conjunctive.ts), q=vec_elements(v); *q; ++p, ++q)
		  *q=(pointer)bdd_to_basic_tr(bddm, (tr)*p, (bdd)*q, vt_fn, vt_env);
	      all_small=0;
	    }
	  else if (!all_small)
	    *result=(pointer)bdd_to_basic_tr(bddm, (tr)*p, (bdd)*result, vt_fn, vt_env);
	  vec_add(v, *result);
	}
      temp1=0;
      if (all_small)
	{
	  temp1=bdd_one(bddm);
	  for (p=vec_elements(v); *p; ++p)
	    {
	      temp2=temp1;
	      temp1=bdd_and(bddm, temp2, (bdd)*p);
	      bdd_free(bddm, temp2);
	      if (!temp1 || (limit && bdd_size(bddm, temp1, 1) > limit))
		{
		  bdd_free(bddm, temp1);
		  break;
		}
	    }
	  if (*p)
	    {
	      for (p=vec_elements(t->info.conjunctive.ts), q=vec_elements(v); *p; ++p, ++q)
		*q=(pointer)bdd_to_basic_tr(bddm, (tr)*p, (bdd)*q, vt_fn, vt_env);
	      all_small=0;
	    }
	  else
	    for (p=vec_elements(v); *p; ++p)
	      bdd_free(bddm, (bdd)*p);
	}
      if (all_small)
	{
	  *result=(pointer)temp1;
	  free_vec(v);
	  return (1);
	}
      *result=(pointer)tr_conjunctive_multiple(bddm, (tr *)vec_elements(v));
      for (p=vec_elements(v); *p; ++p)
	tr_free(bddm, (tr)*p);
      free_vec(v);
      return (0);
      break;
    case TR_DISJUNCTIVE:
      v=new_vec();
      all_small=!keep_disj;
      for (p=vec_elements(t->info.disjunctive.ts); *p; ++p)
	{
	  if (!tr_crunch_aux(bddm, (tr)*p, limit, keep_disj, result, vt_fn, vt_env))
	    {
	      if (all_small)
		for (p=vec_elements(t->info.disjunctive.ts), q=vec_elements(v); *q; ++p, ++q)
		  *q=(pointer)bdd_to_basic_tr(bddm, (tr)*p, (bdd)*q, vt_fn, vt_env);
	      all_small=0;
	    }
	  else if (!all_small)
	    *result=(pointer)bdd_to_basic_tr(bddm, (tr)*p, (bdd)*result, vt_fn, vt_env);
	  vec_add(v, *result);
	}
      temp1=0;
      if (all_small)
	{
	  changes=(int *)mem_get_block((SIZE_T)(sizeof(int)*bdd_vars(bddm)));
	  for (p=vec_elements(t->info.disjunctive.ts); *p; ++p)
	    if (p == vec_elements(t->info.disjunctive.ts))
	      count_vars(bddm, (tr)*p, changes, (int *)0, (int *)0);
	    else
	      count_vars_aux2(bddm, (tr)*p, changes, (int *)0, (int *)0);
	  this_changes=(int *)mem_get_block((SIZE_T)(sizeof(int)*bdd_vars(bddm)));
	  temp1=bdd_zero(bddm);
	  for (p=vec_elements(t->info.disjunctive.ts), q=vec_elements(v); *p; ++p, ++q)
	    {
	      count_vars(bddm, (tr)*p, this_changes, (int *)0, (int *)0);
	      temp2=unchanging(bddm, this_changes, changes, vt_fn, vt_env);
	      temp3=temp2;
	      temp2=bdd_and(bddm, (bdd)*q, temp3);
	      bdd_free(bddm, temp3);
	      temp3=temp1;
	      temp1=bdd_or(bddm, temp2, temp3);
	      bdd_free(bddm, temp2);
	      bdd_free(bddm, temp3);
	      if (!temp1 || (limit && bdd_size(bddm, temp1, 1) > limit))
		{
		  bdd_free(bddm, temp1);
		  break;
		}
	    }
	  mem_free_block((pointer)this_changes);
	  mem_free_block((pointer)changes);
	  if (*p)
	    {
	      for (p=vec_elements(t->info.disjunctive.ts), q=vec_elements(v); *p; ++p, ++q)
		*q=(pointer)bdd_to_basic_tr(bddm, (tr)*p, (bdd)*q, vt_fn, vt_env);
	      all_small=0;
	    }
	  else
	    for (p=vec_elements(v); *p; ++p)
	      bdd_free(bddm, (bdd)*p);
	}
      if (all_small)
	{
	  *result=(pointer)temp1;
	  free_vec(v);
	  return (1);
	}
      *result=(pointer)tr_disjunctive_multiple(bddm, (tr *)vec_elements(v));
      for (p=vec_elements(v); *p; ++p)
	tr_free(bddm, (tr)*p);
      free_vec(v);
      return (0);
      break;
    case TR_CONDITION:
      if (tr_crunch_aux(bddm, t->info.condition.t, limit, 0, result, vt_fn, vt_env))
	{
	  temp1=bdd_and(bddm, (bdd)*result, t->info.condition.pre);
	  if (temp1 && (!limit || bdd_size(bddm, temp1, 1) <= limit))
	    {
	      temp2=primed_form(bddm, t->info.condition.post, vt_fn, vt_env);
	      temp3=temp1;
	      temp1=bdd_and(bddm, temp2, temp3);
	      bdd_free(bddm, temp2);
	      bdd_free(bddm, temp3);
	      if (temp1 && (!limit || bdd_size(bddm, temp1, 1) <= limit))
		{
		  *result=(pointer)temp1;
		  return (1);
		}
	    }
	  bdd_free(bddm, temp1);
	  *result=(pointer)bdd_to_basic_tr(bddm, t->info.condition.t, (bdd)*result, vt_fn, vt_env);
	}
      t=tr_condition(bddm, (tr)*result, t->info.condition.pre, t->info.condition.post, vt_fn, vt_env);
      tr_free(bddm, (tr)*result);
      *result=(pointer)t;
      return (0);
    case TR_REDUCE:
      if (tr_crunch_aux(bddm, t->info.reduce.t, limit, 0, result, vt_fn, vt_env))
	{
	  temp1=bdd_reduce(bddm, (bdd)*result, t->info.reduce.pre);
	  if (temp1 && bdd_size(bddm, temp1, 1) < bdd_size(bddm, (bdd)*result, 1))
	    {
	      bdd_free(bddm, (bdd)*result);
	      *result=(pointer)temp1;
	    }
	  else
	    bdd_free(bddm, temp1);
	  temp1=primed_form(bddm, t->info.reduce.post, vt_fn, vt_env);
	  temp2=temp1;
	  temp1=bdd_reduce(bddm, (bdd)*result, temp2);
	  bdd_free(bddm, temp2);
	  if (temp1 && bdd_size(bddm, temp1, 1) < bdd_size(bddm, (bdd)*result, 1))
	    {
	      bdd_free(bddm, (bdd)*result);
	      *result=(pointer)temp1;
	    }
	  else
	    bdd_free(bddm, temp1);
	  return (1);
	}
      t=tr_reduce(bddm, (tr)*result, t->info.reduce.pre, t->info.reduce.post);
      tr_free(bddm, (tr)*result);
      *result=(pointer)t;
      return (0); 
    default:
      fatal("tr_crunch_aux: unknown TR type");
      *result=0;
      return (0);
    }
}


tr
#if defined(__STDC__)
tr_crunch_limit(bdd_manager bddm,
		tr t,
		long limit,
		int keep_disj,
		bdd (*vt_fn)(bdd_manager, bdd, int, pointer),
		pointer vt_env)
#else
tr_crunch_limit(bddm, t, limit, keep_disj, vt_fn, vt_env)
     bdd_manager bddm;
     tr t;
     long limit;
     int keep_disj;
     bdd (*vt_fn)();
     pointer vt_env;
#endif
{
  pointer p;

  if (t->type == TR_DUMMY)
    fatal("tr_crunch_limit: illegal use of transition relation from tr_search");
  if (tr_crunch_aux(bddm, t, limit, keep_disj, &p, vt_fn, vt_env))
    return (bdd_to_basic_tr(bddm, t, (bdd)p, vt_fn, vt_env));
  return ((tr)p);
}


tr
#if defined(__STDC__)
tr_crunch(bdd_manager bddm, tr t, bdd (*vt_fn)(bdd_manager, bdd, int, pointer), pointer vt_env)
#else
tr_crunch(bddm, t, vt_fn, vt_env)
     bdd_manager bddm;
     tr t;
     bdd (*vt_fn)();
     pointer vt_env;
#endif
{
  return (tr_crunch_limit(bddm, t, 0l, 0, vt_fn, vt_env));
}


static
tr
#if defined(__STDC__)
tr_converse_aux(bdd_manager bddm,
		int id,
		tr t1,
		bdd (*vt_fn)(bdd_manager, bdd, int, pointer),
		pointer vt_env)
#else
tr_converse_aux(bddm, id, t1, changes, vt_fn, vt_env)
     bdd_manager bddm;
     int id;
     tr t1;
     bdd (*vt_fn)();
     pointer vt_env;
#endif
{
  bdd temp;
  tr t, t2;
  pointer *p;

  if (t1->type == TR_BASIC)
    {
      bdd_assoc(bddm, id);
      temp=bdd_substitute(bddm, t1->info.basic.f);
      t=tr_basic(bddm, temp, (bdd *)vec_elements(t1->info.basic.changes), vt_fn, vt_env);
      bdd_free(bddm, temp);
    }
  else if (t1->type == TR_CONDITION)
    {
      t2=tr_converse_aux(bddm, id, t1->info.condition.t, vt_fn, vt_env);
      t=tr_condition(bddm, t2, t1->info.condition.post, t1->info.condition.pre, vt_fn, vt_env);
      tr_free(bddm, t2);
    }
  else
    {
      t=(tr)mem_get_block((long)sizeof(struct tr_));
      t->ref_count=1;
      t->type=t1->type;
      switch (t1->type)
	{
	case TR_CONJUNCTIVE:
	  t->info.conjunctive.ts=new_vec();
	  for (p=vec_elements(t1->info.conjunctive.ts); *p; ++p)
	    vec_add(t->info.conjunctive.ts, (pointer)tr_converse_aux(bddm, id, (tr)*p, vt_fn, vt_env));
	  break;
	case TR_DISJUNCTIVE:
	  t->info.disjunctive.ts=new_vec();
	  for (p=vec_elements(t1->info.disjunctive.ts); *p; ++p)
	    vec_add(t->info.disjunctive.ts, (pointer)tr_converse_aux(bddm, id, (tr)*p, vt_fn, vt_env));
	  break;
	case TR_REDUCE:
	  t->info.reduce.pre=bdd_identity(bddm, t1->info.reduce.post);
	  t->info.reduce.post=bdd_identity(bddm, t1->info.reduce.pre);
	  t->info.reduce.t=tr_converse_aux(bddm, id, t1->info.reduce.t, vt_fn, vt_env);
	  break;
	default:
	  fatal("tr_converse_aux: unknown TR type");
	  t=0;
	}
    }
  t->s=0;
  return (t);
}


tr
#if defined(__STDC__)
tr_converse(bdd_manager bddm, tr t, bdd (*vt_fn)(bdd_manager, bdd, int, pointer), pointer vt_env)
#else
tr_converse(bddm, t, vt_fn, vt_env)
     bdd_manager bddm;
     tr t;
     bdd (*vt_fn)();
     pointer vt_env;
#endif
{
  int *changes;
  vec v;
  vec assoc;
  pointer *p;
  bdd temp;
  int id;
  tr result;

  if (t->type == TR_DUMMY)
    fatal("tr_converse: illegal use of transition relation from tr_search");
  changes=(int *)mem_get_block((SIZE_T)(sizeof(int)*bdd_vars(bddm)));
  count_vars(bddm, t, changes, (int *)0, (int *)0);
  v=var_vec(bddm, changes);
  mem_free_block((pointer)changes);
  assoc=new_vec();
  for (p=vec_elements(v); *p; ++p)
    {
      vec_add(assoc, *p);
      temp=(*vt_fn)(bddm, (bdd)*p, VT_PRIME, vt_env);
      vec_add(assoc, (pointer)temp);
      vec_add(assoc, (pointer)temp);
      vec_add(assoc, *p);
    }
  free_vec(v);
  vec_add(assoc, (pointer)0);
  id=bdd_new_assoc(bddm, (bdd *)vec_elements(assoc), 1);
  free_vec(assoc);
  result=tr_converse_aux(bddm, id, t, vt_fn, vt_env);
  bdd_free_assoc(bddm, id);
  return (result);
}


static
void
#if defined(__STDC__)
push_level(bdd_manager bddm, vec levels, step_stuff s, bdd reached, bdd frontier)
#else
push_level(bddm, levels, s, reached, frontier)
     bdd_manager bddm;
     vec levels;
     step_stuff s;
     bdd reached;
     bdd frontier;
#endif
{
  int i;
  ss_level ssl;

  ssl=(ss_level)mem_get_block((SIZE_T)sizeof(struct ss_level_));
  ssl->reached=bdd_identity(bddm, reached);
  ssl->s=s;
  ssl->i=0;
  ssl->frontiers=new_vec();
  if (ssl->s->type == SS_DISJUNCTIVE)
    i=vec_len(ssl->s->info.disjunctive.ss);
  else
    i=1;
  while (i)
    {
      vec_add(ssl->frontiers, (pointer)bdd_identity(bddm, frontier));
      --i;
    }
  vec_add(levels, (pointer)ssl);
}


static
void
#if defined(__STDC__)
descend(bdd_manager bddm, search_state ss, bdd frontier)
#else
descend(bddm, ss, frontier)
     bdd_manager bddm;
     search_state ss;
     bdd frontier;
#endif
{
  ss_level ssl;

  while ((ssl=(ss_level)vec_last(ss->levels))->s->type == SS_DISJUNCTIVE)
    push_level(bddm,
	       ss->levels,
	       (step_stuff)vec_elements(ssl->s->info.disjunctive.ss)[ssl->i],
	       ssl->reached,
	       frontier);
}


search_state
#if defined(__STDC__)
tr_search_init(bdd_manager bddm, tr t, bdd init, bdd (*vt_fn)(bdd_manager, bdd, int, pointer), pointer vt_env)
#else
tr_search_init(bddm, t, init, vt_fn, vt_env)
     bdd_manager bddm;
     tr t;
     bdd init;
     bdd (*vt_fn)();
     pointer vt_env;
#endif
{
  search_state result;

  result=(search_state)mem_get_block((SIZE_T)sizeof(struct search_state_));
  result->t=tr_copy(bddm, t);
  compute_step_stuff(bddm, t, vt_fn, vt_env);
  result->dummy=(tr)mem_get_block((SIZE_T)sizeof(struct tr_));
  result->dummy->type=TR_DUMMY;
  result->dummy->ns_cs=t->ns_cs;
  result->fixpoint=0;
  result->reached=0;
  result->levels=new_vec();
  push_level(bddm, result->levels, t->s, init, init);
  descend(bddm, result, init);
  return (result);
}


int
#if defined(__STDC__)
tr_search_next(bdd_manager bddm, search_state ss, tr *t, bdd *reached, bdd *frontier)
#else
tr_search_next(bddm, ss, t, reached, frontier)
     bdd_manager bddm;
     search_state ss;
     tr *t;
     bdd *reached;
     bdd *frontier;
#endif
{
  ss_level ssl;
  bdd temp1, temp2;

  if (ss->fixpoint)
    {
      *t=0;
      *reached=bdd_identity(bddm, ss->reached);
      *frontier=0;
      return (1);
    }
  ssl=(ss_level)vec_last(ss->levels);
  *t=ss->dummy;
  ss->dummy->s=ssl->s;
  *reached=bdd_identity(bddm, ssl->reached);
  temp1=bdd_not(bddm, *reached);
  temp2=temp1;
  temp1=bdd_or(bddm, (bdd)vec_elements(ssl->frontiers)[ssl->i], temp2);
  bdd_free(bddm, temp2);
  *frontier=bdd_reduce(bddm, (bdd)vec_elements(ssl->frontiers)[ssl->i], temp1);
  bdd_free(bddm, temp1);
  return (0);
}


void
#if defined(__STDC__)
tr_search_step(bdd_manager bddm, search_state ss, bdd step)
#else
tr_search_step(bddm, ss, step)
     bdd_manager bddm;
     search_state ss;
     bdd step;
#endif
{
  int i;
  ss_level ssl;
  bdd new_states;
  bdd temp;

  step=bdd_identity(bddm, step);
  while ((ssl=(ss_level)vec_last(ss->levels)))
    {
      bdd_free(bddm, (bdd)vec_elements(ssl->frontiers)[ssl->i]);
      temp=bdd_not(bddm, ssl->reached);
      new_states=bdd_and(bddm, temp, step);
      bdd_free(bddm, temp);
      temp=ssl->reached;
      ssl->reached=bdd_or(bddm, temp, step);
      bdd_free(bddm, temp);
      bdd_free(bddm, step);
      vec_elements(ssl->frontiers)[ssl->i]=(pointer)bdd_zero(bddm);
      for (i=vec_len(ssl->frontiers); i; --i)
	if (ssl->s->type != SS_DISJUNCTIVE || i-1 != ssl->i)
	  {
	    temp=(bdd)vec_elements(ssl->frontiers)[i-1];
	    vec_elements(ssl->frontiers)[i-1]=(pointer)bdd_or(bddm, temp, new_states);
	    bdd_free(bddm, temp);
	  }
      bdd_free(bddm, new_states);
      ssl->i++;
      if (ssl->i >= vec_len(ssl->frontiers))
	ssl->i=0;
      if ((bdd)vec_elements(ssl->frontiers)[ssl->i] != bdd_zero(bddm))
	{
	  descend(bddm, ss, (bdd)vec_elements(ssl->frontiers)[ssl->i]);
	  return;
	}
      step=bdd_identity(bddm, ssl->reached);
      bdd_free(bddm, ssl->reached);
      free_vec(ssl->frontiers);
      mem_free_block((pointer)ssl);
      vec_del(ss->levels);
    }
  ss->reached=step;
  ss->fixpoint=1;
}


void
#if defined(__STDC__)
tr_search_quit(bdd_manager bddm, search_state ss)
#else
tr_search_quit(bddm, ss)
     bdd_manager bddm;
     search_state ss;
#endif
{
  ss_level ssl;
  pointer *p, *q;

  tr_free(bddm, ss->t);
  mem_free_block((pointer)ss->dummy);
  if (ss->reached)
    bdd_free(bddm, ss->reached);
  for (p=vec_elements(ss->levels); *p; ++p)
    {
      ssl=(ss_level)*p;
      bdd_free(bddm, ssl->reached);
      for (q=vec_elements(ssl->frontiers); *q; ++q)
	bdd_free(bddm, (bdd)*q);
      free_vec(ssl->frontiers);
    }
  free_vec(ss->levels);
  mem_free_block((pointer)ss);
}


static
long
#if defined(__STDC__)
cp_score(reorder_rec *rrs, long vars, int *cs, int *ns)
#else
cp_score(rrs, vars, cs, ns)
     reorder_rec *rrs;
     long vars;
     int *cs;
     int *ns;
#endif
{
  int i;
  long j;
  long score;
  reorder_rec rr;
  long *p;

  score=vars;
  for (i=0; rrs[i]; ++i)
    {
      rr=rrs[i];
      for (p=(long *)vec_elements(rr->uses), j=0; p[j]; ++j)
	if (!--cs[p[j]-1])
	  --vars;
      for (p=(long *)vec_elements(rr->produces), j=0; p[j]; ++j)
	if (!ns[p[j]-1]++)
	  ++vars;
      score+=vars;
    }
  for (i=0; rrs[i]; ++i)
    {
      rr=rrs[i];
      for (p=(long *)vec_elements(rr->uses), j=0; p[j]; ++j)
	++cs[p[j]-1];
      for (p=(long *)vec_elements(rr->produces), j=0; p[j]; ++j)
	--ns[p[j]-1];
    }
  return (score);
}


static
long
#if defined(__STDC__)
cp_score_delta(reorder_rec rr, int *cs, int *ns, int update)
#else
cp_score_delta(rr, cs, ns, update)
     reorder_rec rr;
     int *cs;
     int *ns;
     int update;
#endif
{
  long i;
  long delta;
  long *p;

  delta=0;
  for (p=(long *)vec_elements(rr->uses), i=0; p[i]; ++i)
    {
      if (cs[p[i]-1] == 1)
	--delta;
      if (update)
	--cs[p[i]-1];
    }
  for (p=(long *)vec_elements(rr->produces), i=0; p[i]; ++i)
    {
      if (!ns[p[i]-1])
	++delta;
      if (update)
	++ns[p[i]-1];
    }
  return (delta);
}


static
int
#if defined(__STDC__)
cp_sift(bdd_manager bddm, reorder_rec *rrs, int i, long vars, int *cs, int *ns)
#else
cp_sift(bddm, rrs, i, vars, cs, ns)
     bdd_manager bddm;
     reorder_rec *rrs;
     int i;
     long vars;
     int *cs;
     int *ns;
#endif
{
  int j;
  long k;
  int best_pos;
  int best_score;
  long score;
  reorder_rec rr;
  long *p;

  best_pos=i;
  best_score=cp_score(rrs, vars, cs, ns);
  rr=rrs[i];
  for (j=i; j; --j)
    rrs[j]=rrs[j-1];
  rrs[0]=rr;
  j=0;
  score=cp_score(rrs, vars, cs, ns);
  while (1)
    {
      if (score < best_score)
	{
	  best_pos=j;
	  best_score=score;
	}
      if (!rrs[j+1])
	break;
      score-=cp_score_delta(rr, cs, ns, 0);
      score+=cp_score_delta(rrs[j+1], cs, ns, 1);
      rrs[j]=rrs[j+1];
      ++j;
      rrs[j]=rr;
    }
  while (j != best_pos)
    {
      rrs[j]=rrs[j-1];
      --j;
    }
  rrs[best_pos]=rr;
  for (j=0; rrs[j]; ++j)
    if (j != best_pos)
      {
	rr=rrs[j];
	for (p=(long *)vec_elements(rr->uses), k=0; p[k]; ++k)
	  ++cs[p[k]-1];
	for (p=(long *)vec_elements(rr->produces), k=0; p[k]; ++k)
	  --ns[p[k]-1];
      }
  return (best_pos != i);
}


static
void
#if defined(__STDC__)
cp_sift_stable(bdd_manager bddm, reorder_rec *rrs, reorder_rec *sift_array, int vars, int *cs, int *ns)
#else
cp_sift_stable(bddm, rrs, sift_array, vars, cs, ns)
     bdd_manager bddm;
     reorder_rec *rrs;
     reorder_rec *sift_array;
     int vars;
     int *cs;
     int *ns;
#endif
{
  int i, j;
  int any_moved;

  do
    {
      any_moved=0;
      for (i=0; rrs[i]; ++i)
	sift_array[i]=rrs[i];
      sift_array[i]=0;
      for (i=0; sift_array[i]; ++i)
	{
	  for (j=0; sift_array[i] != rrs[j]; ++j);
	  if (cp_sift(bddm, rrs, j, vars, cs, ns))
	    any_moved=1;
	}
    }
  while (any_moved);
}


static
vec
#if defined(__STDC__)
pack(bdd_manager bddm, int *vars)
#else
pack(bddm, vars)
     bdd_manager bddm;
     int *vars;
#endif
{
  vec v;
  long i;

  v=new_vec();
  for (i=bdd_vars(bddm); i; --i)
    if (vars[i-1])
      vec_add(v, (pointer)i);
  return (v);
}


static
void
#if defined(__STDC__)
cp_reorder(bdd_manager bddm, vec ts, int *changes)
#else
cp_reorder(bddm, ts, changes)
     bdd_manager bddm;
     vec ts;
     int *changes;
#endif
{
  long i;
  long vars;
  int *cs, *ns;
  reorder_rec *rrs;
  reorder_rec *sift_array;
  rec_mgr rr_mgr;
  pointer *p;

  cs=(int *)mem_get_block(sizeof(int)*bdd_vars(bddm));
  ns=(int *)mem_get_block(sizeof(int)*bdd_vars(bddm));
  rrs=(reorder_rec *)mem_get_block((SIZE_T)(sizeof(reorder_rec)*(vec_len(ts)+1)));
  rr_mgr=mem_new_rec_mgr(sizeof(struct reorder_rec_));
  for (i=0, p=vec_elements(ts); *p; ++i, ++p)
    {
      rrs[i]=(reorder_rec)mem_new_rec(rr_mgr);
      rrs[i]->t=(tr)*p;
      count_vars(bddm, (tr)*p, (int *)0, cs, ns);
      rrs[i]->uses=pack(bddm, cs);
      rrs[i]->produces=pack(bddm, ns);
    }
  rrs[i]=0;
  for (p=vec_elements(ts); *p; ++p)
    if (p == vec_elements(ts))
      count_vars(bddm, (tr)*p, (int *)0, cs, (int *)0);
    else
      count_vars_aux2(bddm, (tr)*p, (int *)0, cs, (int *)0);
  vars=0;
  for (i=bdd_vars(bddm); i ; --i)
    {
      ns[i-1]=0;
      if (!changes[i-1])
	cs[i-1]=0;
      else if (cs[i-1])
	++vars;
    }
  sift_array=(reorder_rec *)mem_get_block(sizeof(reorder_rec)*(vec_len(ts)+1));
  cp_sift_stable(bddm, rrs, sift_array, vars, cs, ns);
  for (i=vec_len(ts); i; --i)
    vec_elements(ts)[i-1]=(pointer)rrs[i-1]->t;
  mem_free_block((pointer)cs);
  mem_free_block((pointer)ns);
  mem_free_block((pointer)rrs);
  mem_free_block((pointer)sift_array);
  mem_free_rec_mgr(rr_mgr);
}


tr
#if defined(__STDC__)
tr_conj_part_reorder(bdd_manager bddm, tr t1)
#else
tr_conj_part_reorder(bddm, t1)
     bdd_manager bddm;
     tr t1;
#endif
{
  vec ts;
  pointer *p;
  tr t;
  int *changes;

  if (t1->type == TR_BASIC)
    return (tr_copy(bddm, t1));
  t=(tr)mem_get_block((SIZE_T)sizeof(struct tr_));
  t->ref_count=1;
  t->type=t1->type;
  switch (t1->type)
    {
    case TR_CONJUNCTIVE:
      ts=new_vec();
      frontier_trs(TR_CONJUNCTIVE, t1, ts);
      for (p=vec_elements(ts); *p; ++p)
	*p=(pointer)tr_conj_part_reorder(bddm, (tr)*p);
      changes=(int *)mem_get_block((SIZE_T)(sizeof(int)*bdd_vars(bddm)));
      count_vars(bddm, t1, changes, (int *)0, (int *)0);
      cp_reorder(bddm, ts, changes);
      mem_free_block((pointer)changes);
      t->info.conjunctive.ts=ts;
      break;
    case TR_DISJUNCTIVE:
      ts=new_vec();
      for (p=vec_elements(t1->info.disjunctive.ts); *p; ++p)
	vec_add(ts, (pointer)tr_conj_part_reorder(bddm, (tr)*p));
      t->info.disjunctive.ts=ts;
      break;
    case TR_CONDITION:
      t->info.condition.uses=vec_copy(t1->info.condition.uses);
      t->info.condition.produces=vec_copy(t1->info.condition.produces);
      t->info.condition.pre=bdd_identity(bddm, t1->info.condition.pre);
      t->info.condition.post=bdd_identity(bddm, t1->info.condition.post);
      t->info.condition.t=tr_conj_part_reorder(bddm, t1->info.condition.t);
      break;
    case TR_REDUCE:
      t->info.reduce.pre=bdd_identity(bddm, t1->info.reduce.pre);
      t->info.reduce.post=bdd_identity(bddm, t1->info.reduce.post);
      t->info.reduce.t=tr_conj_part_reorder(bddm, t1->info.reduce.t);
    case TR_DUMMY:
      fatal("tr_conj_part_reorder: illegal use of TR from tr_search");
    default:
      fatal("tr_conj_part_reorder: unknown TR type");
    }
  t->s=0;
  return (t);
}


char *
#if defined(__STDC__)
tr_version(void)
#else
tr_version()
#endif
{
  return ("1.0");
}
