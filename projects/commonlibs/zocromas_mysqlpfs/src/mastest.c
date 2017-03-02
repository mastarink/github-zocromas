#include "mysqlpfs_defs.h"
#include <malloc.h>
#include <stdio.h>
#include <string.h>

#include <limits.h>
#include <stdlib.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/regerr/masregerr.h>
#include <mastar/exam/masexam.h>
#include <mastar/masxfs/masxfs_pathinfo_base.h>
#include <mastar/masxfs/masxfs_pathinfo.h>

#include "mysqlpfs.h"
#include "mysqlpfs_query.h"

#include "mysqlpfs_base.h"
#include "mysqlpfs_mstmt_base.h"
#include "mysqlpfs_mstmt.h"

#include "mysqlpfs_structs.h"

int test1status( void );
int test2status( void );
int test3drop( void );
int test3create( void );
int test4( void );
int test4o( void );
int test5( void );
int test6( void );

int
main( int argc __attribute__ ( ( unused ) ), char *argv[] __attribute__ ( ( unused ) ) )
{
/* test1status(  ); */
/* test2status(  ); */
  test3drop(  );
  test3create(  );
/* test4o(  ); */
/* test4(  ); */
  /* test5(  ); */
  test6(  );
  /* test6(  ); */
  return 0;
}
