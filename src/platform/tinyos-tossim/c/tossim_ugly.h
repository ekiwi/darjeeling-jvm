#ifndef __TOSSIM_UGLY_H__
#define __TOSSIM_UGLY_H__
//this header file is included in addition to tossim.h
//in order to resolve the cyclic dependancy between
//types.h, pointerwidth.h and tossim.h
//types.h includes pointerwidth.h which has a global variable (ref_t_base_address)
//so it wanted to include tossim.h for that, and tossim.h needed types.h
//in order to be able to define tossim_global_variables struct,
//Our solution in here is the most straightforward one,
//We will be more than happy to see a nicer solution,
#define _global_ref_t_base_address (getUglyGlobalVariables()->ref_t_base_address)

struct tossim_UGLY_global_variables{
	//------------------------------------------------
	//from darjeeling.c
	char *ref_t_base_address;
};
void* tossim_getDarjeelingUglyGlobals();
void tossim_setDarjeelingUglyGlobals(void *global_variables);

static inline struct tossim_UGLY_global_variables* getUglyGlobalVariables(){
	return tossim_getDarjeelingUglyGlobals();
}

static inline void setUglyGlobalVariables(struct tossim_UGLY_global_variables* _global_variables){
	tossim_setDarjeelingUglyGlobals((void *) _global_variables);
}


#endif
