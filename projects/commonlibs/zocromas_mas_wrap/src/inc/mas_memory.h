#ifndef MAS_MEMORY_H
# define MAS_MEMORY_H

/* FILE */
# include <stdio.h>
# include <stdlib.h>

# include  "mas_std_def.h"

typedef void ( *mas_free_fun_t ) ( void *ptr );
typedef void *( *mas_malloc_fun_t ) ( size_t size );
typedef void *( *mas_calloc_fun_t ) ( size_t nmemb, size_t isize );
typedef void *( *mas_realloc_fun_t ) ( void *ptr, size_t new_size );
typedef char *( *mas_strdup_fun_t ) ( const char *str );
typedef char *( *mas_strndup_fun_t ) ( const char *str, size_t n );
struct mas_memfun_block_s
{
  mas_free_fun_t mfree;
  mas_malloc_fun_t mmalloc;
  mas_calloc_fun_t mcalloc;
  mas_realloc_fun_t mrealloc;
  mas_strdup_fun_t mstrdup;
  mas_strndup_fun_t mstrndup;
};
typedef struct mas_memfun_block_s mas_memfun_block_t;

struct mas_mem_head_s
{
  const char *func;
  int line;
  unsigned long long sig;
  unsigned long id;
  size_t size;
};
typedef struct mas_mem_head_s mas_mem_head_t;

/* #  define MAS_TRY_LJ */

void *__real_malloc( size_t size );
void *__real_xmalloc( size_t size );
void *__real_calloc( size_t nmemb, size_t size );
void __real_free( void *ptr );
void *__real_realloc( void *ptr, size_t size );
char *__real___strdup( const char *ptr );

void *mas_other_malloc( size_t size );
void mas_other_free( void * );

extern const mas_memfun_block_t mas_memfun_block_z;
extern mas_memfun_block_t mas_memfun_block;
extern const mas_memfun_block_t *mas_pmemfun_block;

const mas_memfun_block_t *mas_memory_allocation( void ) __attribute__ ( ( weak ) );

# define MAS_MEMORY_ALLOCATION_MASTAR (mas_memfun_block_t *)NULL
# define MAS_MEMORY_ALLOCATION_STD &mas_memfun_block_z
# define MAS_MEMORY_ALLOCATION_OTHER &mas_memfun_block_o
# define MAS_MEMORY_ALLOCATION_EMPTY &mas_memfun_block
# define MAS_MEMORY_ALLOCATION(_ptr) mas_pmemfun_block=_ptr;
/* void mas_memory_allocation_init( const mas_memfun_block_t * m ); */

# ifdef MAS_TRACEMEM

void *_mas_calloc( const char *func, int line, size_t nmemb, size_t size );
void *_mas_malloc( const char *func, int line, size_t size );
void _mas_free( const char *func, int line, void *ptr );
void *_mas_realloc( const char *func, int line, void *ptr, size_t size );
char *_mas_strdup( const char *func, int line, const char *str );
char *_mas_strndup( const char *func, int line, const char *s, size_t n );

int print_memlist_vany( const char *func, int line, int cnt, va_list args );
int print_memlist_any( const char *func, int line, int cnt, ... );
int print_memlist( const char *func, int line, FILE * f );
int print_memlist_msg( const char *func, int line, FILE * f, const char *msg1a, const char *msg1b, const char *msg2, const char *msg3,
                       const char *msg4 );

#  define mas_calloc(...)  _mas_calloc(FL, __VA_ARGS__)
#  define mas_malloc(...)  _mas_malloc(FL, __VA_ARGS__)
#  define mas_free(...)    _mas_free(FL, __VA_ARGS__)
#  define mas_realloc(...) _mas_realloc(FL, __VA_ARGS__)
#  define mas_strdup(...)  _mas_strdup(FL, __VA_ARGS__)
#  define mas_strndup(...) _mas_strndup(FL, __VA_ARGS__)

# else

#  define mas_calloc(...)  calloc (__VA_ARGS__)
#  define mas_malloc(...)  malloc (__VA_ARGS__)
#  define mas_free(...)    free   (__VA_ARGS__)
#  define mas_realloc(...) realloc(__VA_ARGS__)
#  define mas_strdup(...)  strdup (__VA_ARGS__)
#  define mas_strndup(...) strndup(__VA_ARGS__)

# endif

/* mas_tools.c */
#endif
