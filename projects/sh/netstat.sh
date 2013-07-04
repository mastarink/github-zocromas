#!/bin/sh
netstat -4p $@
# | grep '.auxd'
