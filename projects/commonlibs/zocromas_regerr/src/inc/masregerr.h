#ifndef MAS_ZOCROMAS_REGERR_H
# define MAS_ZOCROMAS_REGERR_H
# include <errno.h>

# define MAS_FLF __func__,__LINE__,__FILE__
# define RGEX(_sys) masregerr_reg(MAS_FLF, PACKAGE, &errno, errno?_sys:0)
# define RGE RGEX(0)
# define RGES RGEX(1)
# define RGER(_r)	{ if ((_r)<0) RGE; }
# define RGESR(_r)	{ if ((_r)<0) RGES; }

# define MASREGERR_DEFAULT_SIZE 256

struct masregerr_s
{
  int mark;
  const char *func;
  int line;
  const char *file;
  const char *package;
  int err_no;
  int sys;
  long serial;
};
typedef struct masregerr_s masregerr_t;

masregerr_t *masregerr_init( size_t size );

masregerr_t *masregerr_after( masregerr_t * rge );
masregerr_t *masregerr_before( masregerr_t * rge );

masregerr_t *masregerr_next( void );
masregerr_t *masregerr_prev( void );

int masregerr_reg( const char *func, int line, const char *file, const char *package, int *perrno, int sys );

int masregerr_print_simple( const masregerr_t * rge, const char *msg );
int masregerr_print_simple_all( const char *msg );
int masregerr_print_simple_all_back( const char *msg );
int masregerr_print_simple_last( const char *msg );

#endif
