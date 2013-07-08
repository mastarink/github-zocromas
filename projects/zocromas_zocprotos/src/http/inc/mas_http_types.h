#ifndef MAS_HTTP_TYPES_H
#  define MAS_HTTP_TYPES_H

#  include <mastar/types/mas_varset_types.h>
#ifdef MAS_HTTP_USE_FILEINFO
#  include <mastar/types/mas_fileinfo_types.h>
#elif defined( MAS_HTTP_USE_AUTOOBJECT )
#  include <mastar/types/mas_autoobject_types.h>
#endif

#include <mastar/types/mas_evaluate_types.h>
/* #include <mastar/types/mas_varvec_types.h> */

typedef enum
{
  MAS_HTTP_METHOD_BAD = -1,
  MAS_HTTP_METHOD_NONE = 0,
  MAS_HTTP_METHOD_UNKNOWN,
  MAS_HTTP_METHOD_GET,
  MAS_HTTP_METHOD_HEAD,
  MAS_HTTP_METHOD_POST,
  MAS_HTTP_METHOD_PUT,
  MAS_HTTP_METHOD_OPTIONS,
} mas_http_method_t;

typedef enum
{
  MAS_HTTP_CODE_NONE,
  MAS_HTTP_CODE_CONTINUE = 100,
  MAS_HTTP_CODE_SWITCHING_PROTOCOLS = 101,
  MAS_HTTP_CODE_OK = 200,
  MAS_HTTP_CODE_CREATED = 201,
  MAS_HTTP_CODE_ACCEPTED = 202,
  MAS_HTTP_CODE_NO_CONTENT = 204,
  MAS_HTTP_CODE_RESET_CONTENT = 205,
  MAS_HTTP_CODE_PARTIAL_CONTENT = 206,
  MAS_HTTP_CODE_MULTIPLE_CHOICES = 300,
  MAS_HTTP_CODE_MOVED_PERMANENTLY = 301,
  MAS_HTTP_CODE_FOUND = 302,
  MAS_HTTP_CODE_SEE_OTHER = 303,
  MAS_HTTP_CODE_NOT_MODIFIED = 304,
  MAS_HTTP_CODE_USE_PROXY = 305,
  MAS_HTTP_CODE_TEMPORARY_REDIRECT = 307,
  MAS_HTTP_CODE_BAD_REQUEST = 400,
  MAS_HTTP_CODE_UNAUTHORIZED = 401,
  MAS_HTTP_CODE_PAYMENT_REQUIRED = 402,
  MAS_HTTP_CODE_FORBIDDEN = 403,
  MAS_HTTP_CODE_NOT_FOUND = 404,
  MAS_HTTP_CODE_METHOD_NOT_ALLOWED = 405,
  MAS_HTTP_CODE_NOT_ACCEPTABLE = 406,
  MAS_HTTP_CODE_PROXY_AUTHENTICATION_REQUIRED = 407,
  MAS_HTTP_CODE_REQUEST_TIMEOUT = 408,
  MAS_HTTP_CODE_CONFLICT = 409,
  MAS_HTTP_CODE_GONE = 410,
  MAS_HTTP_CODE_LENGTH_REQUIRED = 411,
  MAS_HTTP_CODE_PRECONDITION_FAILED = 412,
  MAS_HTTP_CODE_REQUEST_ENTITY_TOO_LARGE = 413,
  MAS_HTTP_CODE_REQUEST_URI_TOO_LONG = 414,
  MAS_HTTP_CODE_UNSUPPORTED_MEDIA_TYPE = 415,
  MAS_HTTP_CODE_REQUESTED_RANGE_NOT_SATISFIABLE = 416,
  MAS_HTTP_CODE_EXPECTATION_FAILED = 417,
  MAS_HTTP_CODE_INTERNAL_SERVER_ERROR = 500,
  MAS_HTTP_CODE_NOT_IMPLEMENTED = 501,
  MAS_HTTP_CODE_BAD_GATEWAY = 502,
  MAS_HTTP_CODE_SERVICE_UNAVAILABLE = 503,
  MAS_HTTP_CODE_GATEWAY_TIMEOUT = 504,
  MAS_HTTP_CODE_HTTP_VERSION_NOT_SUPPORTED = 505,
} mas_http_code_t;

struct mas_http_method_pair_s
{
  mas_http_method_t id;
  const char *name;
};
typedef struct mas_http_method_pair_s mas_http_method_pair_t;

struct mas_http_s
{
  mas_http_code_t status_code;
  mas_rcontrol_t *prcontrol;
  char *host;
  unsigned port;
  char *docroot;
  char *smethod;
  mas_http_method_t imethod;
  char *URI;
  char *protocol_name;
  char *boundary;
#ifdef MAS_HTTP_USE_FILEINFO
  mas_fileinfo_t *request_content;
  mas_fileinfo_t *reply_content;
#elif defined( MAS_HTTP_USE_AUTOOBJECT )
  mas_autoobject_t *request_content;
  mas_autoobject_t *reply_content;
#endif
  /* char *rest; */
  mas_varvec_t *indata;
  mas_varvec_t *outdata;
  char *sversion;
  float fversion;
  unsigned long written;
  unsigned connection_keep_alive:1;
  unsigned connection_close:1;
};
typedef struct mas_http_s mas_http_t;





#endif
