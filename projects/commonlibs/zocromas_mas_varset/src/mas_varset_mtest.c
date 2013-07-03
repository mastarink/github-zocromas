#define MAS_USE_VARVEC

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <search.h>


#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/types/mas_varset_types.h>

#ifdef MAS_USE_VARVEC
#  include <mastar/varvec/mas_varvec_object.h>
#  include <mastar/varvec/mas_varvec_element.h>
#  include <mastar/varvec/mas_varvec.h>
#  include <mastar/varvec/mas_varvec_search.h>
#  include <mastar/varvec/mas_varvec_namevalue.h>
#  include <mastar/varvec/mas_varvec_headtail.h>
#else
#  include "mas_varset_vclass_object.h"
#  include "mas_varset_vclass_element.h"
#  include "mas_varset_vclass.h"
#  include "mas_varset_vclass_search.h"
#  include "mas_varset_vclass_namevalue.h"
#  include "mas_varset_vclass_headtail.h"
#endif

#include "mas_varset_object.h"
#include "mas_varset.h"
#include "mas_varset_search.h"
#include "mas_varset_headtail.h"

const char *error_message = "\t\t\t\t@@@@@@@@@@@ Error";
const char *ok_message = " OK";


static void
w_action( const void *nodep, const VISIT which, const int depth )
{
  if ( which == endorder || which == leaf )
  {
    const char *name = NULL;

    if ( nodep )
    {
#ifdef MAS_USE_VARVEC
      mas_varvec_t *vclass = NULL;
#else
      mas_varset_class_t *vclass = NULL;
#endif

      unsigned long id;

#ifdef MAS_USE_VARVEC
      vclass = *( ( mas_varvec_t ** ) nodep );
      id = mas_varvec_id( vclass );
      name = mas_varvec_name( vclass );
#else
      vclass = *( ( mas_varset_class_t ** ) nodep );
      id = mas_varset_vclass_id( vclass );
      name = mas_varset_vclass_name( vclass );
#endif
      /* fprintf( stderr, "nodep:%p; which:%lu; depth:%lu --- %s\n", nodep, ( unsigned long ) which, ( unsigned long ) depth, */
      /*          name ? name : "-" );                                                                                        */
      fprintf( stderr, "[%lu] %lu. %s\n", ( unsigned long ) depth, id, name ? name : "-" );
      if ( vclass->el_cnt )
      {
#ifdef MAS_USE_VARVEC
        mas_varvec_element_t *vec;
#else
        mas_vclass_element_t *vec;
#endif

        vec = vclass->vec;
        for ( int j = vclass->has_head; j < vclass->el_cnt; j += 2 )
        {
          char *name;
          char *val;

#ifdef MAS_USE_VARVEC
          name = mas_varvec_variable_get_name( &vec[j] );
          val = mas_varvec_variable_get_value( &vec[j] );
#else
          name = mas_varset_vclass_variable_get_name( &vec[j] );
          val = mas_varset_vclass_variable_get_value( &vec[j] );
#endif
          fprintf( stderr, "\t\t\t-- %s='%s'\n", name, val );
          mas_free( val );
          mas_free( name );
        }
      }
    }
  }
}

