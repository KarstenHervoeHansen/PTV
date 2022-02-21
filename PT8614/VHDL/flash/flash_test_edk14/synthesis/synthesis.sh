#!/bin/sh

xst -ifn flash_test14_xst.scr
if [ $? -ne 0 ]; then
	exit 1
fi

rm -rf xst
