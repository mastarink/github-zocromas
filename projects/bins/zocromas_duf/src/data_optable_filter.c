#include <stddef.h>
#include "duf_config_types.h"
#include "duf_ufilter_types.h"
#include "duf_levinfo_types.h"

#include "duf_config_ref.h"
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
   {.o = {DO_Q( "same" ) /*             */ , DO_A_O /* */ , DO_VUF( MD5SAME )} /*             */ , DO_CL( FILTER ) /*   */ ,
    /*      */ DO_OU( MINMAXLL, md5same ),.call = {.value = {.u = 2}} /*                       */ , DO_H(  .... ) /*                            */ },
   {.o = {DO_Q( "min-same" ) /*         */ , DO_A_R /* */ , DO_VUF( MINMD5SAME )} /*          */ , DO_CL( FILTER ) /*   */ ,
    /*      */ DO_OU( MINLL, md5same ) /*                                                      */ , DO_H(  .... ) /*                            */ },
   {.o = {DO_Q( "max-same" ) /*         */ , DO_A_R /* */ , DO_VUF( MAXMD5SAME )} /*          */ , DO_CL( FILTER ) /*   */ ,
    /*      */ DO_OU( MAXLL, md5same ) /*                                                      */ , DO_H(  .... ) /*                            */ },

   {.o = {DO_Q( "shasame" ) /*             */ , DO_A_O /* */ , DO_VUF( SHASAME )} /*             */ , DO_CL( FILTER ) /*   */ ,
    /*      */ DO_OU( MINMAXLL, shasame ),.call = {.value = {.u = 2}} /*                       */ , DO_H(  .... ) /*                            */ },
   {.o = {DO_Q( "min-shasame" ) /*         */ , DO_A_R /* */ , DO_VUF( MINSHASAME )} /*          */ , DO_CL( FILTER ) /*   */ ,
    /*      */ DO_OU( MINLL, shasame ) /*                                                      */ , DO_H(  .... ) /*                            */ },
   {.o = {DO_Q( "max-shasame" ) /*         */ , DO_A_R /* */ , DO_VUF( MAXSHASAME )} /*          */ , DO_CL( FILTER ) /*   */ ,
    /*      */ DO_OU( MAXLL, shasame ) /*                                                      */ , DO_H(  .... ) /*                            */ },

   {.o = {DO_Q( "exifsame" ) /*         */ , DO_A_O /* */ , DO_VUF( EXIFSAME )} /*         */ , DO_CL( FILTER ) /*   */ ,
    /*      */ DO_OU( MINMAXLL, exifsame ),.call = {.value = {.u = 2}} /*                   */ , DO_H(  .... ) /*                            */ },
   {.o = {DO_Q( "min-exifsame" ) /*     */ , DO_A_R /* */ , DO_VUF( MINEXIFSAME )} /*      */ , DO_CL( FILTER ) /*   */ ,
    /*      */ DO_OU( MINLL, exifsame ) /*                                                  */ , DO_H(  .... ) /*                            */ },
   {.o = {DO_Q( "max-exifsame" ) /*     */ , DO_A_R /* */ , DO_VUF( MAXEXIFSAME )} /*      */ , DO_CL( FILTER ) /*   */ ,
    /*      */ DO_OU( MAXLL, exifsame ) /*                                                  */ , DO_H(  .... ) /*                            */ },

   {.o = {DO_Q( "mimesame" ) /*         */ , DO_A_O /* */ , DO_VUF( MIMESAME )} /*         */ , DO_CL( FILTER ) /*   */ ,
    /*      */ DO_OU( MINMAXLL, mimesame ),.call = {.value = {.u = 2}} /*                   */ , DO_H(  .... ) /*                            */ },
   {.o = {DO_Q( "min-mimesame" ) /*     */ , DO_A_R /* */ , DO_VUF( MINMIMESAME )} /*      */ , DO_CL( FILTER ) /*   */ ,
    /*      */ DO_OU( MINLL, mimesame ) /*                                                  */ , DO_H(  .... ) /*                            */ },
   {.o = {DO_Q( "max-mimesame" ) /*     */ , DO_A_R /* */ , DO_VUF( MAXMIMESAME )} /*      */ , DO_CL( FILTER ) /*   */ ,
    /*      */ DO_OU( MAXLL, mimesame ) /*                                                  */ , DO_H(  .... ) /*                            */ },

   {.o = {DO_Q( "datasame" ) /*         */ , DO_A_O /* */ , DO_VUF( DATASAME )} /*         */ , DO_CL( FILTER ) /*   */ ,
    /*      */ DO_OU( MINMAXLL, datasame ),.call = {.value = {.u = 2}} /*                   */ , DO_H(  .... ) /*                            */ },
   {.o = {DO_Q( "min-datasame" ) /*     */ , DO_A_R /* */ , DO_VUF( MINDATASAME )} /*      */ , DO_CL( FILTER ) /*   */ ,
    /*      */ DO_OU( MINLL, datasame ) /*                                                  */ , DO_H(  .... ) /*                            */ },
   {.o = {DO_Q( "max-datasame" ) /*     */ , DO_A_R /* */ , DO_VUF( MAXDATASAME )} /*      */ , DO_CL( FILTER ) /*   */ ,
    /*      */ DO_OU( MAXLL, datasame ) /*                                                  */ , DO_H(  .... ) /*                            */ },

   {.o = {DO_Q( "sizesame" ) /*         */ , DO_A_O /* */ , DO_VUF( SIZESAME )} /*         */ , DO_CL( FILTER ) /*   */ ,
    /*      */ DO_OU( MINMAXLL, sizesame ),.call = {.value = {.u = 2}} /*                   */ , DO_H(  .... ) /*                            */ },
   {.o = {DO_Q( "min-sizesame" ) /*     */ , DO_A_R /* */ , DO_VUF( MINSIZESAME )} /*      */ , DO_CL( FILTER ) /*   */ ,
    /*      */ DO_OU( MINLL, sizesame ) /*                                                  */ , DO_H(  .... ) /*                            */ },
   {.o = {DO_Q( "max-sizesame" ) /*     */ , DO_A_R /* */ , DO_VUF( MAXSIZESAME )} /*      */ , DO_CL( FILTER ) /*   */ ,
    /*      */ DO_OU( MAXLL, sizesame ) /*                                                  */ , DO_H(  .... ) /*                            */ },

   {.o = {DO_Q( "size" ) /*             */ , DO_A_R /* */ , DO_VUF( SIZE )} /*             */ , DO_CL( FILTER ) /*   */ ,
    /*      */ DO_OU( MINMAXLL, size ) /*                                                   */ , DO_H(  .... ) /*                            */ },
   {.o = {DO_Q( "min-size" ) /*         */ , DO_A_R /* */ , DO_VUF( MINSIZE )} /*          */ , DO_CL( FILTER ) /*   */ ,
    /*      */ DO_OU( MINLL, size ) /*                                                      */ , DO_H(  .... ) /*                            */ },
   {.o = {DO_Q( "max-size" ) /*         */ , DO_A_R /* */ , DO_VUF( MAXSIZE )} /*          */ , DO_CL( FILTER ) /*   */ ,
    /*      */ DO_OU( MAXLL, size ) /*                                                      */ , DO_H(  .... ) /*                            */ },

   {.o = {DO_Q( "nameid" ) /*           */ , DO_A_R /* */ , DO_VUF( NAMEID )} /*           */ , DO_CL( FILTER ) /*   */ ,
    /*      */ DO_OU( MINMAXLL, nameid ) /*                                                 */ , DO_H(  .... ) /*                            */ },
   {.o = {DO_Q( "min-nameid" ) /*       */ , DO_A_R /* */ , DO_VUF( MINNAMEID )} /*        */ , DO_CL( FILTER ) /*   */ ,
    /*      */ DO_OU( MINLL, nameid ) /*                                                    */ , DO_H(  .... ) /*                            */ },
   {.o = {DO_Q( "max-nameid" ) /*       */ , DO_A_R /* */ , DO_VUF( MAXNAMEID )} /*        */ , DO_CL( FILTER ) /*   */ ,
    /*      */ DO_OU( MAXLL, nameid ) /*                                                    */ , DO_H(  .... ) /*                            */ },

   {.o = {DO_Q( "dataid" ) /*           */ , DO_A_R /* */ , DO_VUF( DATAID )} /*           */ , DO_CL( FILTER ) /*   */ ,
    /*      */ DO_OU( MINMAXLL, dataid ) /*                                                 */ , DO_H(  .... ) /*                            */ },
   {.o = {DO_Q( "min-dataid" ) /*       */ , DO_A_R /* */ , DO_VUF( MINDATAID )} /*        */ , DO_CL( FILTER ) /*   */ ,
    /*      */ DO_OU( MINLL, dataid ) /*                                                    */ , DO_H(  .... ) /*                            */ },
   {.o = {DO_Q( "max-dataid" ) /*       */ , DO_A_R /* */ , DO_VUF( MAXDATAID )} /*        */ , DO_CL( FILTER ) /*   */ ,
    /*      */ DO_OU( MAXLL, dataid ) /*                                                    */ , DO_H(  .... ) /*                            */ },

   {.o = {DO_Q( "dirid" ) /*            */ , DO_A_R /* */ , DO_VUF( DIRID )} /*            */ , DO_CL( FILTER ) /*   */ ,
    /*      */ DO_OU( MINMAXLL, dirid ) /*                                                  */ , DO_H(  .... ) /*                            */ },
   {.o = {DO_Q( "min-dirid" ) /*        */ , DO_A_R /* */ , DO_VUF( MINDIRID )} /*         */ , DO_CL( FILTER ) /*   */ ,
    /*      */ DO_OU( MINLL, dirid ) /*                                                     */ , DO_H(  .... ) /*                            */ },
   {.o = {DO_Q( "max-dirid" ) /*        */ , DO_A_R /* */ , DO_VUF( MAXDIRID )} /*         */ , DO_CL( FILTER ) /*   */ ,
    /*      */ DO_OU( MAXLL, dirid ) /*                                                     */ , DO_H(  .... ) /*                            */ },

   {.o = {DO_Q( "mtime" ) /*            */ , DO_A_R /* */ , DO_VUF( MTIME )} /*            */ , DO_CL( FILTER ) /*   */ ,
    /*      */ DO_OU( MINMAXDATETIME, mtime ) /*                                            */ , DO_H(  .... ) /*                            */ },
   {.o = {DO_Q( "min-mtime" ) /*        */ , DO_A_R /* */ , DO_VUF( MINMTIME )} /*         */ , DO_CL( FILTER ) /*   */ ,
    /*      */ DO_OU( MINDATETIME, mtime ) /*                                               */ , DO_H(  .... ) /*                            */ },
   {.o = {DO_Q( "max-mtime" ) /*        */ , DO_A_R /* */ , DO_VUF( MAXMTIME )} /*         */ , DO_CL( FILTER ) /*   */ ,
    /*      */ DO_OU( MAXDATETIME, mtime ) /*                                               */ , DO_H(  .... ) /*                            */ },

   {.o = {DO_Q( "atime" ) /*            */ , DO_A_R /* */ , DO_VUF( ATIME )} /*            */ , DO_CL( FILTER ) /*   */ ,
    /*      */ DO_OU( MINMAXDATETIME, atime ) /*                                            */ , DO_H(  .... ) /*                            */ },
   {.o = {DO_Q( "min-atime" ) /*        */ , DO_A_R /* */ , DO_VUF( MINATIME )} /*         */ , DO_CL( FILTER ) /*   */ ,
    /*      */ DO_OU( MINDATETIME, atime ) /*                                               */ , DO_H(  .... ) /*                            */ },
   {.o = {DO_Q( "max-atime" ) /*        */ , DO_A_R /* */ , DO_VUF( MAXATIME )} /*         */ , DO_CL( FILTER ) /*   */ ,
    /*      */ DO_OU( MAXDATETIME, atime ) /*                                               */ , DO_H(  .... ) /*                            */ },

   {.o = {DO_Q( "ctime" ) /*            */ , DO_A_R /* */ , DO_VUF( CTIME )} /*            */ , DO_CL( FILTER ) /*   */ ,
    /*      */ DO_OU( MINMAXDATETIME, ctime ) /*                                            */ , DO_H(  .... ) /*                            */ },
   {.o = {DO_Q( "min-ctime" ) /*        */ , DO_A_R /* */ , DO_VUF( MINCTIME )} /*         */ , DO_CL( FILTER ) /*   */ ,
    /*      */ DO_OU( MINDATETIME, ctime ) /*                                               */ , DO_H(  .... ) /*                            */ },
   {.o = {DO_Q( "max-ctime" ) /*        */ , DO_A_R /* */ , DO_VUF( MAXCTIME )} /*         */ , DO_CL( FILTER ) /*   */ ,
    /*      */ DO_OU( MAXDATETIME, ctime ) /*                                               */ , DO_H(  .... ) /*                            */ },

   {.o = {DO_Q( "exifdt" ) /*           */ , DO_A_R /* */ , DO_VUF( EXIFDT )} /*           */ , DO_CL( FILTER ) /*   */ ,
    /*      */ DO_OU( MINMAXDATETIME, exifdt ) /*                                           */ , DO_H(  .... ) /*                            */ },
   {.o = {DO_Q( "min-exifdt" ) /*       */ , DO_A_R /* */ , DO_VUF( MINEXIFDT )} /*        */ , DO_CL( FILTER ) /*   */ ,
    /*      */ DO_OU( MINDATETIME, exifdt ) /*                                              */ , DO_H(  .... ) /*                            */ },
   {.o = {DO_Q( "max-exifdt" ) /*       */ , DO_A_R /* */ , DO_VUF( MAXEXIFDT )} /*        */ , DO_CL( FILTER ) /*   */ ,
    /*      */ DO_OU( MAXDATETIME, exifdt ) /*                                              */ , DO_H(  .... ) /*                            */ },

   {.o = {DO_Q( "inode" ) /*            */ , DO_A_R /* */ , DO_VUF( NAMEID )} /*           */ , DO_CL( FILTER ) /*   */ ,
    /*      */ DO_OU( MINMAXLL, inode ) /*                                                  */ , DO_H(  .... ) /*                            */ },
   {.o = {DO_Q( "min-inode" ) /*        */ , DO_A_R /* */ , DO_VUF( MINNAMEID )} /*        */ , DO_CL( FILTER ) /*   */ ,
    /*      */ DO_OU( MINLL, inode ) /*                                                     */ , DO_H(  .... ) /*                            */ },
   {.o = {DO_Q( "max-inode" ) /*        */ , DO_A_R /* */ , DO_VUF( MAXNAMEID )} /*        */ , DO_CL( FILTER ) /*   */ ,
    /*      */ DO_OU( MAXLL, inode ) /*                                                     */ , DO_H(  .... ) /*                            */ },

   {.o = {DO_Q( "md5id" ) /*            */ , DO_A_R /* */ , DO_VUF( MD5ID )} /*            */ , DO_CL( FILTER ) /*   */ ,
    /*      */ DO_OU( MINMAXLL, md5id ) /*                                                  */ , DO_H(  .... ) /*                            */ },
   {.o = {DO_Q( "min-md5id" ) /*        */ , DO_A_R /* */ , DO_VUF( MINMD5ID )} /*         */ , DO_CL( FILTER ) /*   */ ,
    /*      */ DO_OU( MINLL, md5id ) /*                                                     */ , DO_H(  .... ) /*                            */ },
   {.o = {DO_Q( "max-md5id" ) /*        */ , DO_A_R /* */ , DO_VUF( MAXMD5ID )} /*         */ , DO_CL( FILTER ) /*   */ ,
    /*      */ DO_OU( MAXLL, md5id ) /*                                                     */ , DO_H(  .... ) /*                            */ },

   {.o = {DO_Q( "shaid" ) /*            */ , DO_A_R /* */ , DO_VUF( SHAID )} /*            */ , DO_CL( FILTER ) /*   */ ,
    /*      */ DO_OU( MINMAXLL, shaid ) /*                                                  */ , DO_H(  .... ) /*                            */ },
   {.o = {DO_Q( "min-shaid" ) /*        */ , DO_A_R /* */ , DO_VUF( MINSHAID )} /*         */ , DO_CL( FILTER ) /*   */ ,
    /*      */ DO_OU( MINLL, shaid ) /*                                                     */ , DO_H(  .... ) /*                            */ },
   {.o = {DO_Q( "max-shaid" ) /*        */ , DO_A_R /* */ , DO_VUF( MAXSHAID )} /*         */ , DO_CL( FILTER ) /*   */ ,
    /*      */ DO_OU( MAXLL, shaid ) /*                                                     */ , DO_H(  .... ) /*                            */ },

   {.o = {DO_Q( "sd5id" ) /*            */ , DO_A_R /* */ , DO_VUF( SD5ID )} /*            */ , DO_CL( FILTER ) /*   */ ,
    /*      */ DO_OU( MINMAXLL, sd5id ) /*                                                  */ , DO_H(  .... ) /*                            */ },
   {.o = {DO_Q( "min-sd5id" ) /*        */ , DO_A_R /* */ , DO_VUF( MINSD5ID )} /*         */ , DO_CL( FILTER ) /*   */ ,
    /*      */ DO_OU( MINLL, sd5id ) /*                                                     */ , DO_H(  .... ) /*                            */ },
   {.o = {DO_Q( "max-sd5id" ) /*        */ , DO_A_R /* */ , DO_VUF( MAXSD5ID )} /*         */ , DO_CL( FILTER ) /*   */ ,
    /*      */ DO_OU( MAXLL, sd5id ) /*                                                     */ , DO_H(  .... ) /*                            */ },

   {.o = {DO_Q( "crc32id" ) /*          */ , DO_A_R /* */ , DO_VUF( CRC32ID )} /*          */ , DO_CL( FILTER ) /*   */ ,
    /*      */ DO_OU( MINMAXLL, crc32id ) /*                                                */ , DO_H(  .... ) /*                            */ },
   {.o = {DO_Q( "min-crc32id" ) /*      */ , DO_A_R /* */ , DO_VUF( MINCRC32ID )} /*       */ , DO_CL( FILTER ) /*   */ ,
    /*      */ DO_OU( MINLL, crc32id ) /*                                                   */ , DO_H(  .... ) /*                            */ },
   {.o = {DO_Q( "max-crc32id" ) /*      */ , DO_A_R /* */ , DO_VUF( MAXCRC32ID )} /*       */ , DO_CL( FILTER ) /*   */ ,
    /*      */ DO_OU( MAXLL, crc32id ) /*                                                   */ , DO_H(  .... ) /*                            */ },

   {.o = {DO_Q( "mimeid" ) /*           */ , DO_A_R /* */ , DO_VUF( MIMEID )} /*           */ , DO_CL( FILTER ) /*   */ ,
    /*      */ DO_OU( MINMAXLL, mimeid ) /*                                                 */ , DO_H(  .... ) /*                            */ },
   {.o = {DO_Q( "min-mimeid" ) /*       */ , DO_A_R /* */ , DO_VUF( MINMIMEID )} /*        */ , DO_CL( FILTER ) /*   */ ,
    /*      */ DO_OU( MINLL, mimeid ) /*                                                    */ , DO_H(  .... ) /*                            */ },
   {.o = {DO_Q( "max-mimeid" ) /*       */ , DO_A_R /* */ , DO_VUF( MAXMIMEID )} /*        */ , DO_CL( FILTER ) /*   */ ,
    /*      */ DO_OU( MAXLL, mimeid ) /*                                                    */ , DO_H(  .... ) /*                            */ },

   {.o = {DO_Q( "exifid" ) /*           */ , DO_A_R /* */ , DO_VUF( EXIFID )} /*           */ , DO_CL( FILTER ) /*   */ ,
    /*      */ DO_OU( MINMAXLL, exifid ) /*                                                 */ , DO_H(  .... ) /*                            */ },
   {.o = {DO_Q( "min-exifid" ) /*       */ , DO_A_R /* */ , DO_VUF( MINEXIFID )} /*        */ , DO_CL( FILTER ) /*   */ ,
    /*      */ DO_OU( MINLL, exifid ) /*                                                    */ , DO_H(  .... ) /*                            */ },
   {.o = {DO_Q( "max-exifid" ) /*       */ , DO_A_R /* */ , DO_VUF( MAXEXIFID )} /*        */ , DO_CL( FILTER ) /*   */ ,
    /*      */ DO_OU( MAXLL, exifid ) /*                                                    */ , DO_H(  .... ) /*                            */ },

   {.o = {DO_Q( "filename-db" ) /*      */ , DO_A_R /* */ , DO_VUF( FILENAME )} /*         */ , DO_CL( FILTER ) /*  */ ,
    /*      */ DO_OU( STR, filename ) /*                                                    */ , DO_H( glob db ) /*                          */ },
   {.o = {DO_Q( "glob-db" ) /*          */ , DO_A_R /* */ , DO_VUF( GLOB_DB )} /*          */ , DO_CL( FILTER ) /*  */ ,
    /*      */ DO_OU( STR, glob_db ) /*                                                     */ , DO_H( glob db ) /*                          */ },
   {.o = {DO_Q( "include-db" ) /*       */ , DO_A_R, DO_VUF( GLOB_INCL_DB_FILES )} /*      */ , DO_CL( FILTER ) /*     */ ,
    /*      */ DO_OU( STR, glob_db_include ) /*                                             */ , DO_H( glob db include ) /*                  */ },
   {.o = {DO_Q( "exclude-db" ) /*       */ , DO_A_R, DO_VUF( GLOB_EXCL_DB_FILES )} /*      */ , DO_CL( FILTER ) /*     */ ,
    /*      */ DO_OU( STR, glob_db_exclude ) /*                                             */ , DO_H( glob db exclude ) /*                  */ },

   {.o = {DO_Q( "with-tag-dir" ) /*     */ , DO_A_R /* */ , DO_VUF( WITH_TAG_DIR )} /*     */ , DO_CL( FILTER ) /*     */ ,
    /*      */ DO_OU( STR, tag.dir ) /*                                                     */ , DO_H( with tag dir ) /*                     */ },
   {.o = {DO_Q( "dirs-tagged-as" ) /*   */ , DO_A_R /* */ , DO_VUF( WITH_TAG_DIR )} /*     */ , DO_CL( FILTER ) /*     */ ,
    /*      */ DO_OU( STR, tag.dir ) /*                                                     */ , DO_H( with tag dir ) /*                     */ },

   {.o = {DO_Q( "with-tag-file" ) /*    */ , DO_A_R /* */ , DO_VUF( WITH_TAG_FILE )} /*    */ , DO_CL( FILTER ) /*     */ ,
    /*      */ DO_TDB(  ), DO_SET_STAGE( FIRST, INTERACTIVE ) /*                            */ , DO_H( with tag file ) /*                    */ },
   {.o = {DO_Q( "files-tagged-as" ) /*  */ , DO_A_R /* */ , DO_VUF( WITH_TAG_FILE )} /*    */ , DO_CL( FILTER ) /*     */ ,
    /*      */ DO_TDB(  ), DO_SET_STAGE( FIRST, INTERACTIVE ) /*                            */ , DO_H( with tag file ) /*                    */ },

   {.o = {DO_Q( "with-camera" ) /*      */ , DO_A_R /* */ , DO_VUF( WITH_CAMERA )} /*      */ , DO_CL( FILTER ) /*     */ ,
    /*      */ DO_OU( STR, exif.camera ), DO_SET_STAGE( FIRST, INTERACTIVE ) /*             */ , DO_H( with tag file ) /*                    */ },

   {.o = {DO_Q( "with-mime-type" ) /*   */ , DO_A_R /* */ , DO_VUF( WITH_MIME_TYPE )} /*   */ , DO_CL( FILTER ) /*     */ ,
    /*      */ DO_OU( STR, mime.type ) /*                                                   */ , DO_H( with tag file ) /*                    */ },


   {.o = {DO_Q( "same-as-md5" ) /*      */ , DO_A_R /* */ , DO_VUF( SAME_AS )} /*          */ , DO_CL( FILTER ) /*  */ ,
    /*      */ DO_OU( STR, same_md5 ) /*                                                    */ , DO_H( same md5 ) /*                         */ },
   {.o = {DO_Q( "same-as-sha" ) /*      */ , DO_A_R /* */ , DO_VUF( SAME_AS_SHA )} /*      */ , DO_CL( FILTER ) /*  */ ,
    /*      */ DO_OU( STR, same_sha ) /*                                                    */ , DO_H( same sha ) /*                         */ },

   {.o = {.name = NULL}}
   }
};

/* vi: ft=c colorcolumn=3,44,59,60,95,96,123,145,146
*/