void
test00( void )
{
  mas_varset_t *vs1 = NULL;

  for ( int iter = 0; iter < 1; iter++ )
  {
    const char *name = "User-Agent";

    vs1 = mas_varset_create(  );
    if ( 1 )
    {
      if ( 1 )
      {
#ifdef MAS_USE_VARVEC
        mas_varvec_t *vc;
#else
        mas_varset_class_t *vc;
#endif
        vc = mas_varset_search_vclass( vs1, "tclass1" );

        fprintf( stderr, "[********** vc: [%p]]\n", ( void * ) vc );

        /* mas_varset_vclass_variable_set_element( vc->vec, 0, "\r\n", 2, "HTTP 7.0" ); */
      }
      vs1 = mas_varset_search_variable( vs1, "tclass4", "tc4-varnameX", "varvalueX" );
      fprintf( stderr, "[********** vs1: [%p]]\n", ( void * ) vs1 );
      vs1 = mas_varset_search_variable( vs1, "tclass1", "tc1-varname1", "varvalue1" );
      vs1 = mas_varset_search_variable( vs1, "tclass1", "tc1-Content-Length", "77777" );
      vs1 = mas_varset_search_variable( vs1, "tclass1", "tc1-Content-Type", "text/html" );
      /* vs1 = mas_varset_search_variable( vs1, "tclass4", "varname7", "varvalue7" ); */
      vs1 = mas_varset_search_variable( vs1, "tclass4", "varname7a", "varvalue7" );
      vs1 = mas_varset_search_variable( vs1, "tclass1", "tc1-Content-Type", "text/plain" );
/* 'User-Agent' = 'lwp-request/6.03 libwww-perl/6.03' */

      if ( 1 )
      {
        vs1 = mas_varset_search_variablef( vs1, "inheader", name, NULL, "%s", "lwp-request/6.03 libwww-perl/6.03" );
        vs1 = mas_varset_search_variablef( vs1, "inheader", name, NULL, "[%s]", "lwp-request/6.03 libwww-perl/6.03" );
      }
      else if ( 0 )
      {
#ifdef MAS_USE_VARVEC
        mas_varvec_t *vc;
#else
        mas_varset_class_t *vc;
#endif

        vc = mas_varset_search_vclass( vs1, "tclass1" );
#ifdef MAS_USE_VARVEC
        mas_varvec_search_variablef( vc, "inheader", name, NULL, "%s", "lwp-request/6.03 libwww-perl/6.03" );
        mas_varvec_search_variablef( vc, "inheader", name, NULL, "[%s]", "lwp-request/6.03 libwww-perl/6.03" );
#else
        mas_varset_vclass_search_variablef( vc, "inheader", name, NULL, "%s", "lwp-request/6.03 libwww-perl/6.03" );
        mas_varset_vclass_search_variablef( vc, "inheader", name, NULL, "[%s]", "lwp-request/6.03 libwww-perl/6.03" );
#endif
      }
      else
      {
        vs1 = mas_varset_search_variable( vs1, "inheader", name, "lwp-request/6.03 libwww-perl/6.03" );
        vs1 = mas_varset_search_variable( vs1, "inheader", name, "[lwp-request/6.03 libwww-perl/6.03]" );
      }


      fprintf( stderr, "[********** vs1: [%p]]\n", ( void * ) vs1 );

      fprintf( stderr, "\n@@@ tclass1: [" );
      mas_varset_write( STDOUT_FILENO, vs1, "tclass1" );
      fprintf( stderr, "] @@@\n" );
      /* fprintf( stderr, "\n@@@ inheader: [" );            */
      /* mas_varset_write( STDOUT_FILENO, vs1, "inheader" ); */
      /* fprintf( stderr, "] @@@\n" );                      */

      {
#ifdef MAS_USE_VARVEC
        mas_varvec_t *vc;
#else
        mas_varset_class_t *vc;
#endif

        vc = mas_varset_search_vclass( vs1, "tclass1" );
        if ( vc )
        {
          fprintf( stderr, "\n@@@ [" );
          for ( int j = 0; j < vc->el_cnt + vc->has_head; j++ )
          {
            fprintf( stderr, "[%p] '%s'\n", vc->vec[j].iov_base, ( const char * ) vc->vec[j].iov_base );
          }
          fprintf( stderr, "] @@@\n" );
          /* mas_varset_vclass_variable_set_element( vc->vec, 0, "\r\n", 2, "HTTP ..." ); */
          fprintf( stderr, "\n@@@ [" );
          for ( int j = 0; j < vc->el_cnt + vc->has_head; j++ )
          {
            fprintf( stderr, "[%p] '%s'\n", vc->vec[j].iov_base, ( const char * ) vc->vec[j].iov_base );
          }
          fprintf( stderr, "] @@@\n" );
        }
      }
      fprintf( stderr, "\n@@@ [" );
      mas_varset_write( STDOUT_FILENO, vs1, "tclass1" );
      fprintf( stderr, "] @@@\n" );

      {
        mas_vclass_element_t *tv;

        tv = mas_varset_find_variable( vs1, "inheader", name );
        if ( tv )
        {
#ifdef MAS_USE_VARVEC
          fprintf( stderr, "TO SET inheader '%s'='%s'\n", name, mas_varvec_variable_get_value_ref( tv ) );
#else
          fprintf( stderr, "TO SET inheader '%s'='%s'\n", name, mas_varset_vclass_variable_get_value_ref( tv ) );
#endif
        }
        else
        {
          fprintf( stderr, "DIDN'T APPEAR VAR %s\n", name );
        }
      }
    }
    else
    {
      vs1 = mas_varset_search_variable( vs1, "tclass1", "varname1", "varvalue1" );
      vs1 = mas_varset_search_variable( vs1, "tclass1", "Content-Type", "text/html" );
      vs1 = mas_varset_search_variable( vs1, "tclass1", "Content-type", "text/plain" );
    }


    if ( 0 )
      mas_varset_walk_classes( vs1, w_action );

    /* mas_varset_delete_vclass( vs1, "tclass1" ); */
    /* mas_varset_delete_vclass( vs1, "tclass2" ); */
    mas_varset_delete( vs1 );

    vs1 = NULL;
    fprintf( stderr, "===========================\n" );
  }
}

void
test10( void )
{
  mas_varset_t *vs1 __attribute__ ( ( unused ) ) = NULL;

  fprintf( stderr, "\t+%s %d\t- %s\n", __func__, __LINE__, 1 ? ok_message : error_message );
  vs1 = mas_varset_create(  );
  mas_varset_delete( vs1 );
  fprintf( stderr, "\t-%s %d\t- %s\n", __func__, __LINE__, vs1 ? ok_message : error_message );
  vs1 = NULL;
}

