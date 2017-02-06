#! /usr/bin/python

import os

def stop_handler (event):
    if (isinstance(event, gdb.BreakpointEvent)):
        if event.breakpoint.location == "pbzip2.cpp:3287" :
            gdb.execute("ftrace blocksort.c:627")
            gdb.execute("actions")
            gdb.execute("tstart")
            print("Tracepoint installed")
            os.system("lttng create gdb")
            os.system("lttng enable-channel didier -u --subbuf-size 4M --num-subbuf 16")
            os.system("lttng enable-event -c didier -u gdb_trace:*")
            os.system("lttng start")
            gdb.execute("info b")
        elif event.breakpoint.location == "nonstop_loop.c:184":
            gdb.execute("tstop")
    gdb.execute("continue")

def exit_handler (event):
    gdb.execute("quit")

gdb.execute("set circular-trace-buffer on")
gdb.execute("target remote localhost:1337")

gdb.events.stop.connect (stop_handler)
gdb.events.exited.connect (exit_handler)

gdb.execute("b pbzip2.cpp:3287")

gdb.execute("continue")
