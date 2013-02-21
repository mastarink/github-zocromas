SUBDIRS= inc
pkglib_LTLIBRARIES = http.la

http_la_SOURCES         = mas_transaction_http.c mas_http_request.c mas_http_reply.c mas_http_utils.c mas_http_get.c
http_la_LDFLAGS      =    -module -avoid-version -no-undefined  @HTTP_DEPS_LIBS@ @AM_LDFLAGS@
