#ifndef MAS_DUF_H
#  define MAS_DUF_H





/* 20140412                                                                                            */
/* reset:                                                                                              */
/*    run  --db-name=test20140412  --drop-tables --create-tables  --add-path /home/mastar/a/down/      */
/* test:                                                                                               */
/*    sqlite3 $MSH_SHN_PRJTOP_DIR/duf_db/test20140412 .tables                      */
/*    sqlite3 $MSH_SHN_PRJTOP_DIR/duf_db/test20140412 'select * from dufs.paths'    */
/* fill:                                                                                               */
/*    run  --db-name=test20140412  --uni-scan /mnt/new_media/media/down/  -R  --fill -v --trace-fill=1 */


/**
 20140409: 
run --recursive  --update-path /mnt/old_home/mastar/.mas --max-items=40 --max-depth=3 --min-size=10000000 -vvv

run  --uni-scan /home/mastar/a/down/ -R --max-depth=4  --tree --files
run  --uni-scan /home/mastar/a/down/ --max-depth=4  --max-items=70 --files --tree
run  --uni-scan /home/mastar/a/down/ --max-depth=4  --max-items=70 -R --tree


 **/


/* error codes */
/* - /usr/include/errno.h                  */
/* + /usr/include/bits/errno.h             */
/*   /usr/include/features.h               */
/* - /usr/include/linux/errno.h            */
/* - /usr/include/asm/errno.h              */
/* + /usr/include/asm-generic/errno.h      */
/* + /usr/include/asm-generic/errno-base.h */



/*
 * /mnt/new_media/media/Photo/
 * /mnt/new_media/media/Pictures.R.20120207.164339/
 * /mnt/miscn/video/Pictures/ /home/mastar/a/to-attach/
 * /home/mastar/a/zaurus/sd/Documents/image/
 * /home/mastar/.local/share/Trash/
 * /mnt/old_home/mastar/.mas/HTC_Legend/
 * /mnt/tall/htclegend/
 * /mnt/tall/atalk/backup/
 *
 * /mnt/new_media/media/down/
 * */



#endif
