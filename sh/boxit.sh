#!/bin/sh
if [[ -f "sh/setup.sh" ]] ; then
  . sh/setup.sh

  echo "cp -a $savedir/*`datem`* boxes/box.com.zocromas/"
  echo "cp -a $savedir/*`datem`* boxes/dropbox.com.zocromas/"
fi
