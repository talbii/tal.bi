#ifdef __C22_REFERENCES
#define _Ref(x) &(x)
#define _Ref_get_ref(x) *x
#define _Ref_obj(x) _Ref_get_ref(_Ref(x))
#else
#error "unsupported feature"
#endif
