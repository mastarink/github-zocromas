#ifndef MAS_MOD_EXIF_H
#  define MAS_MOD_EXIF_H

/* file:///usr/share/doc/libexif-0.6.21-r1/libexif-api.html/index.html */

#if 0
#  include <libexif/exif-data.h>
#  include <libexif/exif-loader.h>
#  include <libexif/exif-byte-order.h>
#  include <libexif/exif-data-type.h>
#  include <libexif/exif-ifd.h>
#  include <libexif/exif-log.h>
#  include <libexif/exif-tag.h>
#  include <libexif/exif-content.h>
#  include <libexif/exif-mnote-data.h>
#  include <libexif/exif-mem.h>
#  include <libexif/exif-log.h>
#  include <libexif/exif-mem.h>
#else
/* 20151008.164803 -- using PKG_CHECK_MODULES(LIBEXIF, [libexif >= 0.6.21]) 
  in configire.ac - it sets include directory, so no need here */
#  include <exif-data.h>
#  include <exif-loader.h>
#  include <exif-byte-order.h>
#  include <exif-data-type.h>
#  include <exif-ifd.h>
#  include <exif-log.h>
#  include <exif-tag.h>
#  include <exif-content.h>
#  include <exif-mnote-data.h>
#  include <exif-mem.h>
#  include <exif-log.h>
#  include <exif-mem.h>
#endif

#endif
