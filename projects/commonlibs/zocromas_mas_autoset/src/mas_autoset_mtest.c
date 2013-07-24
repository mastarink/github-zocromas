#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <search.h>


#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/types/mas_autoset_types.h>

#include <mastar/autoobject/mas_autoobject_object.h>
#include <mastar/autoobject/mas_autoobject.h>

#include "mas_autoset_object.h"
#include "mas_autoset_search.h"
#include "mas_autoset.h"

void
testfile( mas_autoobject_t * ao, mas_iaccess_type_t atype, int mode, int maxiter )
{
  int fd = 0;

  fprintf( stderr, "@@@@@@ TIMING @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n" );
  if ( ( fd = open( "/dev/null", O_WRONLY ) ) )
  {
    fprintf( stderr, "[********** ao: [%p]] {reopen_cnt:%lu;load_cnt:%lu;close_cnt:%lu;lseek_cnt:%lu;sendfile_cnt:%lu;}\n",
             ( void * ) ao, ao->reopen_cnt, ao->load_cnt, ao->close_cnt, ao->lseek_cnt, ao->sendfile_cnt );
    mas_autoobject_set_iaccess_type( ao, atype );
    fprintf( stderr, "[********** ao: [%p]] {reopen_cnt:%lu;load_cnt:%lu;close_cnt:%lu;lseek_cnt:%lu;sendfile_cnt:%lu;}\n",
             ( void * ) ao, ao->reopen_cnt, ao->load_cnt, ao->close_cnt, ao->lseek_cnt, ao->sendfile_cnt );
    /* mas_autoobject_cat( STDOUT_FILENO, ao, 0 ); */
    for ( unsigned long ig = 0; ig < maxiter; ig++ )
    {
      /*
       * MAS_IACCESS_SENDFILE (open + close): 490 ns / 7800 ns
       * MAS_IACCESS_SENDFILE: 450 ns / 6600 ns
       * MAS_IACCESS_FCHAR (open + read + close): 1888 ns / 44329 ns
       * MAS_IACCESS_FCHAR (read): 505 ns / 42350 ns
       * MAS_IACCESS_FCHAR: 155 ns / 160 ns
       * */
      unsigned long tcounter = 10;
      unsigned long scounter = 1000;
      unsigned long counter = tcounter * scounter * 3;
      struct timespec ts0, tse, tsd;
      double per;
      size_t sz = 0;

      if ( ao->size > 100000000 )
        scounter = 100;

      /* fprintf( stderr, "@@@@@@@@@@@@@@@@@@@@@@ [opened %lu; loaded %lu; closed %lu; lseek %lu; sendfile %lu;]\n", ao->reopen_cnt, */
      /*          ao->load_cnt, ao->close_cnt, ao->lseek_cnt, ao->sendfile_cnt );                                                    */

      clock_gettime( CLOCK_MONOTONIC, &ts0 );
      {
        for ( unsigned long ic = 0; ic < counter; ic++ )
        {
          ssize_t rr = -1;

          rr = mas_autoobject_cat( fd, ao, mode );
          if ( rr <= 0 )
            break;
          else if ( rr != ao->size )
          {
            fprintf( stderr, "ERROR rr (%lu) != ao->size (%lu)\n", rr, ao->size );
            break;
          }
          else
            sz += rr;
        }
      }
      clock_gettime( CLOCK_MONOTONIC, &tse );
      tsd.tv_sec = tse.tv_sec - ts0.tv_sec;
      tsd.tv_nsec = tse.tv_nsec - ts0.tv_nsec;
      if ( tsd.tv_nsec > 1000000000 )
      {
        tsd.tv_sec++;
        tsd.tv_nsec -= 1000000000;
      }
      per = ( ( ( double ) tsd.tv_nsec ) + ( ( double ) tsd.tv_sec * 1E9 ) ) / counter;
      /* fprintf( stderr, "(wsize:%lu) %lu.%lu - %lu.%lu = %lu.%lu (%10.5g)\n", sz / counter, ts0.tv_sec, ts0.tv_nsec, tse.tv_sec, */
      /*          tse.tv_nsec, tsd.tv_sec, tsd.tv_nsec, per );                                                                     */
      fprintf( stderr, "%lu :: %10.5g\n", sz / counter, per );
      if ( sz == 0 )
      {
        break;
      }
      sleep( 1 );
    }
    close( fd );
  }
}

int
main( void )
{
  mas_autoset_t *as1 = NULL;
  mas_autoobject_t *ao11;
  mas_autoobject_t *ao12;
  mas_autoobject_t *ao21;
  mas_autoobject_t *ao22;

  as1 = mas_autoset_create(  );
  {
/* "configure", MAS_IACCESS_FCHAR, 0 */


    ao11 = mas_autoset_search_autoobject( as1, ".", "configure" );
    fprintf( stderr, "[********** ao11: [%p]] {reopen_cnt:%lu;load_cnt:%lu;close_cnt:%lu;lseek_cnt:%lu;sendfile_cnt:%lu;}\n",
             ( void * ) ao11, ao11->reopen_cnt, ao11->load_cnt, ao11->close_cnt, ao11->lseek_cnt, ao11->sendfile_cnt );
    testfile( ao11, MAS_IACCESS_SENDFILE, 0, 3 );

    fprintf( stderr, "[********** ao11: [%p]] {reopen_cnt:%lu;load_cnt:%lu;close_cnt:%lu;lseek_cnt:%lu;sendfile_cnt:%lu;}\n",
             ( void * ) ao11, ao11->reopen_cnt, ao11->load_cnt, ao11->close_cnt, ao11->lseek_cnt, ao11->sendfile_cnt );
    ao21 = mas_autoset_search_autoobject( as1, ".", "tclass2" );
    fprintf( stderr, "[********** ao21: [%p]]\n", ( void * ) ao21 );

    ao12 = mas_autoset_find_autoobject( as1, ".", "configure" );
    fprintf( stderr, "[********** ao12: [%p]]\n", ( void * ) ao12 );
    ao22 = mas_autoset_find_autoobject( as1, ".", "tclass2" );
    fprintf( stderr, "[********** ao21: [%p]]\n", ( void * ) ao22 );

    fprintf( stderr, "[********** ao11: [%p]] {reopen_cnt:%lu;load_cnt:%lu;close_cnt:%lu;lseek_cnt:%lu;sendfile_cnt:%lu;}\n",
             ( void * ) ao11, ao11->reopen_cnt, ao11->load_cnt, ao11->close_cnt, ao11->lseek_cnt, ao11->sendfile_cnt );
    testfile( ao12, MAS_IACCESS_SENDFILE, 0, 3 );
    fprintf( stderr, "[********** ao12: [%p]] {reopen_cnt:%lu;load_cnt:%lu;close_cnt:%lu;lseek_cnt:%lu;sendfile_cnt:%lu;}\n",
             ( void * ) ao12, ao12->reopen_cnt, ao12->load_cnt, ao12->close_cnt, ao12->lseek_cnt, ao12->sendfile_cnt );
  }
  mas_autoset_delete( as1 );

#ifdef MAS_TRACEMEM
  print_memlist( FL, stderr );
#endif
  return 0;
}
