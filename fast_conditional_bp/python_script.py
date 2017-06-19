#! /usr/bin/python

import os

def stop_handler (event):
    if (isinstance(event, gdb.BreakpointEvent)):
        if event.breakpoint.location == "main.c:222" :
            gdb.execute("set agent on")
            gdb.execute("break 52 if i > 1000000")
            print("conditional bp installed")
    gdb.execute("continue")

def exit_handler (event):
    gdb.execute("quit")

gdb.execute("target remote localhost:1337")

gdb.events.stop.connect (stop_handler)
gdb.events.exited.connect (exit_handler)

gdb.execute("b main.c:222")

gdb.execute("continue")
