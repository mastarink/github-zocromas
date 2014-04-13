#include "mas_std_def.h"

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

#ifndef MAS_NO_THREADS
#  include <pthread.h>
#endif
#include <errno.h>

/* #include "mas_common.h" */

#include "mas_memory.h"

/*
this:
  mas_memory.c
related:
  mas_lib.c
  mas_lib0.c
  mas_tools.c
  mas_cs.c
  mas_cs.h
  mas_control.c
  mas_opts.c
  
  mas_load_module.c

*/


#ifndef MAS_NO_THREADS
pthread_mutex_t malloc_mutex = PTHREAD_MUTEX_INITIALIZER;
#endif

#ifdef MAS_TRACEMEM
int mas_tracemem_flag = 1;
#else
int mas_tracemem_flag = 0;
#endif
#ifdef MAS_TRACEMEM
char *
_mas_strncat_xt( const char *func, int line, char *s1, const char *s2, size_t maxs2 )
{
  char *r = NULL;
  int l, l1, l2;

  l1 = s1 ? strlen( s1 ) : 0;
  l2 = s2 ? strlen( s2 ) : 0;
  if ( maxs2 < l2 )
    l2 = maxs2;
  l = l1 + l2;
  if ( l > 0 )
  {
    r = _mas_malloc( func, line, l + 1 );
    if ( s1 )
    {
      memcpy( r, s1, l1 );
      _mas_free( func, line, s1 );
    }
    if ( s2 )
      memcpy( r + l1, s2, l2 );
    r[l] = 0;
  }
  return r;
}

static void
memerror( mas_mem_head_t * m )
{
  fprintf( stderr, "\nMEMORY ERROR 0x%lx > [sig:0x%llx : id:0x%lx]\n", ( unsigned long ) ( m + 1 ), m->sig, m->id );
  /* sleep( 98 ); */
  exit( 11 );
}

#endif

unsigned long memory_allocated = 0;
unsigned long memory_allocated_cnt = 0;
unsigned long memory_freed = 0;
unsigned long memory_freed_cnt = 0;
unsigned long memory_balance = 0;
unsigned long memory_balance_cnt = 0;

void *
mas_other_calloc( size_t nmemb, size_t size )
{
  return MAS_CALLOC( nmemb, size );
}

void *
mas_other_malloc( size_t size )
{
  return MAS_MALLOC( size );
}


char *
mas_other_strdup( const char *str )
{
  return MAS_STRDUP( str );
}

void
mas_other_free( void *p )
{
  MAS_FREE( p );
}

#ifdef MAS_TRACEMEM
int imemar = 0;
mas_mem_head_t *memar[4096];

static int
_print_memlist( FILE * f, const char *func, int line, int fn_f, int s_f )
{
  int h = 0;
  int r = -1;

  if ( f )
  {
    r = fprintf( f, "WMWMOMWMWM" );
    r = 0;
    for ( int im = 0; r >= 0 && im < ( sizeof( memar ) / sizeof( memar[0] ) ); im++ )
    {
      if ( memar[im] )
      {
        mas_mem_head_t *data_ptr;

        if ( !h++ )
          r = fprintf( f, "%s:%d MEMORY TABLE\n", func, line );

        data_ptr = ( mas_mem_head_t * ) memar[im];
        data_ptr++;
        if ( r >= 0 )
        {
          mas_mem_head_t *_mhp;
          unsigned long _id;
          unsigned long _size;
          unsigned _line;
          const char *_func;

          _mhp = memar[im];
          _id = _mhp->id;
          _size = _mhp->size;
          _func = _mhp->func;
          _line = _mhp->line;
          if ( fn_f )
            fprintf( f, "id: %lx; sz:%lu(%lX); %s:%u [%s]\n", _id, _size, _size, _func, _line, s_f ? ( char * ) data_ptr : "-" );
          else
            fprintf( f, "id: %lx; sz:%lu(%lX); fun#%lx:%u [%s]\n", _id, _size, _size, ( unsigned long ) _func, _line,
                     s_f ? ( char * ) data_ptr : "-" );
          r++;
        }
      }
    }
    if ( !h && r >= 0 )
      r = fprintf( f, "<<EMP MEMT>>" );
    /* r = fprintf( f, "** EMPTY MEMORY TABLE ** %s:%u", func, line ); */
    
    /* if ( r >= 0 )                    */
    /*   r = fprintf( f, "[%d]\n", r ); */
  }
  return r;
}

int
print_memlist_vany( const char *func, int line, int cnt, va_list args )
{

  for ( int i = 0; i < cnt; i++ )
  {
    int r = -1;
    FILE *f = NULL;

    f = va_arg( args, FILE * );
    if ( f && ( r = print_memlist( func, line, f ) ) >= 0 )
      return r;
  }
  return -1;
}

