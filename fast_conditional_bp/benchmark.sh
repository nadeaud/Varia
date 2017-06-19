#! /bin/bash

OUT=stats

ROOT="/home/didier/test/fast_conditional_bp/fast_bp"

GDBCMD="$ROOT/gdb --data-directory=$ROOT/data-directory"
SERVERCMD="$ROOT/gdbserver localhost:1337 ./lattp -i 10 -o 1000000"

export LD_PRELOAD=$ROOT/libinproctrace.so

for i in {1..20};
do
	$SERVERCMD &>/dev/null &
	/usr/bin/time -a -o $OUT -f "agent_on;$i;%e" $GDBCMD
done
