/* Hash table routines */


#include <memuser.h>
#include "hash.h"


struct hash_rec_
{
  pointer key;
  long hash;
  pointer data;
  struct hash_rec_ *next;
};

typedef struct hash_rec_ *hash_rec;


struct hash_table_
{
#if defined(__STDC__)
  long (*hash_fn)(pointer);
  int (*compare_fn)(pointer, pointer);
#else
  long (*hash_fn)();
  int (*compare_fn)();
#endif
  hash_rec *table;
  int size_index;
  long entries;
};


static long primes[]=
{
  1,
  2,
  3,
  7,
  13,
  23,
  59,
  113,
  241,
  503,
  1019,
  2039,
  4091,
  8179,
  16369,
  32749,
  65521,
  131063,
  262139,
  524269,
  1048571,
  2097143,
  4194287,
  8388593,
  16777199,
  33554393,
  67108859,
  134217689,
  268435399,
  536870879,
  1073741789,
  2147483629
};


#define SIZE(h) (primes[(h)->size_index])


static
long
#if defined(__STDC__)
pointer_hash(pointer p)
#else
pointer_hash(p)
     pointer p;
#endif
{
  return ((long)p);
}


static
int
#if defined(__STDC__)
pointer_compare(pointer p1, pointer p2)
#else
pointer_compare(p1, p2)
     pointer p1;
     pointer p2;
#endif
{
  return (p1 == p2);
}


static
long
#if defined(__STDC__)
string_hash(pointer s)
#else
string_hash(s)
     pointer s;
#endif
{
  char *s1;
  long hash;

  s1=(char *)s;
  hash=0;
  while (*s1)
    {
      hash=((hash << 5) | (hash >> (8*sizeof(long)-5))) ^ *s1;
      ++s1;
    }
  return (hash);
}


static
int
#if defined(__STDC__)
string_compare(pointer s1, pointer s2)
#else
string_compare(s1, s2)
     pointer s1;
     pointer s2;
#endif
{
  return (strcmp(s1, s2) == 0);
}


static
void
#if defined(__STDC__)
rehash(hash_table h)
#else
rehash(h)
     hash_table h;
#endif
{
  long i;
  long oldsize;
  long pos;
  hash_rec *newtable;
  hash_rec p, q;

  oldsize=SIZE(h);
  h->size_index++;
  newtable=(hash_rec *)mem_get_block(SIZE(h)*sizeof(hash_rec));
  for (i=0; i < SIZE(h); ++i)
    newtable[i]=0;
  for (i=0; i < oldsize; ++i)
    for (p=h->table[i]; p; p=q)
      {
	q=p->next;
	pos=p->hash%SIZE(h);
	if (pos < 0)
	  pos= -pos;
	p->next=newtable[pos];
	newtable[pos]=p;
      }
  mem_free_block((pointer)h->table);
  h->table=newtable;
}


void
#if defined(__STDC__)
hash_insert(hash_table h, pointer key, pointer data)
#else
hash_insert(h, key, data)
     hash_table h;
     pointer key;
     pointer data;
#endif
{
  long pos;
  hash_rec p;

  p=(hash_rec)mem_get_block((long)sizeof(struct hash_rec_));
  p->key=key;
  p->hash=(*h->hash_fn)(key);
  p->data=data;
  pos=p->hash%SIZE(h);
  if (pos < 0)
    pos= -pos;
  p->next=h->table[pos];
  h->table[pos]=p;
  h->entries++;
  if (h->entries > 4*SIZE(h))
    rehash(h);
}


int
#if defined(__STDC__)
hash_delete(hash_table h, pointer key)
#else
hash_delete(h, key)
     hash_table h;
     pointer key;
#endif
{
  long hash;
  long pos;
  hash_rec *p, q;

  hash=(*h->hash_fn)(key);
  pos=hash%SIZE(h);
  if (pos < 0)
    pos= -pos;
  for (p= &h->table[pos]; *p; p= &(*p)->next)
    if ((*p)->hash == hash && (*h->compare_fn)(key, (*p)->key))
      break;
  if (*p)
    {
      q= *p;
      *p=q->next;
      mem_free_block((pointer)q);
      h->entries--;
      return (1);
    }
  return (0);
}


pointer
#if defined(__STDC__)
hash_lookup(hash_table h, pointer s)
#else
hash_lookup(h, s)
     hash_table h;
     pointer s;
#endif
{
  long hash;
  long pos;
  hash_rec p;

  hash=(*h->hash_fn)(s);
  pos=hash%SIZE(h);
  if (pos < 0)
    pos= -pos;
  for (p=h->table[pos]; p; p=p->next)
    if (p->hash == hash && (*h->compare_fn)(s, p->key))
      return (p->data);
  return ((pointer)0);
}


hash_table
#if defined(__STDC__)
new_generic_hash_table(long (*hash_fn)(pointer), int (*compare_fn)(pointer, pointer))
#else
new_generic_hash_table(hash_fn, compare_fn)
     long (*hash_fn)();
     int (*compare_fn)();
#endif
{
  long i;
  hash_table h;

  h=(hash_table)mem_get_block((long)sizeof(struct hash_table_));
  h->hash_fn=hash_fn;
  h->compare_fn=compare_fn;
  h->size_index=4;
  h->table=(hash_rec *)mem_get_block(SIZE(h)*sizeof(hash_rec));
  for (i=0; i < SIZE(h); ++i)
    h->table[i]=0;
  h->entries=0;
  return (h);
}


hash_table
#if defined(__STDC__)
new_hash_table(void)
#else
new_hash_table()
#endif
{
  return (new_generic_hash_table(string_hash, string_compare));
}


hash_table
#if defined(__STDC__)
new_pointer_hash_table(void)
#else
new_pointer_hash_table()
#endif
{
  return (new_generic_hash_table(pointer_hash, pointer_compare));
}


void
#if defined(__STDC__)
free_hash_table(hash_table h)
#else
free_hash_table(h)
     hash_table h;
#endif
{
  long i;
  hash_rec p, q;

  for (i=0; i < SIZE(h); ++i)
    for (p=h->table[i]; p; p=q)
      {
	q=p->next;
	mem_free_block((pointer)p);
      }
  mem_free_block((pointer)h->table);
  mem_free_block((pointer)h);
}


void
#if defined(__STDC__)
hash_table_walk(hash_table h, void (*f)(pointer, pointer, pointer), pointer env)
#else
hash_table_walk(h, f, env)
     hash_table h;
     void (*f)();
     pointer env;
#endif
{
  long i;
  hash_rec p;

  for (i=0; i < SIZE(h); ++i)
    for (p=h->table[i]; p; p=p->next)
      (*f)(p->key, p->data, env);
}