void
test20( void )
{
#ifdef MAS_USE_VARVEC
  mas_varvec_t *vc1 __attribute__ ( ( unused ) ) = NULL;
  mas_varvec_t *vc2 __attribute__ ( ( unused ) ) = NULL;
  mas_varvec_t *vc3 __attribute__ ( ( unused ) ) = NULL;
  mas_varvec_t *vc4 __attribute__ ( ( unused ) ) = NULL;
  mas_varvec_t *vc5 __attribute__ ( ( unused ) ) = NULL;
#else
  mas_varset_class_t *vc1 __attribute__ ( ( unused ) ) = NULL;
  mas_varset_class_t *vc2 __attribute__ ( ( unused ) ) = NULL;
  mas_varset_class_t *vc3 __attribute__ ( ( unused ) ) = NULL;
  mas_varset_class_t *vc4 __attribute__ ( ( unused ) ) = NULL;
  mas_varset_class_t *vc5 __attribute__ ( ( unused ) ) = NULL;
#endif
  mas_varset_t *vs1 __attribute__ ( ( unused ) ) = NULL;

  fprintf( stderr, "\t+%s %d\t- %s\n", __func__, __LINE__, 1 ? ok_message : error_message );
  vs1 = mas_varset_create(  );

  vc1 = mas_varset_search_vclass( vs1, "tclass1" );
  fprintf( stderr, "\t %s %d\t- %s\n", __func__, __LINE__, vc1 ? ok_message : error_message );
  vc2 = mas_varset_search_vclass( vs1, "tclass2" );
  fprintf( stderr, "\t %s %d\t- %s\n", __func__, __LINE__, vc2 ? ok_message : error_message );

  vc3 = mas_varset_find_vclass( vs1, "tclass1" );
  fprintf( stderr, "\t %s %d\t- %s\n", __func__, __LINE__, vc3 ? ok_message : error_message );
  vc4 = mas_varset_find_vclass( vs1, "tclass2" );
  fprintf( stderr, "\t %s %d\t- %s\n", __func__, __LINE__, vc4 ? ok_message : error_message );
  vc5 = mas_varset_find_vclass( vs1, "tclass3" );
  fprintf( stderr, "\t %s %d\t- %s\n", __func__, __LINE__, !vc5 ? ok_message : error_message );

  mas_varset_delete( vs1 );
  vs1 = NULL;
  fprintf( stderr, "\t-%s %d\t- %s\n", __func__, __LINE__, 1 ? ok_message : error_message );

}

void
test30( void )
{
  mas_varset_t *vs1 __attribute__ ( ( unused ) ) = NULL;
  mas_varset_t *vs2 __attribute__ ( ( unused ) ) = NULL;

#ifdef MAS_USE_VARVEC
  mas_varvec_t *vc1 __attribute__ ( ( unused ) ) = NULL;
  mas_varvec_t *vc2 __attribute__ ( ( unused ) ) = NULL;
#else
  mas_varset_class_t *vc1 __attribute__ ( ( unused ) ) = NULL;
  mas_varset_class_t *vc2 __attribute__ ( ( unused ) ) = NULL;
#endif
  fprintf( stderr, "\t+%s %d\t- %s\n", __func__, __LINE__, 1 ? ok_message : error_message );
  vs1 = mas_varset_create(  );
  fprintf( stderr, "\t-%s %d\t- %s\n", __func__, __LINE__, vs1 ? ok_message : error_message );


  vc1 = mas_varset_search_vclass( vs1, "tclass1" );
  fprintf( stderr, "\t %s %d\t- %s\n", __func__, __LINE__, vc1 ? ok_message : error_message );
  vc2 = mas_varset_find_vclass( vs1, "tclass1" );
  fprintf( stderr, "\t %s %d\t- %s\n", __func__, __LINE__, vc2 && vc1 == vc2 ? ok_message : error_message );

  {
    mas_vclass_element_t *v1;

#ifdef MAS_USE_VARVEC
    v1 = _mas_varvec_add_elements( vc2, 2 );
    fprintf( stderr, "\t %s %d\t- %s\n", __func__, __LINE__, v1 ? ok_message : error_message );
    mas_varvec_variable_set_element( v1, 0, NULL, 0, "Wow" );
#else
    v1 = _mas_varset_vclass_add_elements( vc2, 2 );
    fprintf( stderr, "\t %s %d\t- %s\n", __func__, __LINE__, v1 ? ok_message : error_message );
    mas_varset_vclass_variable_set_element( v1, 0, NULL, 0, "Wow" );
#endif
    fprintf( stderr, "\t %s %d '%s'\t- %s\n", __func__, __LINE__, ( char * ) v1->iov_base, v1
             && 0 == strcmp( v1->iov_base, "Wow" ) ? ok_message : error_message );

#ifdef MAS_USE_VARVEC
    mas_varvec_variable_set_element( v1, 0, ": ", 2, "Wow" );
#else
    mas_varset_vclass_variable_set_element( v1, 0, ": ", 2, "Wow" );
#endif
    fprintf( stderr, "\t %s %d '%s'\t- %s\n", __func__, __LINE__, ( char * ) v1->iov_base, v1
             && 0 == strcmp( v1->iov_base, "Wow: " ) ? ok_message : error_message );
  }

  fprintf( stderr, "\t-%s %d\t- %s\n", __func__, __LINE__, 1 ? ok_message : error_message );
  mas_varset_delete( vs1 );
  fprintf( stderr, "\t-%s %d\t- %s\n", __func__, __LINE__, 1 ? ok_message : error_message );
  vs1 = NULL;
  fprintf( stderr, "\t-%s %d\t- %s\n", __func__, __LINE__, 1 ? ok_message : error_message );
  mas_varset_delete( vs2 );
  fprintf( stderr, "\t-%s %d\t- %s\n", __func__, __LINE__, 1 ? ok_message : error_message );
  vs2 = NULL;
  fprintf( stderr, "\t-%s %d\t- %s\n", __func__, __LINE__, 1 ? ok_message : error_message );
}

