#!/bin/sh

xst -ifn system_xst.scr
if [ $? -ne 0 ]; then
	exit 1
fi

rm -rf xst
