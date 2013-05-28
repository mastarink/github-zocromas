#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include <search.h>


#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "mas_varset_types.h"
#include "mas_varset_vclass.h"
#include "mas_varset.h"

static void
w_action( const void *nodep, const VISIT which, const int depth )
{
  if ( which == endorder || which == leaf )
  {
    const char *name = NULL;

    if ( nodep )
    {
      mas_varset_class_t *vclass;
      unsigned long id;

      vclass = *( ( mas_varset_class_t ** ) nodep );
      id = mas_varset_vclass_id( vclass );
      name = mas_varset_vclass_name( vclass );
      /* fprintf( stderr, "nodep:%p; which:%lu; depth:%lu --- %s\n", nodep, ( unsigned long ) which, ( unsigned long ) depth, */
      /*          name ? name : "-" );                                                                                        */
      fprintf( stderr, "[%lu] %lu. %s\n", ( unsigned long ) depth, id, name ? name : "-" );
      if ( vclass->veccnt )
      {
        mas_var_t *vec;

        vec = vclass->vec;
        for ( int i = 0; i < vclass->veccnt; i++ )
        {
          int j;
          char *name;
          char *val;

          j = i * 2;
          name = mas_varset_vclass_variable_get_name( &vec[j] );
          val = mas_varset_vclass_variable_get_value( &vec[j] );
          fprintf( stderr, "\t\t\t-- %s='%s'\n", name, val );
          mas_free( val );
          mas_free( name );
        }
      }
    }
  }
}

int
main( void )
{
  mas_varset_t *vs = NULL;

  for ( int iter = 0; iter < 20; iter++ )
  {
    vs = mas_varset_create(  );
    vs = mas_varset_search_variable( vs, "tclass4", "varnameX", "varvalueX" );
    vs = mas_varset_search_variable( vs, "tclass1", "varname1", "varvalue1" );
    vs = mas_varset_search_variable( vs, "tclass1", "varname2", "varvalue2" );
    vs = mas_varset_search_variable( vs, "tclass4", "varname7", "varvalue7" );
    vs = mas_varset_search_variable( vs, "tclass4", "varname7a", "varvalue7" );

    mas_varset_walk_classes( vs, w_action );


    /* mas_varset_delete_vclass( vs, "tclass1" ); */
    /* mas_varset_delete_vclass( vs, "tclass2" ); */
    mas_varset_delete( vs );

    vs = NULL;
  }
#ifdef MAS_TRACEMEM
  print_memlist( stderr, FL );
#endif

  return 0;
}
