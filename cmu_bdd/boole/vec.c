#include <memuser.h>
#include "vec.h"


struct vec_
{
  SIZE_T size;
  SIZE_T max_size;
  pointer *elements;
};


vec
#if defined(__STDC__)
new_vec(void)
#else
new_vec()
#endif
{
  vec result;

  result=(vec)mem_get_block((SIZE_T)sizeof(struct vec_));
  result->size=0;
  result->max_size=8;
  result->elements=(pointer *)mem_get_block((SIZE_T)(sizeof(pointer)*(result->max_size+1)));
  result->elements[0]=0;
  return (result);
}


void
#if defined(__STDC__)
free_vec(vec v)
#else
free_vec(v)
     vec v;
#endif
{
  mem_free_block((pointer)v->elements);
  mem_free_block((pointer)v);
}


pointer *
#if defined(__STDC__)
vec_elements(vec v)
#else
vec_elements(v)
     vec v;
#endif
{
  return (v->elements);
}


pointer
#if defined(__STDC__)
vec_add(vec v, pointer e)
#else
vec_add(v, e)
     vec v;
     pointer e;
#endif
{
  if (v->size == v->max_size)
    {
      v->max_size*=2;
      v->elements=(pointer *)mem_resize_block(v->elements, (SIZE_T)(sizeof(pointer)*(v->max_size+1)));
    }
  v->elements[v->size]=e;
  v->size++;
  v->elements[v->size]=0;
  return (e);
}


pointer
#if defined(__STDC__)
vec_del(vec v)
#else
vec_del(v)
     vec v;
#endif
{
  pointer e;

  v->size--;
  e=v->elements[v->size];
  v->elements[v->size]=0;
  return (e);
}


SIZE_T
#if defined(__STDC__)
vec_len(vec v)
#else
vec_len(v)
     vec v;
#endif
{
  return (v->size);
}