int
print_memlist_any( const char *func, int line, int cnt, ... )
{
  int r = -1;
  va_list args;

  va_start( args, cnt );
  r = print_memlist_vany( func, line, cnt, args );
  va_end( args );
  return r;
}

int
print_memlist( const char *func, int line, FILE * f )
{
  int r;

  r = _print_memlist( f, func, line, 0, 0 );
  /* _print_memlist( f, func, line, 1, 0 ); */

  /* if ( r <= 0 ) */
  _print_memlist( f, func, line, 1, 1 );
  return r;
}

void *
_mas_calloc( const char *func, int line, size_t nmemb, size_t isize )
{
  size_t size;

  size = isize * nmemb;
  return _mas_malloc( func, line, size );
}

void *
_mas_malloc( const char *func, int line, size_t size )
{
  void *ptr = NULL;

  if ( !imemar )
  {
    memset( memar, 0, sizeof( memar ) );
    imemar = 1;
  }
  if ( 1 )
  {
    mas_mem_head_t *real_ptr;
    size_t real_size;

    real_size = size + sizeof( mas_mem_head_t );
    real_ptr = ( mas_mem_head_t * ) mas_other_malloc( real_size );
    if ( real_ptr )
    {
#  ifndef MAS_NO_THREADS
      pthread_mutex_lock( &malloc_mutex );
#  endif
      memory_allocated += size;
      memory_allocated_cnt++;
      memory_balance += size;
      memory_balance_cnt++;
      real_ptr->sig = 0x6cdb7c9c89c7ad3a;
      real_ptr->id = memory_allocated_cnt;
      if ( memory_allocated_cnt < sizeof( memar ) / sizeof( memar[0] ) )
        memar[memory_allocated_cnt] = real_ptr;
      real_ptr->func = func;
      real_ptr->line = line;
      real_ptr->size = size;
#  ifndef MAS_NO_THREADS
      pthread_mutex_unlock( &malloc_mutex );
#  endif
      /* memMSG( "@ _mas_malloc %lx > id:%lx; %lu", ( unsigned long ) real_ptr, real_ptr->id, size ); */

      real_ptr++;
      ptr = ( void * ) real_ptr;
    }
    else
    {
      fprintf( stderr, "\nMALLOC - CAN'T ALLOCATE\n" );
      exit( 11 );
    }
  }
  else
  {
    ptr = mas_other_malloc( size );
  }
  /* fprintf( stderr, "\nMALLOC 0x%p\n", ptr ); */
  return ptr;
}

void
_mas_free( const char *func, int line, void *ptr )
{
  if ( 1 )
  {
    size_t size;
    mas_mem_head_t *real_ptr;

    if ( ptr )
    {
      real_ptr = ( mas_mem_head_t * ) ptr;
      real_ptr--;
      if ( real_ptr->sig == 0x6cdb7c9c89c7ad3a )
      {
        {
          unsigned long id;

#  ifndef MAS_NO_THREADS
          pthread_mutex_lock( &malloc_mutex );
#  endif
          id = real_ptr->id;
          if ( id < sizeof( memar ) / sizeof( memar[0] ) )
            memar[id] = 0;
          size = real_ptr->size;
          memory_freed += size;
          memory_freed_cnt++;
          memory_balance -= size;
          memory_balance_cnt--;
          real_ptr->sig = 0xa6cdb7c9c89c7ad3;
          /* real_ptr->size = 0; */
#  ifndef MAS_NO_THREADS
          pthread_mutex_unlock( &malloc_mutex );
#  endif
          /* memMSG( "@ _mas_free %lx > id:%lx; %lu", ( unsigned long ) real_ptr, id, size ); */
        }
        mas_other_free( real_ptr );
      }
      else
      {
        /* memMSG( "@ _mas_free %lx > id:?; ?", ( unsigned long ) real_ptr ); */
        memerror( real_ptr );
      }
    }
  }
  else
  {
    mas_other_free( ptr );
  }
}

void *
_mas_realloc( const char *func, int line, void *ptr, size_t new_size )
{
  char *mem;
  mas_mem_head_t *real_ptr = NULL;
  size_t min_size;

  mem = _mas_malloc( func, line, new_size );
  real_ptr = ( mas_mem_head_t * ) mem;
  real_ptr--;
  min_size = new_size;
  if ( ptr )
  {
    size_t old_size;
    mas_mem_head_t *old_real_ptr;

    old_real_ptr = ( mas_mem_head_t * ) ptr;
    old_real_ptr--;
    old_size = old_real_ptr->size;

    if ( old_size < min_size )
      min_size = old_size;
    memcpy( mem, ptr, min_size );
    _mas_free( func, line, ptr );
  }
  return ( void * ) mem;
}

