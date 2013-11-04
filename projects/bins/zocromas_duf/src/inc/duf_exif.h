#ifndef MAS_DUF_EXIF_H
#  define MAS_DUF_EXIF_H

int duf_update_exif( unsigned long long pathid );

/* int copy_jpeg_by_date( void ); */


int duf_print_exif_same( unsigned long long dupcnt_min, unsigned long long limit );

#endif
