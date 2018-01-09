#include "qstd_defs.h"
#include <stdio.h>

#include <mastar/minierr/minierr.h>
#include <mastar/regerr/masregerr.h>
#include <mastar/exam/masexam.h>

#include <mastar/mysqlpfs/mysqlpfs_defs.h>
#include <mastar/mysqlpfs/mysqlpfs_query.h>

#include <mastar/mysqlpfs/mysqlpfs_base.h>

int
test_create_tables( void )
{
  char *creops[] _uUu_ = {
    "START TRANSACTION",
    "CREATE TABLE IF NOT EXISTS qstd_test_filesizes ("               /* */
            "size BIGINT NOT NULL PRIMARY KEY"                       /* */
            ", nsame INTEGER NOT NULL, INDEX nsame (nsame)"          /* */
            ", last_updated  DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP, INDEX last_updated (last_updated)" /* */
            ")",
    "CREATE TABLE IF NOT EXISTS qstd_test_filedatas ("               /* */
            "id INTEGER PRIMARY KEY AUTO_INCREMENT"                  /* */
            ", dev INTEGER NOT NULL"                                 /* */
            ", inode BIGINT NOT NULL"                                /* */
            ", nlink INTEGER, INDEX nlink (nlink)"                   /* */
            ", nlinkdb INTEGER, INDEX nlinkdb (nlinkdb)"             /* */
            ", last_updated  DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP, INDEX last_updated (last_updated)" /* */
            ", UNIQUE INDEX dev_inode (dev,inode) COMMENT 'this pair is unique'" /* */
            " )",
    "CREATE TABLE IF NOT EXISTS qstd_test_fileprops ("               /* */
            "id INTEGER PRIMARY KEY AUTO_INCREMENT"                  /* */
            ", data_id INTEGER, UNIQUE INDEX data (data_id), FOREIGN KEY (data_id) REFERENCES qstd_test_filedatas (id)" /* */
            ", detype ENUM('BLK','CHR','DIR','FIFO','LNK','REG','SOCK'), INDEX detype (detype)" /* */
            ", mode INTEGER"                                         /* */
            ", uid INTEGER, INDEX uid (uid)"                         /* */
            ", gid INTEGER, INDEX gid (gid)"                         /* */
            ", atim DATETIME, INDEX atim (atim)"                     /* */
            ", mtim DATETIME, INDEX mtim (mtim)"                     /* */
            ", ctim DATETIME, INDEX ctim (ctim)"                     /* */
            ", size BIGINT NOT NULL, INDEX size (size), FOREIGN KEY (size) REFERENCES qstd_test_filesizes (size)" /* */
            ", last_updated DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP, INDEX last_updated (last_updated)" /* */
            ", rdev INTEGER"                                         /* */
            ", blksize INTEGER"                                      /* */
            ", blocks INTEGER"                                       /* */
            ")",
    "CREATE TABLE IF NOT EXISTS qstd_test_parents ("                 /* */
            "id INTEGER PRIMARY KEY AUTO_INCREMENT"                  /* */
            ", dir_id INTEGER, UNIQUE INDEX dir (dir_id)"            /* */
            ", nchilds INTEGER NOT NULL, INDEX nchilds (nchilds)"    /* */
            ", last_updated  DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP, INDEX last_updated (last_updated)" /* */
            ")",
    "CREATE TABLE IF NOT EXISTS qstd_test_filenames ("               /* */
            "id INTEGER PRIMARY KEY AUTO_INCREMENT"                  /* */
            ", parent_id INTEGER NOT NULL, INDEX parent (parent_id), FOREIGN KEY (parent_id) REFERENCES qstd_test_parents (id)" /* */
            ", name VARCHAR(255) COMMENT 'NULL is root', INDEX name (name)" /* */
            ", last_updated  DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP, INDEX last_updated (last_updated)" /* */
            ", data_id INTEGER, INDEX data (data_id), FOREIGN KEY (data_id) REFERENCES qstd_test_filedatas (id)" /* */
            ", detype ENUM('BLK','CHR','DIR','FIFO','LNK','REG','SOCK'), INDEX detype (detype)" /* */
            ", UNIQUE INDEX parent_name (parent_id, name) COMMENT 'this pair is unique'" /* */
            ")",
    "CREATE  VIEW qstd_test_filefull AS "                            /* */
            " SELECT fn.name, fn.id AS name_id, fd.id AS data_id, fp.mtim AS mtim, fs.nsame AS nsamesize, fp.size AS size " /* */
            "   FROM qstd_test_filenames AS fn "                     /* */
            "   LEFT JOIN qstd_test_filedatas AS fd ON(fn.data_id=fd.id) " /* */
            "   JOIN qstd_test_fileprops AS fp ON(fp.data_id=fd.id) " /* */
            "   LEFT JOIN qstd_test_filesizes AS fs ON(fp.size=fs.size) " /* */
            " WHERE fp.detype='REG'",
    "COMMIT",
  };
  mysqlpfs_t *pfs = mysqlpfs_create_setup( "mysql.mastar.lan", "masdufnt", "i2xV9KrTA54HRpj4e", "masdufntdb", 3306, NULL );

  if ( pfs )
  {
    int r = 0;

    for ( size_t i = 0; i < sizeof( creops ) / sizeof( creops[0] ) && !r; i++ )
    {
      r = mas_mysqlpfs_query( pfs, creops[i] );
      INFO( "(%d) %s", r, creops[i] );
    }
    mysqlpfs_delete( pfs );
  }

  return 0;
}
