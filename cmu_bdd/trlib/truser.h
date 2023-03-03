#if !defined(TRH_)
#define TRH_


#include <bdduser.h>


#if defined(__STDC__)
#define ARGS(args) args
#else
#define ARGS(args) ()
#endif


typedef struct tr_ *tr;
typedef struct search_state_ *search_state;


#define VT_NORMAL 0
#define VT_PRIME 1


extern char *tr_version ARGS((void));
extern bdd tr_step ARGS((bdd_manager, tr, bdd, bdd (*) ARGS((bdd_manager, bdd, int, pointer)), pointer));
extern bdd tr_reduce_step ARGS((bdd_manager, tr, bdd, bdd, bdd, bdd (*) ARGS((bdd_manager, bdd, int, pointer)), pointer));
extern bdd tr_reduce_step_multiple ARGS((bdd_manager,
					 tr,
					 bdd,
					 bdd *,
					 bdd *,
					 bdd (*) ARGS((bdd_manager, bdd, int, pointer)),
					 pointer));
extern tr tr_copy ARGS((bdd_manager, tr));
extern tr tr_basic ARGS((bdd_manager, bdd, bdd *, bdd (*) ARGS((bdd_manager, bdd, int, pointer)), pointer));
extern tr tr_conjunctive_multiple ARGS((bdd_manager, tr *));
extern tr tr_conjunctive ARGS((bdd_manager, tr, tr));
extern tr tr_disjunctive_multiple ARGS((bdd_manager, tr *));
extern tr tr_disjunctive ARGS((bdd_manager, tr, tr));
extern tr tr_condition ARGS((bdd_manager, tr, bdd, bdd, bdd (*) ARGS((bdd_manager, bdd, int, pointer)), pointer));
extern tr tr_reduce ARGS((bdd_manager, tr, bdd, bdd));
extern tr tr_crunch ARGS((bdd_manager, tr, bdd (*) ARGS((bdd_manager, bdd, int, pointer)), pointer));
extern tr tr_crunch_limit ARGS((bdd_manager, tr, long, int, bdd (*) ARGS((bdd_manager, bdd, int, pointer)), pointer));
extern tr tr_converse ARGS((bdd_manager, tr, bdd (*) ARGS((bdd_manager, bdd, int, pointer)), pointer));
extern void tr_free ARGS((bdd_manager, tr));
extern search_state tr_search_init ARGS((bdd_manager, tr, bdd, bdd (*)ARGS((bdd_manager, bdd, int, pointer)), pointer));
extern int tr_search_next ARGS((bdd_manager, search_state, tr *, bdd *, bdd *));
extern void tr_search_step ARGS((bdd_manager, search_state, bdd));
extern void tr_search_quit ARGS((bdd_manager, search_state));
extern tr tr_conj_part_reorder ARGS((bdd_manager, tr));


#undef ARGS


#endif