void
test31( void )
{
  mas_varset_t *vs1 __attribute__ ( ( unused ) ) = NULL;
  mas_varset_t *vs2 __attribute__ ( ( unused ) ) = NULL;

#ifdef MAS_USE_VARVEC
  mas_varvec_t *vc1 __attribute__ ( ( unused ) ) = NULL;
  mas_varvec_t *vc2 __attribute__ ( ( unused ) ) = NULL;
#else
  mas_varset_class_t *vc1 __attribute__ ( ( unused ) ) = NULL;
  mas_varset_class_t *vc2 __attribute__ ( ( unused ) ) = NULL;
#endif

  fprintf( stderr, "\t+%s %d\t- %s\n", __func__, __LINE__, 1 ? ok_message : error_message );
  vs1 = mas_varset_create(  );
  fprintf( stderr, "\t-%s %d\t- %s\n", __func__, __LINE__, vs1 ? ok_message : error_message );


  vc1 = mas_varset_search_vclass( vs1, "tclass1" );
  fprintf( stderr, "\t %s %d\t- %s\n", __func__, __LINE__, vc1 ? ok_message : error_message );
  vc2 = mas_varset_find_vclass( vs1, "tclass1" );
  fprintf( stderr, "\t %s %d\t- %s\n", __func__, __LINE__, vc2 && vc1 == vc2 ? ok_message : error_message );

  {
    mas_vclass_element_t *v1;

#ifdef MAS_USE_VARVEC
    v1 = _mas_varvec_add_elements( vc2, 1 );
    fprintf( stderr, "\t %s %d\t- %s\n", __func__, __LINE__, v1 ? ok_message : error_message );
    mas_varvec_variable_set_element( v1, 0, NULL, 0, "Wow" );
#else
    v1 = _mas_varset_vclass_add_elements( vc2, 1 );
    fprintf( stderr, "\t %s %d\t- %s\n", __func__, __LINE__, v1 ? ok_message : error_message );
    mas_varset_vclass_variable_set_element( v1, 0, NULL, 0, "Wow" );
#endif
    fprintf( stderr, "\t %s %d [%lu]\t- %s\n", __func__, __LINE__, ( v1 - vc1->vec ), v1
             && ( v1 - vc1->vec ) == 1 ? ok_message : error_message );
    fprintf( stderr, "\t %s %d '%s'\t- %s\n", __func__, __LINE__, ( char * ) v1->iov_base, v1
             && 0 == strcmp( v1->iov_base, "Wow" ) ? ok_message : error_message );

    mas_vclass_element_t *v2;

#ifdef MAS_USE_VARVEC
    v2 = _mas_varvec_add_elements( vc2, 1 );
    v1 = _mas_varvec_element( vc2, 0 );
#else
    v2 = _mas_varset_vclass_add_elements( vc2, 1 );
    v1 = _mas_varset_vclass_element( vc2, 0 );
#endif
    fprintf( stderr, "\t %s %d\t- %s\n", __func__, __LINE__, v2 ? ok_message : error_message );
    fprintf( stderr, "\t %s %d\t- %s\n", __func__, __LINE__, v1 && v2 && v2 != v1 ? ok_message : error_message );
    fprintf( stderr, "\t %s %d v2 [%lu]\t- %s\n", __func__, __LINE__, ( v2 - vc1->vec ), v2
             && ( v2 - vc1->vec ) == 2 ? ok_message : error_message );
    fprintf( stderr, "\t %s %d v1 [%lu]\t- %s\n", __func__, __LINE__, ( v1 - vc1->vec ), v1
             && ( v1 - vc1->vec ) == 1 ? ok_message : error_message );
#ifdef MAS_USE_VARVEC
    mas_varvec_variable_set_element( v2, 0, NULL, 2, "Hoh" );
#else
    mas_varset_vclass_variable_set_element( v2, 0, NULL, 2, "Hoh" );
#endif
    fprintf( stderr, "\t %s %d '%s'\t- %s\n", __func__, __LINE__, ( char * ) v2->iov_base, v2
             && 0 == strcmp( v2->iov_base, "Hoh" ) ? ok_message : error_message );

    mas_vclass_element_t *v3, *v3a;

#ifdef MAS_USE_VARVEC
    v3 = _mas_varvec_add_elements( vc2, 1 );
    v3a = _mas_varvec_element( vc2, 2 );
    v2 = _mas_varvec_element( vc2, 1 );
    v1 = _mas_varvec_element( vc2, 0 );
#else
    v3 = _mas_varset_vclass_add_elements( vc2, 1 );
    v3a = _mas_varset_vclass_element( vc2, 2 );
    v2 = _mas_varset_vclass_element( vc2, 1 );
    v1 = _mas_varset_vclass_element( vc2, 0 );
#endif
    fprintf( stderr, "\t %s %d\t- %s\n", __func__, __LINE__, v3 ? ok_message : error_message );
    fprintf( stderr, "\t %s %d\t- %s\n", __func__, __LINE__, v1 && v3 && v3 != v1 ? ok_message : error_message );
    fprintf( stderr, "\t %s %dv3 [%lu]\t- %s\n", __func__, __LINE__, ( v3 - vc1->vec ), v3
             && ( v3 - vc1->vec ) == 2 + vc1->has_head ? ok_message : error_message );
    fprintf( stderr, "\t %s %d v3a [%lu]\t- %s\n", __func__, __LINE__, ( v3a - vc1->vec ), v3a
             && ( v3a - vc1->vec ) == 2 + vc1->has_head ? ok_message : error_message );
    fprintf( stderr, "\t %s %d v2 [%lu]\t- %s\n", __func__, __LINE__, ( v2 - vc1->vec ), v2
             && ( v2 - vc1->vec ) == 1 + vc1->has_head ? ok_message : error_message );
    fprintf( stderr, "\t %s %d v1 [%lu]\t- %s\n", __func__, __LINE__, ( v1 - vc1->vec ), v1
             && ( v1 - vc1->vec ) == 0 + vc1->has_head ? ok_message : error_message );
#ifdef MAS_USE_VARVEC
    mas_varvec_variable_set_element( v3, 0, NULL, 2, "Hih" );
#else
    mas_varset_vclass_variable_set_element( v3, 0, NULL, 2, "Hih" );
#endif
    fprintf( stderr, "\t %s %d '%s'\t- %s\n", __func__, __LINE__, ( char * ) v3->iov_base, v3
             && 0 == strcmp( v3->iov_base, "Hih" ) ? ok_message : error_message );


    fprintf( stderr, "\t %s %d\t- %s\n", __func__, __LINE__, ( vc2->el_cnt == 3 ) ? ok_message : error_message );
    fprintf( stderr, "\t %s %d\t- %s\n", __func__, __LINE__, !( vc2->vec[0].iov_base ) ? ok_message : error_message );
    fprintf( stderr, "\t %s %d\t- %s\n", __func__, __LINE__, !( vc2->vec[0].iov_len ) ? ok_message : error_message );

    fprintf( stderr, "\t %s %d\t- %s\n", __func__, __LINE__, ( vc2->vec[1].iov_base ) ? ok_message : error_message );
    fprintf( stderr, "\t %s %d Wow\t- %s\n", __func__, __LINE__, 0 == strcmp( vc2->vec[1].iov_base, "Wow" ) ? ok_message : error_message );
    fprintf( stderr, "\t %s %d\t- %s\n", __func__, __LINE__, ( vc2->vec[1].iov_len == 3 ) ? ok_message : error_message );

    fprintf( stderr, "\t %s %d\t- %s\n", __func__, __LINE__, ( vc2->vec[2].iov_base ) ? ok_message : error_message );
    fprintf( stderr, "\t %s %d Hoh - '%s'\t- %s\n", __func__, __LINE__, ( char * ) vc2->vec[2].iov_base,
             0 == strcmp( vc2->vec[2].iov_base, "Hoh" ) ? ok_message : error_message );
    fprintf( stderr, "\t %s %d [%lu]\t- %s\n", __func__, __LINE__, vc2->vec[2].iov_len,
             ( vc2->vec[2].iov_len == 3 + 2 ) ? ok_message : error_message );

    fprintf( stderr, "\t %s %d\t- %s\n", __func__, __LINE__, &vc2->vec[1] == v1 ? ok_message : error_message );
    fprintf( stderr, "\t %s %d\t- %s\n", __func__, __LINE__, &vc2->vec[2] == v2 ? ok_message : error_message );
    fprintf( stderr, "\t %s %d\t- %s\n", __func__, __LINE__, 1 ? ok_message : error_message );

  }

  fprintf( stderr, "\t %s %d\t- %s\n", __func__, __LINE__, 1 ? ok_message : error_message );
  mas_varset_delete( vs1 );
  fprintf( stderr, "\t %s %d\t- %s\n", __func__, __LINE__, 1 ? ok_message : error_message );
  vs1 = NULL;
  fprintf( stderr, "\t %s %d\t- %s\n", __func__, __LINE__, 1 ? ok_message : error_message );
  mas_varset_delete( vs2 );
  fprintf( stderr, "\t %s %d\t- %s\n", __func__, __LINE__, 1 ? ok_message : error_message );
  vs2 = NULL;
  fprintf( stderr, "\t-%s %d\t- %s\n", __func__, __LINE__, 1 ? ok_message : error_message );
}

