#if !defined(HASHH_)
#define HASHH_


#if defined(__STDC__)
#define ARGS(args) args
#else
#define ARGS(args) ()
#endif


/* Hash tables */

typedef struct hash_table_ *hash_table;


extern void hash_insert ARGS((hash_table, pointer, pointer));
extern int hash_delete ARGS((hash_table, pointer));
extern pointer hash_lookup ARGS((hash_table, pointer));
extern hash_table new_generic_hash_table ARGS((long (*)(pointer), int (*)(pointer, pointer)));
extern hash_table new_hash_table ARGS((void));
extern hash_table new_pointer_hash_table ARGS((void));
extern void free_hash_table ARGS((hash_table));
extern void hash_table_walk ARGS((hash_table, void (*)(pointer, pointer, pointer), pointer));


#undef ARGS


#endif
