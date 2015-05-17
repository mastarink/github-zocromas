#include <stddef.h>
#include "duf_config_types.h"
#include "duf_ufilter_types.h"
#include "duf_levinfo_types.h"

#include "duf_config_ref.h"
#include "duf_option_types.h"
#include "duf_options_enum.h"
#include "duf_optable_def.h"

/*
At duf_options_table.c:
  1. extern const duf_longval_extended_table_t optable_filter[];
  2.  static const duf_longval_extended_table_t *_lo_extended_multi[] = {
         ...
	optable_filter,
	 ...

	NULL
      };
*/


const duf_longval_extended_table_t optable_filter = {
  .table =                      /* */
  {
   {.o = {DO_Q( "same" ) /*              */ , DO_A_O /* */ , DO_V( SAME )} /*           */ , DO_CL( FILTER ) /*   */ ,
    /*      */ DO_OU( MINMAX, same ),.call = {.value =
                                              {.u = 2}} /*                                              */ , DO_H(  .... ) /*             */ },
   {.o = {DO_Q( "min-same" ) /*          */ , DO_A_R /* */ , DO_V( MINSAME )} /*        */ , DO_CL( FILTER ) /*   */ ,
    /*      */ DO_OU( MIN, same ) /*                                                                        */ , DO_H(  .... ) /*             */ },
   {.o = {DO_Q( "max-same" ) /*          */ , DO_A_R /* */ , DO_V( MAXSAME )} /*        */ , DO_CL( FILTER ) /*   */ ,
    /*      */ DO_OU( MAX, same ) /*                                                                        */ , DO_H(  .... ) /*             */ },

   {.o = {DO_Q( "exifsame" ) /*          */ , DO_A_O /* */ , DO_V( EXIFSAME )} /*       */ , DO_CL( FILTER ) /*   */ ,
    /*      */ DO_OU( MINMAX, exifsame ),.call = {.value =
                                                  {.u = 2}} /*                                          */ , DO_H(  .... ) /*             */ },
   {.o = {DO_Q( "min-exifsame" ) /*      */ , DO_A_R /* */ , DO_V( MINEXIFSAME )} /*    */ , DO_CL( FILTER ) /*   */ ,
    /*      */ DO_OU( MIN, exifsame ) /*                                                                    */ , DO_H(  .... ) /*             */ },
   {.o = {DO_Q( "max-exifsame" ) /*      */ , DO_A_R /* */ , DO_V( MAXEXIFSAME )} /*    */ , DO_CL( FILTER ) /*   */ ,
    /*      */ DO_OU( MAX, exifsame ) /*                                                                    */ , DO_H(  .... ) /*             */ },

   {.o = {DO_Q( "mimesame" ) /*          */ , DO_A_O /* */ , DO_V( MIMESAME )} /*       */ , DO_CL( FILTER ) /*   */ ,
    /*      */ DO_OU( MINMAX, mimesame ),.call = {.value =
                                                  {.u = 2}} /*                                          */ , DO_H(  .... ) /*             */ },
   {.o = {DO_Q( "min-mimesame" ) /*      */ , DO_A_R /* */ , DO_V( MINMIMESAME )} /*    */ , DO_CL( FILTER ) /*   */ ,
    /*      */ DO_OU( MIN, mimesame ) /*                                                                    */ , DO_H(  .... ) /*             */ },
   {.o = {DO_Q( "max-mimesame" ) /*      */ , DO_A_R /* */ , DO_V( MAXMIMESAME )} /*    */ , DO_CL( FILTER ) /*   */ ,
    /*      */ DO_OU( MAX, mimesame ) /*                                                                    */ , DO_H(  .... ) /*             */ },

   {.o = {DO_Q( "size" ) /*              */ , DO_A_R /* */ , DO_V( SIZE )} /*           */ , DO_CL( FILTER ) /*   */ ,
    /*      */ DO_OU( MINMAXLL, size ) /*                                                                   */ , DO_H(  .... ) /*             */ },
   {.o = {DO_Q( "min-size" ) /*          */ , DO_A_R /* */ , DO_V( MINSIZE )} /*        */ , DO_CL( FILTER ) /*   */ ,
    /*      */ DO_OU( MINLL, size ) /*                                                                      */ , DO_H(  .... ) /*             */ },
   {.o = {DO_Q( "max-size" ) /*          */ , DO_A_R /* */ , DO_V( MAXSIZE )} /*        */ , DO_CL( FILTER ) /*   */ ,
    /*      */ DO_OU( MAXLL, size ) /*                                                                      */ , DO_H(  .... ) /*             */ },

   {.o = {DO_Q( "nameid" ) /*            */ , DO_A_R /* */ , DO_V( NAMEID )} /*         */ , DO_CL( FILTER ) /*   */ ,
    /*      */ DO_OU( MINMAX, nameid ) /*                                                                   */ , DO_H(  .... ) /*             */ },
   {.o = {DO_Q( "min-nameid" ) /*        */ , DO_A_R /* */ , DO_V( MINNAMEID )} /*      */ , DO_CL( FILTER ) /*   */ ,
    /*      */ DO_OU( MIN, nameid ) /*                                                                      */ , DO_H(  .... ) /*             */ },
   {.o = {DO_Q( "max-nameid" ) /*        */ , DO_A_R /* */ , DO_V( MAXNAMEID )} /*      */ , DO_CL( FILTER ) /*   */ ,
    /*      */ DO_OU( MAX, nameid ) /*                                                                      */ , DO_H(  .... ) /*             */ },

   {.o = {DO_Q( "dirid" ) /*             */ , DO_A_R /* */ , DO_V( DIRID )} /*          */ , DO_CL( FILTER ) /*   */ ,
    /*      */ DO_OU( MINMAX, dirid ) /*                                                                    */ , DO_H(  .... ) /*             */ },
   {.o = {DO_Q( "min-dirid" ) /*         */ , DO_A_R /* */ , DO_V( MINDIRID )} /*       */ , DO_CL( FILTER ) /*   */ ,
    /*      */ DO_OU( MIN, dirid ) /*                                                                       */ , DO_H(  .... ) /*             */ },
   {.o = {DO_Q( "max-dirid" ) /*         */ , DO_A_R /* */ , DO_V( MAXDIRID )} /*       */ , DO_CL( FILTER ) /*   */ ,
    /*      */ DO_OU( MAX, dirid ) /*                                                                       */ , DO_H(  .... ) /*             */ },

   {.o = {DO_Q( "mtime" ) /*             */ , DO_A_R /* */ , DO_V( MTIME )} /*          */ , DO_CL( FILTER ) /*   */ ,
    /*      */ DO_OU( MINMAXDATETIME, mtime ) /*                                                            */ , DO_H(  .... ) /*             */ },
   {.o = {DO_Q( "min-mtime" ) /*         */ , DO_A_R /* */ , DO_V( MINMTIME )} /*       */ , DO_CL( FILTER ) /*   */ ,
    /*      */ DO_OU( MINDATETIME, mtime ) /*                                                               */ , DO_H(  .... ) /*             */ },
   {.o = {DO_Q( "max-mtime" ) /*         */ , DO_A_R /* */ , DO_V( MAXMTIME )} /*       */ , DO_CL( FILTER ) /*   */ ,
    /*      */ DO_OU( MAXDATETIME, mtime ) /*                                                               */ , DO_H(  .... ) /*             */ },

   {.o = {DO_Q( "atime" ) /*             */ , DO_A_R /* */ , DO_V( ATIME )} /*          */ , DO_CL( FILTER ) /*   */ ,
    /*      */ DO_OU( MINMAXDATETIME, atime ) /*                                                            */ , DO_H(  .... ) /*             */ },
   {.o = {DO_Q( "min-atime" ) /*         */ , DO_A_R /* */ , DO_V( MINATIME )} /*       */ , DO_CL( FILTER ) /*   */ ,
    /*      */ DO_OU( MINDATETIME, atime ) /*                                                              */ , DO_H(  .... ) /*             */ },
   {.o = {DO_Q( "max-atime" ) /*         */ , DO_A_R /* */ , DO_V( MAXATIME )} /*       */ , DO_CL( FILTER ) /*   */ ,
    /*      */ DO_OU( MAXDATETIME, atime ) /*                                                              */ , DO_H(  .... ) /*             */ },

   {.o = {DO_Q( "ctime" ) /*             */ , DO_A_R /* */ , DO_V( CTIME )} /*          */ , DO_CL( FILTER ) /*   */ ,
    /*      */ DO_OU( MINMAXDATETIME, ctime ) /*                                                            */ , DO_H(  .... ) /*             */ },
   {.o = {DO_Q( "min-ctime" ) /*         */ , DO_A_R /* */ , DO_V( MINCTIME )} /*       */ , DO_CL( FILTER ) /*   */ ,
    /*      */ DO_OU( MINDATETIME, ctime ) /*                                                              */ , DO_H(  .... ) /*             */ },
   {.o = {DO_Q( "max-ctime" ) /*         */ , DO_A_R /* */ , DO_V( MAXCTIME )} /*       */ , DO_CL( FILTER ) /*   */ ,
    /*      */ DO_OU( MAXDATETIME, ctime ) /*                                                              */ , DO_H(  .... ) /*             */ },

   {.o = {DO_Q( "exifdt" ) /*            */ , DO_A_R /* */ , DO_V( EXIFDT )} /*         */ , DO_CL( FILTER ) /*   */ ,
    /*      */ DO_OU( MINMAXDATETIME, exifdt ) /*                                                           */ , DO_H(  .... ) /*             */ },
   {.o = {DO_Q( "min-exifdt" ) /*        */ , DO_A_R /* */ , DO_V( MINEXIFDT )} /*      */ , DO_CL( FILTER ) /*   */ ,
    /*      */ DO_OU( MINDATETIME, exifdt ) /*                                                               */ , DO_H(  .... ) /*             */ },
   {.o = {DO_Q( "max-exifdt" ) /*        */ , DO_A_R /* */ , DO_V( MAXEXIFDT )} /*      */ , DO_CL( FILTER ) /*   */ ,
    /*      */ DO_OU( MAXDATETIME, exifdt ) /*                                                               */ , DO_H(  .... ) /*             */ },

   {.o = {DO_Q( "inode" ) /*             */ , DO_A_R /* */ , DO_V( NAMEID )} /*         */ , DO_CL( FILTER ) /*   */ ,
    /*      */ DO_OU( MINMAX, inode ) /*                                                                    */ , DO_H(  .... ) /*             */ },
   {.o = {DO_Q( "min-inode" ) /*         */ , DO_A_R /* */ , DO_V( MINNAMEID )} /*      */ , DO_CL( FILTER ) /*   */ ,
    /*      */ DO_OU( MIN, inode ) /*                                                                       */ , DO_H(  .... ) /*             */ },
   {.o = {DO_Q( "max-inode" ) /*         */ , DO_A_R /* */ , DO_V( MAXNAMEID )} /*      */ , DO_CL( FILTER ) /*   */ ,
    /*      */ DO_OU( MAX, inode ) /*                                                                       */ , DO_H(  .... ) /*             */ },

   /* {.o = {DO_Q( "md5id" ), (*       *) DO_A_R, (* *) DO_V( MD5ID )} (*          *), DO_CL( NODESC ) (*   *) , DO_H(  .... ) (*             *) }, */
   {.o = {DO_Q( "md5id" ) /*             */ , DO_A_R /* */ , DO_V( MD5ID )} /*          */ , DO_CL( FILTER ) /*   */ ,
    /*      */ DO_OU( MINMAX, md5id ) /*                                                                    */ , DO_H(  .... ) /*             */ },
   {.o = {DO_Q( "min-md5id" ) /*         */ , DO_A_R /* */ , DO_V( MINMD5ID )} /*       */ , DO_CL( FILTER ) /*   */ ,
    /*      */ DO_OU( MIN, md5id ) /*                                                                       */ , DO_H(  .... ) /*             */ },
   {.o = {DO_Q( "max-md5id" ) /*         */ , DO_A_R /* */ , DO_V( MAXMD5ID )} /*       */ , DO_CL( FILTER ) /*   */ ,
    /*      */ DO_OU( MAX, md5id ) /*                                                                       */ , DO_H(  .... ) /*             */ },

   /* {.o = {DO_Q( "sd5id" ), (*       *) DO_A_R, (* *) DO_V( SD5ID )} (*          *), DO_CL( NODESC ) (*   *) , DO_H(  .... ) (*             *) }, */
   {.o = {DO_Q( "sd5id" ) /*             */ , DO_A_R /* */ , DO_V( SD5ID )} /*          */ , DO_CL( FILTER ) /*   */ ,
    /*      */ DO_OU( MINMAX, sd5id ) /*                                                                    */ , DO_H(  .... ) /*             */ },
   {.o = {DO_Q( "min-sd5id" ) /*         */ , DO_A_R /* */ , DO_V( MINSD5ID )} /*       */ , DO_CL( FILTER ) /*   */ ,
    /*      */ DO_OU( MIN, sd5id ) /*                                                                       */ , DO_H(  .... ) /*             */ },
   {.o = {DO_Q( "max-sd5id" ) /*         */ , DO_A_R /* */ , DO_V( MAXSD5ID )} /*       */ , DO_CL( FILTER ) /*   */ ,
    /*      */ DO_OU( MAX, sd5id ) /*                                                                       */ , DO_H(  .... ) /*             */ },

   /* {.o = {DO_Q( "crc32id" ), (*     *) DO_A_R, (* *) DO_V( CRC32ID )} (*        *), DO_CL( TRACE ) (*    *) , DO_H( trace .... ) (*        *) }, */
   {.o = {DO_Q( "crc32id" ) /*           */ , DO_A_R /* */ , DO_V( CRC32ID )} /*        */ , DO_CL( FILTER ) /*   */ ,
    /*      */ DO_OU( MINMAX, crc32id ) /*                                                                  */ , DO_H(  .... ) /*             */ },
   {.o = {DO_Q( "min-crc32id" ) /*       */ , DO_A_R /* */ , DO_V( MINCRC32ID )} /*     */ , DO_CL( FILTER ) /*   */ ,
    /*      */ DO_OU( MIN, crc32id ) /*                                                                     */ , DO_H(  .... ) /*             */ },
   {.o = {DO_Q( "max-crc32id" ) /*       */ , DO_A_R /* */ , DO_V( MAXCRC32ID )} /*     */ , DO_CL( FILTER ) /*   */ ,
    /*      */ DO_OU( MAX, crc32id ) /*                                                                     */ , DO_H(  .... ) /*             */ },

   /* {.o = {DO_Q( "mimeid" ), (*      *) DO_A_R, (* *) DO_V( MIMEID )} (*         *), DO_CL( FILTER ) (*   *) , DO_H(  .... ) (*             *) }, */
   {.o = {DO_Q( "mimeid" ) /*            */ , DO_A_R /* */ , DO_V( MIMEID )} /*         */ , DO_CL( FILTER ) /*   */ ,
    /*      */ DO_OU( MINMAX, mimeid ) /*                                                                   */ , DO_H(  .... ) /*             */ },
   {.o = {DO_Q( "min-mimeid" ) /*        */ , DO_A_R /* */ , DO_V( MINMIMEID )} /*      */ , DO_CL( FILTER ) /*   */ ,
    /*      */ DO_OU( MIN, mimeid ) /*                                                                      */ , DO_H(  .... ) /*             */ },
   {.o = {DO_Q( "max-mimeid" ) /*        */ , DO_A_R /* */ , DO_V( MAXMIMEID )} /*      */ , DO_CL( FILTER ) /*   */ ,
    /*      */ DO_OU( MAX, mimeid ) /*                                                                      */ , DO_H(  .... ) /*             */ },

   /* {.o = {DO_Q( "exifid" ), (*      *) DO_A_R, (* *) DO_V( EXIFID )} (*         *), DO_CL( TRACE ) (*    *) , DO_H( trace .... ) (*        *) }, */
   {.o = {DO_Q( "exifid" ) /*            */ , DO_A_R /* */ , DO_V( EXIFID )} /*         */ , DO_CL( FILTER ) /*   */ ,
    /*      */ DO_OU( MINMAX, exifid ) /*                                                                   */ , DO_H(  .... ) /*             */ },
   {.o = {DO_Q( "min-exifid" ) /*        */ , DO_A_R /* */ , DO_V( MINEXIFID )} /*      */ , DO_CL( FILTER ) /*   */ ,
    /*      */ DO_OU( MIN, exifid ) /*                                                                      */ , DO_H(  .... ) /*             */ },
   {.o = {DO_Q( "max-exifid" ) /*        */ , DO_A_R /* */ , DO_V( MAXEXIFID )} /*      */ , DO_CL( FILTER ) /*   */ ,
    /*      */ DO_OU( MAX, exifid ) /*                                                                      */ , DO_H(  .... ) /*             */ },

   {.o = {DO_Q( "glob" ) /*              */ , DO_A_R /* */ , DO_V( GLOB )} /*           */ , DO_CL( CONTROL ) /*  */ ,
    /*      */ DO_OU( STR, glob ) /*                                                                        */ , DO_H( glob ) /*              */ },
   {.o = {DO_Q( "same-as" ) /*           */ , DO_A_R /* */ , DO_V( SAME_AS )} /*        */ , DO_CL( CONTROL ) /*  */ ,
    /*      */ DO_OU( STR, same_as ) /*                                                                     */ , DO_H( same as ) /*           */ },

   {.o = {DO_Q( "same-as" ) /*           */ , DO_A_O} /*                              */ , DO_CL( CONTROL ) /*  */ ,
    /*      */ DO_OU( PSTR, same_as ), DO_AT_STAGE( 1 ) /*                                                     */ ,
    DO_H( same as ) /*           */ },
   {.o = {DO_Q( "same-as" ) /*           */ , DO_A_O} /*                              */ , DO_CL( CONTROL ) /*  */ ,
    /*      */ DO_OC( FLAG, cli.act.v ), DO_FL( act, beginning_sql ), DO_AT_STAGE( 1 ) /*                       */ ,
    DO_H( same as ) /*           */ },

   {.o = {.name = NULL}}
   }
};

/* vi: ft=c colorcolumn=3,44,59,60,95,96,123,145,146
*/