void
test32( void )
{
  mas_varset_t *vs1 __attribute__ ( ( unused ) ) = NULL;
  mas_varset_t *vs2 __attribute__ ( ( unused ) ) = NULL;

#ifdef MAS_USE_VARVEC
  mas_varvec_t *vc1 __attribute__ ( ( unused ) ) = NULL;
  mas_varvec_t *vc2 __attribute__ ( ( unused ) ) = NULL;
#else
  mas_varset_class_t *vc1 __attribute__ ( ( unused ) ) = NULL;
  mas_varset_class_t *vc2 __attribute__ ( ( unused ) ) = NULL;
#endif

  fprintf( stderr, "\t+%s %d\t- %s\n", __func__, __LINE__, 1 ? ok_message : error_message );
  vs1 = mas_varset_create(  );
  fprintf( stderr, "\t-%s %d\t- %s\n", __func__, __LINE__, vs1 ? ok_message : error_message );


  vc1 = mas_varset_search_vclass( vs1, "tclass1" );
  fprintf( stderr, "\t %s %d\t- %s\n", __func__, __LINE__, vc1 ? ok_message : error_message );
  vc2 = mas_varset_find_vclass( vs1, "tclass1" );
  fprintf( stderr, "\t %s %d\t- %s\n", __func__, __LINE__, vc2 && vc1 == vc2 ? ok_message : error_message );

  {
    mas_vclass_element_t *v1;
    mas_vclass_element_t *v2;
    mas_vclass_element_t *vx;

#ifdef MAS_USE_VARVEC
    v1 = _mas_varvec_add_elements( vc2, 2 );
    fprintf( stderr, "\t %s %d\t- %s\n", __func__, __LINE__, v1 ? ok_message : error_message );
    mas_varvec_variable_set_element( v1, 0, NULL, 0, "Wow" );
#else
    v1 = _mas_varset_vclass_add_elements( vc2, 2 );
    fprintf( stderr, "\t %s %d\t- %s\n", __func__, __LINE__, v1 ? ok_message : error_message );
    mas_varset_vclass_variable_set_element( v1, 0, NULL, 0, "Wow" );
#endif
    fprintf( stderr, "\t %s %d '%s'\t- %s\n", __func__, __LINE__, ( char * ) v1->iov_base, v1
             && 0 == strcmp( v1->iov_base, "Wow" ) ? ok_message : error_message );

#ifdef MAS_USE_VARVEC
    mas_varvec_variable_set_element( v1, 0, ": ", 2, "Wow" );
#else
    mas_varset_vclass_variable_set_element( v1, 0, ": ", 2, "Wow" );
#endif
    fprintf( stderr, "\t %s %d '%s'\t- %s\n", __func__, __LINE__, ( char * ) v1->iov_base, v1
             && 0 == strcmp( v1->iov_base, "Wow: " ) ? ok_message : error_message );

#ifdef MAS_USE_VARVEC
    v2 = _mas_varvec_add_elements( vc2, 2 );
#else
    v2 = _mas_varset_vclass_add_elements( vc2, 2 );
#endif
    fprintf( stderr, "\t %s %d\t- %s\n", __func__, __LINE__, v2 ? ok_message : error_message );
    fprintf( stderr, "\t %s %d\t- %s\n", __func__, __LINE__, v1 && v2 && v2 != v1 ? ok_message : error_message );
#ifdef MAS_USE_VARVEC
    mas_varvec_variable_set_element( v2, 0, ": ", 2, "Hoh" );
#else
    mas_varset_vclass_variable_set_element( v2, 0, ": ", 2, "Hoh" );
#endif
    fprintf( stderr, "\t %s %d '%s'\t- %s\n", __func__, __LINE__, ( char * ) v2->iov_base, v2
             && 0 == strcmp( v2->iov_base, "Hoh: " ) ? ok_message : error_message );

#ifdef MAS_USE_VARVEC
    vx = mas_varvec_find_variable( vc1, "Wow" );
#else
    vx = mas_varset_vclass_find_variable( vc1, "Wow" );
#endif
    fprintf( stderr, "\t %s %d\t- %s\n", __func__, __LINE__, vx ? ok_message : error_message );
  }

  fprintf( stderr, "\t %s %d\t- %s\n", __func__, __LINE__, 1 ? ok_message : error_message );
  mas_varset_delete( vs1 );
  fprintf( stderr, "\t %s %d\t- %s\n", __func__, __LINE__, 1 ? ok_message : error_message );
  vs1 = NULL;
  fprintf( stderr, "\t %s %d\t- %s\n", __func__, __LINE__, 1 ? ok_message : error_message );
  mas_varset_delete( vs2 );
  fprintf( stderr, "\t %s %d\t- %s\n", __func__, __LINE__, 1 ? ok_message : error_message );
  vs2 = NULL;
  fprintf( stderr, "\t-%s %d\t- %s\n", __func__, __LINE__, 1 ? ok_message : error_message );
}

