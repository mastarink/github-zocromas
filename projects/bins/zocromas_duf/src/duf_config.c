#include <assert.h>
#include <string.h>

/* #include <mastar/wrap/mas_memory.h>                                  (* mas_(malloc|free|strdup); etc. ♣ *) */

#include "duf_tracen_defs.h"                                         /* DUF_TRACE ♠ */
#include "duf_errorn_defs.h"                                         /* DUF_NOERROR; DUF_CLEAR_ERROR; DUF_E_(LOWER|UPPER); DUF_TEST_R ... ♠ */

#include "duf_start_end.h"                                           /* DUF_STARTR ; DUF_ENDR ♠ */
#include "duf_dodefs.h"                                              /* DOR ♠ */


#include "duf_config_ref.h"

/* ###################################################################### */
#include "duf_config.h"
/* ###################################################################### */

duf_config_t *duf_config = NULL;

duf_config_t *
duf_get_config( void )
{
  assert( duf_config );
  return duf_config;
}
