/* BDD satisfying valuation routines */


#include "bddint.h"


#if defined(__STDC__)
extern void qsort(pointer, unsigned, int, int (*)(pointer, pointer));
#else
extern void qsort();
#endif


static
bdd
#if defined(__STDC__)
bdd_satisfy_step(bdd_manager bddm, bdd f)
#else
bdd_satisfy_step(bddm, f)
     bdd_manager bddm;
     bdd f;
#endif
{
  bdd temp;
  bdd result;

  BDD_SETUP(f);
  if (BDD_IS_CONST(f))
    return (f);
  if (BDD_THEN(f) == BDD_ZERO(bddm))
    {
      temp=bdd_satisfy_step(bddm, BDD_ELSE(f));
      result=bdd_find(bddm, BDD_INDEXINDEX(f), BDD_ZERO(bddm), temp);
    }
  else
    {
      temp=bdd_satisfy_step(bddm, BDD_THEN(f));
      result=bdd_find(bddm, BDD_INDEXINDEX(f), temp, BDD_ZERO(bddm));
    }
  return (result);
}


/* bdd_satisfy(bddm, f) returns a BDD which implies f, is true for */
/* some valuation on which f is true, and which has at most one node */
/* at each level. */

bdd
#if defined(__STDC__)
bdd_satisfy(bdd_manager bddm, bdd f)
#else
bdd_satisfy(bddm, f)
     bdd_manager bddm;
     bdd f;
#endif
{
  if (bdd_check_arguments(1, f))
    {
      if (f == BDD_ZERO(bddm))
	{
	  bdd_warning("bdd_satisfy: argument is false");
	  return (f);
	}
      FIREWALL(bddm);
      RETURN_BDD(bdd_satisfy_step(bddm, f));
    }
  return ((bdd)0);
}


static
bdd
#if defined(__STDC__)
bdd_satisfy_support_step(bdd_manager bddm, bdd f, bdd_indexindex_type *support)
#else
bdd_satisfy_support_step(bddm, f, support)
     bdd_manager bddm;
     bdd f;
     bdd_indexindex_type *support;
#endif
{
  bdd temp;
  bdd result;

  BDD_SETUP(f);
  if (!*support)
    return (bdd_satisfy_step(bddm, f));
  if (BDD_INDEX(bddm, f) <= bddm->indexes[*support])
    {
      if (BDD_INDEXINDEX(f) == *support)
	++support;
      if (BDD_THEN(f) == BDD_ZERO(bddm))
	{
	  temp=bdd_satisfy_support_step(bddm, BDD_ELSE(f), support);
	  result=bdd_find(bddm, BDD_INDEXINDEX(f), BDD_ZERO(bddm), temp);
	}
      else
	{
	  temp=bdd_satisfy_support_step(bddm, BDD_THEN(f), support);
	  result=bdd_find(bddm, BDD_INDEXINDEX(f), temp, BDD_ZERO(bddm));
	}
    }
  else
    {
      temp=bdd_satisfy_support_step(bddm, f, support+1);
      result=bdd_find(bddm, *support, BDD_ZERO(bddm), temp);
    }
  return (result);
}


static
int
#if defined(__STDC__)
index_cmp(pointer p1, pointer p2)
#else
index_cmp(p1, p2)
     pointer p1;
     pointer p2;
#endif
{
  bdd_index_type i1, i2;

  i1= *(bdd_indexindex_type *)p1;
  i2= *(bdd_indexindex_type *)p2;
  if (i1 < i2)
    return (-1);
  if (i1 > i2)
    return (1);
  return (0);
}


/* bdd_satisfy_support(bddm, f) returns a BDD which implies f, is true */
/* for some valuation on which f is true, which has at most one node */
/* at each level, and which has exactly one node corresponding to each */
/* variable which is associated with something in the current variable */
/* association. */

bdd
#if defined(__STDC__)
bdd_satisfy_support(bdd_manager bddm, bdd f)
#else
bdd_satisfy_support(bddm, f)
     bdd_manager bddm;
     bdd f;
#endif
{
  bdd_indexindex_type *support, *p;
  long i;
  bdd result;

  if (bdd_check_arguments(1, f))
    {
      if (f == BDD_ZERO(bddm))
	{
	  bdd_warning("bdd_satisfy_support: argument is false");
	  return (f);
	}
      support=(bdd_indexindex_type *)mem_get_block((bddm->vars+1)*sizeof(bdd));
      FIREWALL1(bddm,
		if (retcode == BDD_ABORTED || retcode == BDD_OVERFLOWED)
		  {
		    mem_free_block((pointer)support);
		    return ((bdd)0);
		  }
		);
      for (i=0, p=support; i < bddm->vars; ++i)
	if (bddm->curr_assoc->assoc[i+1])
	  {
	    *p=bddm->indexes[i+1];
	    ++p;
	  }
      *p=0;
      qsort((pointer)support, (unsigned)(p-support), sizeof(bdd_indexindex_type), index_cmp);
      while (p != support)
	{
	  --p;
	  *p=bddm->indexindexes[*p];
	}
      result=bdd_satisfy_support_step(bddm, f, support);
      mem_free_block((pointer)support);
      RETURN_BDD(result);
    }
  return ((bdd)0);
}


double
#if defined(__STDC__)
bdd_satisfying_fraction_step(bdd_manager bddm, bdd f)
#else
bdd_satisfying_fraction_step(bddm, f)
     bdd_manager bddm;
     bdd f;
#endif
{
  long cache_result[2];
  double result;

  BDD_SETUP(f);
  if (BDD_IS_CONST(f))
    {
      if (f == BDD_ZERO(bddm))
	return (0.0);
      return (1.0);
    }
  if (bdd_lookup_in_cache1d(bddm, OP_SATFRAC, f, &cache_result[0], &cache_result[1]))
    {
      result= *((double *)cache_result);
      return (result);
    }
  result=0.5*bdd_satisfying_fraction_step(bddm, BDD_THEN(f))+
    0.5*bdd_satisfying_fraction_step(bddm, BDD_ELSE(f));
  *((double *)cache_result)=result;
  bdd_insert_in_cache1d(bddm, OP_SATFRAC, f, cache_result[0], cache_result[1]);
  return (result);
}


/* bdd_satisfying_fraction(bddm, f) returns the fraction of valuations */
/* which make f true.  (Note that this fraction is independent of */
/* whatever set of variables f is supposed to be a function of.) */

double
#if defined(__STDC__)
bdd_satisfying_fraction(bdd_manager bddm, bdd f)
#else
bdd_satisfying_fraction(bddm, f)
     bdd_manager bddm;
     bdd f;
#endif
{
  if (bdd_check_arguments(1, f))
    return (bdd_satisfying_fraction_step(bddm, f));
  return (0.0);
}