void
test40( void )
{
#ifdef MAS_USE_VARVEC
  mas_varvec_t *vc1 __attribute__ ( ( unused ) ) = NULL;
  mas_varvec_t *vc2 __attribute__ ( ( unused ) ) = NULL;
#else
  mas_varset_class_t *vc1 __attribute__ ( ( unused ) ) = NULL;
  mas_varset_class_t *vc2 __attribute__ ( ( unused ) ) = NULL;
#endif

  mas_varset_t *vs1 __attribute__ ( ( unused ) ) = NULL;

  fprintf( stderr, "\t+%s %d\t- %s\n", __func__, __LINE__, 1 ? ok_message : error_message );
  vs1 = mas_varset_create(  );

  vc1 = mas_varset_search_vclass( vs1, "tclass1" );
  fprintf( stderr, "\t %s %d\t- %s\n", __func__, __LINE__, vc1 ? ok_message : error_message );
  vc2 = mas_varset_find_vclass( vs1, "tclass1" );
  fprintf( stderr, "\t %s %d\t- %s\n", __func__, __LINE__, vc2 && vc1 == vc2 ? ok_message : error_message );

  if ( 1 )
  {
    mas_vclass_element_t *v1;

#ifdef MAS_USE_VARVEC
    v1 = __mas_varvec_search_variable( vc1, "var1" );
#else
    v1 = __mas_varset_vclass_search_variable( vc1, "var1" );
#endif
    fprintf( stderr, "\t %s %d\t- %s\n", __func__, __LINE__, v1 ? ok_message : error_message );
  }
  if ( 1 )
  {
    mas_vclass_element_t *v1;

#ifdef MAS_USE_VARVEC
    v1 = __mas_varvec_search_variable( vc1, "var1" );
#else
    v1 = __mas_varset_vclass_search_variable( vc1, "var1" );
#endif
    fprintf( stderr, "\t %s %d\t- %s\n", __func__, __LINE__, v1 ? ok_message : error_message );
  }
  fprintf( stderr, "\t %s %d\t- %s\n", __func__, __LINE__, 1 ? ok_message : error_message );
  mas_varset_delete( vs1 );
  vs1 = NULL;
  fprintf( stderr, "\t-%s %d\t- %s\n", __func__, __LINE__, 1 ? ok_message : error_message );
}

