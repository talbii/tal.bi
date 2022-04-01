#ifdef __C22_REFERENCES
#include "internal/references.h"
#define __Empty
#define ref _Ref_get_ref(__Empty)
#define get_ref(x) _Ref(x)
#else
#error "Unsupported feature (C22 References)"
#endif