char *
_mas_strndup( const char *func, int line, const char *str, size_t n )
{
  char *s = NULL;

  if ( 1 )
  {
    if ( str )
    {
      size_t l;

      l = strlen( str );
      /* if ( l ) */
      {
        if ( n < l )
          l = n;
        s = _mas_malloc( func, line, l + 1 );
        if ( s )
        {
          if ( l > 0 )
            memcpy( s, str, l );
          s[l] = 0;
        }
      }
    }
  }
  else
  {
    s = strndup( str, n );
  }
  return s;
}

char *
_mas_strdup( const char *func, int line, const char *str )
{
  char *s = NULL;

  if ( 1 )
  {
    if ( str )
      s = _mas_strndup( func, line, str, strlen( str ) );
  }
  else
  {
    s = mas_other_strdup( str );
  }
  return s;
}
#endif



#if 0
#  if 0

void *__real_malloc( size_t size );
void *
__wrap_malloc( size_t size )
{
  return __real_malloc( size );
}

void __real_free( void *ptr );
void
__wrap_free( void *ptr )
{
  __real_free( ptr );
}

void *__real_realloc( void *ptr, size_t size );
void *
__wrap_realloc( void *ptr, size_t size )
{
  return __real_realloc( ptr, size );
}

#  else
/* MUTEX !*/

/*
void __real_strdup( void *ptr );
void
__wrap_strdup( void *ptr )
{
}
*/

void *
__wrap_malloc( size_t size )
{
  void *ptr = NULL;

  /* if ( size > 0 ) */
  {
    size_t real_size;
    mas_mem_head_t *real_ptr = NULL;


    real_size = size;
    real_size += sizeof( mas_mem_head_t );
    real_ptr = ( mas_mem_head_t * ) __real_malloc( real_size );

    if ( real_ptr )
    {
      {
        unsigned long id;

        pthread_mutex_lock( &malloc_mutex );
        real_ptr->sig = 0x6cdb7c9c89c7ad3a;
        real_ptr->id = memory_allocated_cnt;
        real_ptr->size = size;
        real_ptr++;

        id = memory_allocated_cnt;
        memory_allocated += size;
        memory_allocated_cnt++;
        memory_balance += size;
        memory_balance_cnt++;
        pthread_mutex_unlock( &malloc_mutex );
        MSG( "@ malloc %lx > id:%lx; %lu", ( unsigned long ) real_ptr, id, size );
      }
      ptr = ( void * ) real_ptr;
    }
  }
  return ptr;
}

void *
__wrap_calloc( size_t nmemb, size_t size )
{
  /* MFP( "M%d : cmalloc\n", __LINE__ ); */
  return mas_other_malloc( nmemb * size );
}

void *
__wrap_xmalloc( size_t size )
{
  /* MFP( "M%d : xmalloc\n", __LINE__ ); */
  return mas_other_malloc( size );
}


void
__wrap_free( void *ptr )
{
  if ( ptr )
  {
    mas_mem_head_t *real_ptr = NULL;

    real_ptr = ( mas_mem_head_t * ) ptr;
    real_ptr--;
    if ( real_ptr->sig != 0x6cdb7c9c89c7ad3a )
    {
      memerror( real_ptr );
    }
    else
    {
      unsigned long id;
      size_t size = 0;

      pthread_mutex_lock( &malloc_mutex );
      size = real_ptr->size;
      id = real_ptr->id;
      memory_freed += size;
      memory_freed_cnt++;
      memory_balance -= size;
      memory_balance_cnt--;
      pthread_mutex_unlock( &malloc_mutex );
      MSG( "@ free id:%lx; %lu", id, size );
      __real_free( ( void * ) real_ptr );
    }
  }
}

void *
__wrap_realloc( void *ptr, size_t size )
{
  char *mem;
  mas_mem_head_t *real_ptr = NULL;
  size_t min_size;

  mem = mas_other_malloc( size );
  real_ptr = ( mas_mem_head_t * ) mem;
  real_ptr--;
  min_size = size;
  if ( real_ptr->size < min_size )
    min_size = real_ptr->size;
  memcpy( mem, ptr, min_size );
  mas_other_free( ptr );
  /* return __real_realloc( ptr, size ); */
  return ( void * ) mem;
}

char *
__wrap___strdup( const char *ptr )
{
  char *new;
  size_t len;

  len = strlen( ptr );
  new = mas_other_malloc( len + 1 );
  mas_other_free( new );
  new = mas_other_malloc( len + 1 );
  memcpy( new, ptr, len + 1 );
  /* MFP( "M%d : strdup %p\n", __LINE__, new ); */
  return new;
}
#  endif
#endif
