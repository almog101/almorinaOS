#ifndef STDARG_H
#define STDARG_H

typedef unsigned char *va_list;

#define STACKITEM int

#define va_start(AP,LASTARG)                                  \
  ( AP = (( va_list ) & ( LASTARG ) + VA_SIZE( LASTARG )))

#define va_arg(AP,TYPE)                                       \
  (AP+=__va_rounded_size(TYPE),*((TYPE *)(AP-__va_rounded_size(TYPE))))

#define __va_rounded_size(TYPE)                               \
  (((sizeof(TYPE)+sizeof(int)-1)/sizeof(int))*sizeof(int))

#define VA_SIZE(TYPE)                                         \
  ((sizeof(TYPE)+sizeof(STACKITEM)-1)&~(sizeof(STACKITEM)-1))

#define va_end(AP) \
   ( AP = (void *)(0) )
 
#endif
