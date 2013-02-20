#ifndef MAS_MEMORY_H
#  define MAS_MEMORY_H

/* FILE */
#  include <stdio.h>

struct mas_mem_head_s
{
  const char *func;
  int line;
  unsigned long sig;
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



#  ifdef MAS_TRACEMEM

char *_mas_strcat_xt( const char *func, int line, char *s1, const char *s2 );
char *_mas_strncat_xt( const char *func, int line, char *s1, const char *s2, size_t maxs2 );

void *_mas_calloc( const char *func, int line, size_t nmemb, size_t size );
void *_mas_malloc( const char *func, int line, size_t size );
void _mas_free( const char *func, int line, void *ptr );
void *_mas_realloc( const char *func, int line, void *ptr, size_t size );
char *_mas_strdup( const char *func, int line, const char *str );
char *_mas_strndup( const char *func, int line, const char *s, size_t n );

int print_memlist( FILE * f, const char *func, int line );
int _print_memlist( FILE * f, const char *func, int line, int s_f );

#    define mas_strcat_x(...)  _mas_strcat_xt(FL, __VA_ARGS__)
#    define mas_strcat(...)  _mas_strcat_xt(FL, __VA_ARGS__)
#    define mas_strncat_x(...) _mas_strncat_xt(FL, __VA_ARGS__)
#    define mas_calloc(...)  _mas_calloc(FL, __VA_ARGS__)
#    define mas_malloc(...)  _mas_malloc(FL, __VA_ARGS__)
#    define mas_free(...)    _mas_free(FL, __VA_ARGS__)
#    define mas_realloc(...) _mas_realloc(FL, __VA_ARGS__)
#    define mas_strdup(...)  _mas_strdup(FL, __VA_ARGS__)
#    define mas_strndup(...) _mas_strndup(FL, __VA_ARGS__)

#  else

#    define mas_strcat_x(...)  mas_strcat_xt (__VA_ARGS__)
#    define mas_strcat(...)  strcat (__VA_ARGS__)
#    define mas_strncat_x(...) mas_strncat_xt(__VA_ARGS__)
#    define mas_calloc(...)  calloc (__VA_ARGS__)
#    define mas_malloc(...)  malloc (__VA_ARGS__)
#    define mas_free(...)    free   (__VA_ARGS__)
#    define mas_realloc(...) realloc(__VA_ARGS__)
#    define mas_strdup(...)  strdup (__VA_ARGS__)
#    define mas_strndup(...) strndup(__VA_ARGS__)

#  endif



/* mas_tools.c */
#endif