void
test50( void )
{
#ifdef MAS_USE_VARVEC
  mas_varvec_t *vc1 __attribute__ ( ( unused ) ) = NULL;
  mas_varvec_t *vc2 __attribute__ ( ( unused ) ) = NULL;
#else
  mas_varset_class_t *vc1 __attribute__ ( ( unused ) ) = NULL;
  mas_varset_class_t *vc2 __attribute__ ( ( unused ) ) = NULL;
#endif

  mas_varset_t *vs1 __attribute__ ( ( unused ) ) = NULL;

  fprintf( stderr, "\t+%s %d\t- %s\n", __func__, __LINE__, 1 ? ok_message : error_message );
  vs1 = mas_varset_create(  );

  vc1 = mas_varset_search_vclass( vs1, "tclass1" );
  fprintf( stderr, "\t %s %d\t- %s\n", __func__, __LINE__, vc1 ? ok_message : error_message );
  vc2 = mas_varset_find_vclass( vs1, "tclass1" );
  fprintf( stderr, "\t %s %d\t- %s\n", __func__, __LINE__, vc2 && vc1 == vc2 ? ok_message : error_message );

  {
    mas_vclass_element_t *v1, *v2, *v3, *v4;

#ifdef MAS_USE_VARVEC
    v1 = __mas_varvec_search_variable( vc1, "var1" );
    fprintf( stderr, "\t %s %d\t- %s\n", __func__, __LINE__, v1 ? ok_message : error_message );
    v2 = mas_varvec_find_variable( vc1, "var1" );
    fprintf( stderr, "\t %s %d\t- %s\n", __func__, __LINE__, v2 && v2 == v1 ? ok_message : error_message );
    v3 = mas_varvec_find_variable( vc1, "var2" );
    fprintf( stderr, "\t %s %d\t- %s\n", __func__, __LINE__, !v3 ? ok_message : error_message );
    v4 = mas_varvec_find_variable( vc1, "var1" );
#else
    v1 = __mas_varset_vclass_search_variable( vc1, "var1" );
    fprintf( stderr, "\t %s %d\t- %s\n", __func__, __LINE__, v1 ? ok_message : error_message );
    v2 = mas_varset_vclass_find_variable( vc1, "var1" );
    fprintf( stderr, "\t %s %d\t- %s\n", __func__, __LINE__, v2 && v2 == v1 ? ok_message : error_message );
    v3 = mas_varset_vclass_find_variable( vc1, "var2" );
    fprintf( stderr, "\t %s %d\t- %s\n", __func__, __LINE__, !v3 ? ok_message : error_message );
    v4 = mas_varset_vclass_find_variable( vc1, "var1" );
#endif
    fprintf( stderr, "\t %s %d\t- %s\n", __func__, __LINE__, v4 && v4 == v1 ? ok_message : error_message );
    fprintf( stderr, "\t %s %d [%s]\t- %s\n", __func__, __LINE__, ( char * ) v4->iov_base, v4
             && 0 == strcmp( v4->iov_base, "var1: " ) ? ok_message : error_message );
  }
  fprintf( stderr, "\t %s %d\t- %s\n", __func__, __LINE__, 1 ? ok_message : error_message );
  mas_varset_delete( vs1 );
  vs1 = NULL;
  fprintf( stderr, "\t-%s %d\t- %s\n", __func__, __LINE__, 1 ? ok_message : error_message );
}

