#!/bin/sh
echo " gmas ----------------------------------------"
git push gmas master
echo " gh ----------------------------------------"
git push gh master
echo " gitlab ----------------------------------------"
git push gitlab master
echo " bitbucket ----------------------------------------"
git push bitbucket master

# # remote: 
# # remote: ============================== WARNING ==============================
# # remote: 
# # remote: Gitorious.org is entering read-only mode on May 15th. If you still
# # remote: need push capabilities please migrate your repository to GitLab.com
# # remote: 
# # remote: ============================== WARNING ==============================
# # remote: 
# echo " gitorius-zoc ----------------------------------------"
# git push gitorius-zoc master
