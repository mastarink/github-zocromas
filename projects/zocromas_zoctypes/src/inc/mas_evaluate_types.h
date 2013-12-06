#ifndef MAS_EVALUATE_TYPES_H
#  define MAS_EVALUATE_TYPES_H

#  include <sys/types.h>

#  include <mastar/types/mas_opts_types.h>

typedef enum mas_cmdtype_e
{
  MAS_CMD_PCHAR,
  MAS_CMD_CONST_PCHAR,
  MAS_CMD_FILE_LOAD,
  MAS_CMD_CONST_FNAME_LOAD,
} mas_cmdtype_t;

typedef struct mas_evaluated_s mas_evaluated_t;
struct mas_evaluated_s
{
  mas_cmdtype_t type;
  void *data;
};

typedef mas_evaluated_t *( *data_loader_t ) ( const mas_options_t * popts, const char *root, const char *tail, size_t size,
                                              size_t * ptruesize, ino_t * ptrueinode, time_t * ptruefiletime, const void *arg );


typedef enum mas_content_type_e
{
  MAS_CONTENT_BAD = -1,
  MAS_CONTENT_NONE = 0,
  MAS_CONTENT_HTML,
  MAS_CONTENT_TEXT,
  MAS_CONTENT_GIF,
  MAS_CONTENT_JPEG,
  MAS_CONTENT_FORM_DATA,
} mas_content_type_t;


struct content_type_details_s
{
  mas_content_type_t ctype;
  char *mtype;
  char *dtype;
};
typedef struct content_type_details_s content_type_details_t;



typedef struct content_type_ext_s content_type_ext_t;
struct content_type_ext_s
{
  mas_content_type_t ctype;
  char *extset;
};



#endif
