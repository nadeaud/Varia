#! /bin/bash

OUT=stats

ROOT="/home/didier/test/gdb_scripting/lttng"

GDBCMD="$ROOT/gdb --data-directory=$ROOT/data-directory"
SERVERCMD="$ROOT/gdbserver localhost:1337 $ROOT/pbzip2"
TESTFILE="$ROOT/test"
DEFAULTFILE="$ROOT/test2"
ARCHIVE="$ROOT/test.bz2"

export LD_PRELOAD=$ROOT/libinproctrace.so

for i in {1..1};
do
	for j in {1..1};
	do	
		cp $DEFAULTFILE $TESTFILE
		rm -f $ARCHIVE
		$SERVERCMD -1 -p$i $TESTFILE &
		/usr/bin/time -a -o $OUT -f "pbzip2;$i;%e" $GDBCMD
		lttng destroy
	done
done
