#if !defined(VECH_)
#define VECH_


#if defined(__STDC__)
#define ARGS(args) args
#else
#define ARGS(args) ()
#endif


typedef struct vec_ *vec;


extern vec new_vec ARGS((void));
extern void free_vec ARGS((vec));
extern pointer *vec_elements ARGS((vec));
extern pointer vec_add ARGS((vec, pointer));
extern pointer vec_del ARGS((vec));
extern SIZE_T vec_len ARGS((vec));


#undef ARGS


#endif
