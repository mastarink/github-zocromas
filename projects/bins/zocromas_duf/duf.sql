BEGIN TRANSACTION;
CREATE TABLE files (pathindex NUMERIC, name TEXT, inode INTEGER PRIMARY KEY, mdindex INTEGER);
CREATE TABLE paths (name TEXT, inode INTEGER PRIMARY KEY, parentindex NUMERIC);
CREATE INDEX "filename" on files (name ASC);
COMMIT;
