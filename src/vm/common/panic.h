#ifndef __panic_h
#define __panic_h

#define DJ_PANIC_OUT_OF_MEMORY              42
#define DJ_PANIC_ILLEGAL_INTERNAL_STATE     43
#define DJ_PANIC_UNIMPLEMENTED_FEATURE      44
#define DJ_PANIC_UNCAUGHT_EXCEPTION         45
#define DJ_PANIC_UNSATISFIED_LINK			46
#define DJ_PANIC_MALFORMED_INFUSION			47

void dj_panic(int32_t panictype);

#endif
