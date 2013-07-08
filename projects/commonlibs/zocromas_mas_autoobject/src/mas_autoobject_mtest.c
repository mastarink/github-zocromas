#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <time.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/types/mas_autoobject_types.h>

#include "mas_autoobject.h"
#include "mas_autoobject_object.h"


void
testfile( const char *fname, mas_iaccess_type_t atype, int mode )
{
  int fd = 0;
  mas_autoobject_t *ao;

  ao = mas_autoobject_create(  );
  fprintf( stderr, "@@@@@@ TIMING @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n" );
  if ( ( fd = open( "/dev/null", O_WRONLY ) ) )
  {
    mas_autoobject_set_iaccess_type( ao, atype );
    mas_autoobject_set_name( ao, NULL, fname );
    /* mas_autoobject_cat( STDOUT_FILENO, ao, 0 ); */
    for ( unsigned long ig = 0; ig < 100; ig++ )
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
  mas_autoobject_delete( ao );
}

int
main( void )
{
  mas_autoobject_t *ao;

  ao = mas_autoobject_create(  );
  printf( "=== CHAR ===\n" );
  {
    {
      mas_autoobject_set_iaccess_type( ao, MAS_IACCESS_CHAR );
      mas_autoobject_set_data( ao, "Hello world\n" );
    }
    mas_autoobject_cat( STDOUT_FILENO, ao, 0 );
  }
  printf( "=== FCHAR ===\n" );
  {
    {
      mas_autoobject_set_iaccess_type( ao, MAS_IACCESS_FCHAR );
      mas_autoobject_set_name( ao, NULL,"hwif3.txt" );
    }
    mas_autoobject_cat( STDOUT_FILENO, ao, 0 );
  }
  printf( "=== SENDFILE ===\n" );
  {
    {
      mas_autoobject_set_iaccess_type( ao, MAS_IACCESS_SENDFILE );
      mas_autoobject_set_name( ao, NULL, "hwif3.txt" );
    }
    mas_autoobject_cat( STDOUT_FILENO, ao, 0 );
  }
  printf( "=== SENDFILE (SPLICE ? ?) ===\n" );
  {
    mas_autoobject_cat( STDOUT_FILENO, ao, 0 );
    mas_autoobject_set_name( ao, NULL, "hwif2.txt" );
    mas_autoobject_cat( STDOUT_FILENO, ao, 0 );
    mas_autoobject_cat( STDOUT_FILENO, ao, 0 );
    mas_autoobject_set_name( ao, NULL, "hwif1.txt" );
    mas_autoobject_cat( STDOUT_FILENO, ao, 0 );
    {
      mas_autoobject_set_iaccess_type( ao, MAS_IACCESS_SENDFILE );
      mas_autoobject_set_name( ao, NULL, "hwif3.txt" );
    }
  }
  mas_autoobject_delete( ao );
  /* testfile( "hwif3.txt", MAS_IACCESS_SENDFILE, 4 ); */
  /* testfile( "hwif3.txt", MAS_IACCESS_SENDFILE, 0 ); */
  /* testfile( "hwif3.txt", MAS_IACCESS_FCHAR, 0 ); */
  /* testfile( "configure", MAS_IACCESS_FCHAR, 4 ); */
  /* testfile( "configure", MAS_IACCESS_FCHAR, 1 ); */
  /* testfile( "configure", MAS_IACCESS_SPLICE, 4 ); */
  testfile( "configure", MAS_IACCESS_SENDFILE, 0 );
  /* testfile( "/home/mastar/firefox.cxd1038p.FF20121017a.tar.gz", MAS_IACCESS_SENDFILE, 4 ); */




#ifdef MAS_TRACEMEM
  print_memlist( stderr, FL );
#endif

  return 0;
}