void
test60( void )
{
  mas_varset_t *vs1 __attribute__ ( ( unused ) ) = NULL;
  mas_varset_t *vs2 __attribute__ ( ( unused ) ) = NULL;
  mas_varset_t *vs3 __attribute__ ( ( unused ) ) = NULL;
  mas_varset_t *vs4 __attribute__ ( ( unused ) ) = NULL;
  mas_varset_t *vs5 __attribute__ ( ( unused ) ) = NULL;
  mas_varset_t *vs6 __attribute__ ( ( unused ) ) = NULL;

#ifdef MAS_USE_VARVEC
  mas_varvec_t *vc1 = NULL;
#else
  mas_varset_class_t *vc1 = NULL;
#endif


  fprintf( stderr, "\t+%s %d\t- %s\n", __func__, __LINE__, 1 ? ok_message : error_message );
  vs1 = mas_varset_create(  );

  vs1 = mas_varset_set_head( vs1, "tclass1", "HTTP 7.0" );


  vc1 = mas_varset_search_vclass( vs1, "tclass1" );

  fprintf( stderr, "\t %s %d\t- %s\n", __func__, __LINE__, vs1 ? ok_message : error_message );
  vs2 = mas_varset_search_variable( vs1, "tclass1", "tc1-varname", "tc1-varvalue" );
  fprintf( stderr, "\t %s %d [%u]\t- %s\n", __func__, __LINE__, vc1->el_cnt, ( vc1->el_cnt == 3 ) ? ok_message : error_message );
  fprintf( stderr, "\t %s %d\t- %s\n", __func__, __LINE__, vs2 ? ok_message : error_message );
  fprintf( stderr, "\t %s %d\t- %s\n", __func__, __LINE__, vs1 == vs2 ? ok_message : error_message );

  vs3 = mas_varset_search_variable( vs2, "tclass1", "tc1-varname1", "varvalue1" );
  fprintf( stderr, "\t %s %d [%u]\t- %s\n", __func__, __LINE__, vc1->el_cnt, ( vc1->el_cnt == 5 ) ? ok_message : error_message );
  fprintf( stderr, "\t %s %d\t- %s\n", __func__, __LINE__, vs1 == vs3 ? ok_message : error_message );
  vs4 = mas_varset_search_variable( vs3, "tclass1", "tc1-Content-Length", "77777" );
  fprintf( stderr, "\t %s %d [%u]\t- %s\n", __func__, __LINE__, vc1->el_cnt, ( vc1->el_cnt == 7 ) ? ok_message : error_message );
  fprintf( stderr, "\t %s %d\t- %s\n", __func__, __LINE__, vs1 == vs4 ? ok_message : error_message );
  vs5 = mas_varset_search_variable( vs4, "tclass1", "tc1-Content-Type", "text/html" );
  fprintf( stderr, "\t %s %d [%u]\t- %s\n", __func__, __LINE__, vc1->el_cnt, ( vc1->el_cnt == 9 ) ? ok_message : error_message );
  fprintf( stderr, "\t %s %d\t- %s\n", __func__, __LINE__, vs1 == vs5 ? ok_message : error_message );

  vs6 = mas_varset_add_tail( vs5, "tclass1", "KHVOST" );
  fprintf( stderr, "\n### [" );
  mas_varset_write( STDERR_FILENO, vs5, "tclass1" );
  fprintf( stderr, "] ###\n" );

  fprintf( stderr, "\t %s %d\t- %s\n", __func__, __LINE__, 1 ? ok_message : error_message );
  mas_varset_delete( vs1 );
  fprintf( stderr, "\t %s %d\t- %s\n", __func__, __LINE__, 1 ? ok_message : error_message );
  vs1 = NULL;
  fprintf( stderr, "\t %s %d\t- %s\n", __func__, __LINE__, 1 ? ok_message : error_message );
  vs2 = NULL;
  fprintf( stderr, "\t-%s %d\t- %s\n", __func__, __LINE__, 1 ? ok_message : error_message );
}


int
main( void )
{
  /* mas_strdup( "Hohoho" ); */
  test00(  );
  test10(  );
  test20(  );
  test30(  );
  test31(  );
  test32(  );
  test40(  );
  test50(  );
  test60(  );
  /* test70(  ); */
#ifdef MAS_TRACEMEM
  print_memlist( stderr, FL );
#endif

  return 0;
}
